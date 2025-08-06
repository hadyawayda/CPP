#include "Cat.hpp"

Cat::Cat() {
    type = "Cat";
    std::cout << "[Cat] default constructor called\n";
}

Cat::Cat(const Cat& other) : Animal(other) {
    std::cout << "[Cat] copy constructor called\n";
}

Cat::~Cat() {
    std::cout << "[Cat] destructor called\n";
}

Cat& Cat::operator=(const Cat& other) {
    std::cout << "[Cat] assignment operator called\n";
    if (this != &other)
        Animal::operator=(other);
    return *this;
}

void Cat::makeSound() const {
    std::cout << "[Cat] Meow! 🐱\n";
}
