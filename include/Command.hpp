#pragma once

#include <vector>
#include <string>
#include <ostream>

class Command {
  public:
    virtual bool execute(const std::vector<std::string>& args, std::ostream& oss) = 0;
    virtual ~Command() = default;
};
