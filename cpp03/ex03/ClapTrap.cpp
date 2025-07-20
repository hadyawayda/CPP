#include "ClapTrap.hpp"

ClapTrap::ClapTrap(const std::string& name)
    : name(name), hitPoints(10), energyPoints(10), attackDamage(0)
{
    std::cout << "ClapTrap " << this->name << " constructed\n";
}

ClapTrap::ClapTrap(const ClapTrap& o)
    : name(o.name), hitPoints(o.hitPoints),
      energyPoints(o.energyPoints), attackDamage(o.attackDamage)
{
    std::cout << "ClapTrap " << name << " copy constructed\n";
}

ClapTrap& ClapTrap::operator=(const ClapTrap& o)
{
    if (this != &o) {
        name         = o.name;
        hitPoints    = o.hitPoints;
        energyPoints = o.energyPoints;
        attackDamage = o.attackDamage;
    }
    std::cout << "ClapTrap " << name << " assigned\n";
    return *this;
}

ClapTrap::~ClapTrap()
{
    std::cout << "ClapTrap " << name << " destructed\n";
}

void ClapTrap::attack(const std::string& target) {
    if (!hitPoints)
        std::cout << "ClapTrap " << name
                  << " can't attack, no HP left!\n";
    else if (!energyPoints)
        std::cout << "ClapTrap " << name
                  << " can't attack, no EP left!\n";
    else {
        --energyPoints;
        std::cout << "ClapTrap " << name
                  << " attacks " << target
                  << ", causing " << attackDamage
                  << " points of damage!\n";
    }
}

void ClapTrap::takeDamage(unsigned int amount) {
    hitPoints = (hitPoints <= amount ? 0 : hitPoints - amount);
    std::cout << "ClapTrap " << name
              << " takes " << amount
              << " damage, HP now " << hitPoints << "\n";
}

void ClapTrap::beRepaired(unsigned int amount) {
    if (!hitPoints)
        std::cout << "ClapTrap " << name
                  << " can't be repaired, dead!\n";
    else if (!energyPoints)
        std::cout << "ClapTrap " << name
                  << " can't be repaired, no EP left!\n";
    else {
        --energyPoints;
        hitPoints += amount;
        std::cout << "ClapTrap " << name
                  << " repairs " << amount
                  << ", HP now " << hitPoints << "\n";
    }
}
