#include "utils/vector3d.hpp"

namespace ljmd {
namespace utils {

Vector3D& Vector3D::operator+=(const Vector3D& rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& rhs) {
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
    return *this;
}

Vector3D& Vector3D::operator*=(double scalar) {
    x_ *= scalar;
    y_ *= scalar;
    z_ *= scalar;
    return *this;
}

Vector3D& Vector3D::operator/=(double scalar) {
    x_ /= scalar;
    y_ /= scalar;
    z_ /= scalar;
    return *this;
}

double Vector3D::normSquared() const {
    return x_*x_ + y_*y_ + z_*z_;
}

double Vector3D::norm() const {
    return std::sqrt(normSquared());
}

void Vector3D::normalize() {
    double n = norm();
    if (n > 0) {
        *this /= n;
    }
}

Vector3D Vector3D::normalized() const {
    Vector3D result = *this;
    result.normalize();
    return result;
}

void Vector3D::applyPeriodicBC(const Vector3D& boxSize) {
    // Apply periodic boundary conditions using modulo arithmetic approach
    // This handles both positive and negative coordinates correctly
    x_ = fmod(fmod(x_, boxSize.x()) + boxSize.x(), boxSize.x());
    y_ = fmod(fmod(y_, boxSize.y()) + boxSize.y(), boxSize.y());
    z_ = fmod(fmod(z_, boxSize.z()) + boxSize.z(), boxSize.z());
}

// Non-member functions
Vector3D operator+(Vector3D lhs, const Vector3D& rhs) {
    lhs += rhs;
    return lhs;
}

Vector3D operator-(Vector3D lhs, const Vector3D& rhs) {
    lhs -= rhs;
    return lhs;
}

Vector3D operator*(Vector3D vec, double scalar) {
    vec *= scalar;
    return vec;
}

Vector3D operator*(double scalar, Vector3D vec) {
    vec *= scalar;
    return vec;
}

Vector3D operator/(Vector3D vec, double scalar) {
    vec /= scalar;
    return vec;
}

double dot(const Vector3D& a, const Vector3D& b) {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

Vector3D cross(const Vector3D& a, const Vector3D& b) {
    return Vector3D(
        a.y() * b.z() - a.z() * b.y(),
        a.z() * b.x() - a.x() * b.z(),
        a.x() * b.y() - a.y() * b.x()
    );
}

std::ostream& operator<<(std::ostream& os, const Vector3D& vec) {
    os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")";
    return os;
}

} // namespace utils
} // namespace ljmd