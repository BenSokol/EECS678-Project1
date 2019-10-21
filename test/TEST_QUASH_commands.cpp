/**
* @Filename: TEST_QUASH_commands.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  October 20th, 2019 [4:04am]
* @Modified: October 20th, 2019 [6:55pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdlib>

#include <unistd.h>

#include "QUASH_home.hpp"
#include "QUASH_hostname.hpp"
#include "QUASH_ps1.hpp"
#include "QUASH_public.hpp"
#include "QUASH_pwd.hpp"
#include "QUASH_whoami.hpp"
#include "UTL_colors.hpp"
#include "catch.hpp"

#if __has_include(<filesystem>)
  #include <filesystem>
  #ifndef std_filesystem
    #define std_filesystem std::filesystem
  #endif
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem>
  #ifndef std_filesystem
    #define std_filesystem std::experimental::filesystem
  #endif
#else
  #error Requires std::filesystem or std::experimental::filesystem
#endif

TEST_CASE(__COUNTER__ + "QUASH::COMMANDS::home returns empty string if home isnt set", "[QUASH::COMMANDS]") {
  char *prev_env = getenv("HOME");
  unsetenv("HOME");
  std::string ret = QUASH::COMMANDS::home();
  REQUIRE(ret == "");
  setenv("HOME", prev_env, 1);
}

TEST_CASE("QUASH::COMMANDS::home returns correct string", "[QUASH::COMMANDS]") {
  char *prev_env = getenv("HOME");
  setenv("HOME", "/home/test", 1);
  std::string ret = QUASH::COMMANDS::home();
  REQUIRE(ret == "/home/test");
  setenv("HOME", prev_env, 1);
}

TEST_CASE("QUASH::COMMANDS::hostname MANUAL CHECK", "[QUASH::COMMANDS][!mayfail]") {
  std::string ret = QUASH::COMMANDS::hostname();
  WARN("IS THIS CORRECT? " + ret);
}

TEST_CASE("QUASH::COMMANDS::ps1 MANUAL CHECK", "[QUASH::COMMANDS]") {
  UTL::COLOR::setMode(UTL::COLOR::OFF);
  std::string ret = QUASH::COMMANDS::ps1();
  std::string expected =
    std::string(getenv("USER")) + "@" + QUASH::COMMANDS::hostname() + ":" + QUASH::COMMANDS::pwd() + "$ ";
  REQUIRE(expected == ret);
}

TEST_CASE("QUASH::COMMANDS::pwd returns correct string", "[QUASH::COMMANDS]") {
  std::string ret = QUASH::COMMANDS::pwd();
  std::string expected = std_filesystem::current_path();
  try {
    char *homedir_chr = getenv("HOME");
    if (homedir_chr != nullptr) {
      std::string homedir_str = std::string(homedir_chr);
      expected.replace(expected.find(homedir_str), homedir_str.length(), "~");
    }
  } catch (std::exception &) {}
  REQUIRE(expected == ret);
}

TEST_CASE("QUASH::COMMANDS::whoami returns correct string", "[QUASH::COMMANDS]") {
  std::string ret = QUASH::COMMANDS::whoami();
  std::string expected = getenv("USER");
  REQUIRE(expected == ret);
}
