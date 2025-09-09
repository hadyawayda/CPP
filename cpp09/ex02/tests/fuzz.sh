#!/usr/bin/env bash
# Usage:
#   bash tests/fuzz.sh [-v] [N] [ITER]
#     -v     verbose: show detailed metrics
#     N      number of integers per run (fixed). If omitted, random N per run.
#     ITER   number of iterations (default 100 or $ITER)
#
# Env (used only if N omitted):
#   MIN_N=1 MAX_N=50 MAX_VAL=10000
#
# Example:
#   bash tests/fuzz.sh -v 21 200
#   bash tests/fuzz.sh 40 500
#   bash tests/fuzz.sh -v

set -euo pipefail

# ---- colors ----
RED=$'\033[31m'; GRN=$'\033[32m'; YEL=$'\033[33m'
BLU=$'\033[34m'; MAG=$'\033[35m'; CYN=$'\033[36m'; RST=$'\033[0m'
# "Orange" (256-color). Fallback to yellow if terminal doesn’t support it.
ORG=$'\033[38;5;208m'

# ---- args ----
VERBOSE=0
while getopts ":v" opt; do
  case "$opt" in
    v) VERBOSE=1 ;;
  esac
done
shift $((OPTIND-1))

FIXED_N="${1:-}"                          # $1 => number of elements
ITER="${2:-${ITER:-100}}"                 # $2 => iterations
MIN_N="${MIN_N:-1}"
MAX_N="${MAX_N:-50}"
MAX_VAL="${MAX_VAL:-10000}"

cd "$(dirname "$0")/.."
BIN=./PmergeMe
[[ -x "$BIN" ]] || { printf "%b\n" "${RED}Binary not found at $BIN${RST}"; exit 1; }

# ---- helpers ----
rand32() { echo $(( ((RANDOM << 15) | RANDOM) & 0x3fffffff )); }

# Expected maximum operations curve for Ford–Johnson (merge-insertion).
# Model: ceil(n*log2(n) - 1.3*n + 1), tuned so that n=21 -> ~66.
# Small-N hand caps to avoid underestimation.
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
      thr = n*log2(n) - 1.3*n + 1.0;    # main curve
      if (thr < 0) thr = 0;
      # ceil
      it = int(thr);
      if (thr > it) it++;
      print it;
    }'
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

  # Generate args: strictly positive ints 1..MAX_VAL
  args=()
  for ((k=0; k<n; ++k)); do
    val=$(( $(rand32) % MAX_VAL + 1 ))
    args+=("$val")
  done

  # Run program; capture stdout+stderr
  out=""
  if ! out="$("$BIN" "${args[@]}" 2>&1)"; then
    status="${RED}[FAIL]${RST}"
    after=""
  else
    after="$(printf '%s\n' "$out" | sed -n 's/^After:[[:space:]]*//p' | tr -s ' ' ' ' | sed 's/^ //; s/ $//')"
    expected_sorted="$(printf '%s\n' "${args[@]}" | grep -E '^[0-9]+$' | sort -n | tr '\n' ' ' | sed 's/ $//')"
    if [[ "$after" == "$expected_sorted" ]]; then
      status="${GRN}[OK]${RST}"
    else
      status="${RED}[FAIL]${RST}"
    fi
  fi

  # Parse metrics
  vec_pairs="$(  printf '%s\n' "$out" | sed -n 's/^Ops (vector):.*pairs_cmp=\([0-9]\+\).*/\1/p')"
  vec_sort="$(   printf '%s\n' "$out" | sed -n 's/^Ops (vector):.*sort_cmp=\([0-9]\+\).*/\1/p')"
  vec_bin="$(    printf '%s\n' "$out" | sed -n 's/^Ops (vector):.*binsearch_cmp=\([0-9]\+\).*/\1/p')"
  vec_ins="$(    printf '%s\n' "$out" | sed -n 's/^Ops (vector):.*inserts=\([0-9]\+\).*/\1/p')"
  vec_shifts="$( printf '%s\n' "$out" | sed -n 's/^Ops (vector):.*shifts=\([0-9]\+\).*/\1/p')"

  deq_pairs="$( printf '%s\n' "$out" | sed -n 's/^Ops (deque) :.*pairs_cmp=\([0-9]\+\).*/\1/p')"
  deq_sort="$(  printf '%s\n' "$out" | sed -n 's/^Ops (deque) :.*sort_cmp=\([0-9]\+\).*/\1/p')"
  deq_bin="$(   printf '%s\n' "$out" | sed -n 's/^Ops (deque) :.*binsearch_cmp=\([0-9]\+\).*/\1/p')"
  deq_ins="$(   printf '%s\n' "$out" | sed -n 's/^Ops (deque) :.*inserts=\([0-9]\+\).*/\1/p')"

  vec_pairs=${vec_pairs:-0}; vec_sort=${vec_sort:-0}; vec_bin=${vec_bin:-0}; vec_ins=${vec_ins:-0}; vec_shifts=${vec_shifts:-0}
  deq_pairs=${deq_pairs:-0}; deq_sort=${deq_sort:-0}; deq_bin=${deq_bin:-0}; deq_ins=${deq_ins:-0}

  # Our OP metric (vector total, as agreed)
  vec_total=$(( vec_pairs + vec_sort + vec_bin + vec_ins ))
  deq_total=$(( deq_pairs + deq_sort + deq_bin + deq_ins ))
  OP="$vec_total"

  cap="$(expected_max "$n")"
  over_flag=0
  if (( OP > cap )); then
    over_flag=1
  fi

  # Compose line for this run
  if [[ "$VERBOSE" == "1" ]]; then
    # Color OP orange if over cap
    if (( over_flag == 1 )); then
      op_str="${ORG}${OP}${RST}"
      tail="| ${ORG}over cap=${cap}${RST}"
    else
      op_str="${YEL}${OP}${RST}"
      tail="| cap=${cap}"
    fi
    line="$(printf "%s N=%d OP=%b | vec total=%d (pairs=%d, sort=%d, bin=%d, ins=%d, shifts=%d) | deq total=%d (pairs=%d, sort=%d, bin=%d, ins=%d) %s" \
      "$status" "$n" "$op_str" \
      "$vec_total" "$vec_pairs" "$vec_sort" "$vec_bin" "$vec_ins" "$vec_shifts" \
      "$deq_total" "$deq_pairs" "$deq_sort" "$deq_bin" "$deq_ins" \
      "$tail")"
  else
    # Non-verbose: just N and OP (OP orange if over cap)
    if (( over_flag == 1  )); then
      op_str="${ORG}${OP}${RST}"
    else
      op_str="${YEL}${OP}${RST}"
    fi
    line="$(printf "%s N=%d OP=%b" "$status" "$n" "$op_str")"
  fi

  printf "%b\n" "$line"

  # Update summary stats
  sum_ops=$((sum_ops + OP))
  (( over_flag == 1 )) && over_cap=$((over_cap + 1))
  if [[ -z "${best_op:-}" || OP -lt best_op ]]; then
    best_op=$OP; best_line="$line"
  fi
  if [[ -z "${worst_op:-}" || OP -gt worst_op ]]; then
    worst_op=$OP; worst_line="$line"
  fi
  [[ "$status" == *"[OK]"* ]] && passes=$((passes + 1))
done

avg_op=$(( sum_ops / ITER ))

# For summary expectations, use fixed N if provided; otherwise approximate with
# the mean cap across a uniform distribution in [MIN_N..MAX_N].
avg_cap="n/a"
if [[ -n "$FIXED_N" ]]; then
  avg_cap="$(expected_max "$FIXED_N")"
else
  # rough average cap across the range
  total_cap=0
  for ((n=$MIN_N; n<=$MAX_N; ++n)); do
    total_cap=$(( total_cap + $(expected_max "$n") ))
  done
  range=$(( MAX_N - MIN_N + 1 ))
  avg_cap=$(( total_cap / range ))
fi

# ---- Summary ----
printf "%b\n" "${BLU}========== Summary (${ITER} runs) ==========${RST}"
printf "%b\n" "${GRN}Best${RST}        : $best_line"
printf "%b\n" "${RED}Worst${RST}       : $worst_line"
printf "%b\n" "${MAG}Average OP${RST}  : ${CYN}${avg_op}${RST}"
if [[ "$avg_cap" != "n/a" ]]; then
  # color avg comparison
  if [[ "$avg_op" -le "$avg_cap" ]]; then
    avg_cmp_color="$GRN"
  else
    avg_cmp_color="$ORG"
  fi
  printf "%b\n" "Expected avg cap : ${avg_cmp_color}${avg_cap}${RST}"
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
