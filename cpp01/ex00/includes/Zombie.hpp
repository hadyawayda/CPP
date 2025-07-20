#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include <string>

class Zombie {
private:
    std::string name_;
public:
    Zombie();
    Zombie(const std::string &name);
    ~Zombie();
    void announce() const;
    void setName(const std::string &name);
};

Zombie* newZombie(std::string name);
void    randomChump(std::string name);

#endif  // ZOMBIE_HPP
