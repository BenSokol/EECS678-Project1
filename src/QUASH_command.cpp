//This is the CPP file for Command

#include "QUASH_command.hpp"


namespace QUASH{

    Command::Command() {

    }

    Command::~Command() {

    }

    uint8_t Command::findExecutable(std::string exeName) {
      //Search the PATH and return (and run?) the executable if found
      //Determine which file to use: pwd? home?
      //Return success (0) if found
      //Give error if not found
    }

    uint8_t Command::runExecutable() { //Probably needs more parameters
      //Execute the processes
      //This is probably where fork() and exec() is used?
    }


}
