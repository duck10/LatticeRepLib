#include <cmath>
#include <iostream>

#include <string>
#include <vector>
#include <tuple>

#include "G6.h"
/*
Microsoft Copilot translation from the conditons in Roof, 1969
*/

struct EisensteinResult {
   std::vector<std::string> satisfiedConditions;
   std::vector<std::string> violatedConditions;

   friend std::ostream& operator<<(std::ostream& os, const EisensteinResult& result) {
      os << "=== Eisenstein Conditions Check ===\n";

      if (!result.satisfiedConditions.empty()) {
         os << "\nSatisfied Conditions (" << result.satisfiedConditions.size() << "):\n";
         for (const auto& condition : result.satisfiedConditions) {
            os << "  [OK] " << condition << "\n";
         }
      }

      if (!result.violatedConditions.empty()) {
         os << "\nViolated Conditions (" << result.violatedConditions.size() << "):\n";
         for (const auto& condition : result.violatedConditions) {
            os << "  [FAIL] " << condition << "\n";
         }
      }

      if (result.satisfiedConditions.empty() && result.violatedConditions.empty()) {
         os << "\nNo conditions checked (input may not satisfy prerequisites)\n";
      }

      return os;
   }
};


EisensteinResult CheckEisensteinConditions(double a, double b, double c,
   double cosAlpha, double cosBeta, double cosGamma) {
   EisensteinResult result;

   // Derived quantities
   double abCosG = a * b * cosGamma;
   double acCosB = a * c * cosBeta;
   double bcCosA = b * c * cosAlpha;

   double a2 = a * a;
   double b2 = b * b;
   double c2 = c * c;

   // Condition (1): all products positive or all negative
   bool allPositive = (abCosG > 0 && acCosB > 0 && bcCosA > 0);
   bool allNegative = (abCosG < 0 && acCosB < 0 && bcCosA < 0);

   if (allPositive || allNegative) {
      result.satisfiedConditions.push_back("Condition (1): all cosine products same sign");

      // Condition (2)
      if (a2 <= b2 && b2 <= c2) {
         result.satisfiedConditions.push_back("Condition (2): a² ≤ b² ≤ c²");
      } else {
         result.violatedConditions.push_back("Condition (2): a² ≤ b² ≤ c²");
      }

      double bracketExpr = a2 + b2 + 2 * bcCosA + 2 * acCosB + 2 * abCosG;
      if (!allNegative && bracketExpr >= 0) {
         result.satisfiedConditions.push_back("Condition (2): bracket expression ≥ 0");
      }

      // Condition (3)
      if (a2 >= std::abs(2 * acCosB)) result.satisfiedConditions.push_back("Condition (3a)");
      else result.violatedConditions.push_back("Condition (3a)");

      if (a2 >= std::abs(2 * abCosG)) result.satisfiedConditions.push_back("Condition (3b)");
      else result.violatedConditions.push_back("Condition (3b)");

      if (b2 >= std::abs(2 * bcCosA)) result.satisfiedConditions.push_back("Condition (3c)");
      else result.violatedConditions.push_back("Condition (3c)");

      // Condition (4)
      if (a2 == b2 && std::abs(b * c * cosAlpha) <= std::abs(a * c * cosBeta)) {
         result.satisfiedConditions.push_back("Condition (4a)");
      }
      if (b2 == c2 && std::abs(a * c * cosBeta) <= std::abs(a * b * cosGamma)) {
         result.satisfiedConditions.push_back("Condition (4b)");
      }
      if (bracketExpr == 0.0) {
         double subExpr = a2 + 2 * acCosB + abCosG;
         if (subExpr <= 0.0) {
            result.satisfiedConditions.push_back("Condition (4c)");
         } else {
            result.violatedConditions.push_back("Condition (4c)");
         }
      }
   }

   // Condition (5): all cosine products negative
   if (abCosG < 0 && acCosB < 0 && bcCosA < 0) {
      if (a2 == -2 * abCosG && acCosB == 0.0) result.satisfiedConditions.push_back("Condition (5a)");
      if (a2 == -2 * acCosB && abCosG == 0.0) result.satisfiedConditions.push_back("Condition (5b)");
      if (b2 == -2 * bcCosA && abCosG == 0.0) result.satisfiedConditions.push_back("Condition (5c)");
   }

   // Condition (6): all cosine products positive
   if (abCosG > 0 && acCosB > 0 && bcCosA > 0) {
      if (a2 == 2 * abCosG && acCosB <= 2 * bcCosA) result.satisfiedConditions.push_back("Condition (6a)");
      if (a2 == 2 * acCosB && abCosG <= 2 * bcCosA) result.satisfiedConditions.push_back("Condition (6b)");
      if (b2 == 2 * bcCosA && abCosG <= 2 * acCosB) result.satisfiedConditions.push_back("Condition (6c)");
   }

   return result;
}

EisensteinResult CheckEisensteinConditions(const G6 g) {
   return CheckEisensteinConditions(g[0], g[1], g[2], g[3], g[4], g[5]);
}
