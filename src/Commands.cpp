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

bool PwdCommand::execute(const std::vector<std::string>& args) {
  bool res = true;

  try {
    fs::path current_dir = fs::current_path();
    std::cout << current_dir.string() << std::endl;
  }
  catch (std::exception& e) {
    std::cout << "Error fetching current directory" << std::endl;
    res = false;
  }

  return res;
}

bool CdCommand::execute(const std::vector<std::string>& args) {
  bool res = true;

  std::string desired_working_dir = "";

  try {
    desired_working_dir = args.empty() ? "" : args[0];

    if (desired_working_dir == "~") {
      const char *home_env = std::getenv("HOME");

      if (home_env == nullptr) {
        std::cerr << "HOME environment variable not found." << std::endl;
        return false;
      }

      std::string home_path(home_env);

      desired_working_dir = home_path;
    }

    fs::current_path(desired_working_dir);
  }
  catch (std::exception& e) {
    std::cout << "cd: " << desired_working_dir << ": No such file or directory" << std::endl;
    res = false;
  }

  return res;
}
