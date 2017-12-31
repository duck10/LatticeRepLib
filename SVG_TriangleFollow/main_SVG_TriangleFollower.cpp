// Triangle.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <cmath>
#include <list>
#include <algorithm>
#include <utility>

#include <cmath>
#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <cstdio>

#include "LRL_Cell.h"
#include "D7.h"
#include "G6.h"
#include "FollowerTools.h"
#include "LRL_MaximaTools.h"
#include "ProcessTriangle.h"
#include "ProgressData.h"
#include "LRL_RandTools.h"
#include "ReadGlobalData.h"
#include "Niggli.h"
#include "Selling.h"
#include "TriangleFollow.h"






typedef S6     TVEC;
typedef Selling TREDUCEMETHOD;
typedef G6     TFOLLOWMETHOD;

double TestDistances( std::vector<double>& dist ) {
   const double maxElement = *std::max_element( dist.begin( ), dist.end( ) );
   const double testValue = dist[0] + dist[1] + dist[2] - 2.0*maxElement;
   return(testValue);
}

double VectorLength( const double* a ) {
   return(sqrt( a[0] * a[0] + a[1] * a[1] + a[2] * a[2] + a[3] * a[3] + a[4] * a[4] + a[5] * a[5] + a[6] * a[6] ));
}

/*
g1:     292.73 925.747 949.503 -1.45515 -199.264 -260.49
37.9889 40.4402 737.339 -21.5135 -0.278502 -13.5016
90.8647 134.863 752.96 -19.2477 -55.1196 -44.0231g2: g3:

NCDist g1 g2 686.718
NCDist g1 g3 584.628
NCDist g2 g3 118.159
g123 g1 g2 994.675
g123 g1 g3 879.107
g123 g2 g3 120.797
Herberts-MacBook-Pro:triang yaya$ ./triang
*/


int main( int argc, char* argv[] )
{
   ReadGlobalData();
   //NC_Distance ncDist;
   double a1[7];
   double a2[7];
   double a3[7];
   std::list<double> worstCases;
   std::vector<int> histogram( 205, 0 );
   //const double testDist =ncDist.Distance( 
   //   G6(std::string("29.1121 126.258 559.269 49.9155 8.43192 7.38645")),
   //   G6(std::string("20.6609 694.915 1030.95 194.538 18.3698 0.372618" )), true );
   //std::cout << "Distance " << testDist << std::endl;
   //exit(0);

   SetSeed(GLOBAL_RunInputVector::globalInputRandomSeed);

   std::cout << std::endl;

   std::cout << "minimum percent violation to report  " << GlobalConstants::globalAboveThisValueIsBad<< std::endl;

   unsigned long count = 0;
   double aRan1[7], aRan2[7], aRan3[7];
   bool btemp;

   while (count < GlobalConstants::globalNumberOfTrialsToAttempt) {
      ++count;
      if (count % 1000 == 0) {
         std::cout << count / 1000 << "K" << std::endl;
         std::cerr << count / 1000 << "K" << std::endl;
      }

      const TVEC v1 = TVEC(G6::randDeloneUnreduced());
      const TVEC v2 = TVEC(G6::randDeloneUnreduced());
      const TVEC v3 = TVEC(G6::randDeloneUnreduced());

      const double d1 = TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD >::DIST(v1, v3);
      const double d2 = TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD >::DIST(v2, v3);
      const double d3 = TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD >::DIST(v1, v2);
      const double percentViolation = 100.0*FollowerTools::TriangleInequality(d1, d2, d3);

      ++(histogram[int(percentViolation) + 100]);

      /*if (percentViolation > GlobalConstants::globalAboveThisValueIsBad)*/ {

         ProcessTriangle<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD> pt;
         TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD > tf1(GLOBAL_RunInputVector::globalInputRandomSeed, count, GlobalConstants::globalStepsPerFrame);
         pt.ProcessViolation(v1, v2, v3, worstCases, tf1, count, percentViolation, GlobalConstants::globalAboveThisValueIsBad);
         std::list<double> worstCasesI;
      }
   }

   int histCount = 0;
   for (unsigned int i = 0; i < histogram.size( ); ++i)
   if (histogram[i] != 0) {
      ++histCount;
      std::cout << i - 100 << "  " << histogram[i] << std::endl;
   }

   std::cout << "steps per frame " << GlobalConstants::globalStepsPerFrame << "  histogram count " << histCount << std::endl;

   return 0;
}

