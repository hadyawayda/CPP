#ifndef TEST_BITCOINEXCHANGE_HPP
# define TEST_BITCOINEXCHANGE_HPP

# include "BitcoinExchange.hpp"
# include <fstream>
# include <iostream>
# include <sstream>
# include <string>

class TestBitcoinExchange
{
  public:
	static void runAllTests();

  private:
	// Test helper functions
	static bool createTestFile(const std::string &filename,
		const std::string &content);
	static void cleanupTestFile(const std::string &filename);
	static bool fileExists(const std::string &filename);
	static std::string captureOutput(void (*testFunc)());

	// Test cases
	static void testConstructorValidCSV();
	static void testConstructorInvalidCSV();
	static void testConstructorEmptyCSV();
	static void testProcessInputFileValid();
	static void testProcessInputFileInvalid();
	static void testProcessInputFileBadDates();
	static void testProcessInputFileNegativeValues();
	static void testProcessInputFileLargeValues();
	static void testProcessInputFileNoMatchingDate();
	static void testProcessInputFileExactDateMatch();
	static void testProcessInputFileClosestDateMatch();
	static void testDateValidation();
	static void testLeapYearCalculation();
	static void testDaysInMonth();
	static void testEdgeCases();

	// Test utilities
	static int testCount;
	static int passedTests;
	static void testAssert(bool condition, const std::string &testName);
	static void assertEqual(const std::string &expected,
		const std::string &actual, const std::string &testName);
	static void printTestResult(const std::string &testName, bool passed);
};

#endif
