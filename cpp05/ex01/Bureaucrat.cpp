#include "Bureaucrat.hpp"

// --- Exception messages ---
const char* Bureaucrat::GradeTooHighException::what() const throw() {
    return "Grade too high";
}

const char* Bureaucrat::GradeTooLowException::what() const throw() {
    return "Grade too low";
}

// --- Constructors / Destructor ---
Bureaucrat::Bureaucrat(const std::string& name, int grade)
    : name_(name)
    , grade_(grade)
{
    if (grade_ < 1)   throw GradeTooHighException();
    if (grade_ > 150) throw GradeTooLowException();
    std::cout << "[Bureaucrat] " << name_ 
              << " constructed with grade " << grade_ << "\n";
}

Bureaucrat::Bureaucrat(const Bureaucrat& other)
    : name_(other.name_)
    , grade_(other.grade_)
{
    std::cout << "[Bureaucrat] copy constructor for " << name_ << "\n";
}

Bureaucrat& Bureaucrat::operator=(const Bureaucrat& other) {
    std::cout << "[Bureaucrat] assignment operator for " << other.name_ << "\n";
    if (this != &other) {
        // name_ is const, so we only copy the grade
        grade_ = other.grade_;
    }
    return *this;
}

Bureaucrat::~Bureaucrat() {
    std::cout << "[Bureaucrat] " << name_ << " destructed\n";
}

// --- Accessors ---
const std::string& Bureaucrat::getName() const {
    return name_;
}

int Bureaucrat::getGrade() const {
    return grade_;
}

// --- Modifiers ---
void Bureaucrat::incrementGrade() {
    if (grade_ <= 1)   throw GradeTooHighException();
    --grade_;
}

void Bureaucrat::decrementGrade() {
    if (grade_ >= 150) throw GradeTooLowException();
    ++grade_;
}

// --- operator<< ---
std::ostream& operator<<(std::ostream& os, const Bureaucrat& b) {
    os << b.getName()
       << ", bureaucrat grade "
       << b.getGrade();
    return os;
}

// signForm implementation
void Bureaucrat::signForm(Form& f) const {
    try {
        f.beSigned(*this);
        std::cout << name_
                  << " signed " << f.getName() << "\n";
    }
    catch (std::exception& e) {
        std::cout << name_
                  << " couldn’t sign " << f.getName()
                  << " because " << e.what() << "\n";
    }
}