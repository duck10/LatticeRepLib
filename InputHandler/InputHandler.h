#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <functional>
#include <string>
#include <vector>
#include <memory>

#include "LatticeCell.h"
#include "StringMatcher.h"
#include "WebIO.h"

class BaseControlVariables;  // Forward declaration

class InputHandler {
public:
   using CommandPredicate = std::function<bool(const std::string&)>;
   using CommandHandler = std::function<void(BaseControlVariables&, const std::string&)>;

   static void registerHandler(CommandPredicate predicate, CommandHandler handler);

   static void registerHandler(const std::string& command, double threshold, CommandHandler handler) {
      StringMatcher matcher(threshold);
      //std::cerr << ";Debug: Registering handler for command: " << command << std::endl;
      registerHandler(
         [command, matcher](const std::string& cmd) {
            bool matches = matcher.matches(cmd, command);
            //std::cerr << ";Debug: Matching '" << cmd << "' against '" << command
            //   << "': " << (matches ? "true" : "false") << std::endl;
            return matches;
         },
         handler
      );
   }

   static void clearHandlers();

   static bool handleCommand(BaseControlVariables& controls,
      const std::string& command,
      const std::string& value);

   static void readMixedInput(BaseControlVariables& controls,
      std::vector<LatticeCell>& cells,
      std::istream& input);

   template<typename T>
   static std::vector<LatticeCell> handleInput(T& controls, WebIO& webio) {
      std::vector<LatticeCell> inputVectors;

      std::cout << "; Enter control variables and input vectors (type 'end' to finish):\n";
      InputHandler::readMixedInput(controls, inputVectors, std::cin);

      if (webio.m_hasWebInstructions) {
         webio.SetTimeStamp(true);
      }

      return inputVectors;
   }

   template<typename T>
   static std::vector<LatticeCell> handleInput(T& controls) {
      std::vector<LatticeCell> inputVectors;

      std::cout << "; Enter control variables and input vectors (type 'end' to finish):\n";
      InputHandler::readMixedInput(controls, inputVectors, std::cin);
      return inputVectors;
   }


   static std::vector<std::string> parseInputLine(const std::string& line);
   static void handleLatticeInput(std::vector<LatticeCell>& inputVectors,
      const std::string& key,
      const std::vector<std::string>& tokens,
      const std::string& line);
   static G6 parseG6(const std::vector<std::string>& tokens);
   static S6 parseS6(const std::vector<std::string>& tokens);
   static G6 parseRandom(const std::vector<std::string>& tokens);
   static G6 parseLattice(const std::vector<std::string>& tokens);

private:
   static std::vector<std::pair<CommandPredicate, CommandHandler>> handlers;
   static G6 parseRandom();  // New version without parameters
   static std::vector<LatticeCell> parseRandom(size_t count);  // New version returning vector

   static void handleSingleLattice(
      std::vector<LatticeCell>& inputVectors,
      const std::string& key,
      const std::vector<std::string>& tokens,
      const std::string& line);
};

#endif // INPUT_HANDLER_H