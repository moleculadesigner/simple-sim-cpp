#include "System.hpp"
#include "Body.hpp"
#include "Eigen/src/Core/Matrix.h"
#include <random>
#include <Eigen/Dense>
#include <iostream>
#include <ostream>

namespace sim {
System::System(size_t n_particles) {
    particles_.reserve(n_particles);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> pos_dis(-100.0, 100.0);
    std::uniform_real_distribution<double> mass_dis(1.0, 10.0);

    for (size_t i = 0; i < n_particles; ++i) {
        particles_.emplace_back(
            pos_dis(gen), pos_dis(gen), pos_dis(gen), // x, y, z
            mass_dis(gen)                             // mass
        );
    }
}

System::System(const std::vector<Body>& particles) : particles_{particles} {};

System::System(
    const std::vector<Eigen::Vector3d>& points,
    const std::vector<double>& masses
) {
    if (points.size() != masses.size()) {
        throw std::invalid_argument(
            "Number of points must be equal to number of masses."
        );
    }
    particles_.reserve(points.size());
    for (size_t i = 0; i < points.size(); ++i) {
        particles_.emplace_back(
            points[i],
            masses[i]
        );
    }
}

void System::add_particle(const Body& particle) {
    particles_.push_back(particle);
}
Body& System::particle(size_t n) {
    if (n >= particles_.size()) {
        throw std::invalid_argument("Particle index is out of range");
    }
    return particles_[n];
}
const Body& System::particle(size_t n) const {
    if (n >= particles_.size()) {
        throw std::invalid_argument("Particle index is out of range");
    }
    return particles_[n];
}

double System::T() const {
    double energy = 0.0;
    for (auto p: particles_) {
        energy += 0.5 * p.m() * p.V().squaredNorm();
    }
    return energy;
}

double System::U() const {
    double energy = 0.0;
    //for (auto& p : particles_) p.reset_force();
    for (size_t i = 0; i < n_particles(); ++i) {
        const Body& particle = particles_[i];
        for (size_t j = i + 1; j < n_particles(); ++j) {
            const Body& other = particles_[j];
            float f = gravity_force(
                particle, other,
                g_, ljs_
            ).norm();
            float r = (other.X() - particle.X()).norm();
            energy -= r * f;
        }
    }
    return energy;
}

void System::calculate_forces() {
    for (auto& p : particles_) p.reset_force();

    for (size_t i = 0; i < n_particles(); ++i) {
        Body& particle = particles_[i];
        for (size_t j = i + 1; j < n_particles(); ++j) {
            Body& other = particles_[j];
            Eigen::Vector3d df = gravity_force(
                particle, other,
                g_, ljs_
            );
            particle.F(particle.F() + df);
            other.F(other.F() - df);
        }
    }
}

void System::step(double dt) {
    for (auto& p : particles_) {
        p.move(p.V() * dt + p.a() * dt * dt * 0.5);
        p.accelerate(0.5 * p.a() * dt); // половина скорости
    }
    calculate_forces();
    for (auto& p : particles_) {
        p.accelerate(0.5 * p.a() * dt); // вторая половина
    }
}

Eigen::Vector3d System::com() const {
    Eigen::Vector3d com = Eigen::Vector3d::Zero();
    float total_mass = 0.0;
    for (auto p: particles_) {
        total_mass += p.m();
    }
    for (auto p: particles_) {
        com += p.m() * p.X();
    }
    return com / total_mass;
}

void System::recenter() {
    auto com = this->com();
    for (auto p: particles_) {
        p.X(p.X() - com);
    }
}



} // namespace sim