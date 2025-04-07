#include "ConfigurationReader.h"

namespace Utils {

    bool ConfigurationReader::LoadConfiguration(const std::string& filePath) {
        std::ifstream configFile(filePath);
        if (configFile.is_open()) {
            try {
                configFile >> m_Config;
                return true;
            }
            catch (const std::exception& e) {
                std::cerr << "Error parsing configuration file: " << e.what() << std::endl;
            }
        }
        return false;
    }

    nlohmann::json ConfigurationReader::GetConfig() const {
        return m_Config;
    }
}