/**
* @Filename: QUASH_process.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  October 9th, 2019 [2:29pm]
* @Modified: October 20th, 2019 [4:15pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef QUASH_PROCESS_HPP
#define QUASH_PROCESS_HPP

#include <cstdint>  // uint8_t

#include <deque>   // std::deque
#include <string>  // std::string
#include <thread>  // std::thread
#include <vector>  // std::vector

#include <sys/types.h>  // pid_t

#include "QUASH_public.hpp"

namespace QUASH {
  class process {
  public:
    // Defined in QUASH_process.cpp
    explicit process(const std::deque<std::string> _tokens);
    ~process();
    void command();
    void start();

    static std::vector<process *> processes() {
      return std::vector<process *>();
    }

    // Access data directly from main
    friend class main;

  protected:
    quash_status_t status;     // Status
    intmax_t p_status;         // Process status
    std::string errorMessage;  // Message if error occured.

    std::vector<int *> pipes;

    uint8_t async;  // Is this command async? (i.e. & == false)
    bool done;      // When command has finished == true

    std::thread thread;  // Thread to use for run

    std::thread::id tid;  // id of thread
    pid_t pid;            // id of process

    const std::deque<std::string> tokens;  // arguments for process
  };

}  // namespace QUASH


#endif
