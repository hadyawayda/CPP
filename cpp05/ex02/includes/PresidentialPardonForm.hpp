#ifndef PRESIDENTIALPARDONFORM_HPP
#define PRESIDENTIALPARDONFORM_HPP

#include "AForm.hpp"
#include <string>

class PresidentialPardonForm : public AForm {
public:
    PresidentialPardonForm(const std::string& target);
    PresidentialPardonForm(const PresidentialPardonForm& o);
    PresidentialPardonForm& operator=(const PresidentialPardonForm& o);
    ~PresidentialPardonForm();

protected:
    virtual void executeAction() const;

private:
    const std::string target_;
};

#endif // PRESIDENTIALPARDONFORM_HPP
