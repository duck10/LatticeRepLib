/* define LRL_DEBUG 1 */

#include <algorithm>
#include <cctype>
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
#include "ConventionalCell.h"
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

#include "LabeledDeloneTypeMatrices.h"
#include "DeloneFitComputation.h"
#include "FaceCrossedArrangements.h"
#include "FlexibleSellaTestMode.h"
#include "G6.h"
#include "LRL_Cell.h"
#include "MatG6.h"

//#include "DiagnoseO2Gap.h"
//#include "DiagnoseBoundaryCandidates.h"
#include "CheckABReflection.h"
//#include "DiagnoseO1BCoverage.h"
//#include "SearchAllBoundaryTies.h"
//#include "SearchBoundaryNeighborsByTransform.h"

#include "TestHarness.h"


#include "StoreResults.h"
#include "RemediatedFit.h"



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

inline std::string FormatS6Precise(const S6& s, const int precision) {
   std::ostringstream os;
   os << std::fixed << std::setprecision(precision) << "S";
   for (size_t i = 0; i < 6; ++i) os << " " << s[i];
   return os.str();
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


// PrintConventionalCells (2026-11-08) -- for each matched type, two cells in
// the conventional setting of its Bravais type (ConventionalCell.h):
//   idealized -- the projected fit, with the type's exact symmetry;
//   input -- the input lattice itself in the same setting.
// mC is C-centered with b unique; hR is given in the rhombohedral setting.
// For mC with C-centered beta above MONOCLINIC_I_BETA_THRESHOLD (120), the I-centered
// cells are printed as well.
// Each cell line has the same layout as the projected-fit lines (cell, then
// labels), so it can be passed to the other programs.
// Basis lines (2026-11-09): after each pair of cells, a comment line gives the
// 3x3 whose rows are the conventional A, B, C in terms of the cell as typed
// (AddTypedBasis). typedToReduced is the matrix from
// GetInputSellingReducedVectors; typedS6 is the typed cell itself.
const double TYPED_BASIS_TOLERANCE = 1.0E-8;

static void PrintTypedBasisLine(const Matrix3& M, const double roundTripError,
   const double gramError) {
   std::cout << ";   basis " << FormatBasisMatrix(M)
      << "   (rows A B C in terms of the typed a b c)";
   if (roundTripError > TYPED_BASIS_TOLERANCE || gramError > TYPED_BASIS_TOLERANCE) {
      std::cout << "  CHECK FAILED: round trip " << roundTripError
         << ", Gram " << gramError;
   }
   std::cout << std::endl;
}

static void PrintConventionalCells(const std::vector<DeloneFitResults>& vFilteredDeloneFitResults,
   const S6& LatI, const double maxDeltaForMatch,
   const MatS6& typedToReduced, const S6& typedS6) {
   const std::set<std::string> matchedNames =
      GetMatchedTypeNames(vFilteredDeloneFitResults, LatI.norm(), maxDeltaForMatch);
   std::cout << "; conventional cells (idealized = projected fit; input = the input lattice in the same setting)"
      << std::endl;
   for (const auto& fit : vFilteredDeloneFitResults) {
      if (matchedNames.find(fit.GetGeneralType()) == matchedNames.end()) continue;
      ConventionalCellResult r = ConventionalCellsForFit(fit);
      AddTypedBasis(r, fit, typedToReduced, typedS6);
      if (!r.ok) {
         std::cout << "; " << fit.GetGeneralType() << " " << fit.GetBravaisType()
            << ": no conventional cell (canonical arrangement not reached)" << std::endl;
         continue;
      }
      std::cout << FormatConventionalCell(r.ideal) << "  " << fit.GetGeneralType()
         << " " << fit.GetBravaisType() << " idealized" << std::endl;
      std::cout << FormatConventionalCell(r.input) << "  " << fit.GetGeneralType()
         << " " << fit.GetBravaisType() << " input" << std::endl;
      PrintTypedBasisLine(r.typedToConventional, r.typedBasisRoundTripError, r.typedBasisGramError);
      if (r.hasAlternateI) {
         std::cout << FormatConventionalCell(r.idealI) << "  " << fit.GetGeneralType()
            << " " << fit.GetBravaisType() << " idealized" << std::endl;
         std::cout << FormatConventionalCell(r.inputI) << "  " << fit.GetGeneralType()
            << " " << fit.GetBravaisType() << " input" << std::endl;
         PrintTypedBasisLine(r.typedToConventionalI, r.typedBasisRoundTripError, r.typedBasisGramError);
      }
   }
}

// PrintRhombohedralReading (2026-11-09) -- for R or H input, says how the
// cell was read. LatSymMatG6 chooses by the cell's shape (hexagonal axes if
// the cell is closer to a = b, 90, 90, 120 than to a = b = c with equal
// angles), not by the letter, and assumes the obverse setting. The decision
// is taken from LatSymMatG6 itself (unit matrix = rhombohedral axes), so this
// line cannot disagree with the reduction.
static void PrintRhombohedralReading(const LatticeCell& input) {
   const std::string lattice = input.getLatticeType();
   if (lattice.empty()) return;
   const char letter = static_cast<char>(std::toupper(static_cast<unsigned char>(lattice[0])));
   if (letter != 'R' && letter != 'H') return;
   const MatG6 mLattice = LRL_Cell::LatSymMatG6(lattice, input.getCell());
   if (mLattice.IsUnit()) {
      std::cout << "; " << letter << " input read as rhombohedral axes (primitive cell)" << std::endl;
   }
   else {
      std::cout << "; " << letter << " input read as rhombohedral centering on hexagonal axes (obverse)"
         << std::endl;
   }
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

   std::cout << "; " << input.GetInput() << " input data" << std::endl;
   PrintRhombohedralReading(input);
   std::cout << std::endl;

   std::cout << "Fits for Types" << std::endl;
   for (const auto& fit : vDeloneFitResultsForOneInputLattice) {
      if (!controls.shouldShowType(fit.GetGeneralType())) continue;
      std::cout << fit.GetBravaisType() << " ";
      std::cout << fit.GetDeloneType() << " ";
      std::cout << fit.GetGeneralType() << " ";
      if (controls.useExtendedPrecision()) {
         const int p = controls.getOutputPrecision();
         std::cout << std::fixed << std::setprecision(p) << fit.GetRawFit() << std::endl;
         std::cout << ";    candidate " << FormatS6Precise(fit.GetWinningCandidate(), p) << std::endl;
         std::cout << ";    projected " << FormatS6Precise(fit.GetBestFit(), p) << std::endl;
         std::cout << std::defaultfloat << std::setprecision(6);
      }
      else {
         std::cout << fit.GetRawFit() << " " << std::endl;
      }
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
      // the cell as typed (not made primitive); GetInputSellingReducedVectors
      // reduces the same input.getCell() with input.getLatticeType()
      const S6 typedS6 = S6(G6(input.getCell()));
      PrintConventionalCells(vFilteredDeloneFitResults, oneLattice, controls.getMaxDeltaForMatch(),
         oneReductionMatrix, typedS6);
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

int main(int argc, char* argv[])
{
   std::cout << "; SELLA method symmetry searching\n";
   WebIO webio(argc, argv, "CmdSella", 0);
#ifdef LRL_DEBUG
   for (size_t ii = 0; ii < argc; ii++) {
      std::cout << ii << ": " << argv[ii] << std::endl;
   }
   std::cout << "webio: " << webio << std::endl;
#endif
   CmdSellaControls controls;

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
   const WebFileBlockProgramInput<CmdSellaControls> dc_setup(argc, argv, "CmdSella", 0, controls);
   Sella::SetUseFaceCrossed(controls.shouldUseFaceCrossed());
   Sella::SetUseConeFit(controls.shouldUseConeFit());
   if (Sella::GetUseFaceCrossed()) {
      const size_t problems = VerifyFaceCrossedArrangements(std::cout);
      std::cout << "; FACECROSSED on: " << CountFaceCrossedArrangements()
         << " face arrangements for 11 types, "
         << ((problems == 0) ? "all verified" : "PROBLEMS FOUND") << std::endl;
   }
   if (!Sella::GetUseConeFit()) {
      std::cout << "; CONEFIT off: plain subspace projection" << std::endl;
   }
   if (!controls.shouldRemediate()) {
      std::cout << "; REMEDIATION off: direct fits only" << std::endl;
   }

   // Block limits and file names (Oct 2026), computed here from main's own
   // WebIO -- the same pattern as Follow -- because CmdSella writes THREE
   // files per cell (main plot, _fit_plots, _delonetree), and the web
   // limit counts files, not cells. With graphics off no files are
   // written, so no file cap applies. Desktop runs have no limit.
   const size_t filesPerCell = controls.DoGraphics() ? 3 : 0;
   webio.m_blockstart = controls.getBlockStart();
   webio.m_blocksize = controls.getBlockSize();
   webio.ApplyBlockLimits(dc_setup.getInputList().size(),
      webio.m_hasWebInstructions || controls.getWebRun(), filesPerCell);
   webio.CreateFilenamesAndLinks(dc_setup.getInputList().size(), controls.getPrefix());
   const size_t blockstart = webio.m_blockstart;
   const size_t blockend = webio.m_blockstart + webio.m_blocksize;
   if (controls.getShowControls()) {
      std::cout << controls << std::endl;
   }
#ifdef LRL_DEBUG
   if (!controls.getShowControls()) {
      std::cout << controls << std::endl;
   }
#endif

   //==============================================================================


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

   if (controls.shouldCheckNesting()) {
      RunRandomSeedSpotCheck();
      RunNestingConsistencyCheck(controls, sella);
      exit(0);
   }


   const std::vector<LatticeCell>& inputList = dc_setup.getInputList();


   if (controls.shouldCheckRemediation()) {
      RunRemediationCheck(inputList, controls);
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
      // File name lists hold only the current block: entry k belongs to
      // cell blockstart + k.
      for (size_t whichCell = blockstart; whichCell < blockend; ++whichCell) {
         const size_t k = whichCell - blockstart;
         const std::string& rawFileName = webio.m_FileNameList[k];
         const std::string& basicFileName = webio.m_basicfileNameList[k];
         const std::string& fullFileName = webio.m_FullfileNameList[k];

         // Get all three SVGs - pass sella by reference
         const auto [sellaSvg, fitPlotsSvg, deloneTreeSvg] = ProcessSella(
            doProduceSellaGraphics,
            inputList[whichCell],
            rawFileName,
            controls,
            sella);  // NEW: pass sella
         if (doProduceSellaGraphics) {
            // Write Sella plot
            std::cout << "; Send Sella Plot to graphics file "
               << fullFileName << std::endl;
            std::cout << "; Send Sella Plot to graphics file "
               << AddSuffixToAllSvgOccurrences(fullFileName, "_fit_plots") << std::endl;
            std::cout << "; Send Sella Plot to graphics file "
               << AddSuffixToAllSvgOccurrences(fullFileName, "_delonetree") << std::endl;

            // Web runs write under --rawprefix; desktop runs write the bare name.
            const std::string& outName = webio.m_hasWebInstructions ? rawFileName : basicFileName;
            SendSellaToFile(sellaSvg, outName);
            // Write fit plots immediately after
            SendSellaToFile(fitPlotsSvg, AddSuffixToAllSvgOccurrences(outName, "_fit_plots"));
            // Write delone tree plot immediately after
            SendSellaToFile(deloneTreeSvg, AddSuffixToAllSvgOccurrences(outName, "_delonetree"));
         }
      }
   }
   exit(0);

}