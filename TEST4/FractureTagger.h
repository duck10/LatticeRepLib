#ifndef FRACTURE_TAGGER_H
#define FRACTURE_TAGGER_H

#include <string>
#include "FractureSummary.h"

class FractureTagger {
public:
   static std::string Tag(const FractureSummary& summary) {
      if (!summary.fractureDetected) {
         return "NoBoundary";
      }

      if (summary.repairSucceeded) {
         return "Fracture_Recovered";
      }

      if (summary.finalDistance > 5.0) {
         return "Fracture_Chaotic";
      }

      if (summary.finalDistance >= summary.threshold) {
         return "Fracture_Severe";
      }

      return "Fracture_Mild";
   }
};
#endif // !FRACTURE_TAGGER_H
