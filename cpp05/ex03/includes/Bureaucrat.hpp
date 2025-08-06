#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <string>
#include <exception>
#include <iostream>
class AForm;

class Bureaucrat {
public:
    struct GradeTooHighException : public std::exception {
        virtual const char* what() const throw();
    };
    struct GradeTooLowException : public std::exception {
        virtual const char* what() const throw();
    };

    Bureaucrat(const std::string& name, int grade);
    Bureaucrat(const Bureaucrat& other);
    Bureaucrat& operator=(const Bureaucrat& other);
    ~Bureaucrat();

    const std::string& getName() const;
    int                 getGrade() const;

    void incrementGrade();
    void decrementGrade();

    void signForm(AForm& f) const;
    void executeForm(AForm const & form) const;

private:
    const std::string name_;
    int               grade_;
};

std::ostream& operator<<(std::ostream& os, const Bureaucrat& b);

#endif // BUREAUCRAT_HPP
