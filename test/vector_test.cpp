#include <gtest/gtest.h>
#include "utils/vector3d.hpp"

using namespace ljmd::utils;

// Test Vector3D constructors
TEST(Vector3DTest, Construction) {
    // Default constructor
    Vector3D v1;
    EXPECT_DOUBLE_EQ(0.0, v1.x());
    EXPECT_DOUBLE_EQ(0.0, v1.y());
    EXPECT_DOUBLE_EQ(0.0, v1.z());
    
    // Constructor with parameters
    Vector3D v2(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(1.0, v2.x());
    EXPECT_DOUBLE_EQ(2.0, v2.y());
    EXPECT_DOUBLE_EQ(3.0, v2.z());
}

// Test Vector3D arithmetic operations
TEST(Vector3DTest, ArithmeticOperations) {
    Vector3D v1(1.0, 2.0, 3.0);
    Vector3D v2(2.0, 3.0, 4.0);
    
    // Addition
    Vector3D sum = v1 + v2;
    EXPECT_DOUBLE_EQ(3.0, sum.x());
    EXPECT_DOUBLE_EQ(5.0, sum.y());
    EXPECT_DOUBLE_EQ(7.0, sum.z());
    
    // Subtraction
    Vector3D diff = v2 - v1;
    EXPECT_DOUBLE_EQ(1.0, diff.x());
    EXPECT_DOUBLE_EQ(1.0, diff.y());
    EXPECT_DOUBLE_EQ(1.0, diff.z());
    
    // Scalar multiplication
    Vector3D mult = v1 * 2.0;
    EXPECT_DOUBLE_EQ(2.0, mult.x());
    EXPECT_DOUBLE_EQ(4.0, mult.y());
    EXPECT_DOUBLE_EQ(6.0, mult.z());
    
    // Scalar division
    Vector3D div = v2 / 2.0;
    EXPECT_DOUBLE_EQ(1.0, div.x());
    EXPECT_DOUBLE_EQ(1.5, div.y());
    EXPECT_DOUBLE_EQ(2.0, div.z());
}

// Test Vector3D properties
TEST(Vector3DTest, VectorProperties) {
    Vector3D v(3.0, 4.0, 0.0);
    
    // Norm squared
    EXPECT_DOUBLE_EQ(25.0, v.normSquared());
    
    // Norm
    EXPECT_DOUBLE_EQ(5.0, v.norm());
    
    // Normalization
    Vector3D unit = v.normalized();
    EXPECT_DOUBLE_EQ(0.6, unit.x());
    EXPECT_DOUBLE_EQ(0.8, unit.y());
    EXPECT_DOUBLE_EQ(0.0, unit.z());
    
    // Dot product
    Vector3D v2(1.0, 2.0, 3.0);
    double dotProduct = dot(v, v2);
    EXPECT_DOUBLE_EQ(11.0, dotProduct);
    
    // Cross product
    Vector3D crossProduct = cross(v, v2);
    EXPECT_DOUBLE_EQ(12.0, crossProduct.x());
    EXPECT_DOUBLE_EQ(-9.0, crossProduct.y());
    EXPECT_DOUBLE_EQ(2.0, crossProduct.z());
}

// Test periodic boundary conditions
TEST(Vector3DTest, PeriodicBoundary) {
    Vector3D boxSize(10.0, 10.0, 10.0);
    
    // Test position outside box (positive direction)
    Vector3D v1(16.0, 13.0, 12.0);
    v1.applyPeriodicBC(boxSize);
    EXPECT_NEAR(6.0, v1.x(), 1e-10);
    EXPECT_NEAR(3.0, v1.y(), 1e-10);
    EXPECT_NEAR(2.0, v1.z(), 1e-10);
    
    // Test position outside box (negative direction)
    Vector3D v2(-8.0, -11.0, -13.0);
    v2.applyPeriodicBC(boxSize);
    EXPECT_NEAR(2.0, v2.x(), 1e-10);
    EXPECT_NEAR(9.0, v2.y(), 1e-10);
    EXPECT_NEAR(7.0, v2.z(), 1e-10);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}