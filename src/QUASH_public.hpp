/**
* @Filename: QUASH_public.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:20pm]
* @Modified: October 21st, 2019 [12:00am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef QUASH_PUBLIC_HPP
#define QUASH_PUBLIC_HPP

#include <cstdint>  // uint8_t

#include <deque>
#include <limits>  // std::numeric_limits
#include <memory>
#include <mutex>

namespace QUASH {
  typedef enum quash_status_t : uint8_t {
    STATUS_SUCCESS = 0,

    STATUS_INIT_UNKNOWN_COMMAND_LINE_PARAMETER,

    STATUS_TOKENIZER_EMPTY,
    STATUS_TOKENIZER_MISSING_CLOSE_SINGLE_QUOTE,
    STATUS_TOKENIZER_MISSING_CLOSE_DOUBLE_QUOTE,

    STATUS_TOKENIZER_ERROR_NAME,  // Not Used

    STATUS_COMMAND_NOT_FOUND,
    STATUS_COMMAND_NO_ARGS,
    STATUS_COMMAND_SYNTAX_ERROR,
    STATUS_COMMAND_SEMANTIC_ERROR,
    STATUS_COMMAND_RUNTIME_ERROR,
    STATUS_COMMAND_PRINT_JOBS,
    STATUS_COMMAND_KILL_JOB,

    STATUS_EXIT_NORMAL,
    STATUS_EXIT_ERROR_NAME,  // Not Used

    STATUS_UNKNOWN = std::numeric_limits<uint8_t>::max(),
  } STATUS_CODE;

  class process;
  extern std::pair<std::deque<std::shared_ptr<process>>, std::mutex> *mProcesses;

}  // namespace QUASH

#endif
