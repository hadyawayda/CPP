#include "WrongAnimal.hpp"

WrongAnimal::WrongAnimal() : type("WrongAnimal") {
    std::cout << "[WrongAnimal] default constructor called\n";
}

WrongAnimal::WrongAnimal(const WrongAnimal& other) : type(other.type) {
    std::cout << "[WrongAnimal] copy constructor called\n";
}

WrongAnimal::~WrongAnimal() {
    std::cout << "[WrongAnimal] destructor called\n";
}

WrongAnimal& WrongAnimal::operator=(const WrongAnimal& other) {
    std::cout << "[WrongAnimal] assignment operator called\n";
    if (this != &other)
        type = other.type;
    return *this;
}

const std::string& WrongAnimal::getType() const {
    return type;
}

void WrongAnimal::makeSound() const {
    std::cout << "[WrongAnimal] *generic wrong animal sound*\n";
}
