#include "BoundaryExplorer.h"
#include "P3.h"
#include "P3_Reduce.h"
#include "P3Utilities.h"
#include "StoreResults.h"

#include "PairReporter.h"

#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>

static const Matrix_3x3 identity{ 1,0,0, 0,1,0, 0,0,1 };




std::string GetTransformationSignature(const P3& reduced) {
   const std::vector<double> vec = reduced.GetFlatVector();
   const double epsilon = 1e-5;

   std::string signature;

   // Sign pattern
   signature += "S:";
   for (const double val : vec) {
      if (std::abs(val) < epsilon) signature += "0";
      else if (val > 0) signature += "+";
      else signature += "-";
   }

   // Near-zero count
   int nearZeroCount = 0;
   for (const double val : vec) {
      if (std::abs(val) < epsilon) ++nearZeroCount;
   }
   signature += "|Z:" + std::to_string(nearZeroCount);

   // Permutation class (sorted index pattern)
   std::vector<std::pair<double, int>> indexed;
   for (size_t i = 0; i < vec.size(); ++i) {
      indexed.emplace_back(vec[i], static_cast<int>(i));
   }
   std::sort(indexed.begin(), indexed.end());

   signature += "|P:";
   for (const auto& pair : indexed) {
      signature += std::to_string(pair.second);
   }

   // Optional: symmetry hint (e.g. repeated values)
   std::map<int, int> valueBuckets;
   for (const double val : vec) {
      int bucket = static_cast<int>(val / epsilon);  // crude binning
      valueBuckets[bucket]++;
   }

   int repeatedBuckets = 0;
   for (const auto& kv : valueBuckets) {
      if (kv.second > 1) ++repeatedBuckets;
   }
   signature += "|R:" + std::to_string(repeatedBuckets);

   return signature;
}

std::string MakeStableKey(int binA, int binB, double distance) {
   std::ostringstream oss;
   oss << "TRANSFORM[bin" << binA << "→bin" << binB << "]";
   oss << ":CLASS[Stable]";
   return oss.str();
}

static int GetDistanceBin(const double finalDistance) {
   return static_cast<int>(std::log10(finalDistance * 1000));  // Tunable scale
}

static int GetDistanceBin(const P3& a, const P3& b) {
   return static_cast<int>(std::log10((a - b).norm() * 1000));  // Tunable scale
}

