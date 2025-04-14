#pragma once

#include "core/system.hpp"

namespace ljmd {
namespace minimizers {

class Minimizer {
public:
    // Constructor
    Minimizer() : maxSteps_(1000), tolerance_(1e-6), stepSize_(0.01) {}
    
    // Destructor
    virtual ~Minimizer() = default;
    
    // Getters and setters
    std::size_t maxSteps() const { return maxSteps_; }
    double tolerance() const { return tolerance_; }
    double stepSize() const { return stepSize_; }
    
    void setMaxSteps(std::size_t steps) { maxSteps_ = steps; }
    void setTolerance(double tol) { tolerance_ = tol; }
    void setStepSize(double size) { stepSize_ = size; }
    
    // Main minimization method that uses the template method pattern
    // This method orchestrates the minimization process
    void minimize(core::System& system) {
        initialize(system);
        
        std::size_t step = 0;
        while (step < maxSteps_ && !isConverged()) {
            performStep(system);
            step++;
        }
        
        finalize(system);
    }
    
protected:
    // Methods to be implemented by specific minimizers
    virtual void initialize(core::System& system) = 0;
    virtual void performStep(core::System& system) = 0;
    virtual bool isConverged() = 0;
    virtual void finalize(core::System& system) = 0;
    
    // Common state variables
    std::size_t maxSteps_;
    double tolerance_;
    double stepSize_;
    double initialEnergy_;
    double currentEnergy_;
};

} // namespace minimizers
} // namespace ljmd