#ifndef FOLLOWERPOINTUTILS_H
#define FOLLOWERPOINTUTILS_H

#include <iostream>
#include <string>


struct FollowerPointsUtils {
   static constexpr int MIN_POINTS = 10;
   static constexpr int MAX_POINTS = 1000;
   static constexpr int DEFAULT_POINTS = 100;

   static int validatePointCount(const std::string& value) {
      try {
         int count = std::stoi(value);
         if (count < MIN_POINTS || count > MAX_POINTS) {
            std::cerr << ";Warning: Points out of range [" << MIN_POINTS
               << "," << MAX_POINTS << "], using " << DEFAULT_POINTS << std::endl;
            return DEFAULT_POINTS;
         }
         return count;
      }
      catch (...) {
         std::cerr << ";Warning: Invalid point count, using " << DEFAULT_POINTS << std::endl;
         return DEFAULT_POINTS;
      }
   }
};

#endif  // FOLLOWERPOINTUTILS_H

