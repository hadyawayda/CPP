#include "Zombie.hpp"
#include <iostream>

int main() {
    int N = 5;
    Zombie* h = zombieHorde(N, "HordeZombie");
    if (!h) return 1;
    for (int i = 0; i < N; ++i)
        h[i].announce();
    delete[] h;
    return 0;
}
