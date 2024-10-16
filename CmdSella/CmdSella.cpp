#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

#include "BravaisHeirarchy.h"
#include "FileOperations.h"
#include "DeloneFitResults.h"
#include "GetDate.h"
#include "GrimmerTree.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_CoordinateConversionMatrices.h"
#include "LRL_CreateFileName.h"
#include "LatticeConverter.h"
#include "LRL_DataToSVG.h"
#include "LRL_ReadLatticeData.h"
#include "LatticeConverter.h"
#include "MatS6.h"
////#include "Niggli.h"
#include "S6.h"
#include "Sella.h"
#include "Selling.h"
#include "WebIO.h"


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
//S6 - 100.00000 - 100.00000 - 100.00000 - 100.00000 - 100.00000 - 100.00000  REFERENCE
//S6 - 234.14273 - 1.88359 - 47.67826 - 0.30533 - 4.90138 - 53.63114  scaled
//S6 - 10.41946 - 7.17661 - 95.06707 - 31.36144 - 190.57190 - 116.19378  scaled
//S6 - 240.86238 - 24.92865 - 8.64775 - 7.17380 - 34.21200 - 8.19574  scaled
//S6 - 230.17565 - 17.27155 - 54.34361 - 23.25853 - 11.49257 - 55.62911  scaled
//S6 - 31.02879 - 10.96443 - 14.25264 - 38.96421 - 239.08206 - 5.95120  scaled
//S6 - 13.78936 - 207.70159 - 82.97108 - 30.45375 - 12.34903 - 93.30473  scaled
//S6 - 121.50546 - 10.74570 - 13.31738 - 80.60464 - 33.75129 - 193.15109  scaled
//S6 - 7.70225 - 9.51802 - 105.79984 - 0.04535 - 211.62487 - 62.22050  scaled
//S6 - 181.85349 - 23.36642 - 115.13135 - 18.98134 - 38.38171 - 106.27627  scaled
//S6 - 2.78162 - 34.43272 - 237.34522 - 10.96929 - 43.32243 - 21.83436  scaled
//S6 - 56.99568 - 27.30398 - 7.58384 - 107.64530 - 0.76797 - 210.61902  scaled
//S6 - 1.76325 - 112.70665 - 24.56708 - 38.27043 - 84.68121 - 195.07698  scaled
//S6 - 11.10601 - 81.30216 - 222.54748 - 5.31460 - 54.25901 - 27.69384  scaled
//S6 - 9.12428 - 0.87504 - 63.50943 - 233.55358 - 4.41117 - 36.27397  scaled
//S6 - 3.00480 - 19.63201 - 241.04017 - 9.76684 - 34.86767 - 13.93003  scaled
static double g_maxDeltaForMatch = 0.02;
std::string selectBravaisCase = "";

S6 GetInputSellingReducedVectors(const LRL_ReadLatticeData& input, MatS6& mat) {
      const S6 s6 = LatticeConverter::SellingReduceCell(input.GetLattice(), input.GetCell(), mat);
   return s6;
}

std::vector<std::pair<std::string, double> > DeloneFitToScores(const std::vector< DeloneFitResults>& fits) {

   std::map<std::string, std::pair<std::string, double> > best;

   for (size_t i = 0; i < fits.size(); ++i) {
      auto test = best.find(fits[i].GetBravaisType());
      if (test == best.end()) {
         fits[i].GetZscore();
         best.insert(std::make_pair(fits[i].GetBravaisType(), std::make_pair(fits[i].GetBravaisType(), fits[i].GetRawFit())));
      }
      else
      {
         const double previous = (*test).second.second;
         if (fits[i].GetRawFit() < previous) {
            best[fits[i].GetBravaisType()] = std::make_pair(fits[i].GetBravaisType(), fits[i].GetRawFit());
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

static std::vector<std::string>  ListMatchingTypes(const std::vector<DeloneFitResults>& vFilteredDeloneFitResults,
   const S6& LatI) {
   std::vector<std::string> out;
   for (const auto& type : vFilteredDeloneFitResults) {
      const DeloneFitResults& dfr = type;
      if (dfr.GetBravaisType()[0] == 'a') continue;
      if (type.GetRawFit() / LatI.norm() < g_maxDeltaForMatch) {
         std::ostringstream os;
         os << "P "
            << LRL_Cell_Degrees(type.GetBestFit()) << " "
            << type.GetDeloneType() << " "
            << type.GetRawFit();
         out.emplace_back(os.str());
      }
   }
   return out;
}

void SearchForToCanon(const std::vector<DeloneFitResults>& vfit) {
   std::vector< LabeledSellaMatrices> matrices = LabeledSellaMatrices().GetPrjs();
   static const std::vector<MatS6> refl_one = MatS6::GetReflections();
   std::vector<MatS6> prjs;
            bool matchprj = false;
   for (const auto& f : vfit) {
      const S6 bestfit = f.GetBestFit();
      const S6 inputVector = f.GetOriginalInput();
      bool match = false;
      matchprj = false;
      const auto name = f.GetGeneralType();
      for (const LabeledSellaMatrices& m : matrices) {
         if (name == m.GetLabel()) {
            match = true;
            matchprj = true;
            prjs = m.GetMatrices();
            for (const auto& prj : refl_one) {
               const S6 projected =prj * inputVector ;
               const double test = (bestfit - projected).norm();
               if (test < 1.0E-8) {
                  matchprj = true;
               }
            }
            const int i19191 = 19191;
         }
      }
      if (!matchprj) {
         // did not find the name
         throw("did not find the name");
      }







      const int i19191 = 19191;
   }
}

std::string ProcessSella(const bool doProduceSellaGraphics, const LRL_ReadLatticeData& input,
   const std::string& filename) {

   std::vector< BravaisChainFailures> outBCF;
   MatS6 oneReductionMatrix;
   const S6 oneLattice = GetInputSellingReducedVectors(input, oneReductionMatrix);
   const S6 oneErrors = 0.1 * input.GetCell();
   int sumBravaisTypesFound = 0;

   std::vector<DeloneFitResults> vDeloneFitResultsForOneInputLattice = Sella::SellaFit(selectBravaisCase, oneLattice, oneErrors, oneReductionMatrix);
   MapOFDeloneFits theDelonefits;
   theDelonefits.CreateMapOFDeloneFits(vDeloneFitResultsForOneInputLattice);
   MapOfBravaisFits theBravaisfits;
   theBravaisfits.CreateMapOFBravaisFits(vDeloneFitResultsForOneInputLattice);

   SearchForToCanon(vDeloneFitResultsForOneInputLattice);

   //std::cout << theDelonefits << std::endl;
   //std::cout << theBravaisfits << std::endl;

   GrimmerChains gcs(S6(input.GetCell()));
   gcs.CreateGrimmerChains(theDelonefits, theBravaisfits);
   gcs.CheckAllGrimmerChains();
   //std::cout << gcs << std::endl;
   //std::cout << gcs << std::endl;

   if (gcs.HasFailure()) {
      GrimmerChainFailure gcf = gcs.GetFirstFailure();
      const std::vector<std::pair<std::string, double>> firstFail = gcf.GetFailures();
      const DeloneFitResults revisedFit = gcs.Remediation(firstFail[1].first, firstFail[1].second);
      vDeloneFitResultsForOneInputLattice.emplace_back(revisedFit);
      gcs = gcs.ReplaceRemediation(revisedFit);
   }
   //theDelonefits.CreateMapOFDeloneFits(vDeloneFitResultsForOneInputLattice);
   //std::cout << theDelonefits << std::endl;
   //theBravaisfits.CreateMapOFBravaisFits(vDeloneFitResultsForOneInputLattice);
   //std::cout << theBravaisfits << std::endl;

   std::cout << "; " << input.GetStrCell() << " input data" << std::endl << std::endl;

   const std::vector<DeloneFitResults> vFilteredDeloneFitResults = FilterForBestExample(vDeloneFitResultsForOneInputLattice);
   std::vector<std::string> matches = ListMatchingTypes(vFilteredDeloneFitResults, oneLattice);

   const std::vector<std::pair<std::string, double> > scores = DeloneFitToScores(vFilteredDeloneFitResults);

   if (matches.empty() && doProduceSellaGraphics) {
      std::cout << "; apparently the input is triclinic--no other Bravais types matched" << std::endl;;
   }

   // the next loop might be not the best. If there was a 2nd copy of a lattice added
   // above, then both might be listed here. Probably there should be a filter so
   // only one for each type gets output.
   for (const auto& out: outBCF)
    std::cout << out << std::endl;

   std::ostringstream os;
   if (!matches.empty()) {
      std::cout << "; projected best fits ( reported distances (in A^2))" << std::endl;
      int y = 300;
      const int x = 700;
      os << "<text x = \"" << x-20 << "\" y = \"" << y << "\" font-size = \"15\" >" << "Projected best fits ( reported distances (in A^2))"  << " </text>\n";
      for (const auto& cell : matches) {
         std::cout << cell << std::endl;
         y += 20;
         os << "<text x = \"" << x << "\" y = \"" << y << "\" font-size = \"15\" >" << cell << " </text>\n";
         //std::cout << os.str() << std::endl;
      }
   }
   std::string temp = os.str();
   matches.clear();
   matches.push_back(temp);

   return  BravaisHeirarchy::ProduceSVG(
      input, oneLattice, scores, matches);
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

void AnalyzeS6(const S6 s6) {

   const double average = (abs(s6[0]) + abs(s6[1]) + abs(s6[2]) + abs(s6[3]) + abs(s6[4]) + abs(s6[5])) / s6.size();
   std::vector<double> vec(s6.GetVector());
   std::sort(vec.begin(), vec.end());

   for (size_t i = 0; i < 6; ++i) {
      vec[i] = abs(vec[i]);
   }
   int lowCount = 0;
   for (size_t i = 0; i < 6; ++i) {
      if (vec[i] < average) ++lowCount;
   }
   std::cout << "average " << average << std::endl;
   std::cout << "s6 " << s6 << std::endl;
   std::cout << "sorted and positive " <<  S6(vec) << std::endl;
   std::cout << "lowCount " << lowCount << std::endl;
}

int main(int argc, char* argv[]) 
{
   bool doProduceSellaGraphics = true;

   std::cout << "; SELLA method symmetry searching\n";
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::cout << "; count of input cells " << inputList.size() << std::endl;

   //for (const auto& input : inputList) {
   //   AnalyzeS6(input.GetCell());
   //   std::cout << std::endl;
   //}
   //exit(0);

   WebIO webio(argc, argv, "CmdSella", inputList.size());
   webio.GetWebBlockSize(argc, argv);
   webio.CreateFilenamesAndLinks(inputList.size(), "SEL");

   const size_t& blockstart = webio.m_blockstart;
   const size_t& blocksize = webio.m_blocksize;

   const std::vector<std::string>& basicfileNameList = webio.m_basicfileNameList;
   const std::vector<std::string>& RawFileNameList = webio.m_FileNameList;
   const std::vector<std::string>& FullfileNameList = webio.m_FullfileNameList;

   std::cout << "; Sella cell block start " << blockstart << std::endl;
   std::cout << "; Sella cell block size " << blocksize << std::endl;
   std::cout << std::endl;

   for (size_t i = blockstart; i < (inputList.size()) && (i < blockstart+blocksize); ++i)
   {
      std::cout << "; Sella graphics file(s) " <<
         i + 1 << "  " << FullfileNameList[i - blockstart] << std::endl;
   }

   for (size_t i = blockstart; i < (inputList.size()) && (i < blockstart+blocksize); ++i)
   {
      std::cout << ";----------------------------------------------------------" << std::endl;
      std::cout << "; SELLA results for input case " << i  << std::endl;
      const std::string svgOutput = ProcessSella(doProduceSellaGraphics, inputList[i],
        RawFileNameList[i-blockstart]);
      if (doProduceSellaGraphics) {
        SendSellaToFile(svgOutput, RawFileNameList[i-blockstart]);
        std::cout << "; Send Sella Plot to graphics file " << FullfileNameList[i-blockstart] << std::endl; 
      }
   }

}
