#include "ScavTrap.hpp"

int main() {
    ScavTrap st("Scavvy");
    st.attack("Target");
    st.takeDamage(30);
    st.beRepaired(20);
    st.guardGate();
    return 0;
}
