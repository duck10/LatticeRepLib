// RoofFigures.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <vector>

#include "LRL_ReadLatticeData.h"
#include "RoofFigures.h"

int main()
{
   Figure_44A fig44A;
   std::cout << "; Roof Figures" << std::endl;
   //const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   //for (size_t i = 0; i < inputList.size(); ++i) {
   //   std::cout << "G6 " << G6(inputList[i].GetCell()) << std::endl;
   //}

   std::cout << fig44A << std::endl;
   return 0;
}
