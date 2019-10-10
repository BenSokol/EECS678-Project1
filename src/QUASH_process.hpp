/**
* @Filename: QUASH_process.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  October 9th, 2019 [2:29pm]
* @Modified: October 9th, 2019 [6:13pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef QUASH_PROCESS_HPP
#define QUASH_PROCESS_HPP

#include <cstdint>

#include <deque>
#include <string>
#include <thread>

#include <sys/types.h>
#include <unistd.h>

#include "QUASH_public.hpp"

namespace QUASH {
  class process {
  public:
    // Defined in QUASH_process.cpp
    explicit process(const std::deque<std::string> &_tokens);
    ~process();
    void command();
    void start();

    // Access data directly from main
    friend class main;

  private:
    quash_status_t status;                 // Status
    uint8_t async;                         // Is this command async? (i.e. & == false)
    bool initDone;                         // true after async is determined
    bool done;                             // When command has finished == true
    std::thread thread;                    // Thread to use for run
    std::thread::id tid;                   // id of thread
    pid_t pid;                             // id of process
    const std::deque<std::string> tokens;  // arguments for process
  };
}  // namespace QUASH


#endif
