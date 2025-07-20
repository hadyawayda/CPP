#include "ClapTrap.hpp"

ClapTrap::ClapTrap(const std::string& name)
    : name(name), hitPoints(10), energyPoints(10), attackDamage(0)
{
    std::cout << "ClapTrap " << this->name << " constructed\n";
}

ClapTrap::ClapTrap(const ClapTrap& other)
    : name(other.name),
      hitPoints(other.hitPoints),
      energyPoints(other.energyPoints),
      attackDamage(other.attackDamage)
{
    std::cout << "ClapTrap " << name << " copy constructed\n";
}

ClapTrap& ClapTrap::operator=(const ClapTrap& other)
{
    if (this != &other) {
        name = other.name;
        hitPoints = other.hitPoints;
        energyPoints = other.energyPoints;
        attackDamage = other.attackDamage;
    }
    std::cout << "ClapTrap " << name << " assigned\n";
    return *this;
}

ClapTrap::~ClapTrap()
{
    std::cout << "ClapTrap " << name << " destructed\n";
}

void ClapTrap::attack(const std::string& target)
{
    if (hitPoints == 0)
        std::cout << "ClapTrap " << name << " can't attack, no hit points left!\n";
    else if (energyPoints == 0)
        std::cout << "ClapTrap " << name << " can't attack, no energy points left!\n";
    else {
        --energyPoints;
        std::cout << "ClapTrap " << name
                  << " attacks " << target
                  << ", causing " << attackDamage
                  << " points of damage!\n";
    }
}

void ClapTrap::takeDamage(unsigned int amount)
{
    if (hitPoints <= amount)
        hitPoints = 0;
    else
        hitPoints -= amount;
    std::cout << "ClapTrap " << name
              << " takes " << amount
              << " points of damage! Remaining HP: "
              << hitPoints << "\n";
}

void ClapTrap::beRepaired(unsigned int amount)
{
    if (hitPoints == 0)
        std::cout << "ClapTrap " << name << " can't be repaired, dead!\n";
    else if (energyPoints == 0)
        std::cout << "ClapTrap " << name << " can't be repaired, no energy!\n";
    else {
        --energyPoints;
        hitPoints += amount;
        std::cout << "ClapTrap " << name
                  << " is repaired for " << amount
                  << " points! Remaining HP: "
                  << hitPoints << "\n";
    }
}
