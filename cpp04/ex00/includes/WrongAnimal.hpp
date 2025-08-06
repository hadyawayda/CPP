#ifndef WRONGANIMAL_HPP
#define WRONGANIMAL_HPP

#include <string>
#include <iostream>

class WrongAnimal {
protected:
    std::string type;
public:
    WrongAnimal();
    WrongAnimal(const WrongAnimal& other);
    ~WrongAnimal();

    WrongAnimal& operator=(const WrongAnimal& other);

    const std::string& getType() const;
    // note: makeSound is NOT virtual here!
    void makeSound() const;
};

#endif  // WRONGANIMAL_HPP
