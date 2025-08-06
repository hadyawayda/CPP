// identify.cpp
#include "Identify.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include <iostream>

// pointer version
void identify(Base* p) {
    if (!p) {
        std::cout << "Unknown\n";
        return;
    }
    if (dynamic_cast<A*>(p))      std::cout << "A\n";
    else if (dynamic_cast<B*>(p)) std::cout << "B\n";
    else if (dynamic_cast<C*>(p)) std::cout << "C\n";
    else                          std::cout << "Unknown\n";
}

// reference version
void identify(Base& p) {
    try {
        (void)dynamic_cast<A&>(p);
        std::cout << "A\n";
        return;
    } catch (...) {}
    try {
        (void)dynamic_cast<B&>(p);
        std::cout << "B\n";
        return;
    } catch (...) {}
    try {
        (void)dynamic_cast<C&>(p);
        std::cout << "C\n";
        return;
    } catch (...) {}
    std::cout << "Unknown\n";
}
