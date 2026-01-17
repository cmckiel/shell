#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

#include "CommandParser.hpp"
#include "InputScanner.hpp"

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  CommandParser command_parser;

  while (1) {
    std::cout << "$ ";

    // read the command line
    std::string input;
    std::getline(std::cin, input);

    // tokenize the input
    InputScanner scanner;
    std::vector<std::string> input_tokens = scanner.scan(input);

    // parse the input and execute the command
    if (!input_tokens.empty()) {
      // separate command and arguments
      std::string command = input_tokens[0];
      std::vector<std::string> args(input_tokens.begin() + 1, input_tokens.end());

      command_parser.execute(command, args);
    }
  }
}
