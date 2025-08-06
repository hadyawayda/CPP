#include "Brain.hpp"

Brain::Brain() {
    std::cout << "[Brain] default constructor called\n";
}

Brain::Brain(const Brain& other) {
    std::cout << "[Brain] copy constructor called\n";
    for (int i = 0; i < 100; ++i)
        ideas[i] = other.ideas[i];
}

Brain::~Brain() {
    std::cout << "[Brain] destructor called\n";
}

Brain& Brain::operator=(const Brain& other) {
    std::cout << "[Brain] assignment operator called\n";
    if (this != &other) {
        for (int i = 0; i < 100; ++i)
            ideas[i] = other.ideas[i];
    }
    return *this;
}
