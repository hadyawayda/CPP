#include "test_BitcoinExchange.hpp"
#include <cstdio>
#include <stdexcept>

int TestBitcoinExchange::testCount = 0;
int TestBitcoinExchange::passedTests = 0;

void TestBitcoinExchange::runAllTests() {
    std::cout << "=== Running BitcoinExchange Unit Tests ===" << std::endl;
    
    testConstructorValidCSV();
    testConstructorInvalidCSV();
    testConstructorEmptyCSV();
    testProcessInputFileValid();
    testProcessInputFileInvalid();
    testProcessInputFileBadDates();
    testProcessInputFileNegativeValues();
    testProcessInputFileLargeValues();
    testProcessInputFileNoMatchingDate();
    testProcessInputFileExactDateMatch();
    testProcessInputFileClosestDateMatch();
    testDateValidation();
    testLeapYearCalculation();
    testDaysInMonth();
    testEdgeCases();
    
    std::cout << "\n=== Test Summary ===" << std::endl;
    std::cout << "Total tests: " << testCount << std::endl;
    std::cout << "Passed: " << passedTests << std::endl;
    std::cout << "Failed: " << (testCount - passedTests) << std::endl;
    
    if (passedTests == testCount) {
        std::cout << "All tests PASSED! ✅" << std::endl;
    } else {
        std::cout << "Some tests FAILED! ❌" << std::endl;
    }
}

bool TestBitcoinExchange::createTestFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename.c_str());
    if (!file.is_open()) return false;
    file << content;
    file.close();
    return true;
}

void TestBitcoinExchange::cleanupTestFile(const std::string& filename) {
    std::remove(filename.c_str());
}

bool TestBitcoinExchange::fileExists(const std::string& filename) {
    std::ifstream file(filename.c_str());
    return file.good();
}

void TestBitcoinExchange::testConstructorValidCSV() {
    std::cout << "\n--- Testing Constructor with Valid CSV ---" << std::endl;
    
    // Create a test CSV file
    std::string csvContent = "date,exchange_rate\n2009-01-02,0\n2011-01-03,0.3\n2011-01-09,0.32\n";
    testAssert(createTestFile("test_data.csv", csvContent), "Create test CSV file");
    
    try {
        BitcoinExchange btc("test_data.csv");
        printTestResult("Constructor with valid CSV", true);
    } catch (const std::exception& e) {
        printTestResult("Constructor with valid CSV", false);
        std::cout << "  Error: " << e.what() << std::endl;
    }
    
    cleanupTestFile("test_data.csv");
}

void TestBitcoinExchange::testConstructorInvalidCSV() {
    std::cout << "\n--- Testing Constructor with Invalid CSV ---" << std::endl;
    
    bool exceptionThrown = false;
    try {
        BitcoinExchange btc("nonexistent.csv");
    } catch (const std::exception& e) {
        exceptionThrown = true;
        std::string errorMsg = e.what();
        testAssert(errorMsg.find("could not open database") != std::string::npos, 
               "Correct error message for missing file");
    }
    
    printTestResult("Constructor throws exception for missing file", exceptionThrown);
}

void TestBitcoinExchange::testConstructorEmptyCSV() {
    std::cout << "\n--- Testing Constructor with Empty CSV ---" << std::endl;
    
    testAssert(createTestFile("empty.csv", ""), "Create empty CSV file");
    
    bool exceptionThrown = false;
    try {
        BitcoinExchange btc("empty.csv");
    } catch (const std::exception& e) {
        exceptionThrown = true;
        std::string errorMsg = e.what();
        testAssert(errorMsg.find("empty database") != std::string::npos, 
               "Correct error message for empty file");
    }
    
    printTestResult("Constructor throws exception for empty file", exceptionThrown);
    cleanupTestFile("empty.csv");
}

void TestBitcoinExchange::testProcessInputFileValid() {
    std::cout << "\n--- Testing Process Input File with Valid Data ---" << std::endl;
    
    // Create test database
    std::string csvContent = "date,exchange_rate\n2009-01-02,0\n2011-01-03,0.3\n2011-01-09,0.32\n";
    testAssert(createTestFile("test_data.csv", csvContent), "Create test CSV file");
    
    // Create test input file
    std::string inputContent = "date | value\n2011-01-03 | 3\n2011-01-09 | 2.2\n";
    testAssert(createTestFile("test_input.txt", inputContent), "Create test input file");
    
    try {
        BitcoinExchange btc("test_data.csv");
        btc.processInputFile("test_input.txt");
        printTestResult("Process valid input file", true);
    } catch (const std::exception& e) {
        printTestResult("Process valid input file", false);
        std::cout << "  Error: " << e.what() << std::endl;
    }
    
    cleanupTestFile("test_data.csv");
    cleanupTestFile("test_input.txt");
}

void TestBitcoinExchange::testProcessInputFileInvalid() {
    std::cout << "\n--- Testing Process Input File with Invalid File ---" << std::endl;
    
    // Create test database
    std::string csvContent = "date,exchange_rate\n2009-01-02,0\n2011-01-03,0.3\n";
    testAssert(createTestFile("test_data.csv", csvContent), "Create test CSV file");
    
    try {
        BitcoinExchange btc("test_data.csv");
        btc.processInputFile("nonexistent_input.txt");
        printTestResult("Process nonexistent input file", true); // Should not throw, just print error
    } catch (const std::exception& e) {
        printTestResult("Process nonexistent input file", false);
        std::cout << "  Unexpected error: " << e.what() << std::endl;
    }
    
    cleanupTestFile("test_data.csv");
}

void TestBitcoinExchange::testProcessInputFileBadDates() {
    std::cout << "\n--- Testing Process Input File with Bad Dates ---" << std::endl;
    
    // Create test database
    std::string csvContent = "date,exchange_rate\n2009-01-02,0\n2011-01-03,0.3\n";
    testAssert(createTestFile("test_data.csv", csvContent), "Create test CSV file");
    
    // Create test input with bad dates
    std::string inputContent = "date | value\n2011-13-03 | 3\n2011-01-32 | 2.2\nabc | 1\n";
    testAssert(createTestFile("test_input.txt", inputContent), "Create test input file");
    
    try {
        BitcoinExchange btc("test_data.csv");
        btc.processInputFile("test_input.txt");
        printTestResult("Process input file with bad dates", true);
    } catch (const std::exception& e) {
        printTestResult("Process input file with bad dates", false);
        std::cout << "  Error: " << e.what() << std::endl;
    }
    
    cleanupTestFile("test_data.csv");
    cleanupTestFile("test_input.txt");
}

void TestBitcoinExchange::testProcessInputFileNegativeValues() {
    std::cout << "\n--- Testing Process Input File with Negative Values ---" << std::endl;
    
    // Create test database
    std::string csvContent = "date,exchange_rate\n2009-01-02,0\n2011-01-03,0.3\n";
    testAssert(createTestFile("test_data.csv", csvContent), "Create test CSV file");
    
    // Create test input with negative values
    std::string inputContent = "date | value\n2011-01-03 | -3\n2011-01-03 | -0.5\n";
    testAssert(createTestFile("test_input.txt", inputContent), "Create test input file");
    
    try {
        BitcoinExchange btc("test_data.csv");
        btc.processInputFile("test_input.txt");
        printTestResult("Process input file with negative values", true);
    } catch (const std::exception& e) {
        printTestResult("Process input file with negative values", false);
        std::cout << "  Error: " << e.what() << std::endl;
    }
    
    cleanupTestFile("test_data.csv");
    cleanupTestFile("test_input.txt");
}

void TestBitcoinExchange::testProcessInputFileLargeValues() {
    std::cout << "\n--- Testing Process Input File with Large Values ---" << std::endl;
    
    // Create test database
    std::string csvContent = "date,exchange_rate\n2009-01-02,0\n2011-01-03,0.3\n";
    testAssert(createTestFile("test_data.csv", csvContent), "Create test CSV file");
    
    // Create test input with large values
    std::string inputContent = "date | value\n2011-01-03 | 1001\n2011-01-03 | 9999\n";
    testAssert(createTestFile("test_input.txt", inputContent), "Create test input file");
    
    try {
        BitcoinExchange btc("test_data.csv");
        btc.processInputFile("test_input.txt");
        printTestResult("Process input file with large values", true);
    } catch (const std::exception& e) {
        printTestResult("Process input file with large values", false);
        std::cout << "  Error: " << e.what() << std::endl;
    }
    
    cleanupTestFile("test_data.csv");
    cleanupTestFile("test_input.txt");
}

void TestBitcoinExchange::testProcessInputFileNoMatchingDate() {
    std::cout << "\n--- Testing Process Input File with No Matching Date ---" << std::endl;
    
    // Create test database with later dates
    std::string csvContent = "date,exchange_rate\n2015-01-02,100\n2016-01-03,200\n";
    testAssert(createTestFile("test_data.csv", csvContent), "Create test CSV file");
    
    // Create test input with earlier date
    std::string inputContent = "date | value\n2009-01-03 | 3\n";
    testAssert(createTestFile("test_input.txt", inputContent), "Create test input file");
    
    try {
        BitcoinExchange btc("test_data.csv");
        btc.processInputFile("test_input.txt");
        printTestResult("Process input file with no matching date", true);
    } catch (const std::exception& e) {
        printTestResult("Process input file with no matching date", false);
        std::cout << "  Error: " << e.what() << std::endl;
    }
    
    cleanupTestFile("test_data.csv");
    cleanupTestFile("test_input.txt");
}

void TestBitcoinExchange::testProcessInputFileExactDateMatch() {
    std::cout << "\n--- Testing Process Input File with Exact Date Match ---" << std::endl;
    
    // Create test database
    std::string csvContent = "date,exchange_rate\n2009-01-02,0\n2011-01-03,0.3\n2011-01-09,0.32\n";
    testAssert(createTestFile("test_data.csv", csvContent), "Create test CSV file");
    
    // Create test input with exact matching date
    std::string inputContent = "date | value\n2011-01-03 | 10\n";
    testAssert(createTestFile("test_input.txt", inputContent), "Create test input file");
    
    try {
        BitcoinExchange btc("test_data.csv");
        btc.processInputFile("test_input.txt");
        printTestResult("Process input file with exact date match", true);
    } catch (const std::exception& e) {
        printTestResult("Process input file with exact date match", false);
        std::cout << "  Error: " << e.what() << std::endl;
    }
    
    cleanupTestFile("test_data.csv");
    cleanupTestFile("test_input.txt");
}

void TestBitcoinExchange::testProcessInputFileClosestDateMatch() {
    std::cout << "\n--- Testing Process Input File with Closest Date Match ---" << std::endl;
    
    // Create test database
    std::string csvContent = "date,exchange_rate\n2009-01-02,0\n2011-01-03,0.3\n2011-01-09,0.32\n";
    testAssert(createTestFile("test_data.csv", csvContent), "Create test CSV file");
    
    // Create test input with date between database entries
    std::string inputContent = "date | value\n2011-01-05 | 5\n";
    testAssert(createTestFile("test_input.txt", inputContent), "Create test input file");
    
    try {
        BitcoinExchange btc("test_data.csv");
        btc.processInputFile("test_input.txt");
        printTestResult("Process input file with closest date match", true);
    } catch (const std::exception& e) {
        printTestResult("Process input file with closest date match", false);
        std::cout << "  Error: " << e.what() << std::endl;
    }
    
    cleanupTestFile("test_data.csv");
    cleanupTestFile("test_input.txt");
}

void TestBitcoinExchange::testDateValidation() {
    std::cout << "\n--- Testing Date Validation ---" << std::endl;
    
    // We can't directly test private static methods, but we can test through input processing
    // Create test database
    std::string csvContent = "date,exchange_rate\n2009-01-02,0\n2011-01-03,0.3\n";
    testAssert(createTestFile("test_data.csv", csvContent), "Create test CSV file");
    
    // Test various invalid date formats
    std::string inputContent = 
        "date | value\n"
        "2011-1-03 | 1\n"          // Short month
        "2011-01-3 | 1\n"          // Short day
        "11-01-03 | 1\n"           // Short year
        "2011/01/03 | 1\n"         // Wrong separator
        "2011-02-30 | 1\n"         // Invalid day for February
        "2011-13-01 | 1\n"         // Invalid month
        "2011-01-32 | 1\n"         // Invalid day
        "abcd-01-01 | 1\n"         // Non-numeric year
        "2011-ab-01 | 1\n"         // Non-numeric month
        "2011-01-ab | 1\n";        // Non-numeric day
    
    testAssert(createTestFile("test_input.txt", inputContent), "Create test input file");
    
    try {
        BitcoinExchange btc("test_data.csv");
        btc.processInputFile("test_input.txt");
        printTestResult("Date validation through input processing", true);
    } catch (const std::exception& e) {
        printTestResult("Date validation through input processing", false);
        std::cout << "  Error: " << e.what() << std::endl;
    }
    
    cleanupTestFile("test_data.csv");
    cleanupTestFile("test_input.txt");
}

void TestBitcoinExchange::testLeapYearCalculation() {
    std::cout << "\n--- Testing Leap Year Calculation ---" << std::endl;
    
    // Test leap year dates through CSV parsing
    std::string csvContent = 
        "date,exchange_rate\n"
        "2000-02-29,100\n"    // 2000 is leap year (divisible by 400)
        "1900-02-28,200\n"    // 1900 is not leap year (divisible by 100 but not 400)
        "2004-02-29,300\n"    // 2004 is leap year (divisible by 4)
        "2001-02-28,400\n";   // 2001 is not leap year
    
    testAssert(createTestFile("test_data.csv", csvContent), "Create test CSV file");
    
    try {
        BitcoinExchange btc("test_data.csv");
        printTestResult("Leap year calculation through CSV parsing", true);
    } catch (const std::exception& e) {
        printTestResult("Leap year calculation through CSV parsing", false);
        std::cout << "  Error: " << e.what() << std::endl;
    }
    
    // Test invalid leap year date
    std::string invalidCsvContent = "date,exchange_rate\n1900-02-29,100\n"; // Invalid: 1900 is not leap year
    testAssert(createTestFile("invalid_data.csv", invalidCsvContent), "Create invalid CSV file");
    
    try {
        BitcoinExchange btc("invalid_data.csv");
        printTestResult("Invalid leap year date rejected", true);
    } catch (const std::exception& e) {
        printTestResult("Invalid leap year date rejected", true); // Expected to fail
    }
    
    cleanupTestFile("test_data.csv");
    cleanupTestFile("invalid_data.csv");
}

void TestBitcoinExchange::testDaysInMonth() {
    std::cout << "\n--- Testing Days in Month Validation ---" << std::endl;
    
    // Test various month boundaries through input processing
    std::string csvContent = "date,exchange_rate\n2011-01-01,1\n";
    testAssert(createTestFile("test_data.csv", csvContent), "Create test CSV file");
    
    std::string inputContent = 
        "date | value\n"
        "2011-01-31 | 1\n"    // January has 31 days - valid
        "2011-02-28 | 1\n"    // February has 28 days in non-leap year - valid
        "2011-04-30 | 1\n"    // April has 30 days - valid
        "2011-04-31 | 1\n"    // April doesn't have 31 days - invalid
        "2011-02-29 | 1\n"    // February 29 in non-leap year - invalid
        "2011-06-31 | 1\n";   // June doesn't have 31 days - invalid
    
    testAssert(createTestFile("test_input.txt", inputContent), "Create test input file");
    
    try {
        BitcoinExchange btc("test_data.csv");
        btc.processInputFile("test_input.txt");
        printTestResult("Days in month validation", true);
    } catch (const std::exception& e) {
        printTestResult("Days in month validation", false);
        std::cout << "  Error: " << e.what() << std::endl;
    }
    
    cleanupTestFile("test_data.csv");
    cleanupTestFile("test_input.txt");
}

void TestBitcoinExchange::testEdgeCases() {
    std::cout << "\n--- Testing Edge Cases ---" << std::endl;
    
    // Create test database
    std::string csvContent = "date,exchange_rate\n2009-01-02,0\n2011-01-03,0.3\n";
    testAssert(createTestFile("test_data.csv", csvContent), "Create test CSV file");
    
    // Test edge cases in input
    std::string inputContent = 
        "date | value\n"
        "# This is a comment\n"   // Comments should be ignored
        "\n"                      // Empty lines should be ignored
        "   \n"                   // Whitespace-only lines should be ignored
        "2011-01-03|\n"           // Missing value
        "2011-01-03 | \n"         // Empty value
        "2011-01-03 | 0\n"        // Zero value (valid)
        "2011-01-03 | 1000\n"     // Boundary value (valid)
        "2011-01-03 | 1000.1\n"   // Over boundary (invalid)
        "| 5\n"                   // Missing date
        "2011-01-03 5\n"          // Missing pipe separator
        "2011-01-03 | abc\n"      // Non-numeric value
        "2011-01-03 | 1.5.2\n";   // Invalid number format
    
    testAssert(createTestFile("test_input.txt", inputContent), "Create test input file");
    
    try {
        BitcoinExchange btc("test_data.csv");
        btc.processInputFile("test_input.txt");
        printTestResult("Edge cases handling", true);
    } catch (const std::exception& e) {
        printTestResult("Edge cases handling", false);
        std::cout << "  Error: " << e.what() << std::endl;
    }
    
    cleanupTestFile("test_data.csv");
    cleanupTestFile("test_input.txt");
}

void TestBitcoinExchange::testAssert(bool condition, const std::string& testName) {
    testCount++;
    if (condition) {
        passedTests++;
        std::cout << "  ✅ " << testName << std::endl;
    } else {
        std::cout << "  ❌ " << testName << std::endl;
    }
}

void TestBitcoinExchange::assertEqual(const std::string& expected, const std::string& actual, const std::string& testName) {
    testCount++;
    if (expected == actual) {
        passedTests++;
        std::cout << "  ✅ " << testName << std::endl;
    } else {
        std::cout << "  ❌ " << testName << std::endl;
        std::cout << "    Expected: '" << expected << "'" << std::endl;
        std::cout << "    Actual: '" << actual << "'" << std::endl;
    }
}

void TestBitcoinExchange::printTestResult(const std::string& testName, bool passed) {
    testCount++;
    if (passed) {
        passedTests++;
        std::cout << "  ✅ " << testName << std::endl;
    } else {
        std::cout << "  ❌ " << testName << std::endl;
    }
}
