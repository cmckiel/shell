#pragma once

#include <vector>
#include <string>

class Command {
  public:
    virtual bool execute(const std::vector<std::string>& args) = 0;
    virtual ~Command() = default;
};
