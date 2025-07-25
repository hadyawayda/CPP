#include "Point.hpp"

Point::Point() : _x(0), _y(0) {}

Point::Point(float x, float y)
    : _x(x), _y(y) {}

Point::Point(const Point& other)
    : _x(other._x), _y(other._y) {}

Point& Point::operator=(const Point&) {
    // _x and _y are const → no change possible
    return *this;
}

Point::~Point() {}

Fixed Point::getX() const { return _x; }
Fixed Point::getY() const { return _y; }
