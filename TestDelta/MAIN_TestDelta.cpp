#include <iostream>
#include <iomanip>

#include "TEST_DELTAControls.h"
#include "ProgramSetup.h"

#include "TestDelta.h"
#include "DeltaSweep.h"
#include "LRL_Cell_Degrees.h"
#include "Polar.h"
#include "Trigger_vectors.h"


static const double delta = 1.0E-2;
static const double delta2 = delta / 2.0;
static const double x = 1.0 / 3.0;
static const double y = x;
static const double ym = y - delta / 2.0;
static const double xm = x - delta / 2.0;
static const double yp = y + delta / 2.0;
static const double xp = x + delta / 2.0;

std::pair<bool, bool> test_x_LT_y(double x, double ym, double yp) {
   const bool b1 = x < ym - delta;
   const bool b2 = x < yp - delta;
   return { b1,b2 };
}

std::pair<bool, bool> test_x_LE_y(double x, double xm, double xp) {
   const bool b1 = !(x > xm + delta);  // equivalent to !(y < x - delta)
   const bool b2 = !(x > xp + delta);
   return { b1,b2 };
}

//Grosse-Kunstleve,Sauter,Adams-2003
//exact
//   x < y   x < y - delta "
//   x > y   y < x - delta "
//   x <= y   not y < x - delta "
//   x >= y   not x < y - delta "
//   x = y    not  x < y - delta " or y < x - delta ":
// 

std::pair<bool,bool> test_x_LT_y() {    
   // test x<y
   const bool b1 = x < ym - delta;
   const bool b2 = x < yp - delta;
   return { b1,b2 };
}

static std::pair<bool,bool> test_x_LE_y () {    
   //   x <= y   not y < x - delta "
   const bool b5 = !(y <= xm - delta);
   const bool b6 = !(y <= xp - delta);
   return { b5,b6 };
}

std::pair<bool,bool> test_x_GT_y() {    
   // test x>y
   const bool b3 = y < xm - delta;
   const bool b4 = y < xp - delta;
   return { b3,b4 };
}

static std::pair<bool,bool> test_x_GE_y () {
   //   x >= y   not x < y - delta "
   const bool b7 = !(x <= ym - delta);
   const bool b8 = !(x <= yp - delta);
   return { b7,b8 };
}

static std::pair<bool,bool> test_x_EQ_y() {
   //   x = y    not  x < y - delta  or y < x - delta :
   const bool b9 = !(x < ym - delta || y < xm - delta);
   const bool bA = !(x < yp - delta || y < xp - delta);
   return { b9,bA };
}

#include <iostream>
#include <utility>
#include <string>

void summarize_tests() {
   auto lt = test_x_LT_y();
   //auto gt = test_x_GT_y();
   auto le = test_x_LE_y();
   //auto ge = test_x_GE_y();
   //auto eq = test_x_EQ_y();

   std::cout << "Test         | xm result | x2 result | Sensitivity to x==y\n";
   std::cout << "-------------|-----------|-----------|----------------------\n";

   std::cout << "x < y        | " << lt.first << "         | " << lt.second
      << "         | delta makes test stricter\n";

   //std::cout << "x > y        | " << gt.first << "         | " << gt.second
   //   << "         | delta makes test stricter\n";

   std::cout << "x <= y       | " << le.first << "         | " << le.second
      << "         | delta makes test looser\n";

   //std::cout << "x >= y       | " << ge.first << "         | " << ge.second
   //   << "         | delta makes test looser\n";

   //std::cout << "x == y       | " << eq.first << "         | " << eq.second
   //   << "         | delta makes test more tolerant\n";
}

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

   // Case 1: x < y
   double x1 = 1.00;
   double ym1 = 1.02;
   double yp1 = 1.03;
   double xm1 = 1.02;
   double xp1 = 1.03;

   auto lt1 = test_x_LT_y(x1, ym1, yp1);
   auto le1 = test_x_LE_y(x1, xm1, xp1);

   std::cout << "Case x < y:\n";
   std::cout << "x < y test:     " << lt1.first << " (ym)  " << lt1.second << " (yp)\n";
   std::cout << "x <= y test:    " << le1.first << " (xm)  " << le1.second << " (xp)\n";

   // Case 2: x > y
   double x2 = 1.05;
   double ym2 = 1.02;
   double yp2 = 1.03;
   double xm2 = 1.02;
   double xp2 = 1.03;

   auto lt2 = test_x_LT_y(x2, ym2, yp2);
   auto le2 = test_x_LE_y(x2, xm2, xp2);

   std::cout << "\nCase x > y:\n";
   std::cout << "x < y test:     " << lt2.first << " (ym)  " << lt2.second << " (yp)\n";
   std::cout << "x <= y test:    " << le2.first << " (xm)  " << le2.second << " (xp)\n";

   test_x_LT_y();
   summarize_tests();
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
