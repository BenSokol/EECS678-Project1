/**
* @Filename: QUASH_main.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:59pm]
* @Modified: September 26th, 2019 [2:37am]
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
    void initEnv(const char *const *const envp);
    void printEnv() const;

    uint8_t mStatus;
    std::map<std::string, std::string> mEnv;
  };
}  // namespace QUASH
