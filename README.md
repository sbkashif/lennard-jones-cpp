# Lennard-Jones Molecular Dynamics Simulation

A modular C++ library for molecular dynamics simulations of Lennard-Jones particles. This project implements a basic MD engine that focuses on modularity and extensibility, using modern C++ practices.

## Features

- Simulation of Lennard-Jones fluids in 3D with periodic boundary conditions
- NVE ensemble simulation using Velocity Verlet integrator
- Energy minimization capabilities to optimize initial configurations
- Modular design that enables easy extension with:
  - Different integrators
  - Various thermostats and barostats
  - Additional force fields
- XYZ file output for visualization with external tools
- Performance optimized force calculation
- Configurable simulation parameters via config file

## Requirements

- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake 3.14 or higher
- (Optional) Google Test for running unit tests

## Building the Project

```bash
# Clone the repository
git clone https://github.com/yourusername/lennard-jones-cpp.git
cd lennard-jones-cpp

# Create and enter build directory
mkdir -p build
cd build

# Configure and build
cmake ..
make
```

## Running a Simulation

The simulation can be run with various parameters:

```bash
./ljmd [options]

Options:
  -n, --num-particles   Number of particles (default: 125)
  -d, --density         System density in reduced units (default: 0.8)
  -t, --temperature     Initial temperature in reduced units (default: 1.0)
  -dt, --timestep       Integration time step (default: 0.005)
  -s, --steps           Number of simulation steps (default: 5000)
  -o, --output          Output interval (default: 100)
  -c, --config          Path to configuration file
```

Example:

```bash
./ljmd -n 512 -d 0.8 -t 1.0 -s 10000
./ljmd -c path/to/config.ini
```

## Configuration Options

The simulation can be configured using a config.ini file. Below is a list of supported configuration options:

### System Parameters
- `num_particles`: Number of particles in the simulation
- `density`: Density in reduced units
- `temperature`: Initial temperature in reduced units
- `cutoff_radius`: Cutoff radius for LJ potential
- `minimize_energy`: Whether to perform energy minimization before simulation (default: false)
- `minimization_steps`: Maximum number of steps for energy minimization (default: 1000)
- `minimization_tolerance`: Convergence tolerance for energy minimization (default: 1e-6)

### Simulation Parameters
- `timestep`: Integration timestep
- `equilibration_steps`: Number of equilibration steps
- `production_steps`: Number of production steps
- `output_interval`: How often to output data

### Output Parameters
- `trajectory_file`: Path to trajectory output file
- `properties_file`: Path to properties output file
- `energy_file`: Path to energy components output file

### Visualization Options
- `write_trajectory`: Whether to write trajectory file
- `trajectory_interval`: How often to write frames to trajectory file

### Advanced Options
- `integrator`: Integration algorithm (currently only velocity-verlet is supported)
- `thermostat`: Thermostat type (none, berendsen)
- `random_seed`: Seed for random number generator
- `energy_tolerance`: Maximum allowed energy drift before warning

## Example Configuration File

```ini
# Lennard-Jones MD Simulation Configuration

# System parameters
num_particles = 125    # Number of particles
density = 0.8          # Density in reduced units 
temperature = 1.0      # Initial temperature in reduced units
cutoff_radius = 2.5    # Cutoff radius for LJ potential

# Simulation parameters
timestep = 0.001       # Integration timestep
equilibration_steps = 1000  # Number of equilibration steps
production_steps = 5000     # Number of production steps
output_interval = 50   # How often to output data

# Output parameters
trajectory_file = "trajectory.xyz"  # Trajectory output file
properties_file = "properties.dat"  # Properties output file
energy_file = "energy.dat"          # Energy components output file

# Visualization options
write_trajectory = true     # Whether to write trajectory file
trajectory_interval = 100   # How often to write frames to trajectory file

# Advanced options
integrator = "velocity-verlet"  # Integration algorithm
thermostat = "none"      # Thermostat (none, berendsen)
random_seed = 42         # Seed for random number generator
energy_tolerance = 0.01  # Maximum allowed energy drift before warning
```

## Output Files

- `initial.xyz` - Initial configuration of the system
- `minimized.xyz` - Configuration after energy minimization (if enabled)
- `final.xyz` - Final configuration of the system
- `trajectory.xyz` - Complete trajectory for visualization
- `properties.dat` - Time series of system properties (temperature, energy, etc.)
- `energy.dat` - Detailed energy components over time

## Extending the Project

### Adding a New Integrator

1. Create a new class that inherits from the `Integrator` base class
2. Implement the `step()` method to update the system according to your integration scheme
3. Register the new integrator in the main program

### Adding a Minimizer

1. Create a new class that inherits from the `Minimizer` base class
2. Implement the energy minimization algorithm
3. Register the new minimizer in the main program

### Adding a Thermostat

1. Create a new class that inherits from a `Thermostat` base class
2. Implement the temperature control algorithm
3. Modify the integration step to include temperature regulation

## Visualization

The output XYZ files can be visualized with tools like:
- VMD
- OVITO
- PyMOL

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- This project uses reduced Lennard-Jones units where σ = ε = m = 1
- The Velocity Verlet implementation follows the algorithm described in Frenkel & Smit's "Understanding Molecular Simulation"