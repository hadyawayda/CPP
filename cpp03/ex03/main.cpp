#include "DiamondTrap.hpp"

int main() {
    DiamondTrap dt("Diamond");
    dt.attack("Enemy");       // ScavTrap::attack
    dt.takeDamage(20);        // ClapTrap::takeDamage
    dt.beRepaired(10);        // ClapTrap::beRepaired
    dt.highFivesGuys();       // FragTrap::highFivesGuys
    dt.guardGate();           // ScavTrap::guardGate
    dt.whoAmI();              // DiamondTrap::whoAmI
    return 0;
}
