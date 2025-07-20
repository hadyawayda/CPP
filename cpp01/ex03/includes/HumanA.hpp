#ifndef HUMANA_HPP
#define HUMANA_HPP

#include <string>
#include "Weapon.hpp"

class HumanA {
private:
    std::string name_;
    Weapon     &weapon_;
public:
    HumanA(const std::string &name, Weapon &w);
    void    attack() const;
};

#endif  // HUMANA_HPP
