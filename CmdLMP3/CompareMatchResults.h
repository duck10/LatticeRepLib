#ifndef COMPAREMATCHRESULTS_H
#define COMPAREMATCHRESULTS_H

#include "LatticeMatchResult.h"
#include "MatrixScore.h"

inline bool compareMatchResults(const LatticeMatchResult& a, const LatticeMatchResult& b) {
   if (a.getP3Distance() != b.getP3Distance())
      return a.getP3Distance() < b.getP3Distance();
   return matrixScore(a.getTransformationMatrix()) <
      matrixScore(b.getTransformationMatrix());
}

#endif // !COMPAREMATCHRESULTS_H

