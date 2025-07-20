#include "HumanA.hpp"
#include <iostream>

HumanA::HumanA(const std::string &name, Weapon &w)
    : name_(name), weapon_(w) {}

void HumanA::attack() const {
    std::cout << name_ << " attacks with their " 
              << weapon_.getType() << std::endl;
}
