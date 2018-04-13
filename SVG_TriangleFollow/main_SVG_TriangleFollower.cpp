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
#include "CellInputData.h"
#include "GlobalTriangleFollowerConstants.h"
#include "D7.h"
#include "G6.h"
#include "FollowerTools.h"
#include "GenerateRandomLattice.h"
#include "LRL_MaximaTools.h"
#include "ProcessTriangle.h"
#include "ProgressData.h"
//#include "ProjectorTools.h"
#include "LRL_RandTools.h"
#include "LRL_ToString.h"
#include "LRL_ReadLatticeData.h"
#include "ReadGlobalData.h"
#include "Niggli.h"
#include "Selling.h"
#include "TriangleFollow.h"

#include <omp.h> 





typedef S6     TVEC;
typedef Selling TREDUCEMETHOD;
typedef G6     TFOLLOWMETHOD;
int iseed = 19191; // bad case was 18

const double g_minimumPercentViolationToDetect = -DBL_MAX;
const double g_minimumPercentViolationToReport = 1.;
const double g_maximunPercentViolationToReport = DBL_MAX;
const double g_closeToBoundary = 0.005;
const bool   g_printRawVectors = true;
const int    g_maximumWorstCasesToReport = 11111;

long g_instanceCounter;

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

std::vector<CellInputData> OptionalCellInput(void) {
   std::cout << "Enter some triples of unit cells" << std::endl;
   std::cout << "  (Remainders will be ignored)" << std::endl;
   std::cout << "Input vector/cell type followed by cell parameters" << std::endl;
   std::cout << "V for G6" << std::endl;
   std::cout << "D for D7" << std::endl;
   std::cout << "S for Selling parameters (Delone scalars)" << std::endl;
   std::cout << "p,a,b,c,i,f for cell with lattice type" << std::endl;
   std::cout << " (after triples are exhausted, the rest of the input trial count will be attempted" << std::endl;


   std::vector<CellInputData> celldata;
   std::string lattice("");
   while (lattice != "EOF") {
      LRL_ReadLatticeData rcd(GLOBAL_RunInputVector::globalInputRandomSeed);
      rcd.read();
      lattice = rcd.GetLattice();

      if (!lattice.empty() && lattice != "EOF") {
         rcd.SetVarietyRange(std::make_pair(0, 23));
         celldata.push_back(rcd);
      }
   }
   return celldata;
}

std::vector<CellInputData> Startup() {
   std::cout << "you can input \"FilePrefx\"" << std::endl;
   std::cout << "you can input \"trials\"" << std::endl;
   std::cout << "you can input \"stepsPerFrame\"" << std::endl;
   std::cout << "you can input \"FixRandomSeed (true/false)\"" << std::endl;
   std::cout << "you can input \"RandomSeed\"" << std::endl;
   ReadGlobalData();

   std::cout << std::endl;

   const std::vector<CellInputData> celldata = OptionalCellInput();

   std::cout << "minimum percent violation to report  " << g_minimumPercentViolationToReport << std::endl;

   return celldata;
}

int main( int argc, char* argv[] )
{
   //NC_Distance ncDist;
   double a1[6];
   double a2[6];
   double a3[6];
   std::list<double> worstCases;
   std::vector<int> histogram( 205, 0 );
   //const double testDist =ncDist.Distance( 
   //   G6(std::string("29.1121 126.258 559.269 49.9155 8.43192 7.38645")),
   //   G6(std::string("20.6609 694.915 1030.95 194.538 18.3698 0.372618" )), true );
   //std::cout << "Distance " << testDist << std::endl;
   //exit(0);

   std::vector<CellInputData> celldata = Startup();
   unsigned long triangleCount = 0;
   bool btemp;
   G6 v1;
   G6 v2;
   G6 v3;
   const unsigned long trialsToAttempt = std::max( (unsigned long)(GlobalConstants::globalNumberOfTrialsToAttempt), (unsigned long)(celldata.size()));
   GenerateRandomLattice<S6> grl(GLOBAL_RunInputVector::globalInputRandomSeed);
   while (triangleCount < trialsToAttempt) {
      ++triangleCount;
      if (triangleCount % 1000 == 0) {
         std::cout << triangleCount / 1000 << "K" << std::endl;
         std::cerr << triangleCount / 1000 << "K" << std::endl;
      }

      if (3 * triangleCount <= celldata.size()) {
         const unsigned long pos = 3 * triangleCount - 3;
         const LRL_Cell cell1 = celldata[pos + 0].GetCell();
         const LRL_Cell cell2 = celldata[pos + 1].GetCell();
         const LRL_Cell cell3 = celldata[pos + 2].GetCell();

         const std::string lat1 = celldata[pos + 0].GetLattice();
         const std::string lat2 = celldata[pos + 1].GetLattice();
         const std::string lat3 = celldata[pos + 2].GetLattice();
      }
      //while (count < GlobalConstants::globalNumberOfTrialsToAttempt) {
      //   ++count;
      //   if (count % 1000 == 0) {
      //      std::cout << count / 1000 << "K" << std::endl;
      //      std::cerr << count / 1000 << "K" << std::endl;
      //   }

      //   const TVEC v1 = TVEC(grl.GenerateExtreme().second);
      //   const TVEC v2 = TVEC(grl.GenerateExtreme().second);
      //   const TVEC v3 = TVEC(grl.GenerateExtreme().second);
      //      v1 = LRL_Cell::GetPrimitiveV6Vector(lat1, cell1);
      //      v2 = LRL_Cell::GetPrimitiveV6Vector(lat2, cell2);
      //      v3 = LRL_Cell::GetPrimitiveV6Vector(lat3, cell3);
      //   }
      else {
         v1 = grl.Generate();
         v2 = grl.Generate();
         v3 = grl.Generate();
      }

      const double d1 = TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD >::DIST(v1, v3);
      const double d2 = TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD >::DIST(v2, v3);
      const double d3 = TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD >::DIST(v1, v2);
      if (d1<1.0E-10 || d2<1.0E-10 || d3<1.0E-10 ) {
         std::cout << "rejected due to zero area" << std::endl;
         continue;
      }
      const double percentViolation = 100.0*FollowerTools::TriangleInequality(d1, d2, d3);

      ++(histogram[int(percentViolation) + 100]);

      /*if (percentViolation > GlobalConstants::globalPercentChangeToDetect)*/ {

         ProcessTriangle<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD> pt;
         TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD > tf1(GLOBAL_RunInputVector::globalInputRandomSeed, triangleCount, GlobalConstants::globalStepsPerFrame);
         pt.ProcessViolation(v1, v2, v3, worstCases, tf1, triangleCount, percentViolation, GlobalConstants::globalPercentChangeToDetect);
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

