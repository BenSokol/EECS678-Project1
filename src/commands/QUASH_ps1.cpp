/**
* @Filename: QUASH_ps1.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:24pm]
* @Modified: September 25th, 2019 [5:55pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <string>

#include "QUASH_ps1.hpp"

#include "QUASH_pwd.hpp"
#include "QUASH_whoami.hpp"
#include "UTL_colors.hpp"

namespace QUASH::COMMANDS {


  const std::string ps1() {
    std::string PS1 = "";
    const bool isColorEnabled = UTL::COLOR::isColorEnabled(std::cout);
    if (isColorEnabled) {
      PS1 += UTL::COLOR::getColorCode(UTL::COLOR::FG::BRIGHT::cyan);
    }
    PS1 += QUASH::COMMANDS::whoami();
    if (isColorEnabled) {
      PS1 += UTL::COLOR::getColorCode(UTL::COLOR::reset);
    }
    PS1 += ":";
    if (isColorEnabled) {
      PS1 += UTL::COLOR::getColorCode(UTL::COLOR::FG::BRIGHT::green);
    }
    PS1 += QUASH::COMMANDS::pwd();
    if (isColorEnabled) {
      PS1 += UTL::COLOR::getColorCode(UTL::COLOR::reset);
    }
    PS1 += "$ ";
    return PS1;
  }


}  // namespace QUASH::COMMANDS
