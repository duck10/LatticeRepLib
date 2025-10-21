#include "NiggliProjectors.h"
#include "Niggli.h"
#include "Polar.h"
#include <iostream>
#include <iomanip>

#include <chrono>
#include <vector>
#include <cmath>

struct TestResult {
   G6 input;
   G6 projectorOutput;
   G6 niggliOutput;
   double maxDiff;
   bool match;
   double projectorTime;
   double niggliTime;
};



void compareResults(const G6& result1, const G6& result2, const std::string& label1, const std::string& label2) {
   std::cout << "\n" << std::string(70, '=') << "\n";
   std::cout << "Comparing " << label1 << " vs " << label2 << "\n";
   std::cout << label1 << ": " << result1 << "\n";
   std::cout << label2 << ": " << result2 << "\n";

   // Check component differences
   double maxDiff = 0.0;
   for (int i = 0; i < 6; ++i) {
      double diff = std::abs(result1[i] - result2[i]);
      maxDiff = std::max(maxDiff, diff);
      if (diff > 1e-6) {
         std::cout << "  g" << (i + 1) << " differs by: " << diff << "\n";
      }
   }
   std::cout << "Max component difference: " << maxDiff << "\n";

   // Check if they represent the same cell (G6 metric)
   double metric1 = result1[0] + result1[1] + result1[2];
   double metric2 = result2[0] + result2[1] + result2[2];
   std::cout << "Sum(g1,g2,g3): " << metric1 << " vs " << metric2 << "\n";

   // Check special conditions
   if (std::abs(result1[1] - result1[2]) < 1e-8) {
      std::cout << "Both at g2=g3 boundary\n";
      std::cout << "  Result1: |g5|=" << std::abs(result1[4]) << ", |g6|=" << std::abs(result1[5])
         << " (|g5|<=|g6|: " << (std::abs(result1[4]) <= std::abs(result1[5]) ? "YES" : "NO") << ")\n";
      std::cout << "  Result2: |g5|=" << std::abs(result2[4]) << ", |g6|=" << std::abs(result2[5])
         << " (|g5|<=|g6|: " << (std::abs(result2[4]) <= std::abs(result2[5]) ? "YES" : "NO") << ")\n";
   }
}


bool NiggliReduceByProjectors(const G6& input, G6& output, bool debug, int maxIterations) {
   NiggliProjectors np;
   G6 current = input;
   double epsilon = 1.0e-12;

   if (debug) {
      std::cout << "\n======================================================================\n";
      std::cout << "Test: Debug test\n";
      std::cout << "Input: " << input << "\n\n";
   }

   // Initial Phase 1
   current = np.phase1(current, debug);

   for (int iteration = 0; iteration < maxIterations; ++iteration) {
      if (debug) {
         std::cout << "\n  Iteration " << iteration << ": " << current << "\n";
      }

      // Store state before any changes for cycle detection
      G6 beforeIteration = current;

      // Check all boundaries and find worst violation
      std::string worstBoundary;
      bool worstIsAcute;
      double worstDistance = 0.0;
      int worstPriority = 0;
      np.findWorstViolation(current, worstBoundary, worstIsAcute, worstDistance, worstPriority, debug);

      // Check convergence
      if (worstDistance >= -epsilon) {
         if (debug) {
            std::cout << "    CONVERGED\n";
         }

         // Handle tie-breaking for equal scalars
         if (std::abs(current[0] - current[1]) < 1.0e-10) {
            if (std::abs(current[3]) > std::abs(current[4])) {
               std::swap(current[3], current[4]);
            }
         }
         if (std::abs(current[1] - current[2]) < 1.0e-10) {
            if (std::abs(current[4]) > std::abs(current[5])) {
               std::swap(current[4], current[5]);
            }
         }

         output = current;
         return true;
      }

      // Apply ONE transformation
      bool transformationBranch = worstIsAcute;

      // For boundaries 3, 4, 5: transformation choice depends on SIGN, not which branch detected violation
      if (worstBoundary == "3" || worstBoundary == "4" || worstBoundary == "5") {
         int componentIndex = (worstBoundary == "3") ? 3 : (worstBoundary == "4") ? 4 : 5;
         transformationBranch = (current[componentIndex] > 0);
         if (debug) {
            std::cout << "    Applying " << (transformationBranch ? "acute" : "obtuse")
               << " boundary " << worstBoundary << " (sign-based selection: g"
               << (componentIndex + 1) << "=" << current[componentIndex]
               << ", priority=" << worstPriority << ", distance=" << worstDistance << ")\n";
         }
      } else {
         if (debug) {
            std::cout << "    Applying " << (transformationBranch ? "acute" : "obtuse")
               << " boundary " << worstBoundary << " (branch-based selection, priority="
               << worstPriority << ", distance=" << worstDistance << ")\n";
         }
      }

      if (debug) {
         std::cout << "    Before: " << current << "\n";
      }

      current = np.applyTransformation(current, worstBoundary, transformationBranch);

      if (debug) {
         std::cout << "    After: " << current << "\n";
      }

      // Apply Phase 1 (sorting and sign correction)
      current = np.phase1(current, debug);

      // Check for cycle detection
      if ((current - beforeIteration).norm() < epsilon) {
         if (debug) {
            std::cout << "    CONVERGED: No net change after transformation cycle\n";
         }

         // Handle tie-breaking for equal scalars
         if (std::abs(current[0] - current[1]) < 1.0e-10) {
            if (std::abs(current[3]) > std::abs(current[4])) {
               std::swap(current[3], current[4]);
            }
         }
         if (std::abs(current[1] - current[2]) < 1.0e-10) {
            if (std::abs(current[4]) > std::abs(current[5])) {
               std::swap(current[4], current[5]);
            }
         }

         output = current;
         return true;
      }
   }

   if (debug) {
      std::cout << "    FAILED TO CONVERGE after " << maxIterations << " iterations\n\n";
      std::cout << "Output: " << current << "\n";
      std::cout << "Converged: NO\n";
   }
   output = current;
   return false;
}


void runRandomTests(int numTests = 100) {
   NiggliProjectors np;
   std::vector<TestResult> results;

   int exactMatches = 0;
   int closeMatches = 0;  // within 1e-6
   int mismatches = 0;
   double totalProjectorTime = 0.0;
   double totalNiggliTime = 0.0;
   double maxDifference = 0.0;

   std::cout << "\nRunning " << numTests << " random tests...\n";
   std::cout << std::string(70, '=') << "\n\n";

   for (int i = 0; i < numTests; ++i) {
      TestResult result;
      result.input = Polar::rand();

      if (!LRL_Cell(result.input).IsValid()) {
         continue;
      }

      // Make sure input is primitive and reasonable
      if (result.input[0] < 1.0) result.input[0] = 1.0;
      if (result.input[1] < 1.0) result.input[1] = 1.0;
      if (result.input[2] < 1.0) result.input[2] = 1.0;

      // Time projector algorithm
      auto start = std::chrono::high_resolution_clock::now();
      const bool debugThis = (i == -1); // for which iteration(s) to print debug info for
      if (debugThis) {
         std::cout << "\n DEBUGGING CASE " << i << std::endl;
      }
      bool projectorConverged = NiggliReduceByProjectors(result.input, result.projectorOutput, debugThis, 100);

      auto end = std::chrono::high_resolution_clock::now();
      result.projectorTime = std::chrono::duration<double, std::micro>(end - start).count();

      // Check if output is actually Niggli reduced
      if (projectorConverged && !Niggli::IsNiggli(result.projectorOutput)) {
         std::cout << "WARNING: Projection Algorithm converged but output not Niggli reduced (test " << (i + 1) << ")\n";
         std::cout << "  Output: " << result.projectorOutput << std::endl;
      }


      // Time standard Niggli
      MatG6 m;
      start = std::chrono::high_resolution_clock::now();
      bool niggliConverged = Niggli::Reduce(result.input, m, result.niggliOutput, 1.0e-12);
      end = std::chrono::high_resolution_clock::now();
      result.niggliTime = std::chrono::duration<double, std::micro>(end - start).count();

      // Compare results
      result.maxDiff = 0.0;
      for (int j = 0; j < 6; ++j) {
         double diff = std::abs(result.projectorOutput[j] - result.niggliOutput[j]);
         result.maxDiff = std::max(result.maxDiff, diff);
      }

      maxDifference = std::max(maxDifference, result.maxDiff);

      if (result.maxDiff < 1.0e-8) {
         result.match = true;
         exactMatches++;
      } else if (result.maxDiff < 1.0e-6) {
         result.match = true;
         closeMatches++;
      } else {
         result.match = false;
         mismatches++;
         // Print mismatches
         std::cout << "MISMATCH #" << (mismatches) << " (test " << (i + 1) << "):\n";
         std::cout << "  Input:     " << result.input << "\n";
         std::cout << "  Projector: " << result.projectorOutput << "\n";
         std::cout << "  Niggli:    " << result.niggliOutput << "\n";
         std::cout << "  Max diff:  " << result.maxDiff << "\n\n";
      }

      totalProjectorTime += result.projectorTime;
      totalNiggliTime += result.niggliTime;
      results.push_back(result);

      if (!projectorConverged) {
         std::cout << "WARNING: Projector failed to converge on test " << (i + 1) << "\n";
      }
      if (!niggliConverged) {
         std::cout << "WARNING: Niggli failed to converge on test " << (i + 1) << "\n";
      }
   }

   // Print summary
   std::cout << "\n" << std::string(70, '=') << "\n";
   std::cout << "SUMMARY\n";
   std::cout << std::string(70, '=') << "\n\n";

   std::cout << "Results:\n";
   std::cout << "  Exact matches (diff < 1e-8):  " << exactMatches << " ("
      << (100.0 * exactMatches / numTests) << "%)\n";
   std::cout << "  Close matches (diff < 1e-6):   " << closeMatches << " ("
      << (100.0 * closeMatches / numTests) << "%)\n";
   std::cout << "  Mismatches (diff >= 1e-6):     " << mismatches << " ("
      << (100.0 * mismatches / numTests) << "%)\n";
   std::cout << "  Maximum difference found:      " << maxDifference << "\n\n";

   std::cout << "Timing (microseconds):\n";
   std::cout << "  Projector algorithm:\n";
   std::cout << "    Total:   " << totalProjectorTime << " ?s\n";
   std::cout << "    Average: " << (totalProjectorTime / numTests) << " ?s\n";
   std::cout << "  Standard Niggli:\n";
   std::cout << "    Total:   " << totalNiggliTime << " ?s\n";
   std::cout << "    Average: " << (totalNiggliTime / numTests) << " ?s\n";
   std::cout << "  Speed ratio: " << (totalProjectorTime / totalNiggliTime) << "x\n\n";

   // Find fastest and slowest cases
   auto minProjector = std::min_element(results.begin(), results.end(),
      [](const TestResult& a, const TestResult& b) { return a.projectorTime < b.projectorTime; });
   auto maxProjector = std::max_element(results.begin(), results.end(),
      [](const TestResult& a, const TestResult& b) { return a.projectorTime < b.projectorTime; });

   std::cout << "Projector timing range: " << minProjector->projectorTime
      << " to " << maxProjector->projectorTime << " ?s\n";
   std::cout << "Niggli timing range:    "
      << std::min_element(results.begin(), results.end(),
         [](const TestResult& a, const TestResult& b) { return a.niggliTime < b.niggliTime; })->niggliTime
      << " to "
      << std::max_element(results.begin(), results.end(),
         [](const TestResult& a, const TestResult& b) { return a.niggliTime < b.niggliTime; })->niggliTime
      << " ?s\n";
}


void testCase(const NiggliProjectors& np, const std::string& description, const G6& input, bool debug = false) {
   const LRL_Cell cell(input);
   if (!cell.IsValid()) {
      std::cout << "ERROR IN INPUT CELL IN testcase --------------------\n";
      std::cout << input << input << std::endl;
      std::cout << "--------------------" << std::endl;
      return;
   }

   std::cout << "\n" << std::string(70, '=') << "\n";
   std::cout << "Test: " << description << "\n";
   std::cout << "Input:  " << input << "\n";





   G6 output;
   bool converged = NiggliReduceByProjectors(input, output, debug, 500);

   std::cout << "\nOutput: " << output << "\n";
   std::cout << "Converged: " << (converged ? "YES" : "NO") << "\n";

   bool isReduced = Niggli::IsNiggli(output);
   std::cout << "Is Niggli reduced: " << (isReduced ? "YES" : "NO") << "\n";

   G6 niggliOutput;
   MatG6 m;
   Niggli::Reduce(input, m, niggliOutput, 1.0e-8);
   std::cout << "Standard Niggli:    " << niggliOutput << "\n";

   double diff = 0.0;
   for (int i = 0; i < 6; ++i) {
      diff = std::max(diff, std::abs(output[i] - niggliOutput[i]));
   }
   std::cout << "Max difference: " << diff << "\n";
   std::cout << (diff < 1.0e-6 ? "? MATCH" : "? MISMATCH") << "\n";
}


int main() {

   NiggliProjectors np;
   // 33277 is not a valid unit cell
   //testCase(np, "case 33277", G6(6.486, 46.833, 30.720, -37.984, 27.277, -24.644), true);

   testCase(np, "case 8761", G6(7.718,    14.190,    59.275,    56.091,    41.646,    18.487), true);

   testCase(np, "case 10130", G6(78.145, 6.409, 1.485, -6.169, -4.536, 8.971), true);
   testCase(np, "acute", G6(0.825,     1.000,    65.850,     0.611,     0.051,     0.825), true);
   testCase(np, "obtuse", G6(78.145, 6.409, 1.485, -6.169, -4.536, 8.971), true);


   //G6 problemCell(6.763, 13.768, 30.217, -13.250, -5.470, -1.842);
   //G6 problemCell(6.763, 13.768, 30.217, -13.250, -5.470, -1.842);
   std::string boundary;
   bool isAcute;
   double distance;
   int priority;

   //std::cout << "\n=== DEBUGGING PROBLEM CELL ===\n";
   //np.findWorstViolation(problemCell, boundary, isAcute, distance, priority, true);

   //std::cout << "\nResult:\n";
   //std::cout << "  Worst boundary: " << boundary << "\n";
   //std::cout << "  Distance: " << distance << "\n";
   //std::cout << "  Is acute: " << isAcute << "\n";
   //std::cout << "  Priority: " << priority << "\n";

   G6 intermediate(6.763, 13.768, 30.217, -13.250, -5.470, -1.842);
   //std::cout << "Before: " << intermediate << "\n";
   //std::cout << "IsNiggli: " << Niggli::IsNiggli(intermediate, 1.0e-12) << "\n";

   MatG6 m;
   //G6 output;
   //Niggli::Reduce(intermediate, m, output, 1.0e-12);

   //std::cout << "After: " << output << "\n";
   //std::cout << "Transformation applied\n";

   //G6 testCell(6.763, 13.768, 30.217, -13.250, -5.470, -1.842);
   //MatG6 m;
   //G6 niggliOutput;
   //bool converged = Niggli::Reduce(testCell, m, niggliOutput, 1.0e-12);
   //std::cout << "Niggli::Reduce converged: " << converged << "\n";
   //std::cout << "IsNiggli: " << Niggli::IsNiggli(niggliOutput, 1.0e-12) << "\n";
   //std::cout << "Output: " << niggliOutput << "\n";


   //NiggliProjectors nptest;
   //nptest.testTransformationMatrices();




   // Test if the expected answer is actually Niggli reduced
   //G6 testCell(2.358, 32.210, 34.022, -6.445, -1.319, -1.998);
   //bool isNiggliReduced = Niggli::IsNiggli(testCell);
   //std::cout << "Target cell is Niggli reduced: " << (isNiggliReduced ? "YES" : "NO") << std::endl;

   
   //   testCase(np, "case 98", G6(13.768, 74.707, 6.763, 22.681, -1.842, -43.665), true);
   //testCase(np, "case 10130", G6(78.145, 6.409, 1.485, -6.169, -4.536, 8.971), true);

   //std::cout << "\n" << std::string(70, '=') << "\n";
   //std::cout << "DEBUG: One failing case with full trace\n";
   //std::cout << std::string(70, '=') << "\n";
   //testCase(np, "Debug test", G6(37.646, 37.699, 2.358, 6.035, 7.434, 16.517), true);

   //G6 failedCase(4.709, 16.520, 62.961, -7.205, -2.347, -13.663);
   //std::cout << "\nDEBUG: Checking failed case boundaries:\n";

   //std::string worstBoundary;
   //bool worstIsAcute;
   //double worstDistance;
   //int worstPriority;

   //np.findWorstViolation(failedCase, worstBoundary, worstIsAcute, worstDistance, worstPriority, true);

   //std::cout << "Worst boundary: " << worstBoundary << std::endl;
   //std::cout << "Worst distance: " << worstDistance << std::endl;
   //std::cout << "Is acute: " << worstIsAcute << std::endl;
   //std::cout << "Priority: " << worstPriority << std::endl;



   //testCase(np, "Test ", G6(4.709, 16.520, 62.961, -7.205, -2.347, -13.663), true);

   //testCase(np, "Test 67", G6(6.954, 80.732, 4.474, 4.337, 2.106, -37.846), true);
   //testCase(np, "Test 896", G6(44.912, 1.000, 1.000, 0.425, -1.429, 1.821), true);



   runRandomTests(1000000);  // Test with 1000 random cases





   G6 input(10, 12, 15, -14, -5, -6);
   G6 projectorResult, niggliResult;
   //MatG6 m;

   NiggliReduceByProjectors(input, projectorResult, false, 100);
   Niggli::Reduce(input, m, niggliResult, 1.0e-8);

   compareResults(projectorResult, niggliResult, "Projector", "Niggli::Reduce");





   std::cout << "Testing NiggliReduceByProjectors with Debug Output\n";

   // Simple test first
   std::cout << "\n" << std::string(70, '-') << "\n";
   std::cout << "PART 1: Standard Presentation (Boundary 1)\n";
   std::cout << std::string(70, '-') << "\n";

   testCase(np, "g1 > g2 (needs swap)", G6(12, 10, 15, 5, 4, 6), false);

   // Face diagonal test with debug
   std::cout << "\n" << std::string(70, '-') << "\n";
   std::cout << "PART 2: Face Diagonal Acute (Boundary 6/7)\n";
   std::cout << std::string(70, '-') << "\n";

   testCase(np, "|g4| > g2 (violates face diagonal)", G6(10, 12, 15, 14, 5, 6), true);

   // Obtuse branch test
   std::cout << "\n" << std::string(70, '-') << "\n";
   std::cout << "PART 3: Face Diagonal Obtuse (Boundary 8) - WITH DEBUG\n";
   std::cout << std::string(70, '-') << "\n";

   // Test obtuse case: g4,g5,g6 all negative, |g4| > g2
   testCase(np, "Obtuse: |g4| > g2", G6(10, 12, 15, -14, -5, -6), true);

   return 0;
}
