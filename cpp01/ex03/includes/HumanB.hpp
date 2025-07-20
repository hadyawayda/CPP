#ifndef HUMANB_HPP
#define HUMANB_HPP

#include <string>
#include "Weapon.hpp"

class HumanB {
private:
    std::string name_;
    Weapon     *weapon_;
public:
    HumanB(const std::string &name);
    void   setWeapon(Weapon &w);
    void   attack() const;
};

#endif  // HUMANB_HPP
