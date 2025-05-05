#include <fstream>
#include <string>
#include <filesystem>
#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>
#include "headers/config.h"
#include "deps/json.hpp"

// Relative path to the config
#define DEFAULT_CONFIG_PATH "config.crane.json"

namespace config {
	namespace gcc {
		std::string to_command(nlohmann::json config, std::string profile, bool should_run = false) {
			nlohmann::json sources = config["source_files"];
			nlohmann::json profile_json = config["profiles"][profile];
			std::string command = "g++ ";

			process_optimize_level(profile_json, command);
			process_sources(sources, command);
			process_include_folders(config, command);
			process_define(profile_json, command);
			process_cpp_std(profile_json, command);
			std::string name = process_output_name(profile_json, command);

			if (should_run) {
				command.append("&& " + name);
			}

			return command;
		}
		void process_optimize_level(const nlohmann::json& profile_json, std::string& command) {
			if (profile_json.contains("optimize_level")) {
				int optimize_level = profile_json["optimize_level"].get<int>();
				command.append(get_optimize_flag(optimize_level) + " ");
			}
		}
		void process_sources(const nlohmann::json& sources, std::string& command) {
			for (std::string file_path : sources) {
				command.append(file_path + " ");
			}
		}
		void process_include_folders(const nlohmann::json& config, std::string& command) {
			if (config.contains("include_folders")) {
				nlohmann::json folders = config["include_folders"];

				for (std::string folder_path : folders) {
					command.append("-I " + folder_path + " ");
				}
			};
		}
		std::string process_output_name(const nlohmann::json& profile_json, std::string& command) {
			if (profile_json.contains("output_name")) {
				std::string output_name = profile_json["output_name"].get<std::string>();
				command.append("-o " + output_name + " ");
				return output_name;
			} else {
				return "a.out";
			}
		}
		void process_cpp_std(const nlohmann::json& profile_json, std::string& command) {
			if (profile_json.contains("cpp_std")) {
				command.append("--std=c++" + profile_json["output_name"].get<std::string>() + " ");
			}
			else {
				command.append("--std=c++11 ");
			}
		}
		void process_define(const nlohmann::json& profile_json, std::string& command) {
			if (profile_json.contains("define")) {
				nlohmann::json defines = profile_json["define"];

				for (std::string define : defines) {
					command.append("-D " + define + " ");
				}
			};
		}
		std::string get_optimize_flag(int level) {
			if (level > 3 || level < 0) {
				throw std::exception("Wrong optimization level: expected 0..3, got " + level);
			}

			return "-O" + std::to_string(level);
		}
	}

	namespace msvc {
		std::string to_command(nlohmann::json config, std::string profile) {
			return "";
		}
	}
	
	namespace generic {
		bool is_valid_config() noexcept {
			std::ifstream conf_file(DEFAULT_CONFIG_PATH);
			nlohmann::json conf_json;

			try {
				conf_json = nlohmann::json::parse(conf_file);
			}
			catch (std::exception&) {
				return false;
			}

			if (!conf_json.contains("profiles")) {
				return false;
			}

			nlohmann::json conf_profiles = conf_json["profiles"];

			bool contains_source_files = conf_json.contains("source_files");
			bool contains_debug_profile = conf_profiles.contains("debug");
			bool contains_release_profile = conf_profiles.contains("release");

			return (contains_debug_profile || contains_release_profile) && contains_source_files;
		}
		void install_gcc() {
			const std::wstring install_path = L"C:\\w64devkit";
			const std::string download_url = "https://github.com/skeeto/w64devkit/releases/download/v1.19.0/w64devkit-1.19.0.zip";
			const std::string zip_file_name = "w64devkit.zip";
			const std::string extracted_folder = "C:\\w64devkit-1.19.0";
			const std::string bin_path = "C:\\w64devkit\\bin";

			if (!std::filesystem::exists(install_path)) {
				std::cout << "Downloading w64devkit..." << "\n";
				std::string download_cmd = "curl -L -o " + zip_file_name + " " + download_url;
				if (system(download_cmd.c_str()) != 0) {
					throw std::exception("Failed to download compiler");
				}

				std::cout << "Extracting to C:\\..." << std::endl;
				std::string extract_cmd = "powershell -command Expand-Archive -Path " +
					zip_file_name + " -DestinationPath C:\\";

				if (system(extract_cmd.c_str()) != 0) {
					throw std::exception("Failed to extract archive");
				}

				system(("del " + zip_file_name).c_str());
			} else {
				std::cout << "w64devkit already installed" << std::endl;
			}

			std::cout << "Updating PATH..." << "\n";
			std::string path_update_cmd =
					"powershell -command "
					"$path = [Environment]::GetEnvironmentVariable('Path', 'User');"
					"if ($path -notlike '" + bin_path + "') {"
					"   [Environment]::SetEnvironmentVariable("
					"       'Path',"
					"       ($path + ';' + '" + bin_path + "'),"
					"       'User'"
					"   )"
					"}";

			if (system(path_update_cmd.c_str()) != 0) {
				throw std::exception("PATH update failed (might need admin rights)");
			}

			system(("set PATH=%PATH%;" + bin_path).c_str());
			
			std::cout << "\nDone. There's a good chance that g++ will start working right away!\n";
			std::cout << "If not, launch a new command prompt window.\n";
			std::cout << "Installed to: C:\\w64devkit\n";
		}
	}
}