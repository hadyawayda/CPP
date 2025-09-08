#include "RPN.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    try {
        RPN calc;
        long result = calc.evaluate(argv[1]);
        std::cout << result << std::endl;
    } catch (const std::exception&) {
        std::cerr << "Error" << std::endl; // per subject examples
        return 1;
    }
    return 0;
}
