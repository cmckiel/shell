#pragma once

#include <iostream>
#include <unordered_map>

#include "Command.hpp"

class CommandParser {
  public:
    CommandParser();
    bool execute(const std::string& command, const std::vector<std::string>& args);
    bool find_builtin(const std::string& command);
    bool find_on_system(const std::string& command, std::string& command_path);
  private:
    std::unordered_map<std::string, std::unique_ptr<Command>> shell_builtins_;
};
