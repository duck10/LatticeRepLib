// FILE: CenteringExperiment.cpp
// Experimental code for lattice centering and matching using gradient descent

#include "LRL_Cell.h"
#include "Niggli.h"
#include "Matrix_3x3.h"
#include "NiggliMatrices.h"
#include "TransformationMatrices.h"
#include "MatrixSets_B4Matcher.h"
#include "P3.h"
#include "TransformerUtilities.h"
#include <vector>
#include <iostream>
#include <algorithm>

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

std::vector<Matrix_3x3> getAllTransformationMatrices() {
   std::vector<Matrix_3x3> all_matrices;

   // Add all Niggli matrices AND their inverses
   for (const auto& matrix : NiggliMatrices::ALL_NIGGLI_MATRICES) {
      all_matrices.push_back(matrix);

      try {
         Matrix_3x3 inverse = matrix.Inverse();
         if (std::abs(inverse.Det()) > 1e-10) {
            if (matrix != inverse) {
               all_matrices.push_back(inverse);
            }
         }
      }
      catch (...) {
         continue;
      }
   }

   // Add all 24 crystallographic transformations 
   auto permutations = MatrixSets::getAllPermutationMatrices();
   for (const auto& matrix : permutations) {
      all_matrices.push_back(matrix);
   }

   // Add shear matrices
   for (const auto& matrix : MatrixSets::SHEARS) {
      all_matrices.push_back(matrix);

      try {
         Matrix_3x3 inverse = matrix.Inverse();
         if (std::abs(inverse.Det()) > 1e-10) {
            if (matrix != inverse) {
               all_matrices.push_back(inverse);
            }
         }
      }
      catch (...) {
         continue;
      }
   }

   // Add complex matrices
   for (const auto& matrix : MatrixSets::COMPLEX) {
      all_matrices.push_back(matrix);

      try {
         Matrix_3x3 inverse = matrix.Inverse();
         if (std::abs(inverse.Det()) > 1e-10) {
            if (matrix != inverse) {
               all_matrices.push_back(inverse);
            }
         }
      }
      catch (...) {
         continue;
      }
   }

   // Add B4Matcher matrices
   all_matrices.push_back(MatrixSets::B4_CRITICAL);
   all_matrices.push_back(MatrixSets::B4_SHEAR_XY_NEG_XZ);
   all_matrices.push_back(MatrixSets::B4_NEG_XY);
   all_matrices.push_back(MatrixSets::B4_NEG_XZ);
   all_matrices.push_back(MatrixSets::B4_NEG_YZ);

   return all_matrices;
}

std::vector<CenteringStep> findCenteringPath(const LRL_Cell& input_cell, int max_iterations = 200) {
   std::vector<CenteringStep> centering_path;

   // Calculate Niggli-reduced target
   G6 g6_input = input_cell;
   G6 g6_reduced;
   bool success = Niggli::Reduce(g6_input, g6_reduced);

   if (!success) {
      std::cout << "ERROR: Failed to compute Niggli reduction!" << std::endl;
      return centering_path;
   }

   LRL_Cell niggli_target(g6_reduced);

   // Initialize current state
   LRL_Cell current_cell = input_cell;
   double current_distance = calculateP3Distance(current_cell, niggli_target);

   std::cout << "=== CELL SPACE CENTERING EXPERIMENT ===" << std::endl;
   std::cout << "Input cell: " << LRL_Cell_Degrees(input_cell) << std::endl;
   std::cout << "Niggli target: " << LRL_Cell_Degrees(niggli_target) << std::endl;
   std::cout << "Initial P3 distance: " << current_distance << std::endl;
   std::cout << std::endl;

   // Record initial state
   CenteringStep initial_step;
   initial_step.current_cell = current_cell;
   initial_step.target_cell = niggli_target;
   initial_step.applied_matrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
   initial_step.p3_distance = current_distance;
   initial_step.step_number = 0;
   centering_path.push_back(initial_step);

   // Get all transformation matrices
   auto all_matrices = getAllTransformationMatrices();
   std::cout << "Testing " << all_matrices.size() << " transformation matrices per iteration" << std::endl;
   std::cout << std::endl;

   // Iterative improvement
   for (int iteration = 1; iteration <= max_iterations; ++iteration) {
      Matrix_3x3 best_matrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1); // Identity matrix
      LRL_Cell best_cell = current_cell;
      double best_distance = current_distance;
      bool found_improvement = false;

      // Test all matrices
      for (const auto& matrix : all_matrices) {
         try {
            LRL_Cell test_cell = applyMatrixToCell(current_cell, matrix);
            double test_distance = calculateP3Distance(test_cell, niggli_target);

            if (test_distance < best_distance) {
               best_matrix = matrix;
               best_cell = test_cell;
               best_distance = test_distance;
               found_improvement = true;
            }
         }
         catch (const std::exception& e) {
            continue;
         }
      }

      if (!found_improvement) {
         std::cout << "No improvement found at iteration " << iteration << ". Stopping." << std::endl;
         break;
      }

      // Update current state
      current_cell = best_cell;
      current_distance = best_distance;

      // Record this step
      CenteringStep step;
      step.current_cell = current_cell;
      step.target_cell = niggli_target;
      step.applied_matrix = best_matrix;
      step.p3_distance = current_distance;
      step.step_number = iteration;
      centering_path.push_back(step);

      // Show progress with transformed cell
      if (iteration % 10 == 0 || iteration <= 5 || current_distance < 0.1) {
         std::cout << "Step " << iteration << ": P3 distance = " << current_distance << std::endl;
         std::cout << "  Transformed mobile: " << LRL_Cell_Degrees(current_cell) << std::endl;
      }

      // Check convergence
      if (current_distance < 1e-8) {
         std::cout << "Reached Niggli target! Final distance: " << current_distance << std::endl;
         break;
      }
   }

   return centering_path;
}

std::vector<MatchingStep> findMatchingPath(const LRL_Cell& mobile_cell, const LRL_Cell& reference_cell, int max_iterations = 200) {
   std::vector<MatchingStep> matching_path;

   // Initialize current state
   LRL_Cell current_mobile = mobile_cell;
   double current_distance = calculateP3Distance(current_mobile, reference_cell);

   std::cout << "=== LATTICE MATCHING EXPERIMENT ===" << std::endl;
   std::cout << "Mobile cell: " << LRL_Cell_Degrees(mobile_cell) << std::endl;
   std::cout << "Reference cell: " << LRL_Cell_Degrees(reference_cell) << std::endl;
   std::cout << "Initial P3 distance: " << current_distance << std::endl;
   std::cout << std::endl;

   // Record initial state
   MatchingStep initial_step;
   initial_step.mobile_cell = current_mobile;
   initial_step.reference_cell = reference_cell;
   initial_step.applied_matrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1); // Identity matrix
   initial_step.p3_distance = current_distance;
   initial_step.step_number = 0;
   matching_path.push_back(initial_step);

   // Get all transformation matrices
   auto all_matrices = getAllTransformationMatrices();
   std::cout << "Testing " << all_matrices.size() << " transformation matrices per iteration" << std::endl;
   std::cout << std::endl;

   // Iterative improvement
   for (int iteration = 1; iteration <= max_iterations; ++iteration) {
      Matrix_3x3 best_matrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      LRL_Cell best_mobile = current_mobile;
      double best_distance = current_distance;
      bool found_improvement = false;

      // Test all matrices on mobile cell
      for (const auto& matrix : all_matrices) {
         try {
            LRL_Cell test_mobile = applyMatrixToCell(current_mobile, matrix);
            double test_distance = calculateP3Distance(test_mobile, reference_cell);
            std::cout << LRL_Cell_Degrees(test_mobile) << std::endl;
            if (test_distance < best_distance) {
               best_matrix = matrix;
               best_mobile = test_mobile;
               best_distance = test_distance;
               found_improvement = true;
            }
         }
         catch (const std::exception& e) {
            continue;
         }
      }

      if (!found_improvement) {
         std::cout << "No improvement found at iteration " << iteration << ". Stopping." << std::endl;
         break;
      }

      // Update current state
      current_mobile = best_mobile;
      current_distance = best_distance;

      // Record this step
      MatchingStep step;
      step.mobile_cell = current_mobile;
      step.reference_cell = reference_cell;
      step.applied_matrix = best_matrix;
      step.p3_distance = current_distance;
      step.step_number = iteration;
      matching_path.push_back(step);

      // Show progress with transformed cell
      if (iteration % 10 == 0 || iteration <= 5 || current_distance < 0.1) {
         std::cout << "Step " << iteration << ": P3 distance = " << current_distance << std::endl;
         std::cout << "  Transformed cell: " << LRL_Cell_Degrees(current_mobile) << std::endl;
      }

      // Check convergence
      if (current_distance < 1e-8) {
         std::cout << "Cells matched! Final distance: " << current_distance << std::endl;
         break;
      }
   }

   return matching_path;
}

void analyzeCenteringPath(const std::vector<CenteringStep>& path) {
   std::cout << "\n=== CENTERING PATH ANALYSIS ===" << std::endl;
   std::cout << "Total steps: " << path.size() - 1 << std::endl;

   if (path.size() < 2) {
      std::cout << "No centering steps taken." << std::endl;
      return;
   }

   double initial_distance = path[0].p3_distance;
   double final_distance = path.back().p3_distance;
   double improvement = initial_distance - final_distance;
   double improvement_percent = (improvement / initial_distance) * 100.0;

   std::cout << "Initial P3 distance: " << initial_distance << std::endl;
   std::cout << "Final P3 distance: " << final_distance << std::endl;
   std::cout << "Total improvement: " << improvement << " (" << improvement_percent << "%)" << std::endl;

   // Show key milestones
   std::cout << "\nKey milestones:" << std::endl;
   for (size_t i = 0; i < path.size(); ++i) {
      const auto& step = path[i];
      if (i < 5 || i % 10 == 0 || i >= path.size() - 3) {
         std::cout << "Step " << step.step_number << ": P3 = " << step.p3_distance;
         if (i > 0) {
            double step_improvement = path[i - 1].p3_distance - step.p3_distance;
            std::cout << " (improved by " << step_improvement << ")";
         }
         std::cout << std::endl;
      }
   }
}

void analyzeMatchingPath(const std::vector<MatchingStep>& path) {
   std::cout << "\n=== MATCHING PATH ANALYSIS ===" << std::endl;
   std::cout << "Total steps: " << path.size() - 1 << std::endl;

   if (path.size() < 2) {
      std::cout << "No matching steps taken." << std::endl;
      return;
   }

   double initial_distance = path[0].p3_distance;
   double final_distance = path.back().p3_distance;
   double improvement = initial_distance - final_distance;
   double improvement_percent = (improvement / initial_distance) * 100.0;

   std::cout << "Initial P3 distance: " << initial_distance << std::endl;
   std::cout << "Final P3 distance: " << final_distance << std::endl;
   std::cout << "Total improvement: " << improvement << " (" << improvement_percent << "%)" << std::endl;

   // Show key milestones with cell transformations
   std::cout << "\nKey milestones:" << std::endl;
   for (size_t i = 0; i < path.size(); ++i) {
      const auto& step = path[i];
      if (i < 5 || i % 10 == 0 || i >= path.size() - 3) {
         std::cout << "Step " << step.step_number << ": P3 = " << step.p3_distance;
         if (i > 0) {
            double step_improvement = path[i - 1].p3_distance - step.p3_distance;
            std::cout << " (improved by " << step_improvement << ")";
         }
         std::cout << std::endl;
         std::cout << "  Mobile cell: " << LRL_Cell_Degrees(step.mobile_cell) << std::endl;
      }
   }
}

// Convenience functions for testing
void testCenteringExperiment(const LRL_Cell& test_cell) {
   auto centering_path = findCenteringPath(test_cell);
   analyzeCenteringPath(centering_path);
}

void testMatchingExperiment(const LRL_Cell& mobile_cell, const LRL_Cell& reference_cell) {
   auto matching_path = findMatchingPath(mobile_cell, reference_cell);
   analyzeMatchingPath(matching_path);
}
