
#include <algorithm>
#include <string>
#include <vector>

#include "stdafx.h"

#include "S6.h"
#include "D7Dist_.hpp"
#include "S6Dist.h"
#include "LatticeConverter.h"
#include "NCDist_.hpp"
#include "PairReporter.h"
#include "PrintTable.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "Selling.h"
#include "Triangle.h"


void Header(void) {
   std::cout << "Input:" << std::endl;
   std::cout << "\"v or g\" for G6" << std::endl;
   std::cout << "\"d\" for D7" << std::endl;
   std::cout << "\"s\" for Delone scalars" << std::endl;
   std::cout << "\"p\",\"a\",\"b\",\"c\",\"i\",\"f\",or \"r\" for unit cells or \"random\"" << std::endl;
   std::cout << "typical input:  f 10 10 10  90 90 90" << std::endl << std::endl;
}

std::string Letters(void) {
   return "V,G,D,S,P,A,B,C,I,F,R,C3";
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

   for (size_t i = 0; i < cellDataList.size(); ++i) {
      const std::vector<std::pair<std::string, std::string > > boundaryCases = D7(cellDataList[i].GetCell()).ClassifyVector(0.5);
      std::string boundary;
      for (unsigned long k = 0; k < boundaryCases.size(); ++k) {
         boundary += " " + boundaryCases[k].second;
      }
      s += LRL_ToString("Input ", i, ":  ") + LRL_ToString(C3(cellDataList[i].GetCell())) + "\n";
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
   d7 = converter.DeloneReduceCell(cellDataList.GetLattice(), cellDataList.GetCell());
   s6 = converter.SellingReduceCell(cellDataList.GetLattice(), cellDataList.GetCell());
}

std::vector<LRL_ReadLatticeData> GetInputCells(void) {
   const std::string letters = Letters();
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

static double TriangleAreaFromSides(const double a, const double b, const double c) {
   const double s = (a + b + c) / 2.0; // s is half the perimeter
   const double trialValue = s * (s - a)*(s - b)*(s - c); // Heron's formula
   const double sign = (trialValue >= 0.0) ? 1.0 : -1.0;
}

static double SqrtTriangleAreaFromSides(const double a, const double b, const double c)
{
   const double trialValue = TriangleAreaFromSides(a, b, c);
   const double sign = (trialValue >= 0.0) ? 1.0 : -1.0;
   return sign * std::sqrt(std::abs(trialValue));
}

void ComputeFaceAreasOfBravaisTetrahedron (const S6& s6in, double& a1, double& a2, double& a3, double& a4) {
   const double& s1 = s6in[0];
   const double& s2 = s6in[1];
   const double& s3 = s6in[2];
   const double& s4 = s6in[3];
   const double& s5 = s6in[4];
   const double& s6 = s6in[5];
   a1 = TriangleAreaFromSides(s1, s2, s3);
   a2 = TriangleAreaFromSides(s3, s4, s5);
   a3 = TriangleAreaFromSides(s2, s4, s6);
   a4 = TriangleAreaFromSides(s1, s5, s6);
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
      double f1, f2, f3, f4;
      const LRL_Cell reducedCell = converter.DeloneReduceCell(lattice, rcd.GetCell());
      ComputeFaceAreasOfBravaisTetrahedron(S6(reducedCell), f1, f2, f3, f4);
      std::cout << "AREAS " << f1 << " " << f2 << " " << f3 << " " << f4 << std::endl;
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
      S6Dist s6dist(10000.0);
      for (size_t i1 = 0; i1 < cellDataList.size(); ++i1) {
         ReduceAll(cellDataList[i1], s6red1, d7red1, g6red1);
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

void ListReflections(const S6& s) {
   const std::vector<S6> v = S6Dist::Generate24Reflections(s);
   std::cout << LRL_ToString(v) << std::endl;
}

void ListReflectionsByC3() {
   StoreResults<int, PairReporter<C3, MatS6> > storeMat(100);
   const std::vector<MatS6> vmR = MatS6::GetReflections();
   for (unsigned long i = 0; i < vmR.size(); ++i) {
      int index = 0;
      const S6 ord("1 2 3 4 5 6");
      const C3 c(vmR[i] * ord);
      for (unsigned long kk = 0; kk < 3; ++kk) {
         if (c[kk] == std::complex<double>(1, 4)) index += 1;
         if (c[kk] == std::complex<double>(4, 1)) index += 2;
         if (c[kk] == std::complex<double>(2, 5)) index += 10;
         if (c[kk] == std::complex<double>(5, 2)) index += 20;
         if (c[kk] == std::complex<double>(3, 6)) index += 100;
         if (c[kk] == std::complex<double>(6, 3)) index += 200;
      }
      storeMat.Store(index, PairReporter<C3, MatS6>(c, vmR[i]));

   }
   storeMat.ShowResults();
}

int main(int argc, char* argv[]) {
   //ListReflectionsByC3();
   std::cout << "main file name *** " << __FILE__ << " ***" << std::endl;

   //ListReflections(S6(" -35.78280 -90.89944 -16.76471 -22.15936 -105.64854 -94.23607"));

   //D7_BoundaryTests bt;
   //bt.MainTests();

   //TestConversions(cellDataList);
   //TREDUCE::SetDebug(true);

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
