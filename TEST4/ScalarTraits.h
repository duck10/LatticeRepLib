#ifndef SCALAR_TRAITS_H
#define SCALAR_TRAITS_H

#include <vector>
#include <string>
#include <cmath>
#include <utility>
#include "P3.h"

struct ScalarTraits {
   bool hasNearZero = false;
   bool hasNearEqual = false;
   std::vector<int> nearZeroIndices;
   std::vector<std::pair<int, int>> nearEqualPairs;

   std::string Tag() const {
      std::string tag;
      if (hasNearZero) {
         tag += "Zero_";
         for (int i : nearZeroIndices) tag += std::to_string(i);
      }
      if (hasNearEqual) {
         tag += "_Equal_";
         for (const auto& p : nearEqualPairs)
            tag += std::to_string(p.first) + "-" + std::to_string(p.second) + "_";
      }
      return tag.empty() ? "NoTraits" : tag;
   }
};

inline ScalarTraits AnalyzeScalarTraits(const P3& cell, const double epsilon = 1e-3) {
   ScalarTraits traits;
   const std::vector<double> flat = cell.GetFlatVector();

   for (int i = 0; i < 6; ++i) {
      if (std::abs(flat[i]) < epsilon) {
         traits.hasNearZero = true;
         traits.nearZeroIndices.push_back(i);
      }
   }

   for (int i = 0; i < 6; ++i) {
      for (int j = i + 1; j < 6; ++j) {
         if (std::abs(flat[i] - flat[j]) < epsilon) {
            traits.hasNearEqual = true;
            traits.nearEqualPairs.emplace_back(i, j);
         }
      }
   }

   return traits;
}

#endif // SCALAR_TRAITS_H
