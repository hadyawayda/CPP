// main.cpp
#include "Identify.hpp"
#include "Base.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned>(std::time(NULL)));
    for (int i = 0; i < 10; ++i) {
        Base* p = Base::generate();
        std::cout << "Iteration " << i << ":\n";
        std::cout << "  identify(Base*): ";
        identify(p);
        std::cout << "  identify(Base&): ";
        identify(*p);
        delete p;
    }
    return 0;
}
