#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <string>
#include <iostream>

class Brain {
public:
    std::string ideas[100];

    Brain();
    Brain(const Brain& other);
    ~Brain();

    Brain& operator=(const Brain& other);
};

#endif  // BRAIN_HPP
