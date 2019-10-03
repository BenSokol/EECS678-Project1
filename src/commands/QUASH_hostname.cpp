/**
* @Filename: QUASH_hostname.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  October 2nd, 2019 [10:12pm]
* @Modified: October 2nd, 2019 [10:30pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <climits>  // CHAR_MAX

#include <string>  // std::string

#include <unistd.h>  // gethostname

#include "QUASH_hostname.hpp"

namespace QUASH::COMMANDS {
  const std::string hostname() {
    char hostname[CHAR_MAX];
    gethostname(hostname, CHAR_MAX);
    return std::string(hostname);
  }
}  // namespace QUASH::COMMANDS
