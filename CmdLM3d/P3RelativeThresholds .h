#ifndef  P3_RELATIVE_THRESHOLD_H
#define P3_RELATIVE_THRESHOLD_H

#include "MobileComparisonResult.h"
#include "P3.h"

#include <iomanip>
#include <string>

// P3-Relative Threshold System
// Uses reference P3 norm as natural scale for quality assessment

#include <cmath>

class P3RelativeThresholds {
public:
   struct P3ThresholdResult {
      double referenceP3Norm;
      double excellentThreshold;     // e.g., 1% of P3 norm
      double goodThreshold;          // e.g., 5% of P3 norm  
      double poorThreshold;          // e.g., 20% of P3 norm
      int latticeMultiplicity;       // For centering-adjusted version
      std::string method;
   };

   // Strategy 1: Simple P3 fraction thresholds
   static P3ThresholdResult calculateP3FractionThresholds(
      const LRL_Cell& referenceCell,
      double excellentFraction = 0.01,    // 1%
      double goodFraction = 0.05,         // 5% 
      double poorFraction = 0.20) {       // 20%

      P3ThresholdResult result;

      // Calculate P3 norm of reference
      result.referenceP3Norm = P3(referenceCell).norm();
      result.latticeMultiplicity = 1; // Not used in this version
      result.method = "P3 fraction";

      result.excellentThreshold = result.referenceP3Norm * excellentFraction;
      result.goodThreshold = result.referenceP3Norm * goodFraction;
      result.poorThreshold = result.referenceP3Norm * poorFraction;

      return result;
   }

   // Strategy 2: Centering-adjusted P3 thresholds
   static P3ThresholdResult calculateCenteringAdjustedThresholds(
      const LRL_Cell& referenceCell,
      const std::string& latticeType,
      double excellentFraction = 0.01,
      double goodFraction = 0.05,
      double poorFraction = 0.20) {

      P3ThresholdResult result;

      result.referenceP3Norm = P3(referenceCell).norm();
      result.latticeMultiplicity = getLatticeMultiplicity(latticeType);
      result.method = "Centering-adjusted P3";

      // Adjust thresholds by lattice multiplicity
      double adjustedNorm = result.referenceP3Norm / result.latticeMultiplicity;

      result.excellentThreshold = adjustedNorm * excellentFraction;
      result.goodThreshold = adjustedNorm * goodFraction;
      result.poorThreshold = adjustedNorm * poorFraction;

      return result;
   }

// Strategy 3: Fixed strict P3 thresholds - scale-invariant and predictable
   static P3ThresholdResult calculateAdaptiveP3Thresholds(
      const LRL_Cell& referenceCell,
      const std::string& latticeType,
      const std::vector<double>& distances) {

      P3ThresholdResult result;
      result.referenceP3Norm = P3(referenceCell).norm();
      result.latticeMultiplicity = getLatticeMultiplicity(latticeType);
      result.method = "Fixed strict";

      // Always use tight, physically meaningful thresholds
      // These percentages are scale-invariant and work for any unit cell size
      result.excellentThreshold = result.referenceP3Norm * 0.005;  // 0.5%
      result.goodThreshold = result.referenceP3Norm * 0.02;       // 2%
      result.poorThreshold = result.referenceP3Norm * 0.08;       // 8%

      return result;
   }


private:
   // Get lattice multiplicity for centering adjustment
   static int getLatticeMultiplicity(const std::string& latticeType) {
      if (latticeType == "P") return 1;       // Primitive
      if (latticeType == "C" || latticeType == "A" || latticeType == "B") return 2;  // Face-centered
      if (latticeType == "I") return 2;       // Body-centered
      if (latticeType == "F") return 4;       // Face-centered cubic
      if (latticeType == "R") return 3;       // Rhombohedral
      return 1; // Default to primitive
   }
};


#endif // P3_RELATIVE_THRESHOLD_H