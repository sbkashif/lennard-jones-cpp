#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace ljmd {
namespace utils {

class ConfigReader {
public:
    // Constructor loads from a file
    explicit ConfigReader(const std::string& filename);
    
    // Get methods with default values
    int getInt(const std::string& key, int defaultValue = 0) const;
    double getDouble(const std::string& key, double defaultValue = 0.0) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    
    // Check if a key exists
    bool hasKey(const std::string& key) const;
    
private:
    std::unordered_map<std::string, std::string> configData_;
    
    // Helper method to trim whitespace from strings
    static std::string trim(const std::string& str);
};

} // namespace utils
} // namespace ljmd