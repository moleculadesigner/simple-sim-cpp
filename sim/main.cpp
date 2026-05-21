#include "Integrator.hpp"
#include <iostream>
#include <string>
#include <argparse/argparse.hpp>
#include <filesystem>
#include <fstream>

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
    program.add_argument("-o", "--output")
        .help("Путь к выходному .xyz файлу")
        .default_value(std::string("trajectory.xyz"));
    
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
    std::filesystem::path out_path(
        program.get<std::string>("-o")
    );
    
    sim::System system = sim::System(n);
    
    std::ofstream out_file(out_path);
    
    std::cout << "Running simulation of "
        << n << " particles for "
        << dt << " × " << steps
        << " = " << dt * steps << " seconds:"
        << std::endl;
    
    for (size_t step = 0; step < steps; ++step) {
        system.print_state_xyz(
            out_file,
            "step " + std::to_string(step)
        );
        system.step(dt);
        if (!((step + 1) % 100)) {
            std::cout << "Step " << step + 1 << std::endl;
        }
    }
    std::cout << "Done." << std::endl;
    system.print_state_xyz(
        out_file,
        "step " + std::to_string(steps)
    );
}
