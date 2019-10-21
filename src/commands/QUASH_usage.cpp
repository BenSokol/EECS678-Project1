/**
* @Filename: QUASH_usage.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:24pm]
* @Modified: October 21st, 2019 [4:35am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <string>  // std::string

#include "QUASH_usage.hpp"

#include "UTL_colors.hpp"  // UTL::COLOR::isColorEnabled(), UTL::COLOR::getColorCode()
#include "UTL_textWrap.hpp"

namespace QUASH::COMMANDS {
  const std::string usage() {
    std::string USAGE = "";
    USAGE += UTL::COLOR::getColorCode(UTL::COLOR::bold);
    USAGE += "QUASH(1)                         quash Manual                           QUASH(1)";
    USAGE += UTL::COLOR::getColorCode(UTL::COLOR::reset);
    USAGE += "\n";

    USAGE += "\n";
    USAGE +=
      UTL::COLOR::getColorCode(UTL::COLOR::FG::BRIGHT::red) + "NAME" + UTL::COLOR::getColorCode(UTL::COLOR::reset);
    USAGE += "\n\t" + UTL::COLOR::getColorCode(UTL::COLOR::FG::BRIGHT::red) + "quash";
    USAGE += UTL::COLOR::getColorCode(UTL::COLOR::reset) + " - QUite A SHell";
    USAGE += "\n";

    USAGE += "\n";
    USAGE +=
      UTL::COLOR::getColorCode(UTL::COLOR::FG::BRIGHT::red) + "SYNOPSIS" + UTL::COLOR::getColorCode(UTL::COLOR::reset);
    USAGE += "\n\t" + UTL::COLOR::getColorCode(UTL::COLOR::FG::BRIGHT::red) + "quash --help"
             + UTL::COLOR::getColorCode(UTL::COLOR::reset);
    USAGE += "\n\t" + UTL::COLOR::getColorCode(UTL::COLOR::FG::BRIGHT::red) + "quash --debug"
             + UTL::COLOR::getColorCode(UTL::COLOR::reset);
    USAGE += "\n";

    USAGE += "\n" + UTL::COLOR::getColorCode(UTL::COLOR::FG::BRIGHT::red) + "DESCRIPTION"
             + UTL::COLOR::getColorCode(UTL::COLOR::reset);
    USAGE += UTL::textWrap(
      "\n\tQuash is a shell for unix based operating systems. It implements a variety of features. For more "
      "information, see \'https://git.bensokol.com/EECS678-Project1\'.",
      true,
      8,
      1,
      80);
    // USAGE += "\n\tQuash is a shell for unix based operating systems. It implements a variety of features. For more "
    //          "information, see \'https://git.bensokol.com/EECS678-Project1\'.";
    USAGE += "\n";

    USAGE += "\n";
    USAGE +=
      UTL::COLOR::getColorCode(UTL::COLOR::FG::BRIGHT::red) + "COPYRIGHT" + UTL::COLOR::getColorCode(UTL::COLOR::reset);
    USAGE += "\n\t";
    USAGE += "Copyright (C) Ben Sokol and Shogun Thomas 2019. All Rights Reserved.";
    USAGE += "\n";

    USAGE += UTL::COLOR::getColorCode(UTL::COLOR::bold);
    USAGE += "\nquash                            ";
    USAGE += __DATE__;
    USAGE += "                            QUASH(1)" + UTL::COLOR::getColorCode(UTL::COLOR::reset) + "\n";

    return USAGE;
  }
}  // namespace QUASH::COMMANDS
