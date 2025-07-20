#include "FragTrap.hpp"

int main() {
    FragTrap ft("Fraggy");
    ft.attack("Target");
    ft.takeDamage(40);
    ft.beRepaired(25);
    ft.highFivesGuys();
    return 0;
}
