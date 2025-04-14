# Energy Minimization Module

This module provides energy minimization algorithms for optimizing molecular configurations in the Lennard-Jones MD simulation framework.

## Overview

Energy minimization is a crucial step in molecular dynamics simulations that helps eliminate high-energy configurations and reduce the risk of simulation instabilities. This module implements various minimization algorithms to find local energy minima.

## Available Minimizers

### Steepest Descent

The steepest descent algorithm is a first-order optimization algorithm that takes steps proportional to the negative of the gradient of the potential energy function. It's simple but effective for initial relaxation of molecular structures.

**Key features:**
- Simple and robust implementation
- Good for initial relaxation of structures with high-energy clashes
- Configurable step size and convergence criteria

### Conjugate Gradient (Coming Soon)

The conjugate gradient method is more sophisticated than steepest descent and often converges faster, especially near the minimum.

**Key features:**
- Faster convergence than steepest descent
- Better performance near the energy minimum
- Memory of previous search directions to avoid oscillations

## Usage

To use energy minimization in your simulation, set the following parameters in your `config.ini` file:

```ini
# Energy Minimization parameters
minimize_energy = true               # Enable energy minimization
minimization_algorithm = "steepest"  # Algorithm to use: "steepest" or "conjugate"
minimization_steps = 1000            # Maximum number of minimization steps
minimization_tolerance = 1e-6        # Energy convergence criterion
minimization_step_size = 0.01        # Initial step size
```

## Implementation Details

Minimizers are implemented as classes that inherit from the `Minimizer` base class. Each minimizer must implement:

- `initialize()`: Set up initial conditions for minimization
- `step()`: Perform a single minimization step
- `isConverged()`: Check if convergence criteria are met

## Adding New Minimizers

To add a new minimization algorithm:

1. Create a new class that inherits from `Minimizer`
2. Implement the required methods
3. Register the minimizer in the `MinimizerFactory` class
4. Update documentation to include the new minimizer

## Example

```cpp
// Creating and using a minimizer
Minimizer* minimizer = MinimizerFactory::create("steepest");
minimizer->setMaxSteps(1000);
minimizer->setTolerance(1e-6);
minimizer->minimize(system);
```

## References

- Numerical Optimization by Nocedal and Wright
- Understanding Molecular Simulation by Frenkel and Smit