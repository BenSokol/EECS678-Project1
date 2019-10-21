/**
* @Filename: QUASH_cd.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 25th, 2019 [5:49pm]
* @Modified: October 20th, 2019 [8:07pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdlib>

#include <deque>
#include <iostream>
#include <string>  // std::string

#include <unistd.h>

#include "QUASH_cd.hpp"

#include "DBG_out.hpp"
#include "QUASH_home.hpp"
#include "QUASH_public.hpp"
#include "QUASH_pwd.hpp"

namespace QUASH::COMMANDS {
  bool cd(std::deque<std::string> &currentCommand, quash_status_t &status, long &p_status) {
    if (currentCommand.size() > 2) {
      if (DBG::out::instance().enabled()) {
        DBG::out::instance().wait();
      }
      std::cerr << "-quash: cd: too many arguments\n";
      status = STATUS_COMMAND_RUNTIME_ERROR;
      return false;
    }
    else {
      std::string str = "";

      if (currentCommand.size() == 1) {
        // Changing directory to home
        str = QUASH::COMMANDS::home();
      }
      else {
        if (currentCommand[1][0] == '/') {
          // Changing directory to root
          str = currentCommand[1].c_str();
        }
        else if (currentCommand[1][0] == '~') {
          // Changing directory to home path
          str = std::string(QUASH::COMMANDS::home()) + currentCommand[1].substr(1);
        }
        else {
          str = QUASH::COMMANDS::pwd(false) + "/" + currentCommand[1];
        }
      }

      const char *dir = str.c_str();
      if (DBG::out::instance().enabled()) {
        DBG_print("Changing directories to: ", dir, "\n");
        DBG::out::instance().wait();
      }
      p_status = chdir(dir);


      if (p_status != 0) {
        status = STATUS_COMMAND_RUNTIME_ERROR;
        return false;
      }
    }

    return true;
  }
}  // namespace QUASH::COMMANDS
