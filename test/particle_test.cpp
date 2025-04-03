#include <gtest/gtest.h>
#include "core/particle.hpp"
#include "utils/vector3d.hpp"

using namespace ljmd::core;
using namespace ljmd::utils;

// Test Particle constructors
TEST(ParticleTest, Construction) {
    // Default constructor
    Particle p1;
    EXPECT_DOUBLE_EQ(1.0, p1.mass());
    EXPECT_DOUBLE_EQ(0.0, p1.position().x());
    EXPECT_DOUBLE_EQ(0.0, p1.position().y());
    EXPECT_DOUBLE_EQ(0.0, p1.position().z());
    
    // Constructor with mass
    Particle p2(2.0);
    EXPECT_DOUBLE_EQ(2.0, p2.mass());
    
    // Constructor with mass and position
    Vector3D pos(1.0, 2.0, 3.0);
    Particle p3(2.0, pos);
    EXPECT_DOUBLE_EQ(2.0, p3.mass());
    EXPECT_DOUBLE_EQ(1.0, p3.position().x());
    EXPECT_DOUBLE_EQ(2.0, p3.position().y());
    EXPECT_DOUBLE_EQ(3.0, p3.position().z());
}

// Test Particle methods
TEST(ParticleTest, Methods) {
    Particle p(2.0);
    
    // Set and get position
    Vector3D pos(1.0, 2.0, 3.0);
    p.setPosition(pos);
    EXPECT_DOUBLE_EQ(1.0, p.position().x());
    EXPECT_DOUBLE_EQ(2.0, p.position().y());
    EXPECT_DOUBLE_EQ(3.0, p.position().z());
    
    // Set and get velocity
    Vector3D vel(0.5, 0.6, 0.7);
    p.setVelocity(vel);
    EXPECT_DOUBLE_EQ(0.5, p.velocity().x());
    EXPECT_DOUBLE_EQ(0.6, p.velocity().y());
    EXPECT_DOUBLE_EQ(0.7, p.velocity().z());
    
    // Apply force
    Vector3D f1(1.0, 1.0, 1.0);
    p.resetForces();
    p.applyForce(f1);
    EXPECT_DOUBLE_EQ(1.0, p.force().x());
    EXPECT_DOUBLE_EQ(1.0, p.force().y());
    EXPECT_DOUBLE_EQ(1.0, p.force().z());
    
    // Apply another force
    Vector3D f2(2.0, 3.0, 4.0);
    p.applyForce(f2);
    EXPECT_DOUBLE_EQ(3.0, p.force().x());
    EXPECT_DOUBLE_EQ(4.0, p.force().y());
    EXPECT_DOUBLE_EQ(5.0, p.force().z());
    
    // Reset forces
    p.resetForces();
    EXPECT_DOUBLE_EQ(0.0, p.force().x());
    EXPECT_DOUBLE_EQ(0.0, p.force().y());
    EXPECT_DOUBLE_EQ(0.0, p.force().z());
    
    // Kinetic energy
    EXPECT_DOUBLE_EQ(0.5 * 2.0 * (0.5*0.5 + 0.6*0.6 + 0.7*0.7), p.kineticEnergy());
}

// Test Particle update method
TEST(ParticleTest, Update) {
    Particle p(1.0);
    Vector3D pos(1.0, 1.0, 1.0);
    Vector3D vel(0.5, 0.0, 0.0);
    Vector3D box(10.0, 10.0, 10.0);
    
    p.setPosition(pos);
    p.setVelocity(vel);
    
    p.update(2.0, box);
    
    // Position should be updated based on velocity
    EXPECT_DOUBLE_EQ(2.0, p.position().x());
    EXPECT_DOUBLE_EQ(1.0, p.position().y());
    EXPECT_DOUBLE_EQ(1.0, p.position().z());
    
    // Test periodic boundary conditions
    Vector3D pos2(9.0, 9.0, 9.0);
    Vector3D vel2(0.5, 0.5, 0.5);
    p.setPosition(pos2);
    p.setVelocity(vel2);
    
    p.update(2.0, box);
    
    // Position should wrap around periodic boundaries
    EXPECT_NEAR(0.0, p.position().x(), 1e-10);
    EXPECT_NEAR(0.0, p.position().y(), 1e-10);
    EXPECT_NEAR(0.0, p.position().z(), 1e-10);
}