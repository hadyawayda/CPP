#include "Zombie.hpp"

int main() {
    Zombie *h1 = newZombie("HeapZombie");
    h1->announce();
    delete h1;

    randomChump("StackZombie");
    return 0;
}
