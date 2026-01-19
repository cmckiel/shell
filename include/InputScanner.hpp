#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class InputScanner {

  public:
    InputScanner();

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
      inside_double_quote,
      just_finished_double_quote,
    };

    enum class CharType {
      double_quote,
      single_quote,
      space,
      other,
    };

    bool transition_state(State& state, char c);

    std::unordered_map<State, std::unordered_map<CharType, State>> transition_table;
};
