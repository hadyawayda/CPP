#include "FragTrap.hpp"

FragTrap::FragTrap(const std::string& name)
    : ClapTrap(name)
{
    hitPoints    = 100;
    energyPoints = 100;
    attackDamage = 30;
    std::cout << "FragTrap " << this->name << " constructed\n";
}

FragTrap::FragTrap(const FragTrap& other)
    : ClapTrap(other)
{
    std::cout << "FragTrap " << name << " copy constructed\n";
}

FragTrap& FragTrap::operator=(const FragTrap& other)
{
    ClapTrap::operator=(other);
    std::cout << "FragTrap " << name << " assigned\n";
    return *this;
}

FragTrap::~FragTrap()
{
    std::cout << "FragTrap " << name << " destructed\n";
}

void FragTrap::highFivesGuys()
{
    std::cout << "FragTrap " << name << " requests a high five!\n";
}
