#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "ICharacter.hpp"
#include "AMateria.hpp"
#include <string>
#include <vector>

class Character : public ICharacter {
private:
    std::string       name;
    AMateria*         inventory[4];
    std::vector<AMateria*> floor;  // store unequipped materias for deletion

public:
    Character(std::string const & name);
    Character(Character const & other);
    virtual ~Character();

    Character & operator=(Character const & other);

    virtual std::string const & getName() const;
    virtual void equip(AMateria* m);
    virtual void unequip(int idx);
    virtual void use(int idx, ICharacter& target);
};

#endif  // CHARACTER_HPP
