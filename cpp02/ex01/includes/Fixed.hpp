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
};

std::ostream& operator<<(std::ostream& os, const Fixed& f);

#endif  // FIXED_HPP
