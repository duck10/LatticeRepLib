#include "LRL_WriteLatticeData.h"


LRL_WriteLatticeData data;

/*
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(inputList[i].GetLattice(), inputList[i].GetCell());
      //std::cout << pCell << std::endl;
      G6 g;
      const bool b = Niggli::Reduce(G6(pCell), g);
      //std::cout << "cycles " << Niggli::GetCycles() << std::endl;
      std::cout << "G6 " << g << std::endl;
   }

   */