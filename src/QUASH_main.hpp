/**
* @Filename: QUASH_main.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:59pm]
* @Modified: October 2nd, 2019 [10:26pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef QUASH_MAIN_HPP
#define QUASH_MAIN_HPP

#include <cstdint>  // uint8_t

#include <map>     // std::map
#include <string>  // std::string

namespace QUASH {
  class main {
  public:
    main(const int argc, const char *const *const argv, const char *const *const envp);
    ~main();

    uint8_t run();

  private:
    std::string getInput();
    void initArgs(const int argc, const char *const *const argv);
    void initEnv(const char *const *const envp);
    void postInit();
    void preInit();
    void printEnv() const;
    void usage() const;

    typedef enum QUASH_CLI_FLAGS : uint8_t {
      QUASH_FLAG_HELP,
      QUASH_FLAG_DEBUG,
      QUASH_FLAG_DEBUG_FILE,
      QUASH_FLAG_DEBUG_NO_STDERR,
      QUASH_FLAG_PRINT_ENV
    } QUASH_CLI_FLAG;

    bool mPrintEnv;
    bool mDisplayUsage;

    uint8_t mStatus;

    std::map<std::string, QUASH_CLI_FLAG> mCmdFlags;
    std::map<std::string, std::string> mEnv;
  };
}  // namespace QUASH

#endif
