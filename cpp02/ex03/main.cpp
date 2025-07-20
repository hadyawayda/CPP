#include <iostream>
#include <iomanip>
#include "Point.hpp"

// Declare bsp so main can call it:
bool bsp(Point const a, Point const b, Point const c, Point const point);

int main() {
    Point a(0.0f,  0.0f);
    Point b(10.0f, 0.0f);
    Point c(0.0f, 10.0f);

    Point inside(1.0f,  1.0f);
    Point outside(10.0f, 10.0f);
    Point onEdge(0.0f,   5.0f);
    Point onVertex(0.0f, 0.0f);

    std::cout << std::boolalpha;
    std::cout << "Inside  (1,1)   : " << bsp(a,b,c,inside)  << '\n';  // true
    std::cout << "Outside (10,10) : " << bsp(a,b,c,outside) << '\n';  // false
    std::cout << "On edge (0,5)   : " << bsp(a,b,c,onEdge)  << '\n';  // false
    std::cout << "On vertex (0,0): " << bsp(a,b,c,onVertex) << '\n';  // false
    return 0;
}
