#include "Sed.hpp"
#include <iostream>

int main(int ac, char **av) {
    if (ac != 4) {
        std::cerr << "Usage: " << av[0] << " filename s1 s2\n";
        return 1;
    }
    std::string infile(av[1]);
    std::string s1(av[2]);
    std::string s2(av[3]);

    Sed sed(infile);
    sed.replace(s1, s2);
    sed.writeTo(infile + ".replace");
    return 0;
}
