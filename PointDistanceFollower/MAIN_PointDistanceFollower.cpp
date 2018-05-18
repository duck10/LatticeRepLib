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

std::pair<std::vector<std::pair<S6, S6> >, std::vector<std::pair<S6, S6> > > GenerateS6LineFromStartToCell3(const CellInputData& cell1, const CellInputData& cell2, const CellInputData& cell3) {
   const S6 probe1 = cell1.GetCell();
   const S6 probe2 = cell2.GetCell();
   const S6 focus3 = cell3.GetCell();
   const S6 midpoint = (probe1 + probe2) / 2.0;
   const S6 delta1 = probe1 - midpoint;
   const S6 delta2 = probe2 - midpoint;
   std::vector<std::pair<S6, S6> > points1;
   std::vector<std::pair<S6, S6> > points2;
   S6 reduced1;
   S6 reduced2;

   for (unsigned long step = 0; step<FollowerConstants::globalStepsPerFrame; ++step) {
      const double t(double(step) / (FollowerConstants::globalStepsPerFrame - 1));
      const S6 nextMid = (1.0 - t)*midpoint + t * focus3;
      const S6 next1 = nextMid + delta1;
      const S6 next2 = nextMid + delta2;

      const bool b1 = Selling::Reduce(next1, reduced1);
      if (!reduced1.IsAllMinus()) reduced1 *= 0;
      points1.push_back(std::make_pair(next1, reduced1));

      const bool b2 = Selling::Reduce(next2, reduced2);
      if ( !reduced2.IsAllMinus()) reduced2 *= 0;
      points2.push_back(std::make_pair(next2, reduced2));

   }
   return std::make_pair(points1, points2);
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

MultiFollower ProcessOneLattice(const unsigned long cellNumber, const unsigned long plotCounter, const CellInputData& cell,
   const CellInputData& cell2, const CellInputData& cell3) {
   const std::string baseFileName = NameOneFileForOneLattice(cellNumber) + LRL_ToString(plotCounter);

   std::vector<std::pair<S6, S6> > points1;
   std::vector<std::pair<S6, S6> > points2;
   if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine3) {
      const std::pair<std::vector<std::pair<S6, S6> >, std::vector<std::pair<S6, S6> >> p = GenerateS6LineFromStartToCell3(cell, cell2, cell3);
      points1 = p.first;
      points2 = p.second;
   }
   else
   {
      points1 = GenerateS6LineFromStartToFinish(cell);
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine) {
         points2 = GenerateS6LineFromStartToFinish(cell2);
      }
   }

   MultiFollower mf(points1, points2);
   mf = mf.GenerateAllDistances();

   mf.SetTime2ComputeFrame(std::clock() - mf.GetComputeStartTime());
   if (!mf.HasGlitches() && FollowerConstants::globalOutputGlitchesOnly) return mf;
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
      std::vector<CellInputData>::const_iterator ittemp = it;
      for (unsigned long trialNo = 0; trialNo < std::max(1UL, FollowerConstants::globalNumberOfTrialsToAttempt); ++trialNo) {
         CellInputData cell(*ittemp);
         CellInputData cell2(cell);
         CellInputData cell3;
         std::vector<CellInputData>::const_iterator it2;
         std::vector<CellInputData>::const_iterator it3;
         if (trialNo > 0 || FollowerConstants::globalFollowerMode != FollowerConstants::globalSinglePoint) {
            cell.SetCell(LRL_Cell(ReadGlobalData::GeneratePerturbation(G6((*ittemp).GetCell()))));
            it2 = ittemp + 1;
            if ((FollowerConstants::globalFollowerMode == FollowerConstants::globalLine ||
               FollowerConstants::globalFollowerMode == FollowerConstants::globalLine3) && (it2 != celldata.end())) {
               ++ittemp;
               cell2.SetCell(LRL_Cell(ReadGlobalData::GeneratePerturbation(G6((*ittemp).GetCell()))));
               it3 = it2 + 1;
               if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine3 && (it3 != celldata.end())) {
                  ++ittemp;
                  cell3.SetCell(LRL_Cell(ReadGlobalData::GeneratePerturbation(G6((*ittemp).GetCell()))));
               }
            }
         }
         const MultiFollower mf = ProcessOneLattice(cellcount, plotCounter, cell, cell2, cell3);
         ++plotCounter;
      }
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine)
      {
         ++it;
         if (it == celldata.end()) break;
      }
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine3) {
         ++it;
         if (it == celldata.end()) break;
         ++it;
         if (it == celldata.end()) break;
      }
   }

   return 0;

}

