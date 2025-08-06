#ifndef CAT_HPP
#define CAT_HPP

#include "Animal.hpp"
#include "Brain.hpp"

class Cat : public Animal {
private:
    Brain* brain;
public:
    Cat();
    Cat(const Cat& other);
    virtual ~Cat();

    Cat& operator=(const Cat& other);

    virtual void makeSound() const;
    Brain*   getBrain() const;
};

#endif  // CAT_HPP
