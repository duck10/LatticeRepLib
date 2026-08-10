#include <algorithm>
#include <string>
#include <vector>

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "G6.h"
#include "S6.h"
#include "S6Dist.h"
#include "LatticeCell.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_MaximaTools.h"
#include "MultiMetricDistsControls.h"
#include "NCDist.h"
#include "PairReporter.h"
#include "PrintTable.h"
#include "ProgramSetup.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "Selling.h"


// Local replacements for the LatticeConverter::TextOutput/MaximaOutput/Output/
// *ReducedOutput family, which were removed from LatticeConverter itself since
// most consumers don't need C3/D7. MultiMetricDists still uses C3 (reflection
// matrices below); D7 is no longer used at all.

static bool g_useMaxima = false;

static void LocalTextOutput(const std::string& label, const std::string& lattice, const LRL_Cell& cell) {
   std::cout << label << std::endl;
   std::cout << "lattice " << lattice << std::endl;
   std::cout << "LRL_Cell_Degrees  " << LRL_ToString(LRL_Cell_Degrees(cell)) << std::endl;
   std::cout << "G6 " << LRL_ToString(G6(cell)) << std::endl;
   std::cout << "S6 " << LRL_ToString(S6(cell)) << std::endl;
   std::cout << "C3 " << LRL_ToString(C3(cell)) << std::endl;
}

static void LocalMaximaOutput(const std::string& label, const std::string& lattice, const LRL_Cell& cell) {
   std::cout << label << std::endl;
   std::cout << "lattice " << lattice << std::endl;
   std::cout << "LRL_Cell  " << LRL_MaximaTools::MaximaFromString(LRL_ToString(cell)) << std::endl;
   std::cout << "G6 " << LRL_MaximaTools::MaximaFromString(LRL_ToString(G6(cell))) << std::endl;
   std::cout << "S6 " << LRL_MaximaTools::MaximaFromString(LRL_ToString(S6(cell))) << std::endl;
   std::cout << "C3 " << LRL_MaximaTools::MaximaFromString(LRL_ToString(C3(cell))) << std::endl;
}

static void LocalOutput(const std::string& label, const std::string& lattice, const LRL_Cell& cell) {
   g_useMaxima ? LocalMaximaOutput(label, lattice, LRL_Cell(cell)) : LocalTextOutput(label, lattice, LRL_Cell(cell));
}

static void LocalNiggliReducedOutput(const std::string& label, const std::string& lattice, const LRL_Cell& cell) {
   const LRL_Cell reducedCell = LatticeConverter::NiggliReduceCell(lattice, cell);
   LocalOutput(label, "P", reducedCell);
}

static void LocalDeloneReducedOutput(const std::string& label, const std::string& lattice, const LRL_Cell& cell) {
   const LRL_Cell reducedCell = LatticeConverter::DeloneReduceCell(lattice, cell);
   LocalOutput(label, "P", reducedCell);
}

static void LocalSellingReducedOutput(const std::string& label, const std::string& lattice, const LRL_Cell& cell) {
   const LRL_Cell reducedCell = LatticeConverter::SellingReduceCell(lattice, cell);
   LocalOutput(label, "P", reducedCell);
}

void Header(void) {
   std::cout << "Input:" << std::endl;
   std::cout << "\"v or g\" for G6" << std::endl;
   std::cout << "\"d\" for D7" << std::endl;
   std::cout << "\"s\" for Delone scalars" << std::endl;
   std::cout << "\"C3\" for C3 (Without parens or commas)" << std::endl;
   std::cout << "\"p\",\"a\",\"b\",\"c\",\"i\",\"f\",or \"r\" for unit cells or \"random\"" << std::endl;
   std::cout << "typical input:  f 10 10 10  90 90 90" << std::endl << std::endl;
}

std::string Letters(void) {
   return "V,G,D,S,P,A,B,C,I,F,R,C3,G6,S6,B4,D7,H";
}

std::string OutputIntialInput(const std::vector<LatticeCell>& cellDataList) {
   std::string s;
   for (size_t i = 0; i < cellDataList.size(); ++i) {
      s += LRL_ToString("Input ", i, ":  ") + cellDataList[i].getInputLine() + "\n";
   }

   for (size_t i = 0; i < cellDataList.size(); ++i) {
      s += LRL_ToString("Input ", i, ":  ") + LRL_ToString(C3(cellDataList[i].getCell())) + "\n";
   }

   return s;
}

void PrintModifiedTable(const PrintTable& tbl, const bool labelColumns, const char rowSeparator, const std::string& colSeparator) {
   PrintTable tblX(tbl);
   tblX.SetNumberColumns(labelColumns);
   tblX.SetRowSeparator(rowSeparator);
   tblX.SetColumnSeparator(colSeparator);
   std::cout << std::endl;
   std::cout << tblX << std::endl;

}

void ReduceAll(const LatticeCell& cellData, S6& s6, G6& g6) {
   LatticeConverter converter;
   g6 = LatticeConverter::NiggliReduceCell(cellData.getLatticeType(), cellData.getCell());
   s6 = LatticeConverter::SellingReduceCell(cellData.getLatticeType(), cellData.getCell());
}

void OutputCellData(LatticeConverter& converter, const std::vector<LatticeCell>& cellDataList) {
   const std::string letters = Letters();
   for (size_t i1 = 0; i1 < cellDataList.size(); ++i1) {
      const LatticeCell& rcd = cellDataList[i1];
      const std::string lattice = rcd.getLatticeType();

      if (letters.find(LRL_StringTools::strToupper(lattice)) == std::string::npos) continue;
      std::cout << std::endl;
      std::cout << "LRL_Cell # " << i1 << "  *******************************" << std::endl;
      LocalOutput("Input Data", lattice, rcd.getCell());
      std::cout << std::endl;
      LocalNiggliReducedOutput("Niggli Reduced", lattice, rcd.getCell());
      std::cout << std::endl;
      LocalDeloneReducedOutput("Delone Reduced", lattice, rcd.getCell());
      std::cout << std::endl;
      LocalSellingReducedOutput("Selling Reduced", lattice, rcd.getCell());
      std::cout << std::endl;
      double f1, f2, f3, f4;
      const LRL_Cell reducedCell = converter.DeloneReduceCell(lattice, rcd.getCell());
   }
}

void PrintDistanceData(const std::vector<LatticeCell>& cellDataList) {
   PrintTable gtbl(cellDataList.size(), cellDataList.size(), 13);
   PrintTable stbl(cellDataList.size(), cellDataList.size(), 13);
   PrintTable ctbl(cellDataList.size(), cellDataList.size(), 13);
   G6 g6red1;
   S6 s6red1;
   G6 g6red2;
   S6 s6red2;
   if (cellDataList.size() > 1) {
      std::cout << "Distance_Summary  NCDist(23) S6Dist(23)" << std::endl;
      S6Dist s6dist(10000.0);
      for (size_t i1 = 0; i1 < cellDataList.size(); ++i1) {
         ReduceAll(cellDataList[i1], s6red1, g6red1);
         s6dist.SetDebug(true);
         for (size_t i2 = i1; i2 < cellDataList.size(); ++i2) {
            ReduceAll(cellDataList[i2], s6red2, g6red2);
            const double g12 = NCDist(g6red1.GetVector().data(), g6red2.GetVector().data());
            const double s12 = s6dist.DistanceBetween(s6red1, s6red2);
            const double c12 = CS6Dist(s6red1.data(), s6red2.data());
            gtbl.insert_center(i1, i2, g12);
            stbl.insert_center(i1, i2, s12);
            ctbl.insert_center(i1, i2, c12);

            std::cout << " (" << i1 << "," << i2 << ")   " << g12 << "   " << s12 << std::endl;
         }
      }
   }
   std::cout << G6::GetName();
   PrintModifiedTable(gtbl, true, '#', " | ");
   std::cout << S6::GetName();
   PrintModifiedTable(stbl, true, '#', " | ");
   std::cout << "CSDist";
   PrintModifiedTable(ctbl, true, '#', " | ");
}

void ListReflections(const S6& s) {
   const std::vector<S6> v = S6Dist::Generate24Reflections(s);
   std::cout << LRL_ToString(v) << std::endl;
}

void ListReflectionsByC3() {
   StoreResults<int, PairReporter<C3, MatS6> > storeMat(100);
   const std::vector<MatS6> vmR = MatS6::GetReflections();
   for (size_t i = 0; i < vmR.size(); ++i) {
      int index = 0;
      const S6 ord("1 2 3 4 5 6");
      const C3 c(vmR[i] * ord);
      for (size_t kk = 0; kk < 3; ++kk) {
         if (c[kk] == std::complex<double>(1, 4)) index += 1;
         if (c[kk] == std::complex<double>(4, 1)) index += 2;
         if (c[kk] == std::complex<double>(2, 5)) index += 10;
         if (c[kk] == std::complex<double>(5, 2)) index += 20;
         if (c[kk] == std::complex<double>(3, 6)) index += 100;
         if (c[kk] == std::complex<double>(6, 3)) index += 200;
      }
      storeMat.Store(index, PairReporter<C3, MatS6>(c, vmR[i]));

   }
   //storeMat.ShowResults();
}

int main(int argc, char* argv[]) {
   std::cout << "; MultiMetricDists" << std::endl;

   try {
      std::string doMaxima = "";
      if (argc > 1) {
         const std::string strtest = argv[1];
         doMaxima = strtest;
      }
      LatticeConverter converter;
      Header();

      if (LRL_StringTools::strToupper(doMaxima.substr(0, 1))[0] == 'Y') g_useMaxima = true;

      MultiMetricDistsControls controls;
      const BasicProgramInput<MultiMetricDistsControls> dc_setup("CmdMultiMetricDists", controls);

      if (controls.getShowControls()) {
         std::cout << controls << std::endl;
      }

      const std::vector<LatticeCell>& cellDataList = dc_setup.getInputList();

      OutputCellData(converter, cellDataList);

      std::cout << OutputIntialInput(cellDataList) << std::endl;
      if (!cellDataList.empty()) PrintDistanceData(cellDataList);

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}
