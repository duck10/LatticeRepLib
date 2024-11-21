#include "InputHandler.h"
#include "Polar.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

std::string toUpper(std::string s) {
   std::transform(s.begin(), s.end(), s.begin(),
      [](unsigned char c) { return std::toupper(c); });
   return s;
}

bool isEndOfInput(const std::string& line) {
   std::string upperLine = toUpper(line);
   return upperLine == "END" || upperLine == "DONE" || upperLine == "FINISH" || upperLine == "STOP";
}

void InputHandler::readMixedInput(ControlVariables& cv, std::vector<G6>& inputVectors, std::istream& input) {
   std::string line;
   while (std::getline(input, line)) {
      // Trim leading and trailing whitespace
      line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
      line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

      if (line.empty()) continue;  // Skip empty lines
      if (isEndOfInput(line)) break;  // Stop processing on end markers

      std::vector<std::string> tokens = parseInputLine(line);
      if (tokens.empty()) continue;

      std::string key = toUpper(tokens[0]);
      if (key == "G6" || key == "S6" || key == "V7" ||
         key == "A" || key == "B" || key == "C" || key == "I" ||
         key == "R" || key == "P" || key == "F" || key == "H" ||
         key == "RANDOM") {
         try {
            if (key == "G6") inputVectors.push_back(parseG6(tokens));
            else if (key == "S6") inputVectors.push_back(G6(parseS6(tokens)));
            else if (key == "V7") inputVectors.push_back(parseV7(tokens));
            else if (key == "RANDOM") inputVectors.push_back(parseRandom(tokens));
            else inputVectors.push_back(parseLattice(tokens));
         }
         catch (const std::exception& e) {
            std::cerr << "Warning: Invalid input vector ignored - " << e.what() << std::endl;
         }
      }
      else {
         // Assume it's a control variable
         if (tokens.size() >= 2) {
            handleControlVariable(cv, key, tokens[1]);
         }
         else {
            std::cerr << "Warning: Invalid control variable format: " << line << std::endl;
         }
      }
   }
}

void InputHandler::handleControlVariable(ControlVariables& cv, const std::string& key, const std::string& value) {
   if (key == "FOLLOWERMODE") {
      std::string modeStr = toUpper(value);
      if (modeStr == "POINT") cv.followerMode = FollowerMode::POINT;
      else if (modeStr == "LINE") cv.followerMode = FollowerMode::LINE;
      else if (modeStr == "CHORD") cv.followerMode = FollowerMode::CHORD;
      else if (modeStr == "CHORD3") cv.followerMode = FollowerMode::CHORD3;
      else if (modeStr == "TRIANGLE") cv.followerMode = FollowerMode::TRIANGLE;
      else std::cerr << "Warning: Unknown follower mode '" << value << "'" << std::endl;
   }
   else if (key == "PERTURBATIONS") cv.perturbations = std::stoi(value);
   else if (key == "PERTURBBY") cv.perturbBy = std::stod(value);
   else if (key == "GLITCHLEVEL") cv.glitchLevel = std::stod(value);
   else if (key == "NUMFOLLOWERPOINTS") cv.numFollowerPoints = std::stoi(value);
   else if (key == "PRINTDISTANCEDATA") cv.printDistanceData = (toUpper(value) == "TRUE" || value == "1");
   else if (key == "GLITCHESONLY") cv.glitchesOnly = (toUpper(value) == "TRUE" || value == "1");
   else if (key == "FIXRANDOMSEED") cv.fixRandomSeed = (toUpper(value) == "TRUE" || value == "1");
   else if (key == "RANDOMSEED") cv.randomSeed = std::stoi(value);
   else if (key == "TIMESTAMP") cv.timestamp = (toUpper(value) == "TRUE" || value == "1");
   else if (key == "FILEPREFIX") cv.filePrefix = value;
   else if (key == "SHOWDATAPOINTS") cv.showDataPoints = (toUpper(value) == "TRUE" || value == "1");
   else if (key == "ENABLE") cv.setDistanceTypes(value, false);  // Add this line
   else std::cerr << "Warning: Unknown control variable '" << key << "'" << std::endl;
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
   if (tokens.size() != 7) {
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
   if (tokens.size() != 7) {
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
   if (tokens.size() != 8) {
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
   if (tokens.size() != 7) {
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