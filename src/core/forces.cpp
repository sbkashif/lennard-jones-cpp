#include "core/forces.hpp"
#include <cmath>

namespace ljmd {
namespace core {

Forces::Forces(double cutoffRadius)
    : cutoffRadius_(cutoffRadius), potentialEnergy_(0.0) {
}

void Forces::calculateForces(std::vector<Particle>& particles, const utils::Vector3D& boxSize) {
    // Reset forces and energy
    for (auto& p : particles) {
        p.resetForces();
    }
    potentialEnergy_ = 0.0;
    
    const double cutoffSquared = cutoffRadius_ * cutoffRadius_;
    
    // Loop over all pairs of particles
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            // Calculate separation vector with minimum image convention
            utils::Vector3D rij = particles[i].position() - particles[j].position();
            
            // Apply minimum image convention
            for (int dim = 0; dim < 3; ++dim) {
                double x = 0.0;
                if (dim == 0) x = rij.x();
                else if (dim == 1) x = rij.y();
                else x = rij.z();
                
                double L = 0.0;
                if (dim == 0) L = boxSize.x();
                else if (dim == 1) L = boxSize.y();
                else L = boxSize.z();
                
                if (x > 0.5 * L) {
                    if (dim == 0) rij.setX(x - L);
                    else if (dim == 1) rij.setY(x - L);
                    else rij.setZ(x - L);
                } else if (x < -0.5 * L) {
                    if (dim == 0) rij.setX(x + L);
                    else if (dim == 1) rij.setY(x + L);
                    else rij.setZ(x + L);
                }
            }
            
            double r2 = rij.normSquared();
            
            // Add safety check to prevent numerical issues when particles get too close
            if (r2 < 0.01) {
                r2 = 0.01; // Minimum distance squared to prevent singularity
            }
            
            // Only calculate forces if particles are within cutoff
            if (r2 < cutoffSquared) {
                calculatePairForce(particles[i], particles[j], rij, r2);
            }
        }
    }
}

void Forces::calculatePairForce(Particle& p1, Particle& p2, const utils::Vector3D& rij, double r2) {
    // Calculate Lennard-Jones force
    utils::Vector3D force = calculateLJForce(rij, r2);
    
    // Apply force to both particles (Newton's third law)
    p1.applyForce(force);
    p2.applyForce(-force);
    
    // Add potential energy contribution
    potentialEnergy_ += calculateLJPotential(r2);
}

double Forces::calculateLJPotential(double r2) const {
    // Lennard-Jones potential: 4*epsilon*[(sigma/r)^12 - (sigma/r)^6]
    // With epsilon = 1.0 and sigma = 1.0 in reduced units
    double invr2 = 1.0 / r2;
    double invr6 = invr2 * invr2 * invr2;
    double invr12 = invr6 * invr6;
    
    // Calculate potential with cutoff shift
    double cutoff_r2 = cutoffRadius_ * cutoffRadius_;
    double invrc2 = 1.0 / cutoff_r2;
    double invrc6 = invrc2 * invrc2 * invrc2;
    double invrc12 = invrc6 * invrc6;
    
    // This is an energy-shifted potential that ensures continuity at cutoff
    return 4.0 * (invr12 - invr6 - invrc12 + invrc6);
}

utils::Vector3D Forces::calculateLJForce(const utils::Vector3D& rij, double r2) const {
    // Lennard-Jones force: F = -dV/dr = -24*epsilon*[(2*sigma^12/r^13) - (sigma^6/r^7)]*(r/|r|)
    // With epsilon = 1.0 and sigma = 1.0 in reduced units
    double invr2 = 1.0 / r2;
    double invr6 = invr2 * invr2 * invr2;
    double invr8 = invr6 * invr2;
    double factor = 24.0 * (2.0 * invr6 - 1.0) * invr8;
    
    // Apply a force magnitude limiter for numerical stability
    const double MAX_FORCE = 1000.0;
    if (std::abs(factor) > MAX_FORCE) {
        factor = (factor > 0) ? MAX_FORCE : -MAX_FORCE;
    }
    
    return factor * rij;
}

} // namespace core
} // namespace ljmd