#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include "BoardState.h"

int main()
{
	std::string command;
	int32_t time;
	bool player;
	bool started = false;
	while (std::getline(std::cin, command))
	{
		std::ostringstream result;

		// process the command and fill result
		uint32_t index = command.find(' ');
		auto cmd_name = command.substr(0, index);
		if (cmd_name.compare("START") == 0) {
			if (started) {
				std::cerr << "START was executed already" << std::endl;
				return 1;
			}
			if (command.length() == 5) {
				std::cerr << "No arguments given" << std::endl;
				return 1;
			}

			uint8_t side;
			auto cmd_args = command.substr(index, command.npos - index);
			std::istringstream ss(cmd_args);
			ss >> side >> time;
			if (side == 'W') {
				player = true;
			}
			else if (side == 'B') {
				player = false;
			}
			else {
				std::cerr << "Side argument is not valid" << std::endl;
				return 1;
			}
			if (time <= 0) {
				std::cerr << "Time argument is not valid" << std::endl;
				return 1;
			}
			result << 1;
			started = true;
		}
		else if (cmd_name.compare("MOVE") == 0) {
			if (!started) {
				std::cerr << "Run START before MOVE" << std::endl;
				return 1;
			}
			if (command.length() == 4) {
				std::cerr << "No arguments given" << std::endl;
				return 1;
			}

			auto cmd_args = command.substr(index, command.npos - index);
			std::istringstream ss(cmd_args);
			uint32_t cell1, cell2, cell3, cell4, cell5, cell6, cell7, cell8, cell9, cell10, cell11, cell12,
				cell13, cell14;
			ss >> cell1 >> cell2 >> cell3 >> cell4 >> cell5 >> cell6 >> cell7 >> cell8 >> cell9 >> cell10 >>
				cell11 >> cell12 >> cell13 >> cell14;
			if (ss.fail()) {
				std::cerr << "Error reading board values (should be 14 integers)" << std::endl;
				return 1;
			}

			BoardState bs(player, cell1, cell2, cell3, cell4, cell5, cell6, cell7, cell8, cell9, cell10,
				cell11, cell12, cell13, cell14);

			result << bs.MinMaxIterativeDeepening(std::chrono::steady_clock::now() + std::chrono::seconds(time) - std::chrono::milliseconds(2)) + 1; // + 1 because the class is 0-based
		}
		else if (cmd_name.compare("STOP") == 0) {
			return 0;
		}
		else {
			std::cerr << "Unknown command" << std::endl;
			return 1;
		}

		std::cout << result.str() << std::endl;
	}
}