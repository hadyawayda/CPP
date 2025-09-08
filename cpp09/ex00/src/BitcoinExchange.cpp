// ex00/BitcoinExchange.cpp
#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cctype>

BitcoinExchange::BitcoinExchange(const std::string& csvPath) {
    std::ifstream in(csvPath.c_str());
    if (!in.good()) {
        throw std::runtime_error("Error: could not open database.");
    }
    std::string line;
    // Skip optional header "date,exchange_rate"
    if (std::getline(in, line)) {
        if (line.find(',') == std::string::npos || line.substr(0, 4) != "date")
            in.seekg(0);
    } else {
        throw std::runtime_error("Error: empty database.");
    }

    do {
        std::string date;
        double price;
        if (line.size() && parseCSVLine(line, date, price)) {
            _rates[date] = price;
        }
    } while (std::getline(in, line));
}

void BitcoinExchange::processInputFile(const std::string& path) const {
    std::ifstream in(path.c_str());
    if (!in.good()) {
        std::cout << "Error: could not open file." << std::endl; // sample shows this exact message
        return;
    }
    std::string line;
    // Optional header "date | value"
    if (std::getline(in, line)) {
        std::string t = trim(line);
        if (t != "date | value")
            in.seekg(0);
    }

    while (std::getline(in, line)) {
        std::string date, valueStr, err;
        double value;
        if (!parseInputLine(line, date, valueStr, value, err)) {
            if (err.size())
                std::cout << err << std::endl;
            else
                std::cout << "Error: bad input => " << trim(line) << std::endl;
            continue;
        }
        if (value < 0.0) { std::cout << "Error: not a positive number." << std::endl; continue; }
        if (value > 1000.0) { std::cout << "Error: too large a number." << std::endl; continue; }

        double rate;
        if (!getRateForDateOrLower(date, rate)) {
            // No earlier date exists in DB
            std::cout << "Error: bad input => " << date << std::endl;
            continue;
        }
        // Print with the original token to match examples (e.g., "1.2")
        std::cout << date << " => " << valueStr << " = " << (value * rate) << std::endl;
    }
}

// ---------- helpers

std::string BitcoinExchange::trim(const std::string& s) {
    std::string::size_type a = 0, b = s.size();
    while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1]))) --b;
    return s.substr(a, b - a);
}

bool BitcoinExchange::parseCSVLine(const std::string& line, std::string& date, double& price) {
    std::string::size_type comma = line.find(',');
    if (comma == std::string::npos) return false;
    date = trim(line.substr(0, comma));
    std::string priceStr = trim(line.substr(comma + 1));
    if (!isValidDate(date)) return false;

    std::istringstream iss(priceStr);
    if (!(iss >> price)) return false;
    char c;
    if (iss >> c) return false; // trailing junk
    return true;
}

static bool onlySpaces(const std::string& s) {
    for (std::string::size_type i = 0; i < s.size(); ++i)
        if (!std::isspace(static_cast<unsigned char>(s[i])))
            return false;
    return true;
}

bool BitcoinExchange::parseInputLine(const std::string& line, std::string& date, std::string& valueStr, double& value, std::string& errMsg) {
    std::string raw = trim(line);
    if (raw.empty() || raw[0] == '#') return false; // ignore blanks / comments silently

    std::string::size_type bar = raw.find('|');
    if (bar == std::string::npos) { errMsg = "Error: bad input => " + raw; return false; }

    date = trim(raw.substr(0, bar));
    valueStr = trim(raw.substr(bar + 1));

    if (!isValidDate(date)) { errMsg = "Error: bad input => " + date; return false; }
    if (valueStr.empty() || onlySpaces(valueStr)) { errMsg = "Error: bad input => " + raw; return false; }

    // Parse number while rejecting trailing junk
    std::istringstream iss(valueStr);
    if (!(iss >> value)) { errMsg = "Error: bad input => " + raw; return false; }
    char c;
    if (iss >> c) { errMsg = "Error: bad input => " + raw; return false; }

    return true;
}

bool BitcoinExchange::isValidDate(const std::string& d) {
    if (d.size() != 10 || d[4] != '-' || d[7] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(static_cast<unsigned char>(d[i]))) return false;
    }
    int y = std::atoi(d.substr(0, 4).c_str());
    int m = std::atoi(d.substr(5, 2).c_str());
    int day = std::atoi(d.substr(8, 2).c_str());
    if (m < 1 || m > 12) return false;
    int dim = daysInMonth(y, m);
    if (day < 1 || day > dim) return false;
    return true;
}

bool BitcoinExchange::isLeap(int y) {
    if (y % 400 == 0) return true;
    if (y % 100 == 0) return false;
    return (y % 4 == 0);
}

int BitcoinExchange::daysInMonth(int y, int m) {
    static const int table[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2) return table[1] + (isLeap(y) ? 1 : 0);
    return table[m - 1];
}

bool BitcoinExchange::getRateForDateOrLower(const std::string& date, double& rate) const {
    if (_rates.empty()) return false;
    std::map<std::string, double>::const_iterator it = _rates.lower_bound(date);
    if (it == _rates.end()) {
        // use last available date
        --it;
        rate = it->second;
        return true;
    }
    if (it->first == date) {
        rate = it->second;
        return true;
    }
    if (it == _rates.begin()) {
        return false; // requested date earlier than earliest DB entry
    }
    --it; // previous (strictly lower) date
    rate = it->second;
    return true;
}
