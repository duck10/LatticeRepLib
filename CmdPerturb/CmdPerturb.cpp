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

S6 MakeMorePerturbationAttempts(const double scale, const S6& s6, S6& out) {
   LRL_Cell cell;
   out = s6;
   out.SetValid(false);

   const int maxCount = 10;
   int count = 0;
   const bool outIsValid = out.IsValid() && out.GetValid();
   while (!out.GetValid() && count < maxCount) {
      ++count;
      const S6 perturber = scale * CreateTotallyRandomUnitOrthogonalComponent(out) / 1000.0;
      const S6 perturbation = delta * perturber;
      out = out + perturbation;
      cell = out;
      out.SetValid(cell.IsValid());
   }
   if (count == maxCount) {
      const S6 s = S6::randDeloneReduced();
      out = scale * s / 1000.0;
   }
   return out;
}

G6 PerturbOneVector(const S6& base)
{
   const double scale = base.norm();
   const S6 perturber = scale * CreateTotallyRandomUnitOrthogonalComponent(base) / 1000.0;
   const S6 perturbation = delta * perturber;
   S6 perturbed = base + perturbation;
   LRL_Cell cell(perturbed);  //lca test on cell.isvalid and cycle
   if (!cell.IsValid()) {
      S6 toReturn;
      perturbed = MakeMorePerturbationAttempts(scale, base, toReturn); //xxxxxxxxxxxxxxx should be base
      cell = perturbed;  //lca test on cell.isvalid and cycle
   }
   //std::cout << "scale = norm of base " << scale << std::endl;
   //std::cout << "norm of perturber " << perturber.norm() << std::endl;
   //std::cout << "norm of perturbed " << perturbed.norm() << std::endl;

   //std::cout << " in PerturbOneVector, ratio of input to output = " <<
   //   base.norm() / perturbed.norm() << std::endl;
   //std::cout << " in PerturbOneVector,difference ratio of input and perturbed  = " <<
   //   (base - perturbed).norm()/scale << std::endl;
   perturbed *= base.norm() / perturbed.norm();
   return perturbed;
}

void OutputPerturbedCell(const std::string& lattice, const S6& perturbed,
   const std::string& label) {
   LRL_Cell_Degrees cell(perturbed);

   if (!cell.IsValid()) {
      std::cout << "; invalid perturbed cell " << std::endl;
      //std::cout << "S6 " << perturbed << "  perturbed  " + label << std::endl;
   }
   else if (latticeNames.find(lattice) == std::string::npos)
      std::cout << "S6 " << perturbed << "  perturbed  " + label << std::endl;
   else
      std::cout << lattice + " " << cell << "  perturbed  " + label << std::endl;
}

void HandleOneInputCell(const LRL_ReadLatticeData& inputlattice) {
   const std::string lattice = inputlattice.GetLattice();

   const std::string strcel = inputlattice.GetStrCell();

   std::cout << "\n" << strcel << "    original input cell" << std::endl;

   const size_t pos = strcel.find("IT#");
   const std::string label = (pos != std::string::npos) ? strcel.substr(pos) : "";
   for (size_t k = 0; k < ngen; ++k) {
      const G6 perturbed = PerturbOneVector(inputlattice.GetCell());
      OutputPerturbedCell(lattice, perturbed, label);
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

   std::cout << "; Perturb vectors" << std::endl;;

   LRL_ReadLatticeData reader;
   const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      HandleOneInputCell(inputList[i]);
   }

   std::cout << "; " + LRL_ToString(reader.GetIncomingSemicolons()) << std::endl;
   std::cout << LRL_ToString("; CmdPerturb: number of cells to produce = ", ngen,
      " orthogonal perturbation of ", delta, "/1000\n\n");
}
