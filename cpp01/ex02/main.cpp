#include <iostream>
#include <string>

int main() {
    std::string brain = "HI THIS IS BRAIN";
    std::string *stringPTR = &brain;
    std::string &stringREF = brain;

    std::cout << "Address of brain: " << &brain << std::endl;
    std::cout << "Address held by ptr: " << stringPTR << std::endl;
    std::cout << "Address held by ref: " << &stringREF << std::endl;

    std::cout << "Value of brain: " << brain << std::endl;
    std::cout << "Value pointed by ptr: " << *stringPTR << std::endl;
    std::cout << "Value pointed by ref: " << stringREF << std::endl;
    return 0;
}
