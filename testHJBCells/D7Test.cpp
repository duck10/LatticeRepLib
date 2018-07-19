// Converter.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <string>
#include <vector>

#include "LatticeConverter.h"
#include "D7Dist_.hpp"
#include "NCDist_.hpp"
#include "PrintTable.h"
#include "ProjectorTools.h"
#include "ReadCellData.h"
#include "ToString.h"


void Header( void ) {
   std::cout << "Input:" << std::endl;
   std::cout << "\"v\" for G6" << std::endl;
   std::cout << "\"d\" for D7" << std::endl;
   std::cout << "\"s\" for Delone scalars" << std::endl;
   std::cout << "\"p\",\"a\",\"b\",\"c\",\"i\",\"f\",or \"r\" for unit cells" << std::endl;
   std::cout << "typical input:  f 10 10 10  90 90 90" << std::endl << std::endl;
}

std::string Letters( void ) {
   return "v,d,s,p,a,b,c,i,f,r";
}

std::string OutputIntialInput( const std::vector<ReadCellData>& cellDataList ) {
   std::string s;
   for ( size_t i = 0; i < cellDataList.size( ); ++i ) {
      s += ToString( "Input ", i, ":  " ) + cellDataList[i].GetStrCell( ) + "\n";
   }

   return s;
}

void PrintModifiedTable( const PrintTable& tbl, const bool labelColumns, const bool labelRows, const char rowSeparator, const std::string& colSeparator ) {
   PrintTable tblX(tbl);
   tblX.SetNumberColumns(labelColumns);
   tblX.SetNumberRows( labelRows );
   tblX.SetRowSeparator(rowSeparator);
   tblX.SetColumnSeparator(colSeparator);
   std::cout << std::endl;
   std::cout << tblX << std::endl;

}

void ComputeArrays(const ReadCellData& cellDataList, double gArray[], double dArray[]) {
   LatticeConverter converter;
   const Cell niggliReducedCell1 = converter.NiggliReduceCell  (cellDataList.GetLattice(), cellDataList.GetCell());
   const Cell deloneReducedCell1 = converter.DeloneReduceCell(cellDataList.GetLattice(), cellDataList.GetCell());
   ProjectorTools::ConvertG6ToArray(niggliReducedCell1.Cell2V6(), gArray);
   ProjectorTools::ConvertD7ToArray(D7(deloneReducedCell1.Cell2V6()), dArray);
}

std::vector<ReadCellData> GetInputCells(void) {
   const std::string letters = Letters();
   std::string lattice;
   std::vector<ReadCellData> cellDataList;
   while (lattice != "EOF") {
      ReadCellData rcd;
      lattice = rcd.GetLattice();
      const size_t quest = letters.find(lattice);
      if ((!lattice.empty()) && letters.find(lattice) != std::string::npos)
         cellDataList.push_back(rcd);
   }

   return cellDataList;
}

void OutputCellData( LatticeConverter& converter, const std::vector<ReadCellData>& cellDataList) {
   const std::string letters = Letters();
   std::string lattice;
   for (size_t i1 = 0; i1 < cellDataList.size(); ++i1) {
      const ReadCellData& rcd = cellDataList[i1];
      lattice = rcd.GetLattice();
      if (letters.find(lattice) == std::string::npos) continue;
      std::cout << std::endl;
      std::cout << "Cell # " << i1 << "  *******************************" << std::endl;
      converter.Output("Input Data", lattice, rcd.GetCell());
      std::cout << std::endl;
      converter.NiggliReducedOutput("Niggli Reduced", lattice, rcd.GetCell());
      std::cout << std::endl;
      converter.DeloneReducedOutput("Delaunay Reduced", lattice, rcd.GetCell());
      std::cout << std::endl;
   }
}

void PrintDistanceData(const std::vector<ReadCellData>& cellDataList) {
   PrintTable gtbl(cellDataList.size(), cellDataList.size(), 13);
   PrintTable dtbl(cellDataList.size(), cellDataList.size(), 13);
   if (cellDataList.size() > 1) {
      std::cout << "Distance Summary" << std::endl;
      double g1[6], g2[6], d1[7], d2[7];
      for (size_t i1 = 0; i1 < cellDataList.size(); ++i1) {
         ComputeArrays(cellDataList[i1], g1, d1);
         for (size_t i2 = i1; i2 < cellDataList.size(); ++i2) {
            ComputeArrays(cellDataList[i2], g2, d2);
            const double g12 = NCDist_(g1, g2);
            const double d12 = D7Dist_(d1, d2);
            gtbl.insert_center(i1, i2, g12);
            dtbl.insert_center(i1, i2, d12);

            std::cout << " (" << i1 << "," << i2 << ")   " << g12 << "   " << d12 << std::endl;
            /* ++d7dist; */
         }
      }
   }
   PrintModifiedTable(gtbl, false, false, '#', " | ");
   PrintModifiedTable(dtbl, false, false, '#', " | ");
}

int main( int argc, char* argv[] ) {
   LatticeConverter converter;
   Header( );
   const std::string letters = Letters( );

   std::cout << "Do you want Maxima style output? (y/n)   ";
   std::string answer;
   std::getline( std::cin, answer );
   if ( answer[0] == 'y' ) converter.SetOutputMaxima( );

   std::string lattice;
   const std::vector<ReadCellData> cellDataList = GetInputCells();
   OutputCellData(converter, cellDataList);

   std::cout << OutputIntialInput( cellDataList ) << std::endl;
   PrintDistanceData(cellDataList);

   return 0;
}
