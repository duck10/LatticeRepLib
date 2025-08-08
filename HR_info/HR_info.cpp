// this program grew from KIMI.ai's translation of the Wikipedia page for
// Hexagonal crystal family
// https://en.wikipedia.org/wiki/Hexagonal_crystal_family
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>

#include "R_Settings.h"
#include "Processing.h"
#include "ThetaInsensitive.h"
#include "HR_InfoControls.h"
#include "ProgramSetup.h"

// ------------------------------------------------------------------
//  Centralised input loop
// ------------------------------------------------------------------
int main()
{
   HR_InforControls controls;

   // Show program info
   std::cout << "Navigating the Hexagonal crystal family\n\n";
   std::cout << "This program was partly written by the AI Kimi.\n"
      "Kimi used the Wikipedia page for Hexagonal crystal family\n"
      "for the source data.\n"
      "Some of the logic, especially for controls was written\n"
      "with the aid of Claude.ai";
   std::cout << "One should note that both of these AI's contributed\n"
      "a substantial number of coding errors\n\n";


   std::cout << controls << std::endl;  // Show available commands

   // Use command-based input processing (true = use command system)
   InputHandler::handleInput(controls, true);

   return 0;
}
