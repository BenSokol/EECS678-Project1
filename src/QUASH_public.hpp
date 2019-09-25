/**
* @Filename: QUASH_public.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:20pm]
* @Modified: September 25th, 2019 [5:51pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdlib>
#include <limits>
#include <string>

namespace QUASH {
  typedef enum STATUS_CODES : uint8_t {
    STATUS_SUCCESS = 0,
    STATUS_TOKENIZER_INVALID_CHAR = 1,
    STATUS_UNKNOWN_ERROR = std::numeric_limits<uint8_t>::max(),
  } STATUS;


  std::string getInput();
}  // namespace QUASH
