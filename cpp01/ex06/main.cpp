#include "Harl.hpp"
#include <iostream>

int main(int ac, char **av) {
    if (ac != 2) {
        std::cout << "[ Probably complaining about insignificant problems ]\n";
        return 1;
    }
    std::string level(av[1]);
    Harl harl;

    enum Level { DEBUG=0, INFO, WARNING, ERROR, UNKNOWN };
    Level lvl = UNKNOWN;
    if (level == "DEBUG")       lvl = DEBUG;
    else if (level == "INFO")    lvl = INFO;
    else if (level == "WARNING") lvl = WARNING;
    else if (level == "ERROR")   lvl = ERROR;

    switch (lvl) {
        case DEBUG:
            harl.complain("DEBUG");
            /* fall through */
        case INFO:
            harl.complain("INFO");
            /* fall through */
        case WARNING:
            harl.complain("WARNING");
            /* fall through */
        case ERROR:
            harl.complain("ERROR");
            break;
        default:
            std::cout << "[ Probably complaining about insignificant problems ]\n";
    }
    return 0;
}
