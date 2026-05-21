#include "Integrator.hpp"
#include "Body.hpp"
#include <iostream>
#include <string>
#include <argparse/argparse.hpp>
#include <filesystem>
#include <fstream>
#include <Eigen/Dense>

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program("sim");
    program.add_argument("-n", "--n-particles")
        .help("Количество частиц в системе")
        .scan<'i', size_t>()       
        .default_value(100);
    program.add_argument("-t", "--dt")
        .help("Шаг времени симуляции")
        .scan<'g', double>()      // сканируем как double (float)
        .default_value(1.0);
    program.add_argument("-s", "--n-steps")
        .help("Количество шагов симуляции")
        .scan<'i', size_t>()       
        .default_value(100);
    program.add_argument("-d", "--dump")
        .help("Записывать каждые DUMP фреймов")
        .scan<'i', size_t>()       
        .default_value(100);
    program.add_argument("-o", "--output")
        .help("Путь к выходному .xyz файлу")
        .default_value(std::string("trajectory.xyz"));
    program.add_argument("--sun")
        .help("Добавить тяжелое тело в центр")
        .default_value(false)   
        .implicit_value(true);
    program.add_argument("--com")
        .help("Убрать движение центра масс")
        .default_value(false)   
        .implicit_value(true);
    

    
    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }
    
    size_t n = program.get<size_t>("-n");
    double dt = program.get<double>("-t");
    size_t steps = program.get<size_t>("-s");
    size_t dump = program.get<size_t>("-d");
    std::filesystem::path out_path(
        program.get<std::string>("-o")
    );
    bool with_sun = program.get<bool>("--sun");
    bool update_com = program.get<bool>("--com");
    
    sim::System system = sim::System(n);
    if (with_sun) {
        sim::Body sun1 = sim::Body(-40.0, -40.0, 0.0, 1000000);
        sun1.V(Eigen::Vector3d{-5.0e-4, 5.0e-4, 0});
        
        sim::Body sun2 = sim::Body(40.0, 40.0, 0.0, 1000000);
        sun2.V(Eigen::Vector3d{5.0e-4, -5.0e-4, 0});
        
        system.add_particle(sun1);
        system.add_particle(sun2);
    }
    
    std::ofstream out_file(out_path);
    
    std::cout << "step,T,U"
        << std::endl
        << "0,"
        << system.T() << ","
        << system.U() << std::endl;
    
    system.print_state_xyz(
        out_file,
        "step " + std::to_string(0)
    );
    for (size_t step = 1; step <= steps; ++step) {
        system.step(dt);
        if (!(step % dump)) {
            if (update_com) {
                system.recenter();
            }
            system.print_state_xyz(
                out_file,
                "step " + std::to_string(step)
            );
            std::cout << step << ","
                << system.T() << ","
                << system.U() << std::endl;
        }
    }
}
