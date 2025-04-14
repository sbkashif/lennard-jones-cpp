# Integrators Module

This module provides time integration algorithms for advancing the system state in molecular dynamics simulations.

## Overview

The integrators module contains various algorithms for solving Newton's equations of motion. The choice of integrator affects the accuracy, stability, and performance of the simulation.

## Available Integrators

### Velocity Verlet

The Velocity Verlet algorithm is a second-order symplectic integrator that provides good energy conservation over long time periods.

**Key features:**
- Time reversible and symplectic (phase space volume preserving)
- Excellent long-term energy conservation
- Simple to implement and numerically stable
- Default integrator for Lennard-Jones simulations

**Implementation details:**
The Velocity Verlet algorithm updates positions and velocities in the following steps:
1. Update velocities by half step: v(t+Δt/2) = v(t) + (Δt/2) * a(t)
2. Update positions by full step: r(t+Δt) = r(t) + Δt * v(t+Δt/2)
3. Calculate forces/accelerations at new positions: a(t+Δt)
4. Update velocities by final half step: v(t+Δt) = v(t+Δt/2) + (Δt/2) * a(t+Δt)

### Leapfrog (Coming Soon)

The Leapfrog algorithm is a variation of the Velocity Verlet method that computes velocities at half-steps.

**Key features:**
- Similar numerical stability to Velocity Verlet
- Slightly different implementation approach
- Compatible with thermostats like Berendsen

### Euler (Coming Soon)

The Euler integrator is a simple first-order method, mostly included for educational purposes.

**Key features:**
- Simplest integration scheme
- Lower accuracy and stability than Verlet methods
- Useful for teaching purposes or very simple simulations

## Usage

To specify which integrator to use in your simulation, set the following parameter in your `config.ini` file:

```ini
# Integration parameters
integrator = "velocity-verlet"  # Options: "velocity-verlet", "leapfrog", "euler"
timestep = 0.001                # Integration time step
```

## Implementation Details

All integrators inherit from the `Integrator` base class defined in `integrator.hpp`. Each integrator must implement:

- `step(System& system)`: Advance the system by one timestep

The base class provides common functionality such as:
- Storing and retrieving the timestep size
- Common interface for all integrators

## Adding New Integrators

To add a new integration algorithm:

1. Create a new class that inherits from `Integrator`
2. Implement the required methods, especially the `step()` method
3. Register the integrator in the main program

## Example

```cpp
// Creating and using an integrator
integrators::VelocityVerlet integrator(timestep);
for (std::size_t step = 0; step < numSteps; ++step) {
    integrator.step(system);
    // Other operations like output or analysis
}
```

## References

- Frenkel, D., & Smit, B. (2002). Understanding Molecular Simulation: From Algorithms to Applications.
- Allen, M. P., & Tildesley, D. J. (2017). Computer Simulation of Liquids.
- Leimkuhler, B., & Reich, S. (2004). Simulating Hamiltonian Dynamics.