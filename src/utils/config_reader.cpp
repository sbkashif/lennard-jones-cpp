#include "utils/config_reader.hpp"
#include <algorithm>

namespace ljmd {
namespace utils {

ConfigReader::ConfigReader(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open configuration file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        line = trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Parse key-value pairs
        size_t delimPos = line.find('=');
        if (delimPos != std::string::npos) {
            std::string key = trim(line.substr(0, delimPos));
            std::string value = trim(line.substr(delimPos + 1));
            
            // Remove quotes from string values
            if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.size() - 2);
            }

            configData_[key] = value;
        }
    }
}

int ConfigReader::getInt(const std::string& key, int defaultValue) const {
    auto it = configData_.find(key);
    if (it != configData_.end()) {
        try {
            return std::stoi(it->second);
        } catch (const std::exception& e) {
            return defaultValue;
        }
    }
    return defaultValue;
}

double ConfigReader::getDouble(const std::string& key, double defaultValue) const {
    auto it = configData_.find(key);
    if (it != configData_.end()) {
        try {
            return std::stod(it->second);
        } catch (const std::exception& e) {
            return defaultValue;
        }
    }
    return defaultValue;
}

bool ConfigReader::getBool(const std::string& key, bool defaultValue) const {
    auto it = configData_.find(key);
    if (it != configData_.end()) {
        std::string value = it->second;
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        return value == "true" || value == "yes" || value == "1";
    }
    return defaultValue;
}

std::string ConfigReader::getString(const std::string& key, const std::string& defaultValue) const {
    auto it = configData_.find(key);
    if (it != configData_.end()) {
        return it->second;
    }
    return defaultValue;
}

bool ConfigReader::hasKey(const std::string& key) const {
    return configData_.find(key) != configData_.end();
}

std::string ConfigReader::trim(const std::string& str) {
    const auto begin = str.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) {
        return "";
    }
    
    const auto end = str.find_last_not_of(" \t\r\n");
    return str.substr(begin, end - begin + 1);
}

} // namespace utils
} // namespace ljmd