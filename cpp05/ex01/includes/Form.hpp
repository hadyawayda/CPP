#ifndef FORM_HPP
#define FORM_HPP

#include <string>
#include <exception>
#include <iostream>
class Bureaucrat;  // forward

class Form {
public:
    // Exceptions
    struct GradeTooHighException : public std::exception {
        virtual const char* what() const throw();
    };
    struct GradeTooLowException : public std::exception {
        virtual const char* what() const throw();
    };

    // Canonical form
    Form(const std::string& name, int gradeSign, int gradeExec);
    Form(const Form& other);
    Form& operator=(const Form& other);
    ~Form();

    // Getters
    const std::string& getName() const;
    bool                isSigned() const;
    int                 getGradeSign() const;
    int                 getGradeExec() const;

    // Member
    void beSigned(const Bureaucrat& b);

private:
    const std::string name_;
    bool              signed_;
    const int         gradeSign_;
    const int         gradeExec_;
};

// operator<<
std::ostream& operator<<(std::ostream& os, const Form& f);

#endif  // FORM_HPP
