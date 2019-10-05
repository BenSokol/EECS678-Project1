/**
* @Filename: QUASH_main.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [8:00pm]
* @Modified: October 4th, 2019 [6:20pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdint>  // uint8_t

#include <deque>
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
    if (DBG::out::instance().enabled()) {
      DBG::out::instance().wait();
    }
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
      if (DBG::out::instance().enabled()) {
        DBG::out::instance().wait();
      }

      // TODO: Check if any async processes finished
      /*
       for(int i = 0; i < mProcesses.size(); ++i)
        {
          if(mProcesses[i].done == true)
          {
            std::cout << mProcesses.pid << " Done.\n";
            mProcesses.erase(mProcesses.at(i));
          }
        }
        */

      // Prints PS1
      std::cout << COMMANDS::ps1();

      // Gets input from user
      std::string input_string = getInput();

      // Detects if CTRL-D (EOF) has been entered. If so, exits program.
      if (std::cin.eof()) {
        DBG_write(false, false, true, false, "\n");
        DBG_print("EOF has been detected, exiting...\n");
        break;
      }

      // Tokenize input string
      auto retTokenizer = Tokenizer::Tokenize(input_string);

      // Handle status from tokenize
      if (retTokenizer.first != STATUS_SUCCESS) {
        // Handle error from Tokenize;
      }

      // DEBUG: Print tokenized string
      if (DBG::out::instance().enabled()) {
        Tokenizer::print(retTokenizer.second);
      }

      // TODO: Check if any async processes finished
      /*
       for(int i = 0; i < mProcesses.size(); ++i)
        {
          if(mProcesses[i].done == true)
          {
            std::cout << mProcesses.pid << " Done.\n";
            mProcesses.erase(mProcesses.at(i));
          }
        }
        */

      // ================================
      // TODO: Goal for stage 1
      // system("ls -lha /bin");
      // ================================

      // Can assume (hard code if needed) ls is at /bin/ls will find using QUASH::which when complete
      process *p = new process({ "ls", "-lha", "/bin" });

      while (!p->initDone) {
        std::this_thread::yield();
      }

      if (p->status != STATUS_SUCCESS) {
        // TODO: Handle
      }

      // Add process to vector of processes
      mProcesses.push_back(p);

      // start process
      p->start();

      // Wait for process to finish if not async
      if (!p->async) {
        while (!p->done) {
          std::this_thread::yield();
        }
      }

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

  // uint8_t main::runCommand(process &p) {
  //   std::vector<const char *> args;
  //   bool hasPipe = false;
  //   for (size_t i = 0; i < p.args.size(); ++i) {
  //     if (p.args[i].compare("|")) {
  //       hasPipe = true;
  //       break;
  //     }
  //     args.push_back(p.args[i].c_str());
  //   }
  //   args.push_back(nullptr);
  //
  //   p.initDone = true;
  //
  //   if (hasPipe) {
  //     std::deque<std::string> postPipe;
  //     std::move(p.args.begin() + hasPipe, p.args.end(), postPipe.begin());
  //   }
  //   else {
  //     // call fork/exec using
  //     // args.data() will return char * const*
  //   }
  // }


  std::string main::getInput() {
    std::string inputString;
    std::getline(std::cin, inputString);
    return inputString;
  }


  void main::initArgs(const int argc, const char *const *const argv) {
    if (argc > 1) {
      bool debug_os = false;
      bool debug_ofs = false;
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
              debug_os = true;
              debug_ofs = true;
              break;

            case QUASH_FLAG_DEBUG_FILE:
              // Debug Mode (--debug-no-file)
              DBG_print("Enabled output to file\n");
              debug_ofs = true;
              break;

            case QUASH_FLAG_DEBUG_STDERR:
              // Debug Mode (--debug-no-stderr)
              DBG_print("Enabled output to stderr\n");
              debug_os = true;
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

      DBG::out::instance().ofsEnable(debug_ofs);
      DBG::out::instance().osEnable(debug_os);
      DBG::out::instance().enable(debug_os || debug_ofs);

      // Shutdown debug module if not needed
      if (!debug_os && !debug_ofs) {
        DBG::out::instance().shutdown();
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

    mCmdFlags["-df"] = QUASH_FLAG_DEBUG_FILE;
    mCmdFlags["--debug-file"] = QUASH_FLAG_DEBUG_FILE;

    mCmdFlags["-de"] = QUASH_FLAG_DEBUG_STDERR;
    mCmdFlags["--debug-stderr"] = QUASH_FLAG_DEBUG_STDERR;

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
