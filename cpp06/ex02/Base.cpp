// Base.cpp
#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include <cstdlib>

Base::~Base() {}

Base* Base::generate() {
    int r = std::rand() % 3;
    if (r == 0) return new A;
    if (r == 1) return new B;
    return new C;
}
