#include "Cure.hpp"
#include <iostream>

Cure::Cure() : AMateria("cure") {
    std::cout << "[Cure] constructor\n";
}

Cure::Cure(Cure const & other) : AMateria(other) {
    std::cout << "[Cure] copy constructor\n";
}

Cure::~Cure() {
    std::cout << "[Cure] destructor\n";
}

Cure & Cure::operator=(Cure const & other) {
    AMateria::operator=(other);
    return *this;
}

AMateria* Cure::clone() const {
    return new Cure(*this);
}

void Cure::use(ICharacter& target) {
    std::cout << "* heals "
              << target.getName()
              << "'s wounds *\n";
}
