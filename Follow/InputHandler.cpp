#include "InputHandler.h"
#include "Polar.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

// Helper function to convert string to upper case
std::string toUpper(std::string s) {
   std::transform(s.begin(), s.end(), s.begin(),
      [](unsigned char c) { return std::toupper(c); });
   return s;
}

// Helper function to check for end of input
bool isEndOfInput(const std::string& line) {
   std::string upperLine = toUpper(line);
   return upperLine == "END" || upperLine == "DONE" || upperLine == "FINISH" || upperLine == "STOP";
}

// Define the follower modes table
const std::vector<InputHandler::FollowerModeMatch> InputHandler::FOLLOWER_MODES = {
    {"CHORD3", FollowerMode::CHORD3, 0.15},
    {"CHORD", FollowerMode::CHORD, 0.15},
    {"POINT", FollowerMode::POINT, 0.15},
    {"LINE", FollowerMode::LINE, 0.15},
    {"TRIANGLE", FollowerMode::TRIANGLE, 0.15}
};

// Define the command handlers table
const std::vector<InputHandler::CommandHandler> InputHandler::COMMAND_HANDLERS = {
    {"FOLLOWERMODE", 0.35, [](ControlVariables& cv, const std::string& value) {
        auto [found, mode] = matchFollowerMode(value);
        if (found) {
            cv.followerMode = mode;
        }
 else {
  std::cerr << ";Warning: Unknown follower mode '" << value << "'" << std::endl;
}
}},
{"PERTURBATIONS", 0.35, [](ControlVariables& cv, const std::string& value) {
    cv.perturbations = std::stoi(value);
}},
{"PERTURBBY", 0.35, [](ControlVariables& cv, const std::string& value) {
    cv.perturbBy = std::stod(value);
}},
{"GLITCHTHRESHOLD", 0.35, [](ControlVariables& cv, const std::string& value) {
    cv.glitchThresholdPercent = std::stod(value);
}},
{"NUMFOLLOWERPOINTS", 0.35, [](ControlVariables& cv, const std::string& value) {
    cv.numFollowerPoints = std::stoi(value);
}},
{"PRINTDISTANCEDATA", 0.35, [](ControlVariables& cv, const std::string& value) {
    cv.printDistanceData = toBool(value);
}},
{"GLITCHESONLY", 0.35, [](ControlVariables& cv, const std::string& value) {
    cv.glitchesOnly = toBool(value);
}},
{"FIXRANDOMSEED", 0.35, [](ControlVariables& cv, const std::string& value) {
    cv.fixRandomSeed = toBool(value);
}},
{"RANDOMSEED", 0.35, [](ControlVariables& cv, const std::string& value) {
    cv.randomSeed = std::stoi(value);
}},
{"TIMESTAMP", 0.35, [](ControlVariables& cv, const std::string& value) {
    cv.timestamp = toBool(value);
}},
{"FILEPREFIX", 0.35, [](ControlVariables& cv, const std::string& value) {
    cv.filePrefix = value;
}},
{"SHOWDATAMARKERS", 0.35, [](ControlVariables& cv, const std::string& value) {
    cv.showDataMarkers = toBool(value);
}},
{"ENABLE", 0.35, [](ControlVariables& cv, const std::string& value) {
    cv.setDistanceTypes(value, false);
}},
{"DISABLE", 0.35, [](ControlVariables& cv, const std::string& value) {
    cv.disableDistance(value);
}}
};

// Helper function implementations
bool InputHandler::toBool(const std::string& value) {
   std::string upper = toUpper(value);
   return upper == "TRUE" || value == "1";
}

std::pair<bool, FollowerMode> InputHandler::matchFollowerMode(const std::string& value) {
   const StringMatcher matcher(0.15);
   std::string upperValue = toUpper(value);

   double bestScore = 1.0;
   FollowerMode bestMatch = FollowerMode::POINT;

   for (const auto& mode : FOLLOWER_MODES) {
      double score = matcher.getTheta(upperValue, mode.mode);
      if (score < bestScore) {
         bestScore = score;
         bestMatch = mode.enumVal;
      }
   }

   return { true, bestMatch };
}

std::pair<bool, const InputHandler::CommandHandler*> InputHandler::matchControlCommand(const std::string& key) {
   const StringMatcher matcher(0.2);
   for (const auto& handler : COMMAND_HANDLERS) {
      if (matcher.matches(key, handler.command)) {
         return { true, &handler };
      }
   }
   return { false, nullptr };
}

void InputHandler::handleControlVariable(ControlVariables& cv, const std::string& key, const std::string& value) {
   auto [matched, handler] = matchControlCommand(key);
   if (!matched) {
      if (key[0] != ';') {  // ignore comment lines
         std::cerr << ";Warning: Unknown control variable '" << key << "'" << std::endl;
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

// Existing parsing functions remain the same
std::vector<std::string> InputHandler::parseInputLine(const std::string& line) {
   std::vector<std::string> tokens;
   std::istringstream iss(line);
   std::string token;
   while (iss >> token) {
      tokens.push_back(token);
   }
   return tokens;
}

void InputHandler::readMixedInput(ControlVariables& cv, std::vector<G6>& inputVectors, std::istream& input) {
   std::string line;
   while (std::getline(input, line)) {
      if (!line.empty() && line[0] == ';') continue;
      // Trim leading and trailing whitespace
      line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
      line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

      if (line.empty()) continue;  // Skip empty lines
      if (isEndOfInput(line)) break;  // Stop processing on end markers

      std::vector<std::string> tokens = parseInputLine(line);
      if (tokens.empty()) continue;

      std::string key = toUpper(tokens[0]);
      if (key == "G6" || key == "S6" ||/* key == "V7" disabled ||*/
         key == "A" || key == "B" || key == "C" || key == "I" ||
         key == "R" || key == "P" || key == "F" || key == "H" ||
         key == "RANDOM") {
         G6 result;
         try {
            if (key == "G6") result = parseG6(tokens);
            else if (key == "S6") result = parseS6(tokens);
            //else if (key == "V7") inputVectors.push_back(parseV7(tokens));
            else if (key == "RANDOM") result = parseRandom(tokens);
            else result = parseLattice(tokens);
            const LRL_Cell cell = result;
            if (cell.IsValid()) {
               inputVectors.emplace_back(result);
            }
            else {
               const int i19191 = 19191; // invalid input cells
            }
         }
         catch (const std::exception& e) {
            std::cerr << ";Warning: Invalid input vector ignored - " << e.what() << std::endl;
         }
      }
      else {
         // Assume it's a control variable
         if (tokens.size() >= 2) {
            handleControlVariable(cv, key, tokens[1]);
         }
         else {
            std::cerr << ";Warning: Invalid control variable format: " << line << std::endl;
         }
      }
   }
}

// Keep existing parsing functions
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

G6 InputHandler::parseV7(const std::vector<std::string>& tokens) {
   if (tokens.size() < 8) {
      throw std::runtime_error("Invalid V7 input: expected 8 tokens, got " + std::to_string(tokens.size()));
   }
   // Implement V7 to G6 conversion here
   // This is a placeholder implementation
   G6 result;
   for (int i = 0; i < 6; ++i) {
      try {
         result[i] = std::stod(tokens[i + 1]);
      }
      catch (const std::exception& e) {
         throw std::runtime_error("Invalid V7 input: failed to parse " + tokens[i + 1] + " as double");
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
   // Implement lattice to G6 conversion here
   // This is a placeholder implementation
   G6 result;
   for (int i = 0; i < 6; ++i) {
      try {
         result[i] = std::stod(tokens[i + 1]);
      }
      catch (const std::exception& e) {
         throw std::runtime_error("Invalid lattice input: failed to parse " + tokens[i + 1] + " as double");
      }
   }
   return result;
}
