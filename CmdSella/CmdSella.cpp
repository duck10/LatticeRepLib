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
//;################ Bravais chain failure  mP 0.206 oS 188.739 tP 0.371973         s6  -0.09949 -473.88154  -0.19042  -0.07866 -474.29631 -741.21325       P    21.775    21.785    34.860    90.008   128.630    90.023
// ; P 21.775      21.785  27.232  90.028  89.974  90.023   input data
// on the Grimmer plot:
// mS .543
// mP .454
// oS 13.74
// tP .61
//

/*   2023-12-24
;################  Bravais  chain  failure  mP  0.0653663  oS  1.12195  tP  0.230772
;##  s6  -8.02079  -0.06452  -1.63327  -0.01046  -0.1679  -1.83719
;##  P  1.307  3.134  3.15  144.338  90.898  113.499
;  P  1.307  3.134  3.15  144.338  90.898  113.499  input  data

;################  Bravais  chain  failure  mP  0.198275  oS  0.73327  tP  0.579379
;##  s6  -0.16329  -0.11247  -1.48986  -0.49148  -2.98657  -1.82095
;##  P  1.447  2.154  1.448  93.001  93.077  118.555
;  P  1.447  2.154  1.448  93.001  93.077  118.555  input  data

;################  Bravais  chain  failure  mP  0.610243  oS  0.887935  tP  0.861632
;##  s6  -13.49486  -1.39668  -0.48451  -0.40193  -1.9168  -0.45918
;##  P  1.511  3.987  3.918  149.756  103.646  94.613
;  P  1.511  3.987  3.918  149.756  103.646  94.613  input  data

;################  Bravais  chain  failure  mP  0.99652  oS  1.41667  tP  1.19767
;##  s6  -8.84149  -0.66343  -2.08744  -0.8934  -0.44145  -2.13682
;##  P  1.909  3.372  3.412  140.217  95.846  108.922
;  P  1.909  3.372  3.412  140.217  95.846  108.922  input  data

;################  Bravais  chain  failure  mP  0.713571  oS  1.22805  tP  1.12995
;##  s6  -1.77479  -0.62715  -0.81523  -2.22868  -13.67507  -0.3404
;##  P  1.916  4.033  1.656  105.411  101.4  96.056
;  P  1.916  4.033  1.656  105.411  101.4  96.056  input  data

;################  Bravais  chain  failure  mP  0.212052  oS  0.551513  tP  0.416752
;##  s6  -0.15797  -2.37936  -0.95049  -0.34887  -0.14147  -1.06887
;##  P  1.918  1.118  1.899  94.267  130.788  116.312
;  P  1.918  1.118  1.899  94.267  130.788  116.312  input  data

;################  Bravais  chain  failure  mP  0.683048  oS  2.13444  tP  1.90112
;##  s6  -4.85003  -0.42893  -0.53158  -3.21743  -1.34722  -7.70985
;##  P  2.044  2.594  3.604  121.251  93.338  95.754
;  P  2.044  2.594  3.604  121.251  93.338  95.754  input  data

;################  Bravais  chain  failure  mP  0.361083  oS  1.50637  tP  1.25792
;##  s6  -0.2922  -0.36108  -4.01366  -0.00172  -8.02828  -2.36042
;##  P  2.092  3.512  1.736  92.747  95.706  123.113
;  P  2.092  3.512  1.736  92.747  95.706  123.113  input  data

;################  Bravais  chain  failure  mP  0.93536  oS  1.76066  tP  1.52803
;##  s6  -5.65027  -0.726  -3.57718  -0.58976  -1.19254  -3.30205
;##  P  2.212  3.228  3.111  124.239  96.056  120.065
;  P  2.212  3.228  3.111  124.239  96.056  120.065  input  data

;################  Bravais  chain  failure  mP  0.455146  oS  0.656463  tP  0.524884
;##  s6  -0.05752  -0.71201  -4.90789  -0.22683  -0.89583  -0.4515
;##  P  2.418  2.421  1.105  91.232  105.455  146.97
;  P  2.418  2.421  1.105  91.232  105.455  146.97  input  data

;################  Bravais  chain  failure  mP  0.412718  oS  2.64353  tP  2.47308
;##  s6  -3.08596  -1.47834  -0.41062  -5.82832  -0.04158  -11.40371
;##  P  2.778  1.881  3.996  114.24  97.653  94.507
;  P  2.778  1.881  3.996  114.24  97.653  94.507  input  data

;################  Bravais  chain  failure  mP  1.17268  oS  2.7668  tP  2.36336
;##  s6  -0.08395  -5.36611  -1.16967  -1.8221  -4.03178  -9.28786
;##  P  2.891  2.299  3.839  90.545  118.914  100.136
;  P  2.891  2.299  3.839  90.545  118.914  100.136  input  data

;################  Bravais  chain  failure  mP  0.806189  oS  1.30639  tP  1.02451
;##  s6  -0.31751  -2.32436  -6.36243  -0.15194  -1.55122  -0.79174
;##  P  2.973  2.869  1.853  93.424  114.956  138.239
;  P  2.973  2.869  1.853  93.424  114.956  138.239  input  data

;################  Bravais  chain  failure  mP  0.332197  oS  0.978697  tP  0.78934
;##  s6  -0.33068  -0.03171  -2.30169  -8.46439  -0.15987  -1.31463
;##  P  3.286  1.671  1.295  98.79  90.427  114.783
;  P  3.286  1.671  1.295  98.79  90.427  114.783  input  data

;################  Bravais  chain  failure  mP  0.589591  oS  0.855612  tP  0.842365
;##  s6  -0.12432  -0.81225  -9.97269  -0.40409  -1.4426  -0.57633
;##  P  3.345  3.397  1.23  91.705  101.386  151.36
;  P  3.345  3.397  1.23  91.705  101.386  151.36  input  data
*/

static double g_maxDeltaForMatch = 0.02;
std::string selectBravaisCase = "";

S6 GetInputSellingReducedVectors(const LRL_ReadLatticeData& input, MatS6& mat) {
      const S6 s6 = LatticeConverter::SellingReduceCell(input.GetLattice(), input.GetCell(), mat);
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

static int ListMatchingTypes(const std::vector<DeloneFitResults>& vFilteredDeloneFitResults,
   const S6& LatI) {
   int toReturn = 0;
   for (size_t kk = 0; kk < vFilteredDeloneFitResults.size(); ++kk) {
      const DeloneFitResults& dfr = vFilteredDeloneFitResults[kk];
      if (dfr.GetType()[0] == 'a') continue;
      //const double d = vDeloneFitResults[kk].GetRawFit() / vLat[lat].norm();
      if (vFilteredDeloneFitResults[kk].GetRawFit() / LatI.norm() < g_maxDeltaForMatch) {
         NiggliMatchLatticeType(vFilteredDeloneFitResults[kk]);
         toReturn = 1;
      }
   }
   return toReturn;
}

std::string ProcessSella(const bool doProduceSellaGraphics, const LRL_ReadLatticeData& input,
   const std::string& filename) {

   std::string out;
   MatS6 oneReductionMatrix;
   const S6 oneLattice = GetInputSellingReducedVectors(input, oneReductionMatrix);
   const S6 oneErrors = 0.1 * input.GetCell();
   int sumBravaisTypesFound = 0;

   std::vector<DeloneFitResults> vDeloneFitResultsForOneLattice = Sella().SellaFit(selectBravaisCase, oneLattice, oneErrors, oneReductionMatrix);

   out += BravaisHeirarchy::CheckBravaisChains(vDeloneFitResultsForOneLattice);
   const auto result = BravaisHeirarchy::CheckBravaisChains(vDeloneFitResultsForOneLattice);

   std::cout << "; " << input.GetStrCell() << " input data" << std::endl << std::endl;
   std::cout << "; Graphics output will go to file " << filename << std::endl;

   std::cout << "; reported distances (in A^2)" << std::endl;
   if (!vDeloneFitResultsForOneLattice.empty()) std::cout << "; projected best fits" << std::endl;
   const std::vector<DeloneFitResults> vFilteredDeloneFitResults = FilterForBestExample(vDeloneFitResultsForOneLattice);
   sumBravaisTypesFound += ListMatchingTypes(vFilteredDeloneFitResults, oneLattice);

   const std::vector<std::pair<std::string, double> > scores = DeloneFitToScores(vDeloneFitResultsForOneLattice);

   if (sumBravaisTypesFound == 0 && doProduceSellaGraphics) {
      std::cout << "; apparently the input is triclinic--no other Bravais types matched" << std::endl;;
   }

    std::cout << out << std::endl;

   return  BravaisHeirarchy::ProduceSVG(
      input, oneLattice, scores);
}

std::string SendSellaToFile(const std::string& svg, const std::string& filename) {
   std::ofstream fileout;

   FileOperations::OpenOutputFile(fileout, filename);

   if (fileout.is_open())
   {
      fileout.seekp(0);
      fileout << svg << std::endl;
   }
   else
      std::cout << "Could not open file " << filename << " for write in SendSellaToFile.h" << std::endl;

   fileout.close();
   return filename;
}

static std::vector<std::string> CreateAllFileNames(const size_t n) {
   std::vector<std::string> out;

   for (size_t i = 0; i < n; ++i) {
   const std::string filename = LRL_CreateFileName::Create("SEL", LRL_DataToSVG(i+1), "svg", true);
   out.emplace_back(filename);
   }
   return out;
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

   if (argc > 1 && !(std::string(argv[1]).empty() && std::string(argv[1]) == "all")) {
      doProduceSellaGraphics = false;
   }

   std::cout << "; SELLA method symmetry searching\n";
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   /*
   p 10 20 30  90 90 90
p 10 10 10  90 90 91
end
*/

   const std::vector<std::string> filenames =
      LRL_CreateFileName::CreateListOfFilenames(inputList.size(), "SEL", "svg");

   for (size_t whichCell = 0; whichCell < inputList.size(); ++whichCell) {
      std::cout << "; Send Sella Plot to graphics file " 
         << filenames[whichCell]<< " for case " 
         << whichCell+1 << std::endl;
   }

   std::cout << std::endl;

   for (size_t i = 0; i < inputList.size(); ++i)
   {
      std::cout << "----------------------------------------------------------" << std::endl;
      std::cout << "; SELLA results for input case " << i + 1 << std::endl;
      const std::string svgOutput = ProcessSella(doProduceSellaGraphics, inputList[i], filenames[i]);
      if (doProduceSellaGraphics) SendSellaToFile(svgOutput, filenames[i]);
   }

}
