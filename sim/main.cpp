#include "Integrator.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
            std::cerr 
                << "Usage: " 
                << argv[0] 
                << " <N_particles>"
                << " <dt_value>" 
                << std::endl;
            return 1;
        }
    int particles = std::stoi(argv[1]);
    double dt = std::stod(argv[2]);
    
    sim::System system = sim::System(particles);
    
    for (size_t step = 0; step < 100; ++step) {
        system.print_state_xyz();
        system.step(dt);
    }
    system.print_state_xyz();
}
