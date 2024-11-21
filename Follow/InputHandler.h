#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "ControlVariables.h"
#include "G6.h"
#include "S6.h"
#include <vector>
#include <string>
#include <istream>

class InputHandler {
public:
   static void readMixedInput(ControlVariables& cv, std::vector<G6>& inputVectors, std::istream& input);

private:
   static std::vector<std::string> parseInputLine(const std::string& line);
   static G6 parseG6(const std::vector<std::string>& tokens);
   static S6 parseS6(const std::vector<std::string>& tokens);
   static G6 parseV7(const std::vector<std::string>& tokens);
   static G6 parseRandom(const std::vector<std::string>& tokens);
   static G6 parseLattice(const std::vector<std::string>& tokens);
   static void handleControlVariable(ControlVariables& cv, const std::string& key, const std::string& value);
};

#endif // INPUT_HANDLER_H