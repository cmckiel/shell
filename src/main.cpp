#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

#include "CommandParser.hpp"

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  CommandParser command_parser;

  while (1) {
    std::cout << "$ ";

    // read the command line
    std::string input;
    std::getline(std::cin, input);

    // tokenize the input
    std::vector<std::string> input_tokens;
    std::string token;

    bool inside_word = false;
    bool inside_single_quote = false;
    bool just_pushed_single_quote_token = false;
    char last_char;

    for (char c : input) {
      if (just_pushed_single_quote_token && c == '\'') {
        // great, it seems we've got back to back quotes.
        // now, we need to take out the previous token and
        // continue adding to it.
        if (!input_tokens.empty()) {
          token = std::move(input_tokens.back());
          input_tokens.pop_back();
          just_pushed_single_quote_token = false;
          inside_word = true;
          inside_single_quote = true;
          continue;
        }
      }
      else if (just_pushed_single_quote_token) {
        // Okay, so last time we pushed a token surrounded by single
        // quotes but the next char we encountered afterwards is not
        // another single quote. Therefore, unset the flag and leave
        // things as they were.
        just_pushed_single_quote_token = false;
      }

      // suck up all the spaces
      if (!inside_word && c == ' ' && !inside_single_quote) {
        continue;
      }
      else if (!inside_word) {
        // rising edge of word
        inside_word = true;

        // rising edge of single quote
        if (c == '\'') {
          inside_single_quote = true;
          continue;
        }
      }

      // falling edge of word
      if ((!inside_single_quote && inside_word && c == ' ') ||
          (inside_single_quote && inside_word && c == '\'')) {
        input_tokens.push_back(token);
        token = "";

        // We need to remember if we just pushed a single quote token,
        // because if we immediately encounter another single quote we
        // need to combine these tokens.
        if (inside_single_quote) {
          just_pushed_single_quote_token = true;
        }
        
        inside_word = false;
        inside_single_quote = false;

        continue;
      }
      else if (!inside_single_quote && inside_word && c == '\'') {
        // skip single quotes and just glue normal words together
        continue;
      }

      // Build the token, character by character.
      token.push_back(c);
    }

    if (token != "") {
      input_tokens.push_back(token);
      token = "";
    }

    // todo: rm this
//    std::cout << "TOKENS RECEIVED" << std::endl;
//    for (auto tok : input_tokens) {
//      std::cout << "'" << tok << "'" << " " << std::endl;
//    }
//    std::cout << "TOKENS END" << std::endl;

    // parse the input and execute the command
    if (!input_tokens.empty()) {
      // separate command and arguments
      std::string command = input_tokens[0];
      std::vector<std::string> args(input_tokens.begin() + 1, input_tokens.end());

      command_parser.execute(command, args);
    }
  }
}
