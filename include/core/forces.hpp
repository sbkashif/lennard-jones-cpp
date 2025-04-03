#pragma once

#include <vector>
#include "core/particle.hpp"
#include "utils/vector3d.hpp"

namespace ljmd {
namespace core {

class Forces {
public:
    // Constructor
    Forces(double cutoffRadius = 2.5);
    
    // Getters and setters
    double cutoffRadius() const { return cutoffRadius_; }
    void setCutoffRadius(double cutoffRadius) { cutoffRadius_ = cutoffRadius; }
    
    double potentialEnergy() const { return potentialEnergy_; }
    
    // Calculate forces between all particles in the system
    void calculateForces(std::vector<Particle>& particles, const utils::Vector3D& boxSize);
    
    // Calculate forces and energy for a pair of particles
    void calculatePairForce(Particle& p1, Particle& p2, const utils::Vector3D& rij, double r2);
    
private:
    double cutoffRadius_;
    double potentialEnergy_;
    
    // Helper methods for Lennard-Jones potential
    double calculateLJPotential(double r2) const;
    utils::Vector3D calculateLJForce(const utils::Vector3D& rij, double r2) const;
};

} // namespace core
} // namespace ljmd