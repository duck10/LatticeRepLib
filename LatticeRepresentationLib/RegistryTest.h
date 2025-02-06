#ifndef REGISTRYTEST_H
#define REGISTRYTEST_H

#include "TNear.h"
#include <functional>
#include <iostream>
#include <string>

#include "StringMatcher.h"
#include "LRL_StringTools.h"

// Move Command outside of CommandSystem
struct Command {
   std::string name;
   std::function<void(const std::string&)> handler;
   static inline StringMatcher matcher{ 0.2 };  // inline static member

   Command(const std::string& n, std::function<void(const std::string&)> h)
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
   void registerCommand(const std::string& name, std::function<void(const std::string&)> handler) {
      commandTree.insert(Command(name, handler));
   }

   bool executeCommand(const std::string& cmdName,
      const double threshold, const std::string& value) const {
      Command closest;
      Command probe(LRL_StringTools::strToupper(cmdName), nullptr);
      if (commandTree.NearestNeighbor(threshold, closest, probe)) {
         closest.handler(value);
         return true;
      }
      return false;
   }

   bool executeCommand(const std::string& cmdName, const std::string& value) const {
      Command closest;
      Command probe(LRL_StringTools::strToupper(cmdName), nullptr);
      if (commandTree.NearestNeighbor(0.2, closest, probe)) {
         closest.handler(value);
         return true;
      }
      return false;
   }
};

// Rest of your ControlRegistry class stays the same...

#endif //  REGISTRYTEST_H
