
#include <algorithm>
#include <string>
#include <vector>

#include "stdafx.h"

#include "S6.h"
#include "D7Dist_.hpp"
#include "S6Dist.h"
#include "LatticeConverter.h"
#include "NCDist_.hpp"
#include "PrintTable.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "Selling.h"
#include "Triangle.h"


void Header(void) {
   std::cout << "Input:" << std::endl;
   std::cout << "\"v\" for G6" << std::endl;
   std::cout << "\"d\" for D7" << std::endl;
   std::cout << "\"s\" for Delone scalars" << std::endl;
   std::cout << "\"p\",\"a\",\"b\",\"c\",\"i\",\"f\",or \"r\" for unit cells or \"random\"" << std::endl;
   std::cout << "typical input:  f 10 10 10  90 90 90" << std::endl << std::endl;
}

std::string Letters(void) {
   return "V,D,S,P,A,B,C,I,F,R";
}

std::string OutputIntialInput(const std::vector<LRL_ReadLatticeData>& cellDataList) {
   std::string s;
   for (size_t i = 0; i < cellDataList.size(); ++i) {
      const std::vector<std::pair<std::string, std::string > > boundaryCases = D7(cellDataList[i].GetCell()).ClassifyVector(0.5);
      std::string boundary;
      for (unsigned long k = 0; k < boundaryCases.size(); ++k) {
         boundary += " " + boundaryCases[k].second;
      }
      s += LRL_ToString("Input ", i, ":  ") + cellDataList[i].GetStrCell() + boundary + "\n";
   }

   return s;
}

void PrintModifiedTable(const PrintTable& tbl, const bool labelColumns, const bool labelRows, const char rowSeparator, const std::string& colSeparator) {
   PrintTable tblX(tbl);
   tblX.SetNumberColumns(labelColumns);
   tblX.SetNumberRows(labelRows);
   tblX.SetRowSeparator(rowSeparator);
   tblX.SetColumnSeparator(colSeparator);
   std::cout << std::endl;
   std::cout << tblX << std::endl;

}

void ReduceAll(const LRL_ReadLatticeData& cellDataList, S6& s6, D7& d7, G6& g6) {
   LatticeConverter converter;
   g6 = converter.NiggliReduceCell(cellDataList.GetLattice(), cellDataList.GetCell());
   s6 = converter.DeloneReduceCell(cellDataList.GetLattice(), cellDataList.GetCell());
   d7 = s6;
}

std::vector<LRL_ReadLatticeData> GetInputCells(void) {
   const std::string letters = Letters();
   std::string lattice;
   std::vector<LRL_ReadLatticeData> cellDataList;
   while (lattice != "EOF") {
      LRL_ReadLatticeData rcd;
      lattice = rcd.GetLattice();
      const size_t quest = letters.find(lattice);
      if ((!lattice.empty()) && (letters.find(toupper(lattice[0])) != std::string::npos))
         cellDataList.push_back(rcd);
   }

   return cellDataList;
}

void OutputCellData(LatticeConverter& converter, const std::vector<LRL_ReadLatticeData>& cellDataList) {
   const std::string letters = Letters();
   std::string lattice;
   for (size_t i1 = 0; i1 < cellDataList.size(); ++i1) {
      const LRL_ReadLatticeData& rcd = cellDataList[i1];
      lattice = rcd.GetLattice();
      if (letters.find(LRL_StringTools::strToupper(lattice)) == std::string::npos) continue;
      std::cout << std::endl;
      std::cout << "LRL_Cell # " << i1 << "  *******************************" << std::endl;
      converter.Output("Input Data", lattice, rcd.GetCell());
      std::cout << std::endl;
      converter.NiggliReducedOutput("Niggli Reduced", lattice, rcd.GetCell());
      std::cout << std::endl;
      converter.DeloneReducedOutput("Delone Reduced", lattice, rcd.GetCell());
      std::cout << std::endl;
   }
}

void PrintDistanceData(const std::vector<LRL_ReadLatticeData>& cellDataList) {
   PrintTable gtbl(cellDataList.size(), cellDataList.size(), 13);
   PrintTable dtbl(cellDataList.size(), cellDataList.size(), 13);
   PrintTable stbl(cellDataList.size(), cellDataList.size(), 13);
   G6 g6red1;
   S6 s6red1;
   D7 d7red1;
   G6 g6red2;
   S6 s6red2;
   D7 d7red2;
   if (cellDataList.size() > 1) {
      std::cout << "Distance_Summary  NCDist(23) D7Dist(23) S6Dist(23)" << std::endl;
      for (size_t i1 = 0; i1 < cellDataList.size(); ++i1) {
         ReduceAll(cellDataList[i1], s6red1, d7red1, g6red1);
         S6Dist s6dist(10000.0);
         s6dist.SetDebug(true);
         for (size_t i2 = i1; i2 < cellDataList.size(); ++i2) {
            ReduceAll(cellDataList[i2], s6red2, d7red2, g6red2);
            const double g12 = NCDist(g6red1.GetVector().data(), g6red2.GetVector().data());
            const double d12 = D7Dist(d7red1.GetVector().data(), d7red2.GetVector().data());
            const double s12 = s6dist.DistanceBetween(s6red1, s6red2);
            gtbl.insert_center(i1, i2, g12);
            dtbl.insert_center(i1, i2, d12);
            stbl.insert_center(i1, i2, s12);

            std::cout << " (" << i1 << "," << i2 << ")   " << g12 << "   " << d12 << "   " << s12 << std::endl;
            /* ++d7dist; */
         }
      }
   }
   std::cout << G6::GetName();
   PrintModifiedTable(gtbl, true, false, '#', " | ");
   std::cout << D7::GetName();
   PrintModifiedTable(dtbl, true, false, '#', " | ");
   std::cout << S6::GetName();
   PrintModifiedTable(stbl, true, false, '#', " | ");
}

int main(int argc, char* argv[]) {
   std::cout << "main file name *** " << __FILE__ << " ***" << std::endl;

   //D7_BoundaryTests bt;
   //bt.MainTests();

   //TestConversions(cellDataList);
   //Selling::SetDebug(true);

   LatticeConverter converter;
   Header();
   const std::string letters = Letters();

   std::cout << "Do you want Maxima style output? (y/n)   ";
   std::string answer;
   std::getline(std::cin, answer);
   if (answer[0] == 'y') converter.SetOutputMaxima();

   const std::vector<LRL_ReadLatticeData> cellDataList = GetInputCells();

   std::string lattice;
   OutputCellData(converter, cellDataList);

   std::cout << OutputIntialInput(cellDataList) << std::endl;
   if (!cellDataList.empty())PrintDistanceData(cellDataList);

   Triangle triangle(cellDataList);
   const size_t violations = triangle.Test();
   std::cout << "Triangle violation count = " << violations << std::endl;

   return 0;
}