#include <iostream>
#include "IMateriaSource.hpp"
#include "MateriaSource.hpp"
#include "ICharacter.hpp"
#include "Character.hpp"
#include "Ice.hpp"
#include "Cure.hpp"

int main() {
    std::cout << "=== Basic scenario ===\n";
    IMateriaSource* src = new MateriaSource();
    src->learnMateria(new Ice());
    src->learnMateria(new Cure());
    src->learnMateria(new Ice());
    src->learnMateria(new Cure());
    src->learnMateria(new Ice());  // should report “cannot learn more”

    ICharacter* me = new Character("me");
    AMateria* tmp;
    tmp = src->createMateria("ice");
    me->equip(tmp);
    tmp = src->createMateria("cure");
    me->equip(tmp);
    tmp = src->createMateria("ice");
    me->equip(tmp);
    tmp = src->createMateria("cure");
    me->equip(tmp);
    tmp = src->createMateria("ice"); // should report “unknown materia” or “cannot equip”
    me->equip(tmp);

    ICharacter* bob = new Character("bob");
    // use all slots
    for (int i = 0; i < 5; ++i)
        me->use(i, *bob);

    std::cout << "\n=== Test unequip ===\n";
    me->unequip(1);
    tmp = src->createMateria("cure");
    me->equip(tmp);
    me->use(1, *bob);

    std::cout << "\n=== Cleanup ===\n";
    delete bob;
    delete me;
    delete src;
    return 0;
}
