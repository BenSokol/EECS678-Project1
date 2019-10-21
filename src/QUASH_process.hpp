/**
* @Filename: QUASH_process.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  October 9th, 2019 [2:29pm]
* @Modified: October 21st, 2019 [12:29am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef QUASH_PROCESS_HPP
#define QUASH_PROCESS_HPP

#include <cstdint>  // uint8_t

#include <condition_variable>  // std::condition_variable
#include <deque>               // std::deque
#include <mutex>               // std::mutex, std::unique_lock
#include <string>              // std::string
#include <thread>              // std::thread
#include <vector>              // std::vector

#include <sys/types.h>  // pid_t

#include "QUASH_public.hpp"

namespace QUASH {
  class process {
  public:
    // Defined in QUASH_process.cpp
    explicit process(std::deque<std::string> _tokens,
                     char **envp,
                     std::pair<std::deque<std::shared_ptr<process>>, std::mutex> *processes);
    ~process();
    void command();
    void start();

    // Access data directly from main
    friend class main;

  protected:
    std::pair<std::deque<std::shared_ptr<process>>, std::mutex> *mProcesses;
    quash_status_t status;     // Status
    intmax_t p_status;         // Process status
    std::string errorMessage;  // Message if error occured.

    std::vector<int *> pipes;

    uint8_t async;  // Is this command async? (i.e. & == false)
    bool done;      // When command has finished == true

    std::thread thread;  // Thread to use for run

    std::thread::id tid;      // id of thread
    pid_t pid;                // id of process
    std::vector<pid_t> pids;  // vector of ids of processes

    std::mutex mProcessStartedMutex;
    std::condition_variable mProcessStartedCondition;

    std::deque<std::string> tokens;  // arguments for process
    char **mEnv;
  };

}  // namespace QUASH


#endif
