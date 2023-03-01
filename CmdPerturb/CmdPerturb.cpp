#include <iostream>
#include <string>
#include <vector>

//#include "BasisBase.h" // this has CreateUnitOrthogonalVector
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "NCDist.h"
#include "rhrand.h"
#include "S6.h"

double delta = 1.0;
size_t ngen = 20;

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

G6 PerturbOneVector(const std::string& lattice, const S6& base,
   const std::string& label) {
   return G6();
}

G6 PerturbOneVector( const LRL_ReadLatticeData& input, const S6& base, 
   const std::string& label )
{
   const S6 perturbation = delta * CreateTotallyRandomUnitOrthogonalComponent(base);
   const S6 perturbed = base + perturbation;

   return perturbed;
}

void OutputPerturbedCell(const std::string& lattice, const S6& perturbed,
   const std::string& label) {
   if (latticeNames.find(lattice) == std::string::npos)
      std::cout << "S6 " << perturbed << "  perturbed  " + label << std::endl;
   else {
      std::cout << lattice+" " << LRL_Cell_Degrees(perturbed) << "  perturbed  " + label << std::endl;
   }
}

void HandleOneInputCell(const LRL_ReadLatticeData& inputlattice) {
   const S6 inputCell(inputlattice.GetCell());
   const double scale = 1000.0 / inputCell.Norm();
   const S6 base = inputCell * scale;

   const std::string strcel = inputlattice.GetStrCell();

   std::cout << "\n;" << strcel << "    original inputCell" << std::endl;

   const size_t pos = strcel.find("IT#");
   const std::string label = (pos != std::string::npos) ? strcel.substr(pos) : "";
   for (size_t k = 0; k < ngen; ++k) {
      //const G6 perturbed1 = PerturbOneVector(inputlattice, base, label);
      const G6 perturbed2 = PerturbOneVector(strcel[0], inputCell, label);
      OutputPerturbedCell(strcel.substr(0,1), perturbed2, label);
   }
}

int main(int argc, char* argv[])
{
   if (argc > 1) {
      ngen = atoi(argv[1]);
      if (argc > 2) {
         const double d = atof(argv[2]);
         if (d != 0.0) delta = atof(argv[2]);
      }
   }

   LRL_ReadLatticeData reader;
   const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();
   std::cout << "; Perturb vectors, perturbed in G6, perturbed by (ppt) " << delta
      << "  ngen = " << ngen << std::endl;
   for (size_t i = 0; i < inputList.size(); ++i) {
      HandleOneInputCell(inputList[i]);
   }

   std::cout << "; "+LRL_ToString(reader.GetIncomingSemicolons()) << std::endl;
   std::cout << LRL_ToString("; CmdPerturb ngen = ", ngen,
      " orthogonal delta = ", delta, " of 1000\n\n");
}
