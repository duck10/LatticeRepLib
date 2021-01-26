// CmdLine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "LRL_ReadLatticeData.h"


int main()
{
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      std::cout << "S6 " << S6(inputList[i].GetCell()) << std::endl;
   }
}

