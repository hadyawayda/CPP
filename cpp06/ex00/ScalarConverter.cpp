#include "ScalarConverter.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <cerrno>
#include <cctype>

void ScalarConverter::convert(const std::string& s) {
    enum Type { CHAR, INT, FLOAT, DOUBLE, PSEUDO_FLOAT, PSEUDO_DOUBLE, INVALID };
    Type type = INVALID;
    char    c = 0;
    long    li = 0;
    float   f = 0.0f;
    double  d = 0.0;
    // 1) pseudo‐literals
    if (s == "nanf" || s == "+inff" || s == "-inff") {
        type = PSEUDO_FLOAT;
        if (s[0] == '-')      f = -std::numeric_limits<float>::infinity();
        else if (s[0] == '+') f =  std::numeric_limits<float>::infinity();
        else                   f =  std::numeric_limits<float>::quiet_NaN();
        d = static_cast<double>(f);
    }
    else if (s == "nan"  || s == "+inf"  || s == "-inf") {
        type = PSEUDO_DOUBLE;
        if (s[0] == '-')      d = -std::numeric_limits<double>::infinity();
        else if (s[0] == '+') d =  std::numeric_limits<double>::infinity();
        else                   d =  std::numeric_limits<double>::quiet_NaN();
        f = static_cast<float>(d);
    }
    // 2) single non‐digit char → CHAR
    else if (s.size() == 1 && !std::isdigit(s[0])) {
        type = CHAR;
        c = s[0];
        li = static_cast<long>(c);
        f  = static_cast<float>(c);
        d  = static_cast<double>(c);
    }
    else {
        char* end = NULL;
        errno = 0;
        long val = std::strtol(s.c_str(), &end, 10);
        if (*end == '\0' && errno == 0) {
            type = INT;
            li = val;
            c  = static_cast<char>(li);
            f  = static_cast<float>(li);
            d  = static_cast<double>(li);
        }
        else {
            errno = 0;
            char* endf = NULL;
            double vald = std::strtod(s.c_str(), &endf);
            if (*endf == 'f' && endf[1] == '\0' && errno == 0) {
                type = FLOAT;
                f = static_cast<float>(vald);
                d = static_cast<double>(f);
                li = static_cast<long>(f);
                c  = static_cast<char>(li);
            }
            else {
                errno = 0;
                char* endd = NULL;
                double vald2 = std::strtod(s.c_str(), &endd);
                if (*endd == '\0' && errno == 0) {
                    type = DOUBLE;
                    d = vald2;
                    f = static_cast<float>(d);
                    li = static_cast<long>(d);
                    c  = static_cast<char>(li);
                }
            }
        }
    }

    // --- char ---
    std::cout << "char: ";
    if (type == INVALID || type == PSEUDO_FLOAT || type == PSEUDO_DOUBLE
        || li < std::numeric_limits<char>::min()
        || li > std::numeric_limits<char>::max())
    {
        std::cout << "impossible\n";
    }
    else if (!std::isprint(static_cast<unsigned char>(c))) {
        std::cout << "Non displayable\n";
    }
    else {
        std::cout << "'" << c << "'\n";
    }

    // --- int ---
    std::cout << "int: ";
    if (type == INVALID || type == PSEUDO_FLOAT || type == PSEUDO_DOUBLE
        || d < static_cast<double>(std::numeric_limits<int>::min())
        || d > static_cast<double>(std::numeric_limits<int>::max()))
    {
        std::cout << "impossible\n";
    }
    else {
        std::cout << static_cast<int>(li) << "\n";
    }

    // --- float ---
    std::cout << "float: ";
    if (type == INVALID) {
        std::cout << "impossible\n";
    }
    else if (type == PSEUDO_FLOAT) {
        std::cout << s << "\n";
    }
    else if (type == PSEUDO_DOUBLE) {
        std::cout << s << "f\n";
    }
    else {
        std::cout << std::fixed << std::setprecision(1)
                  << f << "f\n";
    }

    // --- double ---
    std::cout << "double: ";
    if (type == INVALID) {
        std::cout << "impossible\n";
    }
    else if (type == PSEUDO_DOUBLE) {
        std::cout << s << "\n";
    }
    else if (type == PSEUDO_FLOAT) {
        std::cout << s.substr(0, s.size()-1) << "\n";
    }
    else {
        std::cout << std::fixed << std::setprecision(1)
                  << d << "\n";
    }
}
