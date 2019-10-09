/**
* @Filename: QUASH_main.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:59pm]
* @Modified: October 8th, 2019 [5:18am]
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

#include "QUASH_public.hpp"
namespace QUASH {
  class main {
  public:
    main(const int argc, const char *const *const argv, const char *const *const envp);
    ~main();

    quash_status_t run();

  private:
    class process {
    public:
      // TODO: Move function definitions to QUASH_main.cpp
      explicit process(const std::deque<std::string> &_tokens) :
          status(STATUS_SUCCESS), async(false), initDone(false), done(false), pid(0), tokens(_tokens) {
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
        //int pipe_a[2]


        /*
          for(int i = 0; i < tokens.size(); i++)
          {
            if(tokens[i] == "|")
            {
              //Create new list of tokens from the tokens after the Pipe
              process* pipedProcess = new Process(newTokens, piped = true);
              pipedProcess->start();
              //may need a new bool for process stating whether or not it is piped or not.
              //this will allow it to take in an input from another process

              break;
            }
          }

          if(piped == true)
          {
            close(dataPipe[1]);
            dup2(dataPipe[0], input?);
          }

          //put I/O redirection here?
          execv(tokens[0]);
        */
      }

      /*
      void pipeInputs(const std::deque<std::string> tokens, process* pipedProcess)
      {
        pid_t pid_1;
        int dataPipe[2];
        pipe(dataPipe);
        pid_1 = fork();
        if(pid_1 == 0)
        {
          close(dataPipe[0]);
          dup2(dataPipe[1], output?);

          //execute
        }
      }
      */

      void start() {
        if (async) {
          thread = std::thread(&QUASH::main::process::command, this);
        }
        else {
          command();
        }
      }

      // May need mutex to prevent data race of various variables like status
      quash_status_t status;                 // Status
      uint8_t async;                         // Is this command async? (i.e. & == true)
      bool initDone;                         // true after async is determined
      bool done;                             // When command has finished == true
      std::thread thread;                    // Thread to use for run
      std::thread::id pid;                   // id of process
      const std::deque<std::string> tokens;  // arguments for process
      //bool piped;                          // determines if the process is a piped one or not
    };

    void checkJobStatus();
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


    bool mPrintEnv;          // Display environment?
    bool mDisplayUsage;      // Display help and exit?
    quash_status_t mStatus;  // Status

    std::map<std::string, QUASH_CLI_FLAG> mCmdFlags;  // Const map of Command Line Flags to QUASH_CLI_FLAGS enum
    std::map<std::string, std::string> mEnv;          // Map of environment variables
    std::vector<process *> mProcesses;                // Processes running in async
  };
}  // namespace QUASH

#endif
