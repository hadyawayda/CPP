#include "Data.hpp"

Data::Data() : id(0), name("") {}

Data::Data(int i, const std::string& n)
    : id(i), name(n)
{}
