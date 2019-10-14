/**
* @Filename: QUASH_process.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  October 9th, 2019 [2:24pm]
* @Modified: October 9th, 2019 [9:08pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include "QUASH_process.hpp"

#include "QUASH_public.hpp"

namespace QUASH {

  process::process(const std::deque<std::string>& _tokens) :
      status(STATUS_SUCCESS), async(true), initDone(false), done(false), tid(0), pid(0), tokens(_tokens) {
    // Should set default values of status, async, initDone, done
    // check for async

    // for (int i = 0; i < _args.size(); i++) {
    //   if (_args[i] == "&") {
    //     async = false;
    //     initDone = true;
    //     break;
    //   }
    //   else {
    //     initDone = true;
    //   }
    // }
  }

  process::~process() {
    // Ensure everything is cleaned up
    if (thread.joinable()) {
      thread.join();
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

    // std::string execName = args[0]; //grabs the main exec name
    // std::string location = QUASH::which(execName);
    // int pipe_a[2]


     for (int i = 0; i < tokens.size(); i++) {
       if (tokens[i] == "|") {
         //Create new list of tokens from the tokens after the Pipe
         //process* pipedProcess = new Process(newTokens, piped = true);
         //may need a new bool for process stating whether or not it is piped or not.
         //this should allow it to take in an input from another process


         break;
       }
     }
    //
    // if (piped == true) {
    //   call pipedInputs?
    // }
    //
    // //put I/O redirection here?
    // execv(tokens[0], arguments);

    if(tokens[0] == "exit" || tokens[0] == "quit" || tokens[0] == "logout")
    {
      status = STATUS_EXIT_NORMAL;
    }
    else if(tokens[0] == "kill")
    {
      //if the id that is passed to kill is a jobid that is currently
      //being used, map it to a processid then kill it
      //otherwise use the built-in kill process

      //int jobID = atoi(tokens[1]);
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
    else if(tokens[0] == "cd")
    {
      //if(tokens.size() !> 1)
      //{
          //change directory to home directory
      //}
      //else
      //{
      //  std::string newDir = tokens[1];
      //  set to new directory
      //}
    }
    else if(tokens[0] == "jobs")
    {
      //status = STATUS_COMMAND_JOBS;
      //int jobID = atoi(tokens[1]);
      /*for(int i == 0; i<processes().size; i++)
      {
        std::cout << "[" << i << "] " << processes()[i].tokens[1];
      }
      */
    }
    else if(tokens[0] == "set")
    {
      //status = STATUS_COMMAND_SET;
    }
    else if(tokens[0] == "help")
    {
      //status = STATUS_COMMAND_HELP;
      //std::cout <<
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
