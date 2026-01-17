#pragma once

#include <vector>
#include <string>

class InputScanner {

  public:

    /**
     * Given a line of input, return a list of tokens.
     */
    std::vector<std::string> scan(std::string input);

  private:

    enum class State {
      base,
      inside_word,
      inside_single_quote,
      just_finished_single_quote,
    };

    bool transition_state(State& state, char c);
};
