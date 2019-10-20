/**
* @Filename: TEST_QUASH_Tokenizer.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  October 20th, 2019 [2:19am]
* @Modified: October 20th, 2019 [4:28am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include "QUASH_public.hpp"
#include "QUASH_tokenizer.hpp"
#include "catch.hpp"

TEST_CASE("QUASH::Tokenize returns SUCCESS and empty deque if no tokens are provided", "[QUASH::Tokenizer]") {
  auto ret = QUASH::Tokenizer::Tokenize("");
  REQUIRE(ret.second.size() == 0);
  REQUIRE(ret.first == QUASH::STATUS_SUCCESS);
}

TEST_CASE("QUASH::Tokenize returns SUCCESS and correct deque if 1 token is provided", "[QUASH::Tokenizer]") {
  auto ret = QUASH::Tokenizer::Tokenize("ls");
  REQUIRE(ret.second.size() == 1);
  REQUIRE(ret.second[0] == "ls");
  REQUIRE(ret.first == QUASH::STATUS_SUCCESS);
}

TEST_CASE("QUASH::Tokenize returns SUCCESS and correct deque if 2 tokens are provided", "[QUASH::Tokenizer]") {
  std::string token0 = "ls";
  std::string token1 = "/usr/bin";

  auto ret = QUASH::Tokenizer::Tokenize(token0 + " " + token1);
  REQUIRE(ret.second.size() == 2);
  REQUIRE(ret.second[0] == token0);
  REQUIRE(ret.second[1] == token1);
  REQUIRE(ret.first == QUASH::STATUS_SUCCESS);
}

TEST_CASE("QUASH::Tokenize returns SUCCESS and correct deque if 2 tokens are provided with string",
          "[QUASH::Tokenizer]") {
  std::string token0 = "ls";
  std::string token1 = "\"/usr/bin\"";

  auto ret = QUASH::Tokenizer::Tokenize(token0 + " " + token1);
  REQUIRE(ret.second.size() == 2);
  REQUIRE(ret.second[0] == token0);
  REQUIRE(ret.second[1] == token1);
  REQUIRE(ret.first == QUASH::STATUS_SUCCESS);
}

TEST_CASE("QUASH::Tokenize returns FAILURE and empty deque if unclosed single quote occurs", "[QUASH::Tokenizer]") {
  std::string token0 = "ls";
  std::string token1 = "\'/usr/bin";

  auto ret = QUASH::Tokenizer::Tokenize(token0 + " " + token1);
  REQUIRE(ret.second.size() == 0);
  REQUIRE(ret.first == QUASH::STATUS_TOKENIZER_MISSING_CLOSE_SINGLE_QUOTE);
}

TEST_CASE("QUASH::Tokenize returns FAILURE and empty deque if unclosed double quote occurs", "[QUASH::Tokenizer]") {
  std::string token0 = "ls";
  std::string token1 = "\"/usr/bin";

  auto ret = QUASH::Tokenizer::Tokenize(token0 + " " + token1);
  REQUIRE(ret.second.size() == 0);
  REQUIRE(ret.first == QUASH::STATUS_TOKENIZER_MISSING_CLOSE_DOUBLE_QUOTE);
}

TEST_CASE("QUASH::Tokenize returns FAILURE and empty deque if unclosed double quote occurs after single quote",
          "[QUASH::Tokenizer]") {
  std::string token0 = "ls";
  std::string token1 = "\'\"/usr/bin";

  auto ret = QUASH::Tokenizer::Tokenize(token0 + " " + token1);
  REQUIRE(ret.second.size() == 0);
  REQUIRE(ret.first == QUASH::STATUS_TOKENIZER_MISSING_CLOSE_SINGLE_QUOTE);
}

TEST_CASE("QUASH::Tokenize returns SUCCESS and correct deque if no space between pipe and token",
          "[QUASH::Tokenizer]") {
  auto ret = QUASH::Tokenizer::Tokenize("ls \"/usr/bin\" |echo");
  REQUIRE(ret.second.size() == 4);
  REQUIRE(ret.second[0] == "ls");
  REQUIRE(ret.second[1] == "\"/usr/bin\"");
  REQUIRE(ret.second[2] == "|");
  REQUIRE(ret.second[3] == "echo");
  REQUIRE(ret.first == QUASH::STATUS_SUCCESS);
}

TEST_CASE("QUASH::Tokenize returns SUCCESS and correct deque if many tokens are provided", "[QUASH::Tokenizer]") {
  std::deque<std::string> results = { "find", ".",     "-name", "\"*.cpp\"", "-or",       "-name",     "\'*.hpp\'",
                                      "|",    "xargs", "grep",  "-c",        "cout",      "|",         "sort",
                                      "-t",   ":",     "+1.0",  "-2.0",      "--numeric", "--reverse", "|",
                                      "head", "-n",    "5",     ">",         "out.txt" };

  std::string input = "";
  for (size_t i = 0; i < results.size(); ++i) {
    input += results[i];
    if (i < results.size() - 1) {
      input += " ";
    }
  }

  auto ret = QUASH::Tokenizer::Tokenize(input);
  REQUIRE(ret.second.size() == results.size());
  REQUIRE(ret.second == results);
  REQUIRE(ret.first == QUASH::STATUS_SUCCESS);
}

TEST_CASE("QUASH::Tokenize returns SUCCESS and correct deque if only comment is detected", "[QUASH::Tokenizer]") {
  auto ret = QUASH::Tokenizer::Tokenize("# this is a comment");
  REQUIRE(ret.second.size() == 0);
  REQUIRE(ret.first == QUASH::STATUS_SUCCESS);
}

TEST_CASE("QUASH::Tokenize returns SUCCESS and correct deque if comment is detected", "[QUASH::Tokenizer]") {
  auto ret = QUASH::Tokenizer::Tokenize("ls # this is a comment");
  REQUIRE(ret.second.size() == 1);
  REQUIRE(ret.second[0] == "ls");
  REQUIRE(ret.first == QUASH::STATUS_SUCCESS);
}

TEST_CASE("QUASH::Tokenize::str returns correct string", "[QUASH::Tokenizer]") {
  auto ret = QUASH::Tokenizer::Tokenize("ls # this is a comment");
  std::string str = QUASH::Tokenizer::str(ret.second, true);
  REQUIRE(ret.second.size() == 1);
  REQUIRE(ret.second[0] == "ls");
  REQUIRE(ret.first == QUASH::STATUS_SUCCESS);
  REQUIRE(str == "ls");
}

TEST_CASE("QUASH::Tokenize::str returns correct string for long input", "[QUASH::Tokenizer]") {
  std::deque<std::string> results = { "find", ".",     "-name", "\"*.cpp\"", "-or",       "-name",     "\'*.hpp\'",
                                      "|",    "xargs", "grep",  "-c",        "cout",      "|",         "sort",
                                      "-t",   ":",     "+1.0",  "-2.0",      "--numeric", "--reverse", "|",
                                      "head", "-n",    "5",     ">",         "out.txt" };

  std::string input = "";
  for (size_t i = 0; i < results.size(); ++i) {
    input += results[i];
    if (i < results.size() - 1) {
      input += " ";
    }
  }

  auto ret = QUASH::Tokenizer::Tokenize(input);
  std::string str = QUASH::Tokenizer::str(ret.second, true);
  REQUIRE(ret.second.size() == results.size());
  REQUIRE(ret.second == results);
  REQUIRE(ret.first == QUASH::STATUS_SUCCESS);
  REQUIRE(str == input);
}
