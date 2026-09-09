/* define LRL_DEBUG 1 */

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>

#include "BlockUtils.h"
#include "BravaisHeirarchy.h"
#include "CmdSellaControls.h"
#include "DeloneFitFiltering.h"
#include "DeloneParentageTest.h"
#include "ExpandAcrossBoundaryZeros.h"
#include "FileOperations.h"
#include "DeloneFitResults.h"
#include "GetDate.h"
#include "GenerateLatticeTypeExamples.h"
#include "GrimmerDeloneTree.h"
#include "GrimmerTree.h"
#include "InputHandler.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_CoordinateConversionMatrices.h"
#include "LRL_CreateFileName.h"
#include "LatticeConverter.h"
#include "LRL_DataToSVG.h"
#include "LRL_ReadLatticeData.h"
#include "LatticeConverter.h"
#include "MatS6.h"
#include "ProgramSetup.h"
////#include "Niggli.h"
#include "S6.h"
#include "Sella.h"
#include "Selling.h"
#include "TransformationMatrices.h"
#include "WebIO.h"

#include "SellaBuild.h"
#include "LabeledDeloneTypeMatrices.h"
#include "DeloneFitComputation.h"
#include "FlexibleSellaTestMode.h"

#include "CompareBoundaryTransformsTest.h"
#include "DiagnoseO2Gap.h"
#include "DiagnoseBoundaryCandidates.h"
#include "CheckABReflection.h"
#include "DiagnoseO1BCoverage.h"
#include "TestRemediationOnBoundaryCandidates.h"
#include "SearchAllBoundaryTies.h"
#include "SearchBoundaryNeighborsByTransform.h"


#include "StoreResults.h"
#include "RemediatedFit.h"

#include "TestHarness.h"

#include "DirectClassify.h"


// 2023/12/23
// bad case: P 21.775 21.785 27.232 90.028 89.974 90.023 still bad after remediation
//;################ Bravais chain failure  mP 0.206 oS 188.739 tP 0.371973         s6  -0.09949 -473.88154  -0.19042  -0.07866 -474.29631 -741.21325       P    21.775    21.785    34.860    90.008   128.630    90.023
// ; P 21.775      21.785  27.232  90.028  89.974  90.023   input data
// on the Grimmer plot:
// mS .543
// mP .454
// oS 13.74
// tP .61
//

/*   2023-12-24F
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
//S6 -100.00000 -100.00000 -100.00000 -100.00000 -100.00000 -100.00000  REFERENCE
//S6 -234.14273 -1.88359 -47.67826 -0.30533 -4.90138 -53.63114  scaled
//S6 -10.41946 -7.17661 -95.06707 -31.36144 -190.57190 -116.19378  scaled
//S6 -240.86238 -24.92865 -8.64775 -7.17380 -34.21200 -8.19574  scaled still bad after remediation
//S6 -230.17565 -17.27155 -54.34361 -23.25853 -11.49257 -55.62911  scaled still bad after remediation
//S6 -31.02879 -10.96443 -14.25264 -38.96421 -239.08206 -5.95120  scaled
//S6 -13.78936 -207.70159 -82.97108 -30.45375 -12.34903 -93.30473  scaled
//S6 -121.50546 -10.74570 -13.31738 -80.60464 -33.75129 -193.15109  scaled
//S6 -7.70225 -9.51802 -105.79984 -0.04535 -211.62487 -62.22050  scaled
//S6 -181.85349 -23.36642 -115.13135 -18.98134 -38.38171 -106.27627  scaled
//S6 -2.78162 -34.43272 -237.34522 -10.96929 -43.32243 -21.83436  scaled still bad after remediation
//S6 -56.99568 -27.30398 -7.58384 -107.64530 -0.76797 -210.61902  scaled
//S6 -1.76325 -112.70665 -24.56708 -38.27043 -84.68121 -195.07698  scaled
//S6 -11.10601 -81.30216 -222.54748 -5.31460 -54.25901 -27.69384  scaled still bad after remediation
//S6 -9.12428 -0.87504 -63.50943 -233.55358 -4.41117 -36.27397  scaled still bad after remediation
//S6 -3.00480 -19.63201 -241.04017 -9.76684 -34.86767 -13.93003  scaled still bad after remediation
// selectBravaisCase (was: std::string selectBravaisCase = "";) removed --
// it was never reassigned anywhere in this file, so ComputeDeloneFits
// (now defined in DeloneFitComputation.cpp, in SellaLib -- see that
// file's header comment for why it moved) inlines "" directly instead.


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

// CreateMapForBestExamples / FilterForBestExample / GetMatchedTypeNames
// moved to DeloneFitFiltering.h (see that file's header comment) so
// CellClassify can share the exact same logic instead of duplicating it.
// No behavior change -- pure relocation.

static std::vector<std::string> ListMatchingTypes(const std::vector<DeloneFitResults>& vFilteredDeloneFitResults,
   const S6& LatI, const double maxDeltaForMatch) {
   const std::set<std::string> matchedNames =
      GetMatchedTypeNames(vFilteredDeloneFitResults, LatI.norm(), maxDeltaForMatch);
   std::vector<std::string> out;
   for (const auto& type : vFilteredDeloneFitResults) {
      if (matchedNames.find(type.GetGeneralType()) == matchedNames.end()) continue;
      std::ostringstream os;
      os << "P "
         << LRL_Cell_Degrees(type.GetBestFit()) << " "
         << type.GetDeloneType() << " "
         << type.GetRawFit();
      out.emplace_back(os.str());
   }
   return out;
}



// RemediateIfShortlistChanges, SearchForToCanon, and ComputeDeloneFits
// moved to DeloneFitComputation.cpp (in SellaLib) -- this was the ONLY
// definition of ComputeDeloneFits anywhere, which is why CellClassify
// (a second program, genuinely linking separately from this one) hit
// LNK2001 on it: FlexibleSellaTestMode.h's call to it never crossed a
// real executable boundary before, since that header is #include'd
// directly into this file. No behavior change -- pure relocation, same
// as DeloneFitFiltering.h's earlier extraction.

std::tuple<std::string, std::string, std::string> ProcessSella(
   const bool doProduceSellaGraphics,
   const LatticeCell& input,
   const std::string& filename,
   const CmdSellaControls& controls,
   Sella& sella)  // NEW parameter - sella passed by reference
{
   std::vector<BravaisChainFailures> outBCF;
   MatS6 oneReductionMatrix;
   const S6 oneLattice = GetInputSellingReducedVectors(input, oneReductionMatrix);

   DeloneFitComputation computation = ComputeDeloneFits(input, controls, sella);
   std::vector<DeloneFitResults> vDeloneFitResultsForOneInputLattice = computation.fits;
   GrimmerChains gcs = computation.chains;

   std::cout << "; " << input.GetInput() << " input data" << std::endl << std::endl;

   std::cout << "Fits for Types" << std::endl;
   for (const auto xxxx : vDeloneFitResultsForOneInputLattice) {
      if (!controls.shouldShowType(xxxx.GetGeneralType())) continue;
      std::cout << xxxx.GetBravaisType() << " ";
      std::cout << xxxx.GetDeloneType() << " ";
      std::cout << xxxx.GetGeneralType() << " ";
      std::cout << xxxx.GetRawFit() << " ";
      std::cout << std::endl;
   }

   const std::vector<DeloneFitResults> vFilteredDeloneFitResults =
      FilterForBestExample(vDeloneFitResultsForOneInputLattice);
   const std::vector<std::string> matches = ListMatchingTypes(vFilteredDeloneFitResults, oneLattice, controls.getMaxDeltaForMatch());
   const std::vector<std::pair<std::string, double>> scores =
      DeloneFitToScores(vFilteredDeloneFitResults);

   if (matches.empty() && doProduceSellaGraphics) {
      std::cout << "; apparently the input is triclinic--no other Bravais types matched"
         << std::endl;
   }

   for (const auto& out : outBCF)
      std::cout << out << std::endl;

   // BravaisHeirarchy::ProduceSVG wants the projected-fits list pre-wrapped
   // as a single SVG <text> block at its own fixed column (x=700); build
   // that separately from the raw matches list instead of overwriting
   // matches, so GrimmerDeloneTree::ProduceSVG below can lay the same
   // lines out itself, against its own (differently sized) diagram.
   std::ostringstream os;
   if (!matches.empty()) {
      std::cout << "; projected best fits ( reported distances (in A^2))" << std::endl;
      int y = 300;
      const int x = 700;
      os << "<text x = \"" << x - 20 << "\" y = \"" << y
         << "\" font-size = \"15\" >"
         << "Projected best fits ( reported distances (in A^2))"
         << " </text>\n";
      for (const auto& cell : matches) {
         std::cout << cell << std::endl;
         y += 20;
         os << "<text x = \"" << x << "\" y = \"" << y
            << "\" font-size = \"15\" >" << cell << " </text>\n";
      }
   }

   std::vector<std::string> matchesForMainSvg;
   matchesForMainSvg.push_back(os.str());

   std::string mainSvg = BravaisHeirarchy::ProduceSVG(
      input, oneLattice, scores, matchesForMainSvg);

   std::string fitPlotsSvg;
   std::string deloneTreeSvg;
   if (doProduceSellaGraphics) {
      gcs.UpdateFits(vDeloneFitResultsForOneInputLattice);
      fitPlotsSvg = gcs.GenerateSortedFitPlots(900, 700, input.getInput());
      deloneTreeSvg = GrimmerDeloneTree::ProduceSVG(gcs, input, matches, oneLattice);
   }

   return std::make_tuple(mainSvg, fitPlotsSvg, deloneTreeSvg);
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
      const std::string filename = LRL_CreateFileName::Create("SEL", LRL_DataToSVG(i + 1), "svg", true);
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
   std::cout << "sorted and positive " << S6(vec) << std::endl;
   std::cout << "lowCount " << lowCount << std::endl;
}

std::string AddSuffixToAllSvgOccurrences(const std::string& input, const std::string& suffix) {
   std::string result = input;
   std::string pattern = ".svg";
   size_t pos = 0;

   // Find and process each occurrence of ".svg"
   while ((pos = result.find(pattern, pos)) != std::string::npos) {
      // Insert the suffix before ".svg"
      result.insert(pos, suffix);
      // Update position to look for next occurrence, accounting for the added suffix
      pos += suffix.length() + pattern.length();
   }

   return result;
}

// PrintCompareSummary -- prints one table summarizing raw fit values across
// several input cells at once: rows grouped by Delone type (all A1 rows
// together, all A2 rows together, etc., in first-seen order across
// inputs), with a blank line between type groups and one row per input
// within a group -- fit, type, cell, in long/tidy format rather than a
// wide type-by-input grid. The wide grid became unreadable with all 24
// Delone types as columns (lines too long to read); this trades width for
// lines you can actually read, sort, or paste into a spreadsheet. "cell"
// is the actual input vector (DeloneFitResults::GetOriginalInput()), not
// an ordinal "Input N" label, so each row is self-identifying without
// needing to cross-reference a separate header.
void PrintCompareSummary(
   const std::vector<std::pair<std::string, std::vector<DeloneFitResults>>>& compareResults,
   const CmdSellaControls& controls)
{
   struct Row {
      double fit;
      std::string cell;
   };

   // Type order is first-seen across inputs, in input order -- stable and
   // doesn't depend on std::map's alphabetical ordering of type names.
   std::vector<std::string> typeOrder;
   std::set<std::string> seenTypes;
   std::map<std::string, std::vector<Row>> rowsByType;

   for (const auto& labeledFits : compareResults) {
      if (labeledFits.second.empty()) continue;

      std::ostringstream cellOs;
      cellOs << labeledFits.second[0].GetOriginalInput();
      const std::string cell = cellOs.str();

      // A type can appear more than once for this input (e.g. remediation
      // appends an extra DeloneFitResults entry) -- keep the best
      // (smallest) raw fit per type.
      std::map<std::string, double> bestFitForType;
      for (const auto& f : labeledFits.second) {
         const std::string t = f.GetGeneralType();
         const double v = f.GetRawFit();
         const auto it = bestFitForType.find(t);
         if (it == bestFitForType.end() || v < it->second) bestFitForType[t] = v;
      }

      for (const auto& tf : bestFitForType) {
         const std::string& t = tf.first;
         if (!controls.shouldShowType(t)) continue;
         if (seenTypes.insert(t).second) typeOrder.push_back(t);
         rowsByType[t].push_back(Row{ tf.second, cell });
      }
   }

   std::cout << "; COMPARE summary -- fit, type, cell (grouped by type)" << std::endl;
   std::cout << std::fixed << std::setprecision(5);
   bool firstGroup = true;
   for (const auto& t : typeOrder) {
      if (!firstGroup) std::cout << std::endl;
      firstGroup = false;
      for (const auto& r : rowsByType[t]) {
         std::cout << std::setw(12) << std::right << r.fit << "  "
            << std::setw(6) << std::left << t << "  "
            << r.cell << std::endl;
      }
   }
   std::cout.unsetf(std::ios::floatfield);
}


// BuildCompareResults -- computes labeled fit results for every cell in
// inputList (ordinal labels "Input 1", "Input 2", ...), ignoring
// blockstart/blocksize entirely: that window exists only to let a web
// front end split a large run into pages, and has no bearing on which
// cells belong together in a COMPARE comparison.
//
// Pulled out of main() so it's directly testable without invoking main()
// itself (which calls exit(0) and can't be unit tested).
std::vector<std::pair<std::string, std::vector<DeloneFitResults>>> BuildCompareResults(
   const std::vector<LatticeCell>& inputList,
   const CmdSellaControls& controls,
   Sella& sella)
{
   std::vector<std::pair<std::string, std::vector<DeloneFitResults>>> compareResults;
   compareResults.reserve(inputList.size());
   for (size_t i = 0; i < inputList.size(); ++i) {
      const std::string label = "Input " + std::to_string(i + 1);
      compareResults.emplace_back(label, ComputeDeloneFits(inputList[i], controls, sella).fits);
   }
   return compareResults;
}
inline DeloneFitResults SellaFitCanonical(
   const std::shared_ptr<GenerateDeloneBase>& sptype,
   const S6& s6) {

   const MatS6 perp = sptype->GetPerp();
   const MatS6 prj = sptype->GetPrj();

   const S6 perpv = perp * s6;
   const double testFit = perpv.norm();
   const bool valid = LRL_Cell_Degrees(prj * s6).IsValid();
   const double bestFit = (valid && testFit >= 1.0E-8) ? testFit
      : (valid ? 0.0 : DBL_MAX);

   const S6 bestv = prj * s6;
   DeloneFitResults temp(bestFit, bestv, perpv, MatS6().unit());
   temp.SetLatticeType(sptype->GetBravaisLatticeGeneral());
   temp.SetDeloneType(sptype->GetBravaisType());
   temp.SetGeneralType(sptype->GetName());
   temp.SetBavaisType(sptype->GetBravaisType());
   return temp;
}


int main(int argc, char* argv[])
{
   //const S6 vecA(-741.21325, -474.29631, -0.09949, 0.19042, -0.07866, -473.88154);
   //const S6 vecB(0.07866, -0.19042, -0.09949, -473.91546, -741.37057, -473.65801);

   //const std::vector<std::shared_ptr<GenerateDeloneBase>> o4type = GenerateDeloneBase().Select("O4");

   //const DeloneFitResults fitA = Sella::SellaFitXXXXXX(o4type[0], vecA);
   //const DeloneFitResults fitB = Sella::SellaFitXXXXXX(o4type[0], vecB);

   //std::cout << ";   [DIAG] vecA fit " << fitA.GetRawFit() << std::endl;
   //std::cout << ";   [DIAG] vecB fit " << fitB.GetRawFit() << std::endl;


   //RunHistoricalBadCaseCheck();
   //exit(0);


   //RunRemediation2HopComparison();
   //   exit(0);

   //RunMpOcTpLastDigitCheck();
   //exit(0);

   //RunzyComparison();
   //exit(0);

   //RunRemediationShortlistSanityCheck();
   //exit(0);

   //RunGeneralSweepOverIncomparablePairsV2();
   //exit(0);

   //RerunFailingPairsWithBestProjector();
   //exit(0);

   //PrintOneSweepEndpoints(Delone_T5(), Delone_O5());
   //exit(0);


   //RunReflectionRescueCheck();
   //exit(0);


   //RunGeneralSweepOverIncomparablePairs();
   //exit(0);


   //RunThreeCalibrationSweeps();
   //exit(0);


   //SweepRemediatedFitTowardA3ViaM1B();
   //exit(0);


   //SweepRemediatedFitTowardA2();
   //exit(0);


   std::cout << "; SELLA method symmetry searching\n";
   WebIO webio(argc, argv, "CmdSella", 0);
#ifdef LRL_DEBUG
   for (size_t ii = 0; ii < argc; ii++) {
      std::cout << ii << ": " << argv[ii] << std::endl;
   }
   std::cout << "webio: " << webio << std::endl;
#endif
   CmdSellaControls controls;

   // TEST -- runs the real regression suite (FlexibleSellaTestMode.h).
   // TEST N / TEST N1 N2 N3 / TEST 999 all handled inside
   // runFlexibleSellaTestMode via controls.getTestNumbers().
   if (controls.shouldRunTests()) {
      runFlexibleSellaTestMode(controls);
      return 0;
   }



   // TESTTRANSFORMS -- standalone diagnostic, not part of the fit-based
   // FlexibleSellaTestMode framework (S6BoundaryTransforms vs
   // Selling::GetBoundaryTransforms operate on raw S6/MatS6, not
   // LatticeCell -> DeloneFitComputation, so they don't fit that shape).
   // See CompareBoundaryTransformsTest.h.
   //if (controls.shouldTestTransforms()) {
   //   std::cout << "; Compare boundary transform sets" << std::endl;
   //   CompareBoundaryTransformSets();
   //   return 0;
   //}

   // NOTE: the ad hoc TestDeloneParentage(O2,O3)/(O1B,O3) sanity block
   // that used to live here (see chat with Claude, 2026-08-15) has been
   // removed -- it was unreachable dead code (shadowed by an exit(0) in
   // the block above it) and, more importantly, redundant: CHECKNESTING
   // already runs TestDeloneParentage generally, across all Delone type
   // pairs, via RunNestingConsistencyCheck -- not just the hand-picked
   // O2/O3 case. Use CHECKNESTING instead. If CHECKNESTING's output
   // turns out not to cover the O1B/O3 "already trusted" comparison the
   // same way this block did, that's a gap in CHECKNESTING worth fixing
   // there, not a reason to keep two parallel mechanisms.

   controls.setHasWebInput(webio.m_hasWebInstructions);
   const int initblockstart = controls.getBlockStart();
   const int initblocksize = controls.getBlockSize();
   const WebFileBlockProgramInput<CmdSellaControls> dc_setup(argc, argv, "CmdSella", 0, controls);
   const size_t blockstart = dc_setup.getBlockStart();
   const size_t blocksize = dc_setup.getBlockSize();
   const size_t blockend = dc_setup.getBlockEnd();
   if (controls.getShowControls()) {
      std::cout << controls << std::endl;
   }
#ifdef LRL_DEBUG
   if (!controls.getShowControls()) {
      std::cout << controls << std::endl;
   }
#endif

   //==============================================================================

   if (controls.shouldUseHarness())
   {
      RunR1R3RemediatedFitCheck();
      // RunItem8NestingAudit() and RunFullNestingSweep() removed
      // (2026-08-29): both used TestDeloneNesting.h's P*Q=Q idempotency
      // test against one fixed canonical projector per type, no boundary
      // crossing -- shown to systematically under-detect real nestings
      // once compared against RunNestingConsistencyCheck (CHECKNESTING),
      // which uses the existing, already-validated TestDeloneParentage
      // (168-candidate VCP search, per Andrews/Bernstein/Sauter). Running
      // both under `harness` produced two different answers to the same
      // question with no signal about which to trust. CHECKNESTING is
      // the one to use; TestDeloneNesting.h/RunItem8NestingAudit.h/
      // RunFullNestingSweep.h are kept on disk but no longer called here.
      exit(0);


      G6 g1("36.972 106.683 106.683 18.486 36.972 36.972");  // tI
      G6 g2("139.722 187.104 187.104 69.861 139.722 139.722");  // tI
      RunStabilityTrace(g1, "M4");
      RunStabilityTrace(g2, "M4");
      exit(0);

      if (controls.shouldUseHarness())
      {
         RunRemediation168Check(controls);
         exit(0);
      }

      if (controls.shouldUseHarness())
      {
         RunRawFitCheck(controls);
         exit(0);
      }


      //RunHistoricalBadCaseCheck(controls);
      RunStabilityVsCurrentComparison(controls);
      //const LatticeCell traceCell("P 21.775 21.785 27.232  90.028  89.974  90.023");
      //MatS6 traceReductionMatrix;
      //const S6 traceLattice = GetInputSellingReducedVectors(traceCell, traceReductionMatrix);
      //RunStabilityTrace(traceLattice, "M2A");
      exit(0);
   }


   if (controls.shouldUseHarness())
   {
      //RunHistoricalBadCaseCheck(controls);
      RunStabilityVsCurrentComparison(controls);
      exit(0);
   }
   if (controls.shouldUseHarness())
   {
      //RunHistoricalBadCaseCheck(controls);
      RunHistorical144And2HopCheck(controls);
      exit(0);
   }

   // TEST -- runs the generalized FlexibleSellaTestMode suite (see
   // FlexibleSellaTestMode.h / SELLA_handoff.md item 5) instead of normal
   // cell processing, mirroring CmdLMP3's runFlexibleTestMode dispatch.
   // This supersedes the older CmdSellaTests.cpp/RunCmdSellaTests
   // NamedTest registry (retired; do not build that file -- see the note
   // at the top of Cmdsellatests.cpp) and the already-superseded
   // Cmdsellacomparetests.cpp -- remove both from CmdSella.vcxproj.
   if (controls.shouldRunTests()) {
      runFlexibleSellaTestMode(controls);
      exit(0);
   }

   Sella sella;
   // NEW: Build matrices ONCE before processing any inputs
    //SearchAllTypeBoundaryDistances();
    //exit(0);

    //SweepRemediatedFitNearR1R3Boundary(controls, sella);
    //exit(0);



   if (controls.shouldBuildMatrices()) {
      SellaBuild builder;
      const bool shouldPrintMatrices = controls.shouldPrintMatrices();
      std::vector<LabeledDeloneTypeMatrices> deloneTypes =
         builder.Build(shouldPrintMatrices, controls.getSeedBoundariesMode());

      std::vector<LabeledSellaMatrices> dynamicPerps;
      std::vector<LabeledSellaMatrices> dynamicProjectors;

      for (const auto& dt : deloneTypes) {
         const std::string label = dt.GetLabel();
         const std::vector<MatS6> perps = dt.GetPerp(label);
         const std::vector<MatS6> prjs = dt.GetPrj(label);
         if (!perps.empty())
            dynamicPerps.push_back(LabeledSellaMatrices(label, perps));
         if (!prjs.empty())
            dynamicProjectors.push_back(LabeledSellaMatrices(label, prjs));
      }

      sella.SetPerps(dynamicPerps);
      sella.SetProjectors(dynamicProjectors);
      std::cout << "; Matrices built once and stored for all inputs" << std::endl;
   }

   if (controls.shouldCheckNesting()) {
      RunRandomSeedSpotCheck();
      RunNestingConsistencyCheck(controls, sella);
      exit(0);
   }

   const std::vector<LatticeCell>& inputList = dc_setup.getInputList();

   if (controls.shouldClassifyDirect()) {
      int cellIndex = 1;
      for (const auto& inputCell : inputList) {
         MatS6 oneReductionMatrix;
         const S6 oneLattice = GetInputSellingReducedVectors(inputCell, oneReductionMatrix);
         const S6 oneErrors = 0.1 * inputCell.getCell();

         std::cout << "; " << inputCell.GetInput() << std::endl;
         const std::vector<DeloneFitResults> fits =
            PopulateDirectFitsForAllTypes(oneLattice, oneErrors, oneReductionMatrix, sella);
         for (const auto& fit : fits) {
            PrintCompact(fit);
         }

         // NEW: Bravais-type fit grouping -- min raw fit per Bravais type
         // across its Delone siblings. CreateMapOFBravaisFits takes a
         // non-const ref (doesn't appear to mutate fits) so a local copy
         // is needed since fits is const here.
         std::vector<DeloneFitResults> fitsForGrouping = fits;
         MapOfBravaisFits bravaisFits;
         bravaisFits.CreateMapOFBravaisFits(fitsForGrouping);
         std::cout << bravaisFits;

         const std::string fitPlotsSvg =
            GenerateDirectFitPlotsSvg(fits, oneLattice, inputCell.getInput());
         const std::string fitPlotsFilename = AddSuffixToAllSvgOccurrences(
            LRL_CreateFileName::Create("SEL", LRL_DataToSVG(cellIndex), "svg", true), "_fit_plots");
         WriteDirectSvg(fitPlotsSvg, fitPlotsFilename);
         std::cout << "; Send Sella Plot to graphics file " << fitPlotsFilename << std::endl;

         const std::string deloneTreeSvg = GenerateDirectDeloneTreeSvg(
            fits, oneLattice, inputCell, controls.getMaxDeltaForMatch());
         const std::string deloneTreeFilename = AddSuffixToAllSvgOccurrences(
            LRL_CreateFileName::Create("SEL", LRL_DataToSVG(cellIndex), "svg", true), "_delonetree");
         WriteDirectSvg(deloneTreeSvg, deloneTreeFilename);
         std::cout << "; Send Sella Plot to graphics file " << deloneTreeFilename << std::endl;
         ++cellIndex;
      }
      exit(0);
   }

   const bool doProduceSellaGraphics = controls.DoGraphics();

   if (controls.shouldCompare()) {
      // COMPARE ignores blockstart/blocksize -- that window exists only for
      // web-batching of large runs and has no bearing on which cells belong
      // together in a comparison. Every cell in inputList is compared.
      const auto compareResults = BuildCompareResults(inputList, controls, sella);
      PrintCompareSummary(compareResults, controls);
   }
   else {
      for (size_t whichCell = blockstart;
         whichCell < inputList.size() && whichCell < blockstart + blocksize; ++whichCell) {
         // Get all three SVGs - pass sella by reference
         const auto [sellaSvg, fitPlotsSvg, deloneTreeSvg] = ProcessSella(
            doProduceSellaGraphics,
            inputList[whichCell],
            dc_setup.getRawFileNames()[whichCell - blockstart],
            controls,
            sella);  // NEW: pass sella
         if (doProduceSellaGraphics) {
            // Write Sella plot
            std::cout << "; Send Sella Plot to graphics file "
               << dc_setup.getFullFileNameAt(whichCell) << std::endl;
            std::cout << "; Send Sella Plot to graphics file "
               << AddSuffixToAllSvgOccurrences(dc_setup.getFullFileNameAt(whichCell), "_fit_plots") << std::endl;
            std::cout << "; Send Sella Plot to graphics file "
               << AddSuffixToAllSvgOccurrences(dc_setup.getFullFileNameAt(whichCell), "_delonetree") << std::endl;
            if (webio.m_hasWebInstructions) {
               SendSellaToFile(sellaSvg, dc_setup.getRawFileNames()[whichCell - blockstart]);
               // Write fit plots immediately after
               const std::string fitPlotsFilename = AddSuffixToAllSvgOccurrences(dc_setup.getRawFileNames()[whichCell - blockstart], "_fit_plots");
               SendSellaToFile(fitPlotsSvg, fitPlotsFilename);
               // Write delone tree plot immediately after
               const std::string deloneTreeFilename = AddSuffixToAllSvgOccurrences(dc_setup.getRawFileNames()[whichCell - blockstart], "_delonetree");
               SendSellaToFile(deloneTreeSvg, deloneTreeFilename);
            }
            else {
               SendSellaToFile(sellaSvg, dc_setup.getBasicFileNames()[whichCell - blockstart]);
               // Write fit plots immediately after
               const std::string fitPlotsFilename = AddSuffixToAllSvgOccurrences(dc_setup.getBasicFileNames()[whichCell - blockstart], "_fit_plots");
               SendSellaToFile(fitPlotsSvg, fitPlotsFilename);
               // Write delone tree plot immediately after
               const std::string deloneTreeFilename = AddSuffixToAllSvgOccurrences(dc_setup.getBasicFileNames()[whichCell - blockstart], "_delonetree");
               SendSellaToFile(deloneTreeSvg, deloneTreeFilename);
            }
         }
      }
   }
   exit(0);
}
