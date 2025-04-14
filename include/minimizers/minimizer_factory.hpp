#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include "minimizers/minimizer.hpp"
#include "minimizers/steepest_descent.hpp"

namespace ljmd {
namespace minimizers {

class MinimizerFactory {
public:
    static std::unique_ptr<Minimizer> create(const std::string& type) {
        if (type == "steepest" || type == "steepest-descent") {
            return std::make_unique<SteepestDescent>();
        } 
        // Future minimizers can be added here with additional else-if statements
        // else if (type == "conjugate-gradient") {
        //     return std::make_unique<ConjugateGradient>();
        // }
        
        throw std::runtime_error("Unknown minimizer type: " + type);
    }
};

} // namespace minimizers
} // namespace ljmd