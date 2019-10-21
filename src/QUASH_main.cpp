/**
* @Filename: QUASH_main.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [8:00pm]
* @Modified: October 21st, 2019 [1:09am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <csetjmp>  // setjmp, longjmp
#include <csignal>  // kill
#include <cstdint>  // uint8_t

#include <deque>     // std::deque
#include <iostream>  // std::cout
#include <iterator>  // std::distance
#include <map>       // std::map
#include <string>    // std::string
#include <vector>    // std::vector

#include <unistd.h>  // isatty

#include "QUASH_main.hpp"

#include "DBG_out.hpp"          // DBG::out::instance(), DBG_print, etc.
#include "QUASH_home.hpp"       // QUASH::COMMANDS::home()
#include "QUASH_hostname.hpp"   // QUASH::COMMANDS::hostname()
#include "QUASH_process.hpp"    // QUASH::process
#include "QUASH_ps1.hpp"        // QUASH::COMMANDS::ps1()
#include "QUASH_public.hpp"     // QUASH::STATUS_CODES
#include "QUASH_pwd.hpp"        // QUASH::COMMANDS::pwd()
#include "QUASH_tokenizer.hpp"  // QUASH::Tokenizer()
#include "QUASH_whoami.hpp"     // QUASH::COMMANDS::whoami()
#include "UTL_assert.h"         // UTL_assert(), UTL_assert_always()
#include "UTL_colors.hpp"       // UTL::COLORS::FG::red, etc.
#include "UTL_trim.hpp"         // UTL::trim

namespace QUASH {

  std::pair<std::deque<std::shared_ptr<process>>, std::mutex> *mProcesses;

  main::main() : mPrintEnv(false), mDisplayUsage(false), mStatus(STATUS_SUCCESS) {
    mProcesses = new std::pair<std::deque<std::shared_ptr<process>>, std::mutex>();
  }

  void main::init(int argc, char **argv, char **envp) {
    DBG_printv(1, "Starting Quash Initialization\n");

    // Initialize command line flags map
    initCmdFlags();

    // Process command line flags
    initArgs(argc, argv);
    if (mStatus != STATUS_SUCCESS) {
      return;
    }

    // Process environment
    initEnv(envp);
    if (mStatus != STATUS_SUCCESS) {
      return;
    }

    DBG_printv(1, "Finished Quash Initialization\n");
    DBG_printv(1, "Debug Information:\n");
    DBG_printv(1, "HOME     = ", QUASH::COMMANDS::home(), "\n");
    DBG_printv(1, "HOSTNAME = ", QUASH::COMMANDS::hostname(), "\n");
    DBG_printv(1, "PATH     = ", getenv("PATH"), "\n");
    DBG_printv(1, "PWD      = ", QUASH::COMMANDS::pwd(), "\n");
    DBG_printv(1, "USERNAME = ", QUASH::COMMANDS::whoami(), "\n");
  }


  main::~main() {
    if (DBG::out::instance().enabled()) {
      DBG::out::instance().wait();
    }
  }


  quash_status_t main::run() {
    if (mDisplayUsage || mStatus != STATUS_SUCCESS) {
      usage();
      return mStatus;
    }

    DBG_print("Starting Quash...\n");

    while (true) {
      setjmp(mJumpBufferSIGINT);
      signal(SIGINT, QUASH::main::signalHandlerSIGINT);

      // Check if any jobs are complete
      checkJobStatus();

      if (DBG::out::instance().enabled()) {
        if (isatty(STDIN_FILENO)) {
          DBG_printv(1, "Reading from Command line\n");
        }
        else {
          DBG_printv(1, "Reading from IO redirection\n");
        }
        DBG::out::instance().wait();
      }

      std::string input_string = "";

      if (isatty(STDIN_FILENO)) {
        // Prints PS1
        std::cout << COMMANDS::ps1();
      }

      // Gets input from user
      input_string = getInput();

      // Detects if CTRL-D (EOF) has been entered. If so, exits program.
      if (std::cin.eof()) {
        if (isatty(STDIN_FILENO)) {
          std::cout << "\n";
        }
        DBG_print("EOF has been detected, exiting...\n");
        break;
      }

      // Check for read error
      if (std::cin.fail() || std::cin.bad()) {
        DBG_print("std::cin.fail() or std::cin.bad() returned true, reseting...\n");
        std::cin.clear();
        [[maybe_unused]] FILE *f = freopen("/dev/tty", "rw", stdin);
        UTL_assert(std::cin.good());
        UTL_assert(std::cin.rdstate() == 0);
        DBG_print("Reset std::cin...\n");
        continue;
      }

      // If input_string is empty, continue
      if (UTL::trim(input_string).empty()) {
        continue;
      }

      // Tokenize input string
      std::pair<quash_status_t, std::deque<std::string>> retTokenizer = Tokenizer::Tokenize(input_string);

      // Handle status from tokenize
      switch (retTokenizer.first) {
        case STATUS_SUCCESS:
          DBG_printv(1, "Tokenizer was successful\n");
          break;
        case STATUS_TOKENIZER_MISSING_CLOSE_SINGLE_QUOTE:
          DBG_print("ERROR: Found unmatched closing single quote.\n");
          if (!DBG::out::instance().enabled()) {
            std::cerr << "ERROR: Found unmatched closing single quote.\n";
          }
          continue;

        case STATUS_TOKENIZER_MISSING_CLOSE_DOUBLE_QUOTE:
          DBG_print("ERROR: Found unmatched closing single quote.\n");
          if (!DBG::out::instance().enabled()) {
            std::cerr << "ERROR: Found unmatched closing single quote.\n";
          }
          continue;

        default:
          if (DBG::out::instance().enabled()) {
            DBG::out::instance().wait();
          }
          std::cerr << "ERROR: Unknown error from Tokenizer. Exiting...\n";
          if (DBG::out::instance().enabled()) {
            DBG_print("ERROR: Unknown error from Tokenizer. Exiting...\n");
            DBG::out::instance().wait();
          }
          exit(retTokenizer.first);
      }

      if (retTokenizer.second.empty()) {
        DBG_printv(1, "Input string is whitespace or a comment.\n");
        continue;
      }

      // DEBUG: Print tokenized string
      DBG_print("\n", Tokenizer::str(retTokenizer.second, false));

      // Check if any jobs are complete
      checkJobStatus();


      // Create new process
      std::shared_ptr<process> p(new process(retTokenizer.second, mEnv, mProcesses));

      switch (p->status) {
        case STATUS_SUCCESS:
          DBG_printv(1, "Process creation was successful\n");
          break;
        case STATUS_COMMAND_SYNTAX_ERROR:
          if (DBG::out::instance().enabled()) {
            DBG_print("ERROR: ", p->errorMessage, "\n");
            DBG::out::instance().wait();
          }
          else {
            std::cerr << "ERROR: " << p->errorMessage << "\n";
          }
          continue;

        case STATUS_COMMAND_SEMANTIC_ERROR:
          if (DBG::out::instance().enabled()) {
            DBG_print("ERROR: ", p->errorMessage, "\n");
            DBG::out::instance().wait();
          }
          else {
            std::cerr << "ERROR: " << p->errorMessage << "\n";
          }
          continue;

        default:
          if (DBG::out::instance().enabled()) {
            DBG::out::instance().wait();
          }
          std::cerr << "ERROR: Unknown error from Process Creation. Exiting...\n";
          if (DBG::out::instance().enabled()) {
            DBG_print("ERROR: Unknown error from Process Creation. Exiting...\n");
            DBG::out::instance().wait();
          }
          exit(p->status);
      }

      // start process
      p->start();

      // If process was async it has now finished.
      if (p->async) {
        quash_status_t cmdStatus = p->status;
        std::string cmdErrorMessage = p->errorMessage;

        switch (cmdStatus) {
          case STATUS_SUCCESS:
            DBG_printv(1, "Process was successful\n");
            break;

          case STATUS_COMMAND_PRINT_JOBS:
          {
            std::unique_lock<std::mutex> lock(mProcesses->second);
            for (size_t j = 0; j < mProcesses->first.size(); ++j) {
              if (mProcesses->first[j]) {
                std::cout << "[" << j + 1 << "]\t" << mProcesses->first[j]->pid << "\t"
                          << "Running\t";
                std::cout << QUASH::Tokenizer::str(mProcesses->first[j]->tokens, true);
                std::cout << "\n";
              }
            }
            break;
          }

          case STATUS_EXIT_NORMAL:
            if (DBG::out::instance().enabled()) {
              DBG_print("Process requested exit normal\n");
              DBG::out::instance().wait();
            }
            exit(0);

          case STATUS_COMMAND_RUNTIME_ERROR:
            break;

          case STATUS_COMMAND_NOT_FOUND:
            std::cout << cmdErrorMessage << "\n";
            break;

          default:
            if (DBG::out::instance().enabled()) {
              DBG::out::instance().wait();
            }
            std::cerr << "ERROR: Unknown error from Process Creation. Exiting...\n";
            if (DBG::out::instance().enabled()) {
              DBG_print("ERROR: Unknown error from Process Creation. Exiting...\n");
              DBG::out::instance().wait();
            }
            exit(cmdStatus);
        }
      }
      else {
        // Add process to vector of processes
        DBG_printv(1, "Adding process to vector\n");
        std::unique_lock<std::mutex> lock(mProcesses->second);
        mProcesses->first.push_back(p);
        std::cout << "[" << mProcesses->first.size() << "]\t";
        std::cout << p->pid << "\t";
        std::cout << "Started\t";
        std::cout << QUASH::Tokenizer::str(p->tokens, true);
        std::cout << "\n" << std::flush;
      }
    }

    DBG_print("Exiting Quash...\n");
    if (DBG::out::instance().enabled()) {
      DBG::out::instance().wait();
    }
    return mStatus;
  }


  void main::checkJobStatus() {
    bool debugWait = DBG::out::instance().enabled();
    size_t i = 0;

    std::unique_lock<std::mutex> lock(mProcesses->second);
    for (std::deque<std::shared_ptr<process>>::iterator it = mProcesses->first.begin(); it != mProcesses->first.end();
         ++it) {
      i++;
      if ((*it) == nullptr) {
        continue;
      }
      if ((*it)->done == true) {
        if (debugWait) {
          DBG::out::instance().wait();
          debugWait = false;
        }
        std::cout << "[" << i << "]\t";
        std::cout << (*it)->pid << "\t";
        std::cout << ((*it)->status == STATUS_SUCCESS ? "Done" : "Exit " + std::to_string((*it)->p_status)) << "\t";
        std::cout << QUASH::Tokenizer::str((*it)->tokens, true);
        std::cout << "\n" << std::flush;
        mProcesses->first.erase(it);
      }
    }
  }


  std::string main::getInput() {
    std::string inputString;
    std::getline(std::cin, inputString);
    return inputString;
  }


  [[noreturn]] void main::signalHandlerSIGINT(int) {
    DBG_print("CTRL-C detected\n");
    std::cout << "\n";
    longjmp(QUASH::main::instance().mJumpBufferSIGINT, 1);
  }


  void main::initArgs(int argc, char **argv) {
    if (argc > 1) {
      bool debug_os = false;
      bool debug_ofs = false;
      uint8_t verbosity = 0;
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
              DBG_print("Debug Mode - Enabled\n");
              verbosity += 1;
              debug_os = true;
              debug_ofs = true;
              break;

            case QUASH_FLAG_DEBUG_FILE:
              // Debug Mode (--debug-no-file)
              DBG_print("Enabled output to file\n");
              DBG::out::instance().verbosity(DBG::out::instance().verbosity() + 1);
              debug_ofs = true;
              verbosity += 1;
              break;

            case QUASH_FLAG_DEBUG_STDERR:
              // Debug Mode (--debug-no-stderr)
              DBG::out::instance().verbosity(DBG::out::instance().verbosity() + 1);
              DBG_print("Enabled output to stderr\n");
              debug_os = true;
              verbosity += 1;
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

      // Shutdown debug module if not needed
      if (!debug_os && !debug_ofs) {
        DBG::out::instance().shutdown();
      }
      else {
        DBG::out::instance().ofsEnable(debug_ofs);
        DBG::out::instance().osEnable(debug_os);
        DBG::out::instance().enable(debug_os || debug_ofs);
        DBG::out::instance().verbosity(verbosity - 1);
      }
    }
  }


  void main::initCmdFlags() {
    mCmdFlags["-h"] = QUASH_FLAG_HELP;
    mCmdFlags["--help"] = QUASH_FLAG_HELP;

    mCmdFlags["-d"] = QUASH_FLAG_DEBUG;
    mCmdFlags["--debug"] = QUASH_FLAG_DEBUG;

    mCmdFlags["-df"] = QUASH_FLAG_DEBUG_FILE;
    mCmdFlags["--debug-file"] = QUASH_FLAG_DEBUG_FILE;

    mCmdFlags["-de"] = QUASH_FLAG_DEBUG_STDERR;
    mCmdFlags["--debug-stderr"] = QUASH_FLAG_DEBUG_STDERR;

    mCmdFlags["--print-env"] = QUASH_FLAG_PRINT_ENV;
  }


  void main::initEnv(char **envp) {
    mEnv = envp;
  }


  void main::usage() const {
    std::cout << "Usage: ./quash\n";
  }
}  // namespace QUASH
