#ifndef INTERN_HPP
#define INTERN_HPP

#include <string>
#include "AForm.hpp"

class Intern {
public:
    Intern();
    Intern(const Intern& other);
    Intern& operator=(const Intern& other);
    ~Intern();

    // formName: one of "shrubbery creation", "robotomy request", "presidential pardon"
    // target:    whatever you like
    // returns new AForm* or nullptr on unknown type
    AForm* makeForm(const std::string& formName,
                    const std::string& target) const;
};

#endif  // INTERN_HPP
