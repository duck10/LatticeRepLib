
#include <iostream>
#include <vector>

#include "B4.h"
#include "D7.h"
#include "Delone.h"
#include "LRL_Cell_Degrees.h"
#include "MatG6.h"
#include "MatS6.h"
#include "Pattlv.h"
#include "Selling.h"
#include "StoreResults.h"
#include "LRL_Vector3.h"

bool greater(std::pair<double, Vector_3>& s1, const std::pair<double, Vector_3>& s2) { return s1.first<s2.first; }

bool Pattlv::ReduceS6ToNiggli(const S6& si, G6& g6out) {
   S6 s6out;
   const bool b = Selling::Reduce(si, s6out);
   if (!b) return false;

   B4 b4(s6out);
   std::vector<std::pair<double, Vector_3> > vp;
   for (unsigned long i = 0; i < 4; ++i) vp.push_back(std::make_pair(b4[i].Norm(), b4[i]));
   std::sort(vp.begin(), vp.end(), greater);

   vp.resize(6);

   Vector_3 vtemp = vp[0].second + vp[1].second;
   vp[3] = std::make_pair(vtemp.Norm(), vtemp);  //OVERWRITE THE LONGEST ONE

   vtemp = vp[0].second + vp[2].second;
   vp[4] = std::make_pair(vtemp.Norm(), vtemp);

   vtemp = vp[1].second + vp[2].second;
   vp[5] = std::make_pair(vtemp.Norm(), vtemp);

   std::sort(vp.begin(), vp.end(), greater);

   b4[0] = vp[0].second;
   b4[1] = vp[1].second;
   b4[2] = vp[2].second;
   b4[3] = -b4[0]-b4[1]-b4[2];

   //std::cout << b4 << std::endl;

   s6out = S6(b4);
   g6out = S6(b4);
   return s6out.IsValid();
}
