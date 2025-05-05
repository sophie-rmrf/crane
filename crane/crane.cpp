#include <iostream>
#include <filesystem>
#include <fstream>
#include "headers/config.h"

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	std::string arg1(argv[1]);
	std::string arg2(argv[2]);


	std::ifstream conf_file(std::filesystem::absolute("config.crane.json"));
	nlohmann::json conf_json;

	if (!conf_file.is_open()) {
		std::cout << "Failed to open config file\n";
		return 1;
	}

	try {
		conf_json = nlohmann::json::parse(conf_file);
	}
	catch (const std::exception& e) {
		std::cout << e.what();
	}

	if (arg1 == "setup") {
		config::generic::install_gcc();
	}
	else if (arg1 == "run") {
		system((config::gcc::to_command(conf_json, arg2, true)).c_str());
	}
}