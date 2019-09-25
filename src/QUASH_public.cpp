/**
* @Filename: QUASH_public.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 25th, 2019 [5:52pm]
* @Modified: September 25th, 2019 [6:06pm]
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
    // std::cin.clear();
    // std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return inputString;
  }
}  // namespace QUASH
