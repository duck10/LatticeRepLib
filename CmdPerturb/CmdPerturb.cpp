#include <iostream>

//#include "BasisBase.h" // this has CreateUnitOrthogonalVector
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "NCDist.h"
#include "rhrand.h"
#include "S6.h"

const double delta = 1.0;
const size_t ngen = 20;

const std::string latticeNames = "pifrhcPIFRHC";
RHrand ran;


template<typename T>
T CreateTotallyRandomUnitOrthogonalComponent(const T& t) {
   // assume t is not null and rand is not parallel to t
   const T base = t / t.norm();
   S6 temp;
   for (size_t i = 0; i < 6; ++i) temp[i] = ran.urand() - 0.5;
   temp /= temp.norm();
   const double d = dot(VecN(base.GetVector()), VecN(T(temp).GetVector()));
   T ortho = temp - d * base;
   ortho.SetValid(true);
   return ortho / ortho.norm();
}


int main()
{
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::cout << "; Perturb vectors, input scaled to 1000, perturbed by " << delta
      << "  ngen = " << ngen << std::endl;
   for (size_t i = 0; i < inputList.size(); ++i) {
      const G6 input(inputList[i].GetCell());
      const G6 base = 1000.0 * input / input.Norm();

      const std::string strcel = inputList[i].GetStrCell();
      const size_t pos = strcel.find("IT#");
      const std::string label = (pos != std::string::npos) ? strcel.substr(pos) : "";

      std::cout << "\n;" << inputList[i].GetStrCell() << std::endl;
      std::cout << "G6 " << base << "    scaled input" <<  std::endl;
      for (size_t k = 0; k < ngen; ++k) {
         const G6 perturbation = delta * CreateTotallyRandomUnitOrthogonalComponent(input);
         const G6 perturbed = base + perturbation;
         const double d = NCDist(base.data(), perturbed.data());

         const char cl = inputList[i].GetStrCell()[0];
         if (latticeNames.find(cl) == std::string::npos)
            std::cout << "G6 " << perturbed << "  perturbed  " + label << std::endl;
         else {
            std::string s(2, ' ');
            s[0] = cl;
            std::cout << s << LRL_Cell_Degrees(perturbed) << "  perturbed  " + label << std::endl;
         }
      }
   }
}
