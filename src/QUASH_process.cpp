/**
* @Filename: QUASH_process.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  October 9th, 2019 [2:24pm]
* @Modified: October 19th, 2019 [6:36pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <algorithm>  // std::min
#include <deque>      // std::deque
#include <string>     // std::string
#include <thread>     // std::thread

#include <unistd.h>

#include "QUASH_process.hpp"

#include "DBG_out.hpp"
#include "QUASH_public.hpp"
#include "QUASH_tokenizer.hpp"
#include "UTL_assert.h"

namespace QUASH {

  process::process(const std::deque<std::string> _tokens) :
      status(STATUS_SUCCESS), errorMessage(""), async(true), done(false), pid(0), tokens(_tokens) {
    // Should set default values of status, async, initDone, done
    // check for async

    UTL_assert(tokens.size() > 0);

    for (size_t i = 0; i < tokens.size(); i++) {
      if (tokens[i] == "&") {
        if (i == tokens.size() - 1) {
          async = false;
        }
        else {
          status = STATUS_COMMAND_SYNTAX_ERROR;
          errorMessage = "";
        }
        break;
      }
      else if (tokens[i] == "exit") {
        status = STATUS_COMMAND_SEMANTIC_ERROR;
        errorMessage = "\'exit\' can only be called asynchronously.";
      }
      else if (tokens[i] == "quit") {
        status = STATUS_COMMAND_SEMANTIC_ERROR;
        errorMessage = "\'quit\' can only be called asynchronously.";
      }
      else if (tokens[i] == "logout") {
        status = STATUS_COMMAND_SEMANTIC_ERROR;
        errorMessage = "\'logout\' can only be called asynchronously.";
      }
      else if (tokens[i] == "kill") {
        status = STATUS_COMMAND_SEMANTIC_ERROR;
        errorMessage = "\'kill\' can only be called asynchronously.";
      }
      else if (tokens[i] == "jobs") {
        status = STATUS_COMMAND_SEMANTIC_ERROR;
        errorMessage = "\'jobs\' can only be called asynchronously.";
      }
      else if (tokens[i] == "|") {
        pipes.push_back(new int[2]);
        int pStatus = pipe(pipes.back());
        if (pStatus != 0) {
          errorMessage = "Failed to create pipe.";
          UTL_assert_always();
        }
      }
    }
  }


  process::~process() {
    // Ensure everything is cleaned up
    if (thread.joinable()) {
      thread.join();
    }

    // Delete pipes
    for (auto& p : pipes) {
      delete[] p;
    }
  }


  void process::command() {
    // Bulk of work:
    //   can create new process started on process::thread that can be run
    //   needs to handle setting status
    //   handle internal quash commands (perhaps using seperate function)
    //     example: quit, exit, help, set, cd, jobs
    //     Possible extra commands: alias, unalias
    //   I/O Redirection >/</>>/<<
    // Should fork/exec call

    if (status != STATUS_SUCCESS) {
      UTL_assert_always();
      return;
    }

    tid = std::this_thread::get_id();

    size_t begCommand = 0;
    size_t endCommand = 0;
    size_t i = 0;

    while (i < tokens.size()) {
      begCommand = i;

      bool foundPipe = false;

      // Find end of command
      for (; i < tokens.size(); i++) {
        if (tokens[i] == "|") {
          // Advance to next command after pipe
          DBG_printv(1, "Found pipe at ", i, "\n");
          i++;
          foundPipe = true;
          break;
        }
        endCommand = i;
      }

      UTL_assert(begCommand <= endCommand);
      if (DBG::out::instance().enabled()) {
        DBG_print("Running command [",
                  begCommand,
                  ", ",
                  endCommand,
                  "]: \n",
                  QUASH::Tokenizer::str(std::deque<std::string>(tokens.begin() + static_cast<char>(begCommand),
                                                                tokens.begin() + static_cast<char>(endCommand)),
                                        false),
                  "\n");
        DBG::out::instance().wait();
      }

      //
      // if (piped == true) {
      //   call pipedInputs?
      // }
      //
      // //put I/O redirection here?
      // execv(tokens[0], arguments);

      // if (tokens[0] == "exit" || tokens[0] == "quit" || tokens[0] == "logout") {
      //   status = STATUS_EXIT_NORMAL;
      // }
      // else if (tokens[0] == "kill") {
      //   //if the id that is passed to kill is a jobid that is currently
      //   //being used, map it to a processid then kill it
      //   //otherwise use the built-in kill process
      //
      //   //int jobID = atoi(tokens[1]);
      //   /*for(int i == 0; i<processes().size; i++)
      //   {
      //     if(i == jobID)
      //     {
      //       //kill the process with that JobID
      //     }
      //     else
      //     {
      //       general kill command
      //     }
      //   }
      //   */
      // }
      // else if (tokens[0] == "cd") {
      //   //if(tokens.size() !> 1)
      //   //{
      //   //change directory to home directory
      //   //}
      //   //else
      //   //{
      //   //  std::string newDir = tokens[1];
      //   //  set to new directory
      //   //}
      // }
      // else if (tokens[0] == "jobs") {
      //   //status = STATUS_COMMAND_JOBS;
      //   //int jobID = atoi(tokens[1]);
      //   /*for(int i == 0; i<processes().size; i++)
      //   {
      //     std::cout << "[" << i << "] " << processes()[i].tokens[1];
      //   }
      //   */
      // }
      // else if (tokens[0] == "set") {
      //   //status = STATUS_COMMAND_SET;
      // }
      // else if (tokens[0] == "help") {
      //   //status = STATUS_COMMAND_HELP;
      //   //std::cout <<
      // }
    }
  }

  // void pipeInputs(const std::deque<std::string> newTokens, process* pipedProcess) {
  //   pipedProcess->start();
  //   pid_t pid_1;
  //   int dataPipe[2];
  //   pipe(dataPipe);
  //   pid_1 = fork();
  //   if (pid_1 == 0) {
  //     close(dataPipe[0]);
  //     dup2(dataPipe[1], output?);
  //
  //     execv(newTokens[1], arguments);
  //   }
  // }


  void process::start() {
    if (async) {
      command();
    }
    else {
      thread = std::thread(&process::command, this);
    }
  }
}  // namespace QUASH
