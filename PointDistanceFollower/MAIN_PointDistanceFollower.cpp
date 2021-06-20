// D7_Follower.cpp : Defines the entry point for the console application.
//

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

std::vector<std::pair<S6, S6> > GenerateS6LineFromStartToCell2ForModeLine(const CellInputData& cell1, const CellInputData& cell2, const CellInputData& cell3) {
   const std::vector<std::pair<S6, S6> > v1 = GenerateS6LineFromStartToFinish(cell1);
   const std::vector<std::pair<S6, S6> > v2 = GenerateS6LineFromStartToFinish(cell2);
   std::vector<std::pair<S6, S6> > vout;
   for (size_t i = 0; i < v1.size(); ++i) {
      vout.push_back(std::make_pair(v1[i].first, v2[i].first));
   }
   return vout;
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
      const bool b2 = Selling::Reduce(next2, reduced2);
      if (!b1 || !reduced1.IsAllMinus() || !reduced1.IsValid()) reduced1 = InvalidPoint();
      if (!b2 || !reduced2.IsAllMinus() || !reduced2.IsValid()) reduced2 = InvalidPoint();
      points1.push_back(std::make_pair(reduced1, reduced2));
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

MultiFollower ProcessOneLattice(const size_t inputCellOrdinal, const size_t plotCounter, const CellInputData& cell1,
   const CellInputData& cell2, const CellInputData& cell3) {
   const std::string baseFileName = NameOneFileForOneLattice(inputCellOrdinal) + LRL_ToString(plotCounter);

   const size_t npoints = FollowerConstants::globalStepsPerFrame;
   std::unique_ptr<FollowerPathGenerator> fpg;
   std::string inputCells;
   if (FollowerConstants::globalFollowerMode == FollowerConstants::globalTriangle) {
      std::unique_ptr<FollowerPathGenerator> fpgT(new FollowerTriangle(npoints, cell1.GetCell(), cell2.GetCell(), cell3.GetCell()));
      fpg = std::move(fpgT);
      inputCells += cell2.GetStrCell() + "\n";
      inputCells += cell3.GetStrCell() + "\n";
   }
   else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord3) {
      std::unique_ptr<FollowerPathGenerator> fpgC3(new FollowerChord3(npoints, cell1.GetCell(), cell2.GetCell(), cell3.GetCell()));
      fpg = std::move(fpgC3);
      inputCells += cell2.GetStrCell() + "\n";
      inputCells += cell3.GetStrCell() + "\n";
   }
   else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord) {
      std::unique_ptr<FollowerPathGenerator> fpgC(new FollowerChord(npoints, cell1.GetCell(), cell2.GetCell()));
      fpg = std::move(fpgC);
      inputCells += cell2.GetStrCell() + "\n";
   }
   else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine) {
      std::unique_ptr<FollowerPathGenerator> fpgL(new FollowerLine(npoints, cell1.GetCell(), cell2.GetCell()));
      fpg = std::move(fpgL);
      inputCells += cell2.GetStrCell() + "\n";
   }
   else {// globalSinglePoint
      std::unique_ptr<FollowerPathGenerator> fpgP(new FollowerPoint(npoints, cell1.GetCell()));
      fpg = std::move(fpgP);
   }

   std::string firstAndLastPoints;
   const std::vector<std::pair<S6, S6> > path = fpg->GetPath();
   std::pair<std::string, std::string> sp = GetLineEnds(path);
   const size_t n = path.size();
   const std::pair < std::string, std::string> pathExtremes = GetLineEnds(path);
   if (FollowerConstants::globalFollowerMode == FollowerConstants::globalTriangle) {
      firstAndLastPoints = pathExtremes.first + pathExtremes.second;
   }
   else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord3) {
      firstAndLastPoints = pathExtremes.first + pathExtremes.second;
   }
   else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord) {
      firstAndLastPoints = pathExtremes.first + pathExtremes.second;
   }
   else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine) {
      firstAndLastPoints = pathExtremes.first;
   }
   else {// globalSinglePoint
      firstAndLastPoints = pathExtremes.first;
   }


   const std::string name = fpg->GetName();
   GLOBAL_Report::globalDataReport +=
      "\nINPUT   " + baseFileName + "   " + fpg->GetName() +
      "\n" + cell1.GetStrCell() + "\n" + inputCells +
      "  Path starts and ends \n" + firstAndLastPoints;


   const std::vector<std::pair<S6, S6> > sellingReducedPath = ReducePath<S6, S6, Selling>(path);
   const std::vector<std::pair<G6, G6> > niggliReducedPath = ReducePath<S6, G6, Niggli>(sellingReducedPath);

   std::vector<std::pair<DC, DC> > DCpath;
   if (FollowerConstants::IsEnabled("DC")) {
      for (size_t i = 0; i < niggliReducedPath.size(); ++i)
         DCpath.push_back(std::make_pair(DC(niggliReducedPath[i].first), DC(niggliReducedPath[i].second)));
   }

   MultiFollower mf;
   mf.SetNiggliPath(niggliReducedPath);
   mf.SetSellingPath(sellingReducedPath);
   mf.SetDCPath(DCpath);
   mf.SetInputVectors(fpg->GetInput());

   mf = mf.GenerateAllDistances();
   mf.SetTime2ComputeFrame(double(std::clock() - mf.GetComputeStartTime()));

   if (!mf.HasGlitches() && FollowerConstants::globalOutputGlitchesOnly) return mf;
   SVG_DistancePlot<S6> distanceplot(mf, baseFileName);
   std::cout << "#  " << baseFileName << std::endl;

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

