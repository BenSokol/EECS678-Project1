//This is the CPP file for Command

#include <cstdint>  // uint8_t

#include <map>     // std::map
#include <string>  // std::string

#include "QUASH_command.hpp"

#include "QUASH_public.hpp"  // QUASH::STATUS_CODES

namespace QUASH {

  Command::Command(const std::map<std::string, std::string> &aEnv) : mEnv(aEnv) {
  }

  uint8_t Command::findExecutable(const std::string &exeName) {
    //Search the PATH and return (and run?) the executable if found
    //Determine which file to use: pwd? home?
    //Return success (0) if found
    //Give error if not found

    // TODO: Delete - Prevents compiler warning
    static_cast<void>(exeName);

    return STATUS_SUCCESS;
  }

  uint8_t Command::runExecutable() {  //Probably needs more parameters
    //Execute the processes
    //This is probably where fork() and exec() is used?

    return STATUS_SUCCESS;
  }


}  // namespace QUASH
