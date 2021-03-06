// ReductionTimeingTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <ctime>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "D7.h"
#include "Delone.h"
#include "G6.h"
#include "GenerateRandomLattice.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "Niggli.h"
#include "NullOps.h"
#include "S6.h"

template<typename VEC1, typename METHOD1>
VEC1 TestTiming(const std::string& label, const std::vector<std::string> vstr, const std::vector<S6> vin) {
   LatticeConverter converter;
   LRL_Cell dummy;
   std::clock_t start;
   std::vector<LRL_Cell> vcell;
   std::list<LRL_Cell>::iterator itcell;

   VEC1 red1;
   for (size_t i = 0; i < vin.size(); ++i) {
      vcell.push_back(converter.MakePrimitiveCell(vstr[i], vin[i]));
   }

   LRL_Cell cellP;
   start = std::clock();
   for (size_t i = 0; i < vcell.size(); ++i) {
      const bool b = METHOD1::Reduce(VEC1(cellP), red1);
      dummy = red1;
   }

   dummy *= 2.0; // to try to get the optimizer to keep calculating dummy as unit cell parmeters
   std::cout << std::endl;
   int timex = std::clock() - start;
   std::cout << VEC1::GetName() << "  " << METHOD1::GetName() << "  " << timex << " msec" << std::endl;
   std::cout << label << " " << timex << std::endl;
   return dummy;
}

template<typename VEC1, typename METHOD1, typename VEC2, typename METHOD2>
VEC2 TestTiming(const std::string& label, const std::vector<std::string> vstr, const std::vector<S6> vin) {
   LatticeConverter converter;
   LRL_Cell dummy;
   std::clock_t start;
   std::vector<LRL_Cell> vcell;
   std::list<LRL_Cell>::iterator itcell;

   VEC1 red1;
   VEC2 red2;
   for (size_t i = 0; i < vin.size(); ++i) vcell.push_back(vin[i]);

   LRL_Cell cellP;
   start = std::clock();
   for (size_t i = 0; i < vcell.size(); ++i) {
      const LRL_Cell cell = converter.MakePrimitiveCell(vstr[i], vcell[i]);
      const bool b = METHOD1::Reduce(VEC1(cellP), red1);
      dummy = red1;
      const bool b2 = METHOD2::Reduce(VEC2(red1), red2);
      dummy = red2;
   }

   dummy *= 2.0; // to try to get the optimizer to keep calculating dummy as unit cell parmeters
   int timex = std::clock() - start;
   std::cout << std::endl;
   std::cout << "Vector type1 " << VEC1::GetName() << "   Reduction method1  " << METHOD1::GetName() << std::endl <<
      "Vector type2 " << VEC2::GetName() << "   Reduction method2  " << METHOD2::GetName() << "  " << timex << " msec" << std::endl;
   std::cout << label << " " << timex << std::endl;

   return dummy;
}

std::vector<LRL_ReadLatticeData> GetInputCells(void) {
   const std::string letters ("V,G,D,S,P,A,B,C,I,F,R,C3,G6,S6,B4,D7,H");
   std::string lattice;
   std::vector<LRL_ReadLatticeData> cellDataList;
   LRL_ReadLatticeData rcd;
   while (lattice != "EOF") {
      rcd.read();
      lattice = rcd.GetLattice();
      if ((!lattice.empty()) && (letters.find(toupper(lattice[0])) != std::string::npos))
         cellDataList.push_back(rcd);
   }

   return cellDataList;
}

void TestRandomGeneration() {
   std::vector<S6> v;
   std::vector<std::string> vstr;
   int seed = 19191;
   GenerateRandomLattice<S6> grl(seed);
   size_t count = 100000;
   std::cout << "samples generated " << count << "    grl.randSellingUnreduced()" << std::endl;
   for (size_t i = 0; i < count; ++i) {
      v.push_back(grl.randSellingUnreduced());
      vstr.push_back("P");
   }

   TestTiming<G6, Niggli >("#A", vstr, v); //A
   TestTiming<D7, Delone >("#B", vstr, v); //B
   TestTiming<S6, Selling>("#C", vstr, v); //C

   TestTiming<G6, Selling, G6, Niggli >("#D", vstr, v); //D
   TestTiming<D7, Selling, S6, Delone >("#E", vstr, v); //E
   TestTiming<S6, Selling, S6, Selling>("#F", vstr, v); //F
}

void TestPDBCells() {
   std::cout << "PDB Cells input " << std::endl;
   const std::vector<LRL_ReadLatticeData> input = GetInputCells();

   std::cout << "PDB Cells input " << input.size() << " samples" << std::endl;
   if (!input.empty()) {
      std::vector<S6> v;
      std::vector<std::string> vstr;
      for (size_t i = 0; i < input.size(); ++i) {
         v.push_back(input[i].GetCell());
         vstr.push_back(input[i].GetLattice());
      }

      TestTiming<G6, Niggli >("#A", vstr, v); //A
      TestTiming<D7, Delone >("#B", vstr, v); //B
      TestTiming<S6, Selling>("#C", vstr, v); //C

      TestTiming<G6, Selling, G6, Niggli >("#D",  vstr, v); //D
      TestTiming<D7, Selling, S6, Delone >("#E",  vstr, v); //E
      TestTiming<S6, Selling, S6, Selling>("#F",  vstr, v); //F
   }
}

int main(int argc, char *argv[])
{
   TestPDBCells();
   TestRandomGeneration();

}

