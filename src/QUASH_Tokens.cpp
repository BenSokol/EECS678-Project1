/**
* @Filename: QUASH_Tokens.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  August 28th, 2019 [4:56pm]
* @Modified: September 25th, 2019 [6:40pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "QUASH_Tokens.hpp"

#include "QUASH_public.hpp"
#include "UTL_assert.h"

namespace QUASH {
  Tokens::Tokens(const std::string &aString) : mStatus(), mString(aString) {
    Tokenize();
  }

  Tokens::~Tokens() {
  }

  uint8_t Tokens::status() const {
    return mStatus;
  }

  void Tokens::Tokenize() {
  }

  void Tokens::printTokens() const {
    for (size_t i = 0; i < mVector.size(); ++i) {
      std::cout << std::setfill(' ') << std::setw(3);
      std::cout << i << mVector[i] << std::endl;
    }
  }
}  // namespace QUASH
