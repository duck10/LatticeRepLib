#include <iostream>

//#include "BasisBase.h" // this has CreateUnitOrthogonalVector
#include "GenerateLatticeTypeExamples.h"
#include "LatticeConverter.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "NCDist.h"
#include "rhrand.h"
#include "S6.h"
#include "Selling.h"

const double delta = 1.0;
const size_t ngen = 100;

const std::string latticeNames = "pifrhcPIFRHC";


template<typename T>
T CreateTotallyRandomUnitOrthogonalComponent(const T& t) {
   // assume t is not null and rand is not parallel to t
   const T base = t / t.norm();
   T temp;
   for (size_t i = 0; i < 6; ++i) temp[i] = RHrand().urand() - 0.5;
   const double d = dot(VecN(base.GetVector()), VecN(temp.GetVector()));
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
      std::cout << "\n;" << inputList[i].GetStrCell() << std::endl;
      std::cout << "G6 " << base << "    scaled input" <<  std::endl;
      for (size_t k = 0; k < ngen; ++k) {
         const G6 perturbation = delta * CreateTotallyRandomUnitOrthogonalComponent(input);
         const G6 perturbed = base + perturbation;
         const double d = NCDist(base.data(), perturbed.data());

         const char cl = inputList[i].GetStrCell()[0];
         if (latticeNames.find(cl) == std::string::npos)
            std::cout << "G6 " << perturbed << "  perturbed" << std::endl;
         else {
            std::string s(2, ' ');
            s[0] = cl;
            std::cout << s << LRL_Cell_Degrees(perturbed) << "  perturbed" << std::endl;
         }

         //std::cout << std::endl;
         //std::cout << input << "    " << input.norm() << " input" << std::endl;
         //std::cout << base << "    " << base.norm() << "  base" << std::endl;
         //std::cout << perturbation << "    " << perturbation.norm() << "  perturbation" << std::endl;
         //std::cout << perturbed << "     " << perturbed.norm() << "  perturbed" << std::endl;
         //std::cout << "NCDist = " << d << std::endl;
      }
   }
}
