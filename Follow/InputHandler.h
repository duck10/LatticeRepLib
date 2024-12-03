#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "ControlVariables.h"
#include "G6.h"
#include "S6.h"
#include "LatticeCell.h"
#include "LRL_Cell.h"
#include "StringMatcher.h"


#include <string>
#include <vector>
#include <functional>
#include <istream>

class InputHandler {
private:
   struct FollowerModeMatch {
      std::string mode;
      FollowerMode enumVal;
      double threshold;
      FollowerModeMatch(const std::string& m, FollowerMode e, double t)
         : mode(m), enumVal(e), threshold(t) {}
   };

   struct CommandHandler {
      std::string command;
      double threshold;
      std::function<void(ControlVariables&, const std::string&)> handler;

      CommandHandler(const std::string& cmd, double t,
         std::function<void(ControlVariables&, const std::string&)> h)
         : command(cmd), threshold(t), handler(h) {}
   };

   static const std::vector<FollowerModeMatch> FOLLOWER_MODES;
   static const std::vector<CommandHandler> COMMAND_HANDLERS;

   static std::pair<bool, FollowerMode> matchFollowerMode(const std::string& value);
   static std::pair<bool, const CommandHandler*> matchControlCommand(const std::string& key);

   // Helper functions
   static bool toBool(const std::string& value);
   static std::vector<std::string> parseInputLine(const std::string& line);
   static G6 parseG6(const std::vector<std::string>& tokens);
   static S6 parseS6(const std::vector<std::string>& tokens);
   static G6 parseV7(const std::vector<std::string>& tokens);
   static G6 parseRandom(const std::vector<std::string>& tokens);
   static G6 parseLattice(const std::vector<std::string>& tokens);

public:
   static void readMixedInput(ControlVariables& cv, std::vector<LatticeCell>& inputVectors, std::istream& input);
   static void handleControlVariable(ControlVariables& cv, const std::string& key, const std::string& value);
};

#endif
