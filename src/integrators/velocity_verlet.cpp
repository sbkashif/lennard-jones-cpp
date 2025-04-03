#include "integrators/velocity_verlet.hpp"
#include "utils/vector3d.hpp"

namespace ljmd {
namespace integrators {

void VelocityVerlet::step(core::System& system) {
    const double dt = timestep_;
    const double halfdt = 0.5 * dt;
    const auto& boxSize = system.boxSize();

    // Step 1: Update positions and half-step velocities
    for (std::size_t i = 0; i < system.numParticles(); ++i) {
        auto& particle = system.getParticle(i);
        const double invmass = 1.0 / particle.mass();
        
        // Update velocity by half step
        utils::Vector3D vel = particle.velocity();
        vel += particle.force() * (halfdt * invmass);
        particle.setVelocity(vel);
        
        // Update position by full step
        utils::Vector3D pos = particle.position();
        pos += vel * dt;
        pos.applyPeriodicBC(boxSize);
        particle.setPosition(pos);
    }
    
    // Step 2: Calculate new forces
    system.calculateForces();
    
    // Step 3: Update velocities for the second half-step
    for (std::size_t i = 0; i < system.numParticles(); ++i) {
        auto& particle = system.getParticle(i);
        const double invmass = 1.0 / particle.mass();
        
        // Update velocity by another half step
        utils::Vector3D vel = particle.velocity();
        vel += particle.force() * (halfdt * invmass);
        particle.setVelocity(vel);
    }
}

} // namespace integrators
} // namespace ljmd