#include "P3_Reduce.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "LRL_Cell_Degrees.h"
#include "B4.h"
#include "TransformationMatrices.h"
#include "P3Utilities.h"



constexpr double kTolerance = 1.0e-5;

constexpr bool debug = false;

static const Matrix_3x3 identity{ 1,0,0, 0,1,0, 0,0,1 };

static bool IsFullyUpperHalfPlane(const P3& p) {
   for (const auto& [_, y] : p)
      if (y < 0.0) return false;
   return true;
}

Matrix_3x3 ProjectionSubtractionMatrix(const size_t i, const size_t j, const double lambda) {
   Matrix_3x3 M = identity; // Start with identity

   // Modify row i to subtract lambda * row j
   for (size_t col = 0; col < 3; ++col) {
      size_t idx_i = i * 3 + col;
      size_t idx_j = j * 3 + col;
      M[idx_i] -= lambda * M[idx_j];
   }

   return M;
}

Matrix_3x3 CanonicalMatrix(const P3& input) {
   std::array<P3::Vec2, 3> transformed = input.GetVector();
   Matrix_3x3 M_reflect = identity;

   // Step 1: Reflect negative y components
   for (size_t i = 0; i < 3; ++i) {
      const auto& [x, y] = transformed[i];
      if (y < 0.0) {
         transformed[i] = { -x, -y };
         M_reflect[i * 3 + 0] *= -1.0;
         M_reflect[i * 3 + 1] *= -1.0;
      }
   }

   // Step 2: Sort by magnitude
   std::array<std::pair<double, size_t>, 3> mags;
   for (size_t i = 0; i < 3; ++i) {
      const auto& [x, y] = transformed[i];
      mags[i] = { std::sqrt(x * x + y * y), i };
   }

   std::sort(mags.begin(), mags.end());

   Matrix_3x3 M_sort; // permutation matrix
   for (size_t i = 0; i < 3; ++i) {
      size_t src_row = mags[i].second;
      for (size_t j = 0; j < 3; ++j) {
         M_sort[i * 3 + j] = M_reflect[src_row * 3 + j];
      }
   }

   return M_sort;
}

static const double M_PI = 4.0 * atan(1.0);
static const double M_PI_2 = M_PI / 2.0;    // π/2 ≈ 1.5708 (90 degrees)
static const double M_PI_4 = M_PI / 4.0;    // π/4 ≈ 0.7854 (45 degrees)



P3 P3_Reduce::EnforceUpperHalfPlane(const P3& p) {
   P3 result = p;
   for (auto& [x, y] : result) {
      if (y < 0.0) {
         x = -x;
         y = -y;
      }
   }
   return result;
}


//double P3_Reduce::Cost(const P3& p) {
//   const double dot12 = std::abs(Scalar::Dot(p[0], p[1]));
//   const double dot13 = std::abs(Scalar::Dot(p[0], p[2]));
//   const double dot23 = std::abs(Scalar::Dot(p[1], p[2]));
//
//   const double magSum =
//      Scalar::MagnitudeSquared(p[0]) +
//      Scalar::MagnitudeSquared(p[1]) +
//      Scalar::MagnitudeSquared(p[2]);
//
//   return magSum < 1e-10 ? 0.0 : (dot12 + dot13 + dot23) / magSum;
//}

double P3_Reduce::Scalar::Projection(const std::pair<double, double>& pi,
   const std::pair<double, double>& pj) {
   const double dot = Dot(pi, pj);
   const double magSq = MagnitudeSquared(pj);
   return magSq < 1e-10 ? 0.0 : dot / magSq;
}

double P3_Reduce::Scalar::Dot(const std::pair<double, double>& a,
   const std::pair<double, double>& b) {
   return a.first * b.first + a.second * b.second;
}

double P3_Reduce::Scalar::MagnitudeSquared(const std::pair<double, double>& v) {
   return v.first * v.first + v.second * v.second;
}

bool P3_Reduce::CosineSignsUniform(const P3& p) {
   int pos = 0, neg = 0;
   for (const auto& [x, _] : p) {
      if (x < 0) ++neg;
      else       ++pos;
   }
   return (pos == 3 || neg == 3);
}

P3 P3_Reduce::StandardPresentation(const P3& input) {
   std::array<std::pair<double, P3::Vec2>, 3> sorted;
   for (std::size_t i = 0; i < 3; ++i) {
      const auto& [x, y] = input[i];
      const double mag = std::sqrt(x * x + y * y);
      sorted[i] = { mag, { x, y } };
   }

   std::sort(sorted.begin(), sorted.end(),
      [](const auto& a, const auto& b) { return a.first < b.first; });

   P3 result;
   for (std::size_t i = 0; i < 3; ++i)
      result[i] = sorted[i].second;

   if (result[0].first > 0.0) {
      result[0].first = -result[0].first;
      result[2].first = -result[0].first;
   }

   if (result[1].first > 0.0) {
      result[1].first = -result[1].first;
      result[2].first = -result[0].first;
   }

   return result;
}

P3 P3_Reduce::Reduce(const P3& input) {
   const LRL_Cell cell(input);
   if (!cell.IsValid()) {
      return P3();
   }
   const double originalVolume = cell.Volume();
   P3 current = StandardPresentation(input);
   double bestCost = P3Cost(current);

   bool improved = true;

   while (improved) {
      improved = false;

      for (size_t i = 0; i < 3; ++i) {
         for (size_t j = 0; j < 3; ++j) {
            if (i == j) continue;

            const double lambda = Scalar::Projection(current[i], current[j]);
            P3 candidate = current;

            candidate[i].first -= lambda * candidate[j].first;
            candidate[i].second -= lambda * candidate[j].second;

            candidate = StandardPresentation(candidate);

            // Geometry filter — skip early if invalid
            if (!LRL_Cell(candidate).CheckValid())
               continue;

            const double newCost = P3Cost(candidate);
            const double newVolume = LRL_Cell(candidate).Volume();

            const bool accept =
               (newCost + kTolerance < bestCost) &&
               (std::abs(newVolume - originalVolume) < kTolerance);

            if (accept) {
               current = candidate;
               bestCost = newCost;
               improved = true;
               break;
            }
         }
         if (improved) break;
      }
   }

   // Final quadrant enforcement and sorting
   return CanonicalPresentation(current);
}

P3 P3_Reduce::CanonicalPresentation(const P3& input) {
   P3 result;

   // Step 1: Flip all vectors into upper-half-plane
   for (std::size_t i = 0; i < 3; ++i) {
      const auto& [x, y] = input[i];
      result[i] = (y < 0.0) ? P3::Vec2{ -x, -y } : P3::Vec2{ x, y };
   }

   // Step 2: Sort by magnitude (after flipping)
   std::array<std::pair<double, P3::Vec2>, 3> sorted;
   for (std::size_t i = 0; i < 3; ++i) {
      const auto& [x, y] = result[i];
      const double mag = std::sqrt(x * x + y * y);
      sorted[i] = { mag, { x, y } };
   }

   std::sort(sorted.begin(), sorted.end(),
      [](const auto& a, const auto& b) { return a.first < b.first; });

   // Step 3: Output the clean triplet
   for (std::size_t i = 0; i < 3; ++i)
      result[i] = sorted[i].second;

   return result;
}

double P3_Reduce::P3Cost(const P3& p) {
   static constexpr int choice = 0;
   if (choice == 0) {
      return P3ScalarAbsCost(p);
   }
   //else if (choice == 1) { // to find the smallest negative sum, clearly nonsense results
   //   return P3ScalarCost(p);
   //}
   else if (choice == 2) {
      return P3TraceCost(p);
   }
   else if (choice == 3) {
      return ComputeMetricCost(p);
   }
   else {
      throw;
   }
}

double P3_Reduce::P3TraceCost(const P3& p) {
   return
      (Scalar::Dot(p[0], p[0])) +
      (Scalar::Dot(p[1], p[1])) +
      (Scalar::Dot(p[2], p[2]));
}

double P3_Reduce::P3ScalarCost(const P3& p) {
   return
      (Scalar::Dot(p[0], p[1])) +
      (Scalar::Dot(p[0], p[2])) +
      (Scalar::Dot(p[1], p[2]));
}

double P3_Reduce::P3ScalarAbsCost(const P3& p) {
   return std::abs(Scalar::Dot(p[0], p[1])) +
      std::abs(Scalar::Dot(p[0], p[2])) +
      std::abs(Scalar::Dot(p[1], p[2]));
}


double P3_Reduce::ComputeMetricCost(const LRL_Cell& cell) {
   const B4 b4(cell);
   const double ab = std::abs(b4[0].Dot(b4[1]));
   const double ac = std::abs(b4[0].Dot(b4[2]));
   const double bc = std::abs(b4[1].Dot(b4[2]));
   return ab + ac + bc;
}



// Add this to P3_Reduce.cpp - static permutation matrices:
static const std::vector<Matrix_3x3> permutationMatrices = {
   // {0,1,2} - identity permutation
   Matrix_3x3{1,0,0, 0,1,0, 0,0,1},

   // {0,2,1} - swap rows 1 and 2
   Matrix_3x3{1,0,0, 0,0,1, 0,1,0},

   // {1,0,2} - swap rows 0 and 1  
   Matrix_3x3{0,1,0, 1,0,0, 0,0,1},

   // {1,2,0} - cyclic: row 0→1, row 1→2, row 2→0
   Matrix_3x3{0,0,1, 1,0,0, 0,1,0},

   // {2,0,1} - cyclic: row 0→2, row 1→0, row 2→1
   Matrix_3x3{0,1,0, 0,0,1, 1,0,0},

   // {2,1,0} - reverse: row 0→2, row 1→1, row 2→0
   Matrix_3x3{0,0,1, 0,1,0, 1,0,0}
};

std::pair<LRL_Cell, Matrix_3x3> P3_Reduce::ReduceCellWith3480(const LRL_Cell& input) {
   LRL_Cell current = input;
   double currentCost = P3Cost(P3(current));
   double originalCost = currentCost;
   const double epsilon = 1e-6;

   static const std::vector<Matrix_3x3> allMatrices = TransformationMatrices::generateUnimodularMatrices();
   static const std::vector<Matrix_3x3> curatedMatrices = theBasicMatrices;

   if (debug) {
      std::cout << "DEBUG: Starting reduction with cost: " << currentCost << std::endl;
      std::cout << "DEBUG: Curated matrices count: " << curatedMatrices.size() << std::endl;
      std::cout << "DEBUG: All matrices count: " << allMatrices.size() << std::endl;
   }

   // Global loop: Continue until both stages find no improvements
   bool globalImproved = true;
   int globalIteration = 0;

   while (globalImproved) {
      globalImproved = false;

      // Stage 1: Use curated matrices for preprocessing with multiple passes
      bool stage1Improved = true;
      int stage1Iterations = 0;

      while (stage1Improved) {
         stage1Improved = false;
         LRL_Cell bestCell = current;
         double bestCost = currentCost;
         size_t bestMatrixIdx = 0;
         size_t bestPermIdx = 0;
         size_t totalCandidatesEvaluated = 0;
         size_t improvingCandidates = 0;

         for (size_t i = 0; i < curatedMatrices.size(); ++i) {
            const LRL_Cell transformed = ApplyMatrixToCell(current, curatedMatrices[i]);
            const P3 projected(transformed);
            if (!LRL_Cell(projected).IsValid()) {
               const int i19191 = 19191;
            }

            // Generate all 6 permutations and track which one is selected
            const std::vector<P3> permutations = GeneratePermutedP3s(projected);

            for (size_t permIdx = 0; permIdx < permutations.size(); ++permIdx) {
               const P3& permuted = permutations[permIdx];
               const LRL_Cell candidate(permuted);

               if (!candidate.IsValid()) {
                  const int i19191 = 19191;
               }
               const double candidateCost = P3Cost(P3(candidate));
               totalCandidatesEvaluated++;

               if (candidateCost + epsilon < bestCost) {
                  improvingCandidates++;
                  bestCost = candidateCost;
                  bestCell = candidate;
                  bestMatrixIdx = i;
                  bestPermIdx = permIdx;
                  stage1Improved = true;
                  globalImproved = true;
               }
            }
         }

         if (debug) {
            std::cout << "DEBUG Stage 1 iteration " << stage1Iterations
               << " (global " << globalIteration << "): Evaluated "
               << totalCandidatesEvaluated << " candidates, found "
               << improvingCandidates << " improvements";
         }

         if (stage1Improved) {
            current = bestCell;
            currentCost = bestCost;
            if (debug) {
               std::cout << " -> stage 1 new cost: " << currentCost;
            }
         }
         if (debug) {
            std::cout << std::endl;
         }
         stage1Iterations++;
      }

      // Stage 2: Polish with full 3480 set with multiple passes
      bool stage2Improved = true;
      int stage2Iterations = 0;

      while (stage2Improved) {
         stage2Improved = false;
         LRL_Cell bestCell = current;
         double bestCost = currentCost;
         size_t bestMatrixIdx = 0;
         size_t bestPermIdx = 0;
         size_t totalCandidatesEvaluated = 0;
         size_t improvingCandidates = 0;

         for (size_t i = 0; i < allMatrices.size(); ++i) {
            const LRL_Cell transformed = ApplyMatrixToCell(current, allMatrices[i]);
            const P3 projected(transformed);
            if (!transformed.IsValid()) {
               const int i19191 = 19191;
            }

            if (debug && globalIteration == 0 && stage2Iterations == 0 && i < 5) {
               std::cout << "Matrix " << i << " transforms input to: " << P3(transformed) << std::endl;
            }

            // Generate all 6 permutations and track which one is selected
            const std::vector<P3> permutations = GeneratePermutedP3s(projected);

            for (size_t permIdx = 0; permIdx < permutations.size(); ++permIdx) {
               const P3& permuted = permutations[permIdx];
               const LRL_Cell candidate(permuted);
               if (!candidate.IsValid()) {
                  const int i19191 = 19191;
               }

               const double candidateCost = P3Cost(P3(candidate));
               totalCandidatesEvaluated++;

               if (candidateCost + epsilon < bestCost) {
                  improvingCandidates++;
                  bestCost = candidateCost;
                  bestCell = candidate;
                  bestMatrixIdx = i;
                  bestPermIdx = permIdx;
                  stage2Improved = true;
                  globalImproved = true;
               }
            }
         }

         if (debug) {
            std::cout << "DEBUG Stage 2 iteration " << stage2Iterations
               << " (global " << globalIteration << "): Evaluated "
               << totalCandidatesEvaluated << " candidates, found "
               << improvingCandidates << " improvements";
         }

         if (stage2Improved) {
            current = bestCell;
            currentCost = bestCost;
            if (debug) {
               std::cout << " -> stage 2 new cost: " << currentCost;
            }
         }
         if (debug) {
            std::cout << std::endl;
         }
         stage2Iterations++;
      }

      globalIteration++;
   }

   if (debug) {
      std::cout << "DEBUG: Global convergence achieved after " << globalIteration
         << " global iterations. Final cost before StandardPresentation: " << currentCost << std::endl;
      std::cout << "DEBUG: Total improvement: " << originalCost << " -> " << currentCost
         << " (" << (originalCost - currentCost) << ")" << std::endl;
   }

   const P3 final = StandardPresentation(P3(current));
   const double finalCost = P3Cost(final);

   if (debug) {
      std::cout << "DEBUG: Final cost after StandardPresentation: " << finalCost << std::endl;
   }

   return { final, Matrix_3x3() };
}

bool P3_Reduce::VerifyTransformation(const LRL_Cell& input, const LRL_Cell& output, const Matrix_3x3& transform, bool verbose) {
   // Apply the transformation matrix to the input
   LRL_Cell transformedInput = ApplyMatrixToCell(input, transform);

   // Calculate costs
   double outputCost = P3Cost(P3(output));
   double transformedCost = P3Cost(P3(transformedInput));

   // Check tolerances
   const double tolerance = 1e-10;

   // Cost comparison
   bool costsMatch = std::abs(outputCost - transformedCost) < tolerance;

   // Cell parameter comparison
   const auto inputParams = transformedInput;
   const auto outputParams = output;
   bool cellsMatch = true;
   for (size_t i = 0; i < 6; ++i) {
      if (std::abs(inputParams[i] - outputParams[i]) > tolerance) {
         cellsMatch = false;
         break;
      }
   }

   // Matrix properties
   double det = transform.Det();
   bool isUnimodular = (std::abs(det - 1.0) < tolerance) || (std::abs(det + 1.0) < tolerance);

   // Overall verification result
   bool verificationPassed = costsMatch && cellsMatch && isUnimodular;

   if (verbose) {
      std::cout << "\n=== TRANSFORMATION VERIFICATION ===" << std::endl;
      std::cout << "Input:              " << input << std::endl;
      std::cout << "Expected output:    " << output << std::endl;
      std::cout << "Transform*Input:    " << transformedInput << std::endl;
      std::cout << "Expected cost:      " << outputCost << std::endl;
      std::cout << "Transform cost:     " << transformedCost << std::endl;
      std::cout << "Cost difference:    " << std::abs(outputCost - transformedCost) << std::endl;

      std::cout << "\nTransformation Matrix:" << std::endl;
      std::cout << "  [" << std::setw(8) << transform[0] << "  " << std::setw(8) << transform[1] << "  " << std::setw(8) << transform[2] << "]" << std::endl;
      std::cout << "  [" << std::setw(8) << transform[3] << "  " << std::setw(8) << transform[4] << "  " << std::setw(8) << transform[5] << "]" << std::endl;
      std::cout << "  [" << std::setw(8) << transform[6] << "  " << std::setw(8) << transform[7] << "  " << std::setw(8) << transform[8] << "]" << std::endl;
      std::cout << "Determinant: " << det << std::endl;

      std::cout << "\nVerification Results:" << std::endl;
      std::cout << "  Costs match:        " << (costsMatch ? "✓ PASS" : "✗ FAIL") << std::endl;
      std::cout << "  Cells match:        " << (cellsMatch ? "✓ PASS" : "✗ FAIL") << std::endl;
      std::cout << "  Is unimodular:      " << (isUnimodular ? "✓ PASS" : "✗ FAIL") << std::endl;
      std::cout << "  Overall:            " << (verificationPassed ? "✓ VERIFICATION PASSED" : "✗ VERIFICATION FAILED") << std::endl;
      std::cout << "==================================\n" << std::endl;
   }

   return verificationPassed;
}

// Helper function to check vector equality with tolerance
bool P3_Reduce::VectorsEqual(const P3& a, const P3& b, double tolerance) {
   for (size_t i = 0; i < 3; ++i) {
      if (std::abs(a[i].first - b[i].first) > tolerance ||
         std::abs(a[i].second - b[i].second) > tolerance) {
         return false;
      }
   }
   return true;
}

// Step 1: Magnitude ordering |v0| ≤ |v1| ≤ |v2|
P3 P3_Reduce::Step1_MagnitudeOrdering(const P3& input) {
   return StandardPresentation(input);
}

// Step 2: Upper half-plane constraint (y ≥ 0)
P3 P3_Reduce::Step2_UpperHalfPlane(const P3& input) {
   return EnforceUpperHalfPlane(input);
}

// Step 3: Intelligent scalar product reduction
P3 P3_Reduce::Step3_ReduceScalarProducts(const P3& input) {
   P3 current = input;
   P3 best = current;
   double bestCost = P3Cost(current);
   const double scalarProductThreshold = 0.3; // Normalized threshold

   // Check all pairs for excessive scalar products
   for (size_t i = 0; i < 3; ++i) {
      for (size_t j = i + 1; j < 3; ++j) {
         double dot = std::abs(Scalar::Dot(current[i], current[j]));
         double mag_i = std::sqrt(Scalar::MagnitudeSquared(current[i]));
         double mag_j = std::sqrt(Scalar::MagnitudeSquared(current[j]));

         if (mag_i > 1e-10 && mag_j > 1e-10) {
            double normalizedDot = dot / (mag_i * mag_j);

            if (normalizedDot > scalarProductThreshold) {
               // Try multiple reduction strategies and pick the best
               P3 candidate = ChooseBestScalarReduction(current, i, j);
               double candidateCost = P3Cost(candidate);

               if (candidateCost < bestCost) {
                  best = candidate;
                  bestCost = candidateCost;
                  std::cout << "    Reduced scalar product between v" << i
                     << " and v" << j << " (normalized: " << normalizedDot
                     << " → " << std::abs(Scalar::Dot(best[i], best[j])) / (mag_i * mag_j) << ")" << std::endl;
               }
            }
         }
      }
   }

   return best;
}

// Choose the best scalar product reduction strategy
P3 P3_Reduce::ChooseBestScalarReduction(const P3& input, size_t i, size_t j) {
   P3 best = input;
   double bestCost = P3Cost(input);

   // Strategy 1: Project j onto i (reduce j using i)
   P3 option1 = input;
   double proj1 = Scalar::Projection(option1[j], option1[i]);
   option1[j].first -= proj1 * option1[i].first;
   option1[j].second -= proj1 * option1[i].second;
   option1 = StandardPresentation(option1);

   double cost1 = P3Cost(option1);
   if (cost1 < bestCost) {
      best = option1;
      bestCost = cost1;
   }

   // Strategy 2: Project i onto j (reduce i using j)
   P3 option2 = input;
   double proj2 = Scalar::Projection(option2[i], option2[j]);
   option2[i].first -= proj2 * option2[j].first;
   option2[i].second -= proj2 * option2[j].second;
   option2 = StandardPresentation(option2);

   double cost2 = P3Cost(option2);
   if (cost2 < bestCost) {
      best = option2;
      bestCost = cost2;
   }

   // Strategy 3: Integer linear combinations
   for (int sign = -1; sign <= 1; sign += 2) {
      P3 option3 = input;
      option3[j].first += sign * option3[i].first;
      option3[j].second += sign * option3[i].second;
      option3 = StandardPresentation(option3);

      double cost3 = P3Cost(option3);
      if (cost3 < bestCost) {
         best = option3;
         bestCost = cost3;
      }
   }

   return best;
}

// Step 4: Optimize angles for better orthogonality
P3 P3_Reduce::Step4_OptimizeAngles(const P3& input) {
   P3 current = input;
   P3 best = current;
   double bestCost = P3Cost(current);

   // Try reflecting vectors to improve overall orthogonality
   for (size_t i = 0; i < 3; ++i) {
      double angle = std::atan2(current[i].second, current[i].first);

      // Consider reflection if angle is in second quadrant
      if (angle > M_PI_2 && angle <= M_PI) {
         P3 candidate = current;
         candidate[i].first = -candidate[i].first;
         candidate[i].second = -candidate[i].second;
         candidate = StandardPresentation(candidate);

         double candidateCost = P3Cost(candidate);
         if (candidateCost < bestCost) {
            best = candidate;
            bestCost = candidateCost;
            std::cout << "    Reflected vector " << i << " (angle "
               << angle * 180 / M_PI << "° → "
               << std::atan2(best[i].second, best[i].first) * 180 / M_PI << "°)" << std::endl;
         }
      }
   }

   return best;
}

// Step 5: Try lattice-preserving vector combinations
P3 P3_Reduce::Step5_LatticePreservingCombinations(const P3& input) {
   P3 current = input;
   P3 best = current;
   double bestCost = P3Cost(current);

   // Try combinations like v2 = v2 ± v0 ± v1
   for (int sign0 = -1; sign0 <= 1; ++sign0) {
      for (int sign1 = -1; sign1 <= 1; ++sign1) {
         if (sign0 == 0 && sign1 == 0) continue; // Skip identity

         P3 candidate = current;
         candidate[2].first += sign0 * candidate[0].first + sign1 * candidate[1].first;
         candidate[2].second += sign0 * candidate[0].second + sign1 * candidate[1].second;
         candidate = StandardPresentation(candidate);

         double candidateCost = P3Cost(candidate);
         if (candidateCost < bestCost) {
            best = candidate;
            bestCost = candidateCost;
            std::cout << "    Applied combination v2 = v2 + " << sign0
               << "*v0 + " << sign1 << "*v1" << std::endl;
         }
      }
   }

   // Also try modifying v1 using v0
   for (int sign = -1; sign <= 1; sign += 2) {
      P3 candidate = current;
      candidate[1].first += sign * candidate[0].first;
      candidate[1].second += sign * candidate[0].second;
      candidate = StandardPresentation(candidate);

      double candidateCost = P3Cost(candidate);
      if (candidateCost < bestCost) {
         best = candidate;
         bestCost = candidateCost;
         std::cout << "    Applied combination v1 = v1 " << (sign > 0 ? "+" : "-")
            << " v0" << std::endl;
      }
   }

   return best;
}