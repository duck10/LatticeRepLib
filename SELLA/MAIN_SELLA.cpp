// SELLA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

#include "C3.h"
#include "DeloneTypeList.h"
#include "LRL_CoordinateConversionMatrices.h"
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
      if ((!lattice.empty()) && (letters.find(static_cast<char>(toupper(lattice[0]))) != std::string::npos))
         cellDataList.push_back(rcd);
   }

   return cellDataList;
}

std::vector<S6> GetInputSellingReducedVectors(const std::vector<LRL_ReadLatticeData>& input) {
   std::vector<S6> v;
   LatticeConverter converter;


   for ( unsigned long i=0; i<input.size(); ++i ) {
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

S6 ScaleAndThenPerturbByPercent( const S6& s6, const double scale, const double percent) {
   const S6 master = scale * s6 / s6.norm();
   const S6 ran = GenerateRandomSphere(percent/100.0);
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
   S6 stepper = s6 + step * nsteps/2;
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

S6 ReduceLatticeType( const std::string& type, const S6& s6) {
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
   exit(0);
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
   const double y0 = -2*ydel;
   const double z0 = -2*zdel;

   s6[0] = 0.0;
   double& y = s6[1];
   double& z = s6[2];
   s6[3] = -100.0;
   s6[4] = -100.0;
   s6[5] = -100.0;

   const double range = 40;
   int rowcount = 0;
   int colcount = 0;
   for (iy = (long)(-ydel); iy < range; iy += (long)(ydel)) {
      y = y0 + (double)(iy) + ydel;
      colcount = 0;
      ++rowcount;
      for (iz = (long)(-zdel); iz < range; iz += (long)(zdel)) {
         z = z0 + (double)(iz) + zdel;

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
   exit(0);

}

std::string Trim(const std::string type) {
   for ( unsigned long i=0; i<type.size(); ++i ) {
      if (type[i] == ' ') return type.substr(0, i);
   }
   return type;
}

double FitRange(const double f) {
   return f;
   if (f < 1.0E-6) return -6.0;
   return round(log10(f));
}

void AnalyzePDBCells(const std::vector<LRL_ReadLatticeData>& input) {
   StoreResults<std::string, std::string> storeGood(20);
   StoreResults<std::string, std::string> storeLocalProblems(100);
   StoreResults<double, std::string> storeO3(100);
   StoreResults<double, std::string> storeM3(100);
   StoreResults<double, std::string> storeM2B(100);
   storeO3.SetTitle("O3 O3 O3 O3 O3 O3 O3 O3 ");
   storeM3.SetTitle("M3 M3 M3 M3 M3 M3 M3 M3 ");
   storeM2B.SetTitle("M2B M2B M2B M2B M2B M2B ");
   std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
   const std::vector<S6> vLat = GetInputSellingReducedVectors(input);
   Sella sella;

   for (unsigned long lat = 0; lat < vLat.size(); ++lat) {
      const std::string strCell = input[lat].GetStrCell();
      if (strCell[0] == 'A' || strCell[0] == 'a') continue;
      const std::string xtalSystem = std::string(1, strCell[strCell.length() - 1]);
      std::string deloneType = strCell.substr(0, 3);
      deloneType = Trim(deloneType);
      if ( xtalSystem == "a" ) continue;

      const std::pair<std::string, double> fit = sella.GetBestFitForCrystalSystem(xtalSystem, vLat[lat]);
      const double normlat = vLat[lat].norm();
      if (fit.second > 1.0E-5) storeLocalProblems.Store(fit.first, LRL_ToString(" ", fit.first, "  ", fit.second, " (", normlat, ")  ",  strCell, "  ", vLat[lat]));
      if (fit.second < 1.0E-5) storeGood.Store(fit.first, LRL_ToString(" ", fit.first, " ", fit.second, "  ", strCell, "  ", vLat[lat]));
 //     std::cout << LRL_ToString(fit.first, "  ") << LRL_ToString( LRL_ToString(" ", fit.first, " ", fit.second, "  ", strCell, "  ", vLat[lat])) << std::endl;

      //const double fitO3  = FitRange(sella.GetFitForDeloneType("O3", vLat[lat]));
      //const double fitM3  = FitRange(sella.GetFitForDeloneType("M3", vLat[lat]));
      //const double fitM2B = FitRange(sella.GetFitForDeloneType("M2B", vLat[lat]));
      //storeO3 .Store(fitO3 , LRL_ToString("(", fitO3, "  ", strCell, "   ",vLat[lat]));
      //storeM3 .Store(fitM3 , LRL_ToString("(", fitM3, "  ", strCell, "   ",vLat[lat]));
      //storeM2B.Store(fitM2B, LRL_ToString("(", fitM2B, "  ", strCell, "   ",vLat[lat]));
   }
   storeLocalProblems.ShowResults();
   storeGood.ShowResults();

   storeO3 .ShowResults();
   storeM3 .ShowResults();
   storeM2B.ShowResults();
   exit(0);
}

void ReportFit( const unsigned long i, const DeloneType& type, const std::tuple<double, S6, MatS6>& fit ) {
   std::cout << i << "  " << type.GetName( ) << "  " << std::get<double>( fit ) << "     " << type.GetCharacter( ) <<
      std::endl << "input  " << std::get<S6>( fit ) << std::endl << std::get<MatS6>( fit ) << std::endl;
   std::cout << "result  " << std::get<MatS6>( fit ) * std::get<S6>( fit ) << std::endl;
   const MatS6 ms6 = std::get<MatS6>( fit );
   std::cout << type.GetToCentered( ) * std::get<MatS6>( fit ) * std::get<S6>( fit ) << std::endl;
   std::cout << "centered cell  " << LRL_Cell_Degrees( type.GetToCentered( ) * std::get<MatS6>( fit ) * std::get<S6>( fit ) ) << std::endl << std::endl;
   std::cout << "  "
      << type.GetName( ) 
      << "  " 
      << std::get<double>( fit ) 
      << "     " 
      << std::get<S6>( fit ) 
      << "     "
      << LRL_Cell_Degrees( std::get<S6>( fit ) )
      << std::endl;
}

void ReportFit( const unsigned long n, const DeloneTypeList& types, const std::vector<std::tuple<double, S6, MatS6> >& fit ) {
   for ( unsigned long i=0; i<fit.size(); ++i )
      ReportFit( i, types[i], fit[i] );
}

int main()
{
   const std::vector<LRL_ReadLatticeData> input = GetInputCells();
   static const DeloneTypeList deloneList;

   const std::vector<S6> vLat = GetInputSellingReducedVectors(input);

   for (unsigned long lat = 0; lat < vLat.size(); ++lat) {
      std::cout << "input  " << input[lat].GetStrCell() << "    (" << S6(input[lat].GetCell()) <<")" << std::endl;
      //const std::vector<std::tuple<double, S6, MatS6> >  v = DeloneTypeList::Fit( "T", vLat[lat] );
      const std::vector<std::tuple<double, S6, MatS6> >  v = deloneList.Fit( vLat[lat] );
      ReportFit( lat, deloneList,  v );
      std::cout << "************************************" << std::endl;
   }

   const int  i19191 = 19191;
}
