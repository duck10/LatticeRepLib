#include "LatticeCentering.h"

#include "MatG6.h"

const std::vector<std::pair<char, MatS6> > m_ToPrimitive = LatticeCentering::MakeUncenterMatrices();;
const std::vector<std::pair<char, MatS6> > m_ToCentered = MakeCenteringMatrices();

std::vector<std::pair<char, MatS6> > LatticeCentering::MakeUncenterMatrices() {
   m_ToPrimitive.push_back(std::make_pair('P', MatG6::Eye()));
   m_ToPrimitive.push_back(std::make_pair('I', MatG6("1 0 0 0 0 0    0 1 0 0 0 0    .25 .25 .25 .25 .25 .25   0 1 0 .5 0 .5   1 0 0 0 .5 .5   0 0 0 0 0 1"))); // for monoclinic, assumes b unique
   m_ToPrimitive.push_back(std::make_pair('A', MatG6("1 0 0 0 0 0   0 1 0 0 0 0   0 .25 .25 .25 0 0    0 1 0 .5 0 0   0 0 0 0 .5 .5   0 0 0 0 0 1"))); // for monoclinic, assumes b unique
   m_ToPrimitive.push_back(std::make_pair('B', MatG6("1 0 0 0 0 0   0 1 0 0 0 0   .25 0 .25 0 .25 0   0 0 0 .5 0 .5   1 0 0 0 .5 0   0 0 0 0 0 1"))); // for monoclinic, assumes c unique
   m_ToPrimitive.push_back(std::make_pair('C', MatG6("1 0 0 0 0 0   .25 .25 0 0 0 .25   0 0 1 0 0 0    0 0 0 .5 .5 0   0 0 0 0 1 0   1 0 0 0 0 .5"))); // for monoclinic, assumes b unique
   m_ToPrimitive.push_back(std::make_pair('F', MatG6(".25 .25 0 0 0 .25     .25 0 .25 0 .25 0     0 .25 .25 .25  0 0    0 0 .5 .25 .25 .25     0 .5 0 .25 .25 .25     .5 0 0 .25 .25 .25")));
   m_ToPrimitive.push_back(std::make_pair('R', (1.0 / 9.0)* MatG6("1 1 1 1 -1 -1    4 1 1  1  2  2     1  4  1  -2  -1  2     -4  -4  2  -1  1  -5     2  -4  2  -1  -2  1     -4  2  2  2  1  1")));
   m_ToPrimitive.push_back(std::make_pair('H', MatG6::Eye()));
}

static std::vector<std::pair<char, MatS6> > MakeCenteringMatrices() {
   for ( unsigned long i=0; i< m_ToPrimitive.size(); ++i ) {
   
   }
}


LatticeCentering::LatticeCentering() {
   //if (toupper(latsym[0]) == 'P') return MatG6::Eye();
   //else if (toupper(latsym[0]) == 'I') return MatG6("1 0 0 0 0 0    0 1 0 0 0 0    .25 .25 .25 .25 .25 .25   0 1 0 .5 0 .5   1 0 0 0 .5 .5   0 0 0 0 0 1"); // for monoclinic, assumes b unique
   //else if (toupper(latsym[0]) == 'A') return MatG6("1 0 0 0 0 0   0 1 0 0 0 0   0 .25 .25 .25 0 0    0 1 0 .5 0 0   0 0 0 0 .5 .5   0 0 0 0 0 1"); // for monoclinic, assumes b unique
   //else if (toupper(latsym[0]) == 'B') return MatG6("1 0 0 0 0 0   0 1 0 0 0 0   .25 0 .25 0 .25 0   0 0 0 .5 0 .5   1 0 0 0 .5 0   0 0 0 0 0 1"); // for monoclinic, assumes c unique
   //else if (toupper(latsym[0]) == 'C') return MatG6("1 0 0 0 0 0   .25 .25 0 0 0 .25   0 0 1 0 0 0    0 0 0 .5 .5 0   0 0 0 0 1 0   1 0 0 0 0 .5"); // for monoclinic, assumes b unique
   //else if (toupper(latsym[0]) == 'F') return MatG6(".25 .25 0 0 0 .25     .25 0 .25 0 .25 0     0 .25 .25 .25  0 0    0 0 .5 .25 .25 .25     0 .5 0 .25 .25 .25     .5 0 0 .25 .25 .25");
   //else if (toupper(latsym[0]) == 'R') return (1.0 / 9.0)* MatG6("1 1 1 1 -1 -1    4 1 1  1  2  2     1  4  1  -2  -1  2     -4  -4  2  -1  1  -5     2  -4  2  -1  -2  1     -4  2  2  2  1  1");
   //else if (toupper(latsym[0]) == 'H') return  MatG6::Eye();
   //else return MatG6::Eye();
   //else return MatG6::Eye();
}

std::vector<std::pair<char, MatS6> > LatticeCentering::GetMakePrimitiveMatrices() {
   if (m_ToPrimitive.empty()) throw;
   return m_ToPrimitive;
}

std::vector<std::pair<char, MatS6> > LatticeCentering::GetMakeCenteredMatrices() {
   if (m_ToCentered.empty()) throw;
   return m_ToCentered;
}
