#pragma once

#include <string>
#include <unordered_map>
#include <d2d1.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp> 

namespace Utils {
	class ConfigurationReader {
	public:
		bool LoadConfiguration(const std::string& filePath);
		nlohmann::json GetConfig() const;

	private:
		nlohmann::json m_Config;
	};
}