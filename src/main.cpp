/**
* @Filename: main.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  August 28th, 2019 [2:01pm]
* @Modified: September 26th, 2019 [2:18am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include "QUASH_main.hpp"

int main(const int argc, const char *const *const argv, const char *const *const envp) {
  QUASH::main m = QUASH::main(argc, argv, envp);
  return m.run();
}
