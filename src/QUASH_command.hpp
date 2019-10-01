/*This class should return a status from QUASH_public
Return code is an u_int8
Should search PATH until the executable specified is found
Should utilize fork() and exec()

*/
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>

#include "QUASH_public.hpp"


namespace QUASH {
  class Command {
  public:
    Command(std::map<std::string, std::string> mEnv);
    ~Command();

    uint8_t findExecutable(const std::string &exeName);

    uint8_t runExecutable();  //Probably needs more parameters
  };

}  // namespace QUASH
#endif
