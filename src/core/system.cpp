#include "core/system.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <cassert>

namespace ljmd {
namespace core {

System::System(const utils::Vector3D& boxSize, double cutoffRadius) 
    : boxSize_(boxSize), forces_(cutoffRadius) {}

void System::addParticle(const Particle& particle) {
    particles_.push_back(particle);
}

double System::temperature() const {
    if (particles_.empty()) {
        return 0.0;
    }
    // T = 2 * Ekin / (3 * N * kB), we set kB = 1 in reduced units
    return 2.0 * kineticEnergy() / (3.0 * particles_.size());
}

double System::totalEnergy() const {
    return kineticEnergy() + potentialEnergy();
}

double System::kineticEnergy() const {
    double energy = 0.0;
    for (const auto& particle : particles_) {
        energy += particle.kineticEnergy();
    }
    return energy;
}

void System::resetForces() {
    for (auto& particle : particles_) {
        particle.resetForces();
    }
}

utils::Vector3D System::minimumImage(const utils::Vector3D& r) const {
    utils::Vector3D result = r;
    result.applyPeriodicBC(boxSize_);
    return result;
}

void System::calculateForces() {
    forces_.calculateForces(particles_, boxSize_);
}

void System::createCubicLattice(std::size_t numParticles, double temperature, double density) {
    // Clear existing particles
    particles_.clear();
    
    // Calculate box size based on density
    double volume = numParticles / density;
    double boxLength = std::cbrt(volume);
    boxSize_ = utils::Vector3D(boxLength, boxLength, boxLength);
    
    // Calculate number of particles per dimension
    int particlesPerDim = static_cast<int>(std::cbrt(numParticles) + 0.999);
    
    // Calculate spacing between particles on the lattice
    // Ensure spacing is large enough to avoid strong repulsions
    double spacing = boxLength / particlesPerDim;
    
    // Create particles on a cubic lattice
    std::size_t added = 0;
    for (int i = 0; i < particlesPerDim && added < numParticles; ++i) {
        for (int j = 0; j < particlesPerDim && added < numParticles; ++j) {
            for (int k = 0; k < particlesPerDim && added < numParticles; ++k) {
                // Position with a small random displacement to break symmetry
                // This helps prevent particles from having exactly the same forces
                std::mt19937 gen(42 * added + 1); // Different seed for each particle
                std::uniform_real_distribution<double> dist(-0.1, 0.1); // Small random displacement
                
                utils::Vector3D position(
                    (i + 0.5 + dist(gen) * 0.2) * spacing,
                    (j + 0.5 + dist(gen) * 0.2) * spacing,
                    (k + 0.5 + dist(gen) * 0.2) * spacing
                );
                
                // Center the box around origin to make visualization easier
                position.setX(position.x() - boxLength / 2.0);
                position.setY(position.y() - boxLength / 2.0);
                position.setZ(position.z() - boxLength / 2.0);
                
                Particle particle(1.0, position);
                addParticle(particle);
                ++added;
            }
        }
    }
    
    // Initialize velocities to match desired temperature
    initializeVelocities(temperature);
}

void System::initializeVelocities(double temperature) {
    if (particles_.empty()) {
        return;
    }
    
    // Initialize random number generator with reproducible seed
    std::mt19937 gen(42); // Fixed seed for reproducibility
    std::normal_distribution<double> dist(0.0, 1.0);
    
    // Assign random velocities from a normal distribution
    utils::Vector3D totalMomentum(0.0, 0.0, 0.0);
    
    for (auto& particle : particles_) {
        // Generate random velocities with standard normal distribution
        utils::Vector3D vel(dist(gen), dist(gen), dist(gen));
        particle.setVelocity(vel);
        totalMomentum += particle.mass() * vel;
    }
    
    // Remove center of mass momentum
    utils::Vector3D velocityShift = totalMomentum / (particles_.size() * particles_[0].mass());
    
    for (auto& particle : particles_) {
        particle.setVelocity(particle.velocity() - velocityShift);
    }
    
    // Calculate current kinetic energy
    double currentKineticEnergy = 0.0;
    for (const auto& particle : particles_) {
        currentKineticEnergy += particle.kineticEnergy();
    }
    
    // Calculate current temperature
    // T = 2 * Ekin / (3 * N * kB), where kB = 1 in reduced units
    double currentTemperature = 2.0 * currentKineticEnergy / (3.0 * particles_.size());
    
    // Scale velocities to match desired temperature if current temperature is not zero
    if (currentTemperature > 1e-10) {
        double scaleFactor = std::sqrt(temperature / currentTemperature);
        
        for (auto& particle : particles_) {
            particle.setVelocity(particle.velocity() * scaleFactor);
        }
    }
}

} // namespace core
} // namespace ljmd