#include "DeltaSweep.h"
#include <iostream>
#include <iomanip>
#include "TestDelta.h"

std::vector<DeltaSweepResult> SweepTrigger(
   const std::string& name,
   const G6& vin,
   std::function<std::pair<bool, std::string>(const G6&, double)> triggerFunc,
   std::function<std::pair<double, double>(const G6&, double)> inequalityFunc,
   const std::vector<double>& deltas)
{
   std::vector<DeltaSweepResult> results;
   for (double delta : deltas) {
      auto fired = triggerFunc(vin, delta);
      auto [lhs, rhs] = inequalityFunc(vin, delta);
      results.push_back({ name, delta, fired.first, lhs, rhs, fired.second });
   }
   return results;
}

void PrintResults(const std::vector<DeltaSweepResult>& results) {
   std::cout << "Trigger: " << results[0].trigger << "\n";
   std::cout << "Delta         | Fired | LHS        | RHS        | Reason\n";
   std::cout << "--------------|-------|------------|------------|-------------------------\n";
   for (const auto& r : results) {
      std::cout << std::setw(14) << std::scientific << r.delta << " | "
         << (r.fired ? "Y" : "-") << "     | "
         << std::fixed << std::setprecision(6) << r.lhs << " | "
         << r.rhs << " | " << r.reason << "\n";
   }
   std::cout << "\n";
}
