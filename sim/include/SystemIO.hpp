#ifndef SYSTEM_IO
#define SYSTEM_IO

#include "System.hpp"
#include "Body.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <ostream>
#include <filesystem>
#include <Eigen/Dense>

namespace sim {

using V3 = Eigen::Vector3d;

// Body
void from_json(const nlohmann::json& j, Body& b);
void to_json(nlohmann::json& j, const Body& b);
void write_xyz_entry(const Body& sys, std::ostream& os);

// System
void from_json(const nlohmann::json& j, System& s);
void to_json(nlohmann::json& j, const System& s);

System read_json_file(const std::filesystem::path& path);
void write_state_xyz(const System& sys, std::ostream& os, const std::string& comment);
} // namespace sim::io

#endif // SYSTEM_IO