#pragma warning (disable: 4996)



#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <fstream>
#include <chrono>
#include <random>
#include <array>
#include <map>
#include <tuple>

// Your required includes
#include "B4.h"
#include "G6.h"
#include "Niggli.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_Vector3.h"
#include "P3.h"
#include "Polar.h"
#include "PairReporter.h"
#include "StoreResults.h"
#include "ReduceTransformsTest.h"

static const double M_PI = 4.0 * atan(1.0);

const static MatG6 spnull("1 0 0 0 0 0   0 1 0 0 0 0   0 0 1 0 0 0   0 0 0 1 0 0   0 0 0 0 1 0   0 0 0 0 0 1");

const static MatG6 sp1("0 1 0 0 0 0   1 0 0 0 0 0   0 0 1 0 0 0   0 0 0 0 1 0   0 0 0 1 0 0   0 0 0 0 0 1");  //0,1  3,4
const static MatG6 sp2("1 0 0 0 0 0   0 0 1 0 0 0   0 1 0 0 0 0   0 0 0 1 0 0   0 0 0 0 0 1   0 0 0 0 1 0");  //1,2  4,5


const static MatG6 sp34a("1 0 0 0 0 0   0 1 0 0 0 0   0 0 1 0 0 0   0 0 0 -1 0 0   0 0 0 0 -1 0   0 0 0 0 0  1");  // -3,-4
const static MatG6 sp34b("1 0 0 0 0 0   0 1 0 0 0 0   0 0 1 0 0 0   0 0 0 -1 0 0   0 0 0 0  1 0   0 0 0 0 0 -1");  // -3,-5
const static MatG6 sp34c("1 0 0 0 0 0   0 1 0 0 0 0   0 0 1 0 0 0   0 0 0  1 0 0   0 0 0 0 -1 0   0 0 0 0 0 -1");  // -4,-5
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


const static MatG6 R5_Plus(" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 -1 0 0   0 -2 0 1 0 0   0 0 0 0 1 -1   0 0 0 0 0 1");
const static MatG6 R5_Minus(" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 +1 0 0   0 +2 0 1 0 0   0 0 0 0 1 +1   0 0 0 0 0 1");
const static MatG6 R6_Plus(" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 -1 0   0 0 0 1 0 -1   -2 0 0 0 1 0   0 0 0 0 0 1");
const static MatG6 R6_Minus(" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 +1 0   0 0 0 1 0 +1   +2 0 0 0 1 0   0 0 0 0 0 1");
const static MatG6 R7_Plus(" 1 0 0 0 0 0   1 1 0 0 0 -1   0 0 1 0 0 0   0 0 0 1 -1 0   0 0 0 0 1 0   -2 0 0 0 0 1");
const static MatG6 R7_Minus(" 1 0 0 0 0 0   1 1 0 0 0 +1   0 0 1 0 0 0   0 0 0 1 +1 0   0 0 0 0 1 0   +2 0 0 0 0 1");
const static MatG6 R8(" 1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 2 0 1 0 1   2 0 0 0 1 1    0 0 0 0 0 1");
const static MatG6 R9_Plus(R5_Plus);
const static MatG6 R9_Minus(R5_Minus);
const static MatG6 R10_Plus(R6_Plus);
const static MatG6 R10_Minus(R6_Minus);
const static MatG6 R11_Plus(R7_Plus);
const static MatG6 R11_Minus(R7_Minus);
const static MatG6 R12("1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 -2 0 -1 0 -1   -2 0 0 0 -1 -1   0 0 0 0 0 1");


// Define all 3D matrices corresponding to G6 transformations used in Niggli reduction

// R5 operations
const Matrix_3x3 R5_Plus_3D(
   1.0, 0.0, 0.0,  // a' = a
   0.0, 1.0, 0.0,  // b' = b
   0.0, -1.0, 1.0  // c' = -b + c
);

const Matrix_3x3 R5_Minus_3D(
   1.0, 0.0, 0.0,  // a' = a
   0.0, 1.0, 0.0,  // b' = b
   0.0, 1.0, 1.0   // c' = b + c
);

// R6 operations
const Matrix_3x3 R6_Plus_3D(
   1.0, 0.0, 0.0,  // a' = a
   0.0, 1.0, 0.0,  // b' = b
   -1.0, 0.0, 1.0  // c' = -a + c
);

const Matrix_3x3 R6_Minus_3D(
   1.0, 0.0, 0.0,  // a' = a
   0.0, 1.0, 0.0,  // b' = b
   1.0, 0.0, 1.0   // c' = a + c
);

// R7 operations
const Matrix_3x3 R7_Plus_3D(
   1.0, 0.0, 0.0,  // a' = a
   -1.0, 1.0, 0.0, // b' = -a + b
   0.0, 0.0, 1.0   // c' = c
);

const Matrix_3x3 R7_Minus_3D(
   1.0, 0.0, 0.0,  // a' = a
   1.0, 1.0, 0.0,  // b' = a + b
   0.0, 0.0, 1.0   // c' = c
);

// R8 operation
const Matrix_3x3 R8_3D(
   1.0, 0.0, 0.0,  // a' = a
   0.0, 1.0, 0.0,  // b' = b
   1.0, 1.0, 1.0   // c' = a + b + c
);

// R9 operations
const Matrix_3x3 R9_Plus_3D(
   1.0, -2.0, 0.0, // a' = a - 2b
   0.0, 1.0, 0.0,  // b' = b
   0.0, 0.0, 1.0   // c' = c
);

const Matrix_3x3 R9_Minus_3D(
   1.0, 2.0, 0.0,  // a' = a + 2b
   0.0, 1.0, 0.0,  // b' = b
   0.0, 0.0, 1.0   // c' = c
);

// R10 operations
const Matrix_3x3 R10_Plus_3D(
   1.0, 0.0, -2.0, // a' = a - 2c
   0.0, 1.0, 0.0,  // b' = b
   0.0, 0.0, 1.0   // c' = c
);

const Matrix_3x3 R10_Minus_3D(
   1.0, 0.0, 2.0,  // a' = a + 2c
   0.0, 1.0, 0.0,  // b' = b
   0.0, 0.0, 1.0   // c' = c
);

// R11 operations
const Matrix_3x3 R11_Plus_3D(
   1.0, 0.0, 0.0,  // a' = a
   0.0, 1.0, -2.0, // b' = b - 2c
   0.0, 0.0, 1.0   // c' = c
);

const Matrix_3x3 R11_Minus_3D(
   1.0, 0.0, 0.0,  // a' = a
   0.0, 1.0, 2.0,  // b' = b + 2c
   0.0, 0.0, 1.0   // c' = c
);

// R12 operation
const Matrix_3x3 R12_3D(
   1.0, 1.0, 1.0,    // a' = a + b + c
   -1.0, -1.0, 1.0,  // b' = -a - b + c
   1.0, 1.0, -1.0    // c' = a + b - c
);

// Standard presentation matrices
const Matrix_3x3 sp1_3D(
   0.0, 1.0, 0.0,  // a' = b
   1.0, 0.0, 0.0,  // b' = a
   0.0, 0.0, 1.0   // c' = c
);

const Matrix_3x3 sp2_3D(
   1.0, 0.0, 0.0,  // a' = a
   0.0, 0.0, 1.0,  // b' = c
   0.0, 1.0, 0.0   // c' = b
);

const Matrix_3x3 sp34a_3D(
   1.0, 0.0, 0.0,   // a' = a
   0.0, -1.0, 0.0,  // b' = -b
   0.0, 0.0, -1.0   // c' = -c
);

const Matrix_3x3 sp34b_3D(
   -1.0, 0.0, 0.0,  // a' = -a
   0.0, 1.0, 0.0,   // b' = b
   0.0, 0.0, -1.0   // c' = -c
);

const Matrix_3x3 sp34c_3D(
   -1.0, 0.0, 0.0,  // a' = -a
   0.0, -1.0, 0.0,  // b' = -b
   0.0, 0.0, 1.0    // c' = c
);

// Helper function to get the 3D matrix corresponding to a G6 matrix
Matrix_3x3 Get3DMatrixForG6Matrix(const MatG6& g6Matrix)
{
   // Compare with known G6 matrices and return the corresponding 3D matrix
   if (g6Matrix == R5_Plus) return R5_Plus_3D;
   if (g6Matrix == R5_Minus) return R5_Minus_3D;
   if (g6Matrix == R6_Plus) return R6_Plus_3D;
   if (g6Matrix == R6_Minus) return R6_Minus_3D;
   if (g6Matrix == R7_Plus) return R7_Plus_3D;
   if (g6Matrix == R7_Minus) return R7_Minus_3D;
   if (g6Matrix == R8) return R8_3D;
   if (g6Matrix == R9_Plus) return R9_Plus_3D;
   if (g6Matrix == R9_Minus) return R9_Minus_3D;
   if (g6Matrix == R10_Plus) return R10_Plus_3D;
   if (g6Matrix == R10_Minus) return R10_Minus_3D;
   if (g6Matrix == R11_Plus) return R11_Plus_3D;
   if (g6Matrix == R11_Minus) return R11_Minus_3D;
   if (g6Matrix == R12) return R12_3D;
   if (g6Matrix == sp1) return sp1_3D;
   if (g6Matrix == sp2) return sp2_3D;
   if (g6Matrix == sp34a) return sp34a_3D;
   if (g6Matrix == sp34b) return sp34b_3D;
   if (g6Matrix == sp34c) return sp34c_3D;

   // Default to identity if not recognized
   return Matrix_3x3(
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 0.0, 1.0
   );
}

 G6 operator* (const Matrix_3x3& m, const LRL_Cell& c1) {
    const B4 b4(c1);
    const Vector_3 a = m * b4[0];
    const Vector_3 b = m * b4[1];
    const Vector_3 c = m * b4[2];

    G6 out;
    out[0] = a.Dot(a);
    out[2] = b.Dot(b);
    out[0] = c.Dot(c);
    return out;

 }

bool TestMatrices3D(const MatG6& m6, const Matrix_3x3& m3) {
   const LRL_Cell cell = Polar::rand();
   const G6 cellg6 = cell;
    
   const G6 prodg6 = m6 * cellg6;
   const LRL_Cell xxx = m3 * cell;
   return xxx.IsValid();
}

static StoreResults<double, PairReporter<S6, S6>> g_bestVectors;
static StoreResults<bool, G6> niggliReduceSuccess;



// Generate a random cell with normal distribution
G6 GenerateRandomNormalCell(std::mt19937& gen) {
   std::normal_distribution<> distLength(10.0, 3.0);  // For cell lengths
   std::uniform_real_distribution<> distAngle(-0.7, 0.7);  // For cosines of angles

   double a = fabs(distLength(gen));
   double b = fabs(distLength(gen));
   double c = fabs(distLength(gen));
   double cosAlpha = distAngle(gen);
   double cosBeta = distAngle(gen);
   double cosGamma = distAngle(gen);

   G6 cell;
   cell[0] = a * a;
   cell[1] = b * b;
   cell[2] = c * c;
   cell[3] = 2.0 * b * c * cosAlpha;
   cell[4] = 2.0 * a * c * cosBeta;
   cell[5] = 2.0 * a * b * cosGamma;

   return cell;
}

// Check if angles satisfy triangle inequality and sum < 340°
bool IsValidCellAngles(double alpha, double beta, double gamma) {
   // Check sum < 340°
   if (alpha + beta + gamma >= 340.0) {
      return false;
   }

   // Check triangle inequality for each pair of angles
   if (alpha >= beta + gamma) return false;
   if (beta >= alpha + gamma) return false;
   if (gamma >= alpha + beta) return false;

   // Check that each angle is positive
   if (alpha <= 0.0 || beta <= 0.0 || gamma <= 0.0) return false;

   return true;
}

// Function to check if a G6 cell has valid angles
bool IsValidCell(const G6& cell) {
   if( !cell.IsValid()) return false;
   if (!LRL_Cell(cell).IsValid()) return false;

   // Extract cell parameters
   double a = std::sqrt(cell[0]);
   double b = std::sqrt(cell[1]);
   double c = std::sqrt(cell[2]);

   // Convert G6 to angles
   double cosAlpha = cell[3] / (2.0 * b * c);
   double cosBeta = cell[4] / (2.0 * a * c);
   double cosGamma = cell[5] / (2.0 * a * b);

   // Check for valid cosine values
   if (std::abs(cosAlpha) > 1.0 || std::abs(cosBeta) > 1.0 || std::abs(cosGamma) > 1.0) {
      return false;
   }

   // Convert to degrees
   double alpha = std::acos(cosAlpha) * 180.0 / M_PI;
   double beta = std::acos(cosBeta) * 180.0 / M_PI;
   double gamma = std::acos(cosGamma) * 180.0 / M_PI;

   // Check angle constraints
   return IsValidCellAngles(alpha, beta, gamma);
}

// Function to classify cell characteristics
std::string ClassifyCell(const G6& cell) {
   // Extract cell parameters
   double a = std::sqrt(cell[0]);
   double b = std::sqrt(cell[1]);
   double c = std::sqrt(cell[2]);

   // Extract angles
   double cosAlpha = cell[3] / (2.0 * b * c);
   double cosBeta = cell[4] / (2.0 * a * c);
   double cosGamma = cell[5] / (2.0 * a * b);

   // Ensure valid cosines
   cosAlpha = std::max(std::min(cosAlpha, 1.0), -1.0);
   cosBeta = std::max(std::min(cosBeta, 1.0), -1.0);
   cosGamma = std::max(std::min(cosGamma, 1.0), -1.0);

   double alpha = std::acos(cosAlpha) * 180.0 / M_PI;
   double beta = std::acos(cosBeta) * 180.0 / M_PI;
   double gamma = std::acos(cosGamma) * 180.0 / M_PI;

   // Calculate key metrics
   double aspectRatio = std::max(std::max(a / b, b / a), std::max(std::max(a / c, c / a), std::max(b / c, c / b)));
   double maxAngularDev = std::max(std::max(std::abs(alpha - 90.0), std::abs(beta - 90.0)), std::abs(gamma - 90.0));

   // Calculate matrix condition number (simplified approximation)
   Matrix_3x3 cellMatrix;
   try {
      cellMatrix = Matrix_3x3(
         a, 0.0, 0.0,
         b * cosGamma, b * std::sqrt(1.0 - cosGamma * cosGamma), 0.0,
         c * cosBeta,
         c * (cosAlpha - cosBeta * cosGamma) / std::sqrt(1.0 - cosGamma * cosGamma),
         c * std::sqrt(1.0 - cosBeta * cosBeta - std::pow((cosAlpha - cosBeta * cosGamma) / std::sqrt(1.0 - cosGamma * cosGamma), 2))
      );
   }
   catch (...) {
      return "Invalid_Matrix";
   }

   double conditionNumber = 1.0;
   try {
      Matrix_3x3 invMatrix = cellMatrix.Inver();
      // Simple Frobenius norm-based condition number estimate
      double normA = 0.0, normInvA = 0.0;
      for (int i = 0; i < 9; ++i) {
         normA += cellMatrix[i] * cellMatrix[i];
         normInvA += invMatrix[i] * invMatrix[i];
      }
      conditionNumber = std::sqrt(normA * normInvA);
   }
   catch (...) {
      conditionNumber = 1.0e6; // Very high number indicating poor conditioning
   }

   // Classify the cell
   std::ostringstream oss;

   if (aspectRatio > 10.0) {
      oss << "HighAspect_";
   }
   else if (aspectRatio > 3.0) {
      oss << "ModAspect_";
   }
   else {
      oss << "NormalAspect_";
   }

   if (maxAngularDev > 40.0) {
      oss << "ExtremeAngle_";
   }
   else if (maxAngularDev > 20.0) {
      oss << "ModAngle_";
   }
   else {
      oss << "NearOrtho_";
   }

   if (conditionNumber > 100.0) {
      oss << "PoorCond";
   }
   else if (conditionNumber > 30.0) {
      oss << "ModCond";
   }
   else {
      oss << "GoodCond";
   }

   return oss.str();
}

// Store detailed info about a cell for analysis
void StoreDetailedCellInfo(const G6& cell, bool success, StoreResults<double, std::string>& store) {
   double a = std::sqrt(cell[0]);
   double b = std::sqrt(cell[1]);
   double c = std::sqrt(cell[2]);

   double cosAlpha = cell[3] / (2.0 * b * c);
   double cosBeta = cell[4] / (2.0 * a * c);
   double cosGamma = cell[5] / (2.0 * a * b);

   // Ensure valid cosines
   cosAlpha = std::max(std::min(cosAlpha, 1.0), -1.0);
   cosBeta = std::max(std::min(cosBeta, 1.0), -1.0);
   cosGamma = std::max(std::min(cosGamma, 1.0), -1.0);

   double alpha = std::acos(cosAlpha) * 180.0 / M_PI;
   double beta = std::acos(cosBeta) * 180.0 / M_PI;
   double gamma = std::acos(cosGamma) * 180.0 / M_PI;

   // Store aspect ratio info
   double aspectRatio = std::max(std::max(a / b, b / a), std::max(std::max(a / c, c / a), std::max(b / c, c / b)));
   if (aspectRatio > 10.0) {
      store.Store(success ? 1.0 : 0.0, "AspectRatio>10");
   }
   else if (aspectRatio > 5.0) {
      store.Store(success ? 1.0 : 0.0, "AspectRatio>5");
   }
   else if (aspectRatio > 3.0) {
      store.Store(success ? 1.0 : 0.0, "AspectRatio>3");
   }
   else if (aspectRatio > 2.0) {
      store.Store(success ? 1.0 : 0.0, "AspectRatio>2");
   }
   else {
      store.Store(success ? 1.0 : 0.0, "AspectRatio<2");
   }

   // Store angle deviation info
   double maxAngularDev = std::max(std::max(std::abs(alpha - 90.0), std::abs(beta - 90.0)), std::abs(gamma - 90.0));
   if (maxAngularDev > 40.0) {
      store.Store(success ? 1.0 : 0.0, "AngleDev>40");
   }
   else if (maxAngularDev > 30.0) {
      store.Store(success ? 1.0 : 0.0, "AngleDev>30");
   }
   else if (maxAngularDev > 20.0) {
      store.Store(success ? 1.0 : 0.0, "AngleDev>20");
   }
   else if (maxAngularDev > 10.0) {
      store.Store(success ? 1.0 : 0.0, "AngleDev>10");
   }
   else {
      store.Store(success ? 1.0 : 0.0, "AngleDev<10");
   }

   // Store specific angle types
   if (std::abs(alpha - 90.0) < 2.0 && std::abs(beta - 90.0) < 2.0 && std::abs(gamma - 90.0) < 2.0) {
      store.Store(success ? 1.0 : 0.0, "NearOrthogonal");
   }

   if (std::abs(alpha - 60.0) < 2.0 || std::abs(beta - 60.0) < 2.0 || std::abs(gamma - 60.0) < 2.0) {
      store.Store(success ? 1.0 : 0.0, "NearHexagonal");
   }

   if (std::abs(alpha - 120.0) < 2.0 || std::abs(beta - 120.0) < 2.0 || std::abs(gamma - 120.0) < 2.0) {
      store.Store(success ? 1.0 : 0.0, "Near120Degree");
   }

   // Store info about cells with extreme parameters
   if (alpha + beta + gamma > 300.0) {
      store.Store(success ? 1.0 : 0.0, "AngleSum>300");
   }

   if (a < 2.0 || b < 2.0 || c < 2.0) {
      store.Store(success ? 1.0 : 0.0, "VeryShortAxis");
   }

   if (a > 50.0 || b > 50.0 || c > 50.0) {
      store.Store(success ? 1.0 : 0.0, "VeryLongAxis");
   }

   // Store combined classifications
   std::string cellClass = ClassifyCell(cell);
   store.Store(success ? 1.0 : 0.0, cellClass);

   // For failures, store the input cell for further analysis
   if (!success) {
      // Convert to S6 and store in the global best vectors collection
      S6 s6Cell(cell);
      g_bestVectors.Store(0.0, std::make_pair(s6Cell, S6()));
   }
}

// Main benchmark function to test Niggli reduction
void BenchmarkNiggliReduction(int numTests = 5000) {
   // Create log files
   std::ofstream logFile("niggli_analysis_results.log");
   if (!logFile.is_open()) {
      std::cerr << "Error: Could not open log file." << std::endl;
      return;
   }

   niggliReduceSuccess.SetMaxItemStore(5);
   niggliReduceSuccess.SetTitle("counts of Niggli reduction success");

   // Define a small delta for comparisons
   const double delta = 1.0e-10;

   logFile << "Niggli Reduction Analysis: Detailed Cell Comparison\n";
   logFile << "=============================================\n";

   // Statistics variables
   int successCount = 0;
   int failureCount = 0;
   int invalidCellsSkipped = 0;

   // Advanced classification stats using StoreResults
   StoreResults<double, std::string> cellStats(100); // Store top 100 patterns

   // Random number generation
   std::random_device rd;
   std::mt19937 gen(rd());

   logFile << "Testing " << numTests << " cells...\n\n";

   for (int test = 0; test < numTests; ++test) {
      // Generate a valid cell
      G6 inputCell;
      bool validCell = false;

      while (!validCell) {
         inputCell = GenerateRandomNormalCell(gen);

         // Apply validity checks
         if (!inputCell.IsValid()) {
            invalidCellsSkipped++;
            continue;
         }

         const LRL_Cell cell = inputCell;
         if (!cell.IsValid() || cell[0]<1 || cell[1] < 1 || cell[2] < 1) {
            invalidCellsSkipped++;
            continue;
         }

         validCell = true;
      }

      // Test Niggli reduction
      MatG6 transform;
      G6 reducedCell;
      bool success = Niggli::Reduce(inputCell, transform, reducedCell, delta);
      niggliReduceSuccess.Store(success, inputCell);

      // Store result
      if (success) {
         successCount++;
      }
      else {
         failureCount++;

         // Record detailed information about failures
         if (failureCount <= 100) { // Limit detailed logging to first 100 failures
            double a = std::sqrt(inputCell[0]);
            double b = std::sqrt(inputCell[1]);
            double c = std::sqrt(inputCell[2]);

            double cosAlpha = inputCell[3] / (2.0 * b * c);
            double cosBeta = inputCell[4] / (2.0 * a * c);
            double cosGamma = inputCell[5] / (2.0 * a * b);

            // Ensure valid cosines
            cosAlpha = std::max(std::min(cosAlpha, 1.0), -1.0);
            cosBeta = std::max(std::min(cosBeta, 1.0), -1.0);
            cosGamma = std::max(std::min(cosGamma, 1.0), -1.0);

            double alpha = std::acos(cosAlpha) * 180.0 / M_PI;
            double beta = std::acos(cosBeta) * 180.0 / M_PI;
            double gamma = std::acos(cosGamma) * 180.0 / M_PI;

            logFile << "Failed Case #" << failureCount << ":\n";
            logFile << "  G6: " << inputCell << "\n";
            logFile << "  Cell Parameters: a=" << a << ", b=" << b << ", c=" << c;
            logFile << ", alpha=" << alpha << "°, beta=" << beta << "°, gamma=" << gamma << "°\n";
            logFile << "  Aspect Ratio: " << std::max(std::max(a / b, b / a), std::max(std::max(a / c, c / a), std::max(b / c, c / b))) << "\n";
            logFile << "  Angular Deviations from 90°: " << std::abs(alpha - 90.0) << "°, "
               << std::abs(beta - 90.0) << "°, " << std::abs(gamma - 90.0) << "°\n";
            logFile << "  Classification: " << ClassifyCell(inputCell) << "\n\n";
         }
      }

      // Record detailed statistics for both successes and failures
      StoreDetailedCellInfo(inputCell, success, cellStats);

      // Progress indicator every 1000 tests
      if ((test + 1) % 1000 == 0) {
         std::cout << "Completed " << (test + 1) << " tests. Current success rate: "
            << (double)successCount / (test + 1) * 100.0 << "%" << std::endl;
      }
   }

   // Calculate success percentage
   double successRate = (double)successCount / numTests * 100.0;

   // Output summary statistics
   logFile << "=============== Summary ===============\n";
   logFile << "Total test cases: " << numTests << "\n";
   logFile << "Invalid cells skipped: " << invalidCellsSkipped << "\n";
   logFile << "Success Rate: " << successCount << " / " << numTests << " (" << successRate << "%)\n";
   logFile << "Failure Rate: " << failureCount << " / " << numTests << " (" << (100.0 - successRate) << "%)\n\n";

   // Detailed classification statistics
   logFile << "=============== Detailed Cell Statistics ===============\n";

   // Show the stored results
   logFile << "Cell Classification Statistics:\n";
   cellStats.ShowResults();

   // Output to console as well
   std::cout << "Analysis completed. See 'niggli_analysis_results.log' for detailed results.\n";
   std::cout << "Total test cases: " << numTests << "\n";
   std::cout << "Success Rate: " << successRate << "%\n";
   std::cout << "StoreResults has captured " << cellStats.size() << " different cell patterns.\n";

   logFile.close();

   // Further statistics and analysis can be added here
}


void TestReduceTransformationMatrix(const size_t numTests) {
   std::ofstream logFile("niggli_transform_matrix_validation.log");
   if (!logFile.is_open()) {
      std::cerr << "Error: Could not open log file." << std::endl;
      return;
   }

   StoreResults<double, std::string> transformStats(100);
   transformStats.SetTitle("Validation of 3D Transformation Matrix from ReduceWithTransforms");

   // Random number generation
   std::random_device rd;
   std::mt19937 gen(rd());

   logFile << "Testing if mat3d from ReduceWithTransforms correctly transforms input cell to reduced cell\n";
   logFile << "=========================================================================\n\n";

   size_t successCount = 0;
   size_t mismatchCount = 0;
   size_t failureCount = 0;

   for (size_t test = 0; test < numTests; ++test) {
      // Generate a valid cell
      G6 inputCell;
      bool validCell = false;

      while (!validCell) {
         inputCell = GenerateRandomNormalCell(gen);
         if (!inputCell.IsValid()) continue;
         if (!LRL_Cell(inputCell).IsValid()) continue;
         validCell = true;
      }

      // Apply Niggli reduction with transforms to get both the reduced cell and the 3D transform
      MatG6 matG6;
      Matrix_3x3 mat3d;
      G6 reducedCell;
      const bool success = Niggli::ReduceWithTransforms(inputCell, matG6, mat3d, reducedCell, 1.0E-10);

      // If reduction failed, count it and continue
      if (!success) {
         ++failureCount;
         transformStats.Store(0.0, "ReductionFailed");
         continue;
      }

      // Convert input G6 to LRL_Cell for easier handling
      const LRL_Cell inputCell_LRL(inputCell);
      const LRL_Cell reducedCell_LRL(reducedCell);

      // Try multiple approaches to see what works

      // Approach 1: Extract cell vectors and transform individually
      // a vector
      Vector_3 a_vec(inputCell_LRL[0], 0.0, 0.0);

      // b vector
      const double cosGamma = cos(inputCell_LRL[5]);
      const double sinGamma = sin(inputCell_LRL[5]);
      Vector_3 b_vec(inputCell_LRL[1] * cosGamma, inputCell_LRL[1] * sinGamma, 0.0);

      // c vector
      const double cosAlpha = cos(inputCell_LRL[3]);
      const double cosBeta = cos(inputCell_LRL[4]);
      const double c1 = inputCell_LRL[2] * cosBeta;
      const double c2 = inputCell_LRL[2] * (cosAlpha - cosBeta * cosGamma) / sinGamma;
      const double c3 = inputCell_LRL[2] * sqrt(1.0 - cosBeta * cosBeta -
         pow((cosAlpha - cosBeta * cosGamma) / sinGamma, 2));
      Vector_3 c_vec(c1, c2, c3);

      // Apply transformation to each vector
      Vector_3 a_trans = mat3d * a_vec;
      Vector_3 b_trans = mat3d * b_vec;
      Vector_3 c_trans = mat3d * c_vec;

      // Approach 2: Try inverse transformation
      Matrix_3x3 inv_mat3d;
      try {
         inv_mat3d = mat3d.Inver();
      }
      catch (...) {
         ++failureCount;
         transformStats.Store(0.0, "InverseCalculationFailed");
         continue;
      }

      Vector_3 a_inv = inv_mat3d * a_vec;
      Vector_3 b_inv = inv_mat3d * b_vec;
      Vector_3 c_inv = inv_mat3d * c_vec;

      // Approach 3: Try transposed matrix
      Matrix_3x3 trans_mat3d = mat3d.Transpose();

      Vector_3 a_trans_t = trans_mat3d * a_vec;
      Vector_3 b_trans_t = trans_mat3d * b_vec;
      Vector_3 c_trans_t = trans_mat3d * c_vec;

      // Calculate G6 representations for each approach
      G6 transformedCell1, transformedCell2, transformedCell3;

      // Approach 1
      transformedCell1[0] = a_trans.SquaredLength();
      transformedCell1[1] = b_trans.SquaredLength();
      transformedCell1[2] = c_trans.SquaredLength();
      transformedCell1[3] = 2.0 * b_trans.Dot(c_trans);
      transformedCell1[4] = 2.0 * a_trans.Dot(c_trans);
      transformedCell1[5] = 2.0 * a_trans.Dot(b_trans);

      // Approach 2
      transformedCell2[0] = a_inv.SquaredLength();
      transformedCell2[1] = b_inv.SquaredLength();
      transformedCell2[2] = c_inv.SquaredLength();
      transformedCell2[3] = 2.0 * b_inv.Dot(c_inv);
      transformedCell2[4] = 2.0 * a_inv.Dot(c_inv);
      transformedCell2[5] = 2.0 * a_inv.Dot(b_inv);

      // Approach 3
      transformedCell3[0] = a_trans_t.SquaredLength();
      transformedCell3[1] = b_trans_t.SquaredLength();
      transformedCell3[2] = c_trans_t.SquaredLength();
      transformedCell3[3] = 2.0 * b_trans_t.Dot(c_trans_t);
      transformedCell3[4] = 2.0 * a_trans_t.Dot(c_trans_t);
      transformedCell3[5] = 2.0 * a_trans_t.Dot(b_trans_t);

      // Compare the transformed cells with the reduced cell
      const double diff1 = (transformedCell1 - reducedCell).norm();
      const double diff2 = (transformedCell2 - reducedCell).norm();
      const double diff3 = (transformedCell3 - reducedCell).norm();

      // Define a tolerance for the comparison
      const double tolerance = 1.0E-6;

      bool anyMatched = false;
      int bestApproach = 0;
      double bestDiff = DBL_MAX;

      if (diff1 < tolerance) {
         anyMatched = true;
         bestApproach = 1;
         bestDiff = diff1;
      }

      if (diff2 < tolerance && diff2 < bestDiff) {
         anyMatched = true;
         bestApproach = 2;
         bestDiff = diff2;
      }

      if (diff3 < tolerance && diff3 < bestDiff) {
         anyMatched = true;
         bestApproach = 3;
         bestDiff = diff3;
      }

      if (anyMatched) {
         ++successCount;
         transformStats.Store(1.0, "TransformationCorrect_Approach" + std::to_string(bestApproach));
      }
      else {
         ++mismatchCount;
         transformStats.Store(0.0, "TransformationIncorrect");

         // Log detailed information about the mismatch for analysis
         if (mismatchCount <= 10) {
            logFile << "Mismatch #" << mismatchCount << ":\n";
            logFile << "Input Cell (G6): " << inputCell << "\n";
            logFile << "Input Cell (LRL): " << inputCell_LRL << "\n";
            logFile << "Reduced Cell (from algorithm): " << reducedCell << "\n";
            logFile << "Approach 1 Cell: " << transformedCell1 << " (diff: " << diff1 << ")\n";
            logFile << "Approach 2 Cell: " << transformedCell2 << " (diff: " << diff2 << ")\n";
            logFile << "Approach 3 Cell: " << transformedCell3 << " (diff: " << diff3 << ")\n";
            logFile << "3D Transformation Matrix:\n" << mat3d << "\n";
            logFile << "Determinant: " << mat3d.Det() << "\n\n";

            // Print the MatG6 from Niggli::Reduce for comparison
            logFile << "MatG6 from Niggli::Reduce:\n" << matG6 << "\n\n";
         }
      }

      // Progress indicator
      if ((test + 1) % 100 == 0) {
         std::cout << "Completed " << (test + 1) << " tests. ";
         std::cout << "Correct Transformations: " << successCount
            << ", Incorrect: " << mismatchCount
            << ", Failures: " << failureCount << std::endl;
      }
   }

   // Output summary statistics
   logFile << "=============== Summary ===============\n";
   logFile << "Total test cases: " << numTests << "\n";
   logFile << "Correct Transformations: " << successCount << " / " << numTests
      << " (" << (static_cast<double>(successCount) / numTests * 100.0) << "%)\n";
   logFile << "Incorrect Transformations: " << mismatchCount << " / " << numTests
      << " (" << (static_cast<double>(mismatchCount) / numTests * 100.0) << "%)\n";
   logFile << "Reduction Failures: " << failureCount << " / " << numTests
      << " (" << (static_cast<double>(failureCount) / numTests * 100.0) << "%)\n\n";

   // Show detailed statistics
   logFile << "Detailed Statistics:\n";
   transformStats.ShowResults();

   // Output to console as well
   std::cout << "Validation completed. See 'niggli_transform_matrix_validation.log' for details.\n";
   std::cout << "Total test cases: " << numTests << "\n";
   std::cout << "Correct Transformation Rate: " << (static_cast<double>(successCount) / numTests * 100.0) << "%\n";

   logFile.close();
}




int main() {
   // Initialize global StoreResults
   g_bestVectors.SetTitle("Failed Cell Examples");

   // Run the benchmark
   BenchmarkNiggliReduction(1000);

   // Run our new test to validate the 3D transformation matrix
   std::cout << "\nTesting the 3D transformation matrix from ReduceWithTransforms...\n";
   TestReduceTransformationMatrix(2000);

   // Output results to console
   std::cout << "\nExamples of Failed Cells:\n";
   // Show a few examples of cells that failed reduction
   g_bestVectors.ShowResults();
   niggliReduceSuccess.SetTitle("counts of Niggli reduction success");
   niggliReduceSuccess.ShowResults();
   return 0;
}

