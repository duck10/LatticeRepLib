// InputHandler.h
#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "BaseControlVariables.h"
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
   struct CommandHandler {
      std::string command;
      double threshold;
      std::function<void(BaseControlVariables&, const std::string&)> handler;

      CommandHandler(const std::string& cmd, double t,
         std::function<void(BaseControlVariables&, const std::string&)> h)
         : command(cmd), threshold(t), handler(h) {}
   };

   static std::vector<CommandHandler> commandHandlers;  // Now non-const
   static std::pair<bool, const CommandHandler*> matchControlCommand(const std::string& key);

   // Helper functions
   static bool toBool(const std::string& value);
   static std::string toUpper(std::string s);
   static bool isEndOfInput(const std::string& line);
   static std::vector<std::string> parseInputLine(const std::string& line);
   static G6 parseG6(const std::vector<std::string>& tokens);
   static S6 parseS6(const std::vector<std::string>& tokens);
   static G6 parseRandom(const std::vector<std::string>& tokens);
   static G6 parseLattice(const std::vector<std::string>& tokens);

public:
   // Registration methods
   static void registerHandler(const std::string& command,
      double threshold,
      std::function<void(BaseControlVariables&, const std::string&)> handler);
   static void clearHandlers();

   template<typename T>
   static void readMixedInput(T& controls, std::vector<LatticeCell>& inputVectors, std::istream& input) {
      static_assert(std::is_base_of<BaseControlVariables, T>::value,
         "Control variables type must be derived from BaseControlVariables");

      std::string line;
      while (std::getline(input, line)) {
         if (!line.empty() && line[0] == ';') continue;
         // Trim leading and trailing whitespace
         line.erase(0, line.find_first_not_of(" \t\n\r\f\v,"));
         line.erase(line.find_last_not_of(" \t\n\r\f\v,") + 1);

         if (line.empty()) continue;  // Skip empty lines
         if (isEndOfInput(line)) break;  // Stop processing on end markers

         std::vector<std::string> tokens = parseInputLine(line);
         if (tokens.empty()) continue;

         std::string key = toUpper(tokens[0]);
         handleInput(controls, inputVectors, key, tokens, line);
      }
   }

   static void handleControlVariable(BaseControlVariables& controls,
      const std::string& key,
      const std::string& value);

private:
   template<typename T>
   static void handleInput(T& controls,
      std::vector<LatticeCell>& inputVectors,
      const std::string& key,
      const std::vector<std::string>& tokens,
      const std::string& line) {
      if (key == "G6" || key == "S6" ||
         key == "G" || key == "S" ||
         key == "A" || key == "B" || key == "C" || key == "I" ||
         key == "R" || key == "P" || key == "F" || key == "H" ||
         key == "RANDOM") {
         handleLatticeInput(inputVectors, key, tokens, line);
      }
      else if (tokens.size() >= 2) {
         if (key == "TYPE" && tokens.size() > 2) {
            // For TYPE, collect all remaining tokens
            std::string allTypes;
            for (size_t i = 1; i < tokens.size(); ++i) {
               if (i > 1) allTypes += " ";
               allTypes += tokens[i];
            }
            handleControlVariable(controls, key, allTypes);
         }
         else {
            // For other controls, just use the second token
            handleControlVariable(controls, key, tokens[1]);
         }
      }
      else {
         std::cerr << ";Warning: Invalid control variable format: " << line << std::endl;
      }
   }

   static void handleLatticeInput(std::vector<LatticeCell>& inputVectors,
      const std::string& key,
      const std::vector<std::string>& tokens,
      const std::string& line);
};

#endif // INPUT_HANDLER_H