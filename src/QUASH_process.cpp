/**
* @Filename: QUASH_process.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  October 9th, 2019 [2:24pm]
* @Modified: October 20th, 2019 [6:07pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <algorithm>  // std::min
#include <deque>      // std::deque
#include <iostream>   // std::cerr
#include <string>     // std::string
#include <thread>     // std::thread

#include <unistd.h>

#include "QUASH_process.hpp"

#include "DBG_out.hpp"
#include "QUASH_home.hpp"
#include "QUASH_public.hpp"
#include "QUASH_pwd.hpp"
#include "QUASH_tokenizer.hpp"
#include "UTL_assert.h"

namespace QUASH {

  process::process(const std::deque<std::string> _tokens) :
      status(STATUS_SUCCESS), p_status(0), errorMessage(""), async(true), done(false), pid(0), tokens(_tokens) {
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
      else if (tokens[i] == "|") {
        pipes.push_back(new int[2]);
        int pStatus = pipe(pipes.back());
        if (pStatus != 0) {
          errorMessage = "Failed to create pipe.";
          UTL_assert_always();
        }
      }
    }

    if (!async) {
      for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "exit") {
          status = STATUS_COMMAND_SEMANTIC_ERROR;
          errorMessage = "\'exit\' can only be called asynchronously.";
          break;
        }
        else if (tokens[i] == "quit") {
          status = STATUS_COMMAND_SEMANTIC_ERROR;
          errorMessage = "\'quit\' can only be called asynchronously.";
          break;
        }
        else if (tokens[i] == "logout") {
          status = STATUS_COMMAND_SEMANTIC_ERROR;
          errorMessage = "\'logout\' can only be called asynchronously.";
          break;
        }
        else if (tokens[i] == "kill") {
          status = STATUS_COMMAND_SEMANTIC_ERROR;
          errorMessage = "\'kill\' can only be called asynchronously.";
          break;
        }
        else if (tokens[i] == "jobs") {
          status = STATUS_COMMAND_SEMANTIC_ERROR;
          errorMessage = "\'jobs\' can only be called asynchronously.";
          break;
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

    size_t i = 0;

    while (i < tokens.size()) {
      std::deque<std::string> currentCommand;

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
        currentCommand.push_back(tokens[i]);
      }

      UTL_assert(currentCommand.size() > 0);
      if (DBG::out::instance().enabled()) {
        DBG_print("Running command:\n", QUASH::Tokenizer::str(currentCommand, false));
        DBG::out::instance().wait();
      }

      if (currentCommand[0] == "exit" || currentCommand[0] == "quit" || currentCommand[0] == "logout") {
        status = STATUS_EXIT_NORMAL;
        return;
      }
      else if (currentCommand[0] == "kill") {
        //if the id that is passed to kill is a jobid that is currently
        //being used, map it to a processid then kill it
        //otherwise use the built-in kill process

        //int jobID = atoi(currentCommand[1]);
        /*for(int i == 0; i<processes().size; i++)
        {
          if(i == jobID)
          {
            //kill the process with that JobID
          }
          else
          {
            general kill command
          }
        }
        */
      }
      else if (currentCommand[0] == "cd") {
        if (currentCommand.size() > 2) {
          std::cerr << "-quash: cd: too many arguments\n";
          status = STATUS_COMMAND_RUNTIME_ERROR;
          return;
        }
        else {
          if (currentCommand.size() == 1) {
            // Changing directory to home
            const char* dir = QUASH::COMMANDS::home();

            if (DBG::out::instance().enabled()) {
              DBG_print("Changing directories to: ", dir, "\n");
              DBG::out::instance().wait();
            }

            p_status = chdir(dir);
          }
          else {
            if (currentCommand[1][0] == '/') {
              // Changing directory to root
              const char* dir = currentCommand[1].c_str();
              if (DBG::out::instance().enabled()) {
                DBG_print("Changing directories to: ", dir, "\n");
                DBG::out::instance().wait();
              }
              p_status = chdir(dir);
            }
            else if (currentCommand[1][0] == '~') {
              // Changing directory to home path
              std::string str = std::string(QUASH::COMMANDS::home()) + currentCommand[1].substr(1);
              const char* dir = str.c_str();
              if (DBG::out::instance().enabled()) {
                DBG_print("Changing directories to: ", dir, "\n");
                DBG::out::instance().wait();
              }
              p_status = chdir(dir);
            }
            else {
              std::string str = QUASH::COMMANDS::pwd(false) + "/" + currentCommand[1];
              const char* dir = str.c_str();
              if (DBG::out::instance().enabled()) {
                DBG_print("Changing directories to: ", dir, "\n");
                DBG::out::instance().wait();
              }
              p_status = chdir(dir);
            }
          }

          if (p_status != 0) {
            status = STATUS_COMMAND_RUNTIME_ERROR;
            return;
          }
        }
      }
      else if (currentCommand[0] == "jobs") {
        for (size_t j = 0; QUASH::process::processes().size(); ++j) {
          std::cout << "[" << j << "]\t" << QUASH::process::processes()[j]->pid << "\t"
                    << "running"
                    << "\t";
          std::cout << QUASH::Tokenizer::str(QUASH::process::processes()[j]->tokens, true);
          std::cout << "\n";
        }
      }
      else if (currentCommand[0] == "set") {
        if (currentCommand.size() != 3) {
          std::cerr << "-quash: set: expected 2 arguments [NAME] [VALUE]\n";
          status = STATUS_COMMAND_RUNTIME_ERROR;
          return;
        }
        else {
          p_status = setenv(currentCommand[1].c_str(), currentCommand[2].c_str(), 1);

          if (p_status != 0) {
            status = STATUS_COMMAND_RUNTIME_ERROR;
            return;
          }
        }
      }
      else if (currentCommand[0] == "help") {
        //status = STATUS_COMMAND_HELP;
        //std::cout <<
      }
      else {
        // if (0 == fork()) {
        //   // Child
        //   // call exec if not custom command
        // }
        // else {
        //   // Parent
        //   // wait for child
        // }
      }
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
