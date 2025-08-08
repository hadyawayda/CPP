#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Span.hpp"

static void basic_demo() {
    std::cout << "== Basic demo ==\n";
    Span sp(5);
    sp.addNumber(6);
    sp.addNumber(3);
    sp.addNumber(17);
    sp.addNumber(9);
    sp.addNumber(11);
    std::cout << sp.shortestSpan() << std::endl; // 2
    std::cout << sp.longestSpan()  << std::endl; // 14
}

static void errors_demo() {
    std::cout << "\n== Errors demo ==\n";
    Span s(2);
    try {
        s.shortestSpan();
    } catch (const std::exception& e) {
        std::cout << "shortestSpan() error: " << e.what() << "\n";
    }
    s.addNumber(1);
    try {
        s.longestSpan();
    } catch (const std::exception& e) {
        std::cout << "longestSpan() error: " << e.what() << "\n";
    }
    s.addNumber(2);
    try {
        s.addNumber(3);
    } catch (const std::exception& e) {
        std::cout << "addNumber() error: " << e.what() << "\n";
    }
}

static void bulk_demo() {
    std::cout << "\n== Bulk add (10,000) ==\n";
    const unsigned int N = 10000;
    Span big(N);

    std::srand(static_cast<unsigned int>(std::time(NULL)));
    std::vector<int> buf;
    buf.reserve(N);
    for (unsigned int i = 0; i < N; ++i) {
        // random-ish spread
        buf.push_back(std::rand());
    }

    big.addRange(buf.begin(), buf.end());

    std::cout << "size=" << big.size()
              << ", shortest=" << big.shortestSpan()
              << ", longest="  << big.longestSpan()
              << "\n";
}

int main() {
    basic_demo();
    errors_demo();
    bulk_demo();
    return 0;
}
