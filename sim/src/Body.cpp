#include "Body.hpp"
#include <cmath>
#include <array>

namespace sim {
Body::Body(std::array<double, 3> &coords, double mass) {
    this->x = coords[0];
    this->y = coords[1];
    this->z = coords[2];
    this->m = mass;
}

std::array<double, 3> Body::X() {
    return std::array<double, 3>{
        this->x,
        this->y,
        this->z
    };
}

double Body::M() {
    return this->m;
}
    
double Body::sq_dist_to(Body &other) {
    return std::pow(this->x - other.x, 2)
         + std::pow(this->y - other.y, 2)
         + std::pow(this->z - other.z, 2);
}

double gravity_force(Body &b1, Body &b2) {
    return G * b1.M() * b2.M() / b1.sq_dist_to(b2);
}

} // namespace gravity