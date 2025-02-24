#ifndef FOLLOWER_MODE_H
#define FOLLOWER_MODE_H

#include <string>
#include <algorithm>
#include <iostream>
#include <array>

#include "StringMatcher.h"

enum class FollowerMode {
   POINT,
   LINE,
   CHORD,
   CHORD3,
   TRIANGLE,
   SPLINE
};

struct ModeInfo {
   std::string name;
   FollowerMode mode;
   int vectors_needed;  // Removed const
};

struct FollowerModeUtils {
   static constexpr double MODE_MATCH_THRESHOLD = 0.6;
   static inline StringMatcher matcher{ MODE_MATCH_THRESHOLD };

   static const std::array<ModeInfo, 6>& getModeInfo() {
      static const std::array<ModeInfo, 6> MODE_INFO = { {
         {"POINT",    FollowerMode::POINT, 1},
         {"LINE",     FollowerMode::LINE, 2},
         {"CHORD",    FollowerMode::CHORD, 2},
         {"CHORD3",   FollowerMode::CHORD3, 3},
         {"TRIANGLE", FollowerMode::TRIANGLE, 3},
         {"SPLINE",   FollowerMode::SPLINE, 2}
      } };
      return MODE_INFO;
   }

   static int getVectorsNeeded(FollowerMode mode) {
      for (const auto& info : getModeInfo()) {
         if (info.mode == mode) return info.vectors_needed;
      }
      return 1;  // Default
   }

   static bool validateVectorCount(FollowerMode mode, size_t vectorCount) {
      const int needed = getVectorsNeeded(mode);
      return vectorCount >= needed;
   }

   static FollowerMode fromString(const std::string& str) {
      std::string upperStr = str;
      std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);

      auto [bestInfo, bestTheta] = std::make_pair(getModeInfo()[0], MODE_MATCH_THRESHOLD);

      for (const auto& info : getModeInfo()) {
         double theta = matcher.getTheta(upperStr, info.name);
         if (theta <= bestTheta) {
            bestTheta = theta;
            bestInfo = info;  // This assignment now works since we removed const
         }
      }

      if (bestTheta > MODE_MATCH_THRESHOLD) {
         // No acceptable match found
         std::cout << ";Warning: Unrecognized mode '" << str << "', using default mode POINT" << std::endl;
         return FollowerMode::POINT;
      }
      else if (upperStr != bestInfo.name) {
         // Close match found, but needed correction
         std::cout << ";Mode '" << str << "' corrected to '" << bestInfo.name << "'" << std::endl;
         return bestInfo.mode;
      }
      else {
         // Exact match
         return bestInfo.mode;
      }
   }

   static std::string toString(FollowerMode mode) {
      for (const auto& info : getModeInfo()) {
         if (info.mode == mode) return info.name;
      }
      return "UNKNOWN";
   }
};

#endif // FOLLOWER_MODE_H
