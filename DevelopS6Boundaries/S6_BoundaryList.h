#ifndef S6_BOUNDARYLIST_H 
#define S6_BOUNDARYLIST_H

#include <vector>

#include "S6_Boundary.h"

class S6_BoundaryList {
public:
   S6_BoundaryList();
   virtual ~S6_BoundaryList() {};

   static std::vector<S6_Boundary> GetVectorOfBoundaries(void) { CalculateBoundaries(); return m_bounds; }
   static S6_Boundary GetBoundary(const unsigned int n) { CalculateBoundaries(); return m_bounds[n]; }
   static std::vector<S6_Boundary> CalculateBoundaries();

private:
   static std::vector<S6_Boundary> m_bounds;
};


#endif  // S6_BOUNDARYLIST_H

