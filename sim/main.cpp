#include "System.hpp"
#include "SystemIO.hpp"
#include <iostream>
#include <ostream>
#include <string>
#include <argparse/argparse.hpp>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>


int main(int argc, char* argv[]) {
    argparse::ArgumentParser program("sim");
    program.add_argument("-i", "--input")
        .help("JSON с параметрами симуляции")
        .default_value(std::string("config.json"));
    program.add_argument("-o", "--output")
        .help("Путь к папке с результатами")
        .default_value(std::string("."));
    

    
    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }
    
    std::filesystem::path config_path(
        program.get<std::string>("-i")
    );
    std::filesystem::path out_path(
        program.get<std::string>("-o")
    );
    std::filesystem::create_directories(out_path);
    
    std::ifstream config(config_path);
    nlohmann::json data;
    try {
        data = nlohmann::json::parse(config);
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Критическая ошибка в конфиге: " << e.what() << std::endl;
        return 1;
    }
    auto dt = data.at("dt").get<double>();
    auto steps = data.at("n_steps").get<size_t>();
    auto dump = data.at("nst_dump").get<size_t>();
    auto update_com = data.at("update_com").get<bool>();
    sim::System system = data.at("system").get<sim::System>();
    
    std::ofstream trajectory_fs(out_path / "trajectory.xyz");
    std::ofstream energies_fs(out_path / "energies.csv");
    
    std::cout << "Run simulation with system of "
        << system.n_particles() << " particles for "
        << steps << " × "
        << dt << " = "
        << steps * dt << " seconds\n";
    
    double t = system.T();
    double u = system.U();
    double e = t + u;
    energies_fs << "step,T,U,E"
        << "\n"
        << "0,"
        << t << ","
        << u << ","
        << e << "\n";
    
    sim::write_state_xyz(
        system,
        trajectory_fs,
        "step " + std::to_string(0)
    );
    for (size_t step = 1; step <= steps; ++step) {
        system.step(dt);
        if (!(step % dump)) {
            if (update_com) {
                system.recenter();
            }
            sim::write_state_xyz(
                system,
                trajectory_fs,
                "step " + std::to_string(step)
            );
            t = system.T();
            u = system.U();
            e = t + u;
            energies_fs << step << ","
                << t << ","
                << u << ","
                << e << "\n";
            
            std::cout << "\r\33[2K" << "step " << step << " | E = " << e << std::flush;
        }
    }
    std::cout << std::endl << "Done" << std::endl;
}
