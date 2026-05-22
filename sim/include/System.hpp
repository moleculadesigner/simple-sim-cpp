// VV intergator for N-body gravity problem
// 
#ifndef SYSTEM
#define SYSTEM


#include "Body.hpp"
#include "Constants.hpp"
#include <vector>
#include <Eigen/Dense>


namespace sim {

class System {
private:
    std::vector<Body> particles_;
    double g_ = sim::GRAVITY_CONSTANT;
    double ljs_ = sim::LJ_SCALE_DEFAULT;

public:
    System() = default;
    System(size_t n_particles); // random init
    System(const std::vector<Body>& particles); // from bodies
    System(
        const std::vector<Eigen::Vector3d>& points,
        const std::vector<double>& masses
    ); // from coords + masses
    
    void add_particle(const Body& particle);
    Body& particle(size_t n);
    const Body& particle(size_t n) const;
    std::vector<Body>& particles() {return particles_;}
    const std::vector<Body>& particles() const {return particles_;}
    size_t n_particles() const {return particles_.size();}
    
    double g() const {return g_;}
    double lj_scale() const {return ljs_;}
    
    void g(double g) {g_ = g;}
    void lj_scale(double ljs) {ljs_ = ljs;}

// Energy
public:
    double T() const;
    double U() const;
    Eigen::Vector3d p() const;
    
// Movement
public:
    void calculate_forces();
    void step(double dt);
    Eigen::Vector3d com() const;
    void recenter();
    
}; // System
} // namespace sim

#endif // SYSTEM