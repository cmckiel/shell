#include <iostream>
#include <string>
#include <vector>

#include "CommandParser.hpp"
#include "InputScanner.hpp"

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  InputScanner scanner;
  CommandParser command_parser;

  while (1) {
    std::cout << "$ ";

    // read the command line
    std::string input;
    std::getline(std::cin, input);

    // tokenize the input
    std::vector<std::string> argv = scanner.scan(input);

    // execute the command
    command_parser.execute(argv);
  }
}
