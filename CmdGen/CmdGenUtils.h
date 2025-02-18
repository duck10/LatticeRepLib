#ifndef CMDGENUTILS_H
#define CMDGENUTILS_H

#include <iostream>
#include <set>

struct CmdGenUtils {
   static constexpr int MIN_POINTS = 1;
   static constexpr int MAX_POINTS = 1000;
   static constexpr int DEFAULT_POINTS = 5;

   static int validatePointCount(int count) {
      if (count < MIN_POINTS || count > MAX_POINTS) {
         std::cout << ";Warning: Point count to generate " << count << " out of range ["
            << MIN_POINTS << "," << MAX_POINTS << "], using "
            << DEFAULT_POINTS << std::endl;
         return DEFAULT_POINTS;
      }
      return count;
   }

   static std::string validateTypes(const std::string& text) {
      if (splitTokens(text).size() < 2) {

      }
      else
      {

      }


      return text;
   }

   static std::set<std::string> splitTokens(const std::string& text) {
      std::istringstream iss(text);
      std::string token;
      std::set<std::string> out;
      while (iss >> token) {
         out.insert(token);
      }
      return out;
   }
};


#endif  // CMDGENUTILS_H