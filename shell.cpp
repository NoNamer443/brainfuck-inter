#include <iterator> // std::istreambuf_iterator
#include <iostream> // std::cout, std::getline, std::endl, std::cin, std::cerr
#include <string> // std::string
#include <fstream> // std::ifstream
#include "cxxopts.hpp" // cxxopts::Options, cxxopts::value
#include "brainfuck.hpp" // bf::Brainfuck

int main(int argc, char** argv) {

	bf::Brainfuck shell;
	
	
	cxxopts::Options options("Brainfuck", "A Brainfuck shell");
	options.add_options()
		("i,input", "Input file's path", cxxopts::value<std::string>()->default_value(""))
		("h,help", "Show this help", cxxopts::value<bool>()->default_value("false"));
	options.parse_positional({"input"});
	auto result = options.parse(argc, argv);
	if (result["help"].as<bool>()) {
		std::cout << options.help() << std::endl;
		return 0;
	}

	std::string file_path = result["input"].as<std::string>();
	// std::cout << file_path << std::endl;
	if (!file_path.empty()) {
		std::ifstream file(file_path);
		if (file.fail()) {
			std::cerr << "Error: fail to open file" << std::endl;
			return 1;
		} else {
			std::string code{
				std::istreambuf_iterator<char>(file),
				std::istreambuf_iterator<char>()
			};
			file.close();
			try {
				shell.run(code);
			} catch (const bf::BrainfuckSyntaxError& e) {
				std::cerr << "Error: " << e.what() << std::endl;
				return 1;
			}
		}
	} else {

		std::string code;

		std::cout << "Command: restart, exit" << std::endl;
		std::cout << "Tip: It is better to use ASCII characters only." << std::endl;

		while (true) {

			std::cout << "$ ";

			std::getline(std::cin, code);
			if (std::cin.eof()) {
				break;
			}

			if (code == "restart") {
				shell.reset();
				std::cout << "Done." << std::endl;
			} else if (code == "exit") {
				break;
			} else {
				try {
					shell.run(code);
				} catch (const bf::BrainfuckSyntaxError& e) {
					std::cout << "Error: " << e.what() << std::endl;
				}
				std::cout << std::endl;
			}
		}
	}
	
	return 0;
}
