#include "InputScanner.hpp"

std::vector<std::string> InputScanner::scan(std::string input) {
  std::string token;
  std::vector<std::string> tokens;
  State state = State::base;

  for (auto c : input) {

    // If `c` caused a state transition, check which state
    // we transitioned into.
    if (transition_state(state, c)) {

      // just transitioned to base state
      if (state == State::base) {
        // We just completed a valid token. Push it.
        tokens.push_back(token);
        token = "";
        continue;
      }

      // The transition was not into a normal word, therefore
      // skip this control character.
      if (state != State::inside_word) {
        continue;
      }
    }

    // Never add characters to a token in the base state.
    if (state == State::base)
      continue;

    // If `c` did not cause a state transition and we're
    // not in the base state
    // then it is not a control character.
    // We should append it to `token`.
    token.push_back(c);
  }

  // If token was not zero'd out, then
  // there is still a token to add.
  if (token.size() != 0) {
    tokens.push_back(token);
  }

  return tokens;
}

bool InputScanner::transition_state(State& state, char c) {
  State previous_state = state;

  if (state == State::base) {
    if (c == '\'') {
      state = State::inside_single_quote;
    }
    else if (c != ' ') {
      state = State::inside_word;
    }
  }
  else if (state == State::inside_word) {
    if (c == '\'') {
      state = State::inside_single_quote;
    }
    else if (c == ' ') {
      state = State::base;
    }
  }
  else if (state == State::inside_single_quote) {
    if (c == '\'') {
      state = State::just_finished_single_quote;
    }
  }
  else if (state == State::just_finished_single_quote) {
    if (c == '\'') {
      state = State::inside_single_quote;
    }
    else if (c == ' ') {
      state = State::base;
    }
    else {
      state = State::inside_word;
    }
  }
  else {
    // Unknown state
    return false;
  }

  // Return true if the state changed.
  return (previous_state != state);
}
