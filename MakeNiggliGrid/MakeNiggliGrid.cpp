// MakeNiggliGrid.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

//#include "IT_TypeList.h"
#include "GenerateLatticeTypeExamples.h"

int main()
{
   const auto IT_types = GenerateLatticeTypeExamples::CreateListOfNiggliTypes();

   for (const auto& type : IT_types) {

      const std::string itNum     = type->GetITNumber();
      const std::string name      = type->GetName();
      const std::string character = type->GetCharacter();
      const std::string xtalType  = type->GetXtalType("");
      std::cout << "IT # " << itNum << "  Roof name " << name << std::endl;
      std::cout << "   Bravais class " << xtalType << "  character  " << character << std::endl;
      std::cout <<std::endl;
   }

   const int i19191 = 19191;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
