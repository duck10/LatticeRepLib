
#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "P3.h"

int main()
{
   std::cout << "; To P3" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (const auto& input : inputList) {
      std::cout << "P3 " << P3(input.GetCell()) << std::endl;
   }
}
