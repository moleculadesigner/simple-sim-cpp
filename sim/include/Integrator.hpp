// VV intergator for N-body gravity problem
// 
#ifndef INTEGRATOR
#define INTEGRATOR


#include "Body.hpp"
#include <vector>
#include <Eigen/Dense>
#include <ostream>


namespace sim {
static double G = 6.675542e-11;
static double SigmaScale = 1e-10;

class System {
private:
    std::vector<Body> particles_;
    double G_ = G;
    double ljs_ = SigmaScale;

public:
    System(size_t n_particles); // random init
    System(const std::vector<Body>& particles); // from bodies
    System(
        const std::vector<Eigen::Vector3d>& points,
        const std::vector<double>& masses
    ); // from coords + masses
    
    void add_particle(const Body& particle);
    size_t n_particles() const {return particles_.size();}
    
// Movement
public:
    void calculate_forces();
    void step(double dt);
    void print_state_xyz(std::ostream& os, const std::string& comment) const;
    
}; // System
} // namespace sim

#endif // INTEGRATOR