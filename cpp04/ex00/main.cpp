#include <iostream>
#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"

int main() {
    std::cout << "=== CORRECT POLYMORPHISM ===\n";
    const Animal* meta = new Animal();
    const Animal* j    = new Dog();
    const Animal* i    = new Cat();

    std::cout << j->getType() << " says: ";
    j->makeSound();

    std::cout << i->getType() << " says: ";
    i->makeSound();

    std::cout << meta->getType() << " says: ";
    meta->makeSound();

    delete meta;
    delete j;
    delete i;

    std::cout << "\n=== WRONG POLYMORPHISM ===\n";
    const WrongAnimal* wmeta = new WrongAnimal();
    const WrongAnimal* wc    = new WrongCat();

    std::cout << wc->getType() << " says: ";
    wc->makeSound();   // calls WrongAnimal::makeSound()

    std::cout << wmeta->getType() << " says: ";
    wmeta->makeSound();

    delete wmeta;
    delete wc;

    std::cout << "\n=== BULK TEST ===\n";
    Animal* herd[4];
    herd[0] = new Dog();
    herd[1] = new Cat();
    herd[2] = new Dog();
    herd[3] = new Cat();
    for (int k = 0; k < 4; ++k) {
        std::cout << herd[k]->getType()
                  << " at index " << k
                  << " goes: ";
        herd[k]->makeSound();
    }
    for (int k = 0; k < 4; ++k)
        delete herd[k];

    return 0;
}
