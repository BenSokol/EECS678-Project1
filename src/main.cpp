/**
* @Filename: main.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  August 28th, 2019 [2:01pm]
* @Modified: October 9th, 2019 [5:57pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include "QUASH_main.hpp"

int main(int argc, char** argv, char** envp) {
  QUASH::main::instance().init(argc, argv, envp);
  return QUASH::main::instance().run();
}
