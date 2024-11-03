// Testbed.cpp : Defines the entry point for the console application.
//

#include "CellInputData.h"
#include "LRL_Cell_Degrees.h"
#include "MatS6.h"
#include "NCDist.h"
#include "Niggli.h"
#include "S6.h"
#include "S6BoundaryTransforms.h"
#include "S6Dist.h"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

#include "S6BoundaryTransforms.h"

int main() {
   //const auto trial = S6BoundaryTransforms::generateFlat24BoundaryTransforms();
   std::cout << " Check each boundary generation separately " << std::endl;
   for ( size_t i=1; i<=6; ++i)
   {
      S6BoundaryTransforms::PrintMatrix(S6BoundaryTransforms::generateOneBoundaryTransform(i));
      std::cout << std::endl;
   }
   std::cout << " Check 4 for a single boundary " << std::endl;
   S6BoundaryTransforms::PrintMatrix(S6BoundaryTransforms::GenerateFourTransformsForOneBoundry(3));
   std::cout << std::endl;
   std::cout << " Check generating one transform for each boundary " << std::endl;
   S6BoundaryTransforms::PrintMatrix(S6BoundaryTransforms::generate6BoundaryTransforms());
   std::cout << std::endl;
   std::cout << " Check generating 4 transform for each boundary " << std::endl;
   S6BoundaryTransforms::PrintMatrix(S6BoundaryTransforms::generate24BoundaryTransforms());
   exit(0);
}
