#include "BaseControlVariables.h"
#include "InputHandler.h"
#include "LatticeCell.h"
#include "Polar.h"
#include "LRL_Cell.h"

#include <iostream>

std::vector<std::pair<InputHandler::CommandPredicate, InputHandler::CommandHandler>> InputHandler::handlers;

void InputHandler::registerHandler(CommandPredicate predicate, CommandHandler handler) {
   handlers.emplace_back(predicate, handler);
}

//void InputHandler::registerHandler(const std::string& command, double threshold, CommandHandler handler) {
//   StringMatcher matcher(threshold);
//   registerHandler(
//      [command, matcher](const std::string& cmd) {
//         return matcher.matches(cmd, command);
//      },
//      handler
//   );
//}

void InputHandler::clearHandlers() {
   handlers.clear();
}

bool InputHandler::handleCommand(BaseControlVariables& controls,
   const std::string& command,
   const std::string& value) {
   //std::cerr << ";Debug: Handling command: " << command << std::endl;
   for (const auto& [predicate, handler] : handlers) {
      if (predicate(command)) {
         handler(controls, value);
         return true;
      }
   }
   std::cerr << ";Warning: Unrecognized command '" << command << "'" << std::endl;
   return false;
}

std::vector<std::string> InputHandler::parseInputLine(const std::string& line) {
   std::vector<std::string> tokens;
   std::istringstream iss(line);
   std::string token;
   while (iss >> token) {
      tokens.push_back(token);
   }
   return tokens;
}

G6 InputHandler::parseG6(const std::vector<std::string>& tokens) {
   if (tokens.size() < 7) {
      throw std::runtime_error("Invalid G6 input: expected 7 tokens, got " + std::to_string(tokens.size()));
   }
   G6 result;
   for (int i = 0; i < 6; ++i) {
      try {
         result[i] = std::stod(tokens[i + 1]);
      }
      catch (const std::exception) {
         throw std::runtime_error("Invalid G6 input: failed to parse " + tokens[i + 1] + " as double");
      }
   }
   return result;
}

S6 InputHandler::parseS6(const std::vector<std::string>& tokens) {
   if (tokens.size() < 7) {
      throw std::runtime_error("Invalid S6 input: expected 7 tokens, got " + std::to_string(tokens.size()));
   }
   S6 result;
   for (int i = 0; i < 6; ++i) {
      try {
         result[i] = std::stod(tokens[i + 1]);
      }
      catch (const std::exception) {
         throw std::runtime_error("Invalid S6 input: failed to parse " + tokens[i + 1] + " as double");
      }
   }
   return result;
}

G6 InputHandler::parseRandom(const std::vector<std::string>& tokens) {
   // Don't check token count for random
   return G6(Polar::rand());
}

G6 InputHandler::parseLattice(const std::vector<std::string>& tokens) {
   if (tokens.size() < 7) {
      throw std::runtime_error("Incomplete lattice input: expected 7 tokens, got " + std::to_string(tokens.size()));
   }

   const LRL_Cell cell(
      tokens[1] + " " +
      tokens[2] + " " +
      tokens[3] + " " +
      tokens[4] + " " +
      tokens[5] + " " +
      tokens[6]);
   return cell;
}

void InputHandler::readMixedInput(BaseControlVariables& controls,
   std::vector<LatticeCell>& cells,
   std::istream& input) {
   std::string line;
   while (std::getline(input, line)) {
      if (controls.getEcho() || LRL_StringTools::strToupper(line)=="ECHO") {
         std::cout << line << std::endl;
      }
      if (line.empty() || line[0] == ';') continue;
      std::string rawline(line);
      std::transform(line.begin(), line.end(), line.begin(), ::toupper);
      const bool isEnd = (line.find("END") != std::string::npos);
      if (isEnd) break;

      try {
         // Parse line into tokens
         std::vector<std::string> tokens = parseInputLine(line);
         std::vector<std::string> rawTokens = parseInputLine(rawline);
         if (tokens.empty()) continue;

         // Try to handle as lattice input first
         std::string upperKey = tokens[0];  // Make a copy
         std::transform(upperKey.begin(), upperKey.end(), upperKey.begin(), ::toupper);

         if (upperKey == "RANDOM" || upperKey == "R" ||  // Add "R"
            upperKey == "G6" || upperKey == "G" || upperKey == "S6" || upperKey == "S" ||
            upperKey == "P" || upperKey == "A" || upperKey == "B" || upperKey == "C" ||
            upperKey == "F" || upperKey == "I" || upperKey == "H") {
            handleLatticeInput(cells, upperKey, tokens, rawline);  // Use original key for handling
            continue;
         }

         // If not a lattice command, try as a control command
         std::string rest;
         for (size_t i = 1; i < tokens.size(); ++i) {
            rest += rawTokens[i];
            if (i < rawTokens.size() - 1) rest += " ";
         }

         if (!handleCommand(controls,rawTokens[0], rest)) {
            std::cerr << ";Warning: Unrecognized command '" << rawTokens[0] << "'" << std::endl;
         }
      }
      catch (const std::exception& e) {
         std::cerr << ";Warning: Failed to parse input: " << e.what() << std::endl;
      }
   }
}

G6 InputHandler::parseRandom() {
   return G6(Polar::rand());
}

std::vector<LatticeCell> InputHandler::parseRandom(size_t count) {
   std::vector<LatticeCell> results;
   results.reserve(count);

   for (size_t i = 0; i < count; ++i) {
      G6 result = parseRandom();  // Use the parameterless version
      const LRL_Cell cell = result;
      if (cell.IsValid()) {
         std::string numberedLine = "RANDOM #" + std::to_string(i + 1);
         results.emplace_back(result, "P", numberedLine);
      }
      else {
         std::cerr << ";Warning: Invalid random vector #" << (i + 1) << " ignored" << std::endl;
      }
   }
   return results;
}

void InputHandler::handleSingleLattice(
   std::vector<LatticeCell>& inputVectors,
   const std::string& key,
   const std::vector<std::string>& tokens,
   const std::string& line) {
   G6 result;
   std::string latticeType = "P";  // Default to primitive

   if (key == "G6" || key == "G") result = parseG6(tokens);
   else if (key == "S6" || key == "S") result = parseS6(tokens);
   else if (key == "RANDOM") result = parseRandom();  // Using parameterless version
   else {
      result = parseLattice(tokens);
      latticeType = key;  // Use the key as lattice type for A,B,C,F,I,H
   }

   const LRL_Cell cell = result;
   if (cell.IsValid()) {
      inputVectors.emplace_back(result, latticeType, line);
   }
   else {
      std::cerr << ";Warning: Invalid input vector ignored" << std::endl;
   }
}

void InputHandler::handleLatticeInput(
   std::vector<LatticeCell>& inputVectors,
   const std::string& key,
   const std::vector<std::string>& tokens,
   const std::string& line) {
   try {
      if (key == "RANDOM" && tokens.size() > 1) {
         try {
            const size_t count = std::stoul(tokens[1]);
            if (count > 1) {
               // Use the new vector-returning parseRandom
               std::vector<LatticeCell> randomResults = parseRandom(count);
               inputVectors.insert(
                  inputVectors.end(),
                  randomResults.begin(),
                  randomResults.end()
               );
               return;
            }
         }
         catch (const std::exception&) {
            // Fall through to single lattice case if count parsing fails
         }
      }

      // Handle single lattice case
      handleSingleLattice(inputVectors, key, tokens, line);
   }
   catch (const std::exception& e) {
      std::cerr << ";Warning: Invalid input vector ignored - " << e.what() << std::endl;
   }
}
