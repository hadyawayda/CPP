#include "ScavTrap.hpp"

ScavTrap::ScavTrap(const std::string& name)
    : ClapTrap(name)
{
    hitPoints    = 100;
    energyPoints = 50;
    attackDamage = 20;
    std::cout << "ScavTrap " << this->name << " constructed\n";
}

ScavTrap::ScavTrap(const ScavTrap& other)
    : ClapTrap(other)
{
    std::cout << "ScavTrap " << name << " copy constructed\n";
}

ScavTrap& ScavTrap::operator=(const ScavTrap& other)
{
    ClapTrap::operator=(other);
    std::cout << "ScavTrap " << name << " assigned\n";
    return *this;
}

ScavTrap::~ScavTrap()
{
    std::cout << "ScavTrap " << name << " destructed\n";
}

void ScavTrap::attack(const std::string& target)
{
    if (hitPoints == 0)
        std::cout << "ScavTrap " << name << " can't attack, no HP!\n";
    else if (energyPoints == 0)
        std::cout << "ScavTrap " << name << " can't attack, no EP!\n";
    else {
        --energyPoints;
        std::cout << "ScavTrap " << name
                  << " attacks " << target
                  << ", causing " << attackDamage
                  << " points of damage!\n";
    }
}

void ScavTrap::guardGate()
{
    std::cout << "ScavTrap " << name << " is now in Gate keeper mode.\n";
}
