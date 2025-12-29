#include <iostream>
#include <cfloat>
#include <limits>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <sstream>

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "G6.h"
#include "Niggli.h"
#include "B4.h"
#include "EisensteinControls.h"
#include "EdgeTransformationMatrices.h"
#include "ProgramSetup.h"
#include "Eisenstein.h"
#include "ScoredReservoir.h"
#include "TransformationMatrices.h"
#include "Polar.h"
#include "WebIO.h"

static double GetTrace(const G6& g6) {
   return g6[0] + g6[1] + g6[2];
}

struct TraceResult {
   double trace;
   G6 cell;
   int matrix_index;
};

// Find nice axis values
double FindNiceNumber(double range, bool round) {
   double exponent = std::floor(std::log10(range));
   double fraction = range / std::pow(10.0, exponent);
   double niceFraction;

   if (round) {
      if (fraction < 1.5) niceFraction = 1.0;
      else if (fraction < 3.0) niceFraction = 2.0;
      else if (fraction < 7.0) niceFraction = 5.0;
      else niceFraction = 10.0;
   } else {
      if (fraction <= 1.0) niceFraction = 1.0;
      else if (fraction <= 2.0) niceFraction = 2.0;
      else if (fraction <= 5.0) niceFraction = 5.0;
      else niceFraction = 10.0;
   }

   return niceFraction * std::pow(10.0, exponent);
}

struct AxisInfo {
   double min;
   double max;
   double tickSpacing;
   int numTicks;
};

AxisInfo CalculateNiceAxis(double dataMin, double dataMax, int targetTicks = 5) {
   double range = dataMax - dataMin;
   double niceRange = FindNiceNumber(range, false);
   double niceTickSpacing = FindNiceNumber(niceRange / (targetTicks - 1), true);

   double niceMin = std::floor(dataMin / niceTickSpacing) * niceTickSpacing;
   double niceMax = std::ceil(dataMax / niceTickSpacing) * niceTickSpacing;

   int numTicks = static_cast<int>(std::round((niceMax - niceMin) / niceTickSpacing)) + 1;

   return { niceMin, niceMax, niceTickSpacing, numTicks };
}

// Map trace value to Y coordinate with adaptive compression
// Analyzes data distribution to choose appropriate compression
static double TraceToY(double trace, double minTrace, double maxTrace, double height,
   const std::vector<TraceResult>& allData, int numToPlot) {
   if (maxTrace <= minTrace) return height / 2.0;

   // Normalize to 0-1
   double normalized = (trace - minTrace) / (maxTrace - minTrace);

   // Strategy: Look at the distribution of bottom points to decide compression

   // Count how many distinct values are in bottom 20% of data range
   double bottomRangeThreshold = minTrace + (maxTrace - minTrace) * 0.20;
   int pointsInBottom20Percent = 0;
   for (int i = 0; i < numToPlot && allData[i].trace <= bottomRangeThreshold; ++i) {
      pointsInBottom20Percent++;
   }

   // Calculate fraction of points in bottom 20% of range
   double bottomDensity = pointsInBottom20Percent / static_cast<double>(numToPlot);

   // Also look at absolute concentration - how many points in bottom 1% of range
   double bottom1PercentThreshold = minTrace + (maxTrace - minTrace) * 0.01;
   int pointsInBottom1Percent = 0;
   for (int i = 0; i < numToPlot && allData[i].trace <= bottom1PercentThreshold; ++i) {
      pointsInBottom1Percent++;
   }

   // Choose compression based on concentration:
   double power;

   if (pointsInBottom1Percent > numToPlot * 0.05) {
      // More than 5% of points in bottom 1% of range - very clustered
      power = 0.15;  // Very aggressive compression
   } else if (bottomDensity > 0.35) {
      // More than 35% of points in bottom 20% of range - clustered
      power = 0.2;   // Aggressive compression
   } else if (bottomDensity > 0.22) {
      // 22-35% in bottom 20% - moderately clustered
      power = 0.35;  // Moderate compression
   } else if (bottomDensity < 0.15) {
      // Less than 15% in bottom 20% - well distributed
      power = 1.0;   // Linear, no compression
   } else {
      // 15-22% in bottom 20% - interpolate
      double t = (bottomDensity - 0.15) / (0.22 - 0.15);
      power = 1.0 - t * (1.0 - 0.35);
   }

   double compressed = std::pow(normalized, power);

   // Flip so small values are at bottom
   return height * (1.0 - compressed);
}

// Get unique trace matrices - filter to remove permutation duplicates
// Uses a random triclinic cell to identify matrices with identical traces
// Returns vector of unique trace matrices and their original indices
static std::pair<std::vector<Matrix_3x3>, std::vector<int>>
GetUniqueTraceMatrices(const std::vector<Matrix_3x3>& allMatrices) {
   std::cout << "Filtering " << allMatrices.size() << " matrices using random triclinic cell..." << std::endl;

   // Use random generic triclinic cell for filtering
   G6 randomCell(Polar::rand());
   std::cout << "Random test cell: " << randomCell << std::endl;

   // Apply all matrices to random cell and collect traces
   std::vector<std::pair<int, double>> matrixTraces;  // matrix_index, trace
   for (size_t i = 0; i < allMatrices.size(); ++i) {
      G6 transformed = allMatrices[i] * randomCell;
      if (transformed.IsValid()) {
         double trace = GetTrace(transformed);
         matrixTraces.push_back({ static_cast<int>(i), trace });
      }
   }

   // Sort by trace
   std::sort(matrixTraces.begin(), matrixTraces.end(),
      [](const auto& a, const auto& b) { return a.second < b.second; });

   // Keep only matrices with unique trace values
   std::vector<Matrix_3x3> uniqueTraceMatrices;
   std::vector<int> uniqueTraceIndices;
   const double tolerance = 1e-8;

   uniqueTraceMatrices.push_back(allMatrices[matrixTraces[0].first]);
   uniqueTraceIndices.push_back(matrixTraces[0].first);
   double lastTrace = matrixTraces[0].second;

   for (size_t i = 1; i < matrixTraces.size(); ++i) {
      if (std::abs(matrixTraces[i].second - lastTrace) > tolerance) {
         uniqueTraceMatrices.push_back(allMatrices[matrixTraces[i].first]);
         uniqueTraceIndices.push_back(matrixTraces[i].first);
         lastTrace = matrixTraces[i].second;
      }
   }

   std::cout << "Filtered to " << uniqueTraceMatrices.size() << " unique trace matrices (removed "
      << (allMatrices.size() - uniqueTraceMatrices.size()) << " permutation duplicates)" << std::endl;
   std::cout << "Reduction factor: " << std::fixed << std::setprecision(1)
      << (allMatrices.size() / static_cast<double>(uniqueTraceMatrices.size())) << "x" << std::endl;

   return { uniqueTraceMatrices, uniqueTraceIndices };
}

// Generate SVG plot for trace distribution
// Returns the sum of all trace values
static double GenerateTracePlot(const G6& reducedCell, const std::string& filename, int maxTraces) {
   std::cout << "\n" << std::string(80, '=') << "\n";
   std::cout << "GENERATING TRACE DISTRIBUTION PLOT\n";
   std::cout << std::string(80, '=') << "\n";

   auto allMatrices = TransformationMatrices::generateUnimodularMatrices();
   std::cout << "Starting with " << allMatrices.size() << " unimodular matrices..." << std::endl;

   // Get unique trace matrices (removes permutation duplicates)
   auto [uniqueTraceMatrices, uniqueTraceIndices] = GetUniqueTraceMatrices(allMatrices);

   // Now apply unique trace matrices to the actual user's cell
   std::cout << "\nApplying " << uniqueTraceMatrices.size() << " unique trace matrices to user's cell..." << std::endl;

   std::vector<TraceResult> results;
   results.reserve(uniqueTraceMatrices.size());

   for (size_t i = 0; i < uniqueTraceMatrices.size(); ++i) {
      G6 transformed = uniqueTraceMatrices[i] * reducedCell;
      if (transformed.IsValid()) {
         double trace = GetTrace(transformed);
         results.push_back({ trace, transformed, uniqueTraceIndices[i] });
      }
   }

   std::cout << "Valid transformations: " << results.size() << std::endl;

   if (results.empty()) {
      std::cerr << "ERROR: No valid transformations found!" << std::endl;
      return -1.0;
   }

   std::sort(results.begin(), results.end(),
      [](const TraceResult& a, const TraceResult& b) {
         return a.trace < b.trace;
      });

   std::cout << "\nTrace statistics:" << std::endl;
   std::cout << "  Minimum: " << std::fixed << std::setprecision(6)
      << results.front().trace << std::endl;
   std::cout << "  Maximum: " << results.back().trace << std::endl;
   std::cout << "  Median:  " << results[results.size() / 2].trace << std::endl;

   // Limit to maxTraces
   int numToPlot = std::min(maxTraces, static_cast<int>(results.size()));
   std::cout << "  Plotting: " << numToPlot << " of " << results.size() << " unique trace matrices" << std::endl;

   std::cout << "\nTop 10 lowest traces:" << std::endl;
   for (int i = 0; i < std::min(10, numToPlot); ++i) {
      std::cout << "  " << std::setw(3) << i << ": "
         << std::setw(12) << std::fixed << std::setprecision(6)
         << results[i].trace
         << " (matrix #" << results[i].matrix_index << ")" << std::endl;
   }

   // SVG generation - adjusted size for more width
   const int width = 900;     // increased from 675
   const int height = 525;
   const int plotWidth = 525;
   const int plotHeight = 375;
   const int leftMargin = 75;
   const int topMargin = 60;

   double minTrace = results[0].trace;
   double maxTrace = results[numToPlot - 1].trace;

   // Use nice axis values for display labels
   AxisInfo yAxis = CalculateNiceAxis(minTrace, maxTrace, 6);
   AxisInfo xAxis = CalculateNiceAxis(0, numToPlot - 1, 6);

   // But use actual data range for the compression mapping
   // This eliminates white space at bottom
   double dataMin = minTrace;
   double dataMax = maxTrace;

   std::ofstream svg(filename);

   svg << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
   svg << "<svg width=\"" << width << "\" height=\"" << height
      << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";

   // Title
   svg << "  <g id=\"title\" transform=\"translate(0,0)\">\n";
   svg << "    <text x=\"" << width / 2 << "\" y=\"15\" text-anchor=\"middle\" "
      << "font-family=\"Arial\" font-size=\"15\" font-weight=\"bold\">"
      << "Unimodular Matrix Transformation - Trace Distribution</text>\n";
   svg << "    <text x=\"" << width / 2 << "\" y=\"34\" text-anchor=\"middle\" "
      << "font-family=\"Arial\" font-size=\"11\">"
      << "Lowest " << numToPlot << " of " << uniqueTraceMatrices.size() << " Unique Trace Matrices (sorted by trace)</text>\n";
   svg << "    <text x=\"" << width / 2 << "\" y=\"49\" text-anchor=\"middle\" "
      << "font-family=\"Arial\" font-size=\"10\" fill=\"#666666\">"
      << "Input G6: " << std::fixed << std::setprecision(2)
      << reducedCell[0] << " " << reducedCell[1] << " " << reducedCell[2] << " "
      << reducedCell[3] << " " << reducedCell[4] << " " << reducedCell[5]
      << "</text>\n";
   svg << "  </g>\n";

   // Main plot
   svg << "  <g id=\"mainPlot\" transform=\"translate(" << leftMargin << "," << topMargin << ")\">\n";

   svg << "    <rect fill=\"#FFFFFF\" height=\"" << plotHeight
      << "\" stroke=\"#333333\" stroke-width=\"1\" width=\"" << plotWidth
      << "\" x=\"0\" y=\"0\" />\n";

   // Y-axis label (no special characters)
   svg << "    <text x=\"-45\" y=\"" << plotHeight / 2
      << "\" transform=\"rotate(-90,-45," << plotHeight / 2
      << ")\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"11\">"
      << "Trace (a*a + b*b + c*c)</text>\n";

   svg << "    <text fill=\"#000000\" font-family=\"Arial\" font-size=\"11\" "
      << "text-anchor=\"middle\" x=\"" << plotWidth / 2 << "\" y=\"" << (plotHeight + 30)
      << "\">Matrix Index (sorted by trace)</text>\n";

   // Y-axis - draw grid line at actual minimum
   svg << "    <line stroke=\"#DDDDDD\" stroke-width=\"1\" "
      << "x1=\"0\" x2=\"" << plotWidth << "\" "
      << "y1=\"" << plotHeight << "\" y2=\"" << plotHeight << "\" />\n";
   svg << "    <text fill=\"#000000\" font-family=\"Arial\" font-size=\"10\" "
      << "text-anchor=\"end\" x=\"-5\" y=\"" << (plotHeight + 4) << "\">"
      << std::fixed << std::setprecision(1) << dataMin << "</text>\n";

   // Draw nice axis values in between
   for (int i = 0; i < yAxis.numTicks; ++i) {
      double traceValue = yAxis.min + i * yAxis.tickSpacing;
      if (traceValue > yAxis.max + 0.001) break;

      // Skip grid lines below the actual data minimum or above data maximum
      if (traceValue < dataMin - 0.001 || traceValue > dataMax + 0.001) continue;

      // Skip if too close to min or max to avoid label overlap
      if (std::abs(traceValue - dataMin) < yAxis.tickSpacing * 0.3) continue;
      if (std::abs(traceValue - dataMax) < yAxis.tickSpacing * 0.3) continue;

      double yPos = TraceToY(traceValue, dataMin, dataMax, plotHeight, results, numToPlot);

      svg << "    <line stroke=\"#DDDDDD\" stroke-width=\"1\" "
         << "x1=\"0\" x2=\"" << plotWidth << "\" "
         << "y1=\"" << yPos << "\" y2=\"" << yPos << "\" />\n";

      svg << "    <text fill=\"#000000\" font-family=\"Arial\" font-size=\"10\" "
         << "text-anchor=\"end\" x=\"-5\" y=\"" << (yPos + 4) << "\">";

      if (std::abs(traceValue - std::round(traceValue)) < 0.001) {
         svg << std::fixed << std::setprecision(0) << traceValue;
      } else if (yAxis.tickSpacing >= 1.0) {
         svg << std::fixed << std::setprecision(1) << traceValue;
      } else {
         svg << std::fixed << std::setprecision(2) << traceValue;
      }

      svg << "</text>\n";
   }

   // Draw grid line at actual maximum
   svg << "    <line stroke=\"#DDDDDD\" stroke-width=\"1\" "
      << "x1=\"0\" x2=\"" << plotWidth << "\" "
      << "y1=\"0\" y2=\"0\" />\n";
   svg << "    <text fill=\"#000000\" font-family=\"Arial\" font-size=\"10\" "
      << "text-anchor=\"end\" x=\"-5\" y=\"4\">"
      << std::fixed << std::setprecision(1) << dataMax << "</text>\n";

   // X-axis
   for (int i = 0; i < xAxis.numTicks; ++i) {
      int indexValue = static_cast<int>(xAxis.min + i * xAxis.tickSpacing);
      if (indexValue > xAxis.max) break;
      if (indexValue >= numToPlot) indexValue = numToPlot - 1;

      double xPos = (indexValue / static_cast<double>(numToPlot - 1)) * plotWidth;

      // Vertical grid line
      svg << "    <line stroke=\"#DDDDDD\" stroke-width=\"1\" "
         << "x1=\"" << xPos << "\" x2=\"" << xPos << "\" "
         << "y1=\"0\" y2=\"" << plotHeight << "\" />\n";

      // Tick mark
      svg << "    <line stroke=\"#333333\" stroke-width=\"1\" "
         << "x1=\"" << xPos << "\" x2=\"" << xPos << "\" "
         << "y1=\"" << plotHeight << "\" y2=\"" << (plotHeight + 4) << "\" />\n";

      svg << "    <text fill=\"#000000\" font-family=\"Arial\" font-size=\"10\" "
         << "text-anchor=\"middle\" x=\"" << xPos << "\" y=\"" << (plotHeight + 16)
         << "\">" << indexValue << "</text>\n";
   }

   // Draw thin line through points
   svg << "    <polyline fill=\"none\" points=\"";
   for (int i = 0; i < numToPlot; ++i) {
      double xPos = (i / static_cast<double>(numToPlot - 1)) * plotWidth;
      double yPos = TraceToY(results[i].trace, dataMin, dataMax, plotHeight, results, numToPlot);

      if (i > 0) svg << " ";
      svg << std::fixed << std::setprecision(2) << xPos << "," << yPos;
   }
   svg << "\" stroke=\"#56B4E9\" stroke-width=\"1\" />\n";

   // Plot all points as circles on top of line
   for (int i = 0; i < numToPlot; ++i) {
      double xPos = (i / static_cast<double>(numToPlot - 1)) * plotWidth;
      double yPos = TraceToY(results[i].trace, dataMin, dataMax, plotHeight, results, numToPlot);

      svg << "    <circle cx=\"" << std::fixed << std::setprecision(2) << xPos
         << "\" cy=\"" << yPos << "\" fill=\"#56B4E9\" r=\"2.5\" />\n";
   }

   // Add trace value labels for lowest values (within 1% of minimum)
   // Group identical values and show counts
   double onePercentRange = (dataMax - dataMin) * 0.01;
   int labelsAdded = 0;
   const double traceTolerance = 0.01;  // Consider traces identical if within 0.01

   int i = 0;
   while (i < numToPlot && labelsAdded < 5) {
      if (results[i].trace > dataMin + onePercentRange) break;

      // Count how many consecutive points have the same trace (within tolerance)
      int count = 1;
      double currentTrace = results[i].trace;
      while (i + count < numToPlot &&
         std::abs(results[i + count].trace - currentTrace) < traceTolerance) {
         count++;
      }

      // Calculate position for the label (use middle point of the group)
      int midIndex = i + count / 2;
      double xPos = (midIndex / static_cast<double>(numToPlot - 1)) * plotWidth;
      double yPos = TraceToY(results[midIndex].trace, dataMin, dataMax, plotHeight, results, numToPlot);

      // Format the label
      std::ostringstream label;
      label << std::fixed << std::setprecision(2) << currentTrace;
      if (count > 1) {
         label << " (" << count << "x)";
      }

      // Position label: if multiple points, try to place on left
      // But if too close to left edge, place on right instead
      double labelX;
      std::string anchor;

      if (count > 1) {
         // Multiple points - prefer left side to avoid covering points
         // Estimate label width: ~7 pixels per character for font size 8
         double estimatedWidth = label.str().length() * 7;

         if (xPos > estimatedWidth + 10) {
            // Room on left - place there with more offset
            labelX = xPos - 20;
            anchor = "end";
         } else {
            // Not enough room on left - place on right with more offset
            labelX = xPos + 20;
            anchor = "start";
         }
      } else {
         // Single point - place on right as usual
         labelX = xPos + 8;
         anchor = "start";
      }

      svg << "    <text fill=\"#FF8800\" font-family=\"Arial\" font-size=\"8\" "
         << "text-anchor=\"" << anchor << "\" x=\"" << labelX << "\" y=\"" << (yPos + 3) << "\">"
         << label.str() << "</text>\n";

      labelsAdded++;
      i += count;  // Skip past this group
   }

   svg << "  </g>\n";

   // Statistics
   svg << "  <g id=\"statistics\" transform=\"translate(620,75)\">\n";
   svg << "    <text x=\"0\" y=\"0\" font-family=\"Arial\" font-size=\"11\" font-weight=\"bold\">Statistics</text>\n";
   svg << "    <text x=\"0\" y=\"19\" font-family=\"Arial\" font-size=\"9\">Min: "
      << std::fixed << std::setprecision(3) << minTrace << "</text>\n";
   svg << "    <text x=\"0\" y=\"32\" font-family=\"Arial\" font-size=\"9\">Max: "
      << maxTrace << "</text>\n";
   svg << "    <text x=\"0\" y=\"45\" font-family=\"Arial\" font-size=\"9\">Range: "
      << (maxTrace - minTrace) << "</text>\n";
   svg << "    <text x=\"0\" y=\"58\" font-family=\"Arial\" font-size=\"9\">Input: "
      << GetTrace(reducedCell) << "</text>\n";

   double median = results[numToPlot / 2].trace;
   svg << "    <text x=\"0\" y=\"71\" font-family=\"Arial\" font-size=\"9\">Median: "
      << median << "</text>\n";

   // Calculate sum of all traces (for return value, not displayed)
   double sumTraces = 0.0;
   for (int i = 0; i < numToPlot; ++i) {
      sumTraces += results[i].trace;
   }

   svg << "    <text x=\"0\" y=\"91\" font-family=\"Arial\" font-size=\"9\" fill=\"#666666\">Y-axis adaptive</text>\n";
   svg << "    <text x=\"0\" y=\"104\" font-family=\"Arial\" font-size=\"9\" fill=\"#666666\">compression</text>\n";

   svg << "  </g>\n";

   // Detailed list of lowest traces
   svg << "  <g id=\"lowestTraces\" transform=\"translate(620,200)\">\n";
   svg << "    <text x=\"0\" y=\"0\" font-family=\"Arial\" font-size=\"11\" font-weight=\"bold\">Lowest Trace Values</text>\n";

   int numToList = std::min(5, numToPlot);

   // Count how many are within 1%
   int withinOnePercent = 0;
   for (int i = 0; i < numToPlot; ++i) {
      if (results[i].trace <= dataMin + onePercentRange) withinOnePercent++;
      else break;
   }
   numToList = std::min(withinOnePercent, 5);

   int yOffset = 18;
   for (int i = 0; i < numToList; ++i) {
      // Index and trace
      svg << "    <text x=\"0\" y=\"" << yOffset << "\" font-family=\"Arial\" font-size=\"9\" font-weight=\"bold\">"
         << "#" << i << ": " << std::fixed << std::setprecision(3) << results[i].trace << "</text>\n";
      yOffset += 13;

      // G6 values with more precision
      svg << "    <text x=\"5\" y=\"" << yOffset << "\" font-family=\"Arial\" font-size=\"8\" fill=\"#666666\">G6: "
         << std::fixed << std::setprecision(4)
         << results[i].cell[0] << " " << results[i].cell[1] << " " << results[i].cell[2] << "</text>\n";
      yOffset += 11;
      svg << "    <text x=\"18\" y=\"" << yOffset << "\" font-family=\"Arial\" font-size=\"8\" fill=\"#666666\">"
         << results[i].cell[3] << " " << results[i].cell[4] << " " << results[i].cell[5] << "</text>\n";
      yOffset += 15;
   }

   svg << "  </g>\n";

   // Footer
   svg << "  <g id=\"footer\" transform=\"translate(0," << (height - 15) << ")\">\n";
   svg << "    <text x=\"" << width / 2 << "\" y=\"11\" text-anchor=\"middle\" "
      << "font-family=\"Arial\" font-size=\"9\" fill=\"#666666\">"
      << "Eisenstein Reduction Analysis - Duplicate traces removed</text>\n";
   svg << "  </g>\n";

   svg << "</svg>\n";
   svg.close();

   std::cout << "\nSVG plot written to: " << filename << std::endl;
   std::cout << std::string(80, '=') << "\n";

   // Return the sum we already calculated
   return sumTraces;
}

static std::tuple<double, G6, int> TestToConvergence(const LRL_Cell& cell, const int nmax,
   const std::vector<Matrix_3x3>& matrices, const double tolerance) {

   G6 best = cell;
   double minTrace = GetTrace(best);
   int iterations = 0;

   for (int loop = 0; loop < nmax; ++loop) {
      ++iterations;
      G6 candidateBest = best;
      double candidateMinTrace = minTrace;

      for (const auto& m : matrices) {
         const G6 transformed = m * best;
         if (!transformed.IsValid()) continue;
         const double trace = GetTrace(transformed);

         if (trace < candidateMinTrace) {
            candidateMinTrace = trace;
            candidateBest = transformed;
         }
      }

      if (std::abs(candidateMinTrace - minTrace) < tolerance) break;

      minTrace = candidateMinTrace;
      best = candidateBest;
   }

   return { minTrace, best, iterations };
}

struct ReductionResult {
   std::string method;
   double trace;
   G6 reduced;
   int iterations;
   double time_ms;
};

std::vector<ReductionResult> CompareReductionMethods(const LRL_Cell& cell, const int nmax, const double tolerance) {
   std::vector<ReductionResult> results;

   {
      auto start = std::chrono::high_resolution_clock::now();
      const auto [edgeTrace, edgeG6, edgeCycles] =
         TestToConvergence(cell, nmax, GetSimpleEdgeTransforms(), tolerance);
      auto end = std::chrono::high_resolution_clock::now();
      double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

      results.push_back({ "Eisenstein12", edgeTrace, edgeG6, edgeCycles, time_ms });
   }

   {
      auto start = std::chrono::high_resolution_clock::now();
      const auto [allTrace, allG6, allCycles] =
         TestToConvergence(cell, nmax, GetAllTransforms(), tolerance);
      auto end = std::chrono::high_resolution_clock::now();
      double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

      results.push_back({ "Eisenstein24", allTrace, allG6, allCycles, time_ms });
   }

   {
      G6 niggliInput = cell;
      G6 niggliOut;

      auto start = std::chrono::high_resolution_clock::now();
      bool success = Niggli::ReduceWithoutMatrices(niggliInput, niggliOut, tolerance);
      auto end = std::chrono::high_resolution_clock::now();
      double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

      double trace = GetTrace(niggliOut);
      results.push_back({ "Niggli", trace, niggliOut, success ? 1 : 0, time_ms });
   }

   return results;
}

void PrintSummary(const std::vector<ReductionResult>& results) {
   std::cout << "\n" << std::string(80, '=') << "\n";
   std::cout << "REDUCTION RESULTS\n";
   std::cout << std::string(80, '=') << "\n";

   std::cout << std::left << std::setw(20) << "Method"
      << std::right << std::setw(15) << "Trace"
      << std::setw(12) << "Iterations"
      << std::setw(12) << "Time (ms)" << "\n";
   std::cout << std::string(60, '-') << "\n";

   for (const auto& r : results) {
      std::cout << std::left << std::setw(20) << r.method
         << std::right << std::fixed << std::setprecision(6)
         << std::setw(15) << r.trace
         << std::setw(12) << r.iterations
         << std::setw(12) << std::setprecision(3) << r.time_ms << "\n";
   }

   std::cout << "\nReduced cells (G6):\n";
   for (const auto& r : results) {
      std::cout << "  " << std::left << std::setw(15) << r.method << ": " << r.reduced << "\n";
   }
   std::cout << std::string(80, '=') << "\n";
}

int main(int argc, char* argv[]) {
   const std::string programName("Eisenstein");
   std::cout << "; Eisenstein Reduction with Trace Distribution Analysis" << std::endl;

   const int nmax = 500;
   const double tolerance = 1.0E-8;

   try {
      EisenControls controls;
      const BasicProgramInput<EisenControls> dc_setup("Eisenstein", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      // Get number of inputs and create WebIO with that many files
      const size_t numInputs = dc_setup.getInputList().size();
      WebIO webio(argc, argv, programName, numInputs);
      webio.CreateFilenamesAndLinks(numInputs, controls.getPrefix());

      // Track sum of first plot for comparison
      double firstSum = 0.0;
      std::vector<double> allSums;
      allSums.reserve(numInputs);

      // Process each input cell
      for (size_t inputIndex = 0; inputIndex < numInputs; ++inputIndex) {
         const auto& input = dc_setup.getInputList()[inputIndex];
         const LRL_Cell cell = input.getCell();

         std::cout << "\n" << std::string(80, '=') << "\n";
         std::cout << "Processing input " << (inputIndex + 1) << " of " << numInputs << "\n";
         std::cout << std::string(80, '=') << "\n";

         std::cout << "Input cell (P): ";
         for (size_t i = 0; i < 6; ++i) {
            std::cout << std::setprecision(6) << LRL_Cell_Degrees(cell)[i] << " ";
         }
         std::cout << std::endl;

         const G6 g6cell = cell;
         std::cout << "Input cell (G6): " << g6cell << std::endl;
         std::cout << "Input trace: " << std::setprecision(8) << GetTrace(g6cell) << std::endl;

         auto results = CompareReductionMethods(cell, nmax, tolerance);
         PrintSummary(results);

         // Generate plot with the WebIO-generated filename for this input
         const std::string& graphicsFileName = webio.m_FileNameList[inputIndex];
         std::cout << "\n; Eisenstein graphics file " << webio.m_FullfileNameList[inputIndex] << std::endl;
         double sumTraces = GenerateTracePlot(results[1].reduced, graphicsFileName, controls.getMaxTraces());

         // Store sum (for internal use, not displayed)
         allSums.push_back(sumTraces);

         // Store first sum for comparison
         if (inputIndex == 0) {
            firstSum = sumTraces;
         }
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}
