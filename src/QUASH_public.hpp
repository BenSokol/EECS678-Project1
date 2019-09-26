/**
* @Filename: QUASH_public.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:20pm]
* @Modified: September 26th, 2019 [2:31am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdint>
#include <limits>
#include <string>

namespace QUASH {
  typedef enum STATUS_CODES : uint8_t {
    STATUS_SUCCESS = 0,

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
