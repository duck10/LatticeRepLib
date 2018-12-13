// SELLA_VERIFY.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "GenerateRandomLattice.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "rhrand.h"
#include "Sella.h"

#include <iostream>
#include <vector>



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
   const S6 result = master + scaledRandom;
   return result;
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

std::vector<S6> GenerateOrthorhombicTestCells(const std::string& type) {
   Sella sella;
   std::vector<S6> v;
   unsigned long iy, iz;
   const double x0 = 10.0;
   const double y0 = 0.1;
   const double z0 = 0.1;
   const double ydel = 1.3;
   const double zdel = 1.3;

   double y, z;

   for (iy = 0; iy < 40; ++iy) {
      y = y0 + iy * ydel;
      for (iz = 0; iz < 40; ++iz) {
         z = z0 + iz * zdel;
         const LRL_Cell cell(x0, y, z, 90, 90, 90);
         const S6 s6(cell);
         const S6 red = ReduceLatticeType(type, s6);
         const std::pair<std::string, double> best = sella.GetBestFitForCrystalSystem("O", red);
         const int i19191 = 19191;
         storeProblems.Store(best.first, red);
      }
   }
   storeProblems.ShowResults();
   return v;
   //exit(0);
}

StoreResults<std::string, std::string> store_cP(1000);
StoreResults<std::string, std::string> store_cI(1000);
StoreResults<std::string, std::string> store_cF(1000);

void GenerateCubicTest() {
   S6 s6;
   Sella sella;

   std::vector<S6> v;
   long iy, iz;
   const double x0 = 0.0;
   const double ydel = 1.0;
   const double zdel = 1.0;
   const double y0 = -2 * ydel;
   const double z0 = -2 * zdel;

   s6[0] = 0.0;
   double& y = s6[1];
   double& z = s6[2];
   s6[3] = -100.0;
   s6[4] = -100.0;
   s6[5] = -100.0;

   const double range = 40;
   int rowcount = 0;
   int colcount = 0;
   for (iy = -ydel; iy < range; iy += ydel) {
      y = y0 + iy + ydel;
      colcount = 0;
      ++rowcount;
      for (iz = -zdel; iz < range; iz += zdel) {
         z = z0 + iz + zdel;

         const S6 redP = ReduceLatticeType("p", s6);
         const S6 redI = ReduceLatticeType("i", s6);
         const S6 redF = ReduceLatticeType("f", s6);
         const double bestP = sella.GetFitForDeloneType("C5", redP);
         const double bestI = sella.GetFitForDeloneType("C1", redI);
         const double bestF = sella.GetFitForDeloneType("C3", redF);
         std::cout << bestP << " ";
         //std::cout << iy << " " << iz << "  " << bestP << "   " <<  s6 << "    "<< redP << std::endl;

 /*        store_cP.Store("C1",  LRL_ToString( " ", bestP, "   ", redP));
         store_cI.Store("C3",  LRL_ToString( " ", bestI, "   ", redI));
         store_cF.Store("C5",  LRL_ToString( " ", bestF, "   ", redF));*/


      }
      std::cout << std::endl;
   }
   //store_cP.ShowResults();
   //store_cI.ShowResults();
   //store_cF.ShowResults();
   //exit(0);

}

std::string Trim(const std::string type) {
   for (unsigned long i = 0; i < type.size(); ++i) {
      if (type[i] == ' ') return type.substr(0, i);
   }
   return type;
}

void SellaTwoLatticeLineTest(const S6& s1, const S6& s2) {
   const S6& start(s1);
   const S6& target(s2);
   const unsigned long nsteps = 100;
   S6 step = (target - start) / (nsteps);
   Sella sella;
   std::vector<std::vector<std::pair<std::string, double> >  > fits;

   for (unsigned long i = 0; i <= nsteps; ++i) {
      S6 reducedStepper;
      S6 stepper = start + step * i;
      const bool b = Selling::Reduce(stepper, reducedStepper);
      const std::vector<std::pair<std::string, double> > out = sella.GetVectorOfFits(reducedStepper);
      stepper += step;
      fits.push_back(out);

      if (i == 0) {
         for (unsigned long k = 0; k < out.size(); ++k) {
            std::cout << out[k].first << " ";
         }
         std::cout << std::endl;
      }
      for (unsigned long k = 0; k < out.size(); ++k) {
         std::cout << out[k].second << " ";
      }
      std::cout << std::endl;
   }
   std::cout << fits.size() << std::endl;
}

void M1A_CircleTest() {
   const LRL_Cell cell("10 20 15 90 110 90");
   const S6 centeredS6(cell);
   std::cout << centeredS6 << std::endl;
   S6 circle("0 0 0 0 0 0 ");

   LatticeConverter converter;
   Sella sella;
   const long steps = 101;
   const double radius = 0.1;
   for (long i = 0; i <= steps; ++i) {
      const double angle = double(i) / steps * 2.0*4.0*atan(1.0);
      circle[0] = radius * cos(angle);
      circle[2] = radius * sin(angle);
      const S6 next = centeredS6 + circle;
      const S6 s6 = converter.SellingReduceCell("C", next);
      //const std::pair<std::string, double> best = sella.GetBestFitForCrystalSystem("M2A", s6);
      const double best = sella.GetFitForDeloneType("M1A", s6);
      //std::cout << best.second << std::endl;
      std::cout << best * cos(angle) << "  " << best * sin(angle) << "   " << best << std::endl;
   }

   //exit(0);
}

void M2B_CircleTest() {
   const LRL_Cell cell("20 10 15 90 110 90");
   const S6 centeredS6(cell);
   std::cout << centeredS6 << std::endl;
   S6 circle("0 0 0 0 0 0 ");

   LatticeConverter converter;
   Sella sella;
   const long steps = 101;
   const double radius = 0.1;
   for (long i = 0; i <= steps; ++i) {
      const double angle = double(i) / steps * 2.0*4.0*atan(1.0);
      circle[0] = radius * cos(angle);
      circle[2] = radius * sin(angle);
      const S6 next = centeredS6 + circle;
      const S6 s6 = converter.SellingReduceCell("C", next);
      //const std::pair<std::string, double> best = sella.GetBestFitForCrystalSystem("M2A", s6);
      const double best = sella.GetFitForDeloneType("M2A", s6);
      //std::cout << best.second << std::endl;
      std::cout << best * cos(angle) << "  " << best * sin(angle) << "   " << best << std::endl;
   }

   //exit(0);
}

void M1B_CircleTest() {
   const LRL_Cell cell(" 89.6 75.6 69.7 90 141.9 90");  //  PDB 1SPG
   const S6 centeredS6(cell);
   std::cout << centeredS6 << std::endl;
   S6 circle("0 0 0 0 0 0 ");

   LatticeConverter converter;
   Sella sella;
   const long steps = 101;
   const double radius = 125.0;
   for (long i = 0; i <= steps; ++i) {
      const double angle = double(i) / steps * 2.0*4.0*atan(1.0);
      circle[0] = radius * cos(angle);
      circle[2] = radius * sin(angle);
      const S6 s6 = converter.SellingReduceCell("C", centeredS6 + circle);
      const double best = sella.GetFitForDeloneType("M1B", s6);
      std::cout << best * cos(angle) << "  " << best * sin(angle) << "   " << best << std::endl;
   }

   //exit(0);
}

std::vector<unsigned long> FindZeros(const S6& s6) {
   std::vector<unsigned long> v;
   for (unsigned long i = 0; i < 6; ++i)
      if (s6[i] == 0.0) v.push_back(i);
   return v;
}

std::vector<unsigned long> FindEqualPairs(const S6& s6) {
   std::vector<unsigned long> v;
   for (unsigned long i = 0; i < 5; ++i)
      for (unsigned long k = i + 1; k < 6; ++k)
         if (s6[i] == s6[k]) {
            v.push_back(i);
            v.push_back(k);
         }
   return v;
}

void Nullspace2(const std::string& strcell, const std::string& latticetype, const std::string& label, const double radius ) {
   LRL_ReadLatticeData rld;
   rld.CellReader(strcell);
   const LRL_Cell cell = rld.GetCell();

   const S6 centeredS6(cell);
   std::cout << centeredS6 << std::endl;

   S6 circle("0 0 0 0 0 0 ");

   LatticeConverter converter;
   Sella sella;
   const long steps = 101;
   for (long i = 0; i <= steps; ++i) {
      const double angle = double(i) / steps * 2.0*4.0*atan(1.0);
      circle[0] = radius * cos(angle);
      circle[2] = radius * sin(angle);
      const S6 next = centeredS6 + circle;
      const S6 s6 = converter.SellingReduceCell(latticetype, next);
      //const std::pair<std::string, double> best = sella.GetBestFitForCrystalSystem(label, s6);
      //std::cout << best.second << std::endl;
      const double best = sella.GetFitForDeloneType(label, s6);
      //std::cout << best * cos(angle) << "  " << best * sin(angle) << "   " << best << std::endl;
      std::cout << " " << best * cos(angle) << " " << best * sin(angle) << std::endl;
   }

   //exit(0);
}

void Nullspace2Tests() {
   //Nullspace2("P 10 20 15 90 110 90", "P", "M4", 0.1);
   //Nullspace2("C 10 20 15 90 110 90", "C", "M1A",0.1);
   //Nullspace2("C 89.6 75.6 69.7 90 141.9 90", "C", "M1B",0.1);
   Nullspace2("C 20 10 15 90 110 90",         "C", "M2A",0.1);
   //M1B_CircleTest();
   //M2B_CircleTest();
   //M1A_CircleTest();
}

int main()
{
   Nullspace2Tests();

   //GenerateCubicTest();
   //GenerateOrthorhombicTestCells("I");
   const std::vector<LabeledSellaMatrices> prjs = Sella::CreateAllPrjs();
   for (unsigned long i = 0; i < prjs.size(); ++i) {
      std::cout << prjs[i].GetLabel() << "  " << prjs[i].size() << std::endl;
   }

   Sella sella;

   //const std::vector<LRL_ReadLatticeData> input = GetInputCells();
   //std::vector<S6> vLat = GetInputSellingReducedVectors(input);
   //SellaTwoLatticeLineTest(vLat[0], vLat[1]);
   exit(0);
}
