/**
* @Filename: QUASH_tokenizer.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  August 28th, 2019 [4:56pm]
* @Modified: October 4th, 2019 [6:03pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdint>  // uint8_t

#include <deque>     // std::vector
#include <iomanip>   // std::setfill, std::setw
#include <iostream>  // std::cout
#include <string>    // std::string
#include <utility>   // std::pair

#include "QUASH_tokenizer.hpp"

#include "QUASH_public.hpp"  // QUASH::STATUS_CODES

namespace QUASH {
  Tokenizer::Tokenizer() {
  }

  Tokenizer::~Tokenizer() {
  }

  std::pair<uint8_t, std::deque<std::string>> Tokenizer::Tokenize(const std::string &aString) {
    std::deque<std::string> tokens;
    uint8_t status = STATUS_SUCCESS;

    // TODO: tokenize aString
    static_cast<void>(aString);

    return std::pair<uint8_t, std::deque<std::string>>(status, tokens);
  }

  void Tokenizer::print(const std::deque<std::string> &tokens) {
    for (size_t i = 0; i < tokens.size(); ++i) {
      std::cout << std::setfill(' ') << std::setw(3);
      std::cout << i << tokens[i] << std::endl;
    }
  }
}  // namespace QUASH
