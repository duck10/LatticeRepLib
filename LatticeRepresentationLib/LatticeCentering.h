#ifndef LATTICECENTERING_H
#define LATTICECENTERING_H

#include <algorithm>
#include <utility>
#include <vector>

#include "MatS6.h"

class LatticeCentering {
public:
LatticeCentering();

std::vector<std::pair<char, MatS6> > GetMakePrimitiveMatrices();
std::vector<std::pair<char, MatS6> > GetMakeCenteredMatrices();

static std::vector<std::pair<char, MatS6> > MakeUncenterMatrices();
static std::vector<std::pair<char, MatS6> > MakeCenteringMatrices();
protected:
   static std::vector<std::pair<char, MatS6> > m_ToPrimitive;
   static std::vector<std::pair<char, MatS6> > m_ToCentered;
};

#endif  // LATTICECENTERING_H

