#ifndef TRANSITION_CASE_H
#define TRANSITION_CASE_H

#include <string>
#include "P3.h"
#include "FractureSummary.h"

struct TransitionCase {
   std::string key;
   P3 baseCell;
   P3 perturbedCell;
   P3 reducedBase;
   P3 reducedPerturbed;
   P3 originalVector;  // ← new
   P3 finalVector;     // ← new
   int binBase;
   int binPerturbed;
   bool crossedBoundary;
   FractureSummary summary;
   std::string typeLabel = "UNCLASSIFIED";
   std::string matrixUsed = "19191";
   bool wasRepaired = false;
   std::string tag = "SIMPLE_SHIFT";

   friend std::ostream& operator<<(std::ostream& os, const TransitionCase& tc) {
      os << "Key: " << tc.key << '\n'
         << "Bin Transition: " << tc.binBase << " -> " << tc.binPerturbed
         << (tc.crossedBoundary ? " [X]" : " [-]") << '\n'
         << "Final Scalar Distance: " << tc.summary.finalDistance << '\n'
         << "Reduced Base: " << tc.reducedBase << '\n'
         << "Reduced Perturbed: " << tc.reducedPerturbed << '\n'
         << "Original Vector: " << tc.originalVector << '\n'
         << "Final Vector: " << tc.finalVector << '\n'
         << "Matrix Used: " << ((tc.matrixUsed == "19191") ? "not initialized" : tc.matrixUsed) << '\n'
         << "Was Repaired: " << (tc.wasRepaired ? "true" : "false") << '\n'
         << "Tag: " << tc.tag << '\n'
         << "Type Label: " << tc.typeLabel << '\n';
         return os;
   }
};

static std::string GenerateTransitionKey(const TransitionCase& tc) {
   return tc.summary.note + "_" +
      std::to_string(tc.binBase) + "->" +
      std::to_string(tc.binPerturbed);
}

#endif // TRANSITION_CASE_H
