/**
* @Filename: main.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  October 19th, 2019 [7:00pm]
* @Modified: October 21st, 2019 [5:45am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#define CATCH_CONFIG_DEFAULT_REPORTER "compact"
#define CATCH_CONFIG_RUNNER
#include "TEST_public.hpp"
#include "catch.hpp"

char** TEST_env;

int main(int argc, char* argv[], char** envp) {
  // global setup...
  TEST_env = envp;

  int result = Catch::Session().run(argc, argv);

  // global clean-up...

  return result;
}
