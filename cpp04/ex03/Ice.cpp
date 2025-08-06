#include "Ice.hpp"
#include <iostream>

Ice::Ice() : AMateria("ice") {
    std::cout << "[Ice] constructor\n";
}

Ice::Ice(Ice const & other) : AMateria(other) {
    std::cout << "[Ice] copy constructor\n";
}

Ice::~Ice() {
    std::cout << "[Ice] destructor\n";
}

Ice & Ice::operator=(Ice const & other) {
    AMateria::operator=(other);
    return *this;
}

AMateria* Ice::clone() const {
    return new Ice(*this);
}

void Ice::use(ICharacter& target) {
    std::cout << "* shoots an ice bolt at "
              << target.getName()
              << " *\n";
}
