#include "Cat.hpp"

Cat::Cat()
    : Animal(), brain(new Brain())
{
    type = "Cat";
    std::cout << "[Cat] default constructor called\n";
}

Cat::Cat(const Cat& other)
    : Animal(other), brain(new Brain(*other.brain))
{
    std::cout << "[Cat] copy constructor called\n";
}

Cat::~Cat() {
    delete brain;
    std::cout << "[Cat] destructor called\n";
}

Cat& Cat::operator=(const Cat& other) {
    std::cout << "[Cat] assignment operator called\n";
    if (this != &other) {
        Animal::operator=(other);
        *brain = *other.brain;
    }
    return *this;
}

void Cat::makeSound() const {
    std::cout << "[Cat] Meow! 🐱\n";
}

Brain* Cat::getBrain() const {
    return brain;
}
