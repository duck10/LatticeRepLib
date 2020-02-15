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

std::string NameOneFileForOneLattice(const size_t cellNumber) {
   return
      LRL_ToString(LRL_CreateFileName::Create(FollowerConstants::globalFileNamePrefix, LRL_ToString(cellNumber) + "_",
         GLOBAL_Files::globalShouldTimeStamp));
}

S6 InvalidPoint( void ) {
   S6 s6(" 0 0 0   0 0 0");
   s6.SetValid(false);
   return s6;
}

std::vector<std::pair<S6, S6> > GenerateS6LineFromStartToCell3ForModeLine3(const CellInputData& cell1, const CellInputData& cell2, const CellInputData& cell3) {
   const S6 probe1 = cell1.GetCell();
   const S6 probe2 = cell2.GetCell();
   const S6 focus3 = cell3.GetCell();
   const S6 midpoint = (probe1 + probe2) / 2.0;
   const S6 delta1 = probe1 - midpoint;
   const S6 delta2 = probe2 - midpoint;
   std::vector<std::pair<S6, S6> > points1;
   S6 reduced1;
   S6 reduced2;

   for (size_t step = 0; step<FollowerConstants::globalStepsPerFrame; ++step) {
      const double t(double(step) / (FollowerConstants::globalStepsPerFrame - 1));
      const S6 nextMid = (1.0 - t)*midpoint + t * focus3;
      const S6 next1 = nextMid + delta1;
      const S6 next2 = nextMid + delta2;

      const bool b1 = Selling::Reduce(next1, reduced1);
      if (!b1 || !reduced1.IsAllMinus() || !reduced1.IsValid()) reduced1 = InvalidPoint();
      points1.push_back(std::make_pair(next1, reduced1));

      const bool b2 = Selling::Reduce(next2, reduced2);
      const bool b1s = next1.IsValid();
      const bool b1r = reduced1.IsValid();
      const bool b2s = next2.IsValid();
      const bool b2r = reduced2.IsValid();

      if (!b2 || !reduced2.IsAllMinus() || !reduced2.IsValid()) reduced2 = InvalidPoint();

   }
   return points1;
}

std::vector<std::pair<S6, S6> > GenerateS6LineFromStartToCell3ForModeTriangle(const CellInputData& cell1, const CellInputData& cell2, const CellInputData& cell3) {
   const S6 probe1 = cell1.GetCell();
   const S6 probe2 = cell2.GetCell();
   const S6 focus3 = cell3.GetCell();

   const S6 delta1 = (probe1 - focus3)/double(FollowerConstants::globalStepsPerFrame);
   const S6 delta2 = (probe2 - focus3)/double(FollowerConstants::globalStepsPerFrame);
   std::vector<std::pair<S6, S6> > points1;
   S6 reduced1;
   S6 reduced2;
   S6 reducedFocus;

   const bool bfocus = Selling::Reduce(focus3, reducedFocus);

   for (size_t step = 1; step<FollowerConstants::globalStepsPerFrame+1; ++step) {
      const double nonZeroStep = std::max(1.0E-3, double(step));
      const double t(nonZeroStep / (FollowerConstants::globalStepsPerFrame - 1));

      const S6 next1 = (1.0 - t) * probe1 + t * focus3;
      const S6 next2 = (1.0 - t) * probe2 + t * focus3;

      const bool b1 = Selling::Reduce(next1, reduced1);
      if (!b1 || !reduced1.IsAllMinus() || !LRL_Cell(reduced1).IsValid()) reduced1 = InvalidPoint();
      points1.push_back(std::make_pair(next1, reduced1));

      const bool b2 = Selling::Reduce(next2, reduced2);
      if (!b2 || !reduced2.IsAllMinus() || !LRL_Cell(reduced1).IsValid()) reduced2 = InvalidPoint();

   }
   return points1;
}

std::vector<std::pair<S6,S6> > GenerateS6LineFromStartToFinish(const CellInputData& cell) {
   std::vector<std::pair<S6, S6> > points;
   const S6 probe = cell.GetCell();
   S6 reducedProbe;
   S6 reduced;
   const bool b = Selling::Reduce(probe, reducedProbe);
   if (b) {
      for (size_t step = 0; step < FollowerConstants::globalStepsPerFrame; ++step) {
         const double t(double(step) / (FollowerConstants::globalStepsPerFrame - 1));
         S6 next = (1.0 - t)*probe + t * reducedProbe;
         if (!next.IsValid()) next = InvalidPoint();
         const bool b2 = Selling::Reduce( next, reduced );
         if (!reduced.IsValid()) reduced = InvalidPoint();
         points.push_back(std::make_pair(next, reduced));
      }
   }
   return points;
}

MultiFollower ProcessOneLattice(const size_t inputCellOrdinal, const size_t plotCounter, const CellInputData& cell1,
   const CellInputData& cell2, const CellInputData& cell3) {
   const std::string baseFileName = NameOneFileForOneLattice(inputCellOrdinal) + LRL_ToString(plotCounter);

   std::vector<std::pair<S6, S6> > points1;

   if (FollowerConstants::globalFollowerMode == FollowerConstants::globaltriangle) {
      points1 = GenerateS6LineFromStartToCell3ForModeTriangle( cell1, cell2, cell3 );
   }

   else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine3) {
      points1 = GenerateS6LineFromStartToCell3ForModeLine3(cell1, cell2, cell3);
   }
   else
   {
      points1 = GenerateS6LineFromStartToFinish(cell1);
   }

   MultiFollower mf(points1);
   mf = mf.GenerateAllDistances();
   mf.SetTime2ComputeFrame(double(std::clock() - mf.GetComputeStartTime()));

   if (!mf.HasGlitches() && FollowerConstants::globalOutputGlitchesOnly) return mf;
   SVG_DistancePlot<S6> distanceplot(mf, baseFileName);
   std::cout << baseFileName << std::endl;

   return mf;
}

template<typename T>
void PrintPoints(const std::vector<std::pair<T,T> >& v) {
   for (size_t i = 0; i<v.size(); ++i) {
      std::cout << v[i].first << "   " << v[i].second << std::endl;
   }
   std::cout << std::endl;
}

template<typename T>
void PrintPoints(const std::vector<T>& v) {
   for (size_t i = 0; i<v.size(); ++i) {
      std::cout << v[i] << std::endl;
   }
   std::cout << std::endl;
}

void GetCellsForChosenMode(const std::vector<CellInputData>& inputCellList, const size_t nextCell,
   const FollowerConstants::enumFollowerMode& mode,
   CellInputData& cell1, CellInputData& cell2, CellInputData& cell3) {

   size_t returnedNextCell = nextCell;

   const size_t secondCell = nextCell + 1;
   const size_t thirdCell = secondCell + 1;

   cell1.SetCell(LRL_Cell(0, 0, 0, 0, 0, 0));
   cell2.SetCell(LRL_Cell(0, 0, 0, 0, 0, 0));
   cell3.SetCell(LRL_Cell(0, 0, 0, 0, 0, 0));
   if (nextCell < inputCellList.size()) {
      cell1 = inputCellList[nextCell];
      ++returnedNextCell;
   }
   if (secondCell < inputCellList.size()) {
      cell2 = inputCellList[secondCell];
      ++returnedNextCell;
   }
   if (thirdCell < inputCellList.size()) {
      cell3 = inputCellList[thirdCell];
      ++returnedNextCell;
   }
}

void ProcessTrialsForOneLattice(const std::vector<CellInputData>& inputCellList, const size_t nextCell, const size_t cellcount,
   const FollowerConstants::enumFollowerMode& mode,
   CellInputData& cell1, CellInputData& cell2, CellInputData& cell3) {

   GetCellsForChosenMode(inputCellList, nextCell, mode, cell1, cell2, cell3);

   CellInputData cell1Perturbed;
   CellInputData cell2Perturbed;
   CellInputData cell3Perturbed;

   for (size_t trialNo = 0; trialNo < std::max(size_t(1), FollowerConstants::globalNumberOfTrialsToAttempt); ++trialNo) {
      cell1Perturbed.SetCell(ReadGlobalData::GeneratePerturbation(G6(cell1.GetCell())));
      cell2Perturbed.SetCell(ReadGlobalData::GeneratePerturbation(G6(cell2.GetCell())));
      cell3Perturbed.SetCell(ReadGlobalData::GeneratePerturbation(G6(cell3.GetCell())));
      const MultiFollower mf = ProcessOneLattice(cellcount, trialNo, cell1Perturbed, cell2Perturbed, cell3Perturbed);
   }
}

int main(int argc, char* argv[]) {
   ReadGlobalData();

   std::vector<CellInputData> celldata = ReadAllLatticeData();;

   size_t cellcount = 0;
   std::vector<CellInputData>::const_iterator it = celldata.begin();
   CellInputData cell1;
   CellInputData cell2;
   CellInputData cell3;
   for (size_t i = 0; i < celldata.size(); ++i) {

      // are we done with input?
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine &&
         i + size_t(1) >= celldata.size()) {
         std::cout << "FOLLOWERMODE LINE requires at least two cells input" << std::endl;
         break;
      }

      if ((FollowerConstants::globalFollowerMode == FollowerConstants::globalLine3 || FollowerConstants::globalFollowerMode == FollowerConstants::globaltriangle) &&
         i + size_t(2) >= celldata.size()) {
         std::cout << "FOLLOWERMODE TRIANGLE requires at least three cells input" << std::endl;
         break;
      }

      ProcessTrialsForOneLattice(celldata, i, cellcount,
         FollowerConstants::globalFollowerMode,
         cell1, cell2, cell3);
      ++cellcount;

      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine)
      {
         ++i;
      }
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine3) {
         ++i;
         ++i;
      }
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globaltriangle) {
         ++i;
         ++i;
      }
   }
   //LRL_LatticeMatcher::record.ShowResults( );

   return 0;

}

