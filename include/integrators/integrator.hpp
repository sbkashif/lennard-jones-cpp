#pragma once

#include "core/system.hpp"

namespace ljmd {
namespace integrators {

class Integrator {
public:
    // Constructor
    Integrator(double timestep) : timestep_(timestep) {}
    
    // Destructor
    virtual ~Integrator() = default;
    
    // Getter and setter
    double timestep() const { return timestep_; }
    void setTimestep(double dt) { timestep_ = dt; }
    
    // Integration step - to be implemented by derived classes
    virtual void step(core::System& system) = 0;
    
protected:
    double timestep_;
};

} // namespace integrators
} // namespace ljmd