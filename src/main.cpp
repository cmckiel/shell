#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ranges>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::vector<std::string> builtin_commands = { "exit", "echo", "type" };

  while (1) {
    std::cout << "$ ";

    // Get the command input line
    std::string input;
    std::getline(std::cin, input);

    // Tokenize the input
    std::istringstream iss(input);
    std::vector<std::string> tokenized_input;

    std::string word;
    while (iss >> word) {
      tokenized_input.push_back(word);
    }

    // Parse and execute the command
    if (tokenized_input.size() > 0) {
      if (auto command = tokenized_input.at(0); command == "exit") {
        return 0;
      }
      else if (command == "echo") {
        for (const auto& token : tokenized_input | std::views::drop(1)) {
          std::cout << token << " ";
        }
        std::cout << std::endl;
      }
      else if (command == "type") {
        try {
          std::string command_in_question = tokenized_input.at(1);
          if (std::ranges::contains(builtin_commands, command_in_question)) {
            std::cout << command_in_question << " is a shell builtin" << std::endl;
          }
          else {
            std::cout << command_in_question << ": not found" << std::endl;
          }
        } catch (std::exception& e) {
          std::cout << "type: requires argument" << std::endl;
        }
      }
      else {
        std::cout << command << ": command not found" << std::endl;
      }
    }
  }
}
