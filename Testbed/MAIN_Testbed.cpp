// test_string_matcher.cpp
#include "StringMatcher.h"
#include <iostream>

int main() {
   StringMatcher matcher;
   std::string cmd = "FOLLOWERMODE";
   std::string tests[] = { "FOLLOWERMODE","FOLLOWER_MODE","FOLLOWER__MODE","followermode", "follower_mode", "folowermode", "something" };

   for (const auto& test : tests) {
      double theta = matcher.getTheta(test, cmd);  // Need to add this method
      std::cout << test << " vs " << cmd << ": theta=" << theta
         << " match=" << (theta <= 0.2) << "\n";
   }
   return 0;
}