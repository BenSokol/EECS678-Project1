/**
* @Filename: QUASH_home.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 25th, 2019 [5:49pm]
* @Modified: October 20th, 2019 [4:25pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdlib>

#include <string>  // std::string

#include "QUASH_home.hpp"

namespace QUASH::COMMANDS {
  const char* home() {
    char* homedir = getenv("HOME");
    if (homedir == nullptr) {
      return "";
    }
    return homedir;
  }
}  // namespace QUASH::COMMANDS
