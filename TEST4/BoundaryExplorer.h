#ifndef BOUNDARY_EXPLORER_H
#define BOUNDARY_EXPLORER_H

#include "P3_Reduce.h"  // for perturbations and reduction methods
#include "FractureSummary.h"
#include "P3SpecialGenerator.h"

class BoundaryExplorer {
public:
   void RunTrial(const std::pair<P3, P3>& pr) const {
      return RunTrial(pr.first, pr.second);
   }
   void RunTrial(const P3& base, const P3& perturbed) const;

   BoundaryExplorer(double perturbationMagnitude = 0.05)
      : m_generator(perturbationMagnitude) {
   }

private:
   double m_magnitude;
   mutable P3SpecialGenerator m_generator;
};

#endif  // BOUNDARY_EXPLORER_H
