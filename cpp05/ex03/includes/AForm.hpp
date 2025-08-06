#ifndef AFORM_HPP
#define AFORM_HPP

#include <string>
#include <exception>
#include <iostream>
class Bureaucrat;

class AForm {
public:
    struct GradeTooHighException : public std::exception {
        virtual const char* what() const throw();
    };
    struct GradeTooLowException  : public std::exception {
        virtual const char* what() const throw();
    };
    struct NotSignedException    : public std::exception {
        virtual const char* what() const throw();
    };

    AForm(const std::string& name, int gradeSign, int gradeExec);
    AForm(const AForm& o);
    AForm& operator=(const AForm& o);
    virtual ~AForm();

    const std::string& getName() const;
    bool                isSigned() const;
    int                 getGradeSign() const;
    int                 getGradeExec() const;

    void beSigned(Bureaucrat const & b);
    void execute(Bureaucrat const & executor) const;

protected:
    virtual void executeAction() const = 0;

private:
    const std::string name_;
    bool              signed_;
    const int         gradeSign_;
    const int         gradeExec_;
};

std::ostream& operator<<(std::ostream& os, const AForm& f);

#endif // AFORM_HPP
