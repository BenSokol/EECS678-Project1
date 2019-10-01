/**
* @Filename: QUASH_main.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:59pm]
* @Modified: October 1st, 2019 [2:38am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdint>
#include <cstdlib>
#include <map>
#include <string>


namespace QUASH {
  class main {
  public:
    main(const int argc, const char *const *const argv, const char *const *const envp);
    ~main();

    uint8_t run();

  private:
    void initArgs(const int argc, const char *const *const argv);
    void initEnv(const char *const *const envp);
    void postInit();
    void preInit();
    void printEnv() const;
    void usage() const;

    typedef enum QUASH_CMD_FLAGS : uint8_t {
      QUASH_FLAG_HELP,
      QUASH_FLAG_DEBUG,
      QUASH_FLAG_DEBUG_FILE,
      QUASH_FLAG_DEBUG_NO_STDERR,
      QUASH_FLAG_PRINT_ENV
    } QUASH_CMD_FLAG;

    bool mPrintEnv;
    bool mDisplayUsage;


    uint8_t mStatus;

    std::map<std::string, QUASH_CMD_FLAG> mCmdFlags;
    std::map<std::string, std::string> mEnv;
  };
}  // namespace QUASH
