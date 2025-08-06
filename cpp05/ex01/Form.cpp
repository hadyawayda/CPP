#include "Form.hpp"
#include "Bureaucrat.hpp"

// --- Exception Messages ---
const char* Form::GradeTooHighException::what() const throw() {
    return "Form grade too high";
}
const char* Form::GradeTooLowException::what() const throw() {
    return "Form grade too low";
}

// --- Constructors / Destructor ---
Form::Form(const std::string& name, int gs, int ge)
    : name_(name)
    , signed_(false)
    , gradeSign_(gs)
    , gradeExec_(ge)
{
    if (gradeSign_ < 1 || gradeExec_ < 1)
        throw GradeTooHighException();
    if (gradeSign_ > 150 || gradeExec_ > 150)
        throw GradeTooLowException();
    std::cout << "[Form] " << name_
              << " constructed (sign " << gradeSign_
              << ", exec " << gradeExec_ << ")\n";
}

Form::Form(const Form& other)
    : name_(other.name_)
    , signed_(other.signed_)
    , gradeSign_(other.gradeSign_)
    , gradeExec_(other.gradeExec_)
{
    std::cout << "[Form] copy constructor for " << name_ << "\n";
}

Form& Form::operator=(const Form& other) {
    std::cout << "[Form] assignment operator for " << other.name_ << "\n";
    if (this != &other) {
        // name_, gradeSign_, gradeExec_ are const → only copy signed status
        signed_ = other.signed_;
    }
    return *this;
}

Form::~Form() {
    std::cout << "[Form] " << name_ << " destructed\n";
}

// --- Getters ---
const std::string& Form::getName() const    { return name_; }
bool                Form::isSigned() const  { return signed_; }
int                 Form::getGradeSign() const { return gradeSign_; }
int                 Form::getGradeExec() const { return gradeExec_; }

// --- beSigned ---
void Form::beSigned(const Bureaucrat& b) {
    if (b.getGrade() > gradeSign_)
        throw GradeTooLowException();
    signed_ = true;
}

// --- operator<< ---
std::ostream& operator<<(std::ostream& os, const Form& f) {
    os << f.getName()
       << ", sign grade "  << f.getGradeSign()
       << ", exec grade "  << f.getGradeExec()
       << ", signed: "     << (f.isSigned()? "yes":"no");
    return os;
}
