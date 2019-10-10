/**
* @Filename: QUASH_tokenizer.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  August 28th, 2019 [4:56pm]
* @Modified: October 10th, 2019 [5:09pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdint>  // uint8_t

#include <deque>     // std::vector
#include <iomanip>   // std::setfill, std::setw
#include <iostream>  // std::cout
#include <sstream>   // std::stringstream
#include <string>    // std::string
#include <utility>   // std::pair

#include "QUASH_tokenizer.hpp"

#include "DBG_out.hpp"
#include "QUASH_public.hpp"  // QUASH::STATUS_CODES
#include "UTL_assert.h"

namespace QUASH {
  Tokenizer::Tokenizer() {
  }


  Tokenizer::~Tokenizer() {
  }


  std::string Tokenizer::str(const std::deque<std::string> &tokens, bool oneline) {
    std::stringstream ss;
    for (size_t i = 0; i < tokens.size(); ++i) {
      if (!oneline) {
        ss << std::setfill(' ') << std::setw(3) << std::to_string(i);
      }
      ss << (oneline ? "" : "  ") << tokens.at(i) << (oneline ? " " : "\n") << std::flush;
    }

    return ss.str();
  }


  std::pair<quash_status_t, std::deque<std::string>> Tokenizer::Tokenize(std::string aString) {
    quash_status_t status = STATUS_SUCCESS;
    std::deque<std::string> tokens;

    DBG_printv(1, "Starting tokenizer...\n");
    DBG_printv(1, "Tokenizing: ", aString, "\n");

    // State machine
    enum state_t : uint8_t {
      NORMAL,
      SINGLE_QUOTE,
      DOUBLE_QUOTE,
      REDIRECT_OUT_APPEND,
      PIPE_OR,
      ASYNC_AND,
      DONE,
      COUNT
    } state = NORMAL;

    std::string token = "";

    for (size_t i = 0; i < aString.size(); ++i) {
      // The following debug line produces a lot of output.
      // Do not commit with the line uncommented.
      // DBG_print("Looking at ", aString[i], ", in state = ", state, "\n");
      switch (state) {
        case SINGLE_QUOTE:
          // ---------------------------------
          // Single Quote
          // ---------------------------------
          if (aString[i] == '\'') {
            token += aString[i];
            tokens.push_back(token);
            token = "";
            state = NORMAL;
          }
          else {
            token += aString[i];
          }
          break;

        case DOUBLE_QUOTE:
          // ---------------------------------
          // Double Quote
          // ---------------------------------
          if (aString[i] == '\"') {
            token += aString[i];
            tokens.push_back(token);
            token = "";
            state = NORMAL;
          }
          else {
            token += aString[i];
          }
          break;

        case REDIRECT_OUT_APPEND:
          // ---------------------------------
          // > or >>
          // ---------------------------------
          if (aString[i] == '>') {
            token += aString[i];
          }
          tokens.push_back(token);
          token = "";
          state = NORMAL;
          if (aString[i] != '>') {
            --i;
          }
          break;

        case PIPE_OR:
          // ---------------------------------
          // Pipe or OR
          // ---------------------------------
          if (aString[i] == '|') {
            token += aString[i];
          }
          tokens.push_back(token);
          token = "";
          state = NORMAL;
          if (aString[i] != '|') {
            --i;
          }
          break;

        case ASYNC_AND:
          // ---------------------------------
          // Async or AND
          // ---------------------------------
          if (aString[i] == '&') {
            token += aString[i];
          }
          tokens.push_back(token);
          token = "";
          state = NORMAL;
          if (aString[i] != '&') {
            --i;
          }
          break;

        case DONE:
          UTL_assert_always();
          break;

        case NORMAL:
          // ---------------------------------
          // Check Next character
          // ---------------------------------
          if (aString[i] == '#') {
            // ---------------------------------
            // Comment
            // ---------------------------------
            if (!token.empty()) {
              tokens.push_back(token);
              token = "";
            }
            state = DONE;
          }
          else if (aString[i] == '\'') {
            // ---------------------------------
            // Single quote
            // ---------------------------------
            state = SINGLE_QUOTE;
            token += aString[i];
          }
          else if (aString[i] == '\"') {
            // ---------------------------------
            // Double quote
            // ---------------------------------
            state = DOUBLE_QUOTE;
            token += aString[i];
          }
          else if (aString[i] == '|') {
            // ---------------------------------
            // Pipe or OR
            // ---------------------------------
            if (!token.empty()) {
              tokens.push_back(token);
              token = "";
            }
            state = PIPE_OR;
            token += aString[i];
          }
          else if (aString[i] == '&') {
            // ---------------------------------
            // Async or AND
            // ---------------------------------
            if (!token.empty()) {
              tokens.push_back(token);
              token = "";
            }
            state = ASYNC_AND;
            token += aString[i];
          }
          else if (aString[i] == '>') {
            // ---------------------------------
            // > or >>
            // ---------------------------------
            if (!token.empty()) {
              tokens.push_back(token);
              token = "";
            }
            state = REDIRECT_OUT_APPEND;
            token += aString[i];
          }
          else if (aString[i] == ';') {
            // ---------------------------------
            // Semicolon
            // ---------------------------------
            if (!token.empty()) {
              tokens.push_back(token);
              token = "";
            }
            token = aString[i];
            tokens.push_back(token);
            token = "";
          }
          else if (isblank(aString[i])) {
            // ---------------------------------
            // Whitespace
            // ---------------------------------
            if (!token.empty()) {
              tokens.push_back(token);
              token = "";
            }
          }
          else {
            // ---------------------------------
            // Anything else
            // ---------------------------------
            token += aString[i];
          }
          break;
        default:
          UTL_assert_always();
          break;
      }

      if (state == DONE) {
        // The rest of the string is a comment, so break out of loop
        break;
      }
    }


    if (!token.empty()) {
      // Add final token
      tokens.push_back(token);
    }

    // Validate tokenized string
    if (state == SINGLE_QUOTE) {
      status = STATUS_TOKENIZER_MISSING_CLOSE_SINGLE_QUOTE;
    }

    if (state == DOUBLE_QUOTE) {
      status = STATUS_TOKENIZER_MISSING_CLOSE_DOUBLE_QUOTE;
    }

    DBG_printv(1, "Tokenizer is complete.\n");
    return std::pair<quash_status_t, std::deque<std::string>>(status, tokens);
  }  // namespace QUASH

}  // namespace QUASH
