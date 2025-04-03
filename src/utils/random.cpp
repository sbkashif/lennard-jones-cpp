#include "utils/random.hpp"
#include <chrono>

namespace ljmd {
namespace utils {

Random::Random() 
    : generator_(std::chrono::system_clock::now().time_since_epoch().count()),
      uniformDist_(0.0, 1.0),
      normalDist_(0.0, 1.0) {
}

Random::Random(unsigned int seed) 
    : generator_(seed),
      uniformDist_(0.0, 1.0),
      normalDist_(0.0, 1.0) {
}

double Random::uniform(double min, double max) {
    return min + (max - min) * uniformDist_(generator_);
}

double Random::normal(double mean, double stddev) {
    return mean + stddev * normalDist_(generator_);
}

Vector3D Random::randomVector(double stddev) {
    return Vector3D(
        normal(0.0, stddev),
        normal(0.0, stddev),
        normal(0.0, stddev)
    );
}

} // namespace utils
} // namespace ljmd