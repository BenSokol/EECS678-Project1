/**
* @Filename: QUASH_tokenizer.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  August 28th, 2019 [4:56pm]
* @Modified: September 26th, 2019 [2:32am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "QUASH_tokenizer.hpp"

#include "QUASH_public.hpp"
#include "UTL_assert.h"

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
