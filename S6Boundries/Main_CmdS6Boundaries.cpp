
#include <iostream>

#include "LRL_ReadLatticeData.h"
#include "MatS6.h"
#include "S6BoundaryTransforms.h"

std::vector<S6> EliminateDuplicates(const std::vector<S6>& vs) {
   std::vector<S6> out(1, vs[0]);
   for (const auto& vs_1 : vs) {
      std::vector<S6> toAdd(1, vs_1);
      for (const auto& out_1 : out)
      {
         const double d = (vs_1 - out_1).norm();
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
   std::cout << "; S6 boundaries" << std::endl;
   if (inputList.empty()) exit(2);

   S6BoundaryTransforms sbt;
   const std::vector<MatS6> v = sbt.GetBoundaries();

   std::vector<S6> fromBound;
   for (auto& input : inputList) {
      for (auto& refl : refl_one) {
         fromBound.emplace_back(refl * S6(input.GetCell()));
      }
   }

   const std::vector<S6> noDups = EliminateDuplicates(fromBound);

   for (size_t i = 0; i < noDups.size(); ++i)
   {
      std::cout << "S6 " << noDups[i] << "  " << i << std::endl;
   }

}
