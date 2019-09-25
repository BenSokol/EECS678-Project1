/**
* @Filename: main.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  August 28th, 2019 [2:01pm]
* @Modified: September 25th, 2019 [7:45pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include "QUASH_main.hpp"

int main(int argc, const char *argv[]) {
  QUASH::main m = QUASH::main(argc, argv);
  return m.status();
}
