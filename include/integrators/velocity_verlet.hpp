#pragma once

#include "integrators/integrator.hpp"

namespace ljmd {
namespace integrators {

class VelocityVerlet : public Integrator {
public:
    // Constructor
    VelocityVerlet(double timestep) : Integrator(timestep) {}
    
    // Integration step implementation
    void step(core::System& system) override;
};

} // namespace integrators
} // namespace ljmd