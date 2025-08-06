#include "Bureaucrat.hpp"
#include "AForm.hpp"

// --- Exceptions ---
const char* Bureaucrat::GradeTooHighException::what() const throw() {
    return "Grade too high";
}
const char* Bureaucrat::GradeTooLowException::what() const throw() {
    return "Grade too low";
}

// --- Constructors / Destructor ---
Bureaucrat::Bureaucrat(const std::string& name, int grade)
    : name_(name), grade_(grade)
{
    if (grade_ < 1)   throw GradeTooHighException();
    if (grade_ > 150) throw GradeTooLowException();
    std::cout << "[Bureaucrat] " << name_ << " constructed, grade " << grade_ << "\n";
}

Bureaucrat::Bureaucrat(const Bureaucrat& o)
    : name_(o.name_), grade_(o.grade_)
{
    std::cout << "[Bureaucrat] copy ctor for " << name_ << "\n";
}

Bureaucrat& Bureaucrat::operator=(const Bureaucrat& o) {
    std::cout << "[Bureaucrat] assign op for " << o.name_ << "\n";
    if (this != &o)
        grade_ = o.grade_;
    return *this;
}

Bureaucrat::~Bureaucrat() {
    std::cout << "[Bureaucrat] " << name_ << " destructed\n";
}

// --- Accessors ---
const std::string& Bureaucrat::getName() const { return name_; }
int                 Bureaucrat::getGrade() const { return grade_; }

// --- Modifiers ---
void Bureaucrat::incrementGrade() {
    if (grade_ <= 1) throw GradeTooHighException();
    --grade_;
}
void Bureaucrat::decrementGrade() {
    if (grade_ >= 150) throw GradeTooLowException();
    ++grade_;
}

// --- signForm ---
#include "AForm.hpp"
void Bureaucrat::signForm(AForm& f) const {
    try {
        f.beSigned(*this);
        std::cout << name_ << " signed " << f.getName() << "\n";
    } catch (std::exception& e) {
        std::cout << name_ << " couldn’t sign " << f.getName()
                  << " because " << e.what() << "\n";
    }
}

// --- executeForm ---
void Bureaucrat::executeForm(AForm const & form) const {
    try {
        form.execute(*this);
        std::cout << name_ << " executed " << form.getName() << "\n";
    } catch (std::exception& e) {
        std::cout << name_ << " couldn’t execute " << form.getName()
                  << " because " << e.what() << "\n";
    }
}

// --- operator<< ---
std::ostream& operator<<(std::ostream& os, const Bureaucrat& b) {
    os << b.getName() << ", bureaucrat grade " << b.getGrade();
    return os;
}
