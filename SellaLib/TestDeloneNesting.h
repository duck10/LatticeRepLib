// TestDeloneNesting.h
//
// SELLA item 8: reusable version of the idempotent-projector nesting test
// (P*Q=Q) that originally confirmed O4 nests inside M1A only, not
// M1B/M2A. "child nests inside parent" means every cell already
// constrained to child's Delone subspace is automatically fixed by
// parent's projector too: parentProjector * childProjector == childProjector
// (within tolerance -- these come from real Selling-reduced matrices, not
// exact rational arithmetic, so an exact MatS6::operator== is too strict).
//
// Projectors come straight from GenerateLatticeTypeExamples::
// CreateListOfDeloneTypes() -- the same source the original mC/oC test
// used -- via each type's GetPrj(). No new projector data is defined
// here, only the test and a name-keyed lookup over the existing list.

#ifndef TESTDELONENESTING_H
#define TESTDELONENESTING_H

#include "GenerateLatticeTypeExamples.h"
#include "MatS6.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

class TestDeloneNesting {
public:
   static bool NestsInside(
      const MatS6& parentProjector,
      const MatS6& childProjector,
      const double tolerance = 1.0E-8) {

      const MatS6 product = parentProjector * childProjector;
      const MatS6 difference = product - childProjector;
      return difference.norm() < tolerance;
   }

   // Builds a Delone-name -> projector map from the full type list, once
   // per audit run (not once per pair -- CreateListOfDeloneTypes()
   // allocates the whole list, so callers testing many pairs should call
   // this once and reuse the map, as RunItem8NestingAudit does below).
   static std::map<std::string, MatS6> BuildProjectorMap() {
      std::map<std::string, MatS6> out;
      const auto deloneTypes = GenerateLatticeTypeExamples::CreateListOfDeloneTypes();
      for (const auto& dt : deloneTypes) {
         out[dt->GetName()] = dt->GetPrj();
      }
      return out;
   }

   // Looks up both names in an already-built projector map and runs
   // NestsInside on them. Returns false (with a console note) if either
   // name isn't found, rather than silently treating a typo as "doesn't
   // nest" -- a wrong answer from a bad name is worse than a loud one.
   static bool TestPair(
      const std::map<std::string, MatS6>& projectorsByName,
      const std::string& parentName,
      const std::string& childName,
      const double tolerance = 1.0E-8) {

      const auto parentIt = projectorsByName.find(parentName);
      const auto childIt = projectorsByName.find(childName);
      if (parentIt == projectorsByName.end()) {
         std::cout << "; TestDeloneNesting: unknown Delone type \"" << parentName << "\"" << std::endl;
         return false;
      }
      if (childIt == projectorsByName.end()) {
         std::cout << "; TestDeloneNesting: unknown Delone type \"" << childName << "\"" << std::endl;
         return false;
      }
      return NestsInside(parentIt->second, childIt->second, tolerance);
   }
};

#endif // TESTDELONENESTING_H