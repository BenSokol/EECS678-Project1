/**
* @Filename: QUASH_Tokens.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  August 28th, 2019 [4:56pm]
* @Modified: September 25th, 2019 [5:40pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <string>
#include <vector>

namespace QUASH {
  class Tokens {
   public:
    Tokens(std::string aString);
    ~Tokens();

    uint8_t status() const;
    void printTokens() const;

   private:
    void Tokenize();

    uint8_t mStatus;
    std::string mString;
    std::vector<std::string> mVector;
  };
}  // namespace QUASH


#endif
