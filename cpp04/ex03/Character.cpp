#include "Character.hpp"
#include <iostream>

Character::Character(std::string const & n) : name(n) {
    for (int i = 0; i < 4; ++i)
        inventory[i] = NULL;
    std::cout << "[Character " << name << "] constructor\n";
}

Character::Character(Character const & other) : name(other.name) {
    for (int i = 0; i < 4; ++i) {
        if (other.inventory[i])
            inventory[i] = other.inventory[i]->clone();
        else
            inventory[i] = NULL;
    }
    for (size_t i = 0; i < other.floor.size(); ++i)
        floor.push_back(other.floor[i]->clone());
    std::cout << "[Character " << name << "] copy constructor\n";
}

Character::~Character() {
    for (int i = 0; i < 4; ++i)
        delete inventory[i];
    for (size_t i = 0; i < floor.size(); ++i)
        delete floor[i];
    std::cout << "[Character " << name << "] destructor\n";
}

Character & Character::operator=(Character const & other) {
    if (this != &other) {
        name = other.name;
        for (int i = 0; i < 4; ++i) {
            delete inventory[i];
            if (other.inventory[i])
                inventory[i] = other.inventory[i]->clone();
            else
                inventory[i] = NULL;
        }
        for (size_t i = 0; i < floor.size(); ++i)
            delete floor[i];
        floor.clear();
        for (size_t i = 0; i < other.floor.size(); ++i)
            floor.push_back(other.floor[i]->clone());
    }
    std::cout << "[Character " << name << "] assignment operator\n";
    return *this;
}

std::string const & Character::getName() const {
    return name;
}

void Character::equip(AMateria* m) {
    if (!m) return;
    for (int i = 0; i < 4; ++i) {
        if (!inventory[i]) {
            inventory[i] = m;
            std::cout << "[Character " << name 
                      << "] equips " << m->getType()
                      << " in slot " << i << "\n";
            return;
        }
    }
    // full
    std::cout << "[Character " << name 
              << "] inventory full, cannot equip " 
              << m->getType() << "\n";
}

void Character::unequip(int idx) {
    if (idx < 0 || idx > 3 || !inventory[idx])
        return;
    floor.push_back(inventory[idx]);
    std::cout << "[Character " << name 
              << "] unequips slot " << idx << "\n";
    inventory[idx] = NULL;
}

void Character::use(int idx, ICharacter& target) {
    if (idx < 0 || idx > 3 || !inventory[idx])
        return;
    inventory[idx]->use(target);
}
