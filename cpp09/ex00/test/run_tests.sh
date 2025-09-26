#!/bin/bash

# Comprehensive test script for BitcoinExchange
# This script tests both the unit tests and integration tests

echo "========================================="
echo "BitcoinExchange Comprehensive Test Suite"
echo "========================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counter
TOTAL_TESTS=0
PASSED_TESTS=0

# Function to run a test and check result
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_exit_code="${3:-0}"
    
    echo -e "\n${YELLOW}Testing: $test_name${NC}"
    echo "Command: $command"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if eval "$command"; then
        if [ $? -eq $expected_exit_code ]; then
            echo -e "${GREEN}✅ PASSED${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}❌ FAILED - Unexpected exit code${NC}"
        fi
    else
        if [ $? -eq $expected_exit_code ]; then
            echo -e "${GREEN}✅ PASSED${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}❌ FAILED - Unexpected exit code${NC}"
        fi
    fi
}

# Change to the project directory
PROJECT_DIR="$(dirname "$0")/.."
cd "$PROJECT_DIR"

echo -e "\n${YELLOW}=== BUILDING MAIN PROGRAM ===${NC}"
run_test "Build main program" "make clean && make"

echo -e "\n${YELLOW}=== BUILDING UNIT TESTS ===${NC}"
run_test "Build unit tests" "cd test && make clean && make"

echo -e "\n${YELLOW}=== RUNNING UNIT TESTS ===${NC}"
run_test "Run unit tests" "cd '$PROJECT_DIR/test' && ./test_btc"

echo -e "\n${YELLOW}=== CREATING TEST DATA FILES ===${NC}"

# Create a sample database file
cat > data.csv << 'EOF'
date,exchange_rate
2009-01-02,0
2009-01-09,0.00001
2011-01-03,0.3
2011-01-08,0.3
2011-01-09,0.32
2011-01-11,0.32
2012-01-11,7.4
2013-03-29,92.16
2014-04-08,484.21
2015-04-08,247.67
2017-01-01,997.69
2017-02-09,1138.26
2017-03-01,1309.59
2017-03-21,1086.69
2017-03-25,975.19
2017-04-01,1097.80
2017-05-09,1847.20
2017-11-08,7406.90
2017-11-20,8036.10
2017-12-01,10859.56
2017-12-17,19783.21
2017-12-18,18611.45
EOF

echo "✅ Created sample data.csv"

# Create various test input files
echo -e "\n${YELLOW}=== CREATING TEST INPUT FILES ===${NC}"

# Valid input file
cat > input_valid.txt << 'EOF'
date | value
2011-01-03 | 3
2011-01-09 | 2.2
2012-01-11 | 1
EOF
echo "✅ Created input_valid.txt"

# Input with errors
cat > input_errors.txt << 'EOF'
date | value
2011-01-03 | 3
2011-01-03 | -1
2011-01-03 | 1001
2010-01-01 | 1
bad_date | 1
2011-01-03 | not_a_number
2011-01-03
EOF
echo "✅ Created input_errors.txt"

# Input with edge cases
cat > input_edge.txt << 'EOF'
date | value
# This is a comment
2011-01-03 | 0
2011-01-03 | 1000

   
2011-01-03 | 0.01
2017-12-18 | 1
EOF
echo "✅ Created input_edge.txt"

# Input with boundary dates
cat > input_boundaries.txt << 'EOF'
date | value
2009-01-01 | 1
2009-01-02 | 1
2009-01-03 | 1
2020-01-01 | 1
EOF
echo "✅ Created input_boundaries.txt"

echo -e "\n${YELLOW}=== INTEGRATION TESTS ===${NC}"

# Test 1: Valid input file
run_test "Process valid input file" '../btc input_valid.txt'

# Test 2: Input with errors (should handle gracefully)
run_test "Process input with errors" '../btc input_errors.txt'

# Test 3: Input with edge cases
run_test "Process input with edge cases" '../btc input_edge.txt'

# Test 4: Input with boundary dates
run_test "Process input with boundary dates" '../btc input_boundaries.txt'

# Test 5: No arguments
run_test "No arguments provided" '../btc' 0

# Test 6: Nonexistent file
run_test "Nonexistent input file" '../btc nonexistent.txt' 0

# Test 7: Too many arguments
run_test "Too many arguments" '../btc input_valid.txt extra_arg' 0

# Test 8: Nonexistent database
run_test "Nonexistent database" 'mv data.csv data_backup.csv 2>/dev/null; ../btc input_valid.txt; EXIT_CODE=$?; mv data_backup.csv data.csv 2>/dev/null; exit $EXIT_CODE' 1

echo -e "\n${YELLOW}=== MEMORY LEAK TESTS ===${NC}"
if command -v valgrind > /dev/null 2>&1; then
    # Copy data.csv to current directory for valgrind test
    run_test "Memory leak test" 'cp ../data.csv . && valgrind --leak-check=full --error-exitcode=1 ../btc input_valid.txt >/dev/null 2>&1 && rm -f data.csv'
else
    echo "⚠️  Valgrind not available, skipping memory leak tests"
fi

echo -e "\n${YELLOW}=== PERFORMANCE TESTS ===${NC}"

# Create a large input file for performance testing
echo "Creating large input file for performance testing..."
cat > input_large.txt << 'EOF'
date | value
EOF

# Add 1000 entries
for i in {1..1000}; do
    echo "2017-01-01 | 1.0" >> input_large.txt
done

echo "✅ Created input_large.txt with 1000 entries"

# Measure time for large input
run_test "Process large input file (1000 entries)" '../btc input_large.txt > /dev/null 2>&1'

echo -e "\n${YELLOW}=== CLEANUP ===${NC}"
rm -f input_valid.txt input_errors.txt input_edge.txt input_boundaries.txt input_large.txt
echo "✅ Cleaned up test files"

echo -e "\n========================================="
echo -e "${YELLOW}TEST SUMMARY${NC}"
echo "========================================="
echo -e "Total tests run: ${TOTAL_TESTS}"
echo -e "Tests passed: ${GREEN}${PASSED_TESTS}${NC}"
echo -e "Tests failed: ${RED}$((TOTAL_TESTS - PASSED_TESTS))${NC}"

if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}"
    exit 0
else
    echo -e "\n${RED}❌ SOME TESTS FAILED${NC}"
    exit 1
fi
