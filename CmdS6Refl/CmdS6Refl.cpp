
#include <iostream>

#include "LRL_ReadLatticeData.h"

std::vector<S6> EliminateDuplicates(const std::vector<S6>& vs) {
   std::vector<S6> out(1, vs[0]);
   for (size_t i = 1; i < vs.size(); ++i) {
      std::vector<S6> toAdd(1, vs[i]);
      for (size_t kkk = 0; kkk < out.size(); ++kkk)
      {
         const double d = (vs[i] - out[kkk]).norm();
         if (d < 1.0E-6) {
            toAdd.clear();
            break;
         }
      }
      out.insert(out.end(), toAdd.begin(), toAdd.end());
   }
   return out;
}

int main()
{
   static const std::vector<MatS6> refl_one = MatS6::GetReflections();
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::cout << "; S6 reflections" << std::endl;
   if (inputList.empty()) exit(2);

   std::vector<S6> reflected;
   for (size_t i = 0; i < inputList.size(); ++i) {
      for (size_t kk = 0; kk < refl_one.size(); ++kk) {
         reflected.emplace_back(refl_one[kk] * S6(inputList[i].GetCell()));
      }
   }

   const std::vector<S6> noDups = EliminateDuplicates(reflected);

   for ( size_t i=0; i<noDups.size(); ++i )
   {
      std::cout << "S6 " << noDups[i] << "  " << i << std::endl;
   }

}
