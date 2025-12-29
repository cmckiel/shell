#include "Commands.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <sstream>

namespace fs = std::filesystem;

bool ExitCommand::execute(const std::vector<std::string>& args) {
  exit(0);
  return false;
}

bool EchoCommand::execute(const std::vector<std::string>& args) {
  for (const auto& arg : args) {
    std::cout << arg << " ";
  }
  std::cout << std::endl;

  return true;
}

TypeCommand::TypeCommand(CommandParser& parser)
  : command_parser_(parser) {
}

bool TypeCommand::execute(const std::vector<std::string>& args) {
  bool res = false;

  if (!args.empty()) {
    std::string command_in_question = args[0];

    if (command_parser_.find_builtin(command_in_question)) {
      std::cout << command_in_question << " is a shell builtin" << std::endl;
    }
    else if (std::string command_path; command_parser_.find_on_system(command_in_question, command_path)) {
      std::cout << command_in_question << " is " << command_path << std::endl;
    }
    else {
      std::cout << command_in_question << ": not found" << std::endl;
    }

    res = true;
  }

  return res;
}
