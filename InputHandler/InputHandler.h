#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "CommandSystem.h"
#include "LatticeCell.h"
#include "WebIO.h"
#include "Polar.h"
#include "LRL_Cell.h"
#include <vector>
#include <memory>
#include <functional>
#include <sstream>


class BaseControlVariables;  // Forward declaration

class InputHandler {
private:
   static CommandSystem commandSystem;

   static bool isLattice(const std::string& s);
   static G6 parseG6(const std::vector<std::string>& tokens);
   static S6 parseS6(const std::vector<std::string>& tokens);
   static G6 parseLattice(const std::vector<std::string>& tokens);
   static G6 parseRandom();
   static std::vector<LatticeCell> parseRandom(size_t count);
   static void handleSingleLattice(
      std::vector<LatticeCell>& inputVectors,
      const std::string& key,
      const std::vector<std::string>& tokens,
      const std::string& line);

public:

   // Global container to store input lines
   static std::vector<std::string> globalInputLines;

   using CommandHandler = std::function<void(BaseControlVariables&, const std::string&)>;

   static void registerHandler(const std::string& command, double threshold, CommandHandler handler);
   static void clearHandlers();
   static bool handleCommand(BaseControlVariables& controls,
      const std::string& command,
      const std::string& value);

   static std::vector<std::string> parseInputLine(const std::string& line);
   static void readMixedInput(BaseControlVariables& controls,
      std::vector<LatticeCell>& cells,
      std::istream& input);
   static void handleLatticeInput(std::vector<LatticeCell>& inputVectors,
      const std::string& key,
      const std::vector<std::string>& tokens,
      const std::string& line);

   template<typename T>
   static std::vector<LatticeCell> handleInput(T& controls, WebIO& webio) {
      std::vector<LatticeCell> inputVectors;
      std::cout << "; Enter control variables and input vectors (type 'end' to finish):\n";
      readMixedInput(controls, inputVectors, std::cin);

      if (webio.m_hasWebInstructions) {
         webio.SetTimeStamp(true);
      }
      return inputVectors;
   }

   template<typename T>
   static std::vector<LatticeCell> handleInput(T& controls) {
      std::vector<LatticeCell> inputVectors;
      std::cout << "; Enter control variables and input vectors (type 'end' to finish):\n";
      readMixedInput(controls, inputVectors, std::cin);
      return inputVectors;
   }

   static std::vector<std::string> GetInputLines() {
      return InputHandler::globalInputLines;
   }

};

#endif // INPUT_HANDLER_H
