#ifndef P3TESTGENERATOR_H
#define P3TESTGENERATOR_H

#include "P3.h"
#include "Polar.h"
#include "P3_Reduce.h"
#include <vector>
#include <string>

class P3TestGenerator {
public:
   enum class TestCaseType {
      RANDOM,
      NEARLY_ORTHOGONAL,
      NEARLY_PARALLEL,
      GENERAL_CASE,
      HIGH_COST,
      LOW_COST,
      EXTREME_RATIOS
   };

   struct TestCase {
      P3 p3;
      TestCaseType type;
      std::string description;
      double originalCost;
   };

   // Generate multiple test cases of various types
   static std::vector<TestCase> GenerateTestCases(size_t count = 20);

   // Generate specific types of test cases
   static std::vector<TestCase> GenerateRandomCases(size_t count);
   static std::vector<TestCase> GenerateNearlyOrthogonalCases(size_t count);
   static std::vector<TestCase> GenerateNearlyParallelCases(size_t count);
   static std::vector<TestCase> GenerateGeneralCases(size_t count);
   static std::vector<TestCase> GenerateHighCostCases(size_t count);
   static std::vector<TestCase> GenerateLowCostCases(size_t count);
   static std::vector<TestCase> GenerateExtremeRatioCases(size_t count);

   // Create specific P3 configurations
   static P3 CreateRandomP3();
   static P3 CreateNearlyOrthogonalP3();
   static P3 CreateNearlyParallelP3();
   static P3 CreateGeneralP3(double x1, double y1, double x2, double y2, double x3, double y3);
   static P3 CreateHighCostP3();
   static P3 CreateLowCostP3();
   static P3 CreateExtremeRatioP3();

   // Utility functions
   static double CalculateP3Cost(const P3& p3);
   static std::string DescribeP3(const P3& p3);
   static bool IsValidP3(const P3& p3);

private:
   static double RandomDouble(double min = 0.1, double max = 2.0);
   static double RandomAngle();
   static P3 CreateP3FromPolar(double r1, double theta1, double r2, double theta2, double r3, double theta3);
};

#endif // P3TESTGENERATOR_H
