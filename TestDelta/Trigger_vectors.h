#pragma once
#include <vector>
#include <functional>
#include "G6.h"
#include "DeltaSweep.h"

// Declare sweep range
extern const std::vector<double> deltas;

// Declare boundary vectors and inequality extractors
extern const G6 sp2Vec;
extern const G6 r5Vec;
extern const G6 r6Vec;
extern const G6 r7Vec;
extern const G6 r8Vec;
extern const G6 r9Vec;
extern const G6 r10Vec;
extern const G6 r11Vec;
extern const G6 r12Vec;

extern const std::function<std::pair<double, double>(const G6&, double)> sp2Ineq;
extern const std::function<std::pair<double, double>(const G6&, double)> r5Ineq;
extern const std::function<std::pair<double, double>(const G6&, double)> r6Ineq;
extern const std::function<std::pair<double, double>(const G6&, double)> r7Ineq;
extern const std::function<std::pair<double, double>(const G6&, double)> r8Ineq;
extern const std::function<std::pair<double, double>(const G6&, double)> r9Ineq;
extern const std::function<std::pair<double, double>(const G6&, double)> r10Ineq;
extern const std::function<std::pair<double, double>(const G6&, double)> r11Ineq;
extern const std::function<std::pair<double, double>(const G6&, double)> r12Ineq;

// Optional: declare a runner
void RunAllSweeps();

