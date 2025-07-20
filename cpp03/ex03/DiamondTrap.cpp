#include "DiamondTrap.hpp"

DiamondTrap::DiamondTrap(const std::string& nm)
    : ClapTrap(nm + "_clap_name")  // initialize the *virtual* base
    , ScavTrap(nm)
    , FragTrap(nm)
    , name(nm)
{
    // pick the right stats:
    hitPoints    = FragTrap::hitPoints;   // 100
    energyPoints = ScavTrap::energyPoints; //  50
    attackDamage = FragTrap::attackDamage; //  30
    std::cout << "DiamondTrap " << name << " constructed\n";
}

DiamondTrap::DiamondTrap(const DiamondTrap& o)
    : ClapTrap(o.ClapTrap::name)
    , ScavTrap(o)
    , FragTrap(o)
    , name(o.name)
{
    hitPoints    = o.hitPoints;
    energyPoints = o.energyPoints;
    attackDamage = o.attackDamage;
    std::cout << "DiamondTrap " << name << " copy constructed\n";
}

DiamondTrap& DiamondTrap::operator=(const DiamondTrap& o) {
    if (this != &o) {
        ClapTrap::operator=(o);
        name         = o.name;
        hitPoints    = o.hitPoints;
        energyPoints = o.energyPoints;
        attackDamage = o.attackDamage;
    }
    std::cout << "DiamondTrap " << name << " assigned\n";
    return *this;
}

DiamondTrap::~DiamondTrap() {
    std::cout << "DiamondTrap " << name << " destructed\n";
}

void DiamondTrap::whoAmI() {
    std::cout << "DiamondTrap name: " << name
              << ", ClapTrap name: " << ClapTrap::name << "\n";
}
