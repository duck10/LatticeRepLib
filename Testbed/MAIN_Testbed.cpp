// Testbed.cpp : Defines the entry point for the console application.
//

#include "CellInputData.h"
#include "LRL_CreateFileName.h"
#include "CS6Dist.h"
#include "DeloneTypeList.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_LatticeMatcher.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "MatN.h"
#include "MatS6.h"
#include "S6.h"
#include "S6Dist.h"
#include "TNear.h"

#include <algorithm>
#include <utility>

std::string Letters( void ) {
   return "V,G,D,S,P,A,B,C,I,F,R,C3,G6,S6,B4,D7,H";
}

std::vector<LRL_ReadLatticeData> GetInputCells( void ) {
   const std::string letters = Letters( );
   std::string lattice;
   std::vector<LRL_ReadLatticeData> cellDataList;
   LRL_ReadLatticeData rcd;
   while (lattice != "EOF") {
      rcd.read( );
      lattice = rcd.GetLattice( );
      if ((!lattice.empty( )) && (letters.find( static_cast<char>(toupper( lattice[0] )) ) != std::string::npos))
         cellDataList.push_back( rcd );
   }

   return cellDataList;
}

std::vector<S6> GetInputSellingReducedVectors( const std::vector<LRL_ReadLatticeData>& input ) {
   std::vector<S6> v;
   LatticeConverter converter;

   MatS6 mat;
   for (size_t i = 0; i < input.size( ); ++i) {
      const S6 s6 = converter.SellingReduceCell( input[i].GetLattice( ), input[i].GetCell( ), mat );
      v.push_back( s6 );
   }
   return v;
}

void Test_E3toS6() {

   // R1 is index 4  R3 is index 5
   std::string lattice( "R" );
   LRL_Cell cellacute( "10 10 10 65 65 65" );
   LRL_Cell cellobtuse( "10 10 10  15 15 15" );
   LatticeConverter lc;
   const  std::vector<std::pair<std::string, std::vector<double> > > vM3d = DeloneTypeList::Make3dCenteringMatrices(  );

   for (size_t i = 0; i < vM3d.size( ); ++i) std::cout << i << " " << vM3d[i].first << std::endl;

   const MatS6 m6_R1( MatS6::e3Tos6( vM3d[4].second ) );
   std::cout << "R1" << std::endl;
   std::cout << LRL_Cell_Degrees( m6_R1 * S6(cellacute) ) << std::endl;

   const MatS6 m6_R3( MatS6::e3Tos6( vM3d[4].second ) );
   std::cout << "R3" << std::endl;
   std::cout << LRL_Cell_Degrees( m6_R3 * cellobtuse ) << std::endl;
   std::cout << LRL_Cell_Degrees( lc.DeloneReduceCell("P", m6_R3 * S6(cellobtuse)) ) << std::endl;

}

class xxxxx {
   double DistanceBetween( const double x, const double xx ) { return 19191; }
};

int main( int argc, char* argv[] )
{
   //Test_E3toS6( );

   const std::vector<LRL_ReadLatticeData> input = GetInputCells( );
   MatS6 mat_reference;
   LRL_Cell cell_reference = LatticeConverter::SellingReduceCell( input[0].GetLattice(), input[0].GetCell(), mat_reference );
   const MV_Pair mv_reducedReference(S6( cell_reference ), MatN(mat_reference.GetMatrix()).inverse());

   const std::vector<S6> vLat = GetInputSellingReducedVectors( input );
   //std::cout << "Reduced input cells " << std::endl;
   //for (size_t lat = 0; lat < vLat.size( ); ++lat) {
   //   std::cout << LRL_Cell_Degrees( vLat[lat] ) << std::endl;
   //   std::cout <<                 ( vLat[lat] ) << std::endl;
   //}
   //std::cout << std::endl;

   //for (size_t i = 0; i < vLat.size( ); ++i) {
   //   const MatS6 inertia = InertiaTensor( vLat[i] );
   //}
   if (vLat.size( ) > 0) {
      LRL_LatticeMatcher lm;
      lm.SetReferenceLattice( mv_reducedReference );
      //lm.SetReferenceLattice( MV_Pair( cell_reference, MatS6().unit() ));
      std::cout << "MV tree size = " << lm.size( ) << std::endl;
      std::cout << "before lattice match " << LRL_CreateFileName( ).Create( "", "" ) << std::endl;

      const std::vector<S6> vs6( lm.MatchReference( vLat ) );
      std::cout << "after lattice match " << LRL_CreateFileName( ).Create( "", "" ) << std::endl;

      S6Dist sd( 1 );
      sd.SetDebug( false );


      LMDist lmd( vLat[0] );
      //double ddddd = lmd.DistanceBetween( vLat[1] );

      StoreResults<int, std::string> srlm(5);
      srlm.SetTitle( LRL_ToString( "reference ", vLat[0] ) );
      double dists6, distlm, distcs, disttest;
      for (size_t lat = 1; lat < vLat.size( ); ++lat) {
         //std::cout << vs6[0] << "   " << vs6[lat] << std::endl << std::endl;
         //distlm = (vLat[0] - vs6[lat]).norm( );
         distlm = lmd.DistanceBetween(vLat[lat]);
         //disttest = lm.DistanceBetween( vLat[0], vLat[lat] );
         //std::cout << "disttest " << disttest << std::endl;
         distcs = CS6Dist( vLat[0].data( ), vLat[lat].data( ) );
         dists6 = sd.DistanceBetween( vLat[0], vLat[lat] );

         //std::cout << "lat match " << distlm << "  cs6dist " << distcs << "  s6dist " << dists6 << std::endl;
         double diff = (distlm - dists6) / dists6 * 100.0;
         srlm.Store( int( diff ), LRL_ToString("# ", lat, "   ", vLat[lat] ) );
      }
      srlm.ShowResultsByKeyAscending( );
      //std::cout << std::endl;

      //std::cout << "vLat list" << std::endl;
      //for (size_t lat = 0; lat < vLat.size( ); ++lat) {
      //   std::cout << vLat[lat] << std::endl;
      //}
      //std::cout << std::endl;

      //std::cout << "vs6 list" << std::endl;
      //for (size_t lat = 0; lat < vLat.size( ); ++lat) {
      //   std::cout << ( vs6[lat] ) << std::endl;
      //}
      //std::cout << std::endl;
      //std::cout << "vs6 list" << std::endl;
      //for (size_t lat = 0; lat < vLat.size( ); ++lat) {
      //   std::cout << LRL_Cell_Degrees( vs6[lat] ) << std::endl;
      //}
      //std::cout << std::endl;








      double dist;
      lmd.SetReferenceLattice( MV_Pair(vLat[0], MatS6()) );
      std::cout << LRL_CreateFileName( ).Create( "", "" ) << std::endl;
      for (size_t lat = 0; lat < vLat.size( ); ++lat) {
         dist = lmd.DistanceBetween(vs6[lat]);
      }
      std::cout << LRL_CreateFileName( ).Create( "", "" ) << std::endl;
      for (size_t lat = 0; lat < vLat.size( ); ++lat) {
         dist = (vs6[0] - vs6[lat]).norm( );
      }
      std::cout << LRL_CreateFileName( ).Create( "", "" ) << std::endl;
      for (size_t lat = 0; lat < vLat.size( ); ++lat) {
         dist = (vs6[0] - vs6[lat]).norm( );
      }
      std::cout << LRL_CreateFileName( ).Create("","" ) << std::endl;
      for (size_t lat = 0; lat < vLat.size( ); ++lat) {
         dist = CS6Dist( vLat[0].data( ), vLat[lat].data( ) );
      }
      std::cout << LRL_CreateFileName( ).Create("","" ) << std::endl;
      for (size_t lat = 0; lat < vLat.size( ); ++lat) {
         dist = sd.DistanceBetween( vLat[0], vLat[lat] );
      }
      std::cout << LRL_CreateFileName( ).Create("","" ) << std::endl;
   }

   return 0;
}

/*
p 2 4 4 60 79.2 75.5
p 2 4 4 60 86.45 75.5
p 2 4 4 120 93.51 100.666
p 2 4 4 117.95 93.52 104.5
p 2 4 4 113.95 100.5 104.5
end

g 4 16 16 16.1 3 4
g 4 16 16 16 1 4.1
g 4 16 16  -16.2 -1 -3
g 4 16 16 -15.2 -1 -4.2
g 4 16 16 -13.3 -3 -4
end


p 2 4 4 60 79.2 75.5
p 2 4 4 60 86.45 75.5
p 2 4 4 120 93.51 100.666
p 2 4 4 117.95 93.52 104.5
p 2 4 4 113.95 100.5 104.5
end

g 4 16 16 16.1 3 4
g 4 16 16 16 1 4.1
g 4 16 16  -16.2 -1 -3
g 4 16 16 -15.2 -1 -4.2
g 4 16 16 -13.3 -3 -4
end


f 10 10.001 10.002  89 89 89
f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one 
f 10 10 11  90 90 90
end



f 10 10.001 10.002  90.01 90.02 90.03
f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one
f 10 10 11  90 90 90
end


f 10 10.001 10.002  90 90 90
f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one
f 10 10 11  90 90 90
end


f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one
f 10 10 11  90 90 90
end

f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one
end



f 10 10 10  90 90 90
i 10 10 10  90 90 90
p  10 10 10  90 90 90
f 10 10 10  90 90 90
f 10 10 11  90 90 90
end

c 10 20 30 90 118 90
p   27.14100  11.18034  26.80144     93.90767 158.64514  95.59396
p   27.14100  11.18034  26.80144     93.90767 158.64514  95.59396
p   27.14100  11.18034  26.80144     93.90767 158.64514  95.59396
p   27.14100  11.18034  11.18034    126.86990  95.59396  95.59396
p   27.14100  26.80144  11.18034     93.90767  95.59396 158.64514
p   27.14100  26.80144  11.18034     93.90767  95.59396 158.64514
p   27.14100  11.18034  11.18034    126.86990  95.59396  95.59396
p   27.14100  11.18034  26.80144     93.90767 158.64514  95.59396
p   11.18034  27.14100  26.80144    158.64514  93.90767  95.59396
p   11.18034  27.14100  11.18034     95.59396 126.86990  95.59396
p   11.18034  26.80144  27.14100    158.64514  95.59396  93.90767
p   11.18034  26.80144  11.18034     93.90767 126.86990  93.90767
p   11.18034  11.18034  27.14100     95.59396  95.59396 126.86990
p   11.18034  11.18034  26.80144     93.90767  93.90767 126.86990
p   26.80144  27.14100  11.18034     95.59396  93.90767 158.64514
p   26.80144  27.14100  11.18034     95.59396  93.90767 158.64514
p   26.80144  11.18034  27.14100     95.59396 158.64514  93.90767
p   26.80144  11.18034  11.18034    126.86990  93.90767  93.90767
p   26.80144  11.18034  27.14100     95.59396 158.64514  93.90767
p   26.80144  11.18034  11.18034    126.86990  93.90767  93.90767
p   11.18034  27.14100  11.18034     95.59396 126.86990  95.59396
p   11.18034  27.14100  26.80144    158.64514  93.90767  95.59396
p   11.18034  11.18034  27.14100     95.59396  95.59396 126.86990
p   11.18034  11.18034  26.80144     93.90767  93.90767 126.86990
p   11.18034  26.80144  27.14100    158.64514  95.59396  93.90767
p   11.18034  26.80144  11.18034     93.90767 126.86990  93.90767
p   28.32800  11.18034  26.80144     86.09233 156.81153 107.53456
p   27.14100  28.32800  26.80144    156.81153  21.35486 156.87148
p   27.14100  11.18034  10.00000    116.56505 102.58967  84.40604
p   27.14100  28.32800  10.00000     88.14729 102.58967 156.87148
p   28.32800  11.18034  28.32800    109.28080 138.65723 107.53456
p   10.00000  28.32800  26.80144    156.81153  98.76519  91.85271
p   28.32800  11.18034  11.18034     53.13010 109.28080 107.53456
p   27.14100  28.32800  11.18034    109.28080  84.40604 156.87148
p   27.14100  11.18034  28.32800    109.28080 156.87148  84.40604
p   27.14100  28.32800  28.32800     41.34277 156.87148 156.87148
p   28.32800  11.18034  10.00000    116.56505  91.85271 107.53456
p   28.32800  10.00000  11.18034    116.56505 107.53456  91.85271
p   10.00000  26.80144  11.18034     86.09233 116.56505  98.76519
p   27.14100  10.00000  11.18034    116.56505  84.40604 102.58967
p   27.14100  26.80144  28.32800    156.81153 156.87148  21.35486
p   27.14100  10.00000  28.32800     88.14729 156.87148 102.58967
p   10.00000  26.80144  28.32800    156.81153  91.85271  98.76519
p   28.32800  28.32800  11.18034    109.28080 107.53456 138.65723
p   28.32800  27.14100  26.80144     21.35486 156.81153 156.87148
p   11.18034  28.32800  26.80144    156.81153  86.09233 107.53456
p   11.18034  27.14100  28.32800    156.87148 109.28080  84.40604
p   11.18034  28.32800  28.32800    138.65723 109.28080 107.53456
p   28.32800  27.14100  10.00000    102.58967  88.14729 156.87148
p   28.32800  10.00000  26.80144     98.76519 156.81153  91.85271
p   28.32800  27.14100  11.18034     84.40604 109.28080 156.87148
p   11.18034  28.32800  11.18034    109.28080  53.13010 107.53456
p   11.18034  27.14100  10.00000    102.58967 116.56505  84.40604
p   11.18034  28.32800  10.00000     91.85271 116.56505 107.53456
p   28.32800  27.14100  28.32800    156.87148  41.34277 156.87148
p   10.00000  28.32800  11.18034    107.53456 116.56505  91.85271
p   28.32800  26.80144  27.14100     21.35486 156.87148 156.81153
p   11.18034  28.32800  27.14100    156.87148  84.40604 109.28080
p   11.18034  26.80144  28.32800    156.81153 107.53456  86.09233
p   11.18034  28.32800  28.32800    138.65723 107.53456 109.28080
p   28.32800  26.80144  10.00000     98.76519  91.85271 156.81153
p   28.32800  10.00000  27.14100    102.58967 156.87148  88.14729
p   28.32800  26.80144  11.18034     86.09233 107.53456 156.81153
p   11.18034  28.32800  11.18034    107.53456  53.13010 109.28080
p   11.18034  26.80144  10.00000     98.76519 116.56505  86.09233
p   11.18034  28.32800  10.00000     88.14729 116.56505 109.28080
p   28.32800  26.80144  28.32800    156.81153  41.34277 156.81153
p   10.00000  28.32800  11.18034    109.28080 116.56505  88.14729
p   10.00000  11.18034  27.14100     84.40604 102.58967 116.56505
p   11.18034  10.00000  27.14100    102.58967  84.40604 116.56505
p   11.18034  11.18034  28.32800    109.28080 107.53456  53.13010
p   11.18034  10.00000  28.32800     91.85271 107.53456 116.56505
p   10.00000  11.18034  28.32800    107.53456  91.85271 116.56505
p   28.32800  28.32800  27.14100    156.87148 156.87148  41.34277
p   10.00000  11.18034  26.80144     86.09233  98.76519 116.56505
p   11.18034  10.00000  26.80144     98.76519  86.09233 116.56505
p   11.18034  11.18034  28.32800    107.53456 109.28080  53.13010
p   11.18034  10.00000  28.32800     88.14729 109.28080 116.56505
p   10.00000  11.18034  28.32800    109.28080  88.14729 116.56505
p   28.32800  28.32800  26.80144    156.81153 156.81153  41.34277
p   10.00000  27.14100  11.18034     84.40604 116.56505 102.58967
p   26.80144  10.00000  11.18034    116.56505  86.09233  98.76519
p   26.80144  27.14100  28.32800    156.87148 156.81153  21.35486
p   26.80144  10.00000  28.32800     91.85271 156.81153  98.76519
p   10.00000  27.14100  28.32800    156.87148  88.14729 102.58967
p   28.32800  28.32800  11.18034    107.53456 109.28080 138.65723
p   28.32800  11.18034  27.14100     84.40604 156.87148 109.28080
p   26.80144  28.32800  27.14100    156.87148  21.35486 156.81153
p   26.80144  11.18034  10.00000    116.56505  98.76519  86.09233
p   26.80144  28.32800  10.00000     91.85271  98.76519 156.81153
p   28.32800  11.18034  28.32800    107.53456 138.65723 109.28080
p   10.00000  28.32800  27.14100    156.87148 102.58967  88.14729
p   28.32800  11.18034  11.18034     53.13010 107.53456 109.28080
p   26.80144  28.32800  11.18034    107.53456  86.09233 156.81153
p   26.80144  11.18034  28.32800    107.53456 156.81153  86.09233
p   26.80144  28.32800  28.32800     41.34277 156.81153 156.81153
p   28.32800  11.18034  10.00000    116.56505  88.14729 109.28080
p   28.32800  10.00000  11.18034    116.56505 109.28080  88.14729
end

*/