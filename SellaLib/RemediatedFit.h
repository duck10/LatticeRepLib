#ifndef REMEDIATED_FIT_H
#define REMEDIATED_FIT_H

// RemediatedFit.h
//
// The Remediation search used by ComputeDeloneFits (DeloneFitComputation.cpp):
// for one Delone type, refit other representations of the same lattice and
// keep the best.
//
// GetInputSellingReducedVectors -- Selling-reduces an input cell.
// RemediationCandidate -- a representation of the input lattice and the
//    matrix that produced it from the reduced input.
// BuildExactBoundaryCandidates -- the 24 reflections of p and of each R_i p.
// MaxS6Scalar, IsNearlyReduced, REMEDIATION_OUTSIDE_FRACTION -- the filter
//    that rejects candidates more than 10% of |s6| outside the reduced cone.
// MinAbsS6Scalar, RemediationCanImprove -- when the search can be skipped.
// Remediation168ForOneSubtype -- the search itself.
//
// 2026-11-07: removed the diagnostic variants Remediation (2-hop only),
// GetRemediatedFitForType, Remediation144, Remediation2Hop,
// RemediateToStability, ClassifyViaStabilityRemediation and Remediation168
// (multi-sibling wrapper). Their only caller was CmdSella's TestHarness.h,
// removed with the HARNESS control.

#include "DeloneFitResults.h"
#include "DeloneParentageTest.h"
#include "GenerateLatticeTypeExamples.h"
#include "InputHandler.h"
#include "LatticeConverter.h"
#include "MatS6.h"
#include "S6.h"
#include "Sella.h"
#include "Selling.h"

#include <cmath>
#include <memory>
#include <string>
#include <vector>

inline S6 GetInputSellingReducedVectors(const LatticeCell& input, MatS6& mat) {
   const S6 s6 = LatticeConverter::SellingReduceCell(input.getLatticeType(), input.getCell(), mat);
   return s6;
}

// RemediationCandidate (2026-11-08) -- one representation of the input
// lattice, with the matrix that produced it from the Selling-reduced input
// x: s6 = matrix * x. Every operation applied to s6 (reflection, boundary
// transform, re-reduction) is applied to matrix too, so the winning
// candidate's basis change is known when the search ends.
struct RemediationCandidate {
   S6 s6;
   MatS6 matrix;
};

inline RemediationCandidate StartingCandidate(const S6& x) {
   return RemediationCandidate{ x, MatS6().unit() };
}

inline RemediationCandidate ApplyToCandidate(const MatS6& m, const RemediationCandidate& c) {
   return RemediationCandidate{ m * c.s6, m * c.matrix };
}

// BuildExactBoundaryCandidates -- 24 reflections of p, plus 24 reflections
// of R_i * p for each of the 6 Selling boundary transforms: 168
// candidates, every one a genuine (unimodular) representation of the SAME
// lattice as p. Replaces BuildVCPCandidates in the fitting search:
// vcp_i = R_i*pbi - pai differs from R_i*p by s_i*(e_i + R_i*e_i)
// (|.| = sqrt(5)|s_i| for R_1), i.e. it is p mirrored through s_i = 0 --
// correct for the paper's point-to-point distance, not a representative
// of p's lattice unless s_i = 0.
inline std::vector<RemediationCandidate> BuildExactBoundaryCandidates(const RemediationCandidate& p) {
   static const std::vector<MatS6> reflections = MatS6::GetReflections();
   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();

   std::vector<RemediationCandidate> candidates;
   candidates.reserve(reflections.size() * (1 + boundaryTransforms.size()));
   for (const auto& M : reflections) {
      candidates.push_back(ApplyToCandidate(M, p));
   }
   for (const auto& R : boundaryTransforms) {
      const RemediationCandidate rp = ApplyToCandidate(R, p);
      for (const auto& M : reflections) {
         candidates.push_back(ApplyToCandidate(M, rp));
      }
   }
   return candidates;
}


// Largest scalar of an S6; > 0 means the setting is outside the reduced region.
inline double MaxS6Scalar(const S6& s) {
   double m = s[0];
   for (size_t i = 1; i < 6; ++i) if (s[i] > m) m = s[i];
   return m;
}

// A candidate setting is admissible only if it is at most slightly outside
// the reduced region: largest scalar <= fraction * |reduced input|.
inline bool IsNearlyReduced(const S6& candidate, const double limit) {
   return MaxS6Scalar(candidate) <= limit;
}

// The fraction used by IsNearlyReduced in Remediation168ForOneSubtype:
// candidates whose largest scalar exceeds this fraction of |reduced input|
// are rejected.
inline constexpr double REMEDIATION_OUTSIDE_FRACTION = 0.10;

// Smallest |s_i| of an S6.
inline double MinAbsS6Scalar(const S6& s) {
   double m = std::fabs(s[0]);
   for (size_t i = 1; i < 6; ++i) if (std::fabs(s[i]) < m) m = std::fabs(s[i]);
   return m;
}

// RemediationCanImprove (2026-11-07) -- false when Remediation cannot change
// any fit for this reduced cell, so the search can be skipped.
//
// Let L = REMEDIATION_OUTSIDE_FRACTION * |x| and suppose every |s_k(x)| > L.
// R_i negates s_i, adds s_i to the four slots that share a vector with
// slot i, and subtracts it from the opposite slot. Then:
//   one hop R_i x:         slot i holds |s_i| > L -- rejected (and so are
//                          its 24 reflections, same largest scalar);
//   R_j R_i x, j not i or opposite: slot j holds |s_m| + |s_i| > L --
//                          rejected;
//   R_j R_i x, j opposite i: slot i holds |s_j| > L -- rejected;
//   R_i R_i x = x; reflections of x give the same fit (projector lists are
//                          closed under the reflections).
// So round 1 finds no improvement, no later round runs, every winner is x,
// and the cross-seeding passes (seeded only with winners) change nothing:
// the result equals the direct fit for every type.
inline bool RemediationCanImprove(const S6& reduced) {
   return MinAbsS6Scalar(reduced) <= REMEDIATION_OUTSIDE_FRACTION * reduced.norm();
}

// Remediation168ForOneSubtype -- the search for one Delone type. Candidates:
// BuildExactBoundaryCandidates of the current point, the 36 two-hop products
// R_j R_i of it, and extraSeeds (other types' winning candidates, from the
// cross-seeding passes). Candidates failing IsNearlyReduced are skipped.
// Iterated with re-reduction while the fit improves by more than 1e-8, up to
// maxRounds. Returns the best fit, with the winning candidate in
// OriginalInput and its matrix (candidate = matrix * s6) in CandidateMatrix.
//
// The re-reduction matrix from LatticeConverter::SellingReduceCell is taken
// to satisfy reduced = reductionMatrix * input, the convention Sella::Zscore
// relies on (it inverts the matrix to go back). CHECKREMEDIATION verifies
// candidate = matrix * s6 for every type on every cell.
inline DeloneFitResults Remediation168ForOneSubtype(
   const S6& s6, const std::shared_ptr<GenerateDeloneBase>& sptype,
   const int maxRounds, const std::vector<RemediationCandidate>& extraSeeds) {
   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();
   const double limit = REMEDIATION_OUTSIDE_FRACTION * s6.norm();

   RemediationCandidate currentPoint = StartingCandidate(s6);
   DeloneFitResults currentFit = Sella::SellaFitXXXXXX(sptype, s6);
   RemediationCandidate currentBestCandidate = currentPoint;

   for (int round = 0; round < maxRounds; ++round) {
      std::vector<RemediationCandidate> candidates = BuildExactBoundaryCandidates(currentPoint);
      for (const auto& first : boundaryTransforms) {
         const RemediationCandidate afterFirst = ApplyToCandidate(first, currentPoint);
         for (const auto& second : boundaryTransforms) {
            candidates.push_back(ApplyToCandidate(second, afterFirst));
         }
      }
      for (const auto& seed : extraSeeds) {
         candidates.push_back(seed);
      }

      DeloneFitResults roundBest = currentFit;
      RemediationCandidate roundBestCandidate = currentBestCandidate;
      for (const auto& candidate : candidates) {
         if (!IsNearlyReduced(candidate.s6, limit)) continue;
         const DeloneFitResults dfrTemp = Sella::SellaFitXXXXXX(sptype, candidate.s6);
         if (dfrTemp.GetRawFit() < roundBest.GetRawFit()) {
            roundBest = dfrTemp;
            roundBestCandidate = candidate;
         }
      }

      if (roundBest.GetRawFit() < currentFit.GetRawFit() - 1.0E-8) {
         currentFit = roundBest;
         currentBestCandidate = roundBestCandidate;
         MatS6 reductionMatrix;
         const S6 reduced = LatticeConverter::SellingReduceCell("P", currentBestCandidate.s6, reductionMatrix);
         currentPoint = RemediationCandidate{ reduced, reductionMatrix * currentBestCandidate.matrix };
      }
      else {
         break;
      }
   }

   currentFit.SetOriginalInput(currentBestCandidate.s6);
   currentFit.SetCandidateMatrix(currentBestCandidate.matrix);
   return currentFit;
}

#endif  // REMEDIATED_FIT_H
