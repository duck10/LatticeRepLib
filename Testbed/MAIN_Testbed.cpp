// test_string_matcher.cpp
//#include "InputHandler.h"
//#include "FollowFeatures.h"

#include <iostream>

int main() {
   //FollowControlVariables::setupHandlers();

   //FollowControlVariables controls;
   //std::vector<LatticeCell> vectors;

   std::string input = R"(
PATHCOUNT 75
block start 22
blockxstart 27
blocksize
P 10 10 10 90 90 90
F 12 12 12 90 90 90
R 10 11 12 120 121 122
END
)";


   //std::istringstream inputStream(input);
   //InputHandler::readMixedInput(controls, vectors, inputStream);

   //std::cout << "Control settings:\n" << controls;
   //std::cout << "Number of lattice vectors: " << vectors.size() << "\n";
   //for (const auto& v : vectors) {
   //   std::cout << "\tinput text " << v.GetInput() << std::endl;
   //   std::cout << "\tinput cell " << v.getCell() << std::endl;
   //   std::cout << "\tinput lattice " << v.getLatticeType() << std::endl;
   //   std::cout << "\tinput Niggli reduced vector G6 " << v.getNiggliReducedCell() << std::endl << std::endl;
   //}



   //std::string cmd = "FOLLOWERMODE";
   //std::string tests[] = { "FOLLOWERMODE","FOLLOWER_MODE","FOLLOWER__MODE","followermode", "follower_mode", "folowermode", "something" };

   //for (const auto& test : tests) {
   //   double theta = matcher.getTheta(test, cmd);  // Need to add this method
   //   std::cout << test << " vs " << cmd << ": theta=" << theta
   //      << " match=" << (theta <= 0.2) << "\n";
   //}
   return 0;
}