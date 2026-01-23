#include "InputScanner.hpp"

InputScanner::InputScanner() {
  transition_table = {
    {State::base, {
      {CharType::double_quote, State::inside_double_quote},
      {CharType::single_quote, State::inside_single_quote},
      {CharType::space,        State::base},
      {CharType::other,        State::inside_word},
    }},
    {State::inside_word, {
      {CharType::double_quote, State::inside_double_quote},
      {CharType::single_quote, State::inside_single_quote},
      {CharType::space,        State::base},
      {CharType::other,        State::inside_word},
    }},
    {State::inside_single_quote, {
      {CharType::double_quote, State::inside_single_quote},
      {CharType::single_quote, State::just_finished_single_quote},
      {CharType::space,        State::inside_single_quote},
      {CharType::other,        State::inside_single_quote},
    }},
    {State::just_finished_single_quote, {
      {CharType::double_quote, State::inside_double_quote},
      {CharType::single_quote, State::inside_single_quote},
      {CharType::space,        State::base},
      {CharType::other,        State::inside_word},
    }},
    {State::inside_double_quote, {
      {CharType::double_quote, State::just_finished_double_quote},
      {CharType::single_quote, State::inside_double_quote},
      {CharType::space,        State::inside_double_quote},
      {CharType::other,        State::inside_double_quote},
    }},
    {State::just_finished_double_quote, {
      {CharType::double_quote, State::inside_double_quote},
      {CharType::single_quote, State::inside_single_quote},
      {CharType::space,        State::base},
      {CharType::other,        State::inside_word},
    }},
  };
}

std::vector<std::string> InputScanner::scan(std::string input) {
  std::string token;
  std::vector<std::string> tokens;
  State state = State::base;

  bool backslashed = false;

  for (auto c : input) {

    // If we detect a backslash, prime the bool and skip to the next char.
    if (!backslashed && c == '\\') {
      backslashed = true;
      continue;
    }

    // If we're not backslashed, go ahead and process normally.
    // Otherwise, if we are backslashed, add the character.
    if (!backslashed) {
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
    }

    // If `c` did not cause a state transition and we're
    // not in the base state
    // then it is not a control character.
    // We should append it to `token`.
    token.push_back(c);

    // Turn backslashed off after we add the character.
    if (backslashed) {
      backslashed = false;
    }
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
  CharType char_type;

  if (c == '\"') {
    char_type = CharType::double_quote;
  }
  else if (c == '\'') {
    char_type = CharType::single_quote;
  }
  else if (c == ' ') {
    char_type = CharType::space;
  }
  else {
    char_type = CharType::other;
  }

  state = transition_table[state][char_type];

  // Return true if the state changed.
  return (previous_state != state);
}
