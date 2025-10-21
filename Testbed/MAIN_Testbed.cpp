#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <limits>
#include "G6.h"
#include "LRL_Cell.h"
#include "ProgramSetup.h"
#include "TestbedControls.h"

struct NiggliScore {
   std::string name;
   std::string description;
   double score;  // Positive means violation, negative means satisfied, magnitude shows degree
   int condition_number;

   NiggliScore(const std::string& n, const std::string& desc, double s, int num)
      : name(n), description(desc), score(s), condition_number(num) {
   }

   bool is_violated() const { return score > 0.0; }

   // For sorting - largest violations first
   bool operator<(const NiggliScore& other) const {
      return score > other.score;  // Reverse for largest first
   }
};

// Overload << operator for easy printing
std::ostream& operator<<(std::ostream& os, const NiggliScore& ns) {
   os << std::setw(3) << ns.condition_number << ": "
      << std::setw(30) << std::left << ns.name
      << " | score: " << std::setw(12) << std::right << std::fixed << std::setprecision(6) << ns.score
      << " | " << (ns.is_violated() ? "VIOLATED" : "satisfied")
      << " | " << ns.description;
   return os;
}

// Check Niggli conditions and return scores
std::vector<NiggliScore> CheckNiggliConditions(const G6& v, const double delta = 1e-6) {
   std::vector<NiggliScore> scores;

   const double& g1 = v[0];
   const double& g2 = v[1];
   const double& g3 = v[2];
   const double& g4 = v[3];
   const double& g5 = v[4];
   const double& g6 = v[5];

   // Condition 0: g1 > 0
   scores.emplace_back("g1 > 0", "g1 must be positive",
      -g1, 0);

   // Condition 1: g2 > 0
   scores.emplace_back("g2 > 0", "g2 must be positive",
      -g2, 1);

   // Condition 2: g3 > 0
   scores.emplace_back("g3 > 0", "g3 must be positive",
      -g3, 2);

   // Condition 3: g1 <= g2
   scores.emplace_back("g1 <= g2", "First cell parameter <= second",
      g1 - g2, 3);

   // Condition 4: g2 <= g3
   scores.emplace_back("g2 <= g3", "Second cell parameter <= third",
      g2 - g3, 4);

   // Condition 5: |g4| <= g2
   scores.emplace_back("|g4| <= g2", "Absolute g4 bounded by g2",
      std::abs(g4) - g2, 5);

   // Condition 6: |g5| <= g1
   scores.emplace_back("|g5| <= g1", "Absolute g5 bounded by g1",
      std::abs(g5) - g1, 6);

   // Condition 7: |g6| <= g1
   scores.emplace_back("|g6| <= g1", "Absolute g6 bounded by g1",
      std::abs(g6) - g1, 7);

   // Condition 8: g4, g5, g6 all <= 0 or all > 0
   int nneg = 0;
   int npos = 0;
   for (size_t i = 3; i < 6; ++i) {
      if (v[i] <= 0.0) ++nneg;
      if (v[i] > 0.0) ++npos;
   }
   // If mixed signs, score is the sum of magnitudes of wrong-sign terms
   double sign_score = 0.0;
   if (nneg != 0 && nneg != 3) {
      // Determine which sign is majority
      bool should_be_negative = (nneg >= npos);
      for (size_t i = 3; i < 6; ++i) {
         if (should_be_negative && v[i] > 0.0) {
            sign_score += std::abs(v[i]);
         } else if (!should_be_negative && v[i] <= 0.0) {
            sign_score += std::abs(v[i]);
         }
      }
   } else {
      // All same sign - satisfied. Score is negative sum of magnitudes
      for (size_t i = 3; i < 6; ++i) {
         sign_score -= std::abs(v[i]);
      }
   }
   scores.emplace_back("g4,g5,g6 signs", "All g4,g5,g6 same sign or zero",
      sign_score, 8);

   // Condition 9: if g4 == g2 then g6 <= 2*g5
   double cond9_score;
   if (std::abs(g4 - g2) < delta) {
      cond9_score = g6 - 2.0 * g5;
   } else {
      // Not applicable - show how far from g4==g2
      cond9_score = -(std::abs(g4 - g2));
   }
   scores.emplace_back("g4==g2 => g6<=2*g5", "Special case g4==g2",
      cond9_score, 9);

   // Condition 10: if g5 == g1 then g6 <= 2*g4
   double cond10_score;
   if (std::abs(g5 - g1) < delta) {
      cond10_score = g6 - 2.0 * g4;
   } else {
      // Not applicable - show how far from g5==g1
      cond10_score = -(std::abs(g5 - g1));
   }
   scores.emplace_back("g5==g1 => g6<=2*g4", "Special case g5==g1",
      cond10_score, 10);

   // Condition 11: if g6 == g1 then g5 <= 2*g4
   double cond11_score;
   if (std::abs(g6 - g1) < delta) {
      cond11_score = g5 - 2.0 * g4;
   } else {
      // Not applicable - show how far from g6==g1
      cond11_score = -(std::abs(g6 - g1));
   }
   scores.emplace_back("g6==g1 => g5<=2*g4", "Special case g6==g1",
      cond11_score, 11);

   // Condition 12: if g2 ~= g3 then |g5| <= |g6|
   double cond12_score;
   if (std::abs(g2 - g3) < delta) {
      cond12_score = std::abs(g5) - std::abs(g6);
   } else {
      // Not applicable - show how far from g2==g3
      cond12_score = -(std::abs(g2 - g3));
   }
   scores.emplace_back("g2~=g3 => |g5|<=|g6|", "Special case g2~=g3",
      cond12_score, 12);

   // Condition 13: |g3| <= g1 + g2 + g3 + g4 + g5 + g6
   double sum = g1 + g2 + g3 + g4 + g5 + g6;
   scores.emplace_back("|g3| <= sum", "g3 bounded by sum of all parameters",
      std::abs(g3) - sum, 13);

   // Condition 14: if g4 == g2 then g6 == 0
   double cond14_score;
   if (std::abs(g4 - g2) < delta) {
      cond14_score = std::abs(g6);
   } else {
      // Not applicable - show how far from g4==g2
      cond14_score = -(std::abs(g4 - g2));
   }
   scores.emplace_back("g4==g2 => g6==0", "Special case g4==g2 implies g6=0",
      cond14_score, 14);

   // Condition 15: if g5 == g1 then g6 == 0
   double cond15_score;
   if (std::abs(g5 - g1) < delta) {
      cond15_score = std::abs(g6);
   } else {
      // Not applicable - show how far from g5==g1
      cond15_score = -(std::abs(g5 - g1));
   }
   scores.emplace_back("g5==g1 => g6==0", "Special case g5==g1 implies g6=0",
      cond15_score, 15);

   // Condition 16: if g6 == g1 then g5 == 0
   double cond16_score;
   if (std::abs(g6 - g1) < delta) {
      cond16_score = std::abs(g5);
   } else {
      // Not applicable - show how far from g6==g1
      cond16_score = -(std::abs(g6 - g1));
   }
   scores.emplace_back("g6==g1 => g5==0", "Special case g6==g1 implies g5=0",
      cond16_score, 16);

   // Condition 17: if g3 == sum then 2*(g1 + g5) <= 0
   double cond17_score;
   if (std::abs(g3 - sum) < delta) {
      cond17_score = 2.0 * (g1 + g5);
   } else {
      // Not applicable - show how far from g3==sum
      cond17_score = -(std::abs(g3 - sum));
   }
   scores.emplace_back("g3==sum => 2*(g1+g5)<=0", "Special case g3==sum",
      cond17_score, 17);

   return scores;
}

// Apply a single Niggli reduction step based on the Krivy-Gruber algorithm
// Returns true if a reduction was applied, false if condition not applicable
bool ApplyNiggliReductionStep(G6& g6, int step_number, const double delta = 1e-6) {
   double& A = g6[0];
   double& B = g6[1];
   double& C = g6[2];
   double& xi = g6[3];
   double& eta = g6[4];
   double& zeta = g6[5];

   switch (step_number) {
   case 1: // If A > B or (A = B and |xi| > |eta|), swap (A,zeta) <-> (B,eta)
      if (A > B + delta || (std::abs(A - B) <= delta && std::abs(xi) > std::abs(eta) + delta)) {
         std::swap(A, B);
         std::swap(zeta, eta);
         return true;
      }
      break;

   case 2: // If B > C or (B = C and |eta| > |zeta|), swap (B,eta) <-> (C,zeta)
      if (B > C + delta || (std::abs(B - C) <= delta && std::abs(eta) > std::abs(zeta) + delta)) {
         std::swap(B, C);
         std::swap(eta, zeta);
         return true;
      }
      break;

   case 3: // If xi*eta*zeta > 0, put (|xi|,|eta|,|zeta|) -> (xi,eta,zeta)
      if (xi * eta * zeta > delta) {
         xi = std::abs(xi);
         eta = std::abs(eta);
         zeta = std::abs(zeta);
         return true;
      }
      break;

   case 4: // If xi*eta*zeta <= 0, put (-|xi|,-|eta|,-|zeta|) -> (xi,eta,zeta)
      if (xi * eta * zeta <= delta) {
         xi = -std::abs(xi);
         eta = -std::abs(eta);
         zeta = -std::abs(zeta);
         return true;
      }
      break;

   case 5: // If |xi| > B or (xi = B and 2*eta < zeta) or (xi = -B and zeta < 0)
      if (std::abs(xi) > B + delta ||
         (std::abs(xi - B) <= delta && 2 * eta < zeta - delta) ||
         (std::abs(xi + B) <= delta && zeta < -delta)) {

         // ALGEBRAIC PRECONDITION: Check that B + C - |xi| > 0
         // This ensures C remains positive after transformation
         if (B + C <= std::abs(xi) + delta) {
            // Transformation would make C <= 0, skip it
            return false;
         }

         double sign_xi = (xi > 0) ? 1.0 : -1.0;
         C = B + C - xi * sign_xi;
         eta = eta - zeta * sign_xi;
         xi = -2 * B * sign_xi;
         return true;
      }
      break;

   case 6: // If |eta| > A or (eta = A and 2*xi < zeta) or (eta = -A and zeta < 0)
      if (std::abs(eta) > A + delta ||
         (std::abs(eta - A) <= delta && 2 * xi < zeta - delta) ||
         (std::abs(eta + A) <= delta && zeta < -delta)) {

         // ALGEBRAIC PRECONDITION: Check that A + C - |eta| > 0
         // This ensures C remains positive after transformation
         if (A + C <= std::abs(eta) + delta) {
            // Transformation would make C <= 0, skip it
            return false;
         }

         double sign_eta = (eta > 0) ? 1.0 : -1.0;
         C = A + C - eta * sign_eta;
         xi = xi - zeta * sign_eta;
         eta = -2 * A * sign_eta;
         return true;
      }
      break;

   case 7: // If |zeta| > A or (zeta = A and 2*xi < eta) or (zeta = -A and eta < 0)
      if (std::abs(zeta) > A + delta ||
         (std::abs(zeta - A) <= delta && 2 * xi < eta - delta) ||
         (std::abs(zeta + A) <= delta && eta < -delta)) {

         // ALGEBRAIC PRECONDITION: Check that A + B - |zeta| > 0
         // This ensures B remains positive after transformation
         if (A + B <= std::abs(zeta) + delta) {
            // Transformation would make B <= 0, skip it
            return false;
         }

         double sign_zeta = (zeta > 0) ? 1.0 : -1.0;
         B = A + B - zeta * sign_zeta;
         xi = xi - eta * sign_zeta;
         zeta = -2 * A * sign_zeta;
         return true;
      }
      break;

   case 8: // If xi + eta + zeta + A + B < 0 or [xi + eta + zeta + A + B = 0 and 2(A + eta) + zeta > 0]
      if (xi + eta + zeta + A + B < -delta ||
         (std::abs(xi + eta + zeta + A + B) <= delta && 2 * (A + eta) + zeta > delta)) {
         C = A + B + C + xi + eta + zeta;
         zeta = 2 * B + eta + zeta;
         eta = 2 * A + xi + eta;
         return true;
      }
      break;

   default:
      return false;
   }

   return false;
}

// Check if a Niggli reduction step would produce a valid cell
// Returns true if the step would create a valid cell, false otherwise
bool WouldProduceValidCell(const G6& g6, int step_number, const double delta = 1e-6) {
   // Make a copy and apply the transformation
   G6 test_g6 = g6;

   if (!ApplyNiggliReductionStep(test_g6, step_number, delta)) {
      return false; // Step didn't apply
   }

   // Check if result is valid
   LRL_Cell test_cell(test_g6);
   return test_cell.IsValid();
}

// Map Niggli condition number (0-17) to Krivy-Gruber reduction step (1-8)
int MapConditionToReductionStep(int condition_number, const G6& g6, const double delta = 1e-6) {
   const double& xi = g6[3];
   const double& eta = g6[4];
   const double& zeta = g6[5];

   switch (condition_number) {
   case 0: case 1: case 2:
      return -1;
   case 3: return 1;
   case 4: return 2;
   case 5: return 5;
   case 6: return 6;
   case 7: return 7;
   case 8:
      return (xi * eta * zeta > delta) ? 3 : 4;
   case 13: return 8;
   case 9: case 14: return 5;
   case 10: case 15: return 6;
   case 11: case 16: return 7;
   case 12: return 2;
   case 17: return 8;
   default: return -1;
   }
}

// Apply standard presentation (normalization) steps 1-4 from Krivy-Gruber
void ApplyStandardPresentation(G6& g6, const double delta = 1e-6) {
   bool changed = true;
   int max_iterations = 10;
   int iteration = 0;

   while (changed && iteration < max_iterations) {
      changed = false;
      iteration++;

      if (ApplyNiggliReductionStep(g6, 1, delta)) {
         changed = true;
      }

      if (ApplyNiggliReductionStep(g6, 2, delta)) {
         changed = true;
         continue;
      }

      if (ApplyNiggliReductionStep(g6, 3, delta)) {
         changed = true;
      } else if (ApplyNiggliReductionStep(g6, 4, delta)) {
         changed = true;
      }
   }
}

// Apply reduction for a specific Niggli condition violation
bool ApplyReductionForCondition(G6& g6, int condition_number, const double delta = 1e-6) {
   int step = MapConditionToReductionStep(condition_number, g6, delta);
   if (step < 0) {
      return false;
   }
   return ApplyNiggliReductionStep(g6, step, delta);
}

// Get the largest violation
const NiggliScore* GetLargestViolation(const std::vector<NiggliScore>& scores) {
   const NiggliScore* largest = nullptr;
   double max_score = 0.0;

   for (const auto& score : scores) {
      if (score.is_violated() && score.score > max_score) {
         max_score = score.score;
         largest = &score;
      }
   }

   return largest;
}

// Get the smallest violation
const NiggliScore* GetSmallestViolation(const std::vector<NiggliScore>& scores) {
   const NiggliScore* smallest = nullptr;
   double min_score = std::numeric_limits<double>::max();

   for (const auto& score : scores) {
      if (score.is_violated() && score.score < min_score) {
         min_score = score.score;
         smallest = &score;
      }
   }

   return smallest;
}

// Get list of all violations
std::vector<NiggliScore> GetAllViolations(const std::vector<NiggliScore>& scores) {
   std::vector<NiggliScore> violations;

   for (const auto& score : scores) {
      if (score.is_violated()) {
         violations.push_back(score);
      }
   }

   std::sort(violations.begin(), violations.end());

   return violations;
}

// Helper function to print all scores
void PrintNiggliScores(const std::vector<NiggliScore>& scores, bool sort_by_violation = true) {
   std::vector<NiggliScore> sorted_scores = scores;

   if (sort_by_violation) {
      std::sort(sorted_scores.begin(), sorted_scores.end());
   }

   std::cout << "\nNiggli Condition Scores:\n";
   std::cout << std::string(100, '=') << "\n";
   for (const auto& score : sorted_scores) {
      std::cout << score << "\n";
   }
   std::cout << std::string(100, '=') << "\n";
}

int main() {
   std::cout << "; TESTBED" << std::endl;
   try {
      TestbedControls controls;
      const BasicProgramInput<TestbedControls> dc_setup("CmdToB4", controls);
      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }
      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }
      for (const auto& input : dc_setup.getInputList()) {
         const LRL_Cell cell(input.getCell());
         G6 g6(cell);
         std::cout << "\nInput: " << input.getInput() << std::endl;
         std::cout << "Cell: " << cell << std::endl;
         std::cout << "Original G6: " << g6 << std::endl;

         ApplyStandardPresentation(g6);
         std::cout << "After standard presentation: " << g6 << std::endl;

         auto scores = CheckNiggliConditions(g6);
         PrintNiggliScores(scores, true);

         int violation_count = 0;
         for (const auto& score : scores) {
            if (score.is_violated()) ++violation_count;
         }
         std::cout << "\nTotal violations: " << violation_count << " out of " << scores.size() << "\n";

         const NiggliScore* largest = GetLargestViolation(scores);
         if (largest) {
            std::cout << "\nLargest violation:\n" << *largest << "\n";
         }

         const NiggliScore* smallest = GetSmallestViolation(scores);
         if (smallest) {
            std::cout << "\nSmallest violation:\n" << *smallest << "\n";
         }

         std::vector<NiggliScore> violations = GetAllViolations(scores);
         if (!violations.empty()) {
            std::cout << "\nAll violations (sorted by severity):\n";
            for (const auto& v : violations) {
               std::cout << "  " << v << "\n";
            }
         }

         // TEST 1: Iteratively reduce LARGEST violation
         {
            G6 test_g6 = g6;
            std::cout << "\n=== TEST 1: Iteratively reducing LARGEST violations ===\n";
            int iteration = 0;
            const int max_iterations = 100;

            while (iteration < max_iterations) {
               ApplyStandardPresentation(test_g6);
               auto test_scores = CheckNiggliConditions(test_g6);
               const NiggliScore* largest_viol = GetLargestViolation(test_scores);

               if (!largest_viol) {
                  std::cout << "Converged after " << iteration << " iterations\n";
                  std::cout << "Final G6: " << test_g6 << std::endl;
                  std::cout << "Cell is Niggli reduced!\n";
                  break;
               }

               iteration++;
               if (!ApplyReductionForCondition(test_g6, largest_viol->condition_number)) {
                  std::cout << "Could not reduce condition " << largest_viol->condition_number
                     << " at iteration " << iteration << "\n";
                  break;
               }
            }

            if (iteration >= max_iterations) {
               std::cout << "Did not converge after " << max_iterations << " iterations\n";
               ApplyStandardPresentation(test_g6);
               auto final_scores = CheckNiggliConditions(test_g6);
               int final_count = 0;
               for (const auto& s : final_scores) if (s.is_violated()) ++final_count;
               std::cout << "Final G6: " << test_g6 << std::endl;
               std::cout << "Final violation count: " << final_count << "\n";
               auto final_viols = GetAllViolations(final_scores);
               if (!final_viols.empty()) {
                  std::cout << "Remaining violations:\n";
                  for (const auto& v : final_viols) std::cout << "  " << v << "\n";
               }
            }
         }

         // TEST 2: Iteratively reduce SMALLEST violation  
         {
            G6 test_g6 = g6;
            std::cout << "\n=== TEST 2: Iteratively reducing SMALLEST violations ===\n";
            int outer_iteration = 0;
            const int max_iterations = 100;

            while (outer_iteration < max_iterations) {
               ApplyStandardPresentation(test_g6);
               auto test_scores = CheckNiggliConditions(test_g6);

               // Get ALL violations sorted by severity (largest first from GetAllViolations)
               std::vector<NiggliScore> violations = GetAllViolations(test_scores);

               if (violations.empty()) {
                  std::cout << "Converged after " << outer_iteration << " outer iterations\n";
                  std::cout << "Final G6: " << test_g6 << std::endl;
                  std::cout << "Cell is Niggli reduced!\n";
                  break;
               }

               // Reverse to get smallest first
               std::reverse(violations.begin(), violations.end());

               std::cout << "\n=== Outer iteration " << (outer_iteration + 1) << " - " << violations.size() << " violations to try ===\n";

               // Try violations in order from smallest to largest
               bool applied_reduction = false;
               for (size_t viol_idx = 0; viol_idx < violations.size(); ++viol_idx) {
                  const NiggliScore& viol = violations[viol_idx];

                  std::cout << "\nTrying violation " << (viol_idx + 1) << " of " << violations.size()
                     << ": condition " << viol.condition_number << " (" << viol.name << ")\n";
                  std::cout << "Score: " << viol.score << "\n";
                  std::cout << "G6: " << test_g6 << std::endl;

                  int kg_step = MapConditionToReductionStep(viol.condition_number, test_g6);
                  std::cout << "K-G step " << kg_step << "\n";

                  if (kg_step > 0) {
                     bool would_be_valid = WouldProduceValidCell(test_g6, kg_step);
                     std::cout << "Validity: " << (would_be_valid ? "OK" : "WOULD BE INVALID") << "\n";

                     if (!would_be_valid) {
                        std::cout << "Skipping - would create invalid cell\n";
                        continue;
                     }
                  }

                  if (!ApplyReductionForCondition(test_g6, viol.condition_number)) {
                     std::cout << "Could not apply - skipping\n";
                     continue;
                  }

                  std::cout << "Applied! New G6: " << test_g6 << std::endl;
                  applied_reduction = true;
                  break;
               }

               if (!applied_reduction) {
                  std::cout << "\n*** NO VALID REDUCTIONS POSSIBLE ***\n";
                  std::cout << "All " << violations.size() << " violations would create invalid cells.\n";
                  break;
               }

               outer_iteration++;
            }
         }
         std::cout << std::string(100, '-') << "\n\n";
      }
      return 0;
   }
   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}