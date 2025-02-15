// scale_test.cpp - corrected version
#include "LinearAxis.h"
#include "WilkinsonScale.h"
#include <iostream>
#include <iomanip>
#include <vector>

struct TestCase {
   std::string name;
   double min;
   double max;
};

void printScale(const std::string& algorithm, const std::string& testName,
   double dataMin, double dataMax,  // actual data range
   double scaleMin, double scaleMax, // scale range
   double step, int numTicks) {
   const double dataRange = dataMax - dataMin;
   const double scaleRange = scaleMax - scaleMin;
   // How much of the scale range is actually used by the data
   const double utilization = (dataRange / scaleRange) * 100.0;

   std::cout << std::setw(15) << algorithm << " | "
      << std::setw(20) << testName << " | "
      << "min: " << std::setw(10) << std::scientific << std::setprecision(3) << scaleMin
      << " max: " << std::setw(10) << std::scientific << std::setprecision(3) << scaleMax
      << " step: " << std::setw(10) << std::scientific << std::setprecision(3) << step
      << " ticks: " << std::setw(3) << numTicks
      << " utilization: " << std::setw(6) << std::fixed << std::setprecision(1)
      << utilization << "%\n";
}

void compareScales() {
   std::vector<TestCase> testCases = {
       {"Normal Range", 0.0, 100.0},
       {"Small Near Zero", 0.001, 0.009},
       {"Large Range", 1000.0, 1000000.0},
       {"Awkward Range", 98.7, 372.1},
       {"Nearly Equal", 10.01, 10.02},
       {"Negative Range", -372.1, -98.7},
       {"Cross Zero", -100.0, 100.0},
       {"Very Small Range", 0.00001, 0.00002},
       {"Very Large Range", 1E6, 1E9},
       {"Zero Range", 100.0, 100.0}
   };

   std::cout << "Comparing LinearAxis and Wilkinson Scaling Algorithms\n"
      << "===================================================\n\n";

   for (const auto& test : testCases) {
      std::cout << "\nTest Case: " << test.name << "\n"
         << "----------------------------------------\n";

      // LinearAxis test
      LinearAxis linearAxis;
      auto linearLimits = linearAxis.LinearAxisLimits(test.min, test.max);

      // Wilkinson test
      WilkinsonScale wilkinson(5);  // 5 desired ticks
      auto wilkinsonScale = wilkinson.calculateScale(test.min, test.max);

      // Print results with data range and scale range for proper utilization calculation
      printScale("LinearAxis", test.name,
         test.min, test.max,
         linearLimits.GetMin(),
         linearLimits.GetMax(),
         linearLimits.GetStepSize(),
         linearLimits.m_numSteps);

      printScale("Wilkinson", test.name,
         test.min, test.max,
         wilkinsonScale.min,
         wilkinsonScale.max,
         wilkinsonScale.step,
         wilkinsonScale.numTicks);

      // Print tick values for both
      std::cout << "\nLinearAxis ticks: ";
      double linearStep = linearLimits.GetStepSize();
      for (int i = 0; i <= linearLimits.m_numSteps; ++i) {
         double value = linearLimits.GetMin() + i * linearStep;
         // For very small numbers, use scientific notation
         if (std::abs(value) < 0.0001 || std::abs(value) > 9999) {
            std::cout << std::scientific;
         }
         else {
            std::cout << std::fixed;
         }
         std::cout << std::setprecision(6) << value << " ";
      }
      std::cout << "\n";

      std::cout << "Wilkinson ticks: ";
      for (int i = 0; i < wilkinsonScale.numTicks; ++i) {
         double value = wilkinsonScale.min + i * wilkinsonScale.step;
         // For very small numbers, use scientific notation
         if (std::abs(value) < 0.0001 || std::abs(value) > 9999) {
            std::cout << std::scientific;
         }
         else {
            std::cout << std::fixed;
         }
         std::cout << std::setprecision(6) << value << " ";
      }
      std::cout << "\n\n";
   }
}

int main() {
   compareScales();
   return 0;
}