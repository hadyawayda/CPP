#include "Animal.hpp"

Animal::Animal() : type("Animal") {
    std::cout << "[Animal] default constructor called\n";
}

Animal::Animal(const Animal& other) : type(other.type) {
    std::cout << "[Animal] copy constructor called\n";
}

Animal::~Animal() {
    std::cout << "[Animal] destructor called\n";
}

Animal& Animal::operator=(const Animal& other) {
    std::cout << "[Animal] assignment operator called\n";
    if (this != &other)
        type = other.type;
    return *this;
}

const std::string& Animal::getType() const {
    return type;
}

void Animal::makeSound() const {
    std::cout << "[Animal] *generic animal sound* 🐾\n";
}
