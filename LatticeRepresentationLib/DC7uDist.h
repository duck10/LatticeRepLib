#ifndef DC7UDIST_H
#define DC7UDIST_H

#include "DC7u.h"
#include "LRL_indices.h"
#include "LRL_MinMaxTools.h"

#include <cmath>

//Distance_DC7U

//class DC7uDistance /*: public Distance*/ {

static double DC7uPermDistanceBetween(const DC7u& v1, const DC7u& v2) {
   double v1abc[7]{ 0,0,0,0,0,0,0 };
   double v2abc[7]{ 0,0,0,0,0,0,0 };
   double v2acb[7]{ 0,0,0,0,0,0,0 };
   double v2bac[7]{ 0,0,0,0,0,0,0 };
   double v2bca[7]{ 0,0,0,0,0,0,0 };
   double v2cab[7]{ 0,0,0,0,0,0,0 };
   double v2cba[7]{ 0,0,0,0,0,0,0 };

   v1abc[DC7u_AA_idx] = v1[DC7u_AA_idx];
   v1abc[DC7u_BB_idx] = v1[DC7u_BB_idx];
   v1abc[DC7u_CC_idx] = v1[DC7u_CC_idx];
   v1abc[DC7u_MIN_BC_diagsq_idx] = v1[DC7u_MIN_BC_diagsq_idx];
   v1abc[DC7u_MIN_AC_diagsq_idx] = v1[DC7u_MIN_AC_diagsq_idx];
   v1abc[DC7u_MIN_AB_diagsq_idx] = v1[DC7u_MIN_AB_diagsq_idx];
   v1abc[DC7u_MIN_ABC_diagsq_idx] = v1[DC7u_MIN_ABC_diagsq_idx];

   v2cba[DC7u_CC_idx] = v2cab[DC7u_CC_idx] = v2bca[DC7u_BB_idx] = v2bac[DC7u_BB_idx] = v2acb[DC7u_AA_idx] = v2abc[DC7u_AA_idx] = v2[DC7u_AA_idx];
   v2cba[DC7u_BB_idx] = v2cab[DC7u_AA_idx] = v2bca[DC7u_CC_idx] = v2bac[DC7u_AA_idx] = v2acb[DC7u_CC_idx] = v2abc[DC7u_BB_idx] = v2[DC7u_BB_idx];
   v2cba[DC7u_CC_idx] = v2cab[DC7u_BB_idx] = v2bca[DC7u_AA_idx] = v2bac[DC7u_CC_idx] = v2acb[DC7u_BB_idx] = v2abc[DC7u_CC_idx] = v2[DC7u_CC_idx];

   v2cba[DC7u_MIN_AB_diagsq_idx] = v2cab[DC7u_MIN_AB_diagsq_idx] = v2bca[DC7u_MIN_AC_diagsq_idx] = v2bac[DC7u_MIN_AC_diagsq_idx]
      = v2acb[DC7u_MIN_BC_diagsq_idx] = v2abc[DC7u_MIN_BC_diagsq_idx] = v2[DC7u_MIN_BC_diagsq_idx];

   v2cba[DC7u_MIN_AC_diagsq_idx] = v2cab[DC7u_MIN_BC_diagsq_idx] = v2bca[DC7u_MIN_AB_diagsq_idx] = v2bac[DC7u_MIN_BC_diagsq_idx]
      = v2acb[DC7u_MIN_AB_diagsq_idx] = v2abc[DC7u_MIN_AC_diagsq_idx] = v2[DC7u_MIN_AC_diagsq_idx];

   v2cba[DC7u_MIN_BC_diagsq_idx] = v2cab[DC7u_MIN_AC_diagsq_idx] = v2bca[DC7u_MIN_BC_diagsq_idx] = v2bac[DC7u_MIN_AB_diagsq_idx]
      = v2acb[DC7u_MIN_AC_diagsq_idx] = v2abc[DC7u_MIN_AB_diagsq_idx] = v2[DC7u_MIN_AB_diagsq_idx];

   v2cba[DC7u_MIN_ABC_diagsq_idx] = v2cab[DC7u_MIN_ABC_diagsq_idx] = v2bca[DC7u_MIN_ABC_diagsq_idx] = v2bac[DC7u_MIN_ABC_diagsq_idx]
      = v2acb[DC7u_MIN_ABC_diagsq_idx] = v2abc[DC7u_MIN_ABC_diagsq_idx] = v2[DC7u_MIN_ABC_diagsq_idx];

   double  distsqabc = 0.0;
   double  distsqacb = 0.0;
   double  distsqbac = 0.0;
   double  distsqbca = 0.0;
   double  distsqcab = 0.0;
   double  distsqcba = 0.0;

   for (int ii = 0; ii < 7; ii++) {
      distsqabc += (v1abc[ii] - v2abc[ii]) * (v1abc[ii] - v2abc[ii]);
      distsqacb += (v1abc[ii] - v2acb[ii]) * (v1abc[ii] - v2acb[ii]);
      distsqbac += (v1abc[ii] - v2bac[ii]) * (v1abc[ii] - v2bac[ii]);
      distsqbca += (v1abc[ii] - v2bca[ii]) * (v1abc[ii] - v2bca[ii]);
      distsqcab += (v1abc[ii] - v2cab[ii]) * (v1abc[ii] - v2cab[ii]);
      distsqcba += (v1abc[ii] - v2cba[ii]) * (v1abc[ii] - v2cba[ii]);
   }

   const double distsq = minNC(distsqabc, distsqacb, distsqbac, distsqbca, distsqcab, distsqcba);

   return sqrt(distsq);

}
//};

#endif  // DC7UDIST_H

