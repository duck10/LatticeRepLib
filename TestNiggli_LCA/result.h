#ifndef SEEBER_Z_RESULT_H
#define SEEBER_Z_RESULT_H


#include <cmath>
#include <string>
#include <vector>

#include "S6.h"

struct ZReductionResult {
   bool isZReduced;
   std::vector<std::string> passed;
   std::vector<std::string> failed;
};

ZReductionResult CheckZReduction(const S6& s) {
   return CheckZReduction(s[0], s[1], s[2], s[3], s[4], s[5]);
}

   ZReductionResult CheckZReduction(double c11, double c22, double c33,
      double c12, double c13, double c23) {
      ZReductionResult result;
   bool acute = (c12 > 0 && c13 > 0 && c23 > 0);
   bool obtuse = (c12 <= 0 && c13 <= 0 && c23 <= 0);

   auto abs = [](double x) { return std::abs(x); };

   if (acute) {
      // Main conditions
      if (c11 <= c22 && c22 <= c33) result.passed.push_back("acute: c11 ? c22 ? c33");
      else result.failed.push_back("acute: c11 ? c22 ? c33");

      if (2 * c12 <= c11) result.passed.push_back("acute: 2c12 ? c11");
      else result.failed.push_back("acute: 2c12 ? c11");

      if (2 * c13 <= c11) result.passed.push_back("acute: 2c13 ? c11");
      else result.failed.push_back("acute: 2c13 ? c11");

      if (2 * c23 <= c22) result.passed.push_back("acute: 2c23 ? c22");
      else result.failed.push_back("acute: 2c23 ? c22");

      // Auxiliary conditions
      if (c11 == c22 && c23 <= c13) result.passed.push_back("acute: c23 ? c13 if c11 = c22");
      if (c22 == c33 && c13 <= c12) result.passed.push_back("acute: c13 ? c12 if c22 = c33");
      if (2 * c23 == c22 && c12 <= 2 * c13) result.passed.push_back("acute: c12 ? 2c13 if 2c23 = c22");
      if (2 * c13 == c11 && c12 <= 2 * c23) result.passed.push_back("acute: c12 ? 2c23 if 2c13 = c11");
      if (2 * c12 == c11 && c13 <= 2 * c23) result.passed.push_back("acute: c13 ? 2c23 if 2c12 = c11");
   }

   if (obtuse) {
      // Main conditions
      if (c11 <= c22 && c22 <= c33) result.passed.push_back("obtuse: c11 ? c22 ? c33");
      else result.failed.push_back("obtuse: c11 ? c22 ? c33");

      if (2 * abs(c12) <= c11) result.passed.push_back("obtuse: 2|c12| ? c11");
      else result.failed.push_back("obtuse: 2|c12| ? c11");

      if (2 * abs(c13) <= c11) result.passed.push_back("obtuse: 2|c13| ? c11");
      else result.failed.push_back("obtuse: 2|c13| ? c11");

      if (2 * abs(c23) <= c22) result.passed.push_back("obtuse: 2|c23| ? c22");
      else result.failed.push_back("obtuse: 2|c23| ? c22");

      double sumAbs = abs(c12) + abs(c13) + abs(c23);
      if (2 * sumAbs <= c11 + c22) result.passed.push_back("obtuse: 2? ? c11 + c22");
      else result.failed.push_back("obtuse: 2? ? c11 + c22");

      // Auxiliary conditions
      if (c11 == c22 && abs(c23) <= abs(c13)) result.passed.push_back("obtuse: |c23| ? |c13| if c11 = c22");
      if (c22 == c33 && abs(c13) <= abs(c12)) result.passed.push_back("obtuse: |c13| ? |c12| if c22 = c33");
      if (2 * abs(c23) == c22 && abs(c12) == 0.0) result.passed.push_back("obtuse: |c12| = 0 if 2|c23| = c22");
      if (2 * abs(c13) == c11 && abs(c12) == 0.0) result.passed.push_back("obtuse: |c12| = 0 if 2|c13| = c11");
      if (2 * abs(c12) == c11 && abs(c13) == 0.0) result.passed.push_back("obtuse: |c13| = 0 if 2|c12| = c11");
      if (2 * sumAbs == c11 + c22 && c11 <= abs(c12) + abs(c13)) result.passed.push_back("obtuse: c11 ? |c12| + |c13| if 2? = c11 + c22");
   }

   result.isZReduced = result.failed.empty();
   return result;
}

#endif // !SEEBER_Z_RESULT_H

