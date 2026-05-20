#include "Body.hpp"
#include <cstdlib>
#include <random>
#include <iostream>
#include <array>
#include <tuple>


gravity::Body rand_body(double mincoord, double maxcoord) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> xdistrib(mincoord, maxcoord);
    static std::uniform_real_distribution<> mdistrib(100, 1000);
    return gravity::Body(
        xdistrib(gen),
        xdistrib(gen),
        xdistrib(gen),
        mdistrib(gen)
    );
}

void show(gravity::Body& body) {
    std::cout
    << "Body ("
    << body.X()[0] << ", "
    << body.X()[1] << ", "
    << body.X()[2] << ")-|"
    << body.M() << "| ";
}

int main() {
    auto b1 = rand_body(-1000, 1000);
    auto b2 = rand_body(-1000, 1000);
    auto f = gravity::gravity_force(b1, b2);
    std::cout << "Gravity force between ";
    show(b1);
    std::cout << "and ";
    show(b2);
    std::cout << "is " << f << std::endl;
}
