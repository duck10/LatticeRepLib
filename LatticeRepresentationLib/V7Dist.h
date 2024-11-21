#ifndef V7DIST_H
#define V7DIST_H

#include "G6.h"
#include "LRL_Cell.h"
#include "Niggli.h"

//const LRL_Cell inverse(data.GetCell().Inverse());

 static double V7Dist(const G6& directRed1, const G6& directRed2) {
    G6 Red1;
    G6 Red2;
    G6 inverseRed1;
    G6 inverseRed2;
    const bool bd1 = Niggli::Reduce(G6(directRed1), Red1);
    const bool bd2 = Niggli::Reduce(G6(directRed2), Red2);
    const LRL_Cell cell1(directRed1);
    const LRL_Cell cell2(directRed2);
    const LRL_Cell cell1inverse(cell1.Inverse());
    const LRL_Cell cell2inverse(cell2.Inverse());
    const bool bi1 = Niggli::Reduce(G6(cell1inverse), inverseRed1);
    const bool bi2 = Niggli::Reduce(G6(cell2inverse), inverseRed2);
    if (!(bi2 && bi2)) {
       return 19193;
    }
    else {
       const double volumed1 = LRL_Cell(directRed1).Volume();
       const double volumed2 = LRL_Cell(directRed2).Volume();

      double terms[7];
      terms[0] = sqrt(Red1[0]) - sqrt(Red2[0]);
      terms[1] = sqrt(Red1[1]) - sqrt(Red2[1]);
      terms[2] = sqrt(Red1[2]) - sqrt(Red2[2]);

      terms[3] = 1.0 / sqrt(inverseRed1[0]) - 1.0 /sqrt(inverseRed2[0]);
      terms[4] = 1.0 / sqrt(inverseRed1[1]) - 1.0 /sqrt(inverseRed2[1]);
      terms[5] = 1.0 / sqrt(inverseRed1[2]) - 1.0 /sqrt(inverseRed2[2]);

      terms[6] = std::pow(volumed1, 1.0 / 3.0) - std::pow(volumed2, 1.0 / 3.0);

      double distSquared = 0.0;
      for (const auto& t : terms) distSquared += t * t;

      return sqrt(distSquared);
   }

}

#endif  //  V7DIST_H