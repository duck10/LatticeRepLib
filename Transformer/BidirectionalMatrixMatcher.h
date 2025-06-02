#ifndef BIDIRECTIONAL_MATRIX_MATCHER_H
#define BIDIRECTIONAL_MATRIX_MATCHER_H

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "Matrix_3x3.h"
#include "P3.h"
#include "MultiTransformFinderControls.h"
#include "TransformationMatrices.h"
#include "NiggliMatrices.h"
#include "MatrixSets_B4Matcher.h"
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <iomanip>
#include <cmath>

struct SearchStep {
   int iteration;
   std::string transformName;
   LRL_Cell cell;
   double distance;

   SearchStep(int iter, const std::string& name, const LRL_Cell& c, double dist)
      : iteration(iter), transformName(name), cell(c), distance(dist) {
   }
};

struct BidirectionalResult {
   bool converged;
   LRL_Cell finalForwardCell;
   LRL_Cell finalBackwardCell;
   std::vector<SearchStep> forwardPath;
   std::vector<SearchStep> backwardPath;
   double finalConvergenceDistance;
   int iterations;

   BidirectionalResult() : converged(false), finalConvergenceDistance(0.0), iterations(0) {}
};

class BidirectionalMatrixMatcher {
private:
   std::vector<Matrix_3x3> transformationMatrices;
   std::vector<std::string> transformationNames;
   double convergenceThreshold;
   int maxIterations;
   const MultiTransformFinderControls& m_controls;  // Add this
   // Generate comprehensive transformation matrix set
   void generateTransformationMatrices();

   // Apply transformation matrix to a cell
   LRL_Cell applyTransformation(const LRL_Cell& cell, const Matrix_3x3& transform) const;

   // Find best transformation for a given cell towards a target
   std::pair<LRL_Cell, std::string> findBestTransformation(
      const LRL_Cell& currentCell,
      const LRL_Cell& targetCell,
      double& bestDistance) const;

   // Print detailed step information
   void printStepInfo(const std::string& stepType, int iteration,
      const LRL_Cell& currentCell, double distance) const;

public:
   BidirectionalMatrixMatcher(double distanceThreshold, int maxIter, const MultiTransformFinderControls& controls);

   // Main bidirectional search algorithm
   BidirectionalResult search(const LRL_Cell& mobileCell, const LRL_Cell& referenceCell) const;

   // Run test case and print results
   bool runTestCase(const std::string& caseName,
      const LRL_Cell& mobileCell,
      const LRL_Cell& referenceCell) const;

   // Print summary of a bidirectional result
   void printResult(const std::string& caseName, const BidirectionalResult& result) const;

   // Getters/Setters
   void setConvergenceThreshold(double threshold) { convergenceThreshold = threshold; }
   void setMaxIterations(int maxIter) { maxIterations = maxIter; }
   double getConvergenceThreshold() const { return convergenceThreshold; }
   int getMaxIterations() const { return maxIterations; }
   size_t getTransformationCount() const { return transformationMatrices.size(); }
};

#endif // BIDIRECTIONAL_MATRIX_MATCHER_H
