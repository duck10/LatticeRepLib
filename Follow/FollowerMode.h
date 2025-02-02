#ifndef FOLLOWER_MODE_H
#define FOLLOWER_MODE_H

#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

enum class FollowerMode {
   POINT,
   LINE,
   CHORD,
   CHORD3,
   TRIANGLE,
   SPLINE
};

struct FollowerModeUtils {
   static int getVectorsNeeded(FollowerMode mode) {
      switch (mode) {
      case FollowerMode::POINT: return 1;
      case FollowerMode::LINE: return 2;
      case FollowerMode::CHORD: return 2;
      case FollowerMode::CHORD3: return 3;
      case FollowerMode::TRIANGLE: return 3;
      case FollowerMode::SPLINE: return 2;
      default: return 1;
      }
   }

   static bool validateVectorCount(FollowerMode mode, size_t vectorCount, std::string& errorMsg) {
      const int needed = getVectorsNeeded(mode);
      if (vectorCount < needed) {
         throw std::runtime_error("; Not enough vectors for MODE " + toString(mode) +
            " (have " + std::to_string(vectorCount) + ", need " +
            std::to_string(needed) + ")");
      }
      return true;
   }

   static FollowerMode fromString(const std::string& str) {
      std::string upperStr = str;
      std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);

      if (upperStr == "POINT") return FollowerMode::POINT;
      if (upperStr == "LINE") return FollowerMode::LINE;
      if (upperStr == "CHORD") return FollowerMode::CHORD;
      if (upperStr == "CHORD3") return FollowerMode::CHORD3;
      if (upperStr == "TRIANGLE") return FollowerMode::TRIANGLE;
      if (upperStr == "SPLINE") return FollowerMode::SPLINE;

      std::cerr << ";Warning: Unrecognized mode '" << str
         << "', using default mode POINT" << std::endl;
      return FollowerMode::POINT;
   }

   static std::string toString(FollowerMode mode) {
      switch (mode) {
      case FollowerMode::POINT: return "POINT";
      case FollowerMode::LINE: return "LINE";
      case FollowerMode::CHORD: return "CHORD";
      case FollowerMode::CHORD3: return "CHORD3"; 
      case FollowerMode::TRIANGLE: return "TRIANGLE";
      case FollowerMode::SPLINE: return "SPLINE";
      default: return "UNKNOWN";
      }
   }
};

#endif // FOLLOWER_MODE_H