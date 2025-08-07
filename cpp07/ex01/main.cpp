#include <iostream>
#include <string>
#include "iter.hpp"

// ---- Test functions for int ----
void printInt(int& x) {
    std::cout << x << ' ';
}
void incInt(int& x) {
    ++x;
}
void printConstInt(int const& x) {
    std::cout << x << ' ';
}

// ---- Test functions for std::string ----
void printStr(std::string& s) {
    std::cout << s << ' ';
}
void printConstStr(std::string const& s) {
    std::cout << s << ' ';
}

// ---- Test functions for char ----
void printChar(char const& c) {
    std::cout << c << ' ';
}
void upperChar(char& c) {
    if (c >= 'a' && c <= 'z')
        c = static_cast<char>(c - 'a' + 'A');
}

int main() {
    // 1) int array, mutate + print
    int iarr[] = { 0, 1, 2, 3, 4 };
    std::cout << "Original ints: ";
    iter(iarr, 5, printInt);
    std::cout << "\nIncrement ints: ";
    iter(iarr, 5, incInt);
    std::cout << "\nAfter inc:      ";
    iter(iarr, 5, printConstInt);
    std::cout << "\n\n";

    // 2) string array, non-const + const overload
    std::string sarr[] = { "foo", "bar", "baz" };
    std::cout << "Strings:        ";
    iter(sarr, 3, printStr);
    std::cout << "\nConst strings:  ";
    iter(sarr, 3, printConstStr);
    std::cout << "\n\n";

    // 3) const string array
    const std::string csarr[] = { "red", "green", "blue" };
    std::cout << "Const only str: ";
    iter(csarr, 3, printConstStr);
    std::cout << "\n\n";

    // 4) char array, mutate + print
    char carr[] = { 'a', 'b', 'c', 'd' };
    std::cout << "Chars:          ";
    iter(carr, 4, printChar);
    std::cout << "\nUppercase:      ";
    iter(carr, 4, upperChar);
    std::cout << "\nAfter upper:    ";
    iter(carr, 4, printChar);
    std::cout << "\n";

    return 0;
}
