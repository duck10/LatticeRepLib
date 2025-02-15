#ifndef COMMAND_SYSTEM_2_H
#define COMMAND_SYSTEM_2_H

#include "StringMatcher.h"
#include "LRL_StringTools.h"
#include <functional>
#include <string>
#include <map>
#include <vector>

// Forward declaration
class BaseControlVariables;

struct Command {
   std::string name;
   std::function<void(BaseControlVariables&, const std::string&)> handler;
   double threshold;

   Command() : name(""), handler(nullptr), threshold(0.2) {}
   Command(const std::string& n,
      std::function<void(BaseControlVariables&, const std::string&)> h,
      double t = 0.2)
      : name(n), handler(h), threshold(t) {}
};

class CommandSystem {
private:
   std::vector<Command> commands;
   StringMatcher matcher;

   double getMatchScore(const std::string& cmd1, const std::string& cmd2) const {
      return matcher.matches(cmd1, cmd2) ? matcher.getTheta(cmd1, cmd2) : 1.0;
   }

public:
   CommandSystem() : matcher(0.2) {}

   // Add copy constructor and assignment operator
   CommandSystem(const CommandSystem&) = default;
   CommandSystem& operator=(const CommandSystem&) = default;

   void clear() {
      commands.clear();
   }

   void registerCommand(const std::string& name,
      std::function<void(BaseControlVariables&, const std::string&)> handler,
      double threshold = 0.2) {
      commands.emplace_back(name, handler, threshold);
   }

   bool executeCommand(BaseControlVariables& controls,
      const std::string& cmdName,
      const std::string& value) const {
      const std::string upperCmd = LRL_StringTools::strToupper(cmdName);
      double bestScore = 1.0;
      const Command* bestMatch = nullptr;

      // Find best matching command
      for (const auto& cmd : commands) {
         double score = getMatchScore(upperCmd, cmd.name);
         if (score <= cmd.threshold && score < bestScore) {
            bestScore = score;
            bestMatch = &cmd;
         }
      }

      if (bestMatch) {
         bestMatch->handler(controls, value);
         return true;
      }
      return false;
   }
};

#endif // COMMAND_SYSTEM_2_H
