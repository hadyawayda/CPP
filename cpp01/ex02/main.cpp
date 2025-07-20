#include <iostream>
#include <string>

int main() {
    std::string brain = "HI THIS IS BRAIN";
    std::string *ptr = &brain;
    std::string &ref = brain;

    std::cout << "Address of brain: " << &brain << std::endl;
    std::cout << "Address held by ptr: " << ptr << std::endl;
    std::cout << "Address held by ref: " << &ref << std::endl;

    std::cout << "Value of brain: " << brain << std::endl;
    std::cout << "Value pointed by ptr: " << *ptr << std::endl;
    std::cout << "Value pointed by ref: " << ref << std::endl;
    return 0;
}
