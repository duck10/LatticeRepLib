// D7_Follower.cpp : Defines the entry point for the console application.
//
#pragma warning(disble: 4189) // unused variables

#include <algorithm>
#include <ctime>
#include <map>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <utility>

#include "LRL_Cell.h"
#include "LRL_CreateFileName.h"
#include "S6.h"
#include "G6.h"
#include "D7.h"
#include "DC.h"
#include "B4.h"
//#include "FileWriter.h"
//#include "Follow.h"
//#include "Follower.h"
#include "FollowerConstants.h"
#include "FollowerPathGenerator.h"
#include "GenerateRandomLattice.h"
//#include "MapBoundaryStrings2Colors.h"
#include "MatG6.h"
#include "LRL_Path.h"
#include "LRL_ReadLatticeData.h"
#include "MultiFollower.h"
#include "Niggli.h"
#include "ReadGlobalData.h"
#include "Selling.h"
#include "SVG_DistancePlot.h"

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

std::vector<std::pair<S6, S6> > GenerateS6LineFromStartToFinish(const CellInputData& cell) {
   std::vector<std::pair<S6, S6> > points;
   const S6 probe = cell.GetCell();
   S6 reducedProbe;
   S6 reduced;
   const bool b = Selling::Reduce(probe, reducedProbe);
   if (b) {
      for (size_t step = 0; step < FollowerConstants::globalStepsPerFrame; ++step) {
         const double t(double(step) / (FollowerConstants::globalStepsPerFrame - 1));
         S6 next = (1.0 - t) * probe + t * reducedProbe;
         if (!next.IsValid()) next = InvalidPoint();
         const bool b2 = Selling::Reduce(next, reduced);
         points.push_back(std::make_pair(next, reduced));
      }
   }
   return points;
}

template<typename Tin, typename Tout, typename R>
std::vector<std::pair<Tout, Tout> > ReducePath(const std::vector<std::pair<Tin, Tin> >& pin) {
   std::vector<std::pair<Tout, Tout> > out;
   Tin tout1;
   Tin tout2;
   for (size_t i = 0; i < pin.size(); ++i) {
      const bool b1 = R::Reduce(pin[i].first, tout1);
      const bool b2 = R::Reduce(pin[i].second, tout2);
      out.push_back(std::make_pair(Tout(tout1), Tout(tout2)));
   }
   return out;
}

std::pair<std::string, std::string> GetLineEnds(const std::vector<std::pair<S6, S6> > v) {
   const size_t n = v.size();
   const S6& s1f = v[0].first;
   const S6& s2f = v[0].second;
   const S6& s1s = v[n - 1].first;
   const S6& s2s = v[n - 1].second;
   const std::string s1 = "start, line1 " + LRL_ToString(LRL_Cell_Degrees(s1s), "\n  end, line1, ", LRL_Cell_Degrees(s1f), "\n");
   const std::string s2 = "start, line2 " + LRL_ToString(LRL_Cell_Degrees(s2s), "\n  end, line2, ", LRL_Cell_Degrees(s2f), "\n");
   return std::make_pair(s1, s2);
}

std::string GetInputCells(const size_t numberInputCells, const CellInputData& cell1, const CellInputData& cell2, const CellInputData& cell3) {
   std::string toReturn = cell1.GetStrCell() + "\n";
   if (numberInputCells == 2) toReturn += cell2.GetStrCell() + "\n";
   if (numberInputCells == 3) toReturn += cell3.GetStrCell() + "\n";
   return toReturn;
}

MultiFollower SetMultiFollower(const std::string& baseFileName, const std::unique_ptr<FollowerPathGenerator>& fpg) {
   MultiFollower mf;
   const std::vector<std::pair<S6, S6> > path = fpg->GetPath();
   mf.SetSellingPath(ReducePath<S6, S6, Selling>(path));
   const std::vector< std::pair<S6, S6> > sellingReducedPath(mf.GetSellingReducedPath());
   const std::vector< std::pair<G6, G6> > niggliReducedPath(ReducePath<S6, G6, Niggli>(sellingReducedPath));
   mf.SetNiggliPath(niggliReducedPath);

   std::vector<std::pair<DC, DC> > DCpath;
   if (FollowerConstants::IsEnabled("DC")) {
      for (size_t i = 0; i < niggliReducedPath.size(); ++i)
         DCpath.push_back(std::make_pair(DC(niggliReducedPath[i].first), DC(niggliReducedPath[i].second)));
   }

   mf.SetDCPath(DCpath);
   mf.SetInputVectors(fpg->GetInput());

   mf = mf.GenerateAllDistances();
   mf.SetTime2ComputeFrame(double(std::clock() - mf.GetComputeStartTime()));

   if (!mf.HasGlitches() && FollowerConstants::globalOutputGlitchesOnly) return mf;
   return mf;
}

MultiFollower ProcessOneLattice(const size_t inputCellOrdinal, const size_t plotCounter, const CellInputData& cell1,
   const CellInputData& cell2, const CellInputData& cell3) {
   const std::string baseFileName = NameOneFileForOneLattice(inputCellOrdinal) + LRL_ToString(plotCounter);

   const std::unique_ptr<FollowerPathGenerator>fpg = 
      FollowerPathGenerator::PointerToFollowerPathType(cell1, cell2, cell3);

   const std::string inputCells = GetInputCells(fpg->GetNumberOfInputCells(), cell1, cell2, cell3);
   const std::string firstAndLastPoints = fpg->GetFirstAndLastPointsAsString();

   GLOBAL_Report::globalDataReport +=
      "\nINPUT   " + baseFileName + "   " + fpg->GetName() +
      "\n" + cell1.GetStrCell() + "\n" + inputCells +
      "  Path starts and ends \n" + firstAndLastPoints;

   std::cout << "#  " << baseFileName << std::endl;

   MultiFollower mf(SetMultiFollower( baseFileName, fpg));
   SVG_DistancePlot<S6> distanceplot(mf, baseFileName);

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

   CellInputData cell1Perturbed(cell1);
   CellInputData cell2Perturbed(cell2);
   CellInputData cell3Perturbed(cell3);

   for (size_t trialNo = 0; trialNo < std::max(size_t(1), FollowerConstants::globalNumberOfTrialsToAttempt); ++trialNo) {
      cell1Perturbed.SetCell(ReadGlobalData::GeneratePerturbation(G6(cell1.GetCell())));
      cell2Perturbed.SetCell(ReadGlobalData::GeneratePerturbation(G6(cell2.GetCell())));
      cell3Perturbed.SetCell(ReadGlobalData::GeneratePerturbation(G6(cell3.GetCell())));
      const MultiFollower mf = ProcessOneLattice(cellcount, trialNo, cell1Perturbed, cell2Perturbed, cell3Perturbed);
   }
}

int main(int argc, char* argv[]) {
   //LRL_LatticeMatcher junk;
   ReadGlobalData();

   std::vector<CellInputData> celldata = ReadAllLatticeData();;

   size_t cellcount = 0;
   std::vector<CellInputData>::const_iterator it = celldata.begin();
   CellInputData cell1;
   CellInputData cell2;
   CellInputData cell3;
   for (size_t i = 0; i < celldata.size(); ++i) {
      // are we done with input?
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord &&
         i + size_t(1) >= celldata.size()) {
         std::cout << "FOLLOWERMODE CHORD requires at least two cells input" << std::endl;
         break;
      }

      if ((FollowerConstants::globalFollowerMode == FollowerConstants::globalChord3 || FollowerConstants::globalFollowerMode == FollowerConstants::globalTriangle) &&
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
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord)
      {
         ++i;
      }
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord3) {
         ++i;
         ++i;
      }
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalTriangle) {
         ++i;
         ++i;
      }
   }
   //LRL_LatticeMatcher::record.ShowResults( );

   return 0;

}

