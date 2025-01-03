#ifndef FOLLOWERPOINTSUTILS_H
#define FOLLOWERPOINTSUTILS_H

struct FollowerPointsUtils {
   static constexpr int MIN_POINTS = 5;
   static constexpr int MAX_POINTS = 1000;
   static constexpr int DEFAULT_POINTS = 100;

   static int validatePointCount(int count) {
      if (count < MIN_POINTS || count > MAX_POINTS) {
         std::cerr << ";Warning: Point count " << count << " out of range ["
            << MIN_POINTS << "," << MAX_POINTS << "], using "
            << DEFAULT_POINTS << std::endl;
         return DEFAULT_POINTS;
      }
      return count;
   }
};

#endif // FOLLOWERPOINTSUTILS_H