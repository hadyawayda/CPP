#include "Fixed.hpp"
#include <cmath>

const int Fixed::_fractionalBits = 8;

Fixed::Fixed() : _raw(0) {}
Fixed::Fixed(const int n)   : _raw(n << _fractionalBits) {}
Fixed::Fixed(const float f) : _raw(static_cast<int>(roundf(f * (1 << _fractionalBits)))) {}
Fixed::Fixed(const Fixed& o) { _raw = o._raw; }
Fixed& Fixed::operator=(const Fixed& o) { if (this!=&o) _raw = o._raw; return *this; }
Fixed::~Fixed() {}

int   Fixed::getRawBits(void) const { return _raw; }
void  Fixed::setRawBits(int const raw) { _raw = raw; }

float Fixed::toFloat(void) const { return static_cast<float>(_raw) / (1 << _fractionalBits); }
int   Fixed::toInt(void)   const { return _raw >> _fractionalBits; }

// comparisons
bool Fixed::operator>(Fixed const& o) const { return _raw >  o._raw; }
bool Fixed::operator<(Fixed const& o) const { return _raw <  o._raw; }
bool Fixed::operator>=(Fixed const& o) const { return _raw >= o._raw; }
bool Fixed::operator<=(Fixed const& o) const { return _raw <= o._raw; }
bool Fixed::operator==(Fixed const& o) const { return _raw == o._raw; }
bool Fixed::operator!=(Fixed const& o) const { return _raw != o._raw; }

// arithmetic
Fixed Fixed::operator+(Fixed const& o) const {
    Fixed r; r._raw = _raw + o._raw; return r;
}
Fixed Fixed::operator-(Fixed const& o) const {
    Fixed r; r._raw = _raw - o._raw; return r;
}
Fixed Fixed::operator*(Fixed const& o) const {
    long long tmp = static_cast<long long>(_raw) * o._raw;
    Fixed r; r._raw = static_cast<int>(tmp >> _fractionalBits);
    return r;
}
Fixed Fixed::operator/(Fixed const& o) const {
    long long num = (static_cast<long long>(_raw) << _fractionalBits);
    Fixed r; r._raw = static_cast<int>(num / o._raw);
    return r;
}

// increment / decrement
Fixed& Fixed::operator++()   { _raw++; return *this; }
Fixed  Fixed::operator++(int){ Fixed tmp(*this); _raw++; return tmp; }
Fixed& Fixed::operator--()   { _raw--; return *this; }
Fixed  Fixed::operator--(int){ Fixed tmp(*this); _raw--; return tmp; }

// min / max
Fixed&       Fixed::min(Fixed& a, Fixed& b)       { return (a < b ? a : b); }
Fixed const& Fixed::min(Fixed const& a, Fixed const& b) { return (a < b ? a : b); }
Fixed&       Fixed::max(Fixed& a, Fixed& b)       { return (a > b ? a : b); }
Fixed const& Fixed::max(Fixed const& a, Fixed const& b) { return (a > b ? a : b); }

std::ostream& operator<<(std::ostream& os, Fixed const& f) {
    os << f.toFloat();
    return os;
}
