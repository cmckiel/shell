#include "Commands.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <sstream>

namespace fs = std::filesystem;

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

bool find_command_on_system(const std::string& command, std::string& command_path) {
  bool res = false;

  const char *path_env = std::getenv("PATH");

  if (path_env == nullptr) {
    std::cerr << "PATH environment variable not found." << std::endl;
    return false;
  }

  std::string path(path_env);

  std::vector<std::string> paths;
  std::string path_component;
  std::istringstream iss(path);

  while (std::getline(iss, path_component, ':')) {
    if (!path_component.empty()) {
      paths.push_back(path_component);
    }
  }

  for (const auto& directory : paths) {
    try {
      auto it = fs::directory_iterator(directory);

      auto found = std::find_if(it, fs::end(it), [&command](const auto& entry) {
          return entry.path().filename() == command;
      });

      if (found != fs::end(it)) {
        auto file = found->path();
        fs::file_status fstatus = fs::status(file);
        fs::perms fperms = fstatus.permissions();

        // Check if any executable bit is set (Owner, Group, or Others)
        bool is_executable = (fperms & fs::perms::owner_exec) != fs::perms::none ||
                             (fperms & fs::perms::group_exec) != fs::perms::none ||
                             (fperms & fs::perms::others_exec) != fs::perms::none;

        if (is_executable) {
          command_path = file;
          res = true;
          break;
        }
      }
    }
    catch (std::exception& e) {
      continue;
    }
  }

  return res;
}

bool Type::execute(std::vector<std::string>& args) {
  bool res = false;

  if (!args.empty()) {
    std::string command_in_question = args[0];

    if (command_parser_.contains(command_in_question)) {
      std::cout << command_in_question << " is a shell builtin" << std::endl;
    }
    else if (std::string command_path; find_command_on_system(command_in_question, command_path)) {
      std::cout << command_in_question << " is " << command_path << std::endl;
    }
    else {
      std::cout << command_in_question << ": not found" << std::endl;
    }

    res = true;
  }

  return res;
}
