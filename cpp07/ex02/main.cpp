#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Array.hpp"

#define MAX_VAL 750

int main() {
    Array<int> numbers(MAX_VAL);
    int* mirror = new int[MAX_VAL];

    std::srand(std::time(NULL));
    for (int i = 0; i < MAX_VAL; i++) {
        int value = std::rand();
        numbers[i] = value;
        mirror[i]  = value;
    }

    // test copy‐ctor and assignment deep‐copy
    {
        Array<int> tmp = numbers;
        Array<int> test(tmp);
    }

    // ensure original still matches mirror
    for (int i = 0; i < MAX_VAL; i++) {
        if (mirror[i] != numbers[i]) {
            std::cerr << "didn't save the same value!!\n";
            delete [] mirror;
            return 1;
        }
    }

    // test out‐of‐bounds access
    try {
        numbers[-2] = 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    try {
        numbers[MAX_VAL] = 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    // mutate again
    for (int i = 0; i < MAX_VAL; i++) {
        numbers[i] = std::rand();
    }

    delete [] mirror;
    return 0;
}
