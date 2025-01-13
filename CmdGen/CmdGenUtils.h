#ifndef CMDGENUTILS_H
#define CMDGENUTILS_H

#include <iostream>

struct CmdGenUtils {
   static constexpr int MIN_POINTS = 1;
   static constexpr int MAX_POINTS = 1000;
   static constexpr int DEFAULT_POINTS = 5;

   static int validatePointCount(int count) {
      if (count < MIN_POINTS || count > MAX_POINTS) {
         std::cerr << ";Warning: Point count to generate " << count << " out of range ["
            << MIN_POINTS << "," << MAX_POINTS << "], using "
            << DEFAULT_POINTS << std::endl;
         return DEFAULT_POINTS;
      }
      return count;
   }
};


#endif  // CMDGENUTILS_H