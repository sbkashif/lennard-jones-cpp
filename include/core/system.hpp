#pragma once

#include <vector>
#include "core/particle.hpp"
#include "core/forces.hpp"
#include "utils/vector3d.hpp"

namespace ljmd {
namespace core {

class System {
public:
    // Constructors
    System(const utils::Vector3D& boxSize, double cutoffRadius = 2.5);
    
    // Getters and setters
    const utils::Vector3D& boxSize() const { return boxSize_; }
    double cutoffRadius() const { return forces_.cutoffRadius(); }
    double temperature() const;
    double totalEnergy() const;
    double kineticEnergy() const;
    double potentialEnergy() const { return forces_.potentialEnergy(); }
    
    // Particle management
    void addParticle(const Particle& particle);
    std::size_t numParticles() const { return particles_.size(); }
    const Particle& getParticle(std::size_t index) const { return particles_[index]; }
    Particle& getParticle(std::size_t index) { return particles_[index]; }
    
    // System operations
    void calculateForces();
    void resetForces();
    
    // Initialization
    void createCubicLattice(std::size_t numParticles, double temperature, double density = 0.8);
    void initializeVelocities(double temperature);
    
private:
    std::vector<Particle> particles_;
    utils::Vector3D boxSize_;
    Forces forces_;
    
    // Helper methods
    utils::Vector3D minimumImage(const utils::Vector3D& r) const;
};

} // namespace core
} // namespace ljmd