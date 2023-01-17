//
//  CS6Dist.c
//
//
//  Created by Herbert J. Bernstein and Lawrence C. Andrews on 3/04/17.
//
//

#ifndef CS6DIST_C
#define CS6DIST_C

#ifdef __cplusplus
#include <cmath>
#include <cfloat>

#else 
#include <math.h>
#include <float.h>
#include <stdlib.h>
#endif



#include "CS6Dist.h"
#include "S6M_SellingReduce.h"


/*
     Compute the minimal distance between two S6-reduced
     vectors in the S6 Cone following the embedding paths
     to the 6 major boundaries

     We need to examine unreduction for boundary i in gvec2
     if 4*||gvec1-gvec2||*||gvec1||
        + 12*|gvec1[i]|*||gvec1-gvec2||
        + 2*|gvec1[i]-gvec2[i]|*sqrt(6)*||gvec1-gvec2||
      >= 9*(gvec1(i))^2+7*(gvec1[i]-gvec2[i])^2

     and similary examine unreduction for boundary i in gvec1
     if 4*||gvec1-gvec2||*||gvec2||
        + 12*|gvec2[i]|*||gvec1-gvec2||
        + 2*|gvec1[i]-gvec2[i]|*sqrt(6)*||gvec1-gvec2||
      >= 9*(gvec2(i))^2+7*(gvec1[i]-gvec2[i])^2

 */



double CS6Dist(double gvec1[6],double gvec2[6]) {
    double dist,dist1, dist2, distmin;
    double norm1, norm2, distsq;
    double mgvec1[6], mgvec2[6];
    int unred1[6], unred2[6];
    int ii;
    dist1 = s6minbddist(gvec1);
    dist2 = s6minbddist(gvec2);
    distmin = CS6M_min(dist1,dist2);
    dist = s61234dist(gvec1,gvec2);
    CS6M_report_double("\n  Entered CS6Dist gdist = ",dist,", ");
    CS6M_report_double_vector("gvec1 = ", gvec1,", ")
    CS6M_report_double_vector("gvec2 = ", gvec2,"\n")
    dist = S6Dist_pass(gvec1,gvec2,dist);
    CS6M_report_double_if_changed("gvec1 gvec2 dist = ",dist,"\n");
    norm1=CS6M_norm(gvec1);
    norm2=CS6M_norm(gvec1);
    for (ii=0; ii <6; ii++) {
      unred1[ii] = unred2[ii] = -1;
      if (dist*(4.*norm1+12.*fabs(gvec1[ii])+2.*sqrt(6.))
          > 9.*gvec1[ii]*gvec1[ii]+7.*dist*dist) {
        unred2[ii]=2*ii;
        CS6M_imv6(gvec2,S6MSX[unred2[ii]],mgvec2);
        CS6M_report_double_vector("mgvec2 = ", mgvec2,"\n");
        dist = S6Dist_pass(gvec1,mgvec2,dist);
        CS6M_report_double_if_changed("gvec1 mgvec2 dist = ",dist,"\n");
      }      
      if (dist*(4.*norm2+12.*fabs(gvec2[ii])+2.*sqrt(6.))
          > 9.*gvec2[ii]*gvec2[ii]+7.*dist*dist) {
        unred1[ii]=2*ii;
        CS6M_imv6(gvec1,S6MSX[unred1[ii]],mgvec1);
        CS6M_report_double_vector("mgvec1 = ", mgvec1,"\n")
        dist = S6Dist_pass(mgvec1,gvec2,dist);
        CS6M_report_double_if_changed("mgvec1 gvec2 dist = ",dist,"\n");
      }
    }
    return dist;
}

 double CS6Dist_in_G6(double g6vec1[6],double g6vec2[6]) {
    double s6vec1[6], s6vec2[6];
    double s6redvec1[6], s6redvec2[6];
    int reduced1=0, reduced2=0;
    int kk;
    CS6M_G6toS6(g6vec1,s6vec1);
    CS6M_G6toS6(g6vec2,s6vec2);
    CS6M_S6Reduce(s6vec1,s6redvec1,reduced1);
    CS6M_S6Reduce(s6vec2,s6redvec2,reduced2);
    if (! reduced1) for (kk=0; kk<6; kk++) s6redvec1[kk]=s6vec1[kk];
    if (! reduced2) for (kk=0; kk<6; kk++) s6redvec2[kk]=s6vec2[kk];
    return CS6Dist(s6redvec1, s6redvec2); 
}







#endif /*CS6DIST_C */

