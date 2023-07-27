#ifndef V7DIST_H
#define V7DIST_H

#include "G6.h"
#include "LRL_Cell.h"
#include "Niggli.h"


 static double V7Dist(const G6& directRed1, const G6& directRed2) {
   G6 inverseRed1;
   G6 inverseRed2;
   const bool bd2 = Niggli::Reduce(directRed1.InverseG6Vector(), inverseRed1);
   const bool bi2 = Niggli::Reduce(directRed2.InverseG6Vector(), inverseRed2);

   if ( !(bi2 && bd2 ) ) {
      return 19193;
   } else {
      const double volumed1 = LRL_Cell(directRed1).Volume();
      const double volumed2 = LRL_Cell(directRed2).Volume();

      double terms[7];
      terms[0] = sqrt(directRed1[0]) - sqrt(directRed2[0]);
      terms[1] = sqrt(directRed1[1]) - sqrt(directRed2[1]);
      terms[2] = sqrt(directRed1[2]) - sqrt(directRed2[2]);

      terms[3] = 1.0 / sqrt(inverseRed1[0]) - 1.0 /sqrt(inverseRed2[0]);
      terms[4] = 1.0 / sqrt(inverseRed1[1]) - 1.0 /sqrt(inverseRed2[1]);
      terms[5] = 1.0 / sqrt(inverseRed1[2]) - 1.0 /sqrt(inverseRed2[2]);

      terms[6] = std::pow(volumed1, 1.0 / 3.0) - std::pow(volumed2, 1.0 / 3.0);

      double distSquared = 0.0;
      for (size_t i = 0; i < 7; ++i) distSquared += terms[i] * terms[i];

      return sqrt(distSquared);
   }

}

#endif  //  V7DIST_H