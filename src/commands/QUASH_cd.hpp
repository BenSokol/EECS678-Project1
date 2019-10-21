/**
* @Filename: QUASH_cd.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 25th, 2019 [5:41pm]
* @Modified: October 20th, 2019 [8:03pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef QUASH_COMMANDS_CD_HPP
#define QUASH_COMMANDS_CD_HPP

#include <deque>   // std::deque
#include <string>  // std::string

#include "QUASH_public.hpp"

namespace QUASH::COMMANDS {
  bool cd(std::deque<std::string> &currentCommand, quash_status_t &status, long &p_status);
}  // namespace QUASH::COMMANDS

#endif
