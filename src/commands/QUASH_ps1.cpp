/**
* @Filename: QUASH_ps1.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:24pm]
* @Modified: October 2nd, 2019 [10:33pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <string>  // std::string

#include "QUASH_ps1.hpp"

#include "QUASH_hostname.hpp"  // QUASH::COMMANDS::whoami()
#include "QUASH_pwd.hpp"       // QUASH::COMMANDS::pwd()
#include "QUASH_whoami.hpp"    // QUASH::COMMANDS::whoami()
#include "UTL_colors.hpp"      // UTL::COLOR::isColorEnabled(), UTL::COLOR::getColorCode()

namespace QUASH::COMMANDS {
  const std::string ps1() {
    std::string PS1 = "";
    const bool isColorEnabled = UTL::COLOR::isColorEnabled(std::cout);
    // username
    if (isColorEnabled) {
      PS1 += UTL::COLOR::getColorCode(UTL::COLOR::FG::BRIGHT::cyan);
    }
    PS1 += QUASH::COMMANDS::whoami();

    // @
    if (isColorEnabled) {
      PS1 += UTL::COLOR::getColorCode(UTL::COLOR::FG::BRIGHT::magenta);
    }
    PS1 += "@";

    // hostname
    if (isColorEnabled) {
      PS1 += UTL::COLOR::getColorCode(UTL::COLOR::FG::BRIGHT::green);
    }
    PS1 += QUASH::COMMANDS::hostname();

    // :
    if (isColorEnabled) {
      PS1 += UTL::COLOR::getColorCode(UTL::COLOR::reset);
    }
    PS1 += ":";

    // pwd
    if (isColorEnabled) {
      PS1 += UTL::COLOR::getColorCode(UTL::COLOR::FG::BRIGHT::green);
    }
    PS1 += QUASH::COMMANDS::pwd();

    // $
    if (isColorEnabled) {
      PS1 += UTL::COLOR::getColorCode(UTL::COLOR::reset);
    }
    PS1 += "$ ";

    return PS1;
  }
}  // namespace QUASH::COMMANDS
