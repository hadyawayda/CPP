#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

# Color helpers
ok()   { printf "\033[32m[OK]\033[0m %s\n" "$*"; }
fail() { printf "\033[31m[FAIL]\033[0m %s\n" "$*"; exit 1; }

BIN=./PmergeMe
[[ -x "$BIN" ]] || fail "Build first: make"

check_case () {
  local args="$1"
  local out
  out="$($BIN $args 2>/dev/null)" || fail "Program exited non-zero for: $args"
  local after expected
  after="$(printf "%s\n" "$out" | sed -n 's/^After: //p' | tr -s ' ' ' ' | sed 's/^ //; s/ $//')"
  expected="$(printf "%s\n" "$args" | tr ' ' '\n' | grep -E '^[0-9]+$' | sort -n | tr '\n' ' ' | sed 's/ $//')"
  [[ "$after" == "$expected" ]] || {
    printf "After:    %s\n" "$after"
    printf "Expected: %s\n" "$expected"
    fail "Mismatch for: $args"
  }
  ok "$args => $after"
}

echo "== Samples =="
check_case "3 5 9 7 4"
check_case "8 9 1 2 3 4 5 6 7"
check_case "42"
check_case "10 9 8 7 6 5 4 3 2 1"
check_case "1 1 1 1 1 1"   # duplicates (allowed by this implementation)
check_case "100 1 1000 50 999 2 3 4 10 11 12 13 14 15"
echo
echo "All sample tests passed."
