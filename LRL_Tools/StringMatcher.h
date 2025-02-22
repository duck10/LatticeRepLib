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

   double getThreshold() const { return matchThreshold; }
};

struct MatchableString {
   std::string name;
   static inline StringMatcher matcher{ 0.2 };  // Default threshold

   MatchableString(const std::string& n) : name(n) {}
   MatchableString() : name("") {}

   MatchableString operator-(const MatchableString& other) const {
      MatchableString result;
      result.name = name + "\n" + other.name;
      return result;
   }

   double norm() const {
      size_t delim = name.find('\n');
      if (delim != std::string::npos) {
         std::string s1 = name.substr(0, delim);
         std::string s2 = name.substr(delim + 1);
         const double theta = matcher.getTheta(s1, s2);
         return theta;
      }
      return 0.0;
   }
};

#endif // STRING_MATCHER_H

