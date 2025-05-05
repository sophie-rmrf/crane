#include <fstream>
#include <string>
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
		
		if (!conf_json.contains("profiles")) {
			return false;
		}

		nlohmann::json conf_profiles = conf_json["profiles"];
		
		bool contains_debug_profile = conf_profiles.contains("debug");
		bool contains_release_profile = conf_profiles.contains("release");

		return contains_debug_profile || contains_debug_profile;
	}

	std::string to_gcc_command() {

	}

	std::string to_msvc_command() {

	}
}