#ifndef COMMAND_SYSTEM_H
#define COMMAND_SYSTEM_H

#include "StringMatcher.h"
#include "LRL_StringTools.h"
#include "TNear.h"
#include <functional>
#include <string>

class BaseControlVariables;  // Forward declaration

struct Command {
   std::string name;
   std::function<void(BaseControlVariables&, const std::string&)> handler;
   static inline StringMatcher matcher{ 0.4 };  // Default threshold

   Command(const std::string& n, std::function<void(BaseControlVariables&, const std::string&)> h)
      : name(n), handler(h) {}
   Command() : name(""), handler(nullptr) {}

   Command operator-(const Command& other) const {
      Command result;
      result.name = name + "\n" + other.name;
      return result;
   }

   double norm() const {
      size_t delim = name.find('\n');
      if (delim != std::string::npos) {
         std::string s1 = name.substr(0, delim);
         std::string s2 = name.substr(delim + 1);
         const double theta = matcher.getTheta(s1, s2);
         return theta;
      }
      return 0.0;
   }
};

class CommandSystem {
private:
   CNearTree<Command> commandTree;

public:
   void registerCommand(const std::string& name,
      std::function<void(BaseControlVariables&, const std::string&)> handler) {
      commandTree.insert(Command(name, handler));
   }

   bool executeCommand(BaseControlVariables& controls,
      const std::string& cmdName,
      const double threshold,
      const std::string& value) const {
      Command closest;
      Command probe(LRL_StringTools::strToupper(cmdName), nullptr);
      if (commandTree.NearestNeighbor(threshold, closest, probe)) {
         closest.handler(controls, value);
         return true;
      }
      return false;
   }

   bool executeCommand(BaseControlVariables& controls,
      const std::string& cmdName,
      const std::string& value) const {
      return executeCommand(controls, cmdName, 0.4, value);  // Use default threshold
   }

   void clear() {
      commandTree = CNearTree<Command>();
   }
};

#endif // COMMAND_SYSTEM_H
