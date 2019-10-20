/**
* @Filename: QUASH_pwd.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 25th, 2019 [5:41pm]
* @Modified: October 20th, 2019 [5:35pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef QUASH_COMMANDS_PWD_HPP
#define QUASH_COMMANDS_PWD_HPP

#include <string>

namespace QUASH::COMMANDS {
  std::string pwd(bool replaceHome = true);
}  // namespace QUASH::COMMANDS

#endif
