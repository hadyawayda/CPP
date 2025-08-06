#include <iostream>
#include "Bureaucrat.hpp"

int main() {
    std::cout << "== Valid construction and print ==\n";
    try {
        Bureaucrat a("Alice", 42);
        std::cout << a << "\n";
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    std::cout << "\n== Increment at top bound ==\n";
    try {
        Bureaucrat top("TopGuy", 1);
        std::cout << top << "\n";
        top.incrementGrade();  // should throw
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    std::cout << "\n== Decrement at bottom bound ==\n";
    try {
        Bureaucrat bot("BottomGuy", 150);
        std::cout << bot << "\n";
        bot.decrementGrade();  // should throw
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    std::cout << "\n== Construction with invalid grades ==\n";
    try {
        Bureaucrat tooHigh("H", 0);
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    try {
        Bureaucrat tooLow("L", 151);
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    std::cout << "\n== Grade moves within bounds ==\n";
    try {
        Bureaucrat b("Bob", 2);
        std::cout << b << "\n";
        b.incrementGrade();  // to grade 1
        std::cout << b << "\n";
        b.decrementGrade();  // back to grade 2
        b.decrementGrade();  // to grade 3
        std::cout << b << "\n";
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    return 0;
}
