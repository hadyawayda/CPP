#include "AForm.hpp"
#include "Bureaucrat.hpp"

// --- Exceptions ---
const char* AForm::GradeTooHighException::what() const throw() {
    return "Form grade too high";
}
const char* AForm::GradeTooLowException::what() const throw() {
    return "Form grade too low";
}
const char* AForm::NotSignedException::what() const throw() {
    return "Form not signed";
}

// --- Ctor / Dtor ---
AForm::AForm(const std::string& name, int gs, int ge)
  : name_(name), signed_(false), gradeSign_(gs), gradeExec_(ge)
{
    if (gradeSign_ < 1 || gradeExec_ < 1)   throw GradeTooHighException();
    if (gradeSign_ > 150 || gradeExec_ > 150) throw GradeTooLowException();
    std::cout << "[AForm] " << name_ << " constructed\n";
}

AForm::AForm(const AForm& o)
  : name_(o.name_), signed_(o.signed_),
    gradeSign_(o.gradeSign_), gradeExec_(o.gradeExec_)
{
    std::cout << "[AForm] copy ctor for " << name_ << "\n";
}

AForm& AForm::operator=(const AForm& o) {
    std::cout << "[AForm] assign op for " << o.name_ << "\n";
    if (this != &o) {
        signed_ = o.signed_;
        // name_, grades are const
    }
    return *this;
}

AForm::~AForm() {
    std::cout << "[AForm] " << name_ << " destructed\n";
}

// --- Accessors ---
const std::string& AForm::getName()      const { return name_; }
bool                AForm::isSigned()    const { return signed_; }
int                 AForm::getGradeSign()const { return gradeSign_; }
int                 AForm::getGradeExec()const { return gradeExec_; }

// --- beSigned ---
void AForm::beSigned(Bureaucrat const & b) {
    if (b.getGrade() > gradeSign_) throw GradeTooLowException();
    signed_ = true;
}

// --- execute ---
void AForm::execute(Bureaucrat const & executor) const {
    if (!signed_)             throw NotSignedException();
    if (executor.getGrade() > gradeExec_) throw GradeTooLowException();
    executeAction();
}

// --- operator<< ---
std::ostream& operator<<(std::ostream& os, const AForm& f) {
    os << f.getName()
       << " [signed="   << (f.isSigned()? "yes":"no")
       << ", sign≤"    << f.getGradeSign()
       << ", exec≤"    << f.getGradeExec() << "]";
    return os;
}
