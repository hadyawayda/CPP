#include <iostream>
#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"

#define ANIMAL_COUNT 4

int main() {
    std::cout << "=== Creating an array of Animals ===\n";
    Animal* animals[ANIMAL_COUNT];
    for (int i = 0; i < ANIMAL_COUNT; ++i) {
        if (i % 2 == 0)
            animals[i] = new Dog();
        else
            animals[i] = new Cat();
    }

    std::cout << "\n=== Populating Brain of first Dog ===\n";
    Dog* d0 = dynamic_cast<Dog*>(animals[0]);
    d0->getBrain()->ideas[0] = "Chase the postman";
    d0->getBrain()->ideas[1] = "Eat the bone";

    std::cout << "\n=== Deep‐copy via copy constructor ===\n";
    Dog* dCopy = new Dog(*d0);
    std::cout << "Original idea[0]: " << d0->getBrain()->ideas[0] << "\n";
    std::cout << "Copied   idea[0]: " << dCopy->getBrain()->ideas[0] << "\n";

    d0->getBrain()->ideas[0] = "Sleep on the sofa";
    std::cout << "\nAfter modifying original:\n";
    std::cout << "Original idea[0]: " << d0->getBrain()->ideas[0] << "\n";
    std::cout << "Copied   idea[0]: " << dCopy->getBrain()->ideas[0] << "\n";

    std::cout << "\n=== Deep‐copy via assignment operator ===\n";
    Dog dAssign;
    dAssign = *d0;
    std::cout << "Assigned idea[1]: " << dAssign.getBrain()->ideas[1] << "\n";
    dAssign.getBrain()->ideas[1] = "Play ball";
    std::cout << "Original idea[1]: " << d0->getBrain()->ideas[1] << "\n";
    std::cout << "Assigned idea[1]: " << dAssign.getBrain()->ideas[1] << "\n";

    std::cout << "\n=== Cleaning up array ===\n";
    for (int i = 0; i < ANIMAL_COUNT; ++i)
        delete animals[i];

    std::cout << "\n=== Cleaning up copies ===\n";
    delete dCopy;

    std::cout << "\n=== Done. Run under Valgrind to confirm zero leaks. ===\n";
    return 0;
}
