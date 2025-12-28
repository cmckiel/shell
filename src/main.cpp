#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Commands.hpp"

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::unordered_map<std::string, std::unique_ptr<Command>> command_parser;

  command_parser.emplace("exit", std::make_unique<Exit>());
  command_parser.emplace("echo", std::make_unique<Echo>());
  command_parser.emplace("type", std::make_unique<Type>(command_parser));

  while (1) {
    std::cout << "$ ";

    // read the command line
    std::string input;
    std::getline(std::cin, input);

    // tokenize the input
    std::vector<std::string> tokens;

    std::istringstream iss(input);
    std::string token;
    while (iss >> token) {
      tokens.push_back(token);
    }

    // parse the input and execute the command
    if (!tokens.empty()) {
      // separate command and arguments
      std::string command = tokens[0];
      std::vector<std::string> args(tokens.begin() + 1, tokens.end());

      if (command_parser.contains(command)) {
        auto& command_handler = command_parser.at(command);
        command_handler->execute(args);
      }
      else {
        std::cout << command << ": command not found" << std::endl;
      }
    }
  }
}
