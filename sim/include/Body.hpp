#ifndef BODY
#define BODY
#include <array>

namespace sim {
static double G = 6.675542e-11;
class Body{
    private:
        double x, y, z;
        double m;
        
    
    public:
        Body(double x, double y, double z, double m): x(x), y(y), z(z), m(m) {}
        Body(std::array<double, 3> &coords, double mass);
        
        // Access
        std::array<double, 3> X();
        double M();
  
        // Measure
        double sq_dist_to(Body &other);
}; // Body

double gravity_force(Body &b1, Body &b2);
} // gravity      

#endif // BODY