// Roof's 1969 Eisenstein Reduction Conditions
// Translated to G6 notation
// 
// G6 vector: [g1, g2, g3, g4, g5, g6]
// where: g1=a², g2=b², g3=c², g4=2bc·cos(α), g5=2ac·cos(β), g6=2ab·cos(γ)
//
// Therefore:
//   ab·cos(γ) = g6/2
//   ac·cos(β) = g5/2  
//   bc·cos(α) = g4/2

#include "G6.h"
#include <cmath>
#include <iostream>

class RoofEisenstein {
public:
   struct ValidationResult {
      bool isValid;
      std::string failedCondition;

      ValidationResult(bool valid = true, const std::string& msg = "")
         : isValid(valid), failedCondition(msg) {
      }
   };

   // Main validation function
   static ValidationResult IsEisensteinReduced(const G6& g6, double tolerance = 1e-10) {
      const double g1 = g6[0];
      const double g2 = g6[1];
      const double g3 = g6[2];
      const double g4 = g6[3];
      const double g5 = g6[4];
      const double g6val = g6[5];

      // Compute the products (divided by 2 from G6 definition)
      const double bc_cos_alpha = g4 / 2.0;  // bc·cos(α)
      const double ac_cos_beta = g5 / 2.0;   // ac·cos(β)
      const double ab_cos_gamma = g6val / 2.0; // ab·cos(γ)

      // Condition 1: Check if all products have same sign
      const bool allPositive = (bc_cos_alpha >= -tolerance &&
         ac_cos_beta >= -tolerance &&
         ab_cos_gamma >= -tolerance);
      const bool allNegative = (bc_cos_alpha <= tolerance &&
         ac_cos_beta <= tolerance &&
         ab_cos_gamma <= tolerance);

      if (!allPositive && !allNegative) {
         return ValidationResult(false, "Condition 1: Scalar products must be all positive or all negative");
      }

      // Condition 2a: a² ≤ b² ≤ c²
      if (g1 > g2 + tolerance) {
         return ValidationResult(false, "Condition 2a: g1 > g2 (a² > b²)");
      }
      if (g2 > g3 + tolerance) {
         return ValidationResult(false, "Condition 2a: g2 > g3 (b² > c²)");
      }

      // Condition 2b: [a² + b² + 2bc·cos(α) + 2ac·cos(β) + 2ab·cos(γ) ≥ 0]
      // In G6: [g1 + g2 + g4 + g5 + g6 ≥ 0]
      // This condition is only checked if all products are negative (bracketed condition)
      if (allNegative) {
         const double sum = g1 + g2 + g4 + g5 + g6val;
         if (sum < -tolerance) {
            return ValidationResult(false, "Condition 2b: g1 + g2 + g4 + g5 + g6 < 0 (sum condition)");
         }
      }

      // Condition 3: Main inequality conditions
      // a² ≥ |2ac·cos(β)| → g1 ≥ |g5|
      if (g1 < std::abs(g5) - tolerance) {
         return ValidationResult(false, "Condition 3a: g1 < |g5| (a² < |2ac·cos(β)|)");
      }

      // a² ≥ |2ab·cos(γ)| → g1 ≥ |g6|
      if (g1 < std::abs(g6val) - tolerance) {
         return ValidationResult(false, "Condition 3b: g1 < |g6| (a² < |2ab·cos(γ)|)");
      }

      // b² ≥ |2bc·cos(α)| → g2 ≥ |g4|
      if (g2 < std::abs(g4) - tolerance) {
         return ValidationResult(false, "Condition 3c: g2 < |g4| (b² < |2bc·cos(α)|)");
      }

      // Condition 4: Tie-breaker conditions
      // If a² = b², then |bc·cos(α)| ≤ |ac·cos(β)| → |g4| ≤ |g5|
      if (std::abs(g1 - g2) <= tolerance) {
         if (std::abs(g4) > std::abs(g5) + tolerance) {
            return ValidationResult(false, "Condition 4a: When g1=g2, require |g4| ≤ |g5|");
         }
      }

      // If b² = c², then |ac·cos(β)| ≤ |ab·cos(γ)| → |g5| ≤ |g6|
      if (std::abs(g2 - g3) <= tolerance) {
         if (std::abs(g5) > std::abs(g6val) + tolerance) {
            return ValidationResult(false, "Condition 4b: When g2=g3, require |g5| ≤ |g6|");
         }
      }

      // If a² + b² + 2bc·cos(α) + 2ac·cos(β) + 2ab·cos(γ) = 0
      // then a² + 2ac·cos(β) + ab·cos(γ) ≤ 0
      // In G6: If g1 + g2 + g4 + g5 + g6 = 0, then g1 + g5 + g6/2 ≤ 0
      // Wait, let me recalculate this...
      // a² + 2ac·cos(β) + ab·cos(γ) = g1 + g5 + g6val/2
      const double mainSum = g1 + g2 + g4 + g5 + g6val;
      if (std::abs(mainSum) <= tolerance) {
         const double subSum = g1 + g5 + g6val / 2.0;
         if (subSum > tolerance) {
            return ValidationResult(false, "Condition 4c: When sum=0, require g1 + g5 + g6/2 ≤ 0");
         }
      }

      // Condition 5: Special conditions when all products are negative
      if (allNegative) {
         // If a² = -2ab·cos(γ), then ac·cos(β) = 0 → If g1 = -g6, then g5 = 0
         if (std::abs(g1 + g6val) <= tolerance) {
            if (std::abs(g5) > tolerance) {
               return ValidationResult(false, "Condition 5a: When g1=-g6 (all negative), require g5=0");
            }
         }

         // If a² = -2ac·cos(β), then ab·cos(γ) = 0 → If g1 = -g5, then g6 = 0
         if (std::abs(g1 + g5) <= tolerance) {
            if (std::abs(g6val) > tolerance) {
               return ValidationResult(false, "Condition 5b: When g1=-g5 (all negative), require g6=0");
            }
         }

         // If b² = -2bc·cos(α), then ab·cos(γ) = 0 → If g2 = -g4, then g6 = 0
         if (std::abs(g2 + g4) <= tolerance) {
            if (std::abs(g6val) > tolerance) {
               return ValidationResult(false, "Condition 5c: When g2=-g4 (all negative), require g6=0");
            }
         }
      }

      // Condition 6: Special conditions when all products are positive
      if (allPositive) {
         // If a² = 2ab·cos(γ), then ac·cos(β) ≤ 2bc·cos(α)
         // → If g1 = g6, then g5 ≤ g4
         if (std::abs(g1 - g6val) <= tolerance) {
            if (g5 > g4 + tolerance) {
               return ValidationResult(false, "Condition 6a: When g1=g6 (all positive), require g5 ≤ g4");
            }
         }

         // If a² = 2ac·cos(β), then ab·cos(γ) ≤ 2bc·cos(α)
         // → If g1 = g5, then g6 ≤ g4
         if (std::abs(g1 - g5) <= tolerance) {
            if (g6val > g4 + tolerance) {
               return ValidationResult(false, "Condition 6b: When g1=g5 (all positive), require g6 ≤ g4");
            }
         }

         // If b² = 2bc·cos(α), then ab·cos(γ) ≤ 2ac·cos(β)
         // → If g2 = g4, then g6 ≤ g5
         if (std::abs(g2 - g4) <= tolerance) {
            if (g6val > g5 + tolerance) {
               return ValidationResult(false, "Condition 6c: When g2=g4 (all positive), require g6 ≤ g5");
            }
         }
      }

      return ValidationResult(true, "All Eisenstein conditions satisfied");
   }

   // Helper function to print validation result
   static void PrintValidation(const G6& g6, double tolerance = 1e-10) {
      ValidationResult result = IsEisensteinReduced(g6, tolerance);

      std::cout << "Eisenstein Reduction Check:" << std::endl;
      std::cout << "G6: " << g6 << std::endl;
      std::cout << "Status: " << (result.isValid ? "VALID" : "INVALID") << std::endl;

      if (!result.isValid) {
         std::cout << "Failed: " << result.failedCondition << std::endl;
      }
   }
};

