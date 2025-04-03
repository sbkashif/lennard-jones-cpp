#pragma once

#include <cmath>
#include <iostream>

namespace ljmd {
namespace utils {

class Vector3D {
public:
    // Constructors
    Vector3D() : x_(0.0), y_(0.0), z_(0.0) {}
    Vector3D(double x, double y, double z) : x_(x), y_(y), z_(z) {}
    
    // Getters and setters
    double x() const { return x_; }
    double y() const { return y_; }
    double z() const { return z_; }
    void setX(double x) { x_ = x; }
    void setY(double y) { y_ = y; }
    void setZ(double z) { z_ = z; }
    
    // Vector operations
    Vector3D& operator+=(const Vector3D& rhs);
    Vector3D& operator-=(const Vector3D& rhs);
    Vector3D& operator*=(double scalar);
    Vector3D& operator/=(double scalar);
    
    // Unary minus operator
    Vector3D operator-() const {
        return Vector3D(-x_, -y_, -z_);
    }
    
    // Vector properties
    double norm() const;
    double normSquared() const;
    void normalize();
    Vector3D normalized() const;
    
    // Periodic boundary conditions
    void applyPeriodicBC(const Vector3D& boxSize);
    
private:
    double x_, y_, z_;
};

// Non-member functions
Vector3D operator+(Vector3D lhs, const Vector3D& rhs);
Vector3D operator-(Vector3D lhs, const Vector3D& rhs);
Vector3D operator*(Vector3D vec, double scalar);
Vector3D operator*(double scalar, Vector3D vec);
Vector3D operator/(Vector3D vec, double scalar);
double dot(const Vector3D& a, const Vector3D& b);
Vector3D cross(const Vector3D& a, const Vector3D& b);
std::ostream& operator<<(std::ostream& os, const Vector3D& vec);

} // namespace utils
} // namespace ljmd