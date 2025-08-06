#include "ShrubberyCreationForm.hpp"
#include <fstream>
#include <iostream>

ShrubberyCreationForm::ShrubberyCreationForm(const std::string& t)
  : AForm("ShrubberyCreationForm", 145, 137), target_(t)
{
    std::cout << "[Shrubbery] ctor for " << target_ << "\n";
}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm& o)
  : AForm(o), target_(o.target_)
{
    std::cout << "[Shrubbery] copy ctor\n";
}

ShrubberyCreationForm& ShrubberyCreationForm::operator=(const ShrubberyCreationForm& o) {
    AForm::operator=(o);
    // target_ is const
    return *this;
}

ShrubberyCreationForm::~ShrubberyCreationForm() {
    std::cout << "[Shrubbery] dtor for " << target_ << "\n";
}

void ShrubberyCreationForm::executeAction() const {
    std::string filename = target_ + "_shrubbery";
    std::ofstream ofs(filename.c_str());
    if (!ofs) {
        std::cerr << "Failed to open file\n";
        return;
    }
    ofs << "   _-_\n"
           " /~~   ~~\\\n"
           "{           }\n"
           " \\  _- _- /\n"
           "   ~~   ~~\n";
}
