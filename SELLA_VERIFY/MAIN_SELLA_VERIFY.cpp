// SELLA_VERIFY.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "GenerateRandomLattice.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "rhrand.h"
#include "Sella.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>


std::map<std::string, int> theMap;


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
   const long x0 = 0;
   const long ydel = 1;
   const long zdel = 1;
   const long y0 = -2 * ydel;
   const long z0 = -2 * zdel;

   s6[0] = 0.0;
   double& y = s6[1];
   double& z = s6[2];
   s6[3] = -100.0;
   s6[4] = -100.0;
   s6[5] = -100.0;

   const double range = 40;
   int rowcount = 0;
   int colcount = 0;
   for (long iy = -ydel; iy < range; iy += ydel) {
      y = y0 + iy + ydel;
      colcount = 0;
      ++rowcount;
      for (long iz = -zdel; iz < range; iz += zdel) {
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

std::vector<unsigned long> FindQuartets(const S6& s6) {
   int count = 0;
   unsigned long i, k;
   double vi;
   for (i = 0; i < 3; ++i) {
      for (k = i + 1; k < 6; ++k) {
         vi = s6[i];
         if (s6[k] == s6[i]) ++count;
         if (count == 4) break;
      }
   }

   std::vector<unsigned long> vout;
   for (i = 0; i < 6; ++i) {
      if (vi == s6[i]) vout.push_back(i);
   }

   return vout;
}

std::vector< std::vector<std::pair<unsigned long, unsigned long> > >FindTripets(const S6& s6) {
   int count = 0;
   unsigned long i, k;
   double vi;
   for (i = 0; i < 3; ++i) {
      for (k = i + 1; k < 6; ++k) {
         vi = s6[i];
         if (s6[k] == s6[i]) ++count;
         if (count == 3) break;
      }
   }

   std::vector<unsigned long> vout;
   for (i = 0; i < 6; ++i) {
      if (vi == s6[i]) vout.push_back(i);
   }

   return vout;
}
//
//std::vector<std::vector<std::pair<unsigned long, unsigned long> > > FindTriplets(const S6& s6) {
//
//}

std::vector<unsigned long> FindZeros(const S6& s6) {
   std::vector<unsigned long> v;
   for (unsigned long i = 0; i < 6; ++i)
      if (abs(s6[i]) < 1.0E-10) v.push_back(i);
   return v;
}

std::vector<std::pair<unsigned long,unsigned long> >FindEqualNonZeroPairs(const S6& s6) {
   std::vector<std::pair<unsigned long, unsigned long> > v;
   for (unsigned long i = 0; i < 5; ++i)
      for (unsigned long k = i + 1; k < 6; ++k)
         if ( abs(s6[i]) > 1.0E-10 && abs(s6[i] -s6[k]) < 1.0E-6) v.push_back(std::make_pair(i,k));
   return v;
}

void Nullspace3(const std::string& strcell, const std::string& latticetype, const std::string& label, const double radius) {

}

void Nullspace2(const std::string& strcell, const std::string& latticetype, const std::string& label, const double radius ) {
   LRL_ReadLatticeData rld;
   rld.CellReader(strcell);
   const LRL_Cell cell = rld.GetCell();

   const S6 centeredS6(cell);
   std::cout << centeredS6 << "  " << latticetype << "  "  << label << std::endl;

   S6 circle("0 0 0 0 0 0 ");

   LatticeConverter converter;
   Sella sella;
   const long steps = 101;
   const std::vector<unsigned long> vzeros = FindZeros(centeredS6); 
   for (long i = 0; i <= steps; ++i) {
      const double angle = double(i) / steps * 2.0*4.0*atan(1.0);
      circle[vzeros[0]] = radius * cos(angle);
      circle[vzeros[1]] = radius * sin(angle);
      const S6 next = centeredS6 + circle;
      const S6 s6primitive = converter.SellingReduceCell(latticetype, centeredS6);
      const S6 s6 = converter.SellingReduceCell(latticetype, next);
      //const std::pair<std::string, double> best = sella.GetBestFitForCrystalSystem(label, s6);
      //std::cout << best.second << std::endl;
      const double best = sella.GetFitForDeloneType(label, s6);
      //std::cout << best * cos(angle) << "  " << best * sin(angle) << "   " << best << std::endl;
      std::cout << " " << best * cos(angle) << "  " << best * sin(angle) << "         " << s6 << std::endl;
   }

   //exit(0);
}

void Nullspace2Tests() {
   Nullspace2("P 10 20 15 90 110 90", "P", "M4", 0.1);
   Nullspace2("C 10 20 15 90 110 90", "C", "M1A",0.1);
   Nullspace2("C 89.6 75.6 69.7 90 141.9 90", "C", "M1B",0.1);
   Nullspace2("C 20 10 15 90 110 90",         "C", "M2A",0.1);
   //Nullspace2("S -173.696979 -23.696979 -51.30302 -50 0  -50",         "P", "M2A",0.1);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool ParseArgs(const int argc, char* argv[], std::string& sRet, std::vector<double>& vRet)
//---------------------------------------------------------------------
{
   if (argc <= 1)
      return(false);


   std::string s(argv[1]);
   if (theMap[s] > argc - 2)
      return(false);

   std::transform(s.begin(), s.end(), s.begin(), toupper);

   sRet = s;

   for (int i = 2; i <= argc - 1; ++i)
   {
      vRet.push_back(atof(argv[i]));
   }

   return(true);
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

void FillZeros(const S6& s6in, const double cirilepart, const std::vector<unsigned long>& vzeros ) {
   S6 s6(s6in);
}

S6 MakeCircle(const std::vector<unsigned long>& vzeros, const Vector_3& v,
   const std::vector<std::pair<unsigned long, unsigned long> >& vpairs ) {

   unsigned long vpos = 0;
   S6 circle("0 0 0  0 0 0");
   for (unsigned long i = 0; i < vzeros.size(); ++i) {
      circle[vzeros[i]] = v[vpos];
      ++vpos;
   }

   for (unsigned long i = 0; i < vpairs.size(); ++i) {
      circle[vpairs[i].first] = v[vpos]/2.0;
      circle[vpairs[i].second] = -v[vpos]/2.0;
      ++vpos;
   }

   return circle;
}

void Test_CenteredNullspace2And3(
   const std::string& deloneVariety,
   const std::string& latticeCentering,
   const S6& s6cell,
   const std::vector<Vector_3>& v) {
   Sella sella;
   //expect a vector_3 file from DataGen Circle, z=0
   const S6 s6 = s6cell;
   const std::vector<unsigned long> vzeros = FindZeros(s6);
   const std::vector<std::pair<unsigned long, unsigned long> > vpairs = FindEqualNonZeroPairs(s6);
   const double radius = v[0].Norm();
   std::cout << std::endl << deloneVariety << "  " << latticeCentering << std::endl << std::endl;

   S6 red;

   for (unsigned int i = 0; i < v.size(); ++i)
   {
      const S6 circle = MakeCircle(vzeros, v[i], vpairs);
      //const bool b = Selling::Reduce(s6 + circle, red);
      const S6 primitive = ReduceLatticeType(latticeCentering, s6 + circle);
      const double best = sella.GetFitForDeloneType(deloneVariety, primitive);
      const Vector_3 vplot = best / radius * v[i];
      std::cout << best / radius * v[i][0] << ",  " << best / radius * v[i][1] << ", 0" << std::endl;
   }
}

void GeneralTests() {
   Nullspace2Tests();

   //GenerateCubicTest();
   //GenerateOrthorhombicTestCells("I");
   const std::vector<LabeledSellaMatrices> prjs = Sella::CreateAllPrjs();
   for (unsigned long i = 0; i < prjs.size(); ++i) {
      std::cout << prjs[i].GetLabel() << "  " << prjs[i].size() << std::endl;
   }

   Sella sella;

   exit(0);
}

void Test_PrimitiveNullspace2(const std::string& label, const S6& s6cell) {
   Sella sella;
   //expect a vector_3 file from DataGen Circle, z=0
   const std::vector<Vector_3> v = ReadVector_3File(std::cin);
   const S6 s6 = s6cell;
   const std::vector<unsigned long> vzeros = FindZeros(s6);
   const std::vector<std::pair<unsigned long, unsigned long> > vpairs = FindEqualNonZeroPairs(s6);
   if (vzeros.size() + vpairs.size() != 2) {
      std::cout << "function Test_PrimitiveNullspace2 requires that the nullspace be dimension 2" << std::endl;
      throw;
   }
   S6 red;

   for (unsigned int i = 0; i < v.size(); ++i)
   {
      const S6 circle = MakeCircle(vzeros, v[i], vpairs);
      const bool b = Selling::Reduce(circle + s6, red);
      const double best = sella.GetFitForDeloneType(label, red);
      const double radius = v[i].Norm();
      const Vector_3 vplot = best / radius * v[i];
      std::cout << best / radius * v[i][0] << ",  " << best / radius * v[i][1] << ", 0" << std::endl;
   }
   std::cout << std::endl;
}

void Test_PrimitiveNullspace2(const std::string& label, const std::string& strcell) {
   Test_PrimitiveNullspace2(label, LRL_Cell(strcell));
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main(int argc, char* argv[])
//---------------------------------------------------------------------
{
   FindQuartets(S6("0 1 1 3 1 5"));
   theMap.insert(std::make_pair("M4", 1));

   std::string cmd; 
   std::vector<double> vArgs;
   ParseArgs(argc, argv, cmd, vArgs);

   if (theMap.find(cmd) == theMap.end()) {
      GeneralTests();
   }
   else if ( cmd == "2D") {
      const std::vector<Vector_3> v = ReadVector_3File(std::cin);

      Test_CenteredNullspace2And3("M4" , "P", S6(LRL_Cell("10 20 15 90 110 90")),v);
      Test_CenteredNullspace2And3("M1A", "C", S6(LRL_Cell("10 20 15 90 110 90")),v);
      Test_CenteredNullspace2And3("M1B", "C", S6(LRL_Cell("89.6 75.6 69.7 90 141.9 90")),v);
      Test_CenteredNullspace2And3("M2A", "C", S6(LRL_Cell("20 10 15 90 110 90")), v);

      Test_CenteredNullspace2And3("M1A", "P", S6("-24.35 -24.35 -75 -25.65 -25.65 -173.68687"), v); 
      Test_CenteredNullspace2And3("M1B", "P", S6("-978.63 -1879.05 -578.2 -978.63-1879.05 -521.790"), v); //  C121 1SPG 
      Test_CenteredNullspace2And3("M2A", "P", S6("-173.696979 -23.696979 -51.30302 -50 0  -50"), v);
   }
   else if (cmd == "3D") {
      const std::vector<Vector_3> v = ReadVector_3File(std::cin);

      Test_CenteredNullspace2And3("O5", "P", S6(LRL_Cell("10 20 15 90 90 90")), v);  // 
      Test_CenteredNullspace2And3("O4", "C", S6(LRL_Cell("22.742 58.341 23.669 90 90 90")), v);  // 3T86
      Test_CenteredNullspace2And3("O2", "F", S6(LRL_Cell("185.8 185.8 186.5 90 90 90")), v);  // 1KK5
      //Test_CenteredNullspace2And3("O1A", "F", S6(LRL_Cell("59.94 81.33 26.86 90 90 90")), v);  // 200D // needs circle from quartet
      Test_CenteredNullspace2And3("O1B", "I", S6(LRL_Cell("42.546 53.084 64.357 90 90 90")), v);  // 3CP0

      //O5   0    P 27.546 31.017 4.804 90 90 90 P21212 3NVF    o      0 0 0 -758.78212 -962.05429 -23.07842
      //O4   0    C 22.742 58.341 23.669 90 90 90 C222 3T86     o -258.59928  -258.59928 -721.61843 0 0 -560.22156
      //O4   0    C 22.532 58.469 24.148 90 90 90 C222 1QYL     o -253.84551  -253.84551 -727.73323 0 0 -583.12590
      //O2   0    F 185.8 185.8 186.5 90 90 90 F222 1KK5        o -8630.41000 -8630.41000 -65.15250 -8630.41000 -8630.41000   0
      //O1A   0    F 59.94 81.33 26.86 90 90 90 F222 200D       o -1473.27732 -180.36490 -180.36490 -717.83600 -180.36490 -180.36490
      //O1B   0    I 42.546 53.084 64.357 90 90 90 I212121 3CP0 o -783.51863  -121.56243 -1287.39310 -783.51863 -121.56243 -1287.39310
   }
   exit(0);
}
