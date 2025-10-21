#include <iostream>
#include <iomanip>

#include "TEST_DELTAControls.h"
#include "ProgramSetup.h"

#include "TestDelta.h"
#include "DeltaSweep.h"
#include "LRL_Cell_Degrees.h"
#include "Polar.h"
#include "Trigger_vectors.h"

void TestDelta::RunTriggerDiagnostics() {
   const std::vector<double> deltas = { -1e-3, 0.0, 1e-3 };

   struct TriggerTest {
      std::string name;
      G6 vin;
      std::pair<bool, std::string>(*trigger)(const G6&, double);
   };

   std::vector<TriggerTest> tests = {
      {"SP1",  {10.0001, 10.0, 10.0, 10.0, 10.0, 10.0}, TriggerSP1},
      {"SP2",  {10.0, 10.0001, 10.0, 10.0, 10.0001, 10.0}, TriggerSP2},
      {"R5",   {10.0, 10.0, 10.0, 11.0, 10.0, 10.0}, TriggerR5},
      {"R6",   {10.0, 10.0, 10.0, 10.0, 11.0, 10.0}, TriggerR6},
      {"R7",   {10.0, 10.0, 10.0, 10.0, 10.0, 11.0}, TriggerR7},
      {"R8",   {10.0, 10.0, 10.0, -21.0, -21.0, -21.0}, TriggerR8},
      {"R9",   {10.0, 10.0, 10.0, 10.0, 4.9, 10.0}, TriggerR9},
      {"R10",  {10.0, 10.0, 10.0, 4.9, 10.0, 10.0}, TriggerR10},
      {"R11",  {10.0, 10.0, 10.0, 4.9, 10.0, 10.0}, TriggerR11},
      {"R12",  {10.0, 10.0, 10.0, -10.0, -10.0, -10.0}, TriggerR12}
   };

   std::cout << "; TRIGGER_DIAGNOSTICS\n";

   for (const auto& test : tests) {
      std::cout << "\nTrigger: " << test.name << "\n";
      std::cout << "Input G6: ";
      for (int i = 0; i < 6; ++i)
         std::cout << std::fixed << std::setprecision(6) << test.vin[i] << " ";
      std::cout << "\n";
      std::cout << "Delta         | Result | Reason\n";
      std::cout << "--------------|--------|---------------------------------------------\n";

      for (double delta : deltas) {
         auto result = test.trigger(test.vin, delta);
         std::cout << std::setw(14) << std::scientific << delta << " |   "
            << (result.first ? "Y" : "-") << "    | " << result.second << "\n";
      }
   }
}



int main() {
   std::cout << "; TEST_DELTA" << std::endl;


   RunAllSweeps();

   std::vector<double> deltas = { -1e-12, 0.0, 1e-12, 1e-10, 1e-3 };

   G6 sp1Vec = { 10.000001, 10.000000, 10.0, 10.0, 10.0, 10.0 };
   auto sp1Ineq = [](const G6& v, double delta) {
      double lhs = fabs(v[0]);
      double rhs = fabs(v[1]) + delta + 1e-12 * (v[0] + v[1]);
      return std::make_pair(lhs, rhs);
      };
   auto results = SweepTrigger("SP1", sp1Vec, TestDelta::TriggerSP1, sp1Ineq, deltas);
   PrintResults(results);





   TestDelta::RunTriggerDiagnostics();

   try {
      TEST_DELTAControls controls;
      const BasicProgramInput<TEST_DELTAControls> dc_setup("TEST_DELTA", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         G6 testVin = input.getCell();

         std::cout << "input cell " << LRL_Cell_Degrees(input.getCell()) << std::endl;
         std::cout << "input cell " << LRL_Cell_Degrees(input.getCell()) << std::endl;
         //TestDelta tester;
         //tester.CompareDeltas();
         std::cout << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}
