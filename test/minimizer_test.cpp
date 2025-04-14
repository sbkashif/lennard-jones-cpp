#include <gtest/gtest.h>
#include "minimizers/minimizer.hpp"
#include "minimizers/steepest_descent.hpp"
#include "minimizers/minimizer_factory.hpp"
#include "core/system.hpp"
#include "core/box.hpp"

using namespace ljmd;

// Test fixture for minimizer tests
class MinimizerTest : public ::testing::Test {
protected:
    MinimizerTest() : box(std::make_unique<core::Box>()) {}
    
    void SetUp() override {
        // Create a small system with particles close together (high energy)
        system = box->createLJSystem(8, 1.0, 0.5, 2.5);
        
        // Manually position particles to create some high-energy interactions
        // (particles too close together)
        system.getParticle(0).setPosition(utils::Vector3D(0.0, 0.0, 0.0));
        system.getParticle(1).setPosition(utils::Vector3D(0.8, 0.0, 0.0)); // Too close to particle 0
        system.getParticle(2).setPosition(utils::Vector3D(0.0, 0.8, 0.0)); // Too close to particle 0
        system.getParticle(3).setPosition(utils::Vector3D(0.0, 0.0, 0.8)); // Too close to particle 0
        system.getParticle(4).setPosition(utils::Vector3D(2.0, 2.0, 2.0));
        system.getParticle(5).setPosition(utils::Vector3D(2.8, 2.0, 2.0)); // Too close to particle 4
        system.getParticle(6).setPosition(utils::Vector3D(2.0, 2.8, 2.0)); // Too close to particle 4
        system.getParticle(7).setPosition(utils::Vector3D(2.0, 2.0, 2.8)); // Too close to particle 4
        
        // Calculate initial forces and energy
        system.calculateForces();
        initialEnergy = system.potentialEnergy();
    }
    
    std::unique_ptr<core::Box> box;
    core::System system = core::System(utils::Vector3D(10.0, 10.0, 10.0), 2.5); // Initialize with box size
    double initialEnergy = 0.0;
};

// Test that steepest descent minimization reduces the system's energy
TEST_F(MinimizerTest, SteepestDescentReducesEnergy) {
    // Create a steepest descent minimizer with test parameters
    minimizers::SteepestDescent minimizer;
    minimizer.setMaxSteps(100);      // Limit steps for test
    minimizer.setTolerance(1e-4);    // Reasonable tolerance for test
    minimizer.setStepSize(0.01);     // Small initial step size
    
    // Run the minimization
    minimizer.minimize(system);
    
    // Verify that energy was reduced
    double finalEnergy = system.potentialEnergy();
    EXPECT_LT(finalEnergy, initialEnergy);
    
    // Verify that forces are smaller after minimization
    double finalMaxForce = 0.0;
    
    for (std::size_t i = 0; i < system.numParticles(); ++i) {
        double forceMag = system.getParticle(i).force().norm();
        finalMaxForce = std::max(finalMaxForce, forceMag);
    }
    
    // The maximum force should be significantly smaller after minimization
    EXPECT_LT(finalMaxForce, 10.0);
    
    // Check that particles that were too close have moved apart
    double minDistance = std::numeric_limits<double>::max();
    
    for (std::size_t i = 0; i < system.numParticles(); ++i) {
        for (std::size_t j = i + 1; j < system.numParticles(); ++j) {
            utils::Vector3D rij = system.getParticle(j).position() - system.getParticle(i).position();
            rij.applyPeriodicBC(system.boxSize());
            double distance = rij.norm();
            minDistance = std::min(minDistance, distance);
        }
    }
    
    // No particles should be extremely close together after minimization
    // The optimal distance for LJ is around 1.122 (minimum of potential)
    EXPECT_GT(minDistance, 0.9);
}

// Test that the minimizer factory correctly creates a steepest descent minimizer
TEST_F(MinimizerTest, MinimizerFactoryCreatesSteepestDescent) {
    // Create a minimizer using the factory with "steepest" keyword
    auto minimizer = minimizers::MinimizerFactory::create("steepest");
    
    // Configure and run the minimization
    minimizer->setMaxSteps(50);
    minimizer->setTolerance(1e-3);
    minimizer->minimize(system);
    
    // Verify that energy was reduced
    double finalEnergy = system.potentialEnergy();
    EXPECT_LT(finalEnergy, initialEnergy);
    
    // Test alternative name
    auto minimizer2 = minimizers::MinimizerFactory::create("steepest-descent");
    minimizer2->setMaxSteps(50);
    minimizer2->setTolerance(1e-3);
    minimizer2->minimize(system);
    
    // Verify that energy was reduced
    EXPECT_LT(system.potentialEnergy(), initialEnergy);
    
    // Test that factory throws for unknown minimizer type
    EXPECT_THROW(minimizers::MinimizerFactory::create("unknown-minimizer"), std::runtime_error);
}

// Test minimizer convergence
TEST_F(MinimizerTest, MinimizerConvergence) {
    minimizers::SteepestDescent minimizer;
    minimizer.setMaxSteps(1000);     // Many steps to ensure convergence
    minimizer.setTolerance(1e-6);    // Tight tolerance
    
    // Run the minimization
    minimizer.minimize(system);
    
    // Store the energy after first minimization
    double firstEnergy = system.potentialEnergy();
    
    // Running minimization again should result in minimal change
    minimizer.minimize(system);
    double secondEnergy = system.potentialEnergy();
    
    // Energy difference should be very small
    EXPECT_NEAR(firstEnergy, secondEnergy, 1e-4);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}