/**
* @Filename: QUASH_main.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [8:00pm]
* @Modified: October 2nd, 2019 [10:42pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdint>  // uint8_t

#include <iostream>  // std::cout
#include <map>       // std::map
#include <string>    // std::string

#include "QUASH_main.hpp"

#include "DBG_out.hpp"          // DBG::out::instance(), DBG_print, etc.
#include "QUASH_hostname.hpp"   // QUASH::COMMANDS::hostname()
#include "QUASH_ps1.hpp"        // QUASH::COMMANDS::ps1()
#include "QUASH_public.hpp"     // QUASH::STATUS_CODES
#include "QUASH_pwd.hpp"        // QUASH::COMMANDS::pwd()
#include "QUASH_tokenizer.hpp"  // QUASH::Tokenizer()
#include "QUASH_whoami.hpp"     // QUASH::COMMANDS::whoami()
#include "UTL_colors.hpp"       // UTL::COLORS::FG::red, etc.

namespace QUASH {
  main::main(const int argc, const char *const *const argv, const char *const *const envp) :
      mPrintEnv(false), mDisplayUsage(false), mStatus(STATUS_SUCCESS) {
    DBG_print("Starting Quash Initialization\n", "");
    if (mStatus == STATUS_SUCCESS) {
      preInit();
      if (mStatus == STATUS_SUCCESS) {
        initArgs(argc, argv);
        if (mStatus == STATUS_SUCCESS) {
          initEnv(envp);
          if (mStatus == STATUS_SUCCESS) {
            postInit();
            if (mStatus == STATUS_SUCCESS) {
              DBG_print("Finished Quash Initialization\n");
              if (DBG::out::instance().enabled()) {
                DBG_print("Debug Information:\n");
                DBG_print("HOME     = ", mEnv["HOME"], "\n");
                DBG_print("HOSTNAME = ", QUASH::COMMANDS::hostname(), "\n");
                DBG_print("PATH     = ", mEnv["PATH"], "\n");
                DBG_print("PWD      = ", QUASH::COMMANDS::pwd(), "\n");
                DBG_print("USERNAME = ", QUASH::COMMANDS::whoami(), "\n");
              }
            }
          }
        }
      }
    }
  }


  main::~main() {
    DBG::out::instance().wait();
  }


  uint8_t main::run() {
    if (mDisplayUsage || mStatus != STATUS_SUCCESS) {
      usage();
      return mStatus;
    }

    if (mPrintEnv) {
      DBG_print("Printing environment variables\n");
      printEnv();
    }

    DBG_print("Starting Quash...\n");

    while (true) {
      // Prints PS1
      if (DBG::out::instance().enabled()) {
        DBG::out::instance().wait();
      }
      std::cout << COMMANDS::ps1();

      // Gets input from user
      std::string input_string = getInput();

      // Detects if CTRL-D (EOF) has been entered. If so, exits program.
      if (std::cin.eof()) {
        DBG_print("EOF has been detected, exiting...\n");
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

    DBG_print("Exiting Quash...\n");

    return mStatus;
  }

  std::string main::getInput() {
    std::string inputString;
    std::getline(std::cin, inputString);
    return inputString;
  }


  void main::initArgs(const int argc, const char *const *const argv) {
    if (argc > 1) {
      bool debug_mode = false;
      std::string str = "";

      for (int i = 1; i < argc; ++i) {
        if (mCmdFlags.count(argv[i])) {
          switch (mCmdFlags[argv[i]]) {
            case QUASH_FLAG_HELP:
              // Help (-h, --help)
              mDisplayUsage = true;
              DBG_print("Found help flag...\n");
              break;

            case QUASH_FLAG_DEBUG:
              // Debug Mode (-d, --debug)
              debug_mode = true;
              DBG_print("Debug Mode Enabled\n");
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

              DBG_print("Using \'", "\' as debug ofstream\n");

              if (!DBG::out::instance().openOFS(str)) {
                std::cerr << "Warning: Unable to open\'" << argv[i] << "\'\n";
              }

              DBG_print("Found debug-file flag, \n");
              i++;
              break;

            case QUASH_FLAG_DEBUG_NO_STDERR:
              // Debug Mode (--debug-no-stderr)
              DBG::out::instance().osDisable();
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

      DBG::out::instance().enable(debug_mode);
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
      std::cout << env.first << " = " << env.second << "\n";
    }
  }

  void main::usage() const {
    std::cout << "Usage: ./quash\n";
  }
}  // namespace QUASH
