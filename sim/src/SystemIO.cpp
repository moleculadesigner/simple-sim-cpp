#ifndef SYSTEM_IO
#define SYSTEM_IO

#include "System.hpp"
#include "Body.hpp"
#include "SystemIO.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <ostream>
#include <filesystem>
#include <Eigen/Dense>

namespace sim {
using V3 = Eigen::Vector3d;

// Body
void from_json(const nlohmann::json& j, Body& b) {
        // j здесь — это только кусочек {...} для одной частицы
        b.X(V3{
            j.at("x").get<double>(),
            j.at("y").get<double>(),
            j.at("z").get<double>()
        });
        b.m(j.value("m", 1.0));
        b.V(V3{   
            j.value("vx", 0.0),
            j.value("vy", 0.0),
            j.value("vz", 0.0)
        });
        b.F(V3::Zero());
    }

void to_json(nlohmann::json& j, const Body& b) {
    j = nlohmann::json{
        {"x", b.X().x()},
        {"y", b.X().y()},
        {"z", b.X().z()},
        {"m", b.m()},
        {"vx", b.V().x()},
        {"vy", b.V().y()},
        {"vz", b.V().z()},
    };
}

void write_xyz_entry(const Body& b, std::ostream& os) {
    os << "P " 
       << b.X().x() << " "
       << b.X().y() << " "
       << b.X().z() << "\n";
}


// System
void from_json(const nlohmann::json& j, System& s) {
    s.g(j.value("g", G));
    s.lj_scale(j.value("lj_scale", SigmaScale));
    for (const auto& jp : j.at("particles")) {
        s.add_particle(jp.get<Body>()); 
    }
}

void to_json(nlohmann::json& j, const System& s) {
    j = nlohmann::json{
            {"g", s.g()},
            {"lj_scale", s.lj_scale()},
            {"particles", nlohmann::json::array()}
        };
    for (const auto& p : s.particles()) {
        j["particles"].push_back(s.particles()); 
    }
}

void write_state_xyz(
    const System& s,
    std::ostream& os,
    const std::string& comment
) {
    os << s.n_particles() << "\n"
       << comment << "\n";
    for (auto p: s.particles()) {
        write_xyz_entry(p, os);
    }
}
} // namespace sim::io

#endif // SYSTEM_IO