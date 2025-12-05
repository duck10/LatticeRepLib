#include "ReductionAnalyzer.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <iostream>

// Stream output operator for Issue
std::ostream& operator<<(std::ostream& os, const ReductionAnalyzer::Issue& issue) {
   os << issue.condition << ": " << issue.description;
   if (issue.magnitude > 0.0) {
      os << " (magnitude: " << std::scientific << std::setprecision(2) << issue.magnitude;
      if (issue.withinEpsilon) {
         os << ", within epsilon";
      }
      os << ")";
   }
   return os;
}

// Stream output operator for Analysis
std::ostream& operator<<(std::ostream& os, const ReductionAnalyzer::Analysis& analysis) {
   os << "=== REDUCTION ANALYSIS ===" << std::endl;
   os << std::endl;
   os << "OVERALL: " << analysis.overallAssessment << std::endl;
   os << std::endl;

   if (!analysis.basicConditionsMet) {
      os << "BASIC CONDITIONS: FAILED" << std::endl;
   } else {
      os << "BASIC CONDITIONS: SATISFIED" << std::endl;
   }
   os << "Trace (g1+g2+g3): " << std::fixed << std::setprecision(6) << analysis.traceValue << std::endl;
   os << std::endl;

   // Show reduction issues
   if (!analysis.reductionIssues.empty()) {
      int realIssues = 0;
      for (const auto& issue : analysis.reductionIssues) {
         if (!issue.withinEpsilon) realIssues++;
      }

      os << "REDUCTION CONDITIONS (Steps 5-8): ";
      if (realIssues == 0) {
         os << "SATISFIED";
      } else {
         os << realIssues << " VIOLATION(S)";
      }
      os << std::endl;

      for (const auto& issue : analysis.reductionIssues) {
         if (!issue.withinEpsilon) {
            os << "  when " << issue << std::endl;
         }
      }
   } else {
      os << "REDUCTION CONDITIONS (Steps 5-8): SATISFIED" << std::endl;
   }
   os << std::endl;

   // Show standardization issues
   if (!analysis.standardizationIssues.empty()) {
      int realIssues = 0;
      for (const auto& issue : analysis.standardizationIssues) {
         if (!issue.withinEpsilon) realIssues++;
      }

      os << "STANDARDIZATION (Steps 1-4): ";
      if (realIssues == 0) {
         os << "SATISFIED";
      } else {
         os << realIssues << " ISSUE(S)";
      }
      os << std::endl;

      for (const auto& issue : analysis.standardizationIssues) {
         if (!issue.withinEpsilon) {
            os << "  when " << issue << std::endl;
         }
      }
   } else {
      os << "STANDARDIZATION (Steps 1-4): SATISFIED" << std::endl;
   }
   os << std::endl;

   // Show floating-point artifacts
   if (!analysis.floatingPointArtifacts.empty()) {
      os << "FLOATING-POINT ARTIFACTS: " << analysis.floatingPointArtifacts.size() << std::endl;
      os << "  Maximum violation magnitude: " << std::scientific << std::setprecision(2)
         << analysis.maxViolationMagnitude << std::endl;
      os << "  All within machine epsilon: " << (analysis.allWithinEpsilon ? "YES" : "NO") << std::endl;

      if (analysis.floatingPointArtifacts.size() <= 5) {
         // Show all if 5 or fewer
         for (const auto& issue : analysis.floatingPointArtifacts) {
            os << "  when " << issue << std::endl;
         }
      } else {
         // Show first 3 if more than 5
         os << "  Showing first 3:" << std::endl;
         for (size_t i = 0; i < 3 && i < analysis.floatingPointArtifacts.size(); ++i) {
            os << "  when " << analysis.floatingPointArtifacts[i] << std::endl;
         }
         os << "  ... and " << (analysis.floatingPointArtifacts.size() - 3) << " more" << std::endl;
      }
   } else {
      os << "FLOATING-POINT ARTIFACTS: NONE" << std::endl;
   }

   os << std::endl;
   os << "=========================" << std::endl;

   return os;
}

ReductionAnalyzer::Analysis ReductionAnalyzer::Analyze(const G6& reducedG6,
   const double tolerance,
   const double epsilon) {
   Analysis result;

   // Extract G6 components
   const double g1 = reducedG6[0];
   const double g2 = reducedG6[1];
   const double g3 = reducedG6[2];
   const double g4 = reducedG6[3];
   const double g5 = reducedG6[4];
   const double g6 = reducedG6[5];

   result.traceValue = g1 + g2 + g3;

   // Run all checks
   CheckBasicConditions(reducedG6, result);
   CheckSortingConditions(reducedG6, result, tolerance, epsilon);
   CheckSignConditions(reducedG6, result, tolerance, epsilon);
   CheckReductionConditions(reducedG6, result, tolerance, epsilon);

   // Determine overall status
   result.perfectlyStandardized = result.standardizationIssues.empty();
   result.allWithinEpsilon = true;
   result.maxViolationMagnitude = 0.0;

   // Find maximum violation and check if all within epsilon
   for (const auto& issue : result.reductionIssues) {
      result.maxViolationMagnitude = std::max(result.maxViolationMagnitude, issue.magnitude);
      if (!issue.withinEpsilon) {
         result.allWithinEpsilon = false;
      }
   }
   for (const auto& issue : result.standardizationIssues) {
      result.maxViolationMagnitude = std::max(result.maxViolationMagnitude, issue.magnitude);
      if (!issue.withinEpsilon) {
         result.allWithinEpsilon = false;
      }
   }

   // Separate floating-point artifacts (within epsilon) from real issues
   for (const auto& issue : result.reductionIssues) {
      if (issue.withinEpsilon) {
         result.floatingPointArtifacts.push_back(issue);
      }
   }
   for (const auto& issue : result.standardizationIssues) {
      if (issue.withinEpsilon) {
         result.floatingPointArtifacts.push_back(issue);
      }
   }

   GenerateSummary(result);

   return result;
}

void ReductionAnalyzer::CheckBasicConditions(const G6& g6, Analysis& result) {
   const double g1 = g6[0];
   const double g2 = g6[1];
   const double g3 = g6[2];

   result.basicConditionsMet = true;

   if (g1 <= 0.0) {
      result.reductionIssues.push_back(
         Issue("g1 > 0", "g1 must be positive", -g1, false));
      result.basicConditionsMet = false;
   }
   if (g2 <= 0.0) {
      result.reductionIssues.push_back(
         Issue("g2 > 0", "g2 must be positive", -g2, false));
      result.basicConditionsMet = false;
   }
   if (g3 <= 0.0) {
      result.reductionIssues.push_back(
         Issue("g3 > 0", "g3 must be positive", -g3, false));
      result.basicConditionsMet = false;
   }
}

void ReductionAnalyzer::CheckSortingConditions(const G6& g6, Analysis& result,
   const double tolerance, const double epsilon) {
   const double g1 = g6[0];
   const double g2 = g6[1];
   const double g3 = g6[2];
   const double g4 = g6[3];
   const double g5 = g6[4];
   const double g6val = g6[5];

   // Step 1 condition: g1 <= g2 (with tie-breaking when g1 = g2)
   if (g1 > g2 + tolerance) {
      const double violation = g1 - g2;
      result.standardizationIssues.push_back(
         Issue("g1 <= g2", "g1 > g2 (should swap)", violation,
            IsWithinEpsilon(violation, epsilon)));
   } else if (AreEqual(g1, g2, tolerance) && std::abs(g4) > std::abs(g5) + tolerance) {
      const double violation = std::abs(g4) - std::abs(g5);
      result.standardizationIssues.push_back(
         Issue("g1 = g2 => |g4| <= |g5|",
            "When g1 ~= g2, require |g4| <= |g5| (tie-breaking)",
            violation, IsWithinEpsilon(violation, epsilon)));
   }

   // Step 2 condition: g2 <= g3 (with tie-breaking when g2 = g3)
   if (g2 > g3 + tolerance) {
      const double violation = g2 - g3;
      result.standardizationIssues.push_back(
         Issue("g2 <= g3", "g2 > g3 (should swap)", violation,
            IsWithinEpsilon(violation, epsilon)));
   } else if (AreEqual(g2, g3, tolerance) && std::abs(g5) > std::abs(g6val) + tolerance) {
      const double violation = std::abs(g5) - std::abs(g6val);
      result.standardizationIssues.push_back(
         Issue("g2 = g3 => |g5| <= |g6|",
            "When g2 ~= g3, require |g5| <= |g6| (tie-breaking)",
            violation, IsWithinEpsilon(violation, epsilon)));
   }
}

void ReductionAnalyzer::CheckSignConditions(const G6& g6, Analysis& result,
   const double tolerance, const double epsilon) {
   const double g4 = g6[3];
   const double g5 = g6[4];
   const double g6val = g6[5];

   // Steps 3-4: Sign normalization (all +++ or all ---)
   // Zero is treated as negative
   int nneg = 0;
   if (g4 <= tolerance) nneg++;
   if (g5 <= tolerance) nneg++;
   if (g6val <= tolerance) nneg++;

   int npos = 0;
   if (g4 > tolerance) npos++;
   if (g5 > tolerance) npos++;
   if (g6val > tolerance) npos++;

   if (nneg != 0 && nneg != 3) {
      // Mixed signs - this is a standardization issue
      std::ostringstream desc;
      desc << "g4,g5,g6 must be all positive or all non-positive (found "
         << npos << " positive, " << nneg << " non-positive)";
      result.standardizationIssues.push_back(
         Issue("Signs: all + or all -", desc.str(), 0.0, false));
   }
}

void ReductionAnalyzer::CheckReductionConditions(const G6& g6, Analysis& result,
   const double tolerance, const double epsilon) {
   const double g1 = g6[0];
   const double g2 = g6[1];
   const double g3 = g6[2];
   const double g4 = g6[3];
   const double g5 = g6[4];
   const double g6val = g6[5];

   // Step 5 conditions: |g4| <= g2
   if (std::abs(g4) > g2 + tolerance) {
      const double violation = std::abs(g4) - g2;
      std::ostringstream desc;
      desc << "|g4| > g2 (violates Step 5). Values: |g4|=" << std::abs(g4) << ", g2=" << g2;
      result.reductionIssues.push_back(
         Issue("|g4| <= g2", desc.str(), violation,
            IsWithinEpsilon(violation, epsilon)));
   }
   // Special case: g4 = g2 and 2*g5 < g6
   if (AreEqual(std::abs(g4), g2, tolerance) && 2 * g5 < g6val - tolerance) {
      const double violation = g6val - 2 * g5;
      result.reductionIssues.push_back(
         Issue("g4 = g2 => 2*g5 >= g6",
            "When |g4| = g2, require 2*g5 >= g6 (Step 5 tie-breaking)",
            violation, IsWithinEpsilon(violation, epsilon)));
   }
   // Special case: g4 = -g2 and g6 < 0
   if (AreEqual(g4, -g2, tolerance) && g6val < -tolerance) {
      const double violation = -g6val;
      result.reductionIssues.push_back(
         Issue("g4 = -g2 => g6 >= 0",
            "When g4 = -g2, require g6 >= 0 (Step 5 special case)",
            violation, IsWithinEpsilon(violation, epsilon)));
   }

   // Step 6 conditions: |g5| <= g1
   if (std::abs(g5) > g1 + tolerance) {
      const double violation = std::abs(g5) - g1;
      result.reductionIssues.push_back(
         Issue("|g5| <= g1", "|g5| > g1 (violates Step 6)", violation,
            IsWithinEpsilon(violation, epsilon)));
   }
   // Special cases for Step 6
   if (AreEqual(std::abs(g5), g1, tolerance) && 2 * g4 < g6val - tolerance) {
      const double violation = g6val - 2 * g4;
      result.reductionIssues.push_back(
         Issue("g5 = g1 => 2*g4 >= g6",
            "When |g5| = g1, require 2*g4 >= g6 (Step 6 tie-breaking)",
            violation, IsWithinEpsilon(violation, epsilon)));
   }
   if (AreEqual(g5, -g1, tolerance) && g6val < -tolerance) {
      const double violation = -g6val;
      result.reductionIssues.push_back(
         Issue("g5 = -g1 => g6 >= 0",
            "When g5 = -g1, require g6 >= 0 (Step 6 special case)",
            violation, IsWithinEpsilon(violation, epsilon)));
   }

   // Step 7 conditions: |g6| <= g1
   if (std::abs(g6val) > g1 + tolerance) {
      const double violation = std::abs(g6val) - g1;
      result.reductionIssues.push_back(
         Issue("|g6| <= g1", "|g6| > g1 (violates Step 7)", violation,
            IsWithinEpsilon(violation, epsilon)));
   }
   // Special cases for Step 7
   if (AreEqual(std::abs(g6val), g1, tolerance) && 2 * g4 < g5 - tolerance) {
      const double violation = g5 - 2 * g4;
      std::ostringstream desc;
      desc << "When |g6| = g1, require 2*g4 >= g5. "
         << "Values: |g6|=" << std::abs(g6val) << ", g1=" << g1
         << ", 2*g4=" << (2 * g4) << ", g5=" << g5;
      result.reductionIssues.push_back(
         Issue("g6 = g1 => 2*g4 >= g5", desc.str(),
            violation, IsWithinEpsilon(violation, epsilon)));
   }
   if (AreEqual(g6val, -g1, tolerance) && g5 < -tolerance) {
      const double violation = -g5;
      result.reductionIssues.push_back(
         Issue("g6 = -g1 => g5 >= 0",
            "When g6 = -g1, require g5 >= 0 (Step 7 special case)",
            violation, IsWithinEpsilon(violation, epsilon)));
   }

   // Step 8 conditions: g4 + g5 + g6 + g1 + g2 >= 0
   const double sum = g4 + g5 + g6val + g1 + g2;
   if (sum < -tolerance) {
      const double violation = -sum;
      std::ostringstream desc;
      desc << "Sum of scalar products is negative (Step 8). Sum = " << sum;
      result.reductionIssues.push_back(
         Issue("g4 + g5 + g6 + g1 + g2 >= 0", desc.str(),
            violation, IsWithinEpsilon(violation, epsilon)));
   }
   // Special case: sum = 0 and 2*(g1+g5)+g6 > 0
   if (AreEqual(sum, 0.0, tolerance) && 2 * (g1 + g5) + g6val > tolerance) {
      const double violation = 2 * (g1 + g5) + g6val;
      result.reductionIssues.push_back(
         Issue("sum = 0 => 2*(g1+g5)+g6 <= 0",
            "When g4+g5+g6+g1+g2 = 0, require 2*(g1+g5)+g6 <= 0 (Step 8 tie-breaking)",
            violation, IsWithinEpsilon(violation, epsilon)));
   }
}

void ReductionAnalyzer::GenerateSummary(Analysis& result) {
   std::ostringstream overall;
   std::ostringstream detailed;

   // Count real issues (not floating-point artifacts)
   int realReductionIssues = 0;
   int realStandardizationIssues = 0;

   for (const auto& issue : result.reductionIssues) {
      if (!issue.withinEpsilon) realReductionIssues++;
   }
   for (const auto& issue : result.standardizationIssues) {
      if (!issue.withinEpsilon) realStandardizationIssues++;
   }

   // Generate overall assessment
   if (!result.basicConditionsMet) {
      overall << "INVALID: Basic conditions violated";
   } else if (realReductionIssues > 0) {
      overall << "REDUCTION INCOMPLETE: " << realReductionIssues << " condition(s) violated";
   } else if (realStandardizationIssues > 0) {
      overall << "REDUCED BUT NOT STANDARDIZED: " << realStandardizationIssues << " sorting issue(s)";
   } else if (!result.floatingPointArtifacts.empty()) {
      overall << "SUCCESSFULLY REDUCED (floating-point artifacts present)";
   } else {
      overall << "SUCCESSFULLY REDUCED AND STANDARDIZED";
   }

   result.overallAssessment = overall.str();

   // Generate detailed explanation
   detailed << std::fixed << std::setprecision(3);
   detailed << "Trace: " << result.traceValue << std::endl;

   if (!result.basicConditionsMet) {
      detailed << "\nBASIC CONDITIONS FAILED:" << std::endl;
      for (const auto& issue : result.reductionIssues) {
         if (issue.condition.find("g1 > 0") != std::string::npos ||
            issue.condition.find("g2 > 0") != std::string::npos ||
            issue.condition.find("g3 > 0") != std::string::npos) {
            detailed << "  when " << issue.description << std::endl;
         }
      }
   }

   if (realReductionIssues > 0) {
      detailed << "\nREDUCTION ISSUES (" << realReductionIssues << "):" << std::endl;
      for (const auto& issue : result.reductionIssues) {
         if (!issue.withinEpsilon) {
            detailed << "  when " << issue.condition << ": " << issue.description
               << " (violation: " << std::scientific << std::setprecision(2)
               << issue.magnitude << ")" << std::endl;
         }
      }
   }

   if (realStandardizationIssues > 0) {
      detailed << "\nSTANDARDIZATION ISSUES (" << realStandardizationIssues << "):" << std::endl;
      for (const auto& issue : result.standardizationIssues) {
         if (!issue.withinEpsilon) {
            detailed << "  when " << issue.condition << ": " << issue.description
               << " (violation: " << std::scientific << std::setprecision(2)
               << issue.magnitude << ")" << std::endl;
         }
      }
   }

   if (!result.floatingPointArtifacts.empty()) {
      detailed << "\nFLOATING-POINT ARTIFACTS (" << result.floatingPointArtifacts.size() << "):" << std::endl;
      detailed << "  Maximum magnitude: " << std::scientific << std::setprecision(2)
         << result.maxViolationMagnitude << std::endl;
      detailed << "  All violations within machine epsilon (< " << std::scientific
         << 1e-13 << ")" << std::endl;
      detailed << "  These do not affect crystallographic interpretation." << std::endl;
   }

   result.detailedExplanation = detailed.str();
}