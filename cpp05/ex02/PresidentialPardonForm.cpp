#include "PresidentialPardonForm.hpp"
#include <iostream>

PresidentialPardonForm::PresidentialPardonForm(const std::string& t)
  : AForm("PresidentialPardonForm", 25, 5), target_(t)
{
    std::cout << "[Pardon] ctor for " << target_ << "\n";
}

PresidentialPardonForm::PresidentialPardonForm(const PresidentialPardonForm& o)
  : AForm(o), target_(o.target_)
{
    std::cout << "[Pardon] copy ctor\n";
}

PresidentialPardonForm& PresidentialPardonForm::operator=(const PresidentialPardonForm& o) {
    AForm::operator=(o);
    return *this;
}

PresidentialPardonForm::~PresidentialPardonForm() {
    std::cout << "[Pardon] dtor for " << target_ << "\n";
}

void PresidentialPardonForm::executeAction() const {
    std::cout << target_ << " has been pardoned by Zaphod Beeblebrox\n";
}
