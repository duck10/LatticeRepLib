// EisensteinReduction.h
#ifndef EISENSTEIN_REDUCTION_H
#define EISENSTEIN_REDUCTION_H

#include "LRL_Cell.h"
#include "LRL_Vector3.h"
#include "B4.h"
#include "G6.h"
#include "Niggli.h"
#include "TransformationMatrices.h"
#include "EdgeTransformationMatrices.h"
#include <vector>
#include <limits>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <set>

class EisensteinReduction {
public:
   struct ReductionResult {
      B4 reducedCell;                    // The reduced cell (base vectors)
      G6 reducedG6;                      // The reduced G6
      double trace;                       // g1 + g2 + g3
      int iterations;                     // Number of iterations to convergence
      size_t transformationsTested;       // Total transformations tested

      ReductionResult() :
         trace(std::numeric_limits<double>::max()),
         iterations(0),
         transformationsTested(0) {
      }
   };

   enum class SearchStrategy {
      FIRST_IMPROVEMENT,
      BEST_IN_SWEEP,
      BEST_CUMULATIVE,
      GRADIENT_GUIDED,
      ADAPTIVE_BY_STATE
   };

   // Main reduction functions (default to ADAPTIVE)
   static ReductionResult Reduce(const LRL_Cell& inputCell, double tolerance = 1.0e-6);
   static ReductionResult Reduce(const B4& inputB4, double tolerance = 1.0e-6);
   static ReductionResult Reduce(const G6& inputG6, double tolerance = 1.0e-6);

   // Quiet versions for bulk testing
   static ReductionResult ReduceQuiet(const B4& inputB4, double tolerance = 1.0e-6);
   static ReductionResult ReduceQuiet(const G6& inputG6, double tolerance = 1.0e-6);

   // Strategy-based reduction (for comparison testing)
   static ReductionResult ReduceWithStrategy(const G6& inputG6,
      SearchStrategy strategy,
      double tolerance = 1.0e-6,
      bool verbose = false);

   // Cell state analysis based on angle deviation from [60°-120°] range
   struct CellStateMetrics {
      double maxAbsCosine;        // max(|cos(?)|, |cos(?)|, |cos(?)|)
      double angleDeviation;       // How far angles are from [60°-120°] range
      double trace;                // Sum of squared lengths

      enum class State {
         VERY_CLOSE_TO_REDUCED,   // All angles in [60°, 120°]
         MODERATELY_SKEWED,       // Some angles outside ideal range
         HIGHLY_SKEWED            // Angles near 0° or 180° (nearly degenerate)
      };

      State state;

      static CellStateMetrics Analyze(const G6& g6);
      State ClassifyState() const;
      int RecommendedStartingComplexity() const;
      int RecommendedNextComplexity(bool firstIteration = false) const;
      std::string StateToString() const;
   };

   // Transformation catalog organized by complexity
   class TransformationCatalog {
   public:
      std::vector<Matrix_3x3> allMatrices;
      std::vector<size_t> level1_simple;    // Simple transformations (a?a±b, etc.)
      std::vector<size_t> level2_medium;    // Medium complexity
      std::vector<size_t> level3_all;       // All transformations

      TransformationCatalog();
      std::vector<size_t> GetByComplexity(int level) const;
      size_t CountByComplexity(int level) const;

   private:
      void CategorizeMatrices();
      int ClassifyComplexity(const Matrix_3x3& M) const;
   };

   // Adaptive reduction based on cell state (recommended default)
   static ReductionResult ReduceAdaptive(const G6& inputG6,
      double tolerance,
      bool verbose);

   // BEST_CUMULATIVE strategy (for comparison testing)
   static ReductionResult ReduceBestCumulative(const G6& inputG6,
      double tolerance = 1.0e-6,
      bool verbose = false);

private:
   // Core reduction engine
   struct ReductionContext {
      B4 currentBest;
      double currentTrace;
      int iteration;
      size_t totalTransformationsTested;
      bool converged;

      ReductionContext(const B4& initial, double trace)
         : currentBest(initial), currentTrace(trace),
         iteration(0), totalTransformationsTested(0), converged(false) {
      }
   };

   // Core reduction step - tests matrices and updates context
   static bool PerformReductionStep(
      ReductionContext& ctx,
      const std::vector<size_t>& matricesToTest,
      const std::vector<Matrix_3x3>& allMatrices,
      bool useCascade,
      double tolerance);

   // Reporting helpers
   static void ReportIterationStart(int iteration, double trace,
      const CellStateMetrics* metrics = nullptr,
      int complexity = 0);

   static void ReportIterationEnd(double previousTrace, double newTrace,
      bool improved, size_t numTested);

   static void ReportFinalResults(const ReductionContext& ctx,
      const std::string& strategyName);

   // Helper functions
   static double ComputeTrace(const B4& cell);
   static B4 ApplyTransformation(const B4& input, const Matrix_3x3& transform);

   // Original reduction with strategy (kept for backward compatibility)
   static ReductionResult ReduceInternal(const B4& inputB4, double tolerance, bool verbose);
   static ReductionResult ReduceInternalWithStrategy(const B4& inputB4,
      SearchStrategy strategy,
      double tolerance,
      bool verbose);
};

// ===================================================================
// IMPLEMENTATION: Cell State Metrics
// ===================================================================

inline EisensteinReduction::CellStateMetrics
EisensteinReduction::CellStateMetrics::Analyze(const G6& g6) {
   CellStateMetrics metrics;

   // Calculate cosines from G6
   // cos(?) = g4 / (2?(g2·g3)), etc.
   double cos_alpha = g6[3] / (2.0 * std::sqrt(g6[1] * g6[2]));
   double cos_beta = g6[4] / (2.0 * std::sqrt(g6[0] * g6[2]));
   double cos_gamma = g6[5] / (2.0 * std::sqrt(g6[0] * g6[1]));

   metrics.maxAbsCosine = std::max({
       std::abs(cos_alpha),
       std::abs(cos_beta),
       std::abs(cos_gamma)
      });

   // Angle deviation from allowed range [-0.5, 0.5]
   // Reduced cells have angles in [60°, 120°] ? cos ? [-0.5, 0.5]
   auto deviationFromRange = [](double cosine) {
      if (cosine < -0.5) return -0.5 - cosine;  // Too obtuse (>120°)
      if (cosine > 0.5) return cosine - 0.5;    // Too acute (<60°)
      return 0.0;                                // In range [60°-120°]
      };

   metrics.angleDeviation =
      deviationFromRange(cos_alpha) +
      deviationFromRange(cos_beta) +
      deviationFromRange(cos_gamma);

   metrics.trace = g6[0] + g6[1] + g6[2];
   metrics.state = metrics.ClassifyState();

   return metrics;
}

inline EisensteinReduction::CellStateMetrics::State
EisensteinReduction::CellStateMetrics::ClassifyState() const {
   // Very close: all angles in range [60°-120°]
   if (angleDeviation < 0.01 && maxAbsCosine <= 0.51) {
      return State::VERY_CLOSE_TO_REDUCED;
   }

   // Highly skewed: angles approaching degenerate (near 0° or 180°)
   if (maxAbsCosine > 0.9 || angleDeviation > 0.5) {
      return State::HIGHLY_SKEWED;
   }

   // Everything else
   return State::MODERATELY_SKEWED;
}

inline int EisensteinReduction::CellStateMetrics::RecommendedStartingComplexity() const {
   switch (state) {
   case State::VERY_CLOSE_TO_REDUCED: return 1;
   case State::MODERATELY_SKEWED: return 2;
   case State::HIGHLY_SKEWED: return 3;
   }
   return 3;
}

inline int EisensteinReduction::CellStateMetrics::RecommendedNextComplexity(bool firstIteration) const {
   if (firstIteration) {
      // First iteration: use conservative recommendation
      return RecommendedStartingComplexity();
   }

   // Subsequent iterations: be more aggressive
   // After first iteration, cell should be significantly improved
   switch (state) {
   case State::VERY_CLOSE_TO_REDUCED:
      return 1;  // Only simple transformations

   case State::MODERATELY_SKEWED:
      return 1;  // More aggressive - try simple first

   case State::HIGHLY_SKEWED:
      return 2;  // More aggressive - try medium complexity
   }
   return 2;
}

inline std::string EisensteinReduction::CellStateMetrics::StateToString() const {
   switch (state) {
   case State::VERY_CLOSE_TO_REDUCED: return "VERY_CLOSE";
   case State::MODERATELY_SKEWED: return "MODERATE";
   case State::HIGHLY_SKEWED: return "HIGHLY_SKEWED";
   }
   return "UNKNOWN";
}

// ===================================================================
// IMPLEMENTATION: Transformation Catalog
// ===================================================================

inline EisensteinReduction::TransformationCatalog::TransformationCatalog() {
   allMatrices = TransformationMatrices::generateUnimodularMatrices();
   CategorizeMatrices();
}

inline std::vector<size_t>
EisensteinReduction::TransformationCatalog::GetByComplexity(int level) const {
   switch (level) {
   case 1: return level1_simple;
   case 2: return level2_medium;
   case 3:
   default: return level3_all;
   }
}

inline size_t
EisensteinReduction::TransformationCatalog::CountByComplexity(int level) const {
   return GetByComplexity(level).size();
}

inline void EisensteinReduction::TransformationCatalog::CategorizeMatrices() {
   for (size_t i = 0; i < allMatrices.size(); ++i) {
      const auto& M = allMatrices[i];
      int complexity = ClassifyComplexity(M);

      if (complexity == 1) {
         level1_simple.push_back(i);
      }
      if (complexity <= 2) {
         level2_medium.push_back(i);
      }
      level3_all.push_back(i);
   }

   //std::cout << "Transformation catalog built:" << std::endl;
   //std::cout << "  Level 1 (simple): " << level1_simple.size() << " matrices" << std::endl;
   //std::cout << "  Level 2 (medium): " << level2_medium.size() << " matrices" << std::endl;
   //std::cout << "  Level 3 (all): " << level3_all.size() << " matrices" << std::endl;
}

inline int
EisensteinReduction::TransformationCatalog::ClassifyComplexity(const Matrix_3x3& M) const {
   // Count changes from identity matrix
   int numChanges = 0;
   int maxAbsValue = 0;

   for (int i = 0; i < 9; ++i) {
      int val = (int)M[i];
      bool isChange = (i % 4 == 0) ? (val != 1) : (val != 0);

      if (isChange) {
         numChanges++;
         maxAbsValue = std::max(maxAbsValue, std::abs(val));
      }
   }

   // Simple: 1-3 changes, small values
   // Examples: a?a+b, b?b-c, single swaps
   if (numChanges <= 3 && maxAbsValue <= 1) {
      return 1;
   }

   // Medium: 4-6 changes or moderate values
   // Examples: a?a+b-c, combined operations
   if (numChanges <= 6 && maxAbsValue <= 1) {
      return 2;
   }

   // Complex: everything else
   return 3;
}

// ===================================================================
// IMPLEMENTATION: Helper Functions
// ===================================================================

inline double EisensteinReduction::ComputeTrace(const B4& cell) {
   LRL_Cell tempCell(cell);
   G6 g6(tempCell);
   return g6[0] + g6[1] + g6[2];
}

inline B4 EisensteinReduction::ApplyTransformation(const B4& input, const Matrix_3x3& transform) {
   const Vector_3& a = input[0];
   const Vector_3& b = input[1];
   const Vector_3& c = input[2];

   B4 result;
   result[0] = transform[0] * a + transform[3] * b + transform[6] * c;
   result[1] = transform[1] * a + transform[4] * b + transform[7] * c;
   result[2] = transform[2] * a + transform[5] * b + transform[8] * c;
   result[3] = -(result[0] + result[1] + result[2]);

   return result;
}

// ===================================================================
// IMPLEMENTATION: Core Reduction Engine
// ===================================================================

inline bool EisensteinReduction::PerformReductionStep(
   ReductionContext& ctx,
   const std::vector<size_t>& matricesToTest,
   const std::vector<Matrix_3x3>& allMatrices,
   bool useCascade,
   double tolerance)
{
   bool foundImprovement = false;
   B4 sweepBest = ctx.currentBest;
   double sweepBestTrace = ctx.currentTrace;
   B4 currentTestTarget = ctx.currentBest;

   for (size_t matrixIndex : matricesToTest) {
      const auto& transform = allMatrices[matrixIndex];
      B4 testCell = useCascade ? currentTestTarget : ctx.currentBest;
      B4 transformedCell = ApplyTransformation(testCell, transform);

      // Validate transformed cell
      LRL_Cell tempCell(transformedCell);
      if (!tempCell.IsValid()) {
         ctx.totalTransformationsTested++;
         continue;
      }

      G6 transformedG6(tempCell);
      double trace = transformedG6[0] + transformedG6[1] + transformedG6[2];
      ctx.totalTransformationsTested++;

      if (trace < sweepBestTrace - tolerance) {
         sweepBest = transformedCell;
         sweepBestTrace = trace;
         foundImprovement = true;

         if (useCascade) {
            currentTestTarget = transformedCell;  // Cascade effect
         }
      }
   }

   if (foundImprovement) {
      ctx.currentBest = sweepBest;
      ctx.currentTrace = sweepBestTrace;
   }

   return foundImprovement;
}

// ===================================================================
// IMPLEMENTATION: Reporting Helpers
// ===================================================================

inline void EisensteinReduction::ReportIterationStart(
   int iteration,
   double trace,
   const CellStateMetrics* metrics,
   int complexity)
{
   if (metrics) {
      std::cout << "Iteration " << iteration
         << ": trace=" << std::fixed << std::setprecision(2) << trace
         << ", maxCos=" << std::setprecision(3) << metrics->maxAbsCosine
         << ", state=" << metrics->StateToString()
         << ", complexity=" << complexity
         << std::endl;
   } else {
      std::cout << "Iteration " << iteration
         << ": trace=" << trace << std::endl;
   }
}

inline void EisensteinReduction::ReportIterationEnd(
   double previousTrace,
   double newTrace,
   bool improved,
   size_t numTested)
{
   if (improved) {
      std::cout << "  Best in sweep: " << previousTrace
         << " -> " << newTrace
         << " (reduction: " << (previousTrace - newTrace) << ")"
         << " [tested " << numTested << " matrices]"
         << std::endl;
   } else {
      std::cout << "  No improvement found after testing "
         << numTested << " matrices" << std::endl;
   }
}

inline void EisensteinReduction::ReportFinalResults(
   const ReductionContext& ctx,
   const std::string& strategyName)
{
   std::cout << "\nEisenstein reduction (" << strategyName
      << ") converged after " << ctx.iteration << " iterations" << std::endl;
   std::cout << "  Final trace: " << ctx.currentTrace << std::endl;
   std::cout << "  Total transformations tested: " << ctx.totalTransformationsTested << std::endl;
   if (ctx.iteration > 0) {
      std::cout << "  Average per iteration: "
         << (double)ctx.totalTransformationsTested / ctx.iteration << std::endl;
   }
}

// ===================================================================
// IMPLEMENTATION: Adaptive Reduction (RECOMMENDED DEFAULT)
// ===================================================================

inline EisensteinReduction::ReductionResult
EisensteinReduction::ReduceAdaptive(const G6& inputG6, double tolerance, bool verbose)
{
   // Validate input for near-degenerate angles
   double cos_alpha = std::abs(inputG6[3]) / (2.0 * std::sqrt(inputG6[1] * inputG6[2]));
   double cos_beta = std::abs(inputG6[4]) / (2.0 * std::sqrt(inputG6[0] * inputG6[2]));
   double cos_gamma = std::abs(inputG6[5]) / (2.0 * std::sqrt(inputG6[0] * inputG6[1]));
   const double cos_5_degrees = 0.9962;

   if (cos_alpha >= cos_5_degrees || cos_beta >= cos_5_degrees || cos_gamma >= cos_5_degrees) {
      if (verbose) {
         std::cout << "ERROR: Input G6 has near-degenerate angle (< 5° or > 175°)" << std::endl;
      }
      return ReductionResult();
   }

   const LRL_Cell cell(inputG6);
   if (!cell.IsValid()) {
      if (verbose) std::cout << "ERROR: Input cell is invalid" << std::endl;
      return ReductionResult();
   }

   B4 inputB4(cell);

   // Get matrices and catalog (static - only initialized once)
   static std::vector<Matrix_3x3> matrices =
      TransformationMatrices::generateUnimodularMatrices();
   static TransformationCatalog catalog;

   // Analyze initial state
   CellStateMetrics initialMetrics = CellStateMetrics::Analyze(inputG6);

   if (verbose) {
      std::cout << "Eisenstein reduction: ADAPTIVE_BY_STATE" << std::endl;
      std::cout << "Initial cell state analysis:" << std::endl;
      std::cout << "  Trace: " << initialMetrics.trace << std::endl;
      std::cout << "  Max |cos(angle)|: " << initialMetrics.maxAbsCosine << std::endl;
      std::cout << "  Angle deviation: " << initialMetrics.angleDeviation << std::endl;
      std::cout << "  State: " << initialMetrics.StateToString() << std::endl;
      std::cout << "  Starting complexity level: "
         << initialMetrics.RecommendedStartingComplexity() << std::endl;
   }

   // Initialize reduction context
   double initialTrace = inputG6[0] + inputG6[1] + inputG6[2];
   ReductionContext ctx(inputB4, initialTrace);
   const int maxIterations = 1000;

   // Main reduction loop
   while (!ctx.converged && ctx.iteration < maxIterations) {
      double previousTrace = ctx.currentTrace;

      // Get current state
      LRL_Cell currentCell(ctx.currentBest);
      G6 currentG6(currentCell);
      CellStateMetrics metrics = CellStateMetrics::Analyze(currentG6);

      // Update complexity based on current state and iteration number
      bool isFirstIteration = (ctx.iteration == 0);
      int currentComplexityLevel = metrics.RecommendedNextComplexity(isFirstIteration);
      std::vector<size_t> matricesToTest = catalog.GetByComplexity(currentComplexityLevel);

      if (verbose) {
         ReportIterationStart(ctx.iteration, ctx.currentTrace, &metrics, currentComplexityLevel);
         std::cout << "  Testing " << matricesToTest.size()
            << " matrices (complexity level " << currentComplexityLevel << ")"
            << std::endl;
      }

      // Perform reduction step (with cascade)
      bool improved = PerformReductionStep(ctx, matricesToTest, matrices, true, tolerance);

      if (verbose) {
         ReportIterationEnd(previousTrace, ctx.currentTrace, improved, matricesToTest.size());
      }

      ctx.converged = !improved;
      ctx.iteration++;
   }

   if (verbose) {
      ReportFinalResults(ctx, "ADAPTIVE_BY_STATE");
   }

   // Build result
   ReductionResult result;
   result.reducedCell = ctx.currentBest;
   result.iterations = ctx.iteration;
   result.trace = ctx.currentTrace;
   result.transformationsTested = ctx.totalTransformationsTested;

   // Apply normalization
   LRL_Cell tempCell(ctx.currentBest);
   G6 unnormalizedG6(tempCell);

   if (verbose) {
      std::cout << "  Before normalization: " << unnormalizedG6 << std::endl;
   }

   G6 normalizedG6;
   Niggli::MKnormWithoutMatrices(unnormalizedG6, normalizedG6, tolerance);
   result.reducedG6 = normalizedG6;

   if (verbose) {
      std::cout << "  After normalization:  " << normalizedG6 << std::endl;
   }

   return result;
}

// Add after ReduceAdaptive, before the public interface section

// ===================================================================
// IMPLEMENTATION: BEST_CUMULATIVE Strategy (for comparison)
// ===================================================================

inline EisensteinReduction::ReductionResult
EisensteinReduction::ReduceBestCumulative(const G6& inputG6, double tolerance, bool verbose)
{
   // Validate input for near-degenerate angles
   double cos_alpha = std::abs(inputG6[3]) / (2.0 * std::sqrt(inputG6[1] * inputG6[2]));
   double cos_beta = std::abs(inputG6[4]) / (2.0 * std::sqrt(inputG6[0] * inputG6[2]));
   double cos_gamma = std::abs(inputG6[5]) / (2.0 * std::sqrt(inputG6[0] * inputG6[1]));
   const double cos_5_degrees = 0.9962;

   if (cos_alpha >= cos_5_degrees || cos_beta >= cos_5_degrees || cos_gamma >= cos_5_degrees) {
      if (verbose) {
         std::cout << "ERROR: Input G6 has near-degenerate angle (< 5° or > 175°)" << std::endl;
      }
      return ReductionResult();
   }

   const LRL_Cell cell(inputG6);
   if (!cell.IsValid()) {
      if (verbose) std::cout << "ERROR: Input cell is invalid" << std::endl;
      return ReductionResult();
   }

   B4 inputB4(cell);

   // Get all matrices (static - only initialized once)
   static std::vector<Matrix_3x3> matrices =
      TransformationMatrices::generateUnimodularMatrices();

   if (verbose) {
      std::cout << "Eisenstein reduction: BEST_CUMULATIVE" << std::endl;
      std::cout << "Testing all " << matrices.size() << " matrices per iteration" << std::endl;
   }

   // Initialize reduction context
   double initialTrace = inputG6[0] + inputG6[1] + inputG6[2];
   ReductionContext ctx(inputB4, initialTrace);
   const int maxIterations = 1000;

   // Create test order - all matrices in default order
   std::vector<size_t> allMatrices;
   allMatrices.reserve(matrices.size());
   for (size_t i = 0; i < matrices.size(); ++i) {
      allMatrices.push_back(i);
   }

   // Main reduction loop
   while (!ctx.converged && ctx.iteration < maxIterations) {
      double previousTrace = ctx.currentTrace;

      if (verbose) {
         std::cout << "Iteration " << ctx.iteration
            << ": trace=" << ctx.currentTrace << std::endl;
         std::cout << "  Testing all " << matrices.size() << " matrices" << std::endl;
      }

      // Perform reduction step - ALWAYS test ALL matrices with cascade
      bool improved = PerformReductionStep(ctx, allMatrices, matrices, true, tolerance);

      if (verbose) {
         ReportIterationEnd(previousTrace, ctx.currentTrace, improved, matrices.size());
      }

      ctx.converged = !improved;
      ctx.iteration++;
   }

   if (verbose) {
      ReportFinalResults(ctx, "BEST_CUMULATIVE");
   }

   // Build result
   ReductionResult result;
   result.reducedCell = ctx.currentBest;
   result.iterations = ctx.iteration;
   result.trace = ctx.currentTrace;
   result.transformationsTested = ctx.totalTransformationsTested;

   // Apply normalization
   LRL_Cell tempCell(ctx.currentBest);
   G6 unnormalizedG6(tempCell);

   if (verbose) {
      std::cout << "  Before normalization: " << unnormalizedG6 << std::endl;
   }

   G6 normalizedG6;
   Niggli::MKnormWithoutMatrices(unnormalizedG6, normalizedG6, tolerance);
   result.reducedG6 = normalizedG6;

   if (verbose) {
      std::cout << "  After normalization:  " << normalizedG6 << std::endl;
   }

   return result;
}

// ===================================================================
// IMPLEMENTATION: Public Interface (use Adaptive as default)
// ===================================================================

inline EisensteinReduction::ReductionResult
EisensteinReduction::Reduce(const LRL_Cell& inputCell, double tolerance) {
   return ReduceAdaptive(G6(inputCell), tolerance, false);
}

inline EisensteinReduction::ReductionResult
EisensteinReduction::Reduce(const G6& inputG6, double tolerance) {
   return ReduceAdaptive(inputG6, tolerance, false);
}

inline EisensteinReduction::ReductionResult
EisensteinReduction::ReduceQuiet(const B4& inputB4, double tolerance) {
   LRL_Cell cell(inputB4);
   G6 inputG6(cell);
   return ReduceAdaptive(inputG6, tolerance, false);
}

inline EisensteinReduction::ReductionResult
EisensteinReduction::ReduceQuiet(const G6& inputG6, double tolerance) {
   return ReduceAdaptive(inputG6, tolerance, false);
}

// ===================================================================
// IMPLEMENTATION: Strategy-based reduction (for backward compatibility)
// ===================================================================

inline EisensteinReduction::ReductionResult
EisensteinReduction::ReduceInternal(const B4& inputB4, double tolerance, bool verbose) {
   G6 inputG6=(LRL_Cell(inputB4));
   return ReduceAdaptive(inputG6, tolerance, verbose);
}

inline EisensteinReduction::ReductionResult
EisensteinReduction::ReduceWithStrategy(const G6& inputG6,
   SearchStrategy strategy,
   double tolerance,
   bool verbose) {
   switch (strategy) {
   case SearchStrategy::ADAPTIVE_BY_STATE:
      return ReduceAdaptive(inputG6, tolerance, verbose);

   case SearchStrategy::BEST_CUMULATIVE:
      return ReduceBestCumulative(inputG6, tolerance, verbose);

   case SearchStrategy::FIRST_IMPROVEMENT:
   case SearchStrategy::BEST_IN_SWEEP:
   case SearchStrategy::GRADIENT_GUIDED:
      // These would require separate implementations
      if (verbose) {
         std::cout << "NOTE: Strategy not fully implemented, using BEST_CUMULATIVE" << std::endl;
      }
      return ReduceBestCumulative(inputG6, tolerance, verbose);

   default:
      if (verbose) {
         std::cout << "NOTE: Unknown strategy, using ADAPTIVE" << std::endl;
      }
      return ReduceAdaptive(inputG6, tolerance, verbose);
   }
}


inline EisensteinReduction::ReductionResult
EisensteinReduction::ReduceInternalWithStrategy(const B4& inputB4,
   SearchStrategy strategy,
   double tolerance,
   bool verbose) {
   G6 inputG6=(LRL_Cell(inputB4));
   return ReduceWithStrategy(inputG6, strategy, tolerance, verbose);
}

#endif // EISENSTEIN_REDUCTION_H