#include <iostream>
#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

int main() {
    try {
        Bureaucrat super("Super", 1);
        Bureaucrat low("Lowbie", 150);

        ShrubberyCreationForm shrub("home");
        RobotomyRequestForm   robo("Bender");
        PresidentialPardonForm pardon("Marvin");

        std::cout << shrub   << "\n"
                  << robo    << "\n"
                  << pardon  << "\n\n";

        low.signForm(shrub);     // too low to sign
        super.signForm(shrub);   // ok
        super.executeForm(shrub);// ok writes file

        super.signForm(robo);
        super.executeForm(robo);

        super.signForm(pardon);
        super.executeForm(pardon);

        std::cout << "\n--- low tries execute only-signed ---\n";
        low.signForm(robo);      // too low
        super.signForm(robo);
        low.executeForm(robo);   // signed but low grade to exec

    } catch (std::exception& e) {
        std::cerr << "Fatal: " << e.what() << "\n";
    }

    return 0;
}
