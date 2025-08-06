#include "Intern.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include <iostream>

// helper functions
static AForm* makeShrub(const std::string& target) {
    return new ShrubberyCreationForm(target);
}
static AForm* makeRobot(const std::string& target) {
    return new RobotomyRequestForm(target);
}
static AForm* makePardon(const std::string& target) {
    return new PresidentialPardonForm(target);
}

Intern::Intern() {}
Intern::Intern(const Intern& other) { (void)other; }
Intern& Intern::operator=(const Intern& other) {
    (void)other;
    return *this;
}
Intern::~Intern() {}

AForm* Intern::makeForm(const std::string& formName,
                        const std::string& target) const
{
    // lookup table
    struct Entry { const char* name; AForm* (*fn)(const std::string&); };
    static const Entry table[] = {
        { "shrubbery creation",   &makeShrub },
        { "robotomy request",     &makeRobot },
        { "presidential pardon",  &makePardon },
    };

    for (size_t i = 0; i < sizeof(table)/sizeof(*table); ++i) {
        if (formName == table[i].name) {
            AForm* f = table[i].fn(target);
            std::cout << "Intern creates " << formName << std::endl;
            return f;
        }
    }
    std::cout << "Intern could not create \"" << formName
              << "\": unknown form type\n";
    return NULL;
}
