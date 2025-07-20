#ifndef FIXED_HPP
#define FIXED_HPP

#include <iostream>

class Fixed {
private:
    int                 _raw;
    static const int    _fractionalBits;

public:
    Fixed();
    Fixed(const int n);
    Fixed(const float f);
    Fixed(const Fixed& other);
    Fixed& operator=(const Fixed& other);
    ~Fixed();

    int     getRawBits(void) const;
    void    setRawBits(int const raw);

    float   toFloat(void) const;
    int     toInt(void) const;

    // comparison
    bool operator>(Fixed const& o) const;
    bool operator<(Fixed const& o) const;
    bool operator>=(Fixed const& o) const;
    bool operator<=(Fixed const& o) const;
    bool operator==(Fixed const& o) const;
    bool operator!=(Fixed const& o) const;

    // arithmetic
    Fixed operator+(Fixed const& o) const;
    Fixed operator-(Fixed const& o) const;
    Fixed operator*(Fixed const& o) const;
    Fixed operator/(Fixed const& o) const;

    // increment / decrement
    Fixed& operator++();    // pre-increment
    Fixed  operator++(int); // post-increment
    Fixed& operator--();    // pre-decrement
    Fixed  operator--(int); // post-decrement

    // min / max
    static Fixed&       min(Fixed& a, Fixed& b);
    static Fixed const& min(Fixed const& a, Fixed const& b);
    static Fixed&       max(Fixed& a, Fixed& b);
    static Fixed const& max(Fixed const& a, Fixed const& b);
};

std::ostream& operator<<(std::ostream& os, Fixed const& f);

#endif  // FIXED_HPP
