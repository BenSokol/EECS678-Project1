/**
* @Filename: QUASH_main.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [8:00pm]
* @Modified: September 26th, 2019 [3:09am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>

#include "QUASH_main.hpp"

#include "QUASH_ps1.hpp"
#include "QUASH_public.hpp"
#include "QUASH_tokenizer.hpp"
#include "QUASH_command.hpp"
#include "UTL_colors.hpp"

namespace QUASH {
  main::main(const int argc, const char *const *const argv, const char *const *const envp) : mStatus(STATUS_SUCCESS) {
    initEnv(envp);

    // Prevent unused variable warning, temporary
    static_cast<void>(argc);
    static_cast<void>(argv);
  }

  main::~main() {
  }


  uint8_t main::run() {
    printEnv();
    while (true) {
      break;
      // Prints PS1
      std::cout << COMMANDS::ps1();

      // Gets input from user
      std::string input_string = getInput();

      // Detects if CTRL-D (EOF) has been entered. If so, exits program.
      if (std::cin.eof()) {
        break;
      }

      // Tokenize input string
      auto retTokenizer = Tokenizer::Tokenize(input_string);

      // TODO: IF DEBUG Print tokens from tokenizer
      // Tokenizer::print(ret.second);

      if (retTokenizer.first != STATUS_SUCCESS) {
        // Handle error from Tokenize;
      }

      // TODO: call out to (class) QUASH::command cmd with ret.second to run command
      //auto retCommand = Command::runCommand();
      std::vector<std::string> ls;
      //executable path string = Command::which(ls); Returns the path string to executable if found

      // TODO: uncomment, exit() should return true if "exit" or "quit" is run.
      // if (retCommand.first != STATUS_SUCCESS) {
      //   if (retCommand.first == STATUS_EXIT_NORMAL) {
      //     break;
      //   }
      //
      //   switch (retCommand.first) {
      //     case STATUS_UNKNOWN:
      //       break;
      //     default:
      //       break;
      //   }
      // }
    }

    return mStatus;
  }


  void main::initEnv(const char *const *const envp) {
    for (char **env = const_cast<char **>(envp); *env != nullptr; env++) {
      std::string row = *env;
      size_t sep = row.find_first_of("=");
      if (sep == std::string::npos) {
        mEnv.insert(std::pair<std::string, std::string>(row.substr(0, row.size() - 1), ""));
      }
      else {
        mEnv.insert(std::pair<std::string, std::string>(row.substr(0, sep), row.substr(sep + 1)));
      }
    }
  }

  void main::printEnv() const {
    for (auto &env : mEnv) {
      std::cout << env.first << " = "
                << "TODO"
                << "\n";
    }
  }
}  // namespace QUASH
