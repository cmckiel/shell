#include "Commands.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <sstream>

namespace fs = std::filesystem;

bool ExitCommand::execute(const std::vector<std::string>& args, std::ostream& oss) {
  exit(0);
  return false;
}

bool EchoCommand::execute(const std::vector<std::string>& args, std::ostream& oss) {
  for (const auto& arg : args) {
    oss << arg << " ";
  }
  oss << std::endl;

  return true;
}

TypeCommand::TypeCommand(CommandParser& parser)
  : command_parser_(parser) {
}

bool TypeCommand::execute(const std::vector<std::string>& args, std::ostream& oss) {
  bool res = false;

  if (!args.empty()) {
    std::string command_in_question = args[0];

    if (command_parser_.find_builtin(command_in_question)) {
      oss << command_in_question << " is a shell builtin" << std::endl;
    }
    else if (std::string command_path; command_parser_.find_on_system(command_in_question, command_path)) {
      oss << command_in_question << " is " << command_path << std::endl;
    }
    else {
      oss << command_in_question << ": not found" << std::endl;
    }

    res = true;
  }

  return res;
}

bool PwdCommand::execute(const std::vector<std::string>& args, std::ostream& oss) {
  bool res = true;

  try {
    fs::path current_dir = fs::current_path();
    oss << current_dir.string() << std::endl;
  }
  catch (std::exception& e) {
    oss << "Error fetching current directory" << std::endl;
    res = false;
  }

  return res;
}

bool CdCommand::execute(const std::vector<std::string>& args, std::ostream& oss) {
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
    oss << "cd: " << desired_working_dir << ": No such file or directory" << std::endl;
    res = false;
  }

  return res;
}
