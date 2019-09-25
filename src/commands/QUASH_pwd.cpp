/**
* @Filename: QUASH_pwd.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 25th, 2019 [5:41pm]
* @Modified: September 25th, 2019 [6:00pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/


#include <string>
#include <unistd.h>

#include "QUASH_pwd.hpp"

namespace QUASH::COMMANDS {
  const std::string pwd() {
    char buff[FILENAME_MAX];
    getcwd(buff, FILENAME_MAX);
    std::string pwd = std::string(buff);

    char *homedir_chr = getenv("HOME");
    if (homedir_chr == nullptr) {
      return pwd;
    }
    std::string homedir_str = std::string(homedir_chr);

    pwd.replace(pwd.find(homedir_str), homedir_str.length(), "~");
    return pwd;
  }
}  // namespace QUASH::COMMANDS
