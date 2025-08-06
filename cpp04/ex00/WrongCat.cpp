#include "WrongCat.hpp"

WrongCat::WrongCat() {
    type = "WrongCat";
    std::cout << "[WrongCat] default constructor called\n";
}

WrongCat::WrongCat(const WrongCat& other) : WrongAnimal(other) {
    std::cout << "[WrongCat] copy constructor called\n";
}

WrongCat::~WrongCat() {
    std::cout << "[WrongCat] destructor called\n";
}

WrongCat& WrongCat::operator=(const WrongCat& other) {
    std::cout << "[WrongCat] assignment operator called\n";
    if (this != &other)
        WrongAnimal::operator=(other);
    return *this;
}

void WrongCat::makeSound() const {
    std::cout << "[WrongCat] Meooow... (wrongly) 🐈\n";
}
