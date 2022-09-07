// SELLA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include "BravaisHeirarchy.h"
#include "FileOperations.h"

#include "GenerateLatticeTypeExamples.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_CoordinateConversionMatrices.h"
#include "LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "MatG6.h"
#include "MatS6.h"
#include "Niggli.h"
#include "S6.h"
#include "Sella.h"
#include "Selling.h"
#include "StoreResults.h"


static const std::string Letters(void) {
   return "V,G,D,S,P,A,B,C,I,F,R,C3,G6,S6,B4,D7,H";
}

static const double g_maxDeltaForMatch = 0.02;

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


std::vector<S6> GetInputSellingReducedVectors( const std::vector<LRL_ReadLatticeData>& input ) {
   std::vector<S6> v;
   LatticeConverter converter;

   for (size_t i = 0; i < input.size( ); ++i) {
      const S6 s6 = converter.SellingReduceCell( input[i].GetLattice( ), input[i].GetCell( ) );
      v.push_back( s6 );
   }
   return v;
}

std::vector<S6> GetInputSellingReducedVectors( const std::vector<LRL_ReadLatticeData>& input, std::vector<MatS6>& vmat ) {
   std::vector<S6> v;
   LatticeConverter converter;
   MatS6 m;
   vmat.clear( );

   for (size_t i = 0; i < input.size( ); ++i) {
      const S6 s6 = converter.SellingReduceCell( input[i].GetLattice( ), input[i].GetCell( ), m );
      v.push_back( s6 );
      vmat.push_back( m );
   }

   return v;
}


S6 ReduceLatticeType( const std::string& type, const S6& s6) {
   const static    LatticeConverter converter;
   return converter.SellingReduceCell(type, LRL_Cell(s6));
}

StoreResults<std::string, S6> storeProblems(10);

StoreResults<std::string, std::string> store_cP(1000);
StoreResults<std::string, std::string> store_cI(1000);
StoreResults<std::string, std::string> store_cF(1000);

std::string Trim(const std::string type) {
   for ( size_t i=0; i<type.size(); ++i ) {
      if (type[i] == ' ') return type.substr(0, i);
   }
   return type;
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

   for (size_t lat = 0; lat < vLat.size( ); ++lat) {
      const std::string strCell = input[lat].GetStrCell( );
      if (strCell[0] == 'A' || strCell[0] == 'a') continue;
      const std::string xtalSystem1 = std::string( 1, strCell[strCell.length( ) - 1] );
      std::string deloneType = strCell.substr( 0, 3 );
      deloneType = Trim( deloneType );
      if (xtalSystem1 == "a") continue;
      /*for (size_t type = 0; type < typelist.size( ); ++type) */{
         //const std::string& xtalSystem2 = typelist[type];
         const std::pair<std::string, double> fit = sella.GetBestFitForCrystalSystem( xtalSystem1, vLat[lat] );
         //const std::vector<std::pair<std::string, double> > fit = sella.GetVectorOfFits( vLat[lat] );
         const double normlat = vLat[lat].norm( );
         if (fit.second > 1.0E-5) storeLocalProblems.Store( fit.first, LRL_ToString( " ", fit.first, "  ", fit.second, " (", normlat, ")  ", strCell, "  ", vLat[lat] ) );
         if (fit.second < 1.0E-5) storeGood.Store( fit.first, LRL_ToString( " ", fit.first, " ", fit.second, "  ", strCell, "  ", vLat[lat] ) );
         //std::cout << LRL_ToString( fit.first, "  " ) << LRL_ToString( LRL_ToString( " ", fit.first, " ", fit.second, "  ", strCell, "  ", vLat[lat] ) ) << std::endl;
      }
   }
   storeLocalProblems.ShowResults( );
   storeGood.ShowResults();
   exit(0);
}
//void ReportFit(const size_t n, const DeloneTypeList& types, const std::vector<DeloneFitResults>& fit) {
//   std::cout << "*******   SELLA report for input cell " << n << std::endl;
//   std::vector<SellaResult> vsr;
//   for (size_t i = 0; i < fit.size(); ++i)
//      vsr.push_back(ReportFit2(i, types[i], fit[i]));
//
//   for (size_t i = 0; i < vsr.size(); ++i)
//      std::cout << vsr[i] << std::endl;
//   std::cout << "*******   END SELLA report for input cell " << n << std::endl;
//
//}

static double sq( const double d ) { return d * d; }

LRL_Cell CellErrorsFromS6(const S6& s, const S6& sig) {
   LRL_Cell cer;
   double& da = cer[0];
   double& db = cer[1];
   double& dc = cer[2];
   double& dal = cer[3];
   double& dbe = cer[4];
   double& dga = cer[5];

   const double& ds1 = sig[0];
   const double& ds2 = sig[1];
   const double& ds3 = sig[2];
   const double& ds4 = sig[3];
   const double& ds5 = sig[4];
   const double& ds6 = sig[5];

   const double& s1 = s[0];
   const double& s2 = s[1];
   const double& s3 = s[2];
   const double& s4 = s[3];
   const double& s5 = s[4];
   const double& s6 = s[5];

   da = sqrt( 0.25 * (sq( -ds4 ) - sq( ds3 ) - sq( ds2 )) / (-s4 - s3 - s2) );
   db = sqrt( 0.25 * (sq( -ds5 ) - sq( ds3 ) - sq( ds1 )) / (-s5 - s3 - s1) );
   dc = sqrt( 0.25 * (sq( -ds6 ) - sq( ds2 ) - sq( ds1 )) / (-s6 - s2 - s1) );

   const double aas = -s4 - s3 - s2; // this is a^2
   const double bas = -s5 - s3 - s1;
   const double cas = -s6 - s2 - s1;
   const double sqrta = sqrt( aas ); // this is just a
   const double sqrtb = sqrt( bas ); // this is just b
   const double sqrtc = sqrt( cas ); // this is just c

   double dsqrta = -0.5 / (sqrta * sqrta * sqrta) * sqrt(sq( ds4 ) + sq( ds3 ) + sq( ds2 ));
   double dsqrtb = -0.5 / (sqrtb * sqrtb * sqrtb) * sqrt(sq( ds5 ) + sq( ds3 ) + sq( ds1 ));
   double dsqrtc = -0.5 / (sqrtc * sqrtc * sqrtc) * sqrt(sq( ds6 ) + sq( ds2 ) + sq( ds1 ));

   double tds1[6] = {
   0,
   -ds1 / (2 * sqrt( (-s5) - s3 - s1 )),

   -ds1 / (2 * sqrt( (-s6) - s2 - s1 )),

   -(ds1 * (1 / (sqrt( (-s5) - s3 - s1 ) * sqrt( (-s6) - s2 - s1 ))
      + s1 / (2 * pow( ((-s5) - s3 - s1) , 1.5 ) * sqrt( (-s6) - s2 - s1 ))
      + s1 / (2 * sqrt( (-s5) - s3 - s1 ) * pow( ((-s6) - s2 - s1) , 1.5 ))))
   / sqrt( 1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1)) ),

   -(ds1 * s2) / (2 * sqrt( (-s4) - s3 - s2 )
      * sqrt( 1 - s2 * s2 / (((-s4) - s3 - s2) * ((-s6) - s2 - s1)) )
      * pow( ((-s6) - s2 - s1) , 1.5 )),


   -(ds1 * s3) / (2 * sqrt( (-s4) - s3 - s2 )
      * sqrt( 1 - s3 * s3 / (((-s4) - s3 - s2) * ((-s5) - s3 - s1)) )
      * ((-s5) - s3 - s1) , 1.5) };



   // ds2 ---------------------------------------------------------------------

double tds2[6] = { 
   -ds2 / (2 * sqrt( (-s4) - s3 - s2 )),

   0,

   -ds2 / (2 * sqrt( (-s6) - s2 - s1 )),

   -(ds2 * s1) / (2 * sqrt( (-s5) - s3 - s1 )
      * sqrt( 1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1)) )
      * pow( ((-s6) - s2 - s1) , 1.5 )),

   -(ds2 * (1 / (sqrt( (-s4) - s3 - s2 ) * sqrt( (-s6) - s2 - s1 ))
      + s2 / (2 * pow( ((-s4) - s3 - s2) , 1.5 ) * sqrt( (-s6) - s2 - s1 ))
      + s2 / (2 * sqrt( (-s4) - s3 - s2 ) * pow( ((-s6) - s2 - s1) , 1.5 ))))
    /sqrt( 1 - s2 * s2 / (((-s4) - s3 - s2) * ((-s6) - s2 - s1)) ),

   -(ds2 * s3) / (2 * pow( ((-s4) - s3 - s2) , 1.5 )
      * sqrt( 1 - s3 * s3 / (((-s4) - s3 - s2) * ((-s5) - s3 - s1)) )
      * sqrt( (-s5) - s3 - s1 )) };
// ds3 ---------------------------------------------------------------------

double tds3[6] = {
   -ds3 / (2 * sqrt( (-s4) - s3 - s2 )), 
   -ds3 / (2 * sqrt( (-s5) - s3 - s1 )), 
   0,
   -(ds3 * s1) / (2 * pow( ((-s5) - s3 - s1) , 1.5 )
      * sqrt( 1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1)) )
      * sqrt( (-s6) - s2 - s1 )),

      -(ds3 * s2) / (2 * pow( ((-s4) - s3 - s2) , 1.5 )
   * sqrt( 1 - s2 * s2 / (((-s4) - s3 - s2) * ((-s6) - s2 - s1)) )
   * sqrt( (-s6) - s2 - s1 )),

   -(ds3 * (1 / (sqrt( (-s4) - s3 - s2 ) * sqrt( (-s5) - s3 - s1 ))
      + s3 / (2 * pow( ((-s4) - s3 - s2) , 1.5 ) * sqrt( (-s5) - s3 - s1 ))
      + s3 / (2 * sqrt( (-s4) - s3 - s2 ) * pow( ((-s5) - s3 - s1) , 1.5 ))))
      / sqrt( 1 - s3 * s3 / (((-s4) - s3 - s2) * ((-s5) - s3 - s1)) ) };

      // ds4 ---------------------------------------------------------------------

double tds4[6] = {
 -ds4 / (2 * sqrt( (-s4) - s3 - s2 )), 
 0, 
 0, 
 0,

   -(ds4 * s2) / (2 * pow( ((-s4) - s3 - s2) , 1.5)
      * sqrt( 1 - s2 *s2 / (((-s4) - s3 - s2) * ((-s6) - s2 - s1)) )
      * sqrt( (-s6) - s2 - s1 )),

   -(ds4 * s3) / (2 * pow( ((-s4) - s3 - s2) , 1.5)
      * sqrt( 1 - s3 *s3 / (((-s4) - s3 - s2) * ((-s5) - s3 - s1)) )
      * sqrt( (-s5) - s3 - s1 ))};

      // ds5 ---------------------------------------------------------------------

double tds5[6] = {
   0, 
   -ds5 / (2 * sqrt( (-s5) - s3 - s1 )), 
   0,
   -(ds5 * s1) / (2 * pow( ((-s5) - s3 - s1) , 1.5)
      * sqrt( 1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1)) )
      * sqrt( (-s6) - s2 - s1 )),
   0,
   -(ds5 * s3) / (2 * sqrt( (-s4) - s3 - s2 )
      * sqrt( 1 - s3 * s3 / (((-s4) - s3 - s2) * ((-s5) - s3 - s1)) )
      * pow( ((-s5) - s3 - s1) , 1.5)) };

      // ds6 ---------------------------------------------------------------------

double tds6[6] = {
   0, 
   0, 
   -ds6 / (2 * sqrt( (-s6) - s2 - s1 )),
   -(ds6 * s1) / (2 * sqrt( (-s5) - s3 - s1 )
      * sqrt( 1 - s1 *s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1)) )
      * pow( ((-s6) - s2 - s1) , 1.5)),
   -(ds6 * s2) / (2 * sqrt( (-s4) - s3 - s2 )
      * sqrt( 1 - s2 *s2 / (((-s4) - s3 - s2) * ((-s6) - s2 - s1)) )
      * pow( ((-s6) - s2 - s1), 1.5 )), 
   0};

for (size_t i = 0; i < 6; ++i) {
   cer[i] = sqrt( sq( tds1[i] ) + sq( tds2[i] ) + sq( tds3[i] ) + sq( tds4[i] ) + +sq( tds5[i] ) + sq( tds6[i] ) );
}

for (size_t i = 0; i < 6; ++i) {
   std::cout << ( tds1[i] ) << "  " << ( tds2[i] ) << "  " << ( tds3[i] ) << "  " << ( tds4[i] ) << "  " <<  "  " << ( tds5[i] ) << "  " << ( tds6[i] ) << std::endl;;
}




   {
      double denom = (2 * sqrt(1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1))) * pow(((-s5) - s3 - s1) * ((-s6) - s2 - s1),1.5     ) );
      double t1 = - (1 / sqrt(((-s5) - s3 - s1) * ((-s6) - s2 - s1))
         - (s1 * (s6 + s5 + s3 + s2 + 2 * s1)) / (2 * pow(((-s5) - s3 - s1) * ((-s6) - s2 - s1),1.5       ) )) / sqrt(1 - s1*s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1)));
      double t2 = (s1 * (s5 + s3 + s1)) / denom;
      double t3 = (s1 * (s6 + s2 + s1)) / denom;
      double t4 = 0;
      double t5 = (s1 * (s6 + s2 + s1)) / denom;
      double t6 = (s1 * (s5 + s3 + s1)) / denom;
      double deral = sqrt(sq(t1 * ds1) + sq(t2 * ds2) + sq(t3 * ds3) + sq(t4 * ds4) + sq(t5 * ds5) + sq(t6 * ds6));

      double test = deral * 1.0 / sqrt( 6.0 );

      std::cout << "deral " << deral << std::endl;
      const int i19191 = 19191;
   }

   return cer;
}

S6 S6ErrorsFromCell( const LRL_Cell& cell, const LRL_Cell& errorsInRadians ) {
   S6 s6errors;

   const double& a = cell[0];
   const double& b = cell[1];
   const double& c = cell[2];
   const double& al = cell[3]; // should be radians !!!!!
   const double& be = cell[4];
   const double& ga = cell[5];

const double& da = errorsInRadians[0];
const double& db = errorsInRadians[1];
const double& dc = errorsInRadians[2];
const double& dal = errorsInRadians[3];
const double& dbe = errorsInRadians[4];
const double& dga = errorsInRadians[5];

const double cosal = cos(al);
const double cosbe = cos(be);
const double cosga = cos(ga);
const double sinal = sin(al);
const double sinbe = sin(be);
const double singa = sin(ga);

std::cout << std::endl;
std::cout << "   db * c * cosal " << db * c * cosal << std::endl;
std::cout << "   da * c * cosbe " << da * c * cosbe << std::endl;
std::cout << "   da * b * cosga " << da * b * cosga << std::endl;
std::cout << "   dc * b * cosal  " << dc * b * cosal << std::endl;
std::cout << "   dc * a * cosbe  " << dc * a * cosbe << std::endl;
std::cout << "   db * a * cosga  " << db * a * cosga << std::endl;
std::cout << " -dal * b * c * sinal " << -dal * b * c * sinal << std::endl;
std::cout << " -dbe * a * c * sinbe " << -dbe * a * c * sinbe << std::endl;
std::cout << " -dga * a * b * singa " << -dga * a * b * singa << std::endl;
std::cout << std::endl;

s6errors[0] = sqrt(sq(db * c * cosal) + sq(dc * b * cosal) + sq(-dal * b * c * sinal));
s6errors[1] = sqrt(sq(da * c * cosbe) + sq(dc * a * cosbe) + sq(-dbe * a * c * sinbe));
s6errors[2] = sqrt(sq(da * b * cosga) + sq(db * a * cosga) + sq(-dga * a * b * singa));

s6errors[3] = sqrt(sq(-2.0 * a * da) + sq(-s6errors[2]) + sq(-s6errors[1]));
s6errors[4] = sqrt(sq(-2.0 * b * db) + sq(-s6errors[2]) + sq(-s6errors[0]));
s6errors[5] = sqrt(sq(-2.0 * c * dc) + sq(-s6errors[0]) + sq(-s6errors[1]));

s6errors[0] = maxNC(abs(db * c * cosal), abs(dc * b * cosal), abs(-dal * b * c * sinal));
s6errors[1] = maxNC(abs(da * c * cosbe), abs(dc * a * cosbe), abs(-dbe * a * c * sinbe));
s6errors[2] = maxNC(abs(da * b * cosga), abs(db * a * cosga), abs(-dga * a * b * singa));

s6errors[3] = maxNC(abs(-2.0 * a * da), abs(-s6errors[2]), abs(-s6errors[1]));
s6errors[4] = maxNC(abs(-2.0 * b * db), abs(-s6errors[2]), abs(-s6errors[0]));
s6errors[5] = maxNC(abs(-2.0 * c * dc), abs(-s6errors[0]), abs(-s6errors[1]));

return s6errors / sqrt(6.0);
}

template<typename T>
std::string InputLattice(const size_t n, const std::string& lattice, const T& t, const LRL_Cell& c) {
   LatticeConverter converter;
   MatS6 ms;
   MatG6 mg;
   std::ostringstream o;
   o << "\tOriginal Cell " << n << std::endl;
   o << "input " << lattice << " " << LRL_Cell_Degrees(t) << std::endl;
   const S6 s6Red = converter.SellingReduceCell(lattice, S6(t), ms);
   o << "errors " << LRL_Cell_Degrees(c) << std::endl;
   return o.str();
}

std::vector<S6> CreateS6Errors(const std::vector< S6>& vs) {
   std::vector<S6> out;
   for (size_t i = 0; i < vs.size(); ++i) {
      out.push_back(0.1 * vs[i]);
   }
   return out;
}

std::vector<LRL_Cell> CreateE3Errors(const std::vector<LRL_Cell>& vc) {
   std::vector<LRL_Cell> out;
   LRL_Cell cellErrors;
   for (size_t i = 0; i < vc.size(); ++i) {
      cellErrors = 0.01 * vc[i];
      cellErrors[3] = 0.01 * vc[i][3];
      cellErrors[4] = 0.01 * vc[i][4];
      cellErrors[5] = 0.01 * vc[i][5];
      out.push_back(cellErrors);
   }
   return out;
}

std::vector<LRL_Cell> CreateCells(const std::vector<LRL_ReadLatticeData>& input) {
   std::vector<LRL_Cell> cells;
   for (size_t i = 0; i < input.size(); ++i ) {
      cells.push_back(input[i].GetCell());
}
   return cells;
}

static const std::vector<std::string> heirarchy{
   "cP","cF","cI",
   "tP","hP","hR","tI",
   "oP","oC","oF","oI",
   "mP","mC",  
   "aP"};


void ReportTypeHeirachy(const std::vector<DeloneFitResults>& vDeloneFitResults) {
   for (size_t i = 0; i < heirarchy.size(); ++i) {
      size_t kkbest = 0;
      double bestScore = DBL_MAX;
      const std::string thisLattice = heirarchy[i];

      for (size_t kk = 0; kk < vDeloneFitResults.size(); ++kk) {
         const DeloneFitResults& thisOne = vDeloneFitResults[kk];
         if ((thisLattice == thisOne.GetType()) &&
            thisOne.GetRawFit() < bestScore) {
            kkbest = kk;
            bestScore = thisOne.GetRawFit();
         }
      }
      std::cout << heirarchy[i] << "  ";
      if (bestScore == DBL_MAX)
         std::cout << "invalid          ";
      else {
         std::cout << std::setw(3) << std::setprecision(5) << bestScore << " ";
         std::cout << std::setw(3) << std::setprecision(2) << vDeloneFitResults[kkbest].GetZscore();
      }
      if (i == 2 || i == 6 || i == 10 || i == 12)
         std::cout << std::endl;
      else std::cout << "     ";
   }
}

std::vector<std::pair<std::string, double> > DeloneFitToScores(std::vector< DeloneFitResults>& fits) {

   std::map<std::string, std::pair<std::string, double> > best;

   for (size_t i = 0; i < fits.size(); ++i) {
      auto test = best.find(fits[i].GetType());
      if (test== best.end()) {
         fits[i].GetZscore();
         best.insert(std::make_pair(fits[i].GetType(), std::make_pair(fits[i].GetType(), fits[i].GetRawFit())));
      }
      else
      {
         const double previous = (*test).second.second;
         if (fits[i].GetRawFit() < previous) {
            best[fits[i].GetType()] = std::make_pair(fits[i].GetType(), fits[i].GetRawFit());
         }
      }
   }
   std::vector<std::pair<std::string, double> > out;
   for (auto i = best.begin(); i != best.end(); ++i) {
      out.push_back((*i).second);
   }
   return out;
}

void NiggliMatchLatticeType(const DeloneFitResults& vDeloneFitResults) {

   const S6 input = vDeloneFitResults.GetOriginalInput();
	const S6 bestFit = vDeloneFitResults.GetBestFit();
	const std::string latticeType = vDeloneFitResults.GetType();
	const std::string latticeGeneral = vDeloneFitResults.GetGeneralType();
	
    const std::vector<std::shared_ptr<GenerateNiggliBase> >
        vglb = GenerateNiggliBase().Select(latticeGeneral);

    for (size_t i = 0; i < vglb.size(); ++i) {
       const std::shared_ptr<GenerateNiggliBase> pt = vglb[i];
       if (pt->GetBravaisType()[0] == 'a') continue;
       G6 probe;
       Niggli::Reduce(G6(bestFit), probe);
       const G6 perpV = pt->GetPerp() * probe;
       double d = perpV.norm();
       if (d < 1.0E-8) d = 0.0;
       if ( d/probe.norm() > g_maxDeltaForMatch) continue;

       std::cout
          << latticeType
          << " input" << input
          << "\n red " << probe
          << "  IT=" << pt->GetITNumber()
          << " type=" << pt->GetBravaisType()
          << " d=" << d << std::endl
          << " ratio " << d / probe.norm() << "\n"
          << "perp       " << perpV << "\n"
          << "projected  " << LRL_Cell_Degrees(pt->GetPrj() * probe) << "\n";

       std::cout << "centered " << LRL_Cell_Degrees(pt->GetToCenter() * probe) << std::endl << std::endl;
    }
}

int main()
{
   std::cout << "SELLA\n";

   const std::vector<LRL_ReadLatticeData> input = GetInputCells();

   std::vector<MatS6> reductionMatrices;

   const std::vector<S6> vLat = GetInputSellingReducedVectors(input, reductionMatrices);
   const std::vector<LRL_Cell> cells = CreateCells(input);
   const std::vector<S6> errors = CreateS6Errors(vLat);
   const std::vector<LRL_Cell> cellErrors = CreateE3Errors(cells);

//-----------------------------------------------------------------------------------
   std::vector<std::shared_ptr<GenerateDeloneBase> > sptest =
      GenerateDeloneBase().Select("");


   for (size_t lat = 0; lat < vLat.size(); ++lat) {
      std::vector<DeloneFitResults> vDeloneFitResults = Sella().SellaFit( sptest, vLat[lat], errors[lat], reductionMatrices[lat]);

      for (size_t kk = 0; kk < vDeloneFitResults.size(); ++kk) {
         const double d = vDeloneFitResults[kk].GetRawFit() / vLat[lat].norm();
          if (vDeloneFitResults[kk].GetRawFit() / vLat[lat].norm() < g_maxDeltaForMatch) {
              NiggliMatchLatticeType(vDeloneFitResults[kk]);
          }
      }

      std::cout << std::endl << "reported distances and zscores (in A^2)" << std::endl;



      //for (size_t kk = 0; kk < vDeloneFitResults.size(); ++kk) {
      //   std::cout << vDeloneFitResults[kk] << std::endl;
      //}
      //std::cout << vDeloneFitResults.size() << std::endl;
      ReportTypeHeirachy(vDeloneFitResults);
      const std::vector<std::pair<std::string, double> > scores = DeloneFitToScores(vDeloneFitResults);

      //std::cout << std::endl << std::endl << "lat " << lat << std::endl << std::endl << std::endl;
      /*std::cout << */BravaisHeirarchy::ProduceSVG(
         input[lat], vLat[lat], scores);
   }
}
