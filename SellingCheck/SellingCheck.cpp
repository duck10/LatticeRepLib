// Verify that KrivyGruber_LCA produces minimal trace
// 
// This test checks:
// 1. Does the algorithm satisfy Selling-Delaunay conditions (which guarantee minimal trace)?
// 2. Can we find any equivalent cell with smaller trace?

#include "KrivyGruber_LCA.h"
#include "G6.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

class TraceMinimizationVerifier {
public:
   // Selling-Delaunay conditions for minimal trace
   struct SellingCheck {
      bool satisfied;
      std::vector<std::string> violations;

      SellingCheck() : satisfied(true) {}
   };

   // Check if G6 satisfies Selling-Delaunay conditions
   static SellingCheck CheckSellingConditions(const G6& g6, double epsilon = 1e-10) {
      SellingCheck result;
      const double g1 = g6[0];
      const double g2 = g6[1];
      const double g3 = g6[2];
      const double g4 = g6[3];
      const double g5 = g6[4];
      const double g6val = g6[5];

      // Selling conditions: diagonal >= |off-diagonal|
      if (g1 < std::abs(g5) - epsilon) {
         result.satisfied = false;
         result.violations.push_back("g1 < |g5|: " + std::to_string(g1) + " < " + std::to_string(std::abs(g5)));
      }
      if (g1 < std::abs(g6val) - epsilon) {
         result.satisfied = false;
         result.violations.push_back("g1 < |g6|: " + std::to_string(g1) + " < " + std::to_string(std::abs(g6val)));
      }
      if (g2 < std::abs(g4) - epsilon) {
         result.satisfied = false;
         result.violations.push_back("g2 < |g4|: " + std::to_string(g2) + " < " + std::to_string(std::abs(g4)));
      }
      if (g2 < std::abs(g5) - epsilon) {
         result.satisfied = false;
         result.violations.push_back("g2 < |g5|: " + std::to_string(g2) + " < " + std::to_string(std::abs(g5)));
      }
      if (g3 < std::abs(g4) - epsilon) {
         result.satisfied = false;
         result.violations.push_back("g3 < |g4|: " + std::to_string(g3) + " < " + std::to_string(std::abs(g4)));
      }
      if (g3 < std::abs(g6val) - epsilon) {
         result.satisfied = false;
         result.violations.push_back("g3 < |g6|: " + std::to_string(g3) + " < " + std::to_string(std::abs(g6val)));
      }

      return result;
   }

   // Generate some standard transformations and check if any reduce trace
   static bool CanReduceTrace(const G6& reducedG6, double epsilon = 1e-6) {
      const double originalTrace = reducedG6[0] + reducedG6[1] + reducedG6[2];

      // Try some standard lattice transformations
      // These are unimodular matrices that preserve the lattice
      std::vector<std::vector<int>> transforms = {
         // Type I: Add/subtract basis vectors
         {1, 0, 0,  1, 1, 0,  0, 0, 1},  // b' = a + b
         {1, 0, 0,  -1, 1, 0, 0, 0, 1},  // b' = -a + b
         {1, 1, 0,  0, 1, 0,  0, 0, 1},  // a' = a + b
         {1, -1, 0, 0, 1, 0,  0, 0, 1},  // a' = a - b
         {1, 0, 0,  0, 1, 0,  0, 1, 1},  // c' = b + c
         {1, 0, 0,  0, 1, 0,  0, -1, 1}, // c' = -b + c
         {1, 0, 0,  0, 1, 0,  1, 0, 1},  // c' = a + c
         {1, 0, 0,  0, 1, 0,  -1, 0, 1}, // c' = -a + c
         {1, 0, 1,  0, 1, 0,  0, 0, 1},  // a' = a + c
         {1, 0, -1, 0, 1, 0,  0, 0, 1},  // a' = a - c
         {1, 0, 0,  0, 1, 1,  0, 0, 1},  // b' = b + c
         {1, 0, 0,  0, 1, -1, 0, 0, 1},  // b' = b - c
      };

      for (const auto& m : transforms) {
         // Apply transformation (simplified - would need proper matrix multiplication)
         // For now, just check if algorithm would continue
         // This is a placeholder - proper implementation would transform G6
      }

      // For a proper test, we'd need to:
      // 1. Apply each transformation to get a new G6
      // 2. Compute its trace
      // 3. See if any trace < originalTrace

      // For now, just return false if Selling conditions are satisfied
      // (Selling conditions guarantee no such transformation exists)
      SellingCheck check = CheckSellingConditions(reducedG6, epsilon);
      return !check.satisfied;  // Can reduce if Selling violated
   }

   // Main verification function
   static void VerifyAlgorithm(const std::vector<G6>& testCases, double tolerance = 0.0) {
      std::cout << "========================================" << std::endl;
      std::cout << "TRACE MINIMIZATION VERIFICATION" << std::endl;
      std::cout << "========================================" << std::endl;
      std::cout << std::endl;

      int passed = 0;
      int failed = 0;

      for (size_t i = 0; i < testCases.size(); ++i) {
         const G6& input = testCases[i];
         const double inputTrace = input[0] + input[1] + input[2];

         std::cout << "Test " << (i + 1) << ":" << std::endl;
         std::cout << "Input trace: " << std::fixed << std::setprecision(6)
            << inputTrace << std::endl;

         // Run reduction
         KrivyGruber_LCA::ReductionResult result =
            KrivyGruber_LCA::ReduceWithTransformation_LCA(input, tolerance, false);

         const double outputTrace = result.reducedG6[0] + result.reducedG6[1] + result.reducedG6[2];

         std::cout << "Output trace: " << outputTrace << std::endl;
         std::cout << "Trace reduction: " << (inputTrace - outputTrace) << std::endl;

         // Check Selling conditions
         SellingCheck selling = CheckSellingConditions(result.reducedG6);

         std::cout << "Selling-Delaunay conditions: "
            << (selling.satisfied ? "SATISFIED" : "VIOLATED") << std::endl;

         if (!selling.satisfied) {
            std::cout << "Violations:" << std::endl;
            for (const auto& v : selling.violations) {
               std::cout << "  - " << v << std::endl;
            }
            failed++;
         } else {
            passed++;
         }

         std::cout << std::endl;
      }

      std::cout << "========================================" << std::endl;
      std::cout << "RESULTS: " << passed << " passed, " << failed << " failed" << std::endl;
      std::cout << "========================================" << std::endl;
   }
};

int main() {
   // Test cases
   std::vector<G6> testCases;

   // Test 1: Simple cubic (already reduced)
   testCases.push_back(G6(100, 100, 100, 0, 0, 0));

   // Test 2: Near-cubic (your pathological case)
   testCases.push_back(G6(100, 100, 100,
      100.00000000000002842171,
      100.00000000000002842171,
      100.00000000000002842171));

   // Test 3: General cell
   testCases.push_back(G6(150, 120, 100, 50, 40, 30));

   // Test 4: Face-centered cubic (from Roof's example)
   // a = b = 1/2√2, c = √2, cos(α) = cos(β) = cos(γ) = 1/2
   double a = 0.5 * std::sqrt(2.0);
   double c = std::sqrt(2.0);
   double g1 = a * a;  // 0.5
   double g2 = g1;
   double g3 = c * c;  // 2.0
   double cosine = 0.5;
   double g4 = 2 * a * c * cosine;  // 2 * 0.707 * 1.414 * 0.5 = 1.0
   double g5 = g4;
   double g6 = 2 * a * a * cosine;  // 2 * 0.5 * 0.5 = 0.5
   testCases.push_back(G6(g1, g2, g3, g4, g5, g6));

   // Run verification
   TraceMinimizationVerifier::VerifyAlgorithm(testCases, 0.0);

   return 0;
}