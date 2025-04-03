#pragma once

#include "utils/vector3d.hpp"

namespace ljmd {
namespace core {

class Particle {
public:
    // Constructors
    Particle() : mass_(1.0) {}
    Particle(double mass) : mass_(mass) {}
    Particle(double mass, const utils::Vector3D& position) 
        : mass_(mass), position_(position) {}
    Particle(double mass, const utils::Vector3D& position, const utils::Vector3D& velocity) 
        : mass_(mass), position_(position), velocity_(velocity) {}

    // Getters and setters
    double mass() const { return mass_; }
    const utils::Vector3D& position() const { return position_; }
    const utils::Vector3D& velocity() const { return velocity_; }
    const utils::Vector3D& force() const { return force_; }
    
    void setMass(double mass) { mass_ = mass; }
    void setPosition(const utils::Vector3D& position) { position_ = position; }
    void setVelocity(const utils::Vector3D& velocity) { velocity_ = velocity; }
    void setForce(const utils::Vector3D& force) { force_ = force; }
    
    // Energy calculations
    double kineticEnergy() const;
    
    // Particle operations
    void update(double dt, const utils::Vector3D& boxSize);
    void applyForce(const utils::Vector3D& force);
    void resetForces();

private:
    double mass_;
    utils::Vector3D position_;
    utils::Vector3D velocity_;
    utils::Vector3D force_;
};

} // namespace core
} // namespace ljmd