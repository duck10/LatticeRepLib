#include "P3TestGenerator.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <cmath>

#ifndef M_PI
static const double PI = 4.0F * ::atan(1.0F);
static const double M_PI = 4.0F * ::atan(1.0F);

#endif

std::vector<P3TestGenerator::TestCase> P3TestGenerator::GenerateTestCases(size_t count) {
   std::vector<TestCase> allCases;

   // Distribute test cases across different types
   size_t perType = count / 7;  // 7 different types
   size_t remainder = count % 7;

   auto randomCases = GenerateRandomCases(perType + (remainder > 0 ? 1 : 0));
   auto orthogonalCases = GenerateNearlyOrthogonalCases(perType + (remainder > 1 ? 1 : 0));
   auto parallelCases = GenerateNearlyParallelCases(perType + (remainder > 2 ? 1 : 0));
   auto generalCases = GenerateGeneralCases(perType + (remainder > 3 ? 1 : 0));
   auto highCostCases = GenerateHighCostCases(perType + (remainder > 4 ? 1 : 0));
   auto lowCostCases = GenerateLowCostCases(perType + (remainder > 5 ? 1 : 0));
   auto extremeCases = GenerateExtremeRatioCases(perType);

   // Combine all cases
   allCases.insert(allCases.end(), randomCases.begin(), randomCases.end());
   allCases.insert(allCases.end(), orthogonalCases.begin(), orthogonalCases.end());
   allCases.insert(allCases.end(), parallelCases.begin(), parallelCases.end());
   allCases.insert(allCases.end(), generalCases.begin(), generalCases.end());
   allCases.insert(allCases.end(), highCostCases.begin(), highCostCases.end());
   allCases.insert(allCases.end(), lowCostCases.begin(), lowCostCases.end());
   allCases.insert(allCases.end(), extremeCases.begin(), extremeCases.end());

   return allCases;
}

std::vector<P3TestGenerator::TestCase> P3TestGenerator::GenerateRandomCases(size_t count) {
   std::vector<TestCase> cases;

   for (size_t i = 0; i < count; ++i) {
      TestCase testCase;
      testCase.p3 = CreateRandomP3();
      testCase.type = TestCaseType::RANDOM;
      testCase.description = "Random P3 using Polar::rand()";
      testCase.originalCost = CalculateP3Cost(testCase.p3);
      cases.push_back(testCase);
   }

   return cases;
}

std::vector<P3TestGenerator::TestCase> P3TestGenerator::GenerateNearlyOrthogonalCases(size_t count) {
   std::vector<TestCase> cases;

   for (size_t i = 0; i < count; ++i) {
      TestCase testCase;
      testCase.p3 = CreateNearlyOrthogonalP3();
      testCase.type = TestCaseType::NEARLY_ORTHOGONAL;
      testCase.description = "Nearly orthogonal vectors";
      testCase.originalCost = CalculateP3Cost(testCase.p3);
      cases.push_back(testCase);
   }

   return cases;
}

std::vector<P3TestGenerator::TestCase> P3TestGenerator::GenerateNearlyParallelCases(size_t count) {
   std::vector<TestCase> cases;

   for (size_t i = 0; i < count; ++i) {
      TestCase testCase;
      testCase.p3 = CreateNearlyParallelP3();
      testCase.type = TestCaseType::NEARLY_PARALLEL;
      testCase.description = "Nearly parallel vectors";
      testCase.originalCost = CalculateP3Cost(testCase.p3);
      cases.push_back(testCase);
   }

   return cases;
}

std::vector<P3TestGenerator::TestCase> P3TestGenerator::GenerateGeneralCases(size_t count) {
   std::vector<TestCase> cases;

   // Pre-defined general cases
   std::vector<std::tuple<double, double, double, double, double, double, std::string>> generalConfigs = {
       {1.0, 0.5, 0.8, 0.6, 0.3, 0.9, "Mixed moderate values"},
       {1.0, 0.0, 0.0, 1.0, 0.5, 0.5, "Some orthogonal components"},
       {2.0, 0.1, 1.5, 0.2, 1.8, 0.15, "Large magnitudes, small angles"},
       {0.5, 1.2, 0.6, 1.1, 0.7, 1.3, "Small magnitudes, large angles"},
       {1.0, 0.785, 1.0, 1.57, 1.0, 2.356, "Equal magnitudes, π/4 steps"}
   };

   size_t configCount = 0;
   for (size_t i = 0; i < count; ++i) {
      TestCase testCase;

      if (configCount < generalConfigs.size()) {
         auto [x1, y1, x2, y2, x3, y3, desc] = generalConfigs[configCount];
         testCase.p3 = CreateGeneralP3(x1, y1, x2, y2, x3, y3);
         testCase.description = desc;
         configCount++;
      }
      else {
         // Generate additional random general cases
         double x1 = RandomDouble(0.3, 2.0);
         double y1 = RandomDouble(0.1, 1.5);
         double x2 = RandomDouble(0.3, 2.0);
         double y2 = RandomDouble(0.1, 1.5);
         double x3 = RandomDouble(0.3, 2.0);
         double y3 = RandomDouble(0.1, 1.5);

         testCase.p3 = CreateGeneralP3(x1, y1, x2, y2, x3, y3);
         testCase.description = "Random general case";
      }

      testCase.type = TestCaseType::GENERAL_CASE;
      testCase.originalCost = CalculateP3Cost(testCase.p3);
      cases.push_back(testCase);
   }

   return cases;
}

std::vector<P3TestGenerator::TestCase> P3TestGenerator::GenerateHighCostCases(size_t count) {
   std::vector<TestCase> cases;

   for (size_t i = 0; i < count; ++i) {
      TestCase testCase;
      testCase.p3 = CreateHighCostP3();
      testCase.type = TestCaseType::HIGH_COST;
      testCase.description = "High cost configuration";
      testCase.originalCost = CalculateP3Cost(testCase.p3);
      cases.push_back(testCase);
   }

   return cases;
}

std::vector<P3TestGenerator::TestCase> P3TestGenerator::GenerateLowCostCases(size_t count) {
   std::vector<TestCase> cases;

   for (size_t i = 0; i < count; ++i) {
      TestCase testCase;
      testCase.p3 = CreateLowCostP3();
      testCase.type = TestCaseType::LOW_COST;
      testCase.description = "Low cost configuration";
      testCase.originalCost = CalculateP3Cost(testCase.p3);
      cases.push_back(testCase);
   }

   return cases;
}

std::vector<P3TestGenerator::TestCase> P3TestGenerator::GenerateExtremeRatioCases(size_t count) {
   std::vector<TestCase> cases;

   for (size_t i = 0; i < count; ++i) {
      TestCase testCase;
      testCase.p3 = CreateExtremeRatioP3();
      testCase.type = TestCaseType::EXTREME_RATIOS;
      testCase.description = "Extreme magnitude ratios";
      testCase.originalCost = CalculateP3Cost(testCase.p3);
      cases.push_back(testCase);
   }

   return cases;
}

P3 P3TestGenerator::CreateRandomP3() {
   // Use Polar::rand() as in the original code
   return P3(Polar::rand());
}

P3 P3TestGenerator::CreateNearlyOrthogonalP3() {
   // Create vectors that are approximately orthogonal
   double r1 = RandomDouble(0.5, 2.0);
   double r2 = RandomDouble(0.5, 2.0);
   double r3 = RandomDouble(0.5, 2.0);

   // Angles approximately π/2 apart with small perturbations
   double theta1 = RandomDouble(-0.1, 0.1);  // Near 0
   double theta2 = M_PI / 2 + RandomDouble(-0.1, 0.1);  // Near π/2
   double theta3 = M_PI + RandomDouble(-0.1, 0.1);  // Near π

   return CreateP3FromPolar(r1, theta1, r2, theta2, r3, theta3);
}

P3 P3TestGenerator::CreateNearlyParallelP3() {
   // Create vectors that are nearly parallel (small angle differences)
   double r1 = RandomDouble(0.5, 2.0);
   double r2 = RandomDouble(0.5, 2.0);
   double r3 = RandomDouble(0.5, 2.0);

   double baseAngle = RandomAngle();
   double theta1 = baseAngle + RandomDouble(-0.1, 0.1);
   double theta2 = baseAngle + RandomDouble(-0.1, 0.1);
   double theta3 = baseAngle + RandomDouble(-0.1, 0.1);

   return CreateP3FromPolar(r1, theta1, r2, theta2, r3, theta3);
}

P3 P3TestGenerator::CreateGeneralP3(double x1, double y1, double x2, double y2, double x3, double y3) {
   P3 result;
   result[0] = std::make_pair(x1, y1);
   result[1] = std::make_pair(x2, y2);
   result[2] = std::make_pair(x3, y3);
   return result;
}

P3 P3TestGenerator::CreateHighCostP3() {
   // Create configurations likely to have high P3 cost
   // Large, nearly parallel vectors
   double r1 = RandomDouble(2.0, 5.0);  // Large magnitudes
   double r2 = RandomDouble(2.0, 5.0);
   double r3 = RandomDouble(2.0, 5.0);

   double baseAngle = RandomAngle();
   double theta1 = baseAngle;
   double theta2 = baseAngle + RandomDouble(-0.05, 0.05);  // Very small differences
   double theta3 = baseAngle + RandomDouble(-0.05, 0.05);

   return CreateP3FromPolar(r1, theta1, r2, theta2, r3, theta3);
}

P3 P3TestGenerator::CreateLowCostP3() {
   // Create configurations likely to have low P3 cost
   // Small, well-separated vectors
   double r1 = RandomDouble(0.1, 1.0);  // Small magnitudes
   double r2 = RandomDouble(0.1, 1.0);
   double r3 = RandomDouble(0.1, 1.0);

   // Well-separated angles
   double theta1 = 0;
   double theta2 = 2 * M_PI / 3;
   double theta3 = 4 * M_PI / 3;

   return CreateP3FromPolar(r1, theta1, r2, theta2, r3, theta3);
}

P3 P3TestGenerator::CreateExtremeRatioP3() {
   // Create vectors with extreme magnitude ratios
   double r1 = RandomDouble(0.01, 0.1);   // Very small
   double r2 = RandomDouble(1.0, 2.0);    // Medium
   double r3 = RandomDouble(5.0, 10.0);   // Very large

   double theta1 = RandomAngle();
   double theta2 = RandomAngle();
   double theta3 = RandomAngle();

   return CreateP3FromPolar(r1, theta1, r2, theta2, r3, theta3);
}

double P3TestGenerator::CalculateP3Cost(const P3& p3) {
   return P3_Reduce::P3Cost(p3);
}

std::string P3TestGenerator::DescribeP3(const P3& p3) {
   std::ostringstream oss;
   oss << std::fixed << std::setprecision(3);
   oss << "P3[(" << p3[0].first << "," << p3[0].second << "), "
      << "(" << p3[1].first << "," << p3[1].second << "), "
      << "(" << p3[2].first << "," << p3[2].second << ")]";
   return oss.str();
}

bool P3TestGenerator::IsValidP3(const P3& p3) {
   for (int i = 0; i < 3; ++i) {
      double magnitude = std::sqrt(p3[i].first * p3[i].first + p3[i].second * p3[i].second);
      if (magnitude < 1e-10) return false;  // Zero vector
      if (!std::isfinite(p3[i].first) || !std::isfinite(p3[i].second)) return false;  // Invalid values
   }
   return true;
}

double P3TestGenerator::RandomDouble(double min, double max) {
   static std::random_device rd;
   static std::mt19937 gen(rd());
   std::uniform_real_distribution<double> dis(min, max);
   return dis(gen);
}

double P3TestGenerator::RandomAngle() {
   return RandomDouble(0, 2 * M_PI);
}

P3 P3TestGenerator::CreateP3FromPolar(double r1, double theta1, double r2, double theta2, double r3, double theta3) {
   P3 result;
   result[0] = std::make_pair(r1 * std::cos(theta1), r1 * std::sin(theta1));
   result[1] = std::make_pair(r2 * std::cos(theta2), r2 * std::sin(theta2));
   result[2] = std::make_pair(r3 * std::cos(theta3), r3 * std::sin(theta3));
   return result;
}
