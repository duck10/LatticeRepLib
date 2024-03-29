#include <iostream>

#include "LRL_ReadLatticeData.h"
#include "WebIO.h"

int main(int argc, char* argv[]) {
   std::cout << "; Command Line Parameters" << std::endl;

   //LRL_ReadLatticeData reader;
   //const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();

   //for (const auto& in : inputList) {
   //   std::cout << in.GetLattice() << " " << in.GetCell() << std::endl;
   //}

   if (argc >= 8) {
      for (size_t i = 1; i < 8; ++i) {
         std::cout << std::string(argv[i]) << " ";
      }
      std::cout << std::endl;
   }
}
