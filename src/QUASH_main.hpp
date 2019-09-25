/**
* @Filename: QUASH_main.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  September 23rd, 2019 [7:59pm]
* @Modified: September 25th, 2019 [6:09pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdlib>

namespace QUASH {
  class main {
   public:
    main(int argc, const char *argv[]);
    ~main();

    uint8_t status() const;

   private:
    uint8_t mStatus;

    //TODO: Setup map of environment variables
  };
}  // namespace QUASH
