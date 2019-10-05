//Takes in a path variable(map) and a string (filename)
//mEnv["PATH"] returns the value
//std::string Commands::which(std::string PATH, std::string exec)
//mEnv["PATH"] is the first argument
//Search each path, check if file exists (std::filesystem::exists)

#include <string>

namespace QUASH::COMMANDS {
  const std::string which();
}  // namespace QUASH::COMMANDS
