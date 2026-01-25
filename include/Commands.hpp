#pragma once

#include <memory>
#include <unordered_map>

#include "Command.hpp"
#include "CommandParser.hpp"

class EchoCommand : public Command {
  public:
    bool execute(const std::vector<std::string>& args, std::ostream& oss) override final;
};

class ExitCommand : public Command {
  public:
    bool execute(const std::vector<std::string>& args, std::ostream& oss) override final;
};

class TypeCommand : public Command {
  public:
    TypeCommand(CommandParser& parser);
    bool execute(const std::vector<std::string>& args, std::ostream& oss) override final;
  private:
    CommandParser& command_parser_;
};

class PwdCommand : public Command {
  public:
    bool execute(const std::vector<std::string>& args, std::ostream& oss) override final;
};

class CdCommand : public Command {
  public:
    bool execute(const std::vector<std::string>& args, std::ostream& oss) override final;
};
