#include "Body.hpp"
#include "Constants.hpp"
#include <cmath>
#include <stdexcept>

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

Eigen::Vector3d gravity_force(
    const Body &b1,
    const Body &b2,
    double g,
    double lj_sigma_scale
) {
    // A gravity force with LJ-like correction to avoid particle overlapping
    // \sigma is proportional to m^1/3
    Eigen::Vector3d direction = b2.X() - b1.X();
    
    // пошло байтоебство для оптимизации
    double r_2 = direction.squaredNorm() + sim::EPSILON;
    double inv_r = 1.0 / std::sqrt(r_2);
    double inv_r_2 = inv_r * inv_r;
    double inv_r_4 = inv_r_2 * inv_r_2;
    double inv_r_8 = inv_r_4 * inv_r_4;
    double inv_r_12 = inv_r_4 * inv_r_8;
    // отставить байтоебство
    double sigma = lj_sigma_scale * std::pow(b1.m() + b2.m(), 1.0 / 3.0);
    
    Eigen::Vector3d f = direction * (
        g * b1.m() * b2.m() * inv_r * inv_r_2 // Gravity
        - sigma * inv_r * inv_r_12
    );
    return f;
}


} // namespace gravity