// StringMatcher.h
#ifndef STRING_MATCHER_H
#define STRING_MATCHER_H

#include <string>
#include <algorithm>
#include "Theta.h"

class StringMatcher {
private:
   ThetaMatch<std::string> matcher;
   const double matchThreshold;

   static std::string toUpper(std::string s) {
      std::transform(s.begin(), s.end(), s.begin(),
         [](unsigned char c) { return std::toupper(c); });
      return s;
   }

public:
   explicit StringMatcher(double threshold = 0.2) : matchThreshold(threshold) {}

   bool matches(const std::string& input, const std::string& command) const {
      return matcher(toUpper(input), toUpper(command)) <= matchThreshold;
   }

   double getTheta(const std::string& input, const std::string& command) const {
      return matcher(toUpper(input), toUpper(command));
   }
};

#endif // STRING_MATCHER_H

