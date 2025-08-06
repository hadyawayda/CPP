#include <iostream>
#include "Intern.hpp"
#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

int main() {
    Intern someRandomIntern;
    AForm* f1 = someRandomIntern.makeForm("robotomy request", "Bender");
    AForm* f2 = someRandomIntern.makeForm("shrubbery creation", "Home");
    AForm* f3 = someRandomIntern.makeForm("presidential pardon", "Marvin");
    AForm* f4 = someRandomIntern.makeForm("unknown form", "Nobody");

    std::cout << "\n-- Testing forms with a top-grade Bureaucrat --\n";
    Bureaucrat boss("Boss", 1);
    if (f1) { boss.signForm(*f1); boss.executeForm(*f1); }
    if (f2) { boss.signForm(*f2); boss.executeForm(*f2); }
    if (f3) { boss.signForm(*f3); boss.executeForm(*f3); }

    // cleanup
    delete f1;
    delete f2;
    delete f3;
    delete f4;

    return 0;
}
