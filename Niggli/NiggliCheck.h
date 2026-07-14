#ifndef NIGGLI_CHECK_H
#define NIGGLI_CHECK_H

#include "G6.h"
#include <cmath>

// Post-hoc check: is g a valid Niggli-reduced cell?
//
// Conditions from Gruber 1973, Krivy-Gruber 1976, and Andrews-Bernstein 1988.
// All three sources agree exactly on these conditions.
// No additional conditions beyond the published algorithms are imposed.

inline bool IsNiggliReduced(const G6& v, const double eps = 0.0) {
   const double g1 = v[0];
   const double g2 = v[1];
   const double g3 = v[2];
   const double g4 = v[3];
   const double g5 = v[4];
   const double g6 = v[5];

   // Positive definite
   if (g1 <= 0.0) return false;
   if (g2 <= 0.0) return false;
   if (g3 <= 0.0) return false;

   // SP1/SP2: ordering g1 <= g2 <= g3
   if (g1 > g2 + eps) return false;
   if (g2 > g3 + eps) return false;

   // SP1 tie-break: g1 = g2 => |g4| <= |g5|
   if (std::abs(g1 - g2) <= eps &&
      std::abs(g4) > std::abs(g5) + eps) return false;

   // SP2 tie-break: g2 = g3 => |g5| <= |g6|
   if (std::abs(g2 - g3) <= eps &&
      std::abs(g5) > std::abs(g6) + eps) return false;

   // SP3/SP4: sign consistency
   // g4*g5*g6 > 0 => all positive (SP3)
   // g4*g5*g6 <= 0 => all negative (SP4, zero treated as negative)
   {
      const bool anyPos = (g4 > eps) || (g5 > eps) || (g6 > eps);
      const bool anyNeg = (g4 < -eps) || (g5 < -eps) || (g6 < -eps);
      if (anyPos && anyNeg) return false;
   }

   // R5: |g4| <= g2
   if (std::abs(g4) > g2 + eps) return false;

   // R6: |g5| <= g1
   if (std::abs(g5) > g1 + eps) return false;

   // R7: |g6| <= g1
   if (std::abs(g6) > g1 + eps) return false;

   // R8: g1+g2+g4+g5+g6 >= 0
   if (g1 + g2 + g4 + g5 + g6 < -eps) return false;

   // R9: boundary of R5
   // g4 = g2 => 2*g5 >= g6
   if (std::abs(g4 - g2) <= eps && 2.0 * g5 < g6 - eps) return false;
   // g4 = -g2 => g6 >= 0
   if (std::abs(g4 + g2) <= eps && g6 < -eps) return false;

   // R10: boundary of R6
   // g5 = g1 => 2*g4 >= g6
   if (std::abs(g5 - g1) <= eps && 2.0 * g4 < g6 - eps) return false;
   // g5 = -g1 => g6 >= 0
   if (std::abs(g5 + g1) <= eps && g6 < -eps) return false;

   // R11: boundary of R7
   // g6 = g1 => 2*g4 >= g5
   if (std::abs(g6 - g1) <= eps && 2.0 * g4 < g5 - eps) return false;
   // g6 = -g1 => g5 >= 0
   if (std::abs(g6 + g1) <= eps && g5 < -eps) return false;

   // R12: boundary of R8
   // g1+g2+g4+g5+g6 = 0 => 2*g1 + 2*g5 + g6 <= 0
   if (std::abs(g1 + g2 + g4 + g5 + g6) <= eps &&
      2.0 * g1 + 2.0 * g5 + g6 > eps) return false;

   return true;
}

#endif // NIGGLI_CHECK_H
