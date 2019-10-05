/*This class should return a status from QUASH_public
Return code is an u_int8
Should search PATH until the executable specified is found
Should utilize fork() and exec()

*/

#ifndef QUASH_COMMAND_HPP
#define QUASH_COMMAND_HPP

#include <map>     // std::map
#include <string>  // std::string

namespace QUASH {
  class Command {
  public:
    explicit Command(const std::map<std::string, std::string> &aEnv);
    ~Command();

    uint8_t runExecutable();  //Probably needs more parameters

  private:
    std::map<std::string, std::string> mEnv;
  };

}  // namespace QUASH

#endif
