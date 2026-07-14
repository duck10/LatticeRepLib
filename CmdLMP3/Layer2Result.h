#ifndef LAYER2RESULT_H
#define LAYER2RESULT_H

#include "LatticeMatchResult.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

struct Layer2Result {
   LRL_Cell     primitiveReference;
   LRL_Cell     primitiveMobile;
   Matrix_3x3   referenceFromPrimitive;  // primitive -> reference conventional frame
   std::string  referenceLatticeType;
   std::string  mobileLatticeType;
   std::vector<LatticeMatchResult> results;

   friend std::ostream& operator<<(std::ostream& os, const Layer2Result& r) {
      os << "; Layer2Result =============================" << std::endl;
      os << "; Reference lattice type: " << r.referenceLatticeType << std::endl;
      os << "; Mobile lattice type:    " << r.mobileLatticeType << std::endl;
      os << "; Primitive reference: "
         << LRL_Cell_Degrees(r.primitiveReference) << std::endl;
      os << "; Primitive mobile:    "
         << LRL_Cell_Degrees(r.primitiveMobile) << std::endl;
      os << "; referenceFromPrimitive: " << r.referenceFromPrimitive << std::endl;
      os << "; Results: " << r.results.size() << std::endl;
      for (size_t i = 0; i < std::min(size_t(3), r.results.size()); ++i) {
         os << "; --- Result " << (i + 1) << " ---" << std::endl;
         os << r.results[i];
      }
      if (r.results.size() > 3)
         os << "; ... " << (r.results.size() - 3)
         << " more results not shown" << std::endl;
      os << "; ===========================================" << std::endl;
      return os;
   }
};
#endif // !LAYER2RESULT_H

