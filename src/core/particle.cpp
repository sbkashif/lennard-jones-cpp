#include "core/particle.hpp"

namespace ljmd {
namespace core {

double Particle::kineticEnergy() const {
    return 0.5 * mass_ * velocity_.normSquared();
}

void Particle::update(double dt, const utils::Vector3D& boxSize) {
    // Update position
    position_ += velocity_ * dt;
    
    // Apply periodic boundary conditions
    position_.applyPeriodicBC(boxSize);
}

void Particle::applyForce(const utils::Vector3D& force) {
    force_ += force;
}

void Particle::resetForces() {
    force_ = utils::Vector3D(0.0, 0.0, 0.0);
}

} // namespace core
} // namespace ljmd