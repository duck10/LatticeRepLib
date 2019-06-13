// SELLA_VERIFY.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "ColorTables.h"
#include "GenerateRandomLattice.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "Nullspace.h"
#include "rhrand.h"
#include "Sella.h"

#include <algorithm>
#include <iostream>
#include <map>
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
      if ((!lattice.empty()) && (letters.find(static_cast<char>(toupper(lattice[0]))) != std::string::npos))
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
         const S6 red = LatticeConverter::SellingReduceCell(type, s6);
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

//void GenerateCubicTest() {
//   S6 s6;
//   Sella sella;
//
//   std::vector<S6> v;
//   const long x0 = 0;
//   const long ydel = 1;
//   const long zdel = 1;
//   const long y0 = -2 * ydel;
//   const long z0 = -2 * zdel;
//
//   s6[0] = 0.0;
//   double& y = s6[1];
//   double& z = s6[2];
//   s6[3] = -100.0;
//   s6[4] = -100.0;
//   s6[5] = -100.0;
//
//   const double range = 40;
//   int rowcount = 0;
//   int colcount = 0;
//   for (long iy = -ydel; iy < range; iy += ydel) {
//      y = y0 + iy + ydel;
//      colcount = 0;
//      ++rowcount;
//      for (long iz = -zdel; iz < range; iz += zdel) {
//         z = z0 + iz + zdel;
//
//         const S6 redP = ReduceLatticeType("p", s6);
//         const S6 redI = ReduceLatticeType("i", s6);
//         const S6 redF = ReduceLatticeType("f", s6);
//         const double bestP = sella.GetFitForDeloneType("C5", redP);
//         const double bestI = sella.GetFitForDeloneType("C1", redI);
//         const double bestF = sella.GetFitForDeloneType("C3", redF);
//         std::cout << bestP << " ";
//         //std::cout << iy << " " << iz << "  " << bestP << "   " <<  s6 << "    "<< redP << std::endl;
//
// /*        store_cP.Store("C1",  LRL_ToString( " ", bestP, "   ", redP));
//         store_cI.Store("C3",  LRL_ToString( " ", bestI, "   ", redI));
//         store_cF.Store("C5",  LRL_ToString( " ", bestF, "   ", redF));*/
//
//
//      }
//      std::cout << std::endl;
//   }
//   //store_cP.ShowResults();
//   //store_cI.ShowResults();
//   //store_cF.ShowResults();
//   //exit(0);
//
//}

std::string Trim(const std::string type) {
   for (unsigned long i = 0; i < type.size(); ++i) {
      if (type[i] == ' ') return type.substr(0, i);
   }
   return type;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ParseArgs(const int argc, char* argv[], std::vector<std::string>& sRet, std::vector<double>& vRet)
//---------------------------------------------------------------------
{
   sRet.clear();
   for (unsigned long i = 1; int(i) < argc; ++i) {
      std::string s(argv[i]);
	  for (unsigned long k = 0; k < s.length(); ++k)
		  s[k] = static_cast<char>(toupper(s[k]));
      sRet.push_back(s);
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::vector<std::string> GetNextLine(std::string& line)
//---------------------------------------------------------------------
{
   std::vector<std::string> result;

   std::stringstream lineStream(line);
   std::string       cell;

   while (std::getline(lineStream, cell, ','))
   {
      result.push_back(cell);
   }
   return(result);
}
 
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::vector<Vector_3> ReadVector_3File(std::istream& str)
//---------------------------------------------------------------------
{
   std::vector<Vector_3> v;

   while (!str.fail() && !str.eof())
   {
      std::string line;
      getline(str, line, '\n');
      if (!str.fail() && !str.eof())
      {
         std::vector<std::string> vs = GetNextLine(line);
         const Vector_3 v1(atof(vs[0].c_str()), atof(vs[1].c_str()), atof(vs[2].c_str()));
         v.push_back(v1);
      }
   }
   return(v);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main(int argc, char* argv[])
//---------------------------------------------------------------------
{
   //std::vector<S6> s6;
   //s6.push_back(S6("3 3 1 3 1 1"));
   //s6.push_back(S6("0 0 0 0 0 0"));
   //s6.push_back(S6("0 0 1 3 1 1"));
   //s6.push_back(S6("1 1 2 2 3 3"));
   //s6.push_back(S6("3 3 3 1 1 0"));
   //for (unsigned long i = 0; i < s6.size(); ++i) {
   //   std::vector<std::pair<unsigned long, unsigned long> >  pairs;
   //   std::vector<std::vector<unsigned long> > triplets;
   //   std::vector<unsigned long>  quartets;
   //   std::vector<unsigned long>  zeros;


   //   Nullspace::FindNullspace(s6[i], quartets, triplets, pairs, zeros);



   //   std::cout << std::endl << s6[i] << std::endl;
   //   std::cout << "quartets " << quartets.size() << std::endl;
   //   std::cout << "triplets " << triplets.size() << std::endl;
   //   std::cout << "pairs " << pairs.size() << std::endl;
   //   std::cout << "zeros " << zeros.size() << std::endl;
   //}

   std::vector<std::string> args;
   std::string cmd;
   std::string cmd2;
   std::vector<double> vArgs;
   ParseArgs(argc, argv, args , vArgs);
   if (argc > 1) cmd = args[0];
   if (argc > 2) cmd2 = args[1];

   if (args.empty()) {
   }
   else if ( cmd == "2D") {
      const std::vector<Vector_3> v = ReadVector_3File(std::cin);

      if ( cmd2=="" || cmd2=="M4" ) Nullspace::Test_CenteredNullspace2And3("M4" , "P", S6(LRL_Cell("10 20 15 90 110 90")),v);
      if ( cmd2=="" || cmd2=="M1A") Nullspace::Test_CenteredNullspace2And3("M1A", "C", S6(LRL_Cell("10 20 15 90 110 90")),v);
      if ( cmd2=="" || cmd2=="M1B") Nullspace::Test_CenteredNullspace2And3("M1B", "C", S6(LRL_Cell("89.6 75.6 69.7 90 141.9 90")),v);
      if ( cmd2=="" || cmd2=="M2A") Nullspace::Test_CenteredNullspace2And3("M2A", "C", S6(LRL_Cell("20 10 15 90 110 90")), v);

      if ( cmd2=="" || cmd2=="M1AP") Nullspace::Test_CenteredNullspace2And3("M1A", "P", S6("-24.35 -24.35 -75 -25.65 -25.65 -173.68687"), v); 
      if ( cmd2=="" || cmd2=="M1BP") Nullspace::Test_CenteredNullspace2And3("M1B", "P", S6("-978.63 -1879.05 -578.2 -978.63-1879.05 -521.790"), v); //  C121 1SPG 
      if ( cmd2=="" || cmd2=="M2AP") Nullspace::Test_CenteredNullspace2And3("M2A", "P", S6("-173.696979 -23.696979 -51.30302 -50 0  -50"), v);
   }
   else if (cmd == "3D") {
      const std::vector<Vector_3> v = ReadVector_3File(std::cin);

      if ( cmd2=="" || cmd2=="O5" ) Nullspace::Test_CenteredNullspace2And3("O5", "P", S6(LRL_Cell("10 20 15 90 90 90")), v);  // 
      if ( cmd2=="" || cmd2=="O4" ) Nullspace::Test_CenteredNullspace2And3("O4", "C", S6(LRL_Cell("22.742 58.341 23.669 90 90 90")), v);  // 3T86
      if ( cmd2=="" || cmd2=="O2" ) Nullspace::Test_CenteredNullspace2And3("O2", "I", S6(LRL_Cell("26.22 44.359 52.336 90 90 90")), v);  // 1S23
      if ( cmd2=="" || cmd2=="O1A") Nullspace::Test_CenteredNullspace2And3("O1A", "F", S6(LRL_Cell("59.94 81.33 26.86 90 90 90")), v);  // 200D // needs circle from quartet
      if ( cmd2=="" || cmd2=="O1B") Nullspace::Test_CenteredNullspace2And3("O1B", "I", S6(LRL_Cell("42.546 53.084 64.357 90 90 90")), v);  // 3CP0

      if ( cmd2=="" || cmd2=="O4P" ) Nullspace::Test_CenteredNullspace2And3("O4", "P", S6("-258.59928  -258.59928 -721.61843 0 0 -560.22156"), v);  // 3T86
      if ( cmd2=="" || cmd2=="O2P" ) Nullspace::Test_CenteredNullspace2And3("O2", "P", S6("-983.86044 -20.96190 -983.86044 -343.74420   0.00000 -343.74420"), v);  // 1S23
      if ( cmd2=="" || cmd2=="O1AP") Nullspace::Test_CenteredNullspace2And3("O1A", "P", S6("-1473.27732 -180.36490 -180.36490 -717.83600 -180.36490 -180.36490"), v);  // 200D // needs circle from quartet
      if ( cmd2=="" || cmd2=="O1BP") Nullspace::Test_CenteredNullspace2And3("O1B", "P", S6("-783.51863  -121.56243 -1287.39310 -783.51863 -121.56243 -1287.39310"), v);  // 3CP0
      //O5   0    P 27.546 31.017 4.804 90 90 90 P21212 3NVF    o      0 0 0 -758.78212 -962.05429 -23.07842
      //O4   0    C 22.742 58.341 23.669 90 90 90 C222 3T86     o -258.59928  -258.59928 -721.61843 0 0 -560.22156
      //O4   0    C 22.532 58.469 24.148 90 90 90 C222 1QYL     o -253.84551  -253.84551 -727.73323 0 0 -583.12590
      // xxxxxxO2   0    F 185.8 185.8 186.5 90 90 90 F222 1KK5        o -8630.41000 -8630.41000 -65.15250 -8630.41000 -8630.41000   0
      //O2   0    I 26.22 44.359 52.336 90 90 90 I212121 1S23 o -983.86044 -20.96190 -983.86044 -343.74420   0.00000 -343.74420
      //O1A   0    F 59.94 81.33 26.86 90 90 90 F222 200D       o -1473.27732 -180.36490 -180.36490 -717.83600 -180.36490 -180.36490
      //O1B   0    I 42.546 53.084 64.357 90 90 90 I212121 3CP0 o -783.51863  -121.56243 -1287.39310 -783.51863 -121.56243 -1287.39310
   }

   else if (cmd == "4D" ) {
   
   }

   exit(0);
}
