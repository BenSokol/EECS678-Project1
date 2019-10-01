/**
* @Filename: QUASH_main.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [8:00pm]
* @Modified: October 1st, 2019 [2:41am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include <unistd.h>

#include "QUASH_main.hpp"

#include "DBG_print.hpp"
#include "QUASH_ps1.hpp"
#include "QUASH_public.hpp"
#include "QUASH_tokenizer.hpp"
#include "UTL_assert.h"
#include "UTL_colors.hpp"

namespace QUASH {
  main::main(const int argc, const char *const *const argv, const char *const *const envp) :
    mPrintEnv(false), mDisplayUsage(false), mStatus(STATUS_SUCCESS) {
    if (mStatus == STATUS_SUCCESS) {
      preInit();
      if (mStatus == STATUS_SUCCESS) {
        initArgs(argc, argv);
        if (mStatus == STATUS_SUCCESS) {
          initEnv(envp);
          if (mStatus == STATUS_SUCCESS) {
            postInit();
          }
        }
      }
    }
  }


  main::~main() {
  }


  uint8_t main::run() {
    if (mDisplayUsage || mStatus != STATUS_SUCCESS) {
      usage();
      return mStatus;
    }

    if (mPrintEnv) {
      printEnv();
    }

    while (true) {
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


  void main::initArgs(const int argc, const char *const *const argv) {
    [[maybe_unused]] std::string str = "";
    if (argc > 1) {
      for (int i = 1; i < argc; ++i) {
        if (mCmdFlags.count(argv[i])) {
          switch (mCmdFlags[argv[i]]) {
            case QUASH_FLAG_HELP:
              // Help (-h, --help)
              mDisplayUsage = true;
              break;

            case QUASH_FLAG_DEBUG:
              // Debug Mode (-d, --debug)
              DBG::print.enable();
              DBG::print << "Debug Mode Enabled\n";
              break;

            case QUASH_FLAG_DEBUG_FILE:
              // Debug Mode (--debug-file <filename>)
              if ((i + 1) == argc || mCmdFlags.count(str)) {
                std::cerr << "ERROR: Missing command line argument. Expected --debug-file <filename>\n";
                mStatus = STATUS_INIT_MISSING_COMMAND_LINE_PARAMETER;
                return;
              }

              str = std::string(argv[i + 1]);

              if (str.empty()) {
                std::cerr << "ERROR: Unknown command line argument \'" << argv[i] << "\'\n";
                mStatus = STATUS_INIT_INVALID_COMMAND_LINE_PARAMETER;
                return;
              }
              std::cout << __LINE__ << "\n" << std::flush;
              if (!DBG::print.open(str)) {
                std::cerr << "Warning: Unable to open\'" << argv[i] << "\'\n";
              }
              DBG::print << "Debug file = " << str << "\n";
              DBG_print_tsf << "test\n";
              i++;
              break;

            case QUASH_FLAG_DEBUG_NO_STDERR:
              // Debug Mode (--debug-no-stderr)
              DBG::print.popOS(std::cerr);
              break;

            case QUASH_FLAG_PRINT_ENV:
              // Print Env (--print-env)
              mPrintEnv = true;
              break;
          }
        }
        else {
          std::cerr << "ERROR: Unknown command line argument \'" << argv[i] << "\'\n";
          mStatus = STATUS_INIT_UNKNOWN_COMMAND_LINE_PARAMETER;
        }
      }
    }
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


  void main::postInit() {
  }


  void main::preInit() {
    mCmdFlags["-h"] = QUASH_FLAG_HELP;
    mCmdFlags["--help"] = QUASH_FLAG_HELP;
    mCmdFlags["-d"] = QUASH_FLAG_DEBUG;
    mCmdFlags["--debug"] = QUASH_FLAG_DEBUG;
    mCmdFlags["--debug-file"] = QUASH_FLAG_DEBUG_FILE;
    mCmdFlags["--debug-no-stderr"] = QUASH_FLAG_DEBUG_NO_STDERR;
    mCmdFlags["--print-env"] = QUASH_FLAG_PRINT_ENV;
  }


  void main::printEnv() const {
    for (auto &env : mEnv) {
      std::cout << env.first << " = "
                << "TODO"
                << "\n";
    }
  }

  void main::usage() const {
    std::cout << "Usage: ./quash\n";
  }
}  // namespace QUASH
