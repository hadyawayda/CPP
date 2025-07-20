#ifndef POINT_HPP
#define POINT_HPP

#include "Fixed.hpp"

class Point {
private:
    const Fixed _x;
    const Fixed _y;

public:
    Point();                                // default → (0,0)
    Point(float x, float y);                // from floats
    Point(const Point& other);              // copy
    Point& operator=(const Point& other);   // assignment (no-op for const members)
    ~Point();

    Fixed getX() const;
    Fixed getY() const;
};

#endif  // POINT_HPP
