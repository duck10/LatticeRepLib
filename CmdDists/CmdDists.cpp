
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_CreateFileName.h"
#include "LatticeConverter.h"
#include "LRL_DataStreamToString.h"
#include "LRL_DataToSVG.h"
#include "LRL_ReadLatticeData.h"
#include "NCDist.h"
#include "PrintTable.h"
#include "S6.h"


static void PrintModifiedTable(const PrintTable& tbl, const bool labelColumns, const bool labelRows, const char rowSeparator, const std::string& colSeparator) {
   PrintTable tblX(tbl);
   tblX.SetNumberColumns(labelColumns);
   tblX.SetNumberRows(labelRows);
   tblX.SetRowSeparator(rowSeparator);
   tblX.SetColumnSeparator(colSeparator);
   std::cout << std::endl;
   std::cout << tblX << std::endl;

}

static std::string DoubleToSting(const double d, const int precision = 3) {
   std::stringstream sst;
   sst << std::fixed << std::setprecision(precision) << d;
   return sst.str();

}


int main()
{
   std::cout << "; CmdDists, calculate distances" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::vector<S6> s6Cells;
   for (int i = 0; i < inputList.size(); ++i) {
      s6Cells.push_back(inputList[i].GetCell());
   }

   if (inputList.size() < 2) {
      std::cout << "; CmdDists requires at least 2 cells input" << std::endl;
      exit(99);
   }

   std::vector<std::string> vLattices;
   std::vector<LRL_Cell> vcells;
   std::vector<G6> vg6Red;
   std::vector<S6> vs6Red;

   for (size_t i = 0; i < inputList.size(); ++i) {
      const std::string lattice = inputList[i].GetLattice();
      const LRL_Cell cell = inputList[i].GetCell();

      vcells.emplace_back(cell);
      vLattices.emplace_back(lattice);
      vg6Red.emplace_back(LatticeConverter::NiggliReduceCell (lattice, cell));
      vs6Red.emplace_back(LatticeConverter::SellingReduceCell(lattice, cell));
   }

   double* dG6_0_data = vg6Red[0].data();  // NCDist does not allow const
   double* dS6_0_data = vs6Red[0].data();  // CS6Dist does not allow const
   const G6 g6First = vcells[0];

   PrintTable table(inputList.size(), 8,12);
   table.insert_left(0, 0, "; ordinals  ");
   table.insert_left(0, 1, "G6-Euclid");
   table.insert_left(0, 2, "NCDist");
   table.insert_left(0, 3, "CS6Dist ||");
   table.insert_left(0, 4, " ordinals");
   table.insert_left(0, 5, "G6-Euclid");
   table.insert_left(0, 6, "NCDist");
   table.insert_left(0, 7, "CS6Dist");

   for (size_t i = 0; i < inputList.size()-1; ++i) {
      const double dEuRolling = (G6(vcells[i]) - G6(vcells[i + 1])).norm();
      const double dG6Rolling = NCDist (vg6Red[i].data(), vg6Red[i + 1].data());
      const double dS6Rolling = CS6Dist(vs6Red[i].data(), vs6Red[i + 1].data());

      const double dEuToFirst = (g6First - G6(vcells[i + 1])).norm();
      const double dG6ToFirst = NCDist(dG6_0_data, vg6Red[i + 1].data());
      const double dS6ToFirst = CS6Dist(dS6_0_data, vs6Red[i + 1].data());

      const std::string ordinal1 = LRL_DataToSVG("; ", i+1, "", "to ", i + 2);
      const std::string ordinal2 = LRL_DataToSVG("", "", "", "1 to ", i + 2);

      table.insert_center(i + 1, 0, ordinal1);
      table.insert_center(i + 1, 4, ordinal2);
      table.insert_center(i+1, 1, DoubleToSting(dEuRolling)+"\0"); //
      table.insert_center(i+1, 2, DoubleToSting(dG6Rolling)+" \0");
      table.insert_center(i+1, 3, DoubleToSting(dS6Rolling)+" ||"); //

      table.insert_center(i+1, 5, "  "+ DoubleToSting(dEuToFirst)); //
      table.insert_center(i+1, 6, DoubleToSting(dG6ToFirst)+" "); //
      table.insert_center(i+1, 7, DoubleToSting(dS6ToFirst)+" "); //
      //std::cout << ";   " << std::fixed << std::setprecision(3)
      //   << i+1 << " to " << i + 2 << "       " 
      //   << dEuRolling << "       " << dG6Rolling << "    " << dS6Rolling
      //   << " ___   " << dEuToFirst << "    " << dG6ToFirst << "    " << dS6ToFirst
      //   << "  " <<  1 << " to " << i + 2 << "       "
      //   << std::endl;


   }
   std::cout << "; <pre>";
   PrintModifiedTable(table, false, false,'\0', " ");

   std::cout << ";.......... input.cells ................"
      "........................ ordinals ....." << std::endl;
   for (size_t i = 0; i < vcells.size(); ++i) {
      std::cout 
         << "  " + vLattices[i]
         << std::fixed << std::setprecision(3)
         << LRL_Cell_Degrees(vcells[i]) << "    " << i + 1 << std::endl;
   }
   std::cout << "; </pre>" << std::endl;
}
