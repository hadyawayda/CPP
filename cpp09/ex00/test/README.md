# BitcoinExchange Unit Testing

This directory contains comprehensive unit tests for the BitcoinExchange project.

## Files

- `test_BitcoinExchange.hpp` - Test class header with all test method declarations
- `test_BitcoinExchange.cpp` - Test implementation with comprehensive test cases
- `test_main.cpp` - Test runner main function
- `Makefile` - Build system for tests
- `run_tests.sh` - Comprehensive test script (unit tests + integration tests)
- `README.md` - This file

## Running Tests

### Option 1: Run Unit Tests Only
```bash
cd test/
make test
```

### Option 2: Run Comprehensive Test Suite
```bash
cd test/
./run_tests.sh
```

The comprehensive test suite includes:
- Unit tests
- Integration tests with various input files
- Memory leak detection (if valgrind is available)
- Performance tests with large datasets
- Edge case testing

## Test Categories

### Unit Tests
1. **Constructor Tests**
   - Valid CSV file loading
   - Invalid/missing CSV file handling
   - Empty CSV file handling

2. **Input File Processing Tests**
   - Valid input files
   - Invalid input files
   - Files with bad dates
   - Files with negative values
   - Files with large values (>1000)
   - Files with no matching dates
   - Exact date matches
   - Closest date matches

3. **Date Validation Tests**
   - Various invalid date formats
   - Leap year calculations
   - Days in month validation

4. **Edge Case Tests**
   - Comments in input files
   - Empty lines and whitespace
   - Missing values
   - Invalid separators
   - Non-numeric values

### Integration Tests
- Tests the complete program with various input scenarios
- Error handling verification
- Memory leak detection
- Performance testing with large datasets

## Test Output

Tests provide clear visual feedback:
- ✅ for passing tests
- ❌ for failing tests
- Detailed error messages when tests fail
- Summary of total tests run vs passed

## Building

The test Makefile will:
1. Compile the test files
2. Link with the main BitcoinExchange.cpp (excluding main.cpp)
3. Create a test executable

## Requirements

- C++98 compiler
- Make
- Optional: valgrind for memory leak detection

## Example Output

```
=== Running BitcoinExchange Unit Tests ===

--- Testing Constructor with Valid CSV ---
  ✅ Create test CSV file
  ✅ Constructor with valid CSV

--- Testing Constructor with Invalid CSV ---
  ✅ Correct error message for missing file
  ✅ Constructor throws exception for missing file

...

=== Test Summary ===
Total tests: 45
Passed: 45
Failed: 0
All tests PASSED! ✅
```
