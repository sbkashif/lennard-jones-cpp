#include "core/box.hpp"
#include <cmath>

namespace ljmd {
namespace core {

System Box::createLJSystem(std::size_t numParticles, double temperature, 
                          double density, double cutoffRadius) {
    // Calculate box size based on number of particles and density
    double volume = numParticles / density;
    double boxLength = std::cbrt(volume);
    size_ = utils::Vector3D(boxLength, boxLength, boxLength);
    
    // Create system with calculated box size
    System system(size_, cutoffRadius);
    
    // Initialize particles on a cubic lattice with the desired temperature
    system.createCubicLattice(numParticles, temperature, density);
    
    // Calculate initial forces
    system.calculateForces();
    
    return system;
}

} // namespace core
} // namespace ljmd