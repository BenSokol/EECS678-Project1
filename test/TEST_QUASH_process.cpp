/**
* @Filename: TEST_QUASH_process.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  October 20th, 2019 [6:09pm]
* @Modified: October 20th, 2019 [6:57pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdlib>

#include <deque>
#include <string>

#include <unistd.h>

#include "QUASH_home.hpp"
#include "QUASH_hostname.hpp"
#include "QUASH_process.hpp"
#include "QUASH_ps1.hpp"
#include "QUASH_public.hpp"
#include "QUASH_pwd.hpp"
#include "QUASH_whoami.hpp"
#include "catch.hpp"

TEST_CASE("QUASH::process cd - empty", "[QUASH::process][QUASH::process::cd]") {
  std::deque<std::string> tokens = { "cd" };
  QUASH::process p(tokens);
  p.start();
  REQUIRE(QUASH::COMMANDS::home() == QUASH::COMMANDS::pwd(false));
}

TEST_CASE("QUASH::process cd - /usr/bin", "[QUASH::process][QUASH::process::cd]") {
  std::deque<std::string> tokens = { "cd", "/usr/bin" };
  QUASH::process p(tokens);
  p.start();
  REQUIRE("/usr/bin" == QUASH::COMMANDS::pwd(false));
}

TEST_CASE("QUASH::process cd - /usr, then bin", "[QUASH::process][QUASH::process::cd]") {
  std::deque<std::string> tokens1 = { "cd", "/usr" };
  QUASH::process p1(tokens1);
  p1.start();
  REQUIRE("/usr" == QUASH::COMMANDS::pwd(false));

  std::deque<std::string> tokens2 = { "cd", "bin" };
  QUASH::process p2(tokens2);
  p2.start();
  REQUIRE("/usr/bin" == QUASH::COMMANDS::pwd(false));
}

TEST_CASE("QUASH::process cd - ~/Desktop", "[QUASH::process][QUASH::process::cd]") {
  std::deque<std::string> tokens = { "cd", "~/Desktop" };
  QUASH::process p(tokens);
  p.start();
  REQUIRE("~/Desktop" == QUASH::COMMANDS::pwd(true));
  REQUIRE((std::string(QUASH::COMMANDS::home()) + "/Desktop") == QUASH::COMMANDS::pwd(false));
}


TEST_CASE("QUASH::process cd - ~/bin after changing HOME to /usr",
          "[QUASH::process][QUASH::process::cd][QUASH::process::set]") {
  std::deque<std::string> tokens1 = { "set", "HOME", "/usr" };
  QUASH::process p1(tokens1);
  p1.start();
  REQUIRE("/usr" == std::string(QUASH::COMMANDS::home()));

  std::deque<std::string> tokens2 = { "cd" };
  QUASH::process p2(tokens2);
  p2.start();
  REQUIRE("~/" == QUASH::COMMANDS::pwd(true));
  REQUIRE(QUASH::COMMANDS::home() == QUASH::COMMANDS::pwd(false));

  std::deque<std::string> tokens3 = { "cd", "bin" };
  QUASH::process p3(tokens3);
  p3.start();
  REQUIRE("~/bin" == QUASH::COMMANDS::pwd(true));
  REQUIRE((std::string(QUASH::COMMANDS::home()) + "/bin") == QUASH::COMMANDS::pwd(false));
}

TEST_CASE("QUASH::process set - remains same from previous test",
          "[QUASH::process][QUASH::process::cd][QUASH::process::set]") {
  std::deque<std::string> tokens2 = { "cd" };
  QUASH::process p2(tokens2);
  p2.start();
  REQUIRE("~/" == QUASH::COMMANDS::pwd(true));
  REQUIRE(QUASH::COMMANDS::home() == QUASH::COMMANDS::pwd(false));

  std::deque<std::string> tokens3 = { "cd", "bin" };
  QUASH::process p3(tokens3);
  p3.start();
  REQUIRE("~/bin" == QUASH::COMMANDS::pwd(true));
  REQUIRE((std::string(QUASH::COMMANDS::home()) + "/bin") == QUASH::COMMANDS::pwd(false));
}
