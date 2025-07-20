#include "Zombie.hpp"
#include <iostream>

Zombie::Zombie() : name_("") {}
Zombie::Zombie(const std::string &name) : name_(name) {}
Zombie::~Zombie() {
    std::cout << name_ << " is destroyed\n";
}

void Zombie::announce() const {
    std::cout << name_ << ": BraiiiiiiinnnzzzZ...\n";
}

void Zombie::setName(const std::string &name) {
    name_ = name;
}
