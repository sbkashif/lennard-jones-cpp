#pragma once

#include "core/system.hpp"
#include "utils/vector3d.hpp"

namespace ljmd {
namespace core {

class Box {
public:
    // Constructors
    Box() = default;
    Box(const utils::Vector3D& size) : size_(size) {}
    Box(double sideLength) : size_(sideLength, sideLength, sideLength) {}

    // Getters and setters
    const utils::Vector3D& size() const { return size_; }
    void setSize(const utils::Vector3D& size) { size_ = size; }

    // Box initialization methods
    System createLJSystem(std::size_t numParticles, double temperature, 
                          double density = 0.8, double cutoffRadius = 2.5);

private:
    utils::Vector3D size_;
};

} // namespace core
} // namespace ljmd