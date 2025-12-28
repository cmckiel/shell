#pragma once

#include <memory>
#include <unordered_map>

#include "Command.hpp"

class Echo : public Command {
  public:
    bool execute(std::vector<std::string>& args) override final;
};

class Exit : public Command {
  public:
    bool execute(std::vector<std::string>& args) override final;
};

class Type : public Command {
  public:
    Type(std::unordered_map<std::string, std::unique_ptr<Command>>& command_parser);
    bool execute(std::vector<std::string>& args) override final;
  private:
    std::unordered_map<std::string, std::unique_ptr<Command>>& command_parser_;
};
