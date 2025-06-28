#ifndef MOBILE_COMPARISON_RESULT_H
#define MOBILE_COMPARISON_RESULT_H

#include "LatticeCell.h"
#include "LatticeMatchResult.h"
struct MobileComparisonResult {
   int mobileIndex;
   LatticeCell mobile;
   LatticeMatchResult bestResult;
   double distance;
};


#endif // MOBILE_COMPARISON_RESULT_H