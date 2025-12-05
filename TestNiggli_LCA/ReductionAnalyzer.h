#ifndef REDUCTION_ANALYZER_H
#define REDUCTION_ANALYZER_H

#include <string>
#include <vector>
#include "G6.h"

// ReductionAnalyzer: Multi-level diagnostic tool for analyzing lattice reduction results
// Distinguishes between actual reduction failures, standardization imperfections, 
// and floating-point artifacts
class ReductionAnalyzer {
public:
   // Represents a single condition violation or issue
   struct Issue {
      std::string condition;        // Which condition (e.g., "g1 <= g2")
      std::string description;      // Human-readable explanation
      double magnitude;             // Size of violation (0 if satisfied)
      bool withinEpsilon;          // Is magnitude within machine epsilon?

      Issue(const std::string& cond, const std::string& desc,
         const double mag, const bool withinEps)
         : condition(cond), description(desc), magnitude(mag), withinEpsilon(withinEps) {
      }
   };

   // Stream output for Issue
   friend std::ostream& operator<<(std::ostream& os, const Issue& issue);

   // Analysis result structure
   struct Analysis {
      // Core Reduction Quality
      bool basicConditionsMet;           // g1,g2,g3 > 0 and sorted
      double traceValue;                 // sum(g1, g2, g3)
      std::vector<Issue> reductionIssues; // Violations of Steps 5-8

      // Standardization Quality  
      bool perfectlyStandardized;        // Steps 1-4 exactly satisfied
      std::vector<Issue> standardizationIssues; // Near-misses in sorting/signs

      // Floating-Point Quality
      double maxViolationMagnitude;      // Largest violation magnitude
      bool allWithinEpsilon;             // Are all violations < epsilon?
      std::vector<Issue> floatingPointArtifacts; // Issues within machine epsilon

      // Summary
      std::string overallAssessment;     // Brief status
      std::string detailedExplanation;   // Full explanation for humans

      Analysis()
         : basicConditionsMet(false)
         , traceValue(0.0)
         , perfectlyStandardized(false)
         , maxViolationMagnitude(0.0)
         , allWithinEpsilon(true) {
      }
   };

   // Stream output for Analysis
   friend std::ostream& operator<<(std::ostream& os, const Analysis& analysis);

   // Analyze a reduced G6 cell
   // tolerance: threshold for "perfect" satisfaction of conditions (typically 1e-12 or 0.0)
   // epsilon: threshold for floating-point noise (typically 1e-13)
   static Analysis Analyze(const G6& reducedG6,
      const double tolerance = 0.0,
      const double epsilon = 1e-13);

private:
   // Check basic validity (g1, g2, g3 > 0)
   static void CheckBasicConditions(const G6& g6, Analysis& result);

   // Check Steps 1-2: Sorting conditions g1 <= g2 <= g3 with tie-breakers
   static void CheckSortingConditions(const G6& g6, Analysis& result,
      const double tolerance, const double epsilon);

   // Check Steps 3-4: Sign normalization (all +++ or all ---)
   static void CheckSignConditions(const G6& g6, Analysis& result,
      const double tolerance, const double epsilon);

   // Check Steps 5-8: Reduction conditions
   static void CheckReductionConditions(const G6& g6, Analysis& result,
      const double tolerance, const double epsilon);

   // Generate human-readable summary
   static void GenerateSummary(Analysis& result);

   // Helper: Check if value is within epsilon of zero
   static bool IsWithinEpsilon(const double value, const double epsilon) {
      return std::abs(value) <= epsilon;
   }

   // Helper: Check if two values are equal within tolerance
   static bool AreEqual(const double a, const double b, const double tolerance) {
      return std::abs(a - b) <= tolerance;
   }
};

#endif // REDUCTION_ANALYZER_H