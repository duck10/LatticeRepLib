// Follow.cpp : Defines the entry point for the console application.
//

#pragma once

#include <vector>

#include "stdafx.h"

#include "LRL_Cell.h"
#include "Delone.h"
#include "D7.h"
#include "D7Dist.h"
#include "LatticeConverter.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "S6Dist.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "S6.h"
#include "Selling.h"


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
void ReduceAll(const LRL_ReadLatticeData& cellDataList, S6& s6, D7& d7, G6& g6) {
   LatticeConverter converter;
   g6 = converter.NiggliReduceCell  (cellDataList.GetLattice(), cellDataList.GetCell());
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
      const char let = char(toupper(lattice[0]));
      if ((!lattice.empty()) && letters.find(let) != std::string::npos)
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

void FollowOneChanging(const S6& s1, const S6& s2) {
   S6 s1red;
   S6 s2red;
   S6Dist s6dist(50000);
   const unsigned long npts = 100;
   const double delta = (s1 - s2).norm() / (npts - 1);
   const bool b1 = Selling::Reduce(s1, s1red);
   for (unsigned long i = 0; i <= npts; ++i) {
      const double factorplus = double(i)/double(npts);
      const double factorminus = double(npts - i)/double(npts);
      const S6 step = factorminus*s1 + factorplus*s2;
      const bool b2 = Selling::Reduce(step, s2red);
      const double dist = s6dist.DistanceBetween(s1red, s2red);
      std::cout << i << "  " << dist << "   " << s1red << "   " << s2red << std::endl;
   }
}

void FollowTwoChanging(const S6& s1, const S6& s2, const S6& s3) {
   S6 s1red;
   S6 s2red;
   S6 s3red;
   S6Dist s6dist(50000);
   const unsigned long npts = 100;
   const bool b1 = Selling::Reduce(s1, s1red);
   if (!b1) throw;
   std::cout << "S1  " << s1red << std::endl;
   for (unsigned long i = 1; i <= npts; ++i) {
      const double factorplus = double(i) / double(npts);
      const double factorminus = double(npts - i) / double(npts);
      const S6 step2 = factorminus*s1 + factorplus*s2;
      const S6 step3 = factorminus*s1 + factorplus*s3;
      const bool b2 = Selling::Reduce(step2, s2red);
      if (!b2) throw;
      const bool b3 = Selling::Reduce(step3, s3red);
      if (!b3) throw;
      const double s6dist12 = s6dist.DistanceBetween(s1red, s2red) /*/ std::max(1.0E-5, factorplus)*/;
      const double s6dist13 = s6dist.DistanceBetween(s1red, s3red) /*/ std::max(1.0E-5, factorplus)*/;
      const double s6dist23 = s6dist.DistanceBetween(s2red, s3red) /*/ std::max(1.0E-5, factorplus)*/;
      const double d7dist23 = D7Dist(D7(s2red).GetVector().data(), D7(s3red).GetVector().data());
      const double triangle = ((s6dist12 + s6dist13 + s6dist23) - 2.0*maxNC(s6dist12, s6dist13, s6dist23))*100.0/ maxNC(s6dist12, s6dist13, s6dist23);
      std::cout << i << "  " << triangle << "  " << s6dist12 << "  " << s6dist13 << "  " << s6dist23 << "  " << d7dist23 << "   " << s2red << "   " << s3red << std::endl;
   }
}

int main(int argc, char* argv[]) {
   std::cout << "main file name *** " << __FILE__ << " ***" << std::endl;
   D7 d7;

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

   S6 start(cellDataList[0].GetCell());
   S6 endpt(cellDataList[1].GetCell());

   std::cout << OutputIntialInput(cellDataList) << std::endl;

   if (cellDataList.size() == 2)
      FollowOneChanging(start, endpt);
   else if (cellDataList.size() == 3)
      FollowTwoChanging(start, endpt, cellDataList[2].GetCell());
   else
      throw;

   return 0;
}

