#include "MutantStack.hpp"
#include <iostream>
#include <list>

int main() {
    std::cout << "=== MutantStack<int> demo ===\n";
    MutantStack<int> mstack;
    mstack.push(5);
    mstack.push(17);
    std::cout << "top(): " << mstack.top() << "\n";
    mstack.pop();
    std::cout << "size(): " << mstack.size() << "\n";
    mstack.push(3);
    mstack.push(5);
    mstack.push(737);
    // [...]
    mstack.push(0);

    MutantStack<int>::iterator it = mstack.begin();
    MutantStack<int>::iterator ite = mstack.end();
    ++it;
    --it;
    std::cout << "contents:\n";
    while (it != ite) {
        std::cout << *it << "\n";
        ++it;
    }

    std::cout << "\n=== std::list<int> demo ===\n";
    std::list<int> lst;
    lst.push_back(5);
    lst.push_back(17);
    std::cout << "back(): " << lst.back() << "\n";
    lst.pop_back();
    std::cout << "size(): " << lst.size() << "\n";
    lst.push_back(3);
    lst.push_back(5);
    lst.push_back(737);
    // [...]
    lst.push_back(0);

    std::cout << "contents:\n";
    for (std::list<int>::iterator lit = lst.begin(); lit != lst.end(); ++lit)
        std::cout << *lit << "\n";

    return 0;
}
