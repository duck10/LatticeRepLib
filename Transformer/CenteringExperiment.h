// FILE: CenteringExperiment.h
// Header for lattice centering and matching using gradient descent

#ifndef CENTERING_EXPERIMENT_H
#define CENTERING_EXPERIMENT_H

#include "LRL_Cell.h"
#include "Matrix_3x3.h"
#include <vector>

struct CenteringStep {
   LRL_Cell current_cell;
   LRL_Cell target_cell;
   Matrix_3x3 applied_matrix;
   double p3_distance;
   int step_number;
};

struct MatchingStep {
   LRL_Cell mobile_cell;
   LRL_Cell reference_cell;
   Matrix_3x3 applied_matrix;
   double p3_distance;
   int step_number;
};

// Core functions
std::vector<Matrix_3x3> getAllTransformationMatrices();
LRL_Cell applyMatrixToCellA(const LRL_Cell& input_cell, const Matrix_3x3& matrix);

// Centering functions
std::vector<CenteringStep> findCenteringPath(const LRL_Cell& input_cell, int max_iterations = 200);
void analyzeCenteringPath(const std::vector<CenteringStep>& path);

// Matching functions
std::vector<MatchingStep> findMatchingPath(const LRL_Cell& mobile_cell, const LRL_Cell& reference_cell, int max_iterations = 200);
void analyzeMatchingPath(const std::vector<MatchingStep>& path);

// Convenience test functions
void testCenteringExperiment(const LRL_Cell& test_cell);
void testMatchingExperiment(const LRL_Cell& mobile_cell, const LRL_Cell& reference_cell);

#endif // CENTERING_EXPERIMENT_H

