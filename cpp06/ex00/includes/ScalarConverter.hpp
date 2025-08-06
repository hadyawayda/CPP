#ifndef SCALARCONVERTER_HPP
#define SCALARCONVERTER_HPP

#include <string>

class ScalarConverter {
private:
    ScalarConverter();
    ~ScalarConverter();
    ScalarConverter(const ScalarConverter&);
    ScalarConverter& operator=(const ScalarConverter&);
public:
    static void convert(const std::string& literal);
};

#endif  // SCALARCONVERTER_HPP
