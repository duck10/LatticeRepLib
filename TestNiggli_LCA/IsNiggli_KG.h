// IsNiggli_KG.h
// Niggli condition checker based on Krivy-Gruber (1976) reduction conditions
// Reports which specific conditions are violated

#ifndef ISNIGGLI_KG_H
#define ISNIGGLI_KG_H

#include "G6.h"
#include <vector>
#include <string>
#include <cmath>

struct NiggliViolation {
   std::string condition;
   std::string description;

   NiggliViolation(const std::string& cond, const std::string& desc)
      : condition(cond), description(desc) {
   }
};

class IsNiggli_KG {
public:
   // Main checking function - returns true if cell is Niggli reduced
   // Populates violations vector with any failed conditions
   static bool Check(const G6& v, std::vector<NiggliViolation>& violations, double tolerance = 1.0e-6) {
      violations.clear();

      // G6 notation: g1=a^2, g2=b^2, g3=c^2, g4=2bc*cos(alpha), g5=2ac*cos(beta), g6=2ab*cos(gamma)
      const double g1 = v[0];
      const double g2 = v[1];
      const double g3 = v[2];
      const double g4 = v[3];
      const double g5 = v[4];
      const double g6 = v[5];

      // Basic validity checks
      if (g1 <= 0.0) {
         violations.push_back(NiggliViolation("g1 > 0", "g1 must be positive"));
      }
      if (g2 <= 0.0) {
         violations.push_back(NiggliViolation("g2 > 0", "g2 must be positive"));
      }
      if (g3 <= 0.0) {
         violations.push_back(NiggliViolation("g3 > 0", "g3 must be positive"));
      }

      // Step 1 condition: g1 <= g2 (with tie-breaking when g1 ~= g2)
      if (g1 > g2 + tolerance) {
         violations.push_back(NiggliViolation("g1 <= g2",
            "g1 > g2 (should swap (g1,g4) <-> (g2,g5))"));
      } else if (std::abs(g1 - g2) <= tolerance && std::abs(g4) > std::abs(g5) + tolerance) {
         violations.push_back(NiggliViolation("g1 = g2 => |g4| <= |g5|",
            "When g1 ~= g2, require |g4| <= |g5| (tie-breaking)"));
      }

      // Step 2 condition: g2 <= g3 (with tie-breaking when g2 ~= g3)
      if (g2 > g3 + tolerance) {
         violations.push_back(NiggliViolation("g2 <= g3",
            "g2 > g3 (should swap (g2,g5) <-> (g3,g6))"));
      } else if (std::abs(g2 - g3) <= tolerance && std::abs(g5) > std::abs(g6) + tolerance) {
         violations.push_back(NiggliViolation("g2 = g3 => |g5| <= |g6|",
            "When g2 ~= g3, require |g5| <= |g6| (tie-breaking)"));
      }

      // Steps 3-4: Sign normalization (all +++ or all ---)
      // In Niggli reduction, zero is treated as negative
      // Use tolerance to handle floating-point noise
      int nneg = 0;
      if (g4 <= tolerance) nneg++;   // Negative or zero (within tolerance)
      if (g5 <= tolerance) nneg++;
      if (g6 <= tolerance) nneg++;

      int npos = 0;
      if (g4 > tolerance) npos++;    // Strictly positive (beyond tolerance)
      if (g5 > tolerance) npos++;
      if (g6 > tolerance) npos++;

      if (nneg != 0 && nneg != 3) {
         violations.push_back(NiggliViolation("Signs: all + or all -",
            "g4,g5,g6 must be all positive or all non-positive (zero counts as negative)"));
      }

      // Step 5 conditions: |g4| <= g2
      if (std::abs(g4) > g2 + tolerance) {
         violations.push_back(NiggliViolation("|g4| <= g2",
            "|g4| > g2 (violates Step 5)"));
      }
      // Special case: g4 = g2 and 2*g5 < g6
      if (std::abs(std::abs(g4) - g2) <= tolerance && 2 * g5 < g6 - tolerance) {
         violations.push_back(NiggliViolation("g4 = g2 => 2*g5 >= g6",
            "When |g4| = g2, require 2*g5 >= g6 (Step 5 tie-breaking)"));
      }
      // Special case: g4 = -g2 and g6 < 0
      if (std::abs(g4 + g2) <= tolerance && g6 < 0.0) {
         violations.push_back(NiggliViolation("g4 = -g2 => g6 >= 0",
            "When g4 = -g2, require g6 >= 0 (Step 5 special case)"));
      }

      // Step 6 conditions: |g5| <= g1
      if (std::abs(g5) > g1 + tolerance) {
         violations.push_back(NiggliViolation("|g5| <= g1",
            "|g5| > g1 (violates Step 6)"));
      }
      // Special case: g5 = g1 and 2*g4 < g6
      if (std::abs(std::abs(g5) - g1) <= tolerance && 2 * g4 < g6 - tolerance) {
         violations.push_back(NiggliViolation("g5 = g1 => 2*g4 >= g6",
            "When |g5| = g1, require 2*g4 >= g6 (Step 6 tie-breaking)"));
      }
      // Special case: g5 = -g1 and g6 < 0
      if (std::abs(g5 + g1) <= tolerance && g6 < 0.0) {
         violations.push_back(NiggliViolation("g5 = -g1 => g6 >= 0",
            "When g5 = -g1, require g6 >= 0 (Step 6 special case)"));
      }

      // Step 7 conditions: |g6| <= g1
      if (std::abs(g6) > g1 + tolerance) {
         violations.push_back(NiggliViolation("|g6| <= g1",
            "|g6| > g1 (violates Step 7)"));
      }
      // Special case: g6 = g1 and 2*g4 < g5
      if (std::abs(std::abs(g6) - g1) <= tolerance && 2 * g4 < g5 - tolerance) {
         violations.push_back(NiggliViolation("g6 = g1 => 2*g4 >= g5",
            "When |g6| = g1, require 2*g4 >= g5 (Step 7 tie-breaking)"));
      }
      // Special case: g6 = -g1 and g5 < 0
      if (std::abs(g6 + g1) <= tolerance && g5 < 0.0) {
         violations.push_back(NiggliViolation("g6 = -g1 => g5 >= 0",
            "When g6 = -g1, require g5 >= 0 (Step 7 special case)"));
      }

      // Step 8 conditions: g4 + g5 + g6 + g1 + g2 >= 0
      double sum = g4 + g5 + g6 + g1 + g2;
      if (sum < -tolerance) {
         violations.push_back(NiggliViolation("g4 + g5 + g6 + g1 + g2 >= 0",
            "Sum of scalar products and squared lengths is negative (Step 8)"));
      }
      // Special case: sum = 0 and 2*(g1+g5)+g6 > 0
      if (std::abs(sum) <= tolerance && 2 * (g1 + g5) + g6 > tolerance) {
         violations.push_back(NiggliViolation("sum = 0 => 2*(g1+g5)+g6 <= 0",
            "When g4+g5+g6+g1+g2 = 0, require 2*(g1+g5)+g6 <= 0 (Step 8 tie-breaking)"));
      }

      // NOTE: The "all negative => sum <= 0" condition is NOT a Niggli reduction condition.
      // It's a lattice validity constraint and belongs in G6_Validity, not here.

      return violations.empty();
   }

   // Simple boolean check without violation details
   static bool IsNiggli(const G6& v, double tolerance = 1.0e-6) {
      std::vector<NiggliViolation> violations;
      return Check(v, violations, tolerance);
   }

   // Print violations to output stream
   static void PrintViolations(const std::vector<NiggliViolation>& violations, std::ostream& os = std::cout) {
      if (violations.empty()) {
         os << "Cell satisfies all Niggli conditions" << std::endl;
         return;
      }

      os << "Niggli condition violations (" << violations.size() << "):" << std::endl;
      for (size_t i = 0; i < violations.size(); ++i) {
         os << "  " << (i + 1) << ". " << violations[i].condition << std::endl;
         os << "     " << violations[i].description << std::endl;
      }
   }
};

#endif // ISNIGGLI_KG_H
