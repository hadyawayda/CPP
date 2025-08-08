#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include "easyfind.hpp"

template <typename Cont>
void testFind(Cont& c, int needle, const char* name) {
    std::cout << "[" << name << "] find(" << needle << "): ";
    typename Cont::const_iterator it = easyfind(c, needle);
    if (it == c.end())
        std::cout << "not found\n";
    else
        std::cout << "found\n";
}

int main() {
    std::vector<int> v;
    for (int i = 0; i < 5; ++i) v.push_back(i * 10); // 0 10 20 30 40

    std::list<int> L;
    for (int i = 3; i <= 7; ++i) L.push_back(i);     // 3 4 5 6 7

    std::deque<int> d;
    d.push_back(42);
    d.push_back(100);
    d.push_back(-5);

    testFind(v, 20, "vector");
    testFind(v, 99, "vector");

    testFind(L, 6,  "list");
    testFind(L, 9,  "list");

    testFind(d, 42,   "deque");
    testFind(d, -123, "deque");

    // Non-const usage example (modify found element)
    std::cout << "\nModify in non-const container example:\n";
    std::vector<int>::iterator it = easyfind(v, 30);
    if (it != v.end()) {
        *it = 777;
        std::cout << "Replaced 30 with " << *it << "\n";
    }

    // Const container example compiles against const overload
    const std::vector<int> cv = v;
    std::vector<int>::const_iterator cit = easyfind(cv, 777);
    std::cout << "777 in const vector: " << (cit != cv.end() ? "found" : "not found") << "\n";

    return 0;
}
