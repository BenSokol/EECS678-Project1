/**
* @Filename: QUASH_pwd.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 25th, 2019 [5:41pm]
* @Modified: October 20th, 2019 [6:07pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/
#include <iostream>
#include <string>  // std::string

#include <unistd.h>

#include "QUASH_pwd.hpp"

#include "UTL_assert.h"

namespace QUASH::COMMANDS {
  std::string pwd(bool replaceHome) {
    char *pwd_chr = getcwd(nullptr, 0);
    UTL_assert(pwd_chr != nullptr);

    std::string pwd(pwd_chr);
    free(pwd_chr);

    if (replaceHome) {
      try {
        char *home_chr = getenv("HOME");
        if (home_chr != nullptr) {
          std::string home(home_chr);
          if (pwd.compare(home) == 0) {
            pwd = "~/";
          }
          else {
            if (pwd.substr(0, home.size()).compare(home) == 0) {
              pwd = "~" + pwd.substr(home.size());
            }
          }
        }
      } catch (std::exception &) {}
    }

    return pwd;
  }
}  // namespace QUASH::COMMANDS
