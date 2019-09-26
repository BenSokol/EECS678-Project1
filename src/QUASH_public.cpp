/**
* @Filename: QUASH_public.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 25th, 2019 [5:52pm]
* @Modified: September 26th, 2019 [2:26am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <iostream>
#include <string>

#include "QUASH_public.hpp"

namespace QUASH {
  std::string getInput() {
    std::string inputString;
    std::getline(std::cin, inputString);
    return inputString;
  }
}  // namespace QUASH
