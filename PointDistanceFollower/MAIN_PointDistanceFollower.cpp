// D7_Follower.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <ctime>
#include <map>
#include <fstream>
#include <iostream>
#include <set>
#include <utility>

#include "LRL_Cell.h"
#include "LRL_CreateFileName.h"
#include "S6.h"
#include "G6.h"
#include "D7.h"
#include "B4.h"
//#include "FileWriter.h"
//#include "Follow.h"
//#include "Follower.h"
#include "FollowerConstants.h"
#include "GenerateRandomLattice.h"
//#include "MapBoundaryStrings2Colors.h"
#include "MatG6.h"
#include "LRL_MaximaTools.h"
#include "LRL_Path.h"
#include "LRL_ReadLatticeData.h"
#include "MultiFollower.h"
#include "Niggli.h"
#include "OutlierFinder.h"
#include "ReadGlobalData.h"
#include "Selling.h"
#include "SVG_DistancePlot.h"
//#include "SVG_Writer.h"

typedef Selling TREDUCE;

//template<typename TVEC, typename TMAT, typename TREDUCE>
///*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//bool WalkFromOnePointToAnother( const TVEC& v1, const TVEC& v2, const unsigned long nSteps, const std::string& s_BaseSVG_FileName, MapBoundaryStrings2Colors& colorMap, const unsigned long goodCellCount )
//   /*-------------------------------------------------------------------------------------*/
//{
//   Follow<TVEC,TMAT,TREDUCE>  follow( v1, v2 );
//   const std::clock_t start = std::clock();
//   follow.SetStepCount( nSteps )
//      .SetWhichComponentsToPlot(FollowerConstants::globalWhichComponentsToPlot.first, FollowerConstants::globalWhichComponentsToPlot.second )
//      .Procession( );
//   follow.SetTime2ComputeFrame(  std::clock() - start );
//
//
//   // a tiny diagonal means that the probe is essentially reduced before we start
//   // just dump those
//   if (FollowerConstants::globalDrawCirclePlot && follow.GetCirclePlotDiagonal( ) < 1.0E-3 )
//   {
//      std::cout << "rejected because of tiny diagonal " << follow.GetCirclePlotDiagonal( ) << std::endl;
//      return( false );
//   }
//   else
//   {
//      const std::list<double> distances(follow.GetDistances());
//      //const std::list<double> distances(follow.GetDeloneDistances());
//
//      std::vector<Glitch<TVEC> > glitches;
//      glitches = follow.DetermineIfSomeDeltaIsTooLarge( distances ); // return the index for first glitch
//
//      if (FollowerConstants::globalPrintAllDistanceData) {
//         if (glitches.empty() && FollowerConstants::globalOutputGlitchesOnly) {
//         }
//         else
//         {
//            FileWriter<TVEC, TMAT, TREDUCE>(s_BaseSVG_FileName + ".txt", follow, glitches).Write();
//
//            //#define DisableForHJB
//#ifndef DisableForHJB
//   //Create a file name for each different SVG file
//            const std::string s_SVG_FileName = LRL_ToString(s_BaseSVG_FileName, goodCellCount, ".svg");
//            const std::vector<triple<double, double, double> > angles(follow.GetLinesFromAngles());
//            const std::vector<int> rejectedFrames;
//            SVG_Writer<TVEC, TMAT, TREDUCE>(s_SVG_FileName, follow, 0, colorMap, glitches, rejectedFrames, 0);
//            if (FollowerConstants::globalFollowerMode == FollowerConstants::globalWeb)
//               exit(19191);
//#endif
//         }
//      }
//   }
//
//   return( true );
//}
//
//template<typename TVEC, typename TMAT>
///*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//unsigned long ProbeOneRegion(const CellInputData& probe, const unsigned long nSteps, const std::string& sBaseFileName)
///*-------------------------------------------------------------------------------------*/
//{
//   S6 vR;
//   MapBoundaryStrings2Colors colorMap;
//   MatS6 m;
//   unsigned long goodCellCount = 0;
//
//   for (unsigned long iloop = 0; iloop<FollowerConstants::globalNumberOfTrialsToAttempt; ++iloop)
//   {
//      const G6 v1(ReadGlobalData::GeneratePerturbation(G6(probe.GetCell())));
//      m.Eye();
//      //const bool bTestReduce(Delone::Delone(v1, m, vR, 0.0));  lca Delone code
//      const bool bTestReduce(TREDUCE::Reduce(S6(v1), m, vR, 0.0));
//
//      const bool beye = (m == MatG6().Eye());
//      if (bTestReduce && !(m == MatG6().Eye()))
//      {
//         const bool bTestWalk = WalkFromOnePointToAnother<TVEC, TMAT, TREDUCE>(TVEC(v1), TVEC(vR), nSteps, sBaseFileName, colorMap, goodCellCount);
//         if (bTestWalk) ++goodCellCount;
//      }
//      else if (!bTestReduce) {
//         //std::cout << "Delone failed " << v1 << std::endl;
//         std::cout << "Reduce failed " << v1 << std::endl;
//      }
//   }
//
//   return(goodCellCount);
//}
//
//template<typename TVEC, typename TMAT>
///*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//unsigned long ProbeOneRegion(const TVEC& probe1, const TVEC& probe2,
//   const unsigned long nSteps, const std::string& sBaseFileName)
///*-------------------------------------------------------------------------------------*/
//{
//   S6 vR;
//   MapBoundaryStrings2Colors colorMap;
//   MatS6 m;
//   unsigned long goodCellCount = 0;
//
//   for (unsigned long iloop = 0; iloop<FollowerConstants::globalNumberOfTrialsToAttempt; ++iloop)
//   {
//         const bool bTestWalk = WalkFromOnePointToAnother<TVEC, TMAT, TREDUCE>(probe1, probe2, nSteps, sBaseFileName, colorMap, goodCellCount);
//         if (bTestWalk) ++goodCellCount;
//   }
//
//   return(goodCellCount);
//}

std::vector<CellInputData> ReadAllLatticeData() {
   std::vector<CellInputData> celldata;
   std::string lattice("");
   LRL_ReadLatticeData rcd(GLOBAL_RunInputVector::globalInputRandomSeed);
   while (lattice != "EOF") {
      rcd.read();
      lattice = "EOF";
      lattice = rcd.GetLattice();
      if (lattice != "EOF" && !lattice.empty()) {
         rcd.SetVarietyRange(std::make_pair(0, 23));
         celldata.push_back(rcd);
      }
   }
   return celldata;
}

std::string NameOneFileForOneLattice(const unsigned long cellNumber) {
   return
      LRL_ToString(LRL_CreateFileName::Create(FollowerConstants::globalFileNamePrefix, LRL_ToString(cellNumber) + "_",
         GLOBAL_Files::globalShouldTimeStamp));
}

std::vector<std::pair<S6,S6> > GenerateS6LineFromStartToFinish(const CellInputData& cell) {
   std::vector<std::pair<S6, S6> > points;
   const S6 probe = cell.GetCell();
   S6 reducedProbe;
   S6 reduced;
   const bool b = Selling::Reduce(probe, reducedProbe);
   for ( unsigned long step=0; step<FollowerConstants::globalStepsPerFrame; ++step) {
      const double t(double(step) / (FollowerConstants::globalStepsPerFrame-1));
      const S6 next = (1.0 - t)*probe + t * reducedProbe;
      const bool b = Selling::Reduce(next, reduced);
         points.push_back(std::make_pair(next, reduced));
      }
   return points;
}

MultiFollower ProcessOneLattice(const unsigned long cellNumber, const unsigned long plotCounter, const CellInputData& cell) {
   const std::string baseFileName = NameOneFileForOneLattice(cellNumber) + LRL_ToString(plotCounter);
   const std::vector<std::pair<S6, S6> > points = GenerateS6LineFromStartToFinish(cell);

   MultiFollower mf(points);
   mf = mf.GenerateAllDistances();

   mf.SetTime2ComputeFrame(mf.GetComputeStartTime() - std::clock());
   SVG_DistancePlot<S6> distanceplot(mf, baseFileName);
   std::cout << baseFileName << std::endl;

   return mf;
}

template<typename T>
void PrintPoints(const std::vector<std::pair<T,T> >& v) {
   for (unsigned long i = 0; i<v.size(); ++i) {
      std::cout << v[i].first << "   " << v[i].second << std::endl;
   }
   std::cout << std::endl;
}

template<typename T>
void PrintPoints(const std::vector<T>& v) {
   for (unsigned long i = 0; i<v.size(); ++i) {
      std::cout << v[i] << std::endl;
   }
   std::cout << std::endl;
}

int main(int argc, char* argv[]) {
   ReadGlobalData();

   std::vector<CellInputData> celldata = ReadAllLatticeData();;

   unsigned long cellcount = 0;
   unsigned long plotCounter = 0;
   std::vector<CellInputData>::const_iterator it = celldata.begin();
   for (; it != celldata.end(); ++it) {
      ++cellcount;
      for (unsigned long trialNo = 0; trialNo < std::max(1UL, FollowerConstants::globalNumberOfTrialsToAttempt); ++trialNo) {
         CellInputData cell(*it);
         if (trialNo > 0) {
            cell.SetCell(LRL_Cell(ReadGlobalData::GeneratePerturbation(G6((*it).GetCell()))));
         }
         const MultiFollower mf = ProcessOneLattice(cellcount, plotCounter, cell);
         ++plotCounter;
      }
   }

   return 0;
}

