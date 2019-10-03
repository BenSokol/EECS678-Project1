/**
* @Filename: QUASH_tokenizer.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  August 28th, 2019 [4:56pm]
* @Modified: October 2nd, 2019 [10:28pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <cstdint>  // uint8_t

#include <string>   // std::string
#include <utility>  // std::pair
#include <vector>   // std::vector

namespace QUASH {
  class Tokenizer {
  public:
    explicit Tokenizer();
    ~Tokenizer();

    static std::pair<uint8_t, std::vector<std::string>> Tokenize(const std::string &aString);
    static void print(const std::vector<std::string> &tokens);
  };
}  // namespace QUASH

#endif
