#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include "BravaisHeirarchy.h"
#include "FileOperations.h"
#include "DeloneFitResults.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_CoordinateConversionMatrices.h"
#include "LRL_CreateFileName.h"
#include "LatticeConverter.h"
#include "LRL_DataToSVG.h"
#include "LRL_ReadLatticeData.h"
#include "LatticeConverter.h"
#include "MatS6.h"
#include "Niggli.h"
#include "S6.h"
#include "Sella.h"
#include "Selling.h"
#include "StoreResults.h"


// 2023/12/23
// bad case: P 21.775 21.785 27.232 90.028 89.974 90.023
//;################ Bravais chain failure  oS 188.739 tP 0.371973         s6  -0.09949 -473.88154  -0.19042  -0.07866 -474.29631 -741.21325       P    21.775    21.785    34.860    90.008   128.630    90.023
// ; P 21.775      21.785  27.232  90.028  89.974  90.023   input data
// on the Grimmer plot:
// mS .543
// mP .454
// oS 13.74
// tP .61
//

static double g_maxDeltaForMatch = 0.02;
std::string selectBravaisCase = "";

S6 GetInputSellingReducedVectors(const LRL_ReadLatticeData& input, MatS6& vmat) {
   std::vector<S6> v;
   MatS6 m;
   //vmat.clear();

   //for (size_t i = 0; i < input.size(); ++i) {
      const S6 s6 = LatticeConverter::SellingReduceCell(input.GetLattice(), input.GetCell(), vmat);
   //}

   return s6;
}

std::vector<std::pair<std::string, double> > DeloneFitToScores(std::vector< DeloneFitResults>& fits) {

   std::map<std::string, std::pair<std::string, double> > best;

   for (size_t i = 0; i < fits.size(); ++i) {
      auto test = best.find(fits[i].GetType());
      if (test == best.end()) {
         fits[i].GetZscore();
         best.insert(std::make_pair(fits[i].GetType(), std::make_pair(fits[i].GetType(), fits[i].GetRawFit())));
      }
      else
      {
         const double previous = (*test).second.second;
         if (fits[i].GetRawFit() < previous) {
            best[fits[i].GetType()] = std::make_pair(fits[i].GetType(), fits[i].GetRawFit());
         }
      }
   }
   std::vector<std::pair<std::string, double> > out;
   for (auto i = best.begin(); i != best.end(); ++i) {
      out.emplace_back((*i).second);
   }
   return out;
}

std::vector<S6> CreateS6Errors(const std::vector< S6>& vs) {
   std::vector<S6> out;
   for (size_t i = 0; i < vs.size(); ++i) {
      out.push_back(0.1 * vs[i]);
   }
   return out;
}

double DeltaInputBest(const DeloneFitResults& vDeloneFitResults) {
   const S6 input = vDeloneFitResults.GetOriginalInput();
   const S6 bestFit = vDeloneFitResults.GetBestFit();
   return (input - bestFit).norm();
}

std::map<std::string, DeloneFitResults>  CreateMapForBestExamples(
   const std::vector<DeloneFitResults>& vDeloneFitResults)
{
   std::map<std::string, DeloneFitResults>  bravaisMap;

   for (size_t i = 0; i < vDeloneFitResults.size(); ++i) {
      std::string name = vDeloneFitResults[i].GetGeneralType();
      //if (name == "oC") name = "oS";
      const double& delta = vDeloneFitResults[i].GetDifference().norm();

      auto mapElement = bravaisMap.find(name);
      if (mapElement == bravaisMap.end())
         bravaisMap.insert(std::make_pair(name, vDeloneFitResults[i]));
      else
         if (delta < (*mapElement).second.GetDifference().norm())
            (*mapElement).second = vDeloneFitResults[i];
   }
   return bravaisMap;
}

std::vector<DeloneFitResults> FilterForBestExample(
   const std::vector<DeloneFitResults>& vDeloneFitResults)
{
   std::map<std::string, DeloneFitResults>  bravaisMap = CreateMapForBestExamples(vDeloneFitResults);

   std::vector<DeloneFitResults> out;
   for (auto i = bravaisMap.begin(); i != bravaisMap.end(); ++i) {
      out.push_back((*i).second);
   }
   return out;
}

void NiggliMatchLatticeType(const DeloneFitResults& deloneFitResults) {
   const S6 bestFit = deloneFitResults.GetBestFit();
   double deltaInputBest = DeltaInputBest(deloneFitResults);
   if (deltaInputBest < 1.0E-6) deltaInputBest = 0.0;
   const std::string latticeType = deloneFitResults.GetType();
   const std::string latticeGeneral = deloneFitResults.GetGeneralType();

   const std::vector<std::shared_ptr<GenerateNiggliBase> >
      vglb = GenerateNiggliBase().Select(latticeGeneral);

   for (size_t i = 0; i < vglb.size(); ++i) {
      const std::shared_ptr<GenerateNiggliBase> pt = vglb[i];
      //if (pt->GetBravaisType()[0] == 'a') continue;
      G6 probe;
      Niggli::Reduce(G6(bestFit), probe);
      const G6 perpV = pt->GetPerp() * probe;
      double d = perpV.norm();
      if (d < 1.0E-8) d = 0.0;
      if (d / probe.norm() > g_maxDeltaForMatch) continue;
      const G6 centered =  pt->GetToCenter() * probe;

      std::cout << latticeType.substr(1,1)+" " << LRL_Cell_Degrees(centered)
         << "  " << latticeType << "  " << deltaInputBest << "\n";
      break;
   }
}

static int ListMatchingTypes(const size_t lat, const std::vector<DeloneFitResults>& vFilteredDeloneFitResults,
   const S6& vLatI) {
   int toReturn = 0;
   for (size_t kk = 0; kk < vFilteredDeloneFitResults.size(); ++kk) {
      const DeloneFitResults& dfr = vFilteredDeloneFitResults[kk];
      if (dfr.GetType()[0] == 'a') continue;
      //const double d = vDeloneFitResults[kk].GetRawFit() / vLat[lat].norm();
      if (vFilteredDeloneFitResults[kk].GetRawFit() / vLatI.norm() < g_maxDeltaForMatch) {
         NiggliMatchLatticeType(vFilteredDeloneFitResults[kk]);
         toReturn = 1;
      }
   }
   return toReturn;
}

std::string ProcessSella(const LRL_ReadLatticeData& input) {
   MatS6 oneReductionMatrix;
   const std::vector<LRL_ReadLatticeData> oneInput(1, input);
   const S6 oneLattice = GetInputSellingReducedVectors(input, oneReductionMatrix);
   const S6 oneErrors = 0.1 * input.GetCell();
   int sumBravaisTypesFound = 0;

   std::vector<DeloneFitResults> vDeloneFitResultsForOneLattice = Sella().SellaFit(selectBravaisCase, oneLattice, oneErrors, oneReductionMatrix);

   BravaisHeirarchy::CheckBravaisChains(vDeloneFitResultsForOneLattice);

   std::cout << "; " << input.GetStrCell() << " input data" << std::endl << std::endl;

   std::cout << "; reported distances and zscores (in A^2)" << std::endl;
   const std::vector<DeloneFitResults> vFilteredDeloneFitResults = FilterForBestExample(vDeloneFitResultsForOneLattice);
   sumBravaisTypesFound += ListMatchingTypes(19191, vFilteredDeloneFitResults, oneLattice);

   const std::vector<std::pair<std::string, double> > scores = DeloneFitToScores(vDeloneFitResultsForOneLattice);

   if (sumBravaisTypesFound == 0) {
      std::cout << "; apparently the input is triclinic--no other Bravais types matched" << std::endl;;
   }

   return  BravaisHeirarchy::ProduceSVG(
      input, oneLattice, scores);
}

void SendSellaToFile(const std::string& svg, const size_t ordinal) {
   std::cout << std::endl << ";Send Sella Plot To File " << std::endl;
   std::ofstream fileout;

   const std::string suffix = LRL_DataToSVG(ordinal);
   const std::string filename = LRL_CreateFileName::Create("SEL_", suffix, "svg", true);
   std::cout << filename << std::endl;

   FileOperations::OpenOutputFile(fileout, filename);

   if (fileout.is_open())
   {
      fileout.seekp(0);
      fileout << svg << std::endl;
   }
   else
      std::cout << "Could not open file " << filename << " for write in SendSellaToFile.h" << std::endl;

   fileout.close();
}

int main(int argc, char* argv[])
{

   bool doProduceSellaGraphics = true;

   if (argc > 1) {
      selectBravaisCase = argv[1];
      if (std::string(argv[1]) == "lca") 
      {
         selectBravaisCase.clear();
         doProduceSellaGraphics = false;
      }
      if (argc > 2) {
         const double d = atof(argv[2]);
         if (d != 0.0) g_maxDeltaForMatch = atof(argv[2]);
      }
   }
   std::cout << "; CmdSELLA\n";
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   /*
   p 10 20 30  90 90 90
p 10 10 10  90 90 91
end
*/
   for ( size_t i=0; i<inputList.size(); ++i ) 
   {
      //const std::vector<LRL_ReadLatticeData> justOneInput(1, inputList[i]);
      const std::string svgOutput = ProcessSella(inputList[i]);
      if (doProduceSellaGraphics) SendSellaToFile(svgOutput, i);

   }

}
