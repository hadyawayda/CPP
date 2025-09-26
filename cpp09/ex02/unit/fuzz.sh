#!/usr/bin/env bash
# Usage:
#   bash tests/fuzz.sh [-v] [-d] [N] [ITER]
#     -v     verbose per-run details
#     -d     allow duplicate numbers (default: no duplicates)
#     N      elements per run (fixed). If omitted, random N per run.
#     ITER   number of runs (default 100 or $ITER env)
#
# Env (only used when N omitted):
#   MIN_N=1 MAX_N=50 MAX_VAL=10000

set -euo pipefail

# ---- colors ----
RED=$'\033[31m'; GRN=$'\033[32m'; YEL=$'\033[33m'
BLU=$'\033[34m'; MAG=$'\033[35m'; CYN=$'\033[36m'; RST=$'\033[0m'
ORG=$'\033[38;5;208m'

# ---- args ----
VERBOSE=0
ALLOW_DUPLICATES=0
while getopts ":vd" opt; do
  case "$opt" in
    v) VERBOSE=1 ;;
    d) ALLOW_DUPLICATES=1 ;;
  esac
done
shift $((OPTIND-1))

FIXED_N="${1:-}"
ITER="${2:-${ITER:-100}}"
MIN_N="${MIN_N:-1}"
MAX_N="${MAX_N:-50}"
MAX_VAL="${MAX_VAL:-10000}"

cd "$(dirname "$0")/.."
BIN=./PmergeMe
[[ -x "$BIN" ]] || { printf "%b\n" "${RED}Binary not found at $BIN${RST}"; exit 1; }

rand32() { echo $(( ((RANDOM << 15) | RANDOM) & 0x3fffffff )); }

# Expected max Ford–Johnson comparisons ~ ceil(n*log2 n - 1.3n + 1)
expected_max() {
  local n="$1"
  if (( n <= 1 )); then echo 0; return; fi
  case "$n" in
    2) echo 3; return ;;
    3) echo 6; return ;;
    4) echo 8; return ;;
    5) echo 12; return ;;
    6) echo 16; return ;;
  esac
  awk -v n="$n" '
    function log2(x){ return log(x)/log(2) }
    BEGIN{
      thr = n*log2(n) - 1.3*n + 1.0;
      if (thr < 0) thr = 0;
      it = int(thr); if (thr > it) it++;
      print it;
    }'
}

# Get "After:" line (single line of numbers)
parse_after_line() {
  printf '%s\n' "$1" | sed -n 's/^After:[[:space:]]*//p' | tr -s ' ' ' ' | sed 's/^ //; s/ $//'
}

# Parse comparison counts from current main output
parse_comparisons() {
  # $1 = full output
  local out="$1"
  # Sed grabs the first integer after the label, portable BRE (no warnings)
  vec_cmp="$(printf '%s\n' "$out" | sed -n 's/^Comparisons (vector):[[:space:]]*\([0-9][0-9]*\).*/\1/p' | tail -n1)"
  deq_cmp="$(printf '%s\n' "$out" | sed -n 's/^Comparisons (deque):[[:space:]]*\([0-9][0-9]*\).*/\1/p'  | tail -n1)"
  vec_cmp=${vec_cmp:-0}
  deq_cmp=${deq_cmp:-0}

  # Backward-compat: older format "Ops (vector): ... binsearch_cmp=... sort_cmp=..."
  if [[ "$vec_cmp" -eq 0 ]]; then
    local old_bin old_sort
    old_bin="$( printf '%s\n' "$out" | sed -n 's/^Ops (vector):.*binsearch_cmp=\([0-9][0-9]*\).*/\1/p' )"
    old_sort="$(printf '%s\n' "$out" | sed -n 's/^Ops (vector):.*sort_cmp=\([0-9][0-9]*\).*/\1/p' )"
    if [[ -n "$old_bin$old_sort" ]]; then
      vec_cmp=$(( ${old_bin:-0} + ${old_sort:-0} ))
    fi
  fi
  if [[ "$deq_cmp" -eq 0 ]]; then
    local old_bin_d old_sort_d
    old_bin_d="$( printf '%s\n' "$out" | sed -n 's/^Ops (deque) :.*binsearch_cmp=\([0-9][0-9]*\).*/\1/p' )"
    old_sort_d="$(printf '%s\n' "$out" | sed -n 's/^Ops (deque) :.*sort_cmp=\([0-9][0-9]*\).*/\1/p' )"
    if [[ -n "$old_bin_d$old_sort_d" ]]; then
      deq_cmp=$(( ${old_bin_d:-0} + ${old_sort_d:-0} ))
    fi
  fi
}

# ---- stats ----
passes=0
sum_ops=0
over_cap=0
best_op=
worst_op=
best_line=""
worst_line=""

for ((i=1; i<=ITER; ++i)); do
  # N per run
  if [[ -n "$FIXED_N" ]]; then
    [[ "$FIXED_N" =~ ^[0-9]+$ && "$FIXED_N" -ge 1 ]] || { printf "%b\n" "${RED}Invalid N: $FIXED_N${RST}"; exit 1; }
    n="$FIXED_N"
  else
    n=$((RANDOM % (MAX_N - MIN_N + 1) + MIN_N))
  fi

  # Generate args in [1..MAX_VAL]
  args=()
  if [[ "$ALLOW_DUPLICATES" == "1" ]]; then
    # Allow duplicates: simple generation
    for ((k=0; k<n; ++k)); do
      val=$(( $(rand32) % MAX_VAL + 1 ))
      args+=("$val")
    done
  else
    # No duplicates: use efficient method based on ratio
    if [[ $n -gt $MAX_VAL ]]; then
      printf "%b\n" "${RED}Warning: Cannot generate $n unique numbers from range [1..$MAX_VAL].${RST}" >&2
      printf "%b\n" "${YEL}Consider using -d flag to allow duplicates or increase MAX_VAL.${RST}" >&2
      continue  # Skip this iteration
    fi
    
    # Ultra-fast approach: generate many candidates and use external tools
    # Generate more candidates than needed to ensure we get enough unique ones
    multiplier=3
    if [[ $n -lt 50 ]]; then multiplier=5; fi
    
    # Use a more efficient approach with external commands
    candidates_count=$((n * multiplier))
    if [[ $candidates_count -gt 1000 ]]; then candidates_count=1000; fi
    
    # Generate candidates using a single awk command for speed
    args=($(awk -v n="$candidates_count" -v max_val="$MAX_VAL" -v seed="$RANDOM" '
      BEGIN {
        srand(seed);
        for(i=0; i<n; i++) {
          print int(rand() * max_val) + 1;
        }
      }' | sort -n | uniq | head -n "$n"))
    
    # If we still don't have enough, use simple rejection sampling  
    if [[ ${#args[@]} -lt $n ]]; then
      # Fallback: simple rejection with timeout
      declare -A used_vals
      for val in "${args[@]}"; do used_vals[$val]=1; done
      
      attempts=0
      max_attempts=$((n * 20))  # Reasonable limit
      while [[ ${#args[@]} -lt $n && $attempts -lt $max_attempts ]]; do
        val=$(( $(rand32) % MAX_VAL + 1 ))
        if [[ -z "${used_vals[$val]:-}" ]]; then
          used_vals[$val]=1
          args+=("$val")
        fi
        ((attempts++))
      done
      
      # If still not enough, just fill with random (shouldn't happen with MAX_VAL=10000)
      while [[ ${#args[@]} -lt $n ]]; do
        args+=("$(( $(rand32) % MAX_VAL + 1 ))")
      done
    fi
  fi

  # echo "Run #$i: N=$n, args=(${args[*]})"

  out=""
  if ! out="$("$BIN" "${args[@]}" 2>&1)"; then
    status="${RED}[FAIL]${RST}"
    after=""
    vec_cmp=0
    deq_cmp=0
  else
    after="$(parse_after_line "$out")"
    expected_sorted="$(printf '%s\n' "${args[@]}" | grep -E '^[0-9]+$' | sort -n | tr '\n' ' ' | sed 's/ $//')"
    if [[ "$after" == "$expected_sorted" ]]; then status="${GRN}[OK]${RST}"; else status="${RED}[FAIL]${RST}"; fi
    parse_comparisons "$out"
  fi

  OP="$vec_cmp"            # score by vector comparisons
  cap="$(expected_max "$n")"
  over_flag=0; (( OP > cap )) && over_flag=1

  if [[ "$VERBOSE" == "1" ]]; then
    if (( over_flag )); then op_str="${ORG}${OP}${RST}"; tail="| ${ORG}over cap=${cap}${RST}"; else op_str="${YEL}${OP}${RST}"; tail="| cap=${cap}"; fi
    line="$(printf "%s N=%d OP=%b | vec cmp=%d | deq cmp=%d %s" \
      "$status" "$n" "$op_str" \
      "$vec_cmp" "$deq_cmp" \
      "$tail")"
  else
    if (( over_flag )); then op_str="${ORG}${OP}${RST}"; else op_str="${YEL}${OP}${RST}"; fi
    line="$(printf "%s N=%d OP=%b" "$status" "$n" "$op_str")"
  fi

  printf "%b\n" "$line"

  sum_ops=$((sum_ops + OP))
  (( over_flag )) && over_cap=$((over_cap + 1))
  if [[ -z "${best_op:-}" || OP -lt best_op ]]; then best_op=$OP; best_line="$line"; fi
  if [[ -z "${worst_op:-}" || OP -gt worst_op ]]; then worst_op=$OP; worst_line="$line"; fi
  [[ "$status" == *"[OK]"* ]] && passes=$((passes + 1))
done

avg_op=$(( sum_ops / ITER ))

# Average cap in summary (use fixed N if provided)
avg_cap="n/a"
if [[ -n "$FIXED_N" ]]; then
  avg_cap="$(expected_max "$FIXED_N")"
else
  total_cap=0
  for ((n=$MIN_N; n<=$MAX_N; ++n)); do total_cap=$(( total_cap + $(expected_max "$n") )); done
  avg_cap=$(( total_cap / (MAX_N - MIN_N + 1) ))
fi

printf "%b\n" "${BLU}========== Summary (${ITER} runs) ==========${RST}"
printf "%b\n" "${GRN}Best${RST}        : $best_line"
printf "%b\n" "${RED}Worst${RST}       : $worst_line"
printf "%b\n" "${MAG}Average OP${RST}  : ${CYN}${avg_op}${RST}"
if [[ "$avg_cap" != "n/a" ]]; then
  if [[ "$avg_op" -le "$avg_cap" ]]; then col="$GRN"; else col="$ORG"; fi
  printf "%b\n" "Expected avg cap : ${col}${avg_cap}${RST}"
fi
if (( over_cap > 0 )); then
  printf "%b\n" "${ORG}Over-cap cases${RST}: ${ORG}${over_cap}${RST}/${ITER}"
else
  printf "%b\n" "${GRN}Over-cap cases${RST}: 0/${ITER}"
fi
if [[ "$passes" -eq "$ITER" ]]; then
  printf "%b\n" "${GRN}[OK]${RST} ${passes}/${ITER} cases passed"
else
  printf "%b\n" "${RED}[FAIL]${RST} ${passes}/${ITER} cases passed"
  exit 1
fi
