/**
* @Filename: QUASH_public.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:20pm]
* @Modified: October 1st, 2019 [2:04am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdint>
#include <limits>
#include <map>
#include <string>

namespace QUASH {
  typedef enum STATUS_CODES : uint8_t {
    STATUS_SUCCESS = 0,

    STATUS_INIT_UNKNOWN_COMMAND_LINE_PARAMETER,
    STATUS_INIT_INVALID_COMMAND_LINE_PARAMETER,
    STATUS_INIT_MISSING_COMMAND_LINE_PARAMETER,

    STATUS_TOKENIZER_INVALID_TOKEN,
    STATUS_TOKENIZER_ERROR_NAME,

    STATUS_COMMAND_NOT_FOUND,
    STATUS_COMMAND_ERROR_NAME,

    STATUS_EXIT_NORMAL,
    STATUS_EXIT_ERROR_NAME,

    STATUS_UNKNOWN = std::numeric_limits<uint8_t>::max(),
  } STATUS;

  std::string getInput();
}  // namespace QUASH
