#ifndef SHRUBBERYCREATIONFORM_HPP
#define SHRUBBERYCREATIONFORM_HPP

#include "AForm.hpp"
#include <string>

class ShrubberyCreationForm : public AForm {
public:
    ShrubberyCreationForm(const std::string& target);
    ShrubberyCreationForm(const ShrubberyCreationForm& o);
    ShrubberyCreationForm& operator=(const ShrubberyCreationForm& o);
    ~ShrubberyCreationForm();

protected:
    virtual void executeAction() const;

private:
    const std::string target_;
};

#endif // SHRUBBERYCREATIONFORM_HPP
