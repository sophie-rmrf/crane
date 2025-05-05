#pragma once

#include <string>
#include "../deps/json.hpp"

namespace config {
	namespace gcc {
		std::string to_command(nlohmann::json config, std::string profile, bool should_run);
		void process_optimize_level(const nlohmann::json& profile_json, std::string& command);
		void process_sources(const nlohmann::json& sources, std::string& command);
		void process_include_folders(const nlohmann::json& config, std::string& command);
		std::string process_output_name(const nlohmann::json& profile_json, std::string& command);
		void process_cpp_std(const nlohmann::json& profile_json, std::string& command);
		void process_define(const nlohmann::json& profile_json, std::string& command);
		std::string get_optimize_flag(int level);
	}
	namespace msvc {
		std::string to_command(nlohmann::json config, std::string profile);
	}
	namespace generic {
		bool is_valid_config() noexcept;
		void install_gcc();
	}
}