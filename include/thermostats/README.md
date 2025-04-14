# Thermostats Module

This module provides temperature control algorithms for molecular dynamics simulations, enabling simulations in canonical (NVT) and other ensembles.

## Overview

Thermostats are algorithms that regulate the temperature of a molecular system by modifying the velocities of particles. They are essential for simulating systems at constant temperature (canonical ensemble) rather than constant energy (microcanonical ensemble).

## Available Thermostats

### Berendsen Thermostat

The Berendsen thermostat provides an exponential relaxation of the system temperature towards a desired temperature.

**Key features:**
- Simple implementation
- Smooth temperature transitions
- Weak coupling approach
- Does not strictly preserve canonical ensemble statistics
- Good for equilibration

**Implementation details:**
The algorithm scales velocities by a factor λ:
```
λ = sqrt(1 + (Δt/τ) * (T₀/T - 1))
```
where:
- Δt is the timestep
- τ is the coupling time constant
- T₀ is the target temperature
- T is the current temperature

### Nosé-Hoover Thermostat (Coming Soon)

The Nosé-Hoover thermostat is more rigorous and properly samples the canonical ensemble.

**Key features:**
- Correctly samples the canonical ensemble
- Uses an extended system approach
- More complex implementation
- Includes an additional degree of freedom

### Andersen Thermostat (Coming Soon)

The Andersen thermostat mimics collisions with a heat bath by randomly reassigning particle velocities.

**Key features:**
- Correctly samples the canonical ensemble
- Stochastic approach
- Disrupts dynamics and time correlation functions
- Simple implementation

## Usage

To use a thermostat in your simulation, set the following parameters in your `config.ini` file:

```ini
# Thermostat parameters
thermostat = "berendsen"        # Options: "none", "berendsen", "nose-hoover", "andersen"
target_temperature = 1.0        # Target temperature in reduced units
coupling_time = 0.1             # Coupling time constant for applicable thermostats
collision_frequency = 10.0      # For stochastic thermostats like Andersen
```

## Implementation Details

All thermostats inherit from a `Thermostat` base class that provides common functionality:

- `apply(System& system)`: Apply the thermostat to the system
- Temperature monitoring and tracking
- Target temperature management

## Adding New Thermostats

To add a new thermostat algorithm:

1. Create a new class that inherits from `Thermostat`
2. Implement the required methods, especially the `apply()` method
3. Register the thermostat in the main program

## Example

```cpp
// Creating and using a thermostat
thermostats::Berendsen thermostat(targetTemp, couplingTime);
for (std::size_t step = 0; step < numSteps; ++step) {
    integrator.step(system);
    if (step % thermostatInterval == 0) {
        thermostat.apply(system);
    }
    // Other operations like output or analysis
}
```

## References

- Berendsen, H. J. C., et al. (1984). Molecular dynamics with coupling to an external bath. J. Chem. Phys., 81, 3684-3690.
- Nosé, S. (1984). A unified formulation of the constant temperature molecular dynamics methods. J. Chem. Phys., 81, 511-519.
- Andersen, H. C. (1980). Molecular dynamics simulations at constant pressure and/or temperature. J. Chem. Phys., 72, 2384-2393.
- Frenkel, D., & Smit, B. (2002). Understanding Molecular Simulation: From Algorithms to Applications.