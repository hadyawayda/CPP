// ex00/BitcoinExchange.hpp
#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange {
public:
    // Loads "data.csv" (provided with the subject) and prepares lookup. :contentReference[oaicite:1]{index=1}
    BitcoinExchange(const std::string& csvPath);

    // Process an input file whose lines are: "YYYY-MM-DD | value"
    void processInputFile(const std::string& path) const;

private:
    std::map<std::string, double> _rates; // date -> price

    static std::string trim(const std::string& s);
    static bool parseCSVLine(const std::string& line, std::string& date, double& price);
    static bool parseInputLine(const std::string& line, std::string& date, std::string& valueStr, double& value, std::string& errMsg);
    static bool isValidDate(const std::string& date);
    static bool isLeap(int y);
    static int daysInMonth(int y, int m);
    bool getRateForDateOrLower(const std::string& date, double& rate) const;
};

#endif
