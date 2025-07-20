#include "Point.hpp"
#include <cmath>

// Cross product helper: (b - a) × (c - a)
static float cross(const Point& a, const Point& b, const Point& c) {
    float x1 = b.getX().toFloat() - a.getX().toFloat();
    float y1 = b.getY().toFloat() - a.getY().toFloat();
    float x2 = c.getX().toFloat() - a.getX().toFloat();
    float y2 = c.getY().toFloat() - a.getY().toFloat();
    return x1 * y2 - y1 * x2;
}

bool bsp(Point const a, Point const b, Point const c, Point const p) {
    // For each edge, check that p is strictly on the same side as the opposite vertex.
    float cp_ab_p = cross(a, b, p);
    float cp_ab_c = cross(a, b, c);
    if (cp_ab_p == 0 || (cp_ab_p > 0) != (cp_ab_c > 0)) return false;

    float cp_bc_p = cross(b, c, p);
    float cp_bc_a = cross(b, c, a);
    if (cp_bc_p == 0 || (cp_bc_p > 0) != (cp_bc_a > 0)) return false;

    float cp_ca_p = cross(c, a, p);
    float cp_ca_b = cross(c, a, b);
    if (cp_ca_p == 0 || (cp_ca_p > 0) != (cp_ca_b > 0)) return false;

    return true;
}
