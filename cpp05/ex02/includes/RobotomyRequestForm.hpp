#ifndef ROBOTOMYREQUESTFORM_HPP
#define ROBOTOMYREQUESTFORM_HPP

#include "AForm.hpp"
#include <string>

class RobotomyRequestForm : public AForm {
public:
    RobotomyRequestForm(const std::string& target);
    RobotomyRequestForm(const RobotomyRequestForm& o);
    RobotomyRequestForm& operator=(const RobotomyRequestForm& o);
    ~RobotomyRequestForm();

protected:
    virtual void executeAction() const;

private:
    const std::string target_;
};

#endif // ROBOTOMYREQUESTFORM_HPP
