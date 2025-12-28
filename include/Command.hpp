#pragma once

#include <vector>
#include <string>

class Command {
  public:
    virtual bool execute(std::vector<std::string>& args) = 0;
    virtual ~Command() = default;
};
