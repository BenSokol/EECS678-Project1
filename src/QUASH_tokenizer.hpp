/**
* @Filename: QUASH_tokenizer.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  August 28th, 2019 [4:56pm]
* @Modified: October 4th, 2019 [6:03pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <cstdint>  // uint8_t

#include <deque>    // std::vector
#include <string>   // std::string
#include <utility>  // std::pair

namespace QUASH {
  class Tokenizer {
  public:
    ~Tokenizer();

    static std::pair<uint8_t, std::deque<std::string>> Tokenize(const std::string &aString);
    static void print(const std::deque<std::string> &tokens);

  private:
    Tokenizer();
  };
}  // namespace QUASH

#endif
