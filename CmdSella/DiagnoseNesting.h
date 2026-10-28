#ifndef DIAGNOSE_NESTING_H
#define DIAGNOSE_NESTING_H

// DiagnoseNesting.h
//
// DiagnoseParentAgainstChildWinner -- for one input cell and a named
// (parent, child) pair of Delone types (e.g. O2, O3), asks whether the
// child's best-fit point is also reachable as a parent point.
//
// Reasoning: if the child type bounds/nests in the parent, then any child
// lattice is also a parent lattice, so fit(parent) <= fit(child) must hold
// when both are measured from the same candidate representation. When the
// computed fits violate that, there are three possibilities:
//   (a) the parent's search never generated the candidate the child won
//       with (search problem);
//   (b) the parent's stored projector list lacks the orientation that
//       contains the child's point (library problem);
//   (c) the child does not lie on the parent's boundary in that
//       orientation at all (geometry/nesting problem).
//
// Steps:
//   1. Selling-reduce the input; run Remediation168ForOneSubtype for the
//      child and for the parent independently (same call DirectClassify.h
//      uses), so both fits are printed for reference.
//   2. Take the child's winning candidate x (GetOriginalInput -- set by
//      Remediation168ForOneSubtype to its winning candidate) and projected
//      point y (GetBestFit). Sanity line: |x - y| should equal the child's
//      raw fit; if not, x and y are not in the same frame and the rest of
//      the output cannot be trusted.
//   3. Score the parent directly on x (SellaFitXXXXXX). If this is <= the
//      child's fit, the parent CAN reach it -- (a) above.
//   4. For every stored parent projector P_i: |P_i y - y| (is y in that
//      subspace?) and |perp_i x| (parent fit from x via that projector).
//   5. Repeat 4 under each of the 24 reflections R applied to x and y,
//      reporting per reflection the parent projector with the smallest
//      |P R y - R y|. If some (R, i) contains y but step 4 found none,
//      the point is reachable only in an orientation the search did not
//      present to the parent -- (a) or (b). If no (R, i) contains y, the
//      child point is in no stored parent subspace in any orientation --
//      (b) or (c).
//
// Diagnostic only: prints, changes nothing, not on any production path.

#include "DeloneFitResults.h"
#include "GenerateLatticeTypeExamples.h"
#include "LatticeCell.h"
#include "LRL_StringTools.h"
#include "MatS6.h"
#include "RemediatedFit.h"
#include "S6.h"
#include "Sella.h"

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// SelectOneDeloneType -- GenerateDeloneBase::Select(name) returns a
// vector; pick the entry whose GetName() matches exactly, so a name that
// selects several types cannot silently hand back the wrong one.
inline std::shared_ptr<GenerateDeloneBase> SelectOneDeloneType(const std::string& name) {
   const std::vector<std::shared_ptr<GenerateDeloneBase>> found = GenerateDeloneBase().Select(name);
   for (const auto& t : found) {
      if (t->GetName() == name) return t;
   }
   return nullptr;
}

inline void DiagnoseParentAgainstChildWinner(
   const LatticeCell& input,
   const std::string& parentNameIn,
   const std::string& childNameIn) {

   const std::string parentName = LRL_StringTools::strToupper(parentNameIn);
   const std::string childName = LRL_StringTools::strToupper(childNameIn);

   std::cout << ";" << std::endl;
   std::cout << "; DiagnoseParentAgainstChildWinner  parent " << parentName
      << "  child " << childName << std::endl;
   std::cout << "; input " << input.GetInput() << std::endl;

   const std::shared_ptr<GenerateDeloneBase> parent = SelectOneDeloneType(parentName);
   const std::shared_ptr<GenerateDeloneBase> child = SelectOneDeloneType(childName);
   if (!parent || !child) {
      std::cout << "; ERROR: could not select "
         << (!parent ? parentName : childName) << " by exact name" << std::endl;
      return;
   }

   MatS6 reductionMatrix;
   const S6 oneLattice = GetInputSellingReducedVectors(input, reductionMatrix);
   std::cout << "; reduced S6 " << oneLattice << std::endl;

   const DeloneFitResults childFit = Remediation168ForOneSubtype(oneLattice, child, 10, {});
   const DeloneFitResults parentFit = Remediation168ForOneSubtype(oneLattice, parent, 10, {});

   const S6 x = childFit.GetOriginalInput();   // child's winning candidate
   const S6 y = childFit.GetBestFit();         // child's projected point
   const double scale = (y.norm() > 0.0) ? y.norm() : 1.0;
   const double containTol = 1.0E-6 * scale;

   std::cout << "; child  " << childName << " fit (Remediation168) " << childFit.GetRawFit() << std::endl;
   std::cout << "; parent " << parentName << " fit (Remediation168) " << parentFit.GetRawFit() << std::endl;
   std::cout << "; x (child winning candidate) " << x << std::endl;
   std::cout << "; y (child projected point)   " << y << std::endl;
   std::cout << "; sanity |x - y| " << (x - y).norm()
      << "  (should equal child fit " << childFit.GetRawFit() << ")" << std::endl;

   const DeloneFitResults parentOnX = Sella::SellaFitXXXXXX(parent, x);
   std::cout << "; parent " << parentName << " scored directly on x: " << parentOnX.GetRawFit()
      << ((parentOnX.GetRawFit() <= childFit.GetRawFit() + containTol)
         ? "   <= child fit: parent CAN reach this candidate -> search problem"
         : "   > child fit") << std::endl;

   const std::vector<MatS6> prjs = parent->GetSellaProjectors();
   const std::vector<MatS6> perps = parent->GetSellaPerps();
   const size_t nMats = (prjs.size() < perps.size()) ? prjs.size() : perps.size();
   std::cout << "; containment tolerance |P*y - y| < " << containTol
      << "   (" << nMats << " stored " << parentName << " projectors)" << std::endl;

   std::cout << "; -- stored orientation (no reflection) --" << std::endl;
   bool containedUnreflected = false;
   for (size_t i = 0; i < nMats; ++i) {
      const double containResidual = (prjs[i] * y - y).norm();
      const double fitFromX = (perps[i] * x).norm();
      const bool contains = containResidual < containTol;
      if (contains) containedUnreflected = true;
      std::cout << ";   " << parentName << " matrix " << std::setw(2) << i
         << "   |P*y - y| " << std::setw(12) << containResidual
         << "   |perp*x| " << std::setw(12) << fitFromX
         << (contains ? "   CONTAINS y" : "") << std::endl;
   }

   std::cout << "; -- under each of the 24 reflections (best projector per reflection) --" << std::endl;
   static const std::vector<MatS6> reflections = MatS6::GetReflections();
   bool containedAnyReflection = false;
   double overallBest = -1.0;
   size_t overallBestR = 0;
   size_t overallBestI = 0;
   for (size_t r = 0; r < reflections.size(); ++r) {
      const S6 ry = reflections[r] * y;
      const S6 rx = reflections[r] * x;
      double best = -1.0;
      size_t bestI = 0;
      for (size_t i = 0; i < nMats; ++i) {
         const double containResidual = (prjs[i] * ry - ry).norm();
         if (best < 0.0 || containResidual < best) {
            best = containResidual;
            bestI = i;
         }
      }
      const double fitFromRx = (nMats > 0) ? (perps[bestI] * rx).norm() : -1.0;
      const bool contains = (nMats > 0) && (best < containTol);
      if (contains) containedAnyReflection = true;
      if (overallBest < 0.0 || best < overallBest) {
         overallBest = best;
         overallBestR = r;
         overallBestI = bestI;
      }
      std::cout << ";   reflection " << std::setw(2) << r
         << "   best matrix " << std::setw(2) << bestI
         << "   |P*Ry - Ry| " << std::setw(12) << best
         << "   |perp*Rx| " << std::setw(12) << fitFromRx
         << (contains ? "   CONTAINS Ry" : "") << std::endl;
   }

   std::cout << "; -- summary --" << std::endl;
   std::cout << ";   smallest containment residual " << overallBest
      << " at reflection " << overallBestR << ", matrix " << overallBestI << std::endl;
   if (containedUnreflected) {
      std::cout << ";   y IS in a stored " << parentName
         << " subspace as presented -> library covers it; compare 'scored directly on x' above"
         << std::endl;
   }
   else if (containedAnyReflection) {
      std::cout << ";   y is in a stored " << parentName
         << " subspace only after a reflection -> orientation not presented to the parent"
         << " (search or library-orientation problem)" << std::endl;
   }
   else {
      std::cout << ";   y is in NO stored " << parentName
         << " subspace under any of the 24 reflections -> library gap, or "
         << childName << " does not bound " << parentName << " here" << std::endl;
   }
}

#endif // DIAGNOSE_NESTING_H
