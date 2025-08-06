#include "RobotomyRequestForm.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

RobotomyRequestForm::RobotomyRequestForm(const std::string& t)
  : AForm("RobotomyRequestForm", 72, 45), target_(t)
{
    std::cout << "[Robotomy] ctor for " << target_ << "\n";
    std::srand(std::time(NULL));
}

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm& o)
  : AForm(o), target_(o.target_)
{
    std::cout << "[Robotomy] copy ctor\n";
}

RobotomyRequestForm& RobotomyRequestForm::operator=(const RobotomyRequestForm& o) {
    AForm::operator=(o);
    return *this;
}

RobotomyRequestForm::~RobotomyRequestForm() {
    std::cout << "[Robotomy] dtor for " << target_ << "\n";
}

void RobotomyRequestForm::executeAction() const {
    std::cout << "Bzzzz... drilling noises...\n";
    if (std::rand() % 2)
        std::cout << target_ << " has been robotomized successfully\n";
    else
        std::cout << target_ << " robotomy failed\n";
}
