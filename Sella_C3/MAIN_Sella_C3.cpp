// SELLA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

#include "C3.h"
#include "LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "S6.h"
#include "Sella.h"
#include "Selling.h"
#include "StoreResults.h"


std::string Letters(void) {
   return "V,G,D,S,P,A,B,C,I,F,R,C3,G6,S6,B4,D7,H";
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

std::vector<S6> GetInputSellingReducedVectors(const std::vector<LRL_ReadLatticeData>& input) {
   std::vector<S6> v;
   LatticeConverter converter;


   for (unsigned long i = 0; i < input.size(); ++i) {
      const LRL_ReadLatticeData& rcd = input[i];
      const S6 s6 = converter.SellingReduceCell(input[i].GetLattice(), input[i].GetCell());
      v.push_back(s6);
   }
   return v;
}

int seed = 19192;
GenerateRandomLattice<S6> generator(seed);

S6 GenerateRandomSphere(const double radius) {
   S6 s6;
   for (unsigned long i = 0; i < 6; ++i) {
      s6[i] = 2.0 * rhrand.urand() - 1.0;
   }
   return s6 / s6.norm();
}

S6 ScaleAndThenPerturbByPercent(const S6& s6, const double scale, const double percent) {
   const S6 master = scale * s6 / s6.norm();
   const S6 ran = GenerateRandomSphere(percent / 100.0);
   const S6 scaledRandom = scale * percent / 100.0 * ran / ran.norm();

   const double n1 = master.norm();
   const double n2 = scaledRandom.norm();
   const double n3 = (master + scaledRandom).norm();
   return master + scaledRandom;
}

void SellaLineTest(const S6& s6) {
   const S6 delta = generator.randSellingReduced();
   const unsigned long nsteps = 100;
   S6 step = 2.0 * delta / (nsteps - 1);
   step /= step.norm();
   S6 stepper = s6 + step * nsteps / 2;
   Sella sella;
   std::vector<std::vector<std::pair<std::string, double> >  > fits;

   for (unsigned long i = 0; i < nsteps; ++i) {
      S6 reducedStepper;
      const bool b = Selling::Reduce(stepper, reducedStepper);
      reducedStepper = s6.norm() * reducedStepper / reducedStepper.norm();
      const std::vector<std::pair<std::string, double> > out = sella.GetVectorOfFits(reducedStepper);
      stepper -= step;
      fits.push_back(out);

      if (i == 0) {
         for (unsigned long k = 0; k < out.size(); ++k) {
            std::cout << out[k].first << " ";
         }
      }
      for (unsigned long k = 0; k < out.size(); ++k) {
         std::cout << out[k].second << " ";
      }
      std::cout << std::endl;
   }
   std::cout << fits.size() << std::endl;
}

S6 ReduceLatticeType(const std::string& type, const S6& s6) {
   const static    LatticeConverter converter;
   return converter.SellingReduceCell(type, LRL_Cell(s6));
}

StoreResults<std::string, S6> storeProblems(10);

StoreResults<std::string, std::string> store_cP(1000);
StoreResults<std::string, std::string> store_cI(1000);
StoreResults<std::string, std::string> store_cF(1000);

std::string Trim(const std::string type) {
   for (unsigned long i = 0; i < type.size(); ++i) {
      if (type[i] == ' ') return type.substr(0, i);
   }
   return type;
}

void AnalyzePDBCells(const std::vector<LRL_ReadLatticeData>& input) {
   StoreResults<std::string, std::string> storeGood(1);
   StoreResults<std::string, std::string> storeProblems(100);
   std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
   const std::vector<S6> vLat = GetInputSellingReducedVectors(input);
   Sella sella;

   //for (unsigned long lat = 0; lat < vLat.size(); ++lat) {
   //   const std::string strCell = input[lat].GetStrCell();
   //   if (strCell[0] == 'A' || strCell[0] == 'a') continue;
   //   const std::string xtalSystem = std::string(1, strCell[strCell.length() - 1]);
   //   std::string deloneType = strCell.substr(0, 3);
   //   deloneType = Trim(deloneType);
   //   if (xtalSystem == "a") continue;

   //   const std::pair<std::string, double> fit = sella.GetBestFitForCrystalSystem(xtalSystem, vLat[lat]);
   //   const double normlat = vLat[lat].norm();
   //   if (fit.second > 1.0E-5) storeProblems.Store(fit.first, LRL_ToString(" ", fit.first, "  ", fit.second, " (", normlat, ")  ", strCell, "  ", vLat[lat]));
   //   if (fit.second < 1.0E-5) storeGood.Store(fit.first, LRL_ToString(" ", fit.first, " ", fit.second, "  ", strCell, "  ", vLat[lat]));
   //   //     std::cout << LRL_ToString(fit.first, "  ") << LRL_ToString( LRL_ToString(" ", fit.first, " ", fit.second, "  ", strCell, "  ", vLat[lat])) << std::endl;
   //}
   //storeProblems.ShowResults();
   //storeGood.ShowResults();
   exit(0);
}

int main()
{
   Sella sella;

   const std::vector<LRL_ReadLatticeData> input = GetInputCells();
   std::vector<S6> vLat = GetInputSellingReducedVectors(input);

   S6 vL;
   for ( unsigned long i=0; i<vLat.size(); ++i ) {
      const bool b = Selling::Reduce(vLat[i], vL);
      std::cout << C3::ConvertToFundamentalUnit(vL) << std::endl;
   }
   //SellaTwoLatticeLineTest(vLat[0], vLat[1]);
   //exit(0);
   //AnalyzePDBCells(input);
   LatticeConverter converter;

   const int  i19191 = 19191;
}
