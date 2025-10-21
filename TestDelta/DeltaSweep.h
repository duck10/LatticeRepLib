#pragma once
#include <string>
#include <vector>
#include <functional>
#include "G6.h"

struct DeltaSweepResult {
   std::string trigger;
   double delta;
   bool fired;
   double lhs;
   double rhs;
   std::string reason;
};

std::vector<DeltaSweepResult> SweepTrigger(
   const std::string& name,
   const G6& vin,
   std::function<std::pair<bool, std::string>(const G6&, double)> triggerFunc,
   std::function<std::pair<double, double>(const G6&, double)> inequalityFunc,
   const std::vector<double>& deltas);

void PrintResults(const std::vector<DeltaSweepResult>& results);
