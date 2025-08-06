#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <string>
#include <exception>
#include <iostream>
#include "Form.hpp"

class Bureaucrat {
public:
    // Exceptions (as before) …
    struct GradeTooHighException : public std::exception {
        virtual const char* what() const throw();
    };
    struct GradeTooLowException : public std::exception {
        virtual const char* what() const throw();
    };

    // Canonical form
    Bureaucrat(const std::string& name, int grade);
    Bureaucrat(const Bureaucrat& other);
    Bureaucrat& operator=(const Bureaucrat& other);
    ~Bureaucrat();

    // Accessors
    const std::string& getName() const;
    int                 getGrade() const;

    // Modifiers
    void incrementGrade();
    void decrementGrade();

    // New:
    void signForm(Form& f) const;

private:
    const std::string name_;
    int               grade_;
};

std::ostream& operator<<(std::ostream& os, const Bureaucrat& b);

#endif  // BUREAUCRAT_HPP
