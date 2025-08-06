#include "Dog.hpp"

Dog::Dog() {
    type = "Dog";
    std::cout << "[Dog] default constructor called\n";
}

Dog::Dog(const Dog& other) : Animal(other) {
    std::cout << "[Dog] copy constructor called\n";
}

Dog::~Dog() {
    std::cout << "[Dog] destructor called\n";
}

Dog& Dog::operator=(const Dog& other) {
    std::cout << "[Dog] assignment operator called\n";
    if (this != &other)
        Animal::operator=(other);
    return *this;
}

void Dog::makeSound() const {
    std::cout << "[Dog] Woof! 🐶\n";
}
