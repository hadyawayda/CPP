#include "Fixed.hpp"
#include <cmath>

const int Fixed::_fractionalBits = 8;

Fixed::Fixed() : _raw(0) {}
Fixed::Fixed(const int n) : _raw(n << _fractionalBits) {}
Fixed::Fixed(const float f)
    : _raw(static_cast<int>(roundf(f * (1 << _fractionalBits)))) {}
Fixed::Fixed(const Fixed& other) { _raw = other._raw; }
Fixed& Fixed::operator=(const Fixed& other) {
    if (this != &other)
        _raw = other._raw;
    return *this;
}
Fixed::~Fixed() {}

int   Fixed::getRawBits(void)   const { return _raw; }
void  Fixed::setRawBits(int const raw) { _raw = raw; }

float Fixed::toFloat(void) const {
    return static_cast<float>(_raw) / (1 << _fractionalBits);
}
int   Fixed::toInt(void)   const { return _raw >> _fractionalBits; }

std::ostream& operator<<(std::ostream& os, const Fixed& f) {
    os << f.toFloat();
    return os;
}
