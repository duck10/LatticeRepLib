#ifndef FOLLOWER_MODE_H
#define FOLLOWER_MODE_H

#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include "StringMatcher.h"
#include "TNear.h"

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


   static CNearTree<MatchableString> createModeTree() {
      CNearTree<MatchableString> tree;
      tree.insert(MatchableString("POINT"));
      tree.insert(MatchableString("LINE"));
      tree.insert(MatchableString("CHORD"));
      tree.insert(MatchableString("CHORD3"));
      tree.insert(MatchableString("TRIANGLE"));
      tree.insert(MatchableString("SPLINE"));
      return tree;
   }

   static FollowerMode fromString(const std::string& str) {
      std::string upperStr = str;
      std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
      static const auto modeTree = createModeTree();

      MatchableString closest;
      if (modeTree.NearestNeighbor(0.6, closest, MatchableString(upperStr))) {
         if (closest.name != upperStr) {
            std::cerr << ";Mode '" << str << "' corrected to '" << closest.name << "'" << std::endl;
         }
         if (closest.name == "POINT") return FollowerMode::POINT;
         if (closest.name == "LINE") return FollowerMode::LINE;
         if (closest.name == "CHORD") return FollowerMode::CHORD;
         if (closest.name == "CHORD3") return FollowerMode::CHORD3;
         if (closest.name == "TRIANGLE") return FollowerMode::TRIANGLE;
         if (closest.name == "SPLINE") return FollowerMode::SPLINE;
      }

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