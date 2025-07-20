#include "ClapTrap.hpp"

int main() {
    ClapTrap ct("Clappy");
    ct.attack("Target");
    ct.takeDamage(5);
    ct.beRepaired(3);
    return 0;
}
