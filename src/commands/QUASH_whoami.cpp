/**
* @Filename: QUASH_whoami.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 25th, 2019 [5:41pm]
* @Modified: October 2nd, 2019 [10:29pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <string>  // std::string

#include <pwd.h>
#include <unistd.h>

#include "QUASH_whoami.hpp"

namespace QUASH::COMMANDS {
  const std::string whoami() {
    uid_t uid = geteuid();
    passwd *pw = getpwuid(uid);
    if (pw) {
      return pw->pw_name;
    }
    return "";
  }
}  // namespace QUASH::COMMANDS
