#include "Body.hpp"
//#include <cmath>
#include <array>
#include <stdexcept>
#include <algorithm>
#include <string>

namespace sim {

Body::Body(const Eigen::Vector3d& vec, double m) :
    coord_{vec},
    m_{m}
    {
        if (m_ <= 0) throw std::invalid_argument("Mass must be positive");
    }

Body::Body(double x, double y, double z, double m) :
    Body(Eigen::Vector3d{x, y, z}, m) {}

// Set
void Body::m(double mass) {
    if (mass <= 0) throw std::invalid_argument("Mass must be positive");
    m_ = mass;
}

// Measure
double Body::sq_dist_to(const Body& other) const {
    return (coord_ - other.coord_).squaredNorm();
}

// Transform
void Body::move(const Eigen::Vector3d& dx) {
    coord_ += dx;
}

void Body::accelerate(const Eigen::Vector3d& dv) {
    vel_ += dv;
}

void Body::reset_force() {
    f_ = Eigen::Vector3d::Zero();
}

std::string Body::show_xyz() const {
    std::string xyz_entry;
    xyz_entry = "P " 
        + std::to_string(coord_[0]) + " "
        + std::to_string(coord_[1]) + " "
        + std::to_string(coord_[2]) + " ";
    return xyz_entry;
}

double gravity_force(Body &b1, Body &b2, double G) {
    return G * b1.m() * b2.m() / std::max(eps, b1.sq_dist_to(b2));
}


} // namespace gravity