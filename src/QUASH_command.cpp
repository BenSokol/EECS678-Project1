//This is the CPP file for Command

#include "QUASH_command.hpp"


namespace QUASH{

    Command::Command(std::map<std::string, std::string> mEnv) {
      path = mEnv;
    }

    Command::~Command() {

    }

    uint8_t Command::findExecutable(std::vector<std::string> exeName) { //not needed
      //Search the PATH and return (and run?) the executable if found

      //Determine which file to use: pwd? home?
      //Return success (0) if found
      //Give error if not found
    }

    uint8_t Command::runExecutable(std::string filePath, std::string exec) { //Probably needs more parameters
      //Execute the processes
      //This is probably where fork() and exec() is used?
    }


}
