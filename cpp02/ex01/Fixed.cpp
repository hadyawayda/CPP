#include "Fixed.hpp"
#include <iostream>
#include <cmath>

const int Fixed::_fractionalBits = 8;

Fixed::Fixed() : _raw(0) {
    std::cout << "Default constructor called" << std::endl;
}

Fixed::Fixed(const int n)
: _raw(n << _fractionalBits)
{
    std::cout << "Int constructor called" << std::endl;
}

Fixed::Fixed(const float f)
: _raw(static_cast<int>(roundf(f * (1 << _fractionalBits))))
{
    std::cout << "Float constructor called" << std::endl;
}

Fixed::Fixed(const Fixed& other) {
    std::cout << "Copy constructor called" << std::endl;
    _raw = other._raw;
}

Fixed& Fixed::operator=(const Fixed& other) {
    std::cout << "Copy assignment operator called" << std::endl;
    if (this != &other)
        _raw = other._raw;
    return *this;
}

Fixed::~Fixed() {
    std::cout << "Destructor called" << std::endl;
}

int Fixed::getRawBits(void) const {
    // no log here per spec
    return _raw;
}

void Fixed::setRawBits(int const raw) {
    _raw = raw;
}

float Fixed::toFloat(void) const {
    return static_cast<float>(_raw) / (1 << _fractionalBits);
}

int Fixed::toInt(void) const {
    return _raw >> _fractionalBits;
}

std::ostream& operator<<(std::ostream& os, const Fixed& f) {
    os << f.toFloat();
    return os;
}
