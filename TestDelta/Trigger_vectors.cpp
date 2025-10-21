#include "DeltaSweep.h"
#include "TestDelta.h"

// Sweep range
std::vector<double> deltas = { -1e-12, 0.0, 1e-12, 1e-10, 1e-3 };

G6 sp1Vec = { 10.000001, 10.000000, 10.0, 10.0, 10.0, 10.0 };
auto sp1Ineq = [](const G6& v, double delta) {
   double lhs = fabs(v[0]);
   double rhs = fabs(v[1]) + delta + 1e-12 * (v[0] + v[1]);
   return std::make_pair(lhs, rhs);
   };


// SP2: g2 ? g3
G6 sp2Vec = { 10.0, 10.000001, 10.000000, 10.0, 10.000100, 10.0 };
auto sp2Ineq = [](const G6& v, double delta) {
   double lhs = fabs(v[1]);
   double rhs = fabs(v[2]) + delta + 1e-12 * (v[1] + v[2]);
   return std::make_pair(lhs, rhs);
   };

// R5: g4 ? g2
G6 r5Vec = { 10.0, 10.000000, 10.0, 10.000001, 10.0, 10.0 };
auto r5Ineq = [](const G6& v, double delta) {
   double lhs = fabs(v[3]);
   double rhs = fabs(v[1]) + delta;
   return std::make_pair(lhs, rhs);
   };

// R6: g5 ? g1
G6 r6Vec = { 10.000000, 10.0, 10.0, 10.0, 10.000001, 10.0 };
auto r6Ineq = [](const G6& v, double delta) {
   double lhs = fabs(v[4]);
   double rhs = fabs(v[0]) + delta;
   return std::make_pair(lhs, rhs);
   };

// R7: g6 ? g1
G6 r7Vec = { 10.000000, 10.0, 10.0, 10.0, 10.0, 10.000001 };
auto r7Ineq = [](const G6& v, double delta) {
   double lhs = fabs(v[5]);
   double rhs = fabs(v[0]) + delta;
   return std::make_pair(lhs, rhs);
   };

// R8: sum ? 0
G6 r8Vec = { 10.0, 10.0, 10.0, -10.0, -10.0, -10.0 };
auto r8Ineq = [](const G6& v, double delta) {
   double lhs = v[3] + v[4] + v[5] + fabs(v[0]) + fabs(v[1]);
   double rhs = -delta;
   return std::make_pair(lhs, rhs);
   };

// R9: g4 ? ±g2
G6 r9Vec = { 10.0, 10.000000, 10.0, 10.000001, 10.0, 10.000000 };
auto r9Ineq = [](const G6& v, double delta) {
   double lhs = fabs(v[3] - v[1]);
   double rhs = delta;
   return std::make_pair(lhs, rhs);
   };

// R10: g5 ? ±g1
G6 r10Vec = { 10.000000, 10.0, 10.0, 10.0, 10.000001, 10.000000 };
auto r10Ineq = [](const G6& v, double delta) {
   double lhs = fabs(v[4] - v[0]);
   double rhs = delta;
   return std::make_pair(lhs, rhs);
   };

// R11: g6 ? ±g1
G6 r11Vec = { 10.000000, 10.0, 10.0, 10.000000, 10.0, 10.000001 };
auto r11Ineq = [](const G6& v, double delta) {
   double lhs = fabs(v[5] - v[0]);
   double rhs = delta;
   return std::make_pair(lhs, rhs);
   };

// R12: sum ? 0
G6 r12Vec = { 10.0, 10.0, 10.0, -10.0, -10.0, -10.0 };
auto r12Ineq = [](const G6& v, double delta) {
   double lhs = fabs(v[3] + v[4] + v[5] + fabs(v[0]) + fabs(v[1]));
   double rhs = delta;
   return std::make_pair(lhs, rhs);
   };

   // Run all sweeps
   void RunAllSweeps() {
   PrintResults(SweepTrigger("SP2", sp2Vec, TestDelta::TriggerSP2, sp2Ineq, deltas));
   PrintResults(SweepTrigger("R5", r5Vec, TestDelta::TriggerR5, r5Ineq, deltas));
   PrintResults(SweepTrigger("R6", r6Vec, TestDelta::TriggerR6, r6Ineq, deltas));
   PrintResults(SweepTrigger("R7", r7Vec, TestDelta::TriggerR7, r7Ineq, deltas));
   PrintResults(SweepTrigger("R8", r8Vec, TestDelta::TriggerR8, r8Ineq, deltas));
   PrintResults(SweepTrigger("R9", r9Vec, TestDelta::TriggerR9, r9Ineq, deltas));
   PrintResults(SweepTrigger("R10", r10Vec, TestDelta::TriggerR10, r10Ineq, deltas));
   PrintResults(SweepTrigger("R11", r11Vec, TestDelta::TriggerR11, r11Ineq, deltas));
   PrintResults(SweepTrigger("R12", r12Vec, TestDelta::TriggerR12, r12Ineq, deltas));
}
