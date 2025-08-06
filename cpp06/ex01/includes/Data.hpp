#ifndef DATA_HPP
#define DATA_HPP

#include <string>

struct Data {
    int         id;
    std::string name;

    Data();
    Data(int i, const std::string& n);
};

#endif  // DATA_HPP
