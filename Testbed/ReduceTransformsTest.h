#ifndef REDUCETRANSFORMSTEST_H
#define REDUCETRANSFORMSTEST_H

#include "G6.h"
#include "LRL_Cell.h"
#include "LRL_Vector3.h"
#include "Matrix_3x3.h"
#include "Niggli.h"
#include "StoreResults.h"
#include "PairReporter.h"

// Forward declarations
class ComparisonResults;
Matrix_3x3 ConvertG6ToMatrix3x3(const G6& cell);

class ComparisonResults {
public:
   ComparisonResults()
      : m_successCount(0)
      , m_mismatchCount(0)
      , m_failureCount(0)
      , m_totalTests(0)
   {
   }

   void IncrementSuccess() { ++m_successCount; }
   void IncrementMismatch() { ++m_mismatchCount; }
   void IncrementFailure() { ++m_failureCount; }
   void IncrementTotalTests() { ++m_totalTests; }

   size_t GetSuccessCount() const { return m_successCount; }
   size_t GetMismatchCount() const { return m_mismatchCount; }
   size_t GetFailureCount() const { return m_failureCount; }
   size_t GetTotalTests() const { return m_totalTests; }

   double GetMatchRate() const {
      return m_totalTests > 0 ? (static_cast<double>(m_successCount) / m_totalTests * 100.0) : 0.0;
   }

   friend std::ostream& operator<<(std::ostream& os, const ComparisonResults& results) {
      os << "Comparison Results:\n";
      os << "  Total tests: " << results.m_totalTests << "\n";
      os << "  Matches: " << results.m_successCount
         << " (" << results.GetMatchRate() << "%)\n";
      os << "  Mismatches: " << results.m_mismatchCount
         << " (" << (static_cast<double>(results.m_mismatchCount) / results.m_totalTests * 100.0) << "%)\n";
      os << "  Failures: " << results.m_failureCount
         << " (" << (static_cast<double>(results.m_failureCount) / results.m_totalTests * 100.0) << "%)\n";
      return os;
   }

private:
   size_t m_successCount;
   size_t m_mismatchCount;
   size_t m_failureCount;
   size_t m_totalTests;
};

// Struct to store detailed information about each test case
struct TestCaseDetails {
   G6 inputCell;
   G6 reducedCell1;      // From standard Reduce
   G6 reducedCell2;      // From ReduceWithTransforms
   Matrix_3x3 transform; // 3D transformation matrix from ReduceWithTransforms
   double maxDifference;
   double transformError;
   bool success1;        // Did standard Reduce succeed?
   bool success2;        // Did ReduceWithTransforms succeed?

   friend std::ostream& operator<<(std::ostream& os, const TestCaseDetails& details) {
      os << "Input G6: " << details.inputCell << "\n";
      os << "Reduced Cell (Reduce): " << details.reducedCell1 << "\n";
      os << "Reduced Cell (ReduceWithTransforms): " << details.reducedCell2 << "\n";
      os << "Max Difference between cells: " << details.maxDifference << "\n";
      os << "3D Transform Error: " << details.transformError << "\n";
      return os;
   }
};

// Function to test the ReduceWithTransforms method by comparing with regular Reduce
ComparisonResults TestReduceWithTransforms(const size_t numTests, std::ostream& logStream = std::cout) {
   StoreResults<double, std::string> transformStats(10);
   transformStats.SetTitle("Comparison between Reduce and ReduceWithTransforms");

   StoreResults<double, TestCaseDetails> mismatchCases(10);
   mismatchCases.SetTitle("Detailed Mismatch Cases");

   // Random number generation
   std::random_device rd;
   std::mt19937 gen(rd());

   logStream << "Testing consistency between Niggli::Reduce and Niggli::ReduceWithTransforms\n";
   logStream << "==============================================================\n\n";

   ComparisonResults results;

   for (size_t test = 0; test < numTests; ++test) {
      results.IncrementTotalTests();

      // Generate a valid cell using your random generators
      LRL_Cell randomCell = Polar::rand();
      G6 inputCell = G6(randomCell);

      // Make sure the cell is valid
      if (!inputCell.IsValid() || !LRL_Cell(inputCell).IsValid()) {
         --test; // Try again with a different cell
         continue;
      }

      // Apply standard Niggli reduction
      MatG6 matG6;
      G6 reducedCell1;
      const bool success1 = Niggli::Reduce(inputCell, matG6, reducedCell1, 1.0E-10);

      // Apply Niggli reduction with transforms
      MatG6 matG6_2;
      Matrix_3x3 mat3d;
      G6 reducedCell2;
      const bool success2 = Niggli::ReduceWithTransforms(inputCell, matG6_2, mat3d, reducedCell2, 1.0E-10);

      // Compare results
      if (!success1 || !success2) {
         results.IncrementFailure();
         transformStats.Store(0.0, "ReductionFailed");
         continue;
      }

      // Calculate difference between the two reduced cells
      double maxDiff = 0.0;
      for (size_t i = 0; i < 6; ++i) {
         maxDiff = std::max(maxDiff, std::abs(reducedCell1[i] - reducedCell2[i]));
      }

      // Calculate the 3D transformation error
      const Matrix_3x3 origCell3D = ConvertG6ToMatrix3x3(inputCell);
      const Matrix_3x3 resultCell3D = mat3d * origCell3D;
      const Matrix_3x3 expectedCell3D = ConvertG6ToMatrix3x3(reducedCell2);

      double mat3dError = 0.0;
      for (size_t i = 0; i < 9; ++i) {
         mat3dError = std::max(mat3dError, std::abs(resultCell3D[i] - expectedCell3D[i]));
      }

      // Check if the reduced cells match (within tolerance)
      if (maxDiff < 1.0E-6) {
         results.IncrementSuccess();
         transformStats.Store(1.0, "Match");
      }
      else {
         results.IncrementMismatch();
         transformStats.Store(0.0, "Mismatch");

         // Store details for mismatches
         TestCaseDetails details;
         details.inputCell = inputCell;
         details.reducedCell1 = reducedCell1;
         details.reducedCell2 = reducedCell2;
         details.transform = mat3d;
         details.maxDifference = maxDiff;
         details.transformError = mat3dError;
         details.success1 = success1;
         details.success2 = success2;

         mismatchCases.Store(maxDiff, details);
      }

      // Progress indicator
      if ((test + 1) % 100 == 0) {
         std::cout << "Completed " << (test + 1) << " tests. ";
         std::cout << "Matches: " << results.GetSuccessCount()
            << ", Mismatches: " << results.GetMismatchCount()
            << ", Failures: " << results.GetFailureCount() << std::endl;
      }
   }

   // Output summary statistics
   logStream << "=============== Summary ===============\n";
   logStream << results << "\n";

   // Show detailed statistics
   logStream << "\nDetailed Statistics:\n";
   transformStats.ShowResults();

   // Show detailed mismatch cases
   if (results.GetMismatchCount() > 0) {
      logStream << "\nDetailed Mismatch Cases:\n";
      mismatchCases.ShowResults();
   }

   return results;
}

// Helper function to convert G6 to Matrix_3x3 representation
Matrix_3x3 ConvertG6ToMatrix3x3(const G6& cell) {
   const double a = std::sqrt(cell[0]);
   const double b = std::sqrt(cell[1]);
   const double c = std::sqrt(cell[2]);

   // Calculate cosines of angles from G6
   double cosAlpha = cell[3] / (2.0 * b * c);
   double cosBeta = cell[4] / (2.0 * a * c);
   double cosGamma = cell[5] / (2.0 * a * b);

   // Ensure valid cosines
   cosAlpha = std::max(std::min(cosAlpha, 1.0), -1.0);
   cosBeta = std::max(std::min(cosBeta, 1.0), -1.0);
   cosGamma = std::max(std::min(cosGamma, 1.0), -1.0);

   const double sinGamma = std::sqrt(1.0 - cosGamma * cosGamma);

   // Avoid division by zero
   if (std::abs(sinGamma) < 1.0E-10) {
      // Return identity matrix if we can't build a proper cell
      return Matrix_3x3();
   }

   try {
      return Matrix_3x3(
         a, 0.0, 0.0,
         b * cosGamma, b * sinGamma, 0.0,
         c * cosBeta,
         c * (cosAlpha - cosBeta * cosGamma) / sinGamma,
         c * std::sqrt(1.0 - cosBeta * cosBeta -
            std::pow((cosAlpha - cosBeta * cosGamma) / sinGamma, 2))
      );
   }
   catch (...) {
      std::cerr << "Error creating cell matrix" << std::endl;
      return Matrix_3x3(); // Return identity matrix on error
   }
}

// Function to modify main to include the test
void RunReduceTransformsComparisonTest(const size_t numTests) {
   std::ofstream logFile("niggli_transform_comparison.log");
   if (!logFile.is_open()) {
      std::cerr << "Error: Could not open log file." << std::endl;
      return;
   }

   std::cout << "\nTesting consistency between Reduce and ReduceWithTransforms...\n";
   const ComparisonResults results = TestReduceWithTransforms(numTests, logFile);

   std::cout << "Comparison completed. See 'niggli_transform_comparison.log' for detailed results.\n";
   std::cout << results << std::endl;

   logFile.close();
}

#endif // REDUCETRANSFORMSTEST_H

