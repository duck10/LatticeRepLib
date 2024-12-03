#ifndef PATHPOINT
#define PATHPOINT
#include <algorithm>
#include <vector>
#include "S6.h"
#include "LatticeCell.h"

using PathPoint = std::pair<LatticeCell, LatticeCell>;
using Path = std::vector<PathPoint>;

#endif  // PATHPOINT