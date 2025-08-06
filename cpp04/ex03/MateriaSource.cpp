#include "MateriaSource.hpp"
#include <iostream>

MateriaSource::MateriaSource() {
    for (int i = 0; i < 4; ++i)
        prototypes[i] = NULL;
    std::cout << "[MateriaSource] constructor\n";
}

MateriaSource::MateriaSource(MateriaSource const & other) {
    for (int i = 0; i < 4; ++i) {
        if (other.prototypes[i])
            prototypes[i] = other.prototypes[i]->clone();
        else
            prototypes[i] = NULL;
    }
    std::cout << "[MateriaSource] copy constructor\n";
}

MateriaSource::~MateriaSource() {
    for (int i = 0; i < 4; ++i)
        delete prototypes[i];
    std::cout << "[MateriaSource] destructor\n";
}

MateriaSource & MateriaSource::operator=(MateriaSource const & other) {
    if (this != &other) {
        for (int i = 0; i < 4; ++i) {
            delete prototypes[i];
            if (other.prototypes[i])
                prototypes[i] = other.prototypes[i]->clone();
            else
                prototypes[i] = NULL;
        }
    }
    std::cout << "[MateriaSource] assignment operator\n";
    return *this;
}

void MateriaSource::learnMateria(AMateria* m) {
    if (!m) return;
    for (int i = 0; i < 4; ++i) {
        if (!prototypes[i]) {
            prototypes[i] = m;
            std::cout << "[MateriaSource] learned " << m->getType() << "\n";
            return;
        }
    }
    std::cout << "[MateriaSource] cannot learn more, full\n";
}

AMateria* MateriaSource::createMateria(std::string const & type) {
    for (int i = 0; i < 4; ++i) {
        if (prototypes[i] && prototypes[i]->getType() == type) {
            AMateria* m = prototypes[i]->clone();
            std::cout << "[MateriaSource] creates " << type << "\n";
            return m;
        }
    }
    std::cout << "[MateriaSource] unknown materia " << type << "\n";
    return NULL;
}
