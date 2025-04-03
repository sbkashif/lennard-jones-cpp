#pragma once

#include <random>
#include "utils/vector3d.hpp"

namespace ljmd {
namespace utils {

class Random {
public:
    // Constructors
    Random();
    explicit Random(unsigned int seed);
    
    // Random number generation methods
    double uniform(double min = 0.0, double max = 1.0);
    double normal(double mean = 0.0, double stddev = 1.0);
    
    // Random vector generation
    Vector3D randomVector(double stddev = 1.0);
    
private:
    std::mt19937 generator_;
    std::uniform_real_distribution<double> uniformDist_;
    std::normal_distribution<double> normalDist_;
};

} // namespace utils
} // namespace ljmd