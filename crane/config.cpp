#include <fstream>
#include "deps/json.hpp"

// Relative path to the config
#define DEFAULT_CONFIG_PATH "config.crane.json"

namespace config {
	bool is_valid() noexcept {
		std::ifstream conf_file(DEFAULT_CONFIG_PATH);
		nlohmann::json conf_json;

		try {
			conf_json = nlohmann::json::parse(conf_file);
		} catch (const std::exception& e) {
			return false;
		}
		
		bool containsDebugProfile = conf_json.contains("debug");
		bool containsReleaseProfile = conf_json.contains("release");

		return containsDebugProfile || containsReleaseProfile;
	}


}