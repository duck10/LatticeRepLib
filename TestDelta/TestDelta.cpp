#include "TestDelta.h"
#include <cmath>
#include <iostream>
#include <iomanip>

using std::pair;
using std::string;

pair<bool, string> TestDelta::TriggerSP1(const G6& vin, double delta) {
   double g1 = vin[0], g2 = vin[1], g4 = vin[3], g5 = vin[4];
   if (fabs(g1) > fabs(g2) + delta + 1.e-12 * (g1 + g2))
      return { true, "SP1: g1 dominates g2" };
   if (fabs(g1 - g2) < 1.e-38 + 1.e-12 * fabs(g1 + g2) &&
      delta < 1.0e-12 &&
      fabs(g4) > fabs(g5) + delta + 1.e-12 * (fabs(g4) + fabs(g5)))
      return { true, "SP1: g1 ? g2, g4 dominates g5" };
   return { false, "" };
}

pair<bool, string> TestDelta::TriggerSP2(const G6& vin, double delta) {
   double g2 = vin[1], g3 = vin[2], g5 = vin[4], g6 = vin[5];
   if (fabs(g2) > fabs(g3) + delta + 1.e-12 * (g2 + g3))
      return { true, "SP2: g2 dominates g3" };
   if (fabs(g2 - g3) < 1.e-38 + 1.e-12 * fabs(g2 + g3) &&
      delta < 1.0e-12 &&
      fabs(g5) > fabs(g6) + delta + 1.e-12 * (fabs(g5) + fabs(g6)))
      return { true, "SP2: g2 ? g3, g5 dominates g6" };
   return { false, "" };
}

pair<bool, string> TestDelta::TriggerR5(const G6& v, double delta) {
   if (fabs(v[3]) > fabs(v[1]) + delta)
      return { true, "R5: g4 dominates g2" };
   return { false, "" };
}

pair<bool, string> TestDelta::TriggerR6(const G6& v, double delta) {
   if (fabs(v[4]) > fabs(v[0]) + delta)
      return { true, "R6: g5 dominates g1" };
   return { false, "" };
}

pair<bool, string> TestDelta::TriggerR7(const G6& v, double delta) {
   if (fabs(v[5]) > fabs(v[0]) + delta)
      return { true, "R7: g6 dominates g1" };
   return { false, "" };
}

pair<bool, string> TestDelta::TriggerR8(const G6& v, double delta) {
   if (v[3] + v[4] + v[5] + fabs(v[0]) + fabs(v[1]) + delta < 0.0)
      return { true, "R8: sum of g4+g5+g6 + |g1| + |g2| < 0" };
   return { false, "" };
}

pair<bool, string> TestDelta::TriggerR9(const G6& v, double delta) {
   if ((fabs(v[3] - v[1]) <= delta && 2.0 * v[4] - delta < v[5]) ||
      (fabs(v[3] + v[1]) <= delta && v[5] < 0.0))
      return { true, "R9: g4 ? ±g2 and g6 violates inequality" };
   return { false, "" };
}

pair<bool, string> TestDelta::TriggerR10(const G6& v, double delta) {
   if ((fabs(v[4] - v[0]) <= delta && 2.0 * v[3] - delta < v[5]) ||
      (fabs(v[4] + v[0]) <= delta && v[5] < 0.0))
      return { true, "R10: g5 ? ±g1 and g6 violates inequality" };
   return { false, "" };
}

pair<bool, string> TestDelta::TriggerR11(const G6& v, double delta) {
   if ((fabs(v[5] - v[0]) <= delta && 2.0 * v[3] - delta < v[4]) ||
      (fabs(v[5] + v[0]) <= delta && v[4] < 0.0))
      return { true, "R11: g6 ? ±g1 and g5 violates inequality" };
   return { false, "" };
}

pair<bool, string> TestDelta::TriggerR12(const G6& v, double delta) {
   if (fabs(v[3] + v[4] + v[5] + v[0] + v[1]) <= delta &&
      (2.0 * (v[0] + v[4]) + v[5] > delta))
      return { true, "R12: near-zero sum with g6 > threshold" };
   return { false, "" };
}
