#ifndef STABLE_TRANSITION_CASE_H
#define STABLE_TRANSITION_CASE_H

#include <string>
#include <iomanip>
#include "LRL_Vector3.h"

struct StableTransitionCase {
   int binA;
   int binB;
   double finalDistance;
   Matrix_3x3 transformMatrix;  // Replace with your actual matrix type
   std::string tag;          // e.g., "SelfMatch", "Stable", "Redundant"

   friend std::ostream& operator<<(std::ostream& os, const StableTransitionCase& stc) {
      std::stringstream mtemp;
      mtemp << stc.transformMatrix;
      os << "Bin Transition: " << stc.binA << " -> " << stc.binB
         << (stc.binA != stc.binB ? " [X]" : " [-]") << '\n'
         << "Final Scalar Distance: " << std::fixed << std::setprecision(6) << stc.finalDistance << '\n'
         << "Transformation Tag: " << stc.tag << '\n'
         << "Matrix used = " << ((stc.transformMatrix[0] == 19191.0) ? "not initialized" : mtemp.str()) << std::endl;
         return os;
      }

};

#endif // !STABLE_TRANSITION_CASE_H
