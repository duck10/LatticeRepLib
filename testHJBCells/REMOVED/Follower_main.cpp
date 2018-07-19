// D7_Follower.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <ctime>
#include <map>
#include <fstream>
#include <iostream>
#include <utility>

#include "Cell.h"
#include "CreateFileName.h"
//#include "D7_BoundaryList.h"
#include "D6.h"
#include "G6.h"
#include "D7.h"
#include "DeloneTetrahedron.h"
#include "FileWriter.h"
#include "Follow.h"
#include "Follower.h"
#include "FollowerConstants.h"
#include "FollowerTools.h"
#include "MapBoundaryStrings2Colors.h"
#include "MaximaTools.h"
#include "ProjectorTools.h"
#include "ReadCellData.h"
#include "ReadGlobalData.h"
#include "Reducer.h"
#include "ReportTools.h"
#include "SuperFastHash.h"
#include "SVG_Writer.h"
#include "G6.h"


template<typename TVEC, typename TMAT>
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool WalkFromOnePointToAnother( const TVEC& v1, const TVEC& v2, const size_t nSteps, const std::string& s_BaseSVG_FileName, MapBoundaryStrings2Colors& colorMap, const size_t goodCellCount )
   /*-------------------------------------------------------------------------------------*/
{
   Follow<TVEC,TMAT>  follow( v1, v2 );
   const std::clock_t start = std::clock();
   follow.SetStepCount( nSteps )
      .SetWhichComponentsToPlot( FollowerConstants::globalWhichComponentsToPlot.first, FollowerConstants::globalWhichComponentsToPlot.second )
      .Procession( );
   follow.SetTime2ComputeFrame(  std::clock() - start );


   // a tiny diagonal means that the probe is essentially reduced before we start
   // just dump those
   if ( follow.GetCirclePlotDiagonal( ) < 1.0E-3 )
   {
      std::cout << "rejected because of tiny diagonal " << follow.GetCirclePlotDiagonal( ) << std::endl;
      return( false );
   }
   else
   {
      //const std::list<double> distances(follow.GetDistances());
      const std::list<double> distances(follow.GetDeloneDistances());

      std::vector<Glitch<TVEC> > glitches;
      glitches = follow.DetermineIfSomeDeltaIsTooLarge( distances ); // return the index for first glitch

      if ( FollowerConstants::globalPrintAllDistanceData || ! glitches.empty( ) )
      {
         FileWriter<TVEC,TMAT>( s_BaseSVG_FileName + ".txt", follow, glitches ).Write();

//#define DisableForHJB
#ifndef DisableForHJB
   //Create a file name for each different SVG file
         const std::string s_SVG_FileName = ToString( s_BaseSVG_FileName, goodCellCount, ".svg" );
         const std::vector<triple<double,double,double> > angles( follow.GetLinesFromAngles( ) );
         const std::vector<int> rejectedFrames;
         SVG_Writer<TVEC, TMAT>( s_SVG_FileName, follow, 0, colorMap, glitches, rejectedFrames, 0 );
         if ( FollowerConstants::globalFollowerMode == FollowerConstants::globalWeb )
            exit( 19191 );
#endif
      }
   }

   return( true );
}

template<typename TVEC, typename TMAT>
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
size_t ProbeOneRegion( const CellInputData& probe, const size_t nSteps, const std::string& sBaseFileName )
/*-------------------------------------------------------------------------------------*/
{
   G6 vR;
   MapBoundaryStrings2Colors colorMap;
   Mat66 m;
   size_t goodCellCount = 0;

   for ( int iloop=0; iloop<FollowerConstants::globalNumberOfTrialsToAttempt; ++iloop )
   {
      const G6 v1( ReadGlobalData::GeneratePerturbation( G6(probe.GetCell().Cell2V6()) ) );
      m.Eye();
      //const bool bTestReduce(Delone::Delone(v1, m, vR, 0.0));  lca Delone code
      const bool bTestReduce(Reducer::Reduce(v1, m, vR, 0.0));

      if ( bTestReduce && ! (m==Mat66().Eye()) )
      {
         const bool bTestWalk = WalkFromOnePointToAnother<TVEC,TMAT>( TVEC(v1), TVEC(vR), nSteps, sBaseFileName, colorMap, goodCellCount );
         if ( bTestWalk ) ++goodCellCount;
      } else if( ! bTestReduce ) {
         //std::cout << "Delone failed " << v1 << std::endl;
         std::cout << "Reduce failed " << v1 << std::endl;
      }
   }

   return( goodCellCount );
}

template<typename TVEC, typename TMAT>
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int ProbeOneRegion( const G6& probe, const int nSteps, const std::string& sBaseFileName )
/*-------------------------------------------------------------------------------------*/
{
   G6 vR;
   MapBoundaryStrings2Colors colorMap;
   Mat66 m;
   long goodCellCount = 0;

   for ( int iloop=0; iloop<FollowerConstants::globalNumberOfTrialsToAttempt; ++iloop )
   {
      const G6 v1( ReadGlobalData::GeneratePerturbation( probe ) );
      m.Eye();
      //const bool bTestReduce( Delone::Delone( v1, m, vR, 0.0 ) );
      const bool bTestReduce(Reducer::Reduce(v1, m, vR, 0.0));

      if ( bTestReduce && ! (m==Mat66().Eye()) )
      {
         const bool bTestWalk = WalkFromOnePointToAnother<TVEC,TMAT>( v1, vR, nSteps, sBaseFileName, colorMap, goodCellCount );
         if ( bTestWalk ) ++goodCellCount;
      } else if( ! bTestReduce ) {
         //std::cout << "Delone failed " << v1 << std::endl;
         std::cout << "Reduce failed " << v1 << std::endl;
      }
   }

   return( goodCellCount );
}



//
//void TransformAndTestVector( const D7& testvec, const D7_Subboundary& sb ) {
//   const D7 transformedRandom = sb.GetTransform( ) * testvec;
//   const std::pair<int, std::string> id = D7_IdentifyNearbyBoundaries( transformedRandom, 10 );
//   boundarymap[sb.GetName( )] = std::make_pair( sb.GetName( ) + " transforms to  " + id.second, "" );
//}

template<typename T>
void TestToType() {
   T a, b;
   if (a.size() == 6) {
      a = T("10 11 12 90 90 90");          // from string
      b = T(G6("10 10 10  10 10 10 "));     // from G6
   }
   else if (a.size() == 7)
   {
      a = T("10 11 12 13 14 15 16");          // from string
      b = T("10 10 10  60 30 30 30 ");     // from G6
   }
   else if (a.size() == 4)
   {
      a = T(Cell("10 11 12 90 90 90"));
      b = T(G6("10 10 10  10 10 10 "));
   }
   else
   {
      throw;
   }
   D7 d7(b);
   T c(d7);                           // from D7
   T d(c);                            // from Cell
   D6 ds("5 5 5 -20 -20 -20");
   T e(ds);                           // from DeloneScalars (D6)
   DeloneTetrahedron dt(G6("10 10 10  10 10 10 "));
   T f(dt);

   std::cout << a << std::endl;
   std::cout << b << std::endl;
   std::cout << c << std::endl;
   std::cout << d << std::endl;
   std::cout << e << std::endl;
   std::cout << f << std::endl;

   std::cout << std::endl;

   std::cout << ToString(a) << std::endl;
   std::cout << ToString(b) << std::endl;
   std::cout << ToString(c) << std::endl;
   std::cout << ToString(d) << std::endl;
   std::cout << ToString(e) << std::endl;
   std::cout << ToString(f) << std::endl;

}


int main( int argc, char* argv [ ] ) {

   //Cell c;
   //c[0];
   //std::cout << "FOR CELL " << std::endl;
   //TestToType<Cell>();

   //std::cout << std::endl << std::endl;
   //std::cout << "FOR G6 " << std::endl;
   //TestToType<G6>();

   //std::cout << std::endl << std::endl;
   //std::cout << "FOR D7 " << std::endl;
   //TestToType<D7>();

   //std::cout << std::endl << std::endl;
   //std::cout << "FOR DeloneScalars " << std::endl;
   //TestToType<D6>();

   //std::cout << std::endl << std::endl;
   //std::cout << "FOR DeloneTetrahedron " << std::endl;
   //TestToType<DeloneTetrahedron>();

   //D7_BoundaryList bl;

   ReadGlobalData();
   
   std::vector<CellInputData> celldata;
   std::string lattice( "" );
   while ( lattice != "EOF" ) {
      ReadCellData rcd;
      lattice = rcd.GetLattice( );
      if ( lattice != "EOF" && ! lattice.empty() ) {
         rcd.SetVarietyRange( std::make_pair( 0, 23 ) );
         celldata.push_back( rcd );
      }
   }




   //for ( unsigned int icell=0; icell<celldata.size( ); ++icell ) {
   size_t cellcount = 0;
      std::vector<CellInputData>::const_iterator it = celldata.begin( );
      for ( ; it!=celldata.end( ); ++it ) {
         const std::string sBaseFileName =
            ToString( CreateFileName::Create( FollowerConstants::globalFileNamePrefix, ToString( cellcount) + "_" ) );
         ++cellcount;
         size_t goodCellCount = ProbeOneRegion<D7,Mat7>( *it, FollowerConstants::globalStepsPerFrame, sBaseFileName );
      }
   //}


   return 0;
}

