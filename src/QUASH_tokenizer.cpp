/**
* @Filename: QUASH_tokenizer.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  August 28th, 2019 [4:56pm]
* @Modified: October 2nd, 2019 [10:28pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdint>  // uint8_t

#include <iomanip>   // std::setfill, std::setw
#include <iostream>  // std::cout
#include <string>    // std::string
#include <utility>   // std::pair
#include <vector>    // std::vector

#include "QUASH_tokenizer.hpp"

#include "QUASH_public.hpp"  // QUASH::STATUS_CODES

namespace QUASH {
  Tokenizer::Tokenizer() {
  }

  Tokenizer::~Tokenizer() {
  }

  std::pair<uint8_t, std::vector<std::string>> Tokenizer::Tokenize(const std::string &aString) {
    std::vector<std::string> tokens;
    uint8_t status = STATUS_SUCCESS;

    // TODO: tokenize aString
    static_cast<void>(aString);

    return std::pair<uint8_t, std::vector<std::string>>(status, tokens);
  }

  void Tokenizer::print(const std::vector<std::string> &tokens) {
    for (size_t i = 0; i < tokens.size(); ++i) {
      std::cout << std::setfill(' ') << std::setw(3);
      std::cout << i << tokens[i] << std::endl;
    }
  }
}  // namespace QUASH
