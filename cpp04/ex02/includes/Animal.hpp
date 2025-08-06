#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <string>
#include <iostream>

class Animal {
protected:
    std::string type;
public:
    Animal();
    Animal(const Animal& other);
    virtual ~Animal();

    Animal& operator=(const Animal& other);

    const std::string& getType() const;
    virtual void makeSound() const = 0;  // now pure virtual → abstract
};

#endif  // ANIMAL_HPP
