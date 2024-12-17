#include "InputHandler.h"
#include "BlockLimits.h"
#include "ControlFeatures.h"
#include "Polar.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

// Initialize static member
std::vector<InputHandler::CommandHandler> InputHandler::commandHandlers;

// Static helper function implementations
std::string InputHandler::toUpper(std::string s) {
   std::transform(s.begin(), s.end(), s.begin(),
      [](unsigned char c) { return std::toupper(c); });
   return s;
}

bool InputHandler::isEndOfInput(const std::string& line) {
   std::string upperLine = toUpper(line);
   return upperLine == "END" || upperLine == "DONE" || upperLine == "FINISH" || upperLine == "STOP";
}

void InputHandler::registerHandler(const std::string& command,
   double threshold,
   std::function<void(BaseControlVariables&, const std::string&)> handler) {
   commandHandlers.emplace_back(command, threshold, handler);
}

void InputHandler::clearHandlers() {
   commandHandlers.clear();
}

void InputHandler::handleControlVariable(BaseControlVariables& cv, const std::string& key, const std::string& value) {
   auto [matched, handler] = matchControlCommand(key);
   if (!matched) {
      if (key[0] != ';') {  // ignore comment lines
         std::cerr << " ;Warning: Unknown control variable '" << key << "'" << std::endl;
      }
      return;
   }

   try {
      handler->handler(cv, value);
   }
   catch (const std::exception& e) {
      std::cerr << ";Warning: Error processing value for '" << key << "': " << e.what() << std::endl;
   }
}

std::pair<bool, const InputHandler::CommandHandler*> InputHandler::matchControlCommand(const std::string& key) {
   const StringMatcher matcher(0.2);
   for (const auto& handler : commandHandlers) {
      if (matcher.matches(key, handler.command)) {
         return { true, &handler };
      }
   }
   return { false, nullptr };
}

// Rest of the implementations stay the same...
// [parseG6, parseS6, parseRandom, parseLattice, etc.]

bool InputHandler::toBool(const std::string& value) {
   std::string upper = toUpper(value);
   return upper == "TRUE" || value == "1";
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

// Keep all the parsing implementations as they were before...
G6 InputHandler::parseG6(const std::vector<std::string>& tokens) {
   if (tokens.size() < 7) {
      throw std::runtime_error("Invalid G6 input: expected 7 tokens, got " + std::to_string(tokens.size()));
   }
   G6 result;
   for (int i = 0; i < 6; ++i) {
      try {
         result[i] = std::stod(tokens[i + 1]);
      }
      catch (const std::exception& e) {
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
      catch (const std::exception& e) {
         throw std::runtime_error("Invalid S6 input: failed to parse " + tokens[i + 1] + " as double");
      }
   }
   return result;
}

G6 InputHandler::parseRandom(const std::vector<std::string>& tokens) {
   return G6(Polar::rand());
}

G6 InputHandler::parseLattice(const std::vector<std::string>& tokens) {
   if (tokens.size() < 7) {
      throw std::runtime_error("Invalid lattice input: expected 7 tokens, got " + std::to_string(tokens.size()));
   }
   LRL_Cell cell(
      tokens[1] + " " +
      tokens[2] + " " +
      tokens[3] + " " +
      tokens[4] + " " +
      tokens[5] + " " +
      tokens[6]);
   return cell;
}

void InputHandler::handleLatticeInput(std::vector<LatticeCell>& inputVectors,
   const std::string& key,
   const std::vector<std::string>& tokens,
   const std::string& line) {
   try {
      G6 result;
      std::string latticeType = "P";  // Default to primitive

      if (key == "G6" || key == "G") result = parseG6(tokens);
      else if (key == "S6" || key == "S") result = parseS6(tokens);
      else if (key == "RANDOM") result = parseRandom(tokens);
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
   catch (const std::exception& e) {
      std::cerr << ";Warning: Invalid input vector ignored - " << e.what() << std::endl;
   }
}