#include "InputHandler.h"
#include "BaseControlVariables.h"
#include "LRL_StringTools.h"
#include "CommandSystem.h"

// Initialize static member
CommandSystem InputHandler::commandSystem;
std::vector<std::string> InputHandler::globalInputLines;


bool InputHandler::isLattice(const std::string& s) {
   const std::string upperKey = LRL_StringTools::strToupper(s);
   return (upperKey == "RANDOM" || upperKey == "R" ||
      upperKey == "G6" || upperKey == "G" || upperKey == "S6" || upperKey == "S" ||
      upperKey == "P" || upperKey == "A" || upperKey == "B" || upperKey == "C" ||
      upperKey == "F" || upperKey == "I" || upperKey == "H" || upperKey == "U" ||
      upperKey == "DC7U");
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
      catch (const std::exception&) {
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
      catch (const std::exception&) {
         throw std::runtime_error("Invalid S6 input: failed to parse " + tokens[i + 1] + " as double");
      }
   }
   return result;
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

G6 InputHandler::parseRandom() {
   Polar resultP = Polar::rand();
   G6 resultG(resultP);
   while (!(resultG.IsValid() && LRL_Cell(resultG).IsValid())) {
      resultP = Polar::rand();
      resultG = resultP;
   }
   return G6(Polar::rand());
}

std::vector<LatticeCell> InputHandler::parseRandom(size_t count) {
   std::vector<LatticeCell> results;
   results.reserve(count);

   for (size_t i = 0; i < count; ++i) {
      G6 random = parseRandom();
      LRL_Cell cell = random;
      int count = 0;
      while (!cell.IsValid()) {
         random = parseRandom();
         cell = random;
         ++count;
      }
      const std::string numberedLine = "RANDOM #" + std::to_string(i + 1);
      results.emplace_back(random, "P", numberedLine);
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
   else if (key == "RANDOM" || key == "R") result = parseRandom();
   //else if (key == "U") result = parseDC7u(tokens);
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

void InputHandler::registerHandler(const std::string& command, double threshold, CommandHandler handler) {
   commandSystem.registerCommand(command, handler);
}

void InputHandler::clearHandlers() {
   commandSystem.clear();
}

bool InputHandler::handleCommand(BaseControlVariables& controls,
   const std::string& command,
   const std::string& value) {
   return commandSystem.executeCommand(controls, command, value);
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

void InputHandler::readMixedInput(BaseControlVariables& controls,
   std::vector<LatticeCell>& cells,
   std::istream& input) {
   std::string line;
   while (std::getline(input, line)) {

      if (*(line.end() - 1) == '\0') {
         line.pop_back();
      }

      for (char& c : line) {
         if (!std::isprint(static_cast<unsigned char>(c))) {
            c = ' '; // Replace non-printable characters
         }
      }

      globalInputLines.emplace_back(line);
      if (line.empty() || line[0] == ';') continue;
      std::string rawline(line);
      const std::vector<std::string> tokens = parseInputLine(line);
      if (tokens.empty()) continue;

      if (controls.getEcho() || LRL_StringTools::strToupper(tokens[0]) == "ECHO") {
         std::cout << line << std::endl;
      }
      if (LRL_StringTools::strToupper(tokens[0]) == "END") {
         break;
      }

      try {
         // Try to handle as lattice input first
         std::string upperKey = LRL_StringTools::strToupper(tokens[0]);
         if (isLattice(upperKey)) {
            handleLatticeInput(cells, upperKey, tokens, rawline);
            continue;
         }

         // If not a lattice command, try as a control command
         std::string rest;
         const std::vector<std::string> rawTokens = parseInputLine(rawline);
         for (size_t i = 1; i < tokens.size(); ++i) {
            rest += rawTokens[i];
            if (i < rawTokens.size() - 1) rest += " ";
         }

         if (!handleCommand(controls, rawTokens[0], rest)) {
            std::cerr << ";Warning: Unrecognized command '" << rawTokens[0] << "'" << std::endl;
         }
      }
      catch (const std::exception& e) {
         std::cerr << ";Warning: Failed to parse input: " << e.what() << std::endl;
      }
   }
}

void InputHandler::handleLatticeInput(std::vector<LatticeCell>& inputVectors,
   const std::string& key,
   const std::vector<std::string>& tokens,
   const std::string& line) {
   try {
      if (key == "RANDOM" && tokens.size() > 1) {
         try {
            const size_t count = std::stoul(tokens[1]);
            if (count > 1) {
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
