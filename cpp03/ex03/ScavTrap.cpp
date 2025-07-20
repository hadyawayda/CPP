#include "ScavTrap.hpp"

ScavTrap::ScavTrap(const std::string& name)
    : ClapTrap(name)
{
    hitPoints    = 100;
    energyPoints = 50;
    attackDamage = 20;
    std::cout << "ScavTrap " << this->name << " constructed\n";
}

ScavTrap::ScavTrap(const ScavTrap& o)
    : ClapTrap(o)
{
    std::cout << "ScavTrap " << name << " copy constructed\n";
}

ScavTrap& ScavTrap::operator=(const ScavTrap& o) {
    ClapTrap::operator=(o);
    std::cout << "ScavTrap " << name << " assigned\n";
    return *this;
}

ScavTrap::~ScavTrap() {
    std::cout << "ScavTrap " << name << " destructed\n";
}

void ScavTrap::attack(const std::string& target) {
    if (!hitPoints)    std::cout << "ScavTrap " << name << " no HP!\n";
    else if (!energyPoints) std::cout << "ScavTrap " << name << " no EP!\n";
    else {
        --energyPoints;
        std::cout << "ScavTrap " << name
                  << " attacks " << target
                  << ", dealing " << attackDamage << "!\n";
    }
}

void ScavTrap::guardGate() {
    std::cout << "ScavTrap " << name << " is now in Gate keeper mode\n";
}
