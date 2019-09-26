/**
* @Filename: QUASH_main.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [8:00pm]
* @Modified: September 25th, 2019 [8:06pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <iostream>
#include <sstream>
#include <unistd.h>

#include "QUASH_main.hpp"

#include "QUASH_Tokens.hpp"
#include "QUASH_ps1.hpp"
#include "QUASH_public.hpp"
#include "UTL_colors.hpp"

namespace QUASH {
  main::main(int argc, const char *argv[]) : mStatus(STATUS_SUCCESS) {
    while (true) {
      // Prints PS1
      std::cout << QUASH::COMMANDS::ps1();

      // Gets input from user
      std::string input_string = QUASH::getInput();

      // Detects if CTRL-D (EOF) has been entered. If so, exits program.
      if (std::cin.eof()) {
        break;
      }

      // Tokenize input string
      QUASH::Tokens tok = QUASH::Tokens(input_string);

      // Print tokens from tokenizer
      tok.printTokens();
    }

    // Prevent unused variable warning, temporary
    static_cast<void>(argc);
    static_cast<void>(argv);
  }

  main::~main() {
  }

  uint8_t main::status() const {
    return mStatus;
  }
}  // namespace QUASH
