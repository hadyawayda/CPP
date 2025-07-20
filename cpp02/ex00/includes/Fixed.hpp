#ifndef FIXED_HPP
#define FIXED_HPP

#include <iostream>

class Fixed {
private:
    int                 _raw;
    static const int    _fractionalBits;

public:
    Fixed();                                    // default
    Fixed(const Fixed& other);                  // copy
    Fixed& operator=(const Fixed& other);       // copy-assign
    ~Fixed();                                   // destructor

    int     getRawBits(void) const;
    void    setRawBits(int const raw);
};

#endif  // FIXED_HPP
