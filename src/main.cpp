#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <string>

#include "core/box.hpp"
#include "core/system.hpp"
#include "integrators/velocity_verlet.hpp"
#include "utils/config_reader.hpp"

using namespace ljmd;

// Function to save system configuration to a file
void saveConfiguration(const core::System& system, const std::string& filename) {
    std::ofstream file(filename);
    file << system.numParticles() << "\n";
    file << "# Particles in a Lennard-Jones system\n";
    
    // Write box dimensions
    file << system.boxSize().x() << " " 
         << system.boxSize().y() << " " 
         << system.boxSize().z() << "\n";
    
    // Write particle positions
    for (std::size_t i = 0; i < system.numParticles(); ++i) {
        const auto& p = system.getParticle(i);
        file << "LJ " 
             << p.position().x() << " " 
             << p.position().y() << " " 
             << p.position().z() << "\n";
    }
}

// Function to write system properties to a file
void writeProperties(std::ofstream& file, int step, const core::System& system) {
    file << step << " " 
         << system.temperature() << " " 
         << system.kineticEnergy() << " " 
         << system.potentialEnergy() << " " 
         << system.totalEnergy() << "\n";
}

// Function to write energy components to a separate file
void writeEnergy(std::ofstream& file, int step, const core::System& system) {
    file << step << " " 
         << system.kineticEnergy() << " " 
         << system.potentialEnergy() << " " 
         << system.totalEnergy() << "\n";
}

int main(int argc, char* argv[]) {
    std::string configFile = "config.ini";
    
    // Allow overriding config file from command line
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-c" || arg == "--config") {
            if (i + 1 < argc) {
                configFile = argv[i+1];
                break;
            }
        }
    }
    
    // Try to load config file
    try {
        std::cout << "Loading configuration from: " << configFile << std::endl;
        utils::ConfigReader config(configFile);
        
        // Read basic simulation parameters from config
        std::size_t numParticles = config.getInt("num_particles", 125);
        double density = config.getDouble("density", 0.8);
        double temperature = config.getDouble("temperature", 1.0);
        double cutoffRadius = config.getDouble("cutoff_radius", 2.5);
        double timestep = config.getDouble("timestep", 0.001);
        std::size_t equilibrationSteps = config.getInt("equilibration_steps", 1000);
        std::size_t numSteps = config.getInt("production_steps", 5000);
        std::size_t outputInterval = config.getInt("output_interval", 50);
        
        // Read output file configuration
        std::string propsFile = config.getString("properties_file", "properties.dat");
        std::string energyFile = config.getString("energy_file", "energy.dat");
        
        // Read trajectory configuration
        bool writeTrajectory = config.getBool("write_trajectory", true);
        std::string trajFile = writeTrajectory ? config.getString("trajectory_file", "trajectory.xyz") : "";
        std::size_t trajInterval = config.getInt("trajectory_interval", 100);
        
        // Read advanced options
        std::string integratorType = config.getString("integrator", "velocity-verlet");
        std::string thermostatType = config.getString("thermostat", "none");
        unsigned int randomSeed = config.getInt("random_seed", 42);
        double energyTolerance = config.getDouble("energy_tolerance", 0.01);
        
        // Adjust cutoff for small boxes to avoid self-interaction across periodic boundaries
        double minBoxDimension = std::cbrt(numParticles / density);
        if (cutoffRadius > minBoxDimension / 2.0) {
            cutoffRadius = minBoxDimension / 2.0 - 0.1;
            std::cout << "Warning: Adjusted cutoff radius to " << cutoffRadius 
                    << " to avoid self-interaction across periodic boundaries.\n";
        }
        
        // Display simulation parameters
        std::cout << "Molecular Dynamics Simulation with Lennard-Jones Potential\n";
        std::cout << "------------------------------------------------------\n";
        std::cout << "Number of particles: " << numParticles << "\n";
        std::cout << "Density: " << density << "\n";
        std::cout << "Initial temperature: " << temperature << "\n";
        std::cout << "Timestep: " << timestep << "\n";
        std::cout << "Cutoff radius: " << cutoffRadius << "\n";
        std::cout << "Equilibration steps: " << equilibrationSteps << "\n";
        std::cout << "Production steps: " << numSteps << "\n";
        std::cout << "Output interval: " << outputInterval << "\n";
        std::cout << "Integrator: " << integratorType << "\n";
        std::cout << "Thermostat: " << thermostatType << "\n";
        std::cout << "Random seed: " << randomSeed << "\n";
        std::cout << "------------------------------------------------------\n";
        
        // Create the simulation box and system with adjusted cutoff and random seed
        core::Box box;
        auto system = box.createLJSystem(numParticles, temperature, density, cutoffRadius);
        
        // Create integrator based on configuration
        integrators::VelocityVerlet integrator(timestep);  // Currently only one type supported
        
        // Calculate initial forces
        system.calculateForces();
        
        // Open output files
        std::ofstream propertiesFile(propsFile);
        propertiesFile << "# Step Temperature KineticEnergy PotentialEnergy TotalEnergy\n";
        
        std::ofstream energyOutputFile;
        if (!energyFile.empty()) {
            energyOutputFile.open(energyFile);
            energyOutputFile << "# Step KineticEnergy PotentialEnergy TotalEnergy\n";
        }
        
        // Equilibration phase
        std::cout << "Starting equilibration phase...\n";
        
        for (std::size_t step = 1; step <= equilibrationSteps; ++step) {
            // Perform a single integration step
            integrator.step(system);
            
            // Apply thermostat if enabled
            if (thermostatType == "berendsen" && step % 10 == 0) {
                double currentTemp = system.temperature();
                double scaleFactor = std::sqrt(temperature / currentTemp);
                
                for (std::size_t i = 0; i < system.numParticles(); ++i) {
                    auto& particle = system.getParticle(i);
                    particle.setVelocity(particle.velocity() * scaleFactor);
                }
            }
            
            // Output progress
            if (step % (equilibrationSteps / 10) == 0 || step == equilibrationSteps) {
                std::cout << "Equilibration: " << step << "/" << equilibrationSteps 
                        << ", T = " << system.temperature() 
                        << ", E = " << system.totalEnergy() << std::endl;
            }
        }
        
        std::cout << "Equilibration completed.\n";
        std::cout << "------------------------------------------------------\n";
        
        // Store initial energy for drift calculation
        double initialEnergy = system.totalEnergy();
        
        // Write initial configuration and properties
        saveConfiguration(system, "initial.xyz");
        writeProperties(propertiesFile, 0, system);
        
        // Write initial energy if requested
        if (energyOutputFile.is_open()) {
            writeEnergy(energyOutputFile, 0, system);
        }
        
        // Prepare trajectory file if requested
        std::ofstream trajectoryFile;
        if (!trajFile.empty() && writeTrajectory) {
            trajectoryFile.open(trajFile);
        }
        
        // Start timing for production phase
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // Main production simulation loop
        std::cout << "Starting production phase...\n";
        
        for (std::size_t step = 1; step <= numSteps; ++step) {
            // Perform a single integration step
            integrator.step(system);
            
            // Output properties at regular intervals
            if (step % outputInterval == 0) {
                writeProperties(propertiesFile, step, system);
                
                // Write energy components if requested
                if (energyOutputFile.is_open()) {
                    writeEnergy(energyOutputFile, step, system);
                }
                
                // Write trajectory frame if requested
                if (trajectoryFile.is_open() && step % trajInterval == 0) {
                    trajectoryFile << system.numParticles() << "\n";
                    trajectoryFile << "Step " << step << "\n";
                    for (std::size_t i = 0; i < system.numParticles(); ++i) {
                        const auto& p = system.getParticle(i);
                        trajectoryFile << "LJ " 
                                     << p.position().x() << " " 
                                     << p.position().y() << " " 
                                     << p.position().z() << "\n";
                    }
                }
                
                // Display progress with proper scientific notation for better readability
                std::cout << "Step " << std::setw(5) << step 
                        << ", T = " << std::fixed << std::setprecision(4) << system.temperature() 
                        << ", E = " << std::scientific << std::setprecision(6) << system.totalEnergy();
                        
                // Also report energy drift
                double energyDrift = std::abs((system.totalEnergy() - initialEnergy) / initialEnergy);
                std::cout << ", drift = " << std::scientific << std::setprecision(6) << energyDrift;
                
                // Warn if energy drift exceeds tolerance
                if (energyDrift > energyTolerance) {
                    std::cout << " [WARNING: Energy drift exceeds tolerance]";
                }
                
                std::cout << std::endl;
                
                // Safety check: if the simulation becomes unstable, exit gracefully
                if (std::isnan(system.totalEnergy()) || 
                    std::abs(system.totalEnergy()) > 1.0e10 ||
                    energyDrift > 1.0) {
                    std::cout << "Error: Simulation became unstable. Exiting." << std::endl;
                    break;
                }
            }
        }
        
        // End timing
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        // Close output files
        if (trajectoryFile.is_open()) {
            trajectoryFile.close();
        }
        
        if (energyOutputFile.is_open()) {
            energyOutputFile.close();
        }
        
        // Write final configuration
        saveConfiguration(system, "final.xyz");
        
        // Calculate energy drift
        double energyDrift = (system.totalEnergy() - initialEnergy) / initialEnergy;
        
        // Output timing information
        std::cout << "------------------------------------------------------\n";
        std::cout << "Simulation completed in " << duration.count() / 1000.0 << " seconds\n";
        std::cout << "Final temperature: " << system.temperature() << "\n";
        std::cout << "Final total energy: " << system.totalEnergy() << "\n";
        std::cout << "Energy drift: " << energyDrift << "\n";
        std::cout << "------------------------------------------------------\n";
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Usage: " << argv[0] << " [-c|--config configfile]" << std::endl;
        return 1;
    }
}