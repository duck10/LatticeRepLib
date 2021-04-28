// SELLA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

#include "C3.h"
#include "DeloneTypeList.h"
#include "LRL_CoordinateConversionMatrices.h"
#include "LatticeConverter.h"
#include "LRL_MinMaxTools.h"
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

   const std::vector<std::string>& typelist = DeloneTypeList::typelist;
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

   storeO3 .ShowResults();
   storeM3 .ShowResults();
   storeM2B.ShowResults();
   exit(0);
}

void ReportFit1( const size_t i, const DeloneType& type, const std::tuple<double, S6, MatS6>& fit ) {
   std::cout << i << "  " << type.GetName( ) << "  " << std::get<double>( fit ) << "     " << type.GetCharacter( ) <<
      std::endl << "input  " << std::get<S6>( fit ) << std::endl << std::get<MatS6>( fit ) << std::endl;
   std::cout << "result  " << std::get<MatS6>( fit ) * std::get<S6>( fit ) << std::endl;
   const MatS6 ms6 = std::get<MatS6>( fit );
   std::cout << type.GetToCentered( ) * std::get<MatS6>( fit ) * std::get<S6>( fit ) << std::endl;
   std::cout << "centered cell  " << LRL_Cell_Degrees( type.GetToCentered( ) * std::get<MatS6>( fit ) * std::get<S6>( fit ) ) << std::endl << std::endl;
}

void ReportFit2( const size_t i, const DeloneType& type, const DeloneFitResults& fit ) {
   std::cout << type << "  " << fit << std::endl;
}

void ReportFit( const size_t n, const DeloneTypeList& types, const std::vector<DeloneFitResults>& fit ) {
   for ( size_t i=0; i<fit.size(); ++i )
      ReportFit2( i, types[i], fit[i] );
}

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

   /*
   %i8) derivative(alp,s1);

(%o8) -(1/sqrt(((-s5)-s3-s1)*((-s6)-s2-s1))
 -(s1*(s6+s5+s3+s2+2*s1))/(2*(((-s5)-s3-s1)*((-s6)-s2-s1))^(3/2)))/sqrt(1-s1^2/(((-s5)-s3-s1)*((-s6)-s2-s1)))
(%i9) derivative(alp,s2);

(%o9) (s1*(s5+s3+s1))/(2*sqrt(1-s1^2/(((-s5)-s3-s1)*((-s6)-s2-s1)))*(((-s5)-s3-s1)*((-s6)-s2-s1))^(3/2))
(%i10) derivative(alp,s3);

(%o10) (s1*(s6+s2+s1))/(2*sqrt(1-s1^2/(((-s5)-s3-s1)*((-s6)-s2-s1)))*(((-s5)-s3-s1)*((-s6)-s2-s1))^(3/2))
(%i11) derivative(alp,s5);

(%o11) (s1*(s6+s2+s1))/(2*sqrt(1-s1^2/(((-s5)-s3-s1)*((-s6)-s2-s1)))*(((-s5)-s3-s1)*((-s6)-s2-s1))^(3/2))
((%i12) derivative(alp,s6);

(%o12) (s1*(s5+s3+s1))/(2*sqrt(1-s1^2/(((-s5)-s3-s1)*((-s6)-s2-s1)))*(((-s5)-s3-s1)*((-s6)-s2-s1))^(3/2))


==============================================================================================
cell2s6(a,b,c,alpha,beta,gamma):=[b*c*cos(alpha),a*c*cos(beta),a*b*cos(gamma),-a*a-a*b*cos(gamma)-a*c*cos(beta),-a*b*cos(gamma)-b*b-b*c*cos(alpha),-a*c*cos(beta)-b*c*cos(alpha)-c*c];
s6tocell(s1,s2,s3,s4,s5,s6):=[sqrt(-s4-s3-s2),sqrt(-s5-s3-s1),sqrt(-s6-s2-s1),acos(s1/(sqrt(-s5-s3-s1)*sqrt(-s6-s2-s1))),acos(s2/(sqrt(-s4-s3-s2)*sqrt(-s6-s2-s1))),acos(s3/(sqrt(-s4-s3-s2)*sqrt(-s5-s3-s1)))];

[ds1*derivative(s6tocell(s1,s2,s3,s4,s5,s6),s1),
ds2*derivative(s6tocell(s1,s2,s3,s4,s5,s6),s2),
ds3*derivative(s6tocell(s1,s2,s3,s4,s5,s6),s3),
ds4*derivative(s6tocell(s1,s2,s3,s4,s5,s6),s4),
ds5*derivative(s6tocell(s1,s2,s3,s4,s5,s6),s5),
ds6*derivative(s6tocell(s1,s2,s3,s4,s5,s6),s6)];

produced:

(%o16) [[0,-ds1/(2*sqrt((-s5)-s3-s1)),-ds1/(2*sqrt((-s6)-s2-s1)),
         -(ds1*(1/(sqrt((-s5)-s3-s1)*sqrt((-s6)-s2-s1))
               +s1/(2*((-s5)-s3-s1)^(3/2)*sqrt((-s6)-s2-s1))
               +s1/(2*sqrt((-s5)-s3-s1)*((-s6)-s2-s1)^(3/2))))
          /sqrt(1-s1^2/(((-s5)-s3-s1)*((-s6)-s2-s1))),
         -(ds1*s2)/(2*sqrt((-s4)-s3-s2)
                     *sqrt(1-s2^2/(((-s4)-s3-s2)*((-s6)-s2-s1)))
                     *((-s6)-s2-s1)^(3/2)),
         -(ds1*s3)/(2*sqrt((-s4)-s3-s2)
                     *sqrt(1-s3^2/(((-s4)-s3-s2)*((-s5)-s3-s1)))
                     *((-s5)-s3-s1)^(3/2))],
        [-ds2/(2*sqrt((-s4)-s3-s2)),0,-ds2/(2*sqrt((-s6)-s2-s1)),
         -(ds2*s1)/(2*sqrt((-s5)-s3-s1)
                     *sqrt(1-s1^2/(((-s5)-s3-s1)*((-s6)-s2-s1)))
                     *((-s6)-s2-s1)^(3/2)),
         -(ds2*(1/(sqrt((-s4)-s3-s2)*sqrt((-s6)-s2-s1))
               +s2/(2*((-s4)-s3-s2)^(3/2)*sqrt((-s6)-s2-s1))
               +s2/(2*sqrt((-s4)-s3-s2)*((-s6)-s2-s1)^(3/2))))
          /sqrt(1-s2^2/(((-s4)-s3-s2)*((-s6)-s2-s1))),
         -(ds2*s3)/(2*((-s4)-s3-s2)^(3/2)
                     *sqrt(1-s3^2/(((-s4)-s3-s2)*((-s5)-s3-s1)))
                     *sqrt((-s5)-s3-s1))],
        [-ds3/(2*sqrt((-s4)-s3-s2)),-ds3/(2*sqrt((-s5)-s3-s1)),0,
         -(ds3*s1)/(2*((-s5)-s3-s1)^(3/2)
                     *sqrt(1-s1^2/(((-s5)-s3-s1)*((-s6)-s2-s1)))
                     *sqrt((-s6)-s2-s1)),
         -(ds3*s2)/(2*((-s4)-s3-s2)^(3/2)
                     *sqrt(1-s2^2/(((-s4)-s3-s2)*((-s6)-s2-s1)))
                     *sqrt((-s6)-s2-s1)),
         -(ds3*(1/(sqrt((-s4)-s3-s2)*sqrt((-s5)-s3-s1))
               +s3/(2*((-s4)-s3-s2)^(3/2)*sqrt((-s5)-s3-s1))
               +s3/(2*sqrt((-s4)-s3-s2)*((-s5)-s3-s1)^(3/2))))
          /sqrt(1-s3^2/(((-s4)-s3-s2)*((-s5)-s3-s1)))],
        [-ds4/(2*sqrt((-s4)-s3-s2)),0,0,0,
         -(ds4*s2)/(2*((-s4)-s3-s2)^(3/2)
                     *sqrt(1-s2^2/(((-s4)-s3-s2)*((-s6)-s2-s1)))
                     *sqrt((-s6)-s2-s1)),
         -(ds4*s3)/(2*((-s4)-s3-s2)^(3/2)
                     *sqrt(1-s3^2/(((-s4)-s3-s2)*((-s5)-s3-s1)))
                     *sqrt((-s5)-s3-s1))],
        [0,-ds5/(2*sqrt((-s5)-s3-s1)),0,
         -(ds5*s1)/(2*((-s5)-s3-s1)^(3/2)
                     *sqrt(1-s1^2/(((-s5)-s3-s1)*((-s6)-s2-s1)))
                     *sqrt((-s6)-s2-s1)),0,
         -(ds5*s3)/(2*sqrt((-s4)-s3-s2)
                     *sqrt(1-s3^2/(((-s4)-s3-s2)*((-s5)-s3-s1)))
                     *((-s5)-s3-s1)^(3/2))],
        [0,0,-ds6/(2*sqrt((-s6)-s2-s1)),
         -(ds6*s1)/(2*sqrt((-s5)-s3-s1)
                     *sqrt(1-s1^2/(((-s5)-s3-s1)*((-s6)-s2-s1)))
                     *((-s6)-s2-s1)^(3/2)),
         -(ds6*s2)/(2*sqrt((-s4)-s3-s2)
                     *sqrt(1-s2^2/(((-s4)-s3-s2)*((-s6)-s2-s1)))
                     *((-s6)-s2-s1)^(3/2)),0]]
*/

   // ds1 ---------------------------------------------------------------------
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

   //{
   //   double t1 = -(1 / sqrt(((-s5) - s3 - s1) * ((-s6) - s2 - s1))
   //      - (s1 * (s6 + s5 + s3 + s2 + 2 * s1)) / (2 * pow((((-s5) - s3 - s1) * ((-s6) - s2 - s1)), 1.5))) / sqrt(1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1)));
   //   double t2 = (s1 * (s5 + s3 + s1)) / (2 * sqrt(1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1))) * pow((((-s5) - s3 - s1) * (-s6 - s2 - s1)), 1.5));
   //   double t3 = (s1 * (s6 + s2 + s1)) / (2 * sqrt(1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1))) * pow((((-s5) - s3 - s1) * (-s6 - s2 - s1)), 1.5));
   //   double t4 = 0;
   //   double t5 = (s1 * (s6 + s2 + s1)) / (2 * sqrt(1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1))) * pow((((-s5) - s3 - s1) * (-s6 - s2 - s1)), 1.5));
   //   double t6 = (s1 * (s5 + s3 + s1)) / (2 * sqrt(1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1))) * pow((((-s5) - s3 - s1) * (-s6 - s2 - s1)), 1.5));
   //   double deraltest = sqrt(sq(t1 * ds1) + sq(t2 * ds2) + sq(t3 * ds3) + sq(t4 * ds4) + sq(t5 * ds5) + sq(t6 * ds6));
   //}
   //{
   //   double term1 = -(1 / sqrt(bas * cas)) / sqrt(1 - s1 * s1 / (bas * cas)) * ds1;
   //   double term1A = -(s1 * (-bas - cas)) / (2 * (bas * cas) * sqrtb * sqrtc) / sqrt(1 - s1 * s1 / (bas * cas)) * ds1;

   //   double denom = (2 * sqrt(1 - s1 * s1 / (bas * cas)) * (bas * cas) * sqrtb * sqrtc);
   //   double term2 = -(s1 * bas) / denom * ds2;
   //   double term3 = -(s1 * cas) / denom * ds3;
   //   double term4 = -(s1 * cas) / denom * ds5;
   //   double term5 = -(s1 * bas) / denom * ds6;
   //   double deralpha = sqrt(sq(term1A) + sq(term1) + sq(term2) + sq(term3) + sq(term4) + sq(term5));
   //}

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

   s6errors[0] = sqrt( sq( db * c * cosal ) + sq( dc * b * cosal ) + sq( -dal * b * c * sinal ) );
   s6errors[1] = sqrt( sq( da * c * cosbe ) + sq( dc * a * cosbe ) + sq( -dbe * a * c * sinbe ) );
   s6errors[2] = sqrt( sq( da * b * cosga ) + sq( db * a * cosga ) + sq( -dga * a * b * singa ) );

   s6errors[3] = sqrt( sq( -2.0 * a * da ) + sq( -s6errors[2] ) + sq( -s6errors[1] ) );
   s6errors[4] = sqrt( sq( -2.0 * b * db ) + sq( -s6errors[2] ) + sq( -s6errors[0] ) );
   s6errors[5] = sqrt( sq( -2.0 * c * dc ) + sq( -s6errors[0] ) + sq( -s6errors[1] ) );

   s6errors[0] = maxNC( abs( db * c * cosal ), abs( dc * b * cosal ), abs( -dal * b * c * sinal ) );
   s6errors[1] = maxNC( abs( da * c * cosbe ), abs( dc * a * cosbe ), abs( -dbe * a * c * sinbe ) );
   s6errors[2] = maxNC( abs( da * b * cosga ), abs( db * a * cosga ), abs( -dga * a * b * singa ) );

   s6errors[3] = maxNC( abs( -2.0 * a * da ), abs( -s6errors[2] ), abs( -s6errors[1] ) );
   s6errors[4] = maxNC( abs( -2.0 * b * db ), abs( -s6errors[2] ), abs( -s6errors[0] ) );
   s6errors[5] = maxNC( abs( -2.0 * c * dc ), abs( -s6errors[0] ), abs( -s6errors[1] ) );

   return s6errors/sqrt(6.0);
}

void TestSigmas( ) {
   const LRL_Cell cell("10 12 15  95 100 105 ");
   const LRL_Cell cellErrors(" .01 .01 .01 .001 .002 .003");
   std::cout << "input cell " << LRL_Cell(cell) << std::endl;
   std::cout << "cell sigs " << cellErrors << std::endl;
   const S6 s6(cell);
   const S6 s6er = S6ErrorsFromCell(cell, cellErrors);
   const LRL_Cell newCellErrors = CellErrorsFromS6(s6, s6er);
   std::cout << std::endl << "output cell sigs " << newCellErrors << std::endl;


}

int main()
{
   //TestSigmas( );




   const std::vector<LRL_ReadLatticeData> input = GetInputCells();
   //std::cout << "input complete" << std::endl;
   //AnalyzePDBCells( input );
   //exit( 0 );
   static const DeloneTypeList deloneList;
   std::vector<MatS6> reductionMatrices;

   const std::vector<S6> vLat = GetInputSellingReducedVectors(input, reductionMatrices);
   std::vector<S6> errors;
   for (size_t i = 0; i < vLat.size( ); ++i) {
      const LRL_Cell currentCell = LRL_Cell( vLat[i] );
      LRL_Cell cellErrors = 0.01 * currentCell;
      cellErrors[3] = 0.01 * currentCell[3];
      cellErrors[4] = 0.01 * currentCell[4];
      cellErrors[5] = 0.01 * currentCell[5];
      errors.push_back( S6ErrorsFromCell( input[i].GetCell( ), cellErrors ));
      std::cout << "input cell::" << std::endl;
      std::cout << input[i].GetStrCell( ) << std::endl;
      std::cout << "input cell errors (A and radians)::" << std::endl;
      std::cout << cellErrors << std::endl;
      std::cout << "S6 and S6 errors::" << std::endl;
      std::cout << vLat[i] << std::endl << errors[i] << std::endl;
      std::cout << "RATIO " << vLat[i].norm() / errors[i].norm() << std::endl << std::endl;
   }

  // for (size_t lat = 0; lat < vLat.size( ); ++lat) {
  //    std::cout << "input  " << input[lat].GetStrCell( ) << "    (" << vLat[lat] << ")" << std::endl;
  //    const std::vector<std::tuple<double, S6, MatS6> >  v = deloneList.Fit( vLat[lat] );
  //}

   for (size_t lat = 0; lat < vLat.size(); ++lat) {
      //std::cout << "input  " << input[lat].GetStrCell() << "    (" << (input[lat].GetCell()) <<")" << std::endl;
      //const std::vector<std::tuple<double, S6, MatS6> >  v = DeloneTypeList::Fit( "T", vLat[lat] );
      const std::vector<DeloneFitResults>  v = deloneList.Fit( vLat[lat], errors[lat], reductionMatrices[lat] );
      ReportFit( lat, deloneList,  v );
      //std::cout << "************************************" << std::endl;
   }

   const int  i19191 = 19191;
}
