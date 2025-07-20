#include "HumanB.hpp"
#include <iostream>

HumanB::HumanB(const std::string &name)
    : name_(name), weapon_(0) {}

void HumanB::setWeapon(Weapon &w) {
    weapon_ = &w;
}

void HumanB::attack() const {
    if (weapon_)
        std::cout << name_ << " attacks with their "
                  << weapon_->getType() << std::endl;
    else
        std::cout << name_ << " has no weapon\n";
}
