// Modified InputHandler.h - move lattice handling methods to public

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

   // Keep only the parsing helpers private
   static std::vector<std::string> reorderLatticeTokens(const std::vector<std::string>& tokens);

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

   // NEWLY PUBLIC: Make these lattice handling methods accessible
   static bool isLattice(const std::string& s);
   static G6 parseG6(const std::vector<std::string>& tokens);
   static S6 parseS6(const std::vector<std::string>& tokens);
   static P3 parseP3(const std::vector<std::string>& tokens);
   static G6 parseLattice(const std::vector<std::string>& tokens);
   static G6 parseRandom();
   static std::vector<LatticeCell> parseRandom(size_t count);

   static void handleSingleLattice(
      std::vector<LatticeCell>& inputVectors,
      const std::string& key,
      const std::vector<std::string>& tokens,
      const std::string& line);

   static void handleLatticeInput(std::vector<LatticeCell>& inputVectors,
      const std::string& key,
      const std::vector<std::string>& tokens,
      const std::string& line);

   // NEW: Direct lattice processing method that returns the parsed cell
   static LatticeCell processSingleLatticeInput(const std::string& line);

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

   // NEW: Command-based overload that processes everything through registered handlers
   template<typename T>
   static std::vector<LatticeCell> handleInput(T& controls, bool useCommandSystem) {
      if (!useCommandSystem) {
         return handleInput(controls);  // Use existing method
      }

      std::vector<LatticeCell> inputVectors;  // Not used in command mode, but returned for compatibility
      std::cout << "; Enter control variables and input vectors (type 'end' to finish):\n";

      std::string line;
      while (std::getline(std::cin, line)) {
         if (line.empty() || line[0] == ';') continue;

         // Apply input cleaning
         for (char& c : line) {
            if (!std::isprint(static_cast<unsigned char>(c))) {
               c = ' ';
            }
         }

         globalInputLines.emplace_back(line);

         // Check for end condition
         std::string upperLine = LRL_StringTools::strToupper(line);
         if (upperLine == "END") break;

         // Parse the input line
         std::vector<std::string> tokens = parseInputLine(line);
         if (tokens.empty()) continue;

         // Handle echo
         if (controls.getEcho() || LRL_StringTools::strToupper(tokens[0]) == "ECHO") {
            std::cout << line << std::endl;
         }

         // Extract command and parameters
         std::string command = tokens[0];
         std::string parameters;
         for (size_t i = 1; i < tokens.size(); ++i) {
            parameters += tokens[i];
            if (i < tokens.size() - 1) parameters += " ";
         }

         // Process through command system
         if (!handleCommand(controls, command, parameters)) {
            std::cout << "Unknown command: " << command << "\n";
            std::cout << "Type 'help' for available commands.\n";
         }
      }

      return inputVectors;
   }

   static std::vector<std::string> GetInputLines() {
      return InputHandler::globalInputLines;
   }
};

#endif // INPUT_HANDLER_H
