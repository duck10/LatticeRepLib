#ifndef BOUNDARY_EXPLORER_H
#define BOUNDARY_EXPLORER_H

#include "P3_Reduce.h"  // for perturbations and reduction methods
#include "PairReporter.h"

class BoundaryExplorer {
public:

   BoundaryExplorer(double perturbationMagnitude = 0.01);


private:
   double m_magnitude;
};

#endif  // BOUNDARY_EXPLORER_H
