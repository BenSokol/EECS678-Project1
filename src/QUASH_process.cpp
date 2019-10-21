/**
* @Filename: QUASH_process.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  October 9th, 2019 [2:24pm]
* @Modified: October 21st, 2019 [12:29pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <csignal>  // kill
#include <cstring>

#include <algorithm>  // std::min
#include <deque>      // std::deque
#include <iostream>   // std::cerr
#include <string>     // std::string
#include <thread>     // std::thread

#include <sys/cdefs.h>
#include <sys/wait.h>
#include <unistd.h>

#include "QUASH_process.hpp"

#include "DBG_out.hpp"
#include "QUASH_cd.hpp"
#include "QUASH_home.hpp"
#include "QUASH_public.hpp"
#include "QUASH_pwd.hpp"
#include "QUASH_tokenizer.hpp"
#include "QUASH_usage.hpp"
#include "UTL_assert.h"
#include "UTL_inputValidation.hpp"

#if __has_include(<filesystem>)
  #include <filesystem>
  #ifndef std_filesystem
    #define std_filesystem std::filesystem
  #endif
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem>
  #ifndef std_filesystem
    #define std_filesystem std::experimental::filesystem
  #endif
#else
  #error Requires std::filesystem or std::experimental::filesystem
#endif

char** environ;

namespace QUASH {

  process::process(std::deque<std::string> _tokens,
                   char** envp,
                   std::pair<std::deque<std::shared_ptr<process>>, std::mutex>* processes) :
      mProcesses(processes),
      status(STATUS_SUCCESS),
      p_status(0),
      errorMessage(""),
      async(true),
      done(false),
      pid(0),
      tokens(_tokens),
      mEnv(envp) {
    if (tokens.size() == 0) {
      status = STATUS_COMMAND_NO_ARGS;
      UTL_assert_always();
      return;
    }

    for (size_t i = 0; i < tokens.size(); i++) {
      if (tokens[i] == "&") {
        if (i == tokens.size() - 1) {
          async = false;
          tokens.pop_back();
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
    if (status != STATUS_SUCCESS) {
      done = true;
      UTL_assert_always();
      return;
    }

    tid = std::this_thread::get_id();

    size_t i = 0;

    while (i < tokens.size()) {
      std::deque<std::string> currentCommand;

      // Find end of command
      for (; i < tokens.size(); i++) {
        if (tokens[i] == "|") {
          // Advance to next command after pipe
          DBG_printv(1, "Found pipe at ", i, "\n");
          i++;
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
        done = true;
        return;
      }
      else if (currentCommand[0] == "kill") {
        if (currentCommand.size() != 3) {
          std::cerr << "-quash: kill: expected 2 arguments [SIGNUM] [JOBID]\n";
          status = STATUS_COMMAND_RUNTIME_ERROR;
          done = true;
          return;
        }
        if (!UTL::isNumber(currentCommand[2])
            || !UTL::isNumber(currentCommand[1], std::numeric_limits<int>::min(), std::numeric_limits<int>::max())) {
          std::cerr << "-quash: kill: expected 2 arguments [SIGNUM] [JOBID]\nusage: kill [SIGNUM] [JOBID]\n";
          status = STATUS_COMMAND_RUNTIME_ERROR;
          done = true;
          return;
        }
        int kill_jobid = std::stoi(currentCommand[2]);
        int kill_signum = std::stoi(currentCommand[1]);
        std::unique_lock<std::mutex> lock(mProcesses->second);
        if (mProcesses->first.empty()) {
          std::cerr << "-quash: kill: No processes are running.\nusage: kill [SIGNUM] [JOBID]\n";
          status = STATUS_COMMAND_RUNTIME_ERROR;
          done = true;
          return;
        }
        if (mProcesses->first.size() < static_cast<size_t>(kill_jobid)
            || mProcesses->first[static_cast<size_t>(kill_jobid) - 1] == nullptr) {
          std::cerr << "-quash: kill: JOBID out of range\nusage: kill [SIGNUM] [JOBID]\n";
          status = STATUS_COMMAND_RUNTIME_ERROR;
          done = true;
          return;
        }

        std::cout << "Sending signal " << currentCommand[1] << " to job " << currentCommand[2];
        std::cout << "\t" << QUASH::Tokenizer::str(mProcesses->first.back()->tokens, true) << "\n";
        if (kill(mProcesses->first[static_cast<size_t>(kill_jobid) - 1]->pid, kill_signum) == 0) {
          mProcesses->first.erase(mProcesses->first.begin() + static_cast<long>(kill_jobid) - 1);
        }
        else {
          DBG_print("Warning: kill failed...\n");
        }
        continue;
      }
      else if (currentCommand[0] == "cd") {
        if (!QUASH::COMMANDS::cd(currentCommand, status, p_status)) {
          done = true;
          return;
        }
        continue;
      }
      else if (currentCommand[0] == "jobs") {
        status = STATUS_COMMAND_PRINT_JOBS;
        done = true;
        return;
      }
      else if (currentCommand[0] == "set") {
        if (currentCommand.size() < 2 || currentCommand.size() > 3) {
          std::cerr << "-quash: set: expected 1 or 2 arguments [NAME] [VALUE], or [NAME]=[VALUE]\n";
          status = STATUS_COMMAND_RUNTIME_ERROR;
          done = true;
          return;
        }
        else {
          std::string name = "";
          std::string value = "";
          if (currentCommand.size() == 2) {
            size_t sep = currentCommand[1].find_first_of("=");
            if (sep == std::string::npos) {
              name = currentCommand[1];
            }
            else {
              name = currentCommand[1].substr(0, sep);
              value = currentCommand[1].substr(sep + 1);
            }
          }
          else if (currentCommand.size() == 3) {
            name = currentCommand[1];
            value = currentCommand[2];
          }
          else {
            UTL_assert_always();
          }

          p_status = setenv(name.c_str(), value.c_str(), 1);

          for (char** env = mEnv; *env != nullptr; env++) {
            std::string row = *env;
            size_t sep = row.find_first_of("=");
            if ((sep == std::string::npos && row.substr(0, row.size() - 1).compare(name) == 0)
                || (row.substr(0, sep).compare(name) == 0)) {
              *env = new char[name.size() + value.size() + 3];
              std::strcpy((*env), name.c_str());
              char equal[] = "=";
              std::strcat((*env), equal);
              std::strcat((*env), value.c_str());
              break;
            }
          }

          if (p_status != 0) {
            status = STATUS_COMMAND_RUNTIME_ERROR;
            done = true;
            return;
          }
        }
        continue;
      }
      else if (currentCommand[0] == "help") {
        std::cout << QUASH::COMMANDS::usage();
        continue;
      }
      else {
        if (mEnv == nullptr) {
          UTL_assert_always();
        }

        // Find PATH from mEnv if on macOS
        std::string PATH = "";
        for (char** env = const_cast<char**>(mEnv); *env != nullptr; env++) {
          std::string row = *env;
          size_t sep = row.find_first_of("=");
          if (sep == std::string::npos && row.substr(0, row.size() - 1).compare("PATH") == 0) {
            PATH = getenv("PATH");
          }
          else if (row.substr(0, sep).compare("PATH") == 0) {
            PATH = row.substr(sep + 1);
          }
        }

        // Convert ~ to HOME (as long as ~ is not first character in string)
        for (size_t j = 0; j < currentCommand.size(); ++j) {
          if (currentCommand[j][0] == '~') {
            std::string newCommand = QUASH::COMMANDS::home();
            if (currentCommand[j].size() > 1) {
              newCommand += currentCommand[j].substr(1);
            }
            currentCommand[j] = newCommand;
          }
        }

        // Check if executable is in PATH
        bool found = false;
        std::stringstream ss;
        ss << PATH;
        std::string str;
        while (getline(ss, str, ':')) {
          std_filesystem::path fs_path = str + "/" + currentCommand[0];
          if (std_filesystem::exists(fs_path)) {
            found = true;
            break;
          }
        }

        // Check if executable is absolute
        if (!found) {
          std_filesystem::path fs_abs_path = currentCommand[0];
          if (std_filesystem::exists(fs_abs_path)) {
            found = true;
          }
        }

        // Check if executable is relative
        if (!found) {
          std_filesystem::path fs_rel_path = std_filesystem::current_path();
          fs_rel_path += "/";
          fs_rel_path += currentCommand[0];
          if (std_filesystem::exists(fs_rel_path)) {
            found = true;
          }
        }

        // Return if not found
        if (!found) {
          status = STATUS_COMMAND_NOT_FOUND;
          errorMessage = "Command \'" + currentCommand[0] + "\' not found.";
          done = true;
          return;
        }

        pids.push_back(fork());
        if (pids.back() == 0) {
          // child

          std::vector<char*> argv_list;
          std::transform(
            currentCommand.begin(), currentCommand.end(), std::back_inserter(argv_list), [](const std::string& s) {
              char* pc = new char[s.size() + 1];
              std::strcpy(pc, s.c_str());
              return pc;
            });

          argv_list.push_back(nullptr);

          size_t thisProcess = 0;
          for (size_t j = 0; j < pids.size(); ++j) {
            if (pids[j] == 0) {
              thisProcess = j;
            }
          }

          if (thisProcess != pipes.size()) {
            dup2(pipes[thisProcess][1], STDOUT_FILENO);
          }

          if (thisProcess != 0) {
            dup2(pipes[thisProcess - 1][0], STDIN_FILENO);
          }

          for (size_t j = 0; j < pipes.size(); ++j) {
            if (j == thisProcess) {
              close(pipes[j][0]);
            }
            else if (j + 1 == thisProcess && thisProcess != 0) {
              close(pipes[j][1]);
            }
            else {
              close(pipes[j][0]);
              close(pipes[j][1]);
            }
          }

#if __APPLE__
          // macOS does NOT have the same function calls as linux apparently.
          // The following line makes the environment available to exec
          // (environ is an extern variable defined in unistd.h on macOS)
          environ = mEnv;
          execvP(currentCommand[0].c_str(), PATH.c_str(), const_cast<char**>(argv_list.data()));
#else
          execvpe(currentCommand[0].c_str(), const_cast<char**>(argv_list.data()), mEnv);
#endif
        }
      }
    }

    // Parent only, child cant get here.
    for (size_t j = 0; j < pids.size(); ++j) {
      if (pids[j] == 0) {
        return;
      }
    }

    mProcessStartedCondition.notify_one();

    if (pids.size() > 0) {
      pid = pids.front();
    }

    std::for_each(pipes.begin(), pipes.end(), [](int* p) {
      close(p[0]);
      close(p[1]);
    });

    int wait_status;
    waitpid(pid, &wait_status, 0);

    p_status = wait_status;

    if (p_status != 0) {
      status = STATUS_COMMAND_RUNTIME_ERROR;
      return;
    }

    done = true;
  }  // namespace QUASH


  void process::start() {
    if (async) {
      command();
    }
    else {
      thread = std::thread(&process::command, this);
      std::unique_lock<std::mutex> lock(mProcessStartedMutex);
      mProcessStartedCondition.wait(lock);
    }
  }
}  // namespace QUASH
