#include "AMateria.hpp"

AMateria::AMateria(std::string const & t) : type(t) {}
AMateria::AMateria(AMateria const & other) : type(other.type) {}
AMateria::~AMateria() {}

AMateria & AMateria::operator=(AMateria const & other) {
    if (this != &other)
        type = other.type;
    return *this;
}

std::string const & AMateria::getType() const {
    return type;
}

void AMateria::use(ICharacter& target) {
    (void)target;
    // base does nothing
}
