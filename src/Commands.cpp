#include "Commands.hpp"

#include <algorithm>
#include <iostream>

bool Exit::execute(std::vector<std::string>& args) {
  exit(0);
  return false;
}

bool Echo::execute(std::vector<std::string>& args) {
  for (const auto& arg : args) {
    std::cout << arg << " ";
  }
  std::cout << std::endl;

  return true;
}

Type::Type(std::unordered_map<std::string, std::unique_ptr<Command>>& command_parser)
  : command_parser_(command_parser) {
}

bool Type::execute(std::vector<std::string>& args) {
  bool res = false;

  if (!args.empty()) {
    std::string command_in_question = args[0];

    if (command_parser_.contains(command_in_question)) {
      std::cout << command_in_question << " is a shell builtin" << std::endl;
    }
    else {
      std::cout << command_in_question << ": not found" << std::endl;
    }

    res = true;
  }

  return res;
}
