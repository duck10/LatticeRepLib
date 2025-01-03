#ifndef GLITCH_TYPES_H
#define GLITCH_TYPES_H

#include <cstddef>

struct Glitch {
   size_t index;
   double value;
   double changePercent;
   Glitch(size_t idx, double val, double chg) : index(idx), value(val), changePercent(chg) {}
};

#endif // GLITCH_TYPES_H