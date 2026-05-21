#ifndef BODY
#define BODY
#include <array>
#include <Eigen/Dense>

namespace sim {
static double eps = 1e-12;
class Body{
    private:
        Eigen::Vector3d coord_;
        Eigen::Vector3d vel_ = Eigen::Vector3d::Zero();
        Eigen::Vector3d f_ = Eigen::Vector3d::Zero();
        double m_;
        
    
    public:
        Body(const Eigen::Vector3d& vec, double m); /*:
            coord_{vec},
            vel_{Eigen::Vector3d::Zero()},
            f_{Eigen::Vector3d::Zero()},
            m_{m} {} */
        
        // Access
        Body(double x, double y, double z, double m);
        const Eigen::Vector3d& X() const {return coord_;}
        const Eigen::Vector3d& V() const {return vel_;}
        const Eigen::Vector3d& F() const {return f_;}
        double m() const {return m_;}
        
        Eigen::Vector3d p() const {return m_ * vel_;}
        Eigen::Vector3d a() const {return f_ / m_;}
        
        // Set
        void X(const Eigen::Vector3d& vec) {coord_ = vec;}
        void V(const Eigen::Vector3d& vec) {vel_ = vec;}
        void F(const Eigen::Vector3d& vec) {f_ = vec;}
        void m(double mass);
  
        // Measure
        double sq_dist_to(const Body& other) const;
        
        // Transform
        void move(const Eigen::Vector3d& dx);
        void accelerate(const Eigen::Vector3d& dv);
        void reset_force();
        
        // Show
        std::string show_xyz() const;

}; // Body

Eigen::Vector3d gravity_force(const Body& b1, const Body& b2, double G, double lj_sigma_scale);
} // gravity      

#endif // BODY