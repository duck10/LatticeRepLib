#ifndef FRACTURE_SUMMARY_H
#define FRACTURE_SUMMARY_H

#include <string>
#include <ostream>

struct FractureSummary {
   double initialDistance = 0.0;     // Distance before repair
   double finalDistance = 0.0;       // Distance after repair
   double threshold = 0.1;
   bool fractureDetected = false;    // Was a scalar fracture found?
   bool repairSucceeded = false;     // Did the repair reduce the fracture?
   std::string note;                 // Optional diagnostic message

   // Stream insertion for logging/debugging
   friend std::ostream& operator<<(std::ostream& os, const FractureSummary& summary) {
      os << "FractureSummary:\n";
      os << "  Initial Distance: " << summary.initialDistance << "\n";
      os << "  Final Distance:   " << summary.finalDistance << "\n";
      os << "  Fracture Detected: " << (summary.fractureDetected ? "Yes" : "No") << "\n";
      os << "  Repair Succeeded: " << (summary.repairSucceeded ? "Yes" : "No") << "\n";
      if (!summary.note.empty())
         os << "  Note: " << summary.note << "\n";
      return os;
   }

   FractureSummary() = default;
   FractureSummary(double initDist, double finalDist, double thresh,
      bool fractured, bool repaired)
      : initialDistance(initDist),
      finalDistance(finalDist),
      threshold(thresh),
      fractureDetected(fractured),
      repairSucceeded(repaired) {
   }
};

#endif // FRACTURE_SUMMARY_H
