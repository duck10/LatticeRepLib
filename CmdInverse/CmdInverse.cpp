#include <iostream>
#include <string>
#include <vector>

#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_WriteLatticeData.h"

static std::string UpdateInverseLatticeType(const std::string& lattice) {
   if (LRL_StringTools::strToupper(lattice) == "F") return "I";
   else if (LRL_StringTools::strToupper(lattice) == "I") return "F";
   else return lattice;
}

int main()
{
   std::cout << "; ToInverse (to reciprocal cell)" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   LRL_WriteLatticeData writercout;
   writercout.SetPrecision(7);

   LRL_WriteLatticeData writercerr;
   writercerr.SetPrecision(12);

   for (const auto& data : inputList)
   {
      const std::string lattice(UpdateInverseLatticeType(data.GetLattice()));
      const LRL_Cell inverse(data.GetCell().Inverse());
      std::cout << writercout.WriteLatticeAndCell(lattice, inverse);
      std::cout << writercerr.WriteLatticeAndCell(lattice, inverse);
   }
}
