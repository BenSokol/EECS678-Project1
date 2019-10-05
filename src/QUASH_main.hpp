/**
* @Filename: QUASH_main.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:59pm]
* @Modified: October 4th, 2019 [6:34pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef QUASH_MAIN_HPP
#define QUASH_MAIN_HPP

#include <cstdint>  // uint8_t

#include <deque>
#include <future>
#include <map>     // std::map
#include <string>  // std::string
#include <vector>

namespace QUASH {
  class main {
  public:
    main(const int argc, const char *const *const argv, const char *const *const envp);
    ~main();

    uint8_t run();

  private:
    class process {
    public:
      // TODO: Move function definitions to QUASH_main.cpp
      process(const std::deque<std::string> _args) : args(_args) {
        // Should set default values of status, async, initDone, done
        // check for async
        /*
        status = 0;
        async = 0;
        done = false;

        for(int i = 0; i < _args.size(); i++)
        {
          if(_args[i] == "&")
          {
            async = 1;
            initDone = true;
            break;
          }
          else
          {
            initDone = false;
          }
        }
        */

      }

      ~process() {
        // Ensure everything is cleaned up
        if (thread.joinable()) {
          thread.join();
        }
      }

      void command() {
        // Bulk of work:
        //   uses which
        //   can create new process started on process::thread that can be run
        //   needs to handle setting status
        //   handle internal quash commands (perhaps using seperate function)
        //     example: quit, exit, help, set, cd, jobs
        //     Possible extra commands: alias, unalias
        //   I/O Redirection >/</>>/<<
        // Should fork/exec call


        //std::string execName = args[0]; //grabs the main exec name
        //std::string location = QUASH::which(execName);

      }

      void start() {
        if (async) {
          thread = std::thread(&QUASH::main::process::command, this);
        }
        else {
          command();
        }
      }

      // May need mutex to prevent data race of various variables like status
      uint8_t status;                      // Status
      uint8_t async;                       // Is this command async? (i.e. & == true)
      bool initDone;                       // true after async is determined
      bool done;                           // When command has finished == true
      std::thread thread;                  // Thread to use for run
      std::thread::id pid;                 // id of process
      const std::deque<std::string> args;  // arguments for process
    };

    std::string getInput();
    void initArgs(const int argc, const char *const *const argv);
    void initEnv(const char *const *const envp);
    void postInit();
    void preInit();
    void printEnv() const;
    void usage() const;

    typedef enum QUASH_CLI_FLAGS : uint8_t {
      QUASH_FLAG_HELP,
      QUASH_FLAG_DEBUG,
      QUASH_FLAG_DEBUG_STDERR,
      QUASH_FLAG_DEBUG_FILE,
      QUASH_FLAG_PRINT_ENV
    } QUASH_CLI_FLAG;


    bool mPrintEnv;      // Display environment?
    bool mDisplayUsage;  // Display help and exit?
    uint8_t mStatus;     // Status

    std::map<std::string, QUASH_CLI_FLAG> mCmdFlags;  // Const map of Command Line Flags to QUASH_CLI_FLAGS enum
    std::map<std::string, std::string> mEnv;          // Map of environment variables
    std::vector<process *> mProcesses;                // Processes running in async
  };
}  // namespace QUASH

#endif
