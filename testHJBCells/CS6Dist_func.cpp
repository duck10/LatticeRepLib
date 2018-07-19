// S6Dist_func.cpp:  Function to return the S6 distance

#include <iostream>

#include "S6.h"
#include "S6Dist.h"
#include "Selling.h"
#include "CS6Dist.h"

double CS6Dist_func(double gvec_A[6], double gvec_B[6])
{
   const S6 s6_A(gvec_A[0],gvec_A[1],gvec_A[2],gvec_A[3],gvec_A[4],gvec_A[5]);
   const S6 s6_B(gvec_B[0],gvec_B[1],gvec_B[2],gvec_B[3],gvec_B[4],gvec_B[5]);
   /* std::cout << "s6_A: ["
   << s6_A[0] << ","
   << s6_A[1] << "," 
   << s6_A[2] << "," 
   << s6_A[3] << "," 
   << s6_A[4] << ","
   << s6_A[5] << "]" << std::endl;
   std::cout << "s6_B: ["
   << s6_B[0] << ","
   << s6_B[1] << "," 
   << s6_B[2] << "," 
   << s6_B[3] << "," 
   << s6_B[4] << ","
   << s6_B[5] <<  "]" << std::endl;
   */
   S6 red_A;
   S6 red_B;
   double dred_A[6];
   double dred_B[6];
   bool bA, bB;
   S6Dist s6dist;
   if (Selling::IsReduced(s6_A)) red_A=s6_A;
   else bA = Selling::Reduce(s6_A, red_A);
   if (Selling::IsReduced(s6_B)) red_B=s6_B;
   else bB = Selling::Reduce(s6_B, red_B);
   for (int ii=0; ii<6; ii++) {
        dred_A[ii] = red_A[ii];
        dred_B[ii] = red_B[ii];
   }
   /* std::cout << "A: ["
   << red_A[0] << ","
   << red_A[1] << "," 
   << red_A[2] << "," 
   << red_A[3] << "," 
   << red_A[4] << ","
   << red_A[5] <<  "]" << std::endl;
   std::cout << "B: ["
   << red_B[0] << ","
   << red_B[1] << "," 
   << red_B[2] << "," 
   << red_B[3] << "," 
   << red_B[4] << ","
   << red_B[5] <<  "]" << std::endl;
   */
   const double dist = CS6Dist(dred_A, dred_B);
   return dist;
}

