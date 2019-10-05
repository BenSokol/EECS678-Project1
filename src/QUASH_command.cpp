//This is the CPP file for Command

#include <cstdint>  // uint8_t

#include <map>     // std::map
#include <string>  // std::string

#include "QUASH_command.hpp"

#include "QUASH_public.hpp"  // QUASH::STATUS_CODES

namespace QUASH {

  Command::Command(const std::map<std::string, std::string> &aEnv) : mEnv(aEnv) {
  }

  uint8_t Command::runExecutable() {  //Probably needs more parameters
    //Execute the processes
    //This is probably where fork() and exec() is used?

    return STATUS_SUCCESS;
  }


}  // namespace QUASH
