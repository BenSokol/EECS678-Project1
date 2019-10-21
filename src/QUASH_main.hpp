/**
* @Filename: QUASH_main.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:59pm]
* @Modified: October 20th, 2019 [7:48pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef QUASH_MAIN_HPP
#define QUASH_MAIN_HPP

#include <csetjmp>  // jmp_buf
#include <cstdint>  // uint8_t

#include <map>     // std::map
#include <string>  // std::string

#include "QUASH_public.hpp"

namespace QUASH {
  class main {
  public:
    main();
    ~main();

    static main &instance() {
      static main m;
      return m;
    }

    void init(int argc, char **argv, char **envp);

    quash_status_t run();

  protected:
    void checkJobStatus();
    std::string getInput();
    [[noreturn]] static void signalHandlerSIGINT(int signum);

    void initArgs(int argc, char **argv);
    void initCmdFlags();
    void initEnv(char **envp);

    void printEnv() const;
    void usage() const;

    typedef enum QUASH_CMD_FLAGS : uint8_t {
      QUASH_FLAG_HELP,
      QUASH_FLAG_DEBUG,
      QUASH_FLAG_DEBUG_STDERR,
      QUASH_FLAG_DEBUG_FILE,
      QUASH_FLAG_PRINT_ENV
    } QUASH_CMD_FLAG;

    jmp_buf mJumpBufferSIGINT;

    bool mPrintEnv;          // Display environment?
    bool mDisplayUsage;      // Display help and exit?
    quash_status_t mStatus;  // Status

    std::map<std::string, QUASH_CMD_FLAG> mCmdFlags;  // Const map of Command Line Flags to QUASH_CLI_FLAGS enum
  };
}  // namespace QUASH

#endif
