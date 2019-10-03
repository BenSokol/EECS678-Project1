/**
* @Filename: QUASH_home.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 25th, 2019 [5:49pm]
* @Modified: October 2nd, 2019 [10:30pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <string>  // std::string

#include "QUASH_home.hpp"

namespace QUASH::COMMANDS {
  const std::string home() {
    char *homedir = getenv("HOME");
    if (homedir == nullptr) {
      return "";
    }
    return std::string(homedir);
  }
}  // namespace QUASH::COMMANDS
