#pragma once

#include "minimizers/minimizer.hpp"

namespace ljmd {
namespace minimizers {

class SteepestDescent : public Minimizer {
public:
    // Constructor
    SteepestDescent() : Minimizer(), lastEnergy_(0.0), maxForce_(0.0) {}
    
protected:
    // Implementation of abstract methods from the base class
    void initialize(core::System& system) override {
        // Calculate initial forces
        system.calculateForces();
        
        // Store initial energy
        initialEnergy_ = system.potentialEnergy();
        currentEnergy_ = initialEnergy_;
        lastEnergy_ = initialEnergy_ + 1.0; // Ensure we don't converge immediately
        
        // Log initial state
        std::cout << "Starting energy minimization (Steepest Descent)\n";
        std::cout << "Initial potential energy: " << initialEnergy_ << "\n";
    }
    
    void performStep(core::System& system) override {
        // Store current energy for convergence check
        lastEnergy_ = currentEnergy_;
        
        // Find maximum force component to normalize step size
        maxForce_ = 0.0;
        for (std::size_t i = 0; i < system.numParticles(); ++i) {
            const auto& force = system.getParticle(i).force();
            double fx = std::abs(force.x());
            double fy = std::abs(force.y());
            double fz = std::abs(force.z());
            maxForce_ = std::max(maxForce_, std::max(fx, std::max(fy, fz)));
        }
        
        // Avoid division by zero
        if (maxForce_ < 1e-10) {
            maxForce_ = 1e-10;
        }
        
        // Move particles along the negative gradient (force direction)
        for (std::size_t i = 0; i < system.numParticles(); ++i) {
            auto& particle = system.getParticle(i);
            utils::Vector3D pos = particle.position();
            
            // Calculate displacement using normalized force
            utils::Vector3D displacement = particle.force() * (stepSize_ / maxForce_);
            
            // Update position
            pos += displacement;
            
            // Apply periodic boundary conditions
            pos.applyPeriodicBC(system.boxSize());
            
            // Set the updated position
            particle.setPosition(pos);
        }
        
        // Recalculate forces and energy
        system.calculateForces();
        currentEnergy_ = system.potentialEnergy();
        
        // Adaptive step size - increase if energy decreases, decrease if energy increases
        if (currentEnergy_ < lastEnergy_) {
            stepSize_ *= 1.1; // Increase step size
        } else {
            stepSize_ *= 0.5; // Decrease step size
            
            // Revert to previous positions and recalculate
            // (This part would need system to store previous positions)
            // For simplicity, we'll just limit step size reduction
            if (stepSize_ < 1e-6) {
                stepSize_ = 1e-6;
            }
        }
    }
    
    bool isConverged() override {
        // Check if energy difference is below tolerance
        double energyDiff = std::abs(currentEnergy_ - lastEnergy_);
        return energyDiff < tolerance_;
    }
    
    void finalize(core::System& system) override {
        // Final recalculation of forces
        system.calculateForces();
        
        // Calculate energy improvement
        double energyImprovement = initialEnergy_ - system.potentialEnergy();
        
        // Log final state
        std::cout << "Energy minimization completed.\n";
        std::cout << "Final potential energy: " << system.potentialEnergy() << "\n";
        std::cout << "Energy improvement: " << energyImprovement << "\n";
    }
    
private:
    double lastEnergy_;
    double maxForce_;
};

} // namespace minimizers
} // namespace ljmd