#include <gtest/gtest.h>
#include "Integrator.hpp"
#include <Eigen/Dense>

using V3 = Eigen::Vector3d;
TEST(PhysicsTest, ForceConservation) {
    sim::System sys(2); // Создаем систему из 2 тел
    sys.particle(0).X(V3{10, 0, 0});
    sys.particle(0).V(V3{0, 1, 0});
    sys.particle(0).m(5.0);
    
    sys.particle(1).X(V3{0, 0, 0});
    sys.particle(1).V(V3{0, -1, 0});
    sys.particle(1).m(5.0);
    
    sys.calculate_forces();
    
    auto f1 = sys.particle(0).F();
    auto f2 = sys.particle(1).F();
    auto df = (f1 + f2).norm();
    // Сумма сил в замкнутой системе должна быть 0
    EXPECT_NEAR(df, 0.0, 1e-9) << "Violation of 3rd Newton's law: " << df;
}

TEST(PhysicsTest, EnergyConservation) {
    sim::System sys(2);
    sys.particle(0).X(V3{10, 0, 0});
    sys.particle(0).V(V3{0, 1, 0});
    sys.particle(0).m(5.0);
    
    sys.particle(1).X(V3{0, 0, 0});
    sys.particle(1).V(V3{0, -1, 0});
    sys.particle(1).m(5.0);
    
    double dt = 0.001;
    double initial_e = sys.T() + sys.U();
    
    for(int i = 0; i < 100000; ++i) {
        sys.step(dt);
    }
    
    double final_e = sys.T() + sys.U();
        
    // Проверяем, что энергия уплыла не более чем на 0.1%
    double error = std::abs((final_e - initial_e) / initial_e);
    EXPECT_LT(error, 0.001) << "Energy drifted too much: " << error;
}

TEST(PhysicsTest, MomentumConservation) {
    sim::System sys(2);
    sys.particle(0).X(V3{10, 0, 0});
    sys.particle(0).V(V3{0, 1, 0});
    sys.particle(0).m(5.0);
    
    sys.particle(1).X(V3{0, 0, 0});
    sys.particle(1).V(V3{0, -1, 0});
    sys.particle(1).m(5.0);
    
    double dt = 0.001;
    V3 initial_p = (
        sys.particle(0).m() * sys.particle(0).V()
        + sys.particle(1).m() * sys.particle(1).V()
    );
    
    for(int i = 0; i < 1000; ++i) {
        sys.step(dt);
    }
    
    V3 final_p = (
        sys.particle(0).m() * sys.particle(0).V()
        + sys.particle(1).m() * sys.particle(1).V()
    );
    EXPECT_NEAR(initial_p.x(), final_p.x(), 1e-9) << "Momentum X component drifted too much";
    EXPECT_NEAR(initial_p.y(), final_p.y(), 1e-9) << "Momentum Y component drifted too much";
    EXPECT_NEAR(initial_p.z(), final_p.z(), 1e-9) << "Momentum Z component drifted too much";
    // Проверяем, что импульс уплыл не более чем на 0.1%
    //double error = std::abs((final_p - initial_p) / initial_p);
    //EXPECT_LT(error, 0.001) << "Momentum drifted too much: " << error;
}
