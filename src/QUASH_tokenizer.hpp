/**
* @Filename: QUASH_tokenizer.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  August 28th, 2019 [4:56pm]
* @Modified: October 9th, 2019 [6:18pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef QUASH_TOKENIZER_HPP
#define QUASH_TOKENIZER_HPP

#include <cstdint>  // uint8_t

#include <deque>    // std::vector
#include <string>   // std::string
#include <utility>  // std::pair

#include "QUASH_public.hpp"

namespace QUASH {
  class Tokenizer {
  public:
    ~Tokenizer();

    static std::string str(const std::deque<std::string> &tokens, bool oneline);
    static std::pair<quash_status_t, std::deque<std::string> > Tokenize(std::string aString);

  private:
    Tokenizer();
  };
}  // namespace QUASH

#endif
