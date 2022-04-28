// SELLA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include "BravaisHeirarchy.h"
#include "C3.h"
#include "FileOperations.h"

#include "DeloneTypeList.h"
#include "GenerateLatticeTypeExamples.h"
#include "LRL_inverse.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_CoordinateConversionMatrices.h"
#include "LatticeConverter.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "MatG6.h"
#include "MatS6.h"
#include "Niggli.h"
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
      if (s6[0] > 0 || s6[1] > 0 || s6[2] > 0 || s6[3] > 0 || s6[4] > 0 || s6[5] > 0) {
         throw;
      }
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

SellaResult ReportFit2(const size_t i, const DeloneType& type, const DeloneFitResults& fit) {
   //std::cout << "Fit Report for Delone Type " << i << std::endl;
   //std::cout << "\t" << type << "  " << fit << std::endl;

   const MatS6 mToCentered = type.GetToCentered();
   //std::cout << std::endl << "\t" << "mToCentered" << std::endl << mToCentered << std::endl;

   double inverseCanon(36);
   //std::cout << std::endl << "To Canon" << std::endl <<  << std::endl;

   const MatS6 canon = fit.GetToCanon();
   //std::cout << "\t" << "canon \n" << canon << std::endl;

   const S6 bestFit = fit.GetBestFit();
   MatG6 g6RedMat;
   G6 g6Red;
   //std::cout << "\tNiggli Reduced " << g6Red << std::endl;
   //std::cout << "\tNiggli Reduced " << LRL_Cell_Degrees(g6Red) << std::endl;
   //std::cout << "\t" <<"Centered Cell" << std::endl;
   const S6 best = mToCentered * canon * bestFit;


   //LCA this is where we need to test the reflections and ToCanon to find
   // the correct to canon matrix
   SellaResult sr;
   sr.SetBravaisType(type.GetBravaisType());
   // need character
   sr.SetDistance(fit.GetRawFit());
   sr.SetName(type.GetName());
   sr.SetPerp(S6());
   sr.SetProjected(bestFit);
   sr.SetZscore(fit.GetZscore());
   sr.SetOriginal(fit.GetOriginalInput());


   //std::cout << sr << std::endl;

   return sr;
   //std::ostream& operator<< (std::ostream & o, const DeloneType & m) {
   //   o << "REPORT FOR ONE DELONE TYPE " << std::endl;
   //   o << "\t" << "Delone Type  " << m.m_deloneName << std::endl;
   //   o << "\t" << "BravaisLattice Type  " << m.m_bravaisType << std::endl;
   //   o << "\t" << "Character Type  " << m.m_character << std::endl;
   //   o << "\t" << "Canonical to Centered (E3)  " << m.m_toCentered_E3 << std::endl;
   //   //o << "\t" << "Canonical to Centered (S6)  " << std::endl << m.m_toCentered_S6 << std::endl;
   //   o << "\t" << "Count of all representations  " << m.m_matrices.size() << std::endl;
   //   return o;
   //}


   //std::cout << "\t" << "S6 " << best << std::endl;
   //std::cout << "\t" << "Cell " << LRL_Cell_Degrees(best) << std::endl;

   //const bool bNiggli = Niggli::Reduce(G6(bestFit), g6RedMat, g6Red);
   //std::cout << std::endl;

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

void ReportSellaFit(const size_t n, const std::vector<std::shared_ptr<GenerateDeloneBase> >& types, const std::vector<DeloneFitResults>& fit) {
   //std::cout << "*******   SELLA report for input cell " << n << std::endl;
   //std::vector<SellaResult> vsr;

   //for (size_t i = 0; i < types.size(); ++i) {
   //   std::cout << types[i]->GetName() << std::endl;
   //}

   //for (size_t i = 0; i < vsr.size(); ++i)
   //   std::cout << vsr[i] << std::endl;
   //std::cout << "*******   END SELLA report for input cell " << n << std::endl;

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

void TestSigmas() {
   const LRL_Cell cell("10 12 15  95 100 105 ");
   const LRL_Cell cellErrors(" .01 .01 .01 .001 .002 .003");
   std::cout << "input cell " << LRL_Cell(cell) << std::endl;
   std::cout << "cell sigs " << cellErrors << std::endl;
   const S6 s6(cell);
   const S6 s6er = S6ErrorsFromCell(cell, cellErrors);
   const LRL_Cell newCellErrors = CellErrorsFromS6(s6, s6er);
   std::cout << std::endl << "output cell sigs " << newCellErrors << std::endl;


}

void TestReflections(const S6& s, const DeloneType& type) {
   static std::vector<MatS6> refls = MatS6::GetReflections();
   const MatS6& tocenter = type.GetToCentered();
   //const MatS6& tocanon = type.GetToCanon();
   for (size_t i = 0; i < refls.size(); ++i) {
      std::cout << i << "  " << refls[i] * s << std::endl;
   }
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

static double Zscore(const S6& s6, const S6& sigmas, const MatS6& reductionMatrix)
{
   //std::cout << "s6 " << s6 << std::endl;
   //std::cout << "sigmas " << sigmas << std::endl;
   //std::cout << "red mat " << reductionMatrix.norm() << std::endl;
   //std::cout << "s6 norm " << s6.Norm() << std::endl;
   //std::cout << "denom " << (reductionMatrix * sigmas).Norm() << std::endl;
   const double zscore = s6.Norm() / (MatS6::Inverse(reductionMatrix) * sigmas).Norm();
   return (zscore < 1.0E-6) ? 0.0 : zscore;
}



static DeloneFitResults GetFit(
   const std::shared_ptr<GenerateDeloneBase>& type, 
   const S6& s6, 
   const MatS6& reductionMatrix) {

   size_t nBest = 0;
   S6 smallestPerp;
   double bestFit = DBL_MAX;
   const std::vector<MatS6> perps = type->GetSellaPerps();
   for (size_t i = 0; i < perps.size(); ++i) {
      const S6 perpv = perps[i] * s6;
      const double rawFit = perpv.norm();
      if (bestFit > rawFit) {
         nBest = i;
         bestFit = rawFit;
         smallestPerp = perpv;

         //std::cout << " representation   " << i << std::endl;
         //std::cout << "original " << s6 << std::endl;
         //std::cout << "projected  " << bestv << std::endl;
         //std::cout << "perp v " << perpv << std::endl;
         //std::cout << "projected s6 " << bestv << std::endl;
         //std::cout << "distance " << rawFit << std::endl;
      }
   }
   if (bestFit < 1.0E-8) bestFit = 0.0;
   const S6 bestv = perps[nBest] * s6;
   //const MatS6 toCanonicalDeloneType = type->GetToCanon(nBest);

   return DeloneFitResults(bestFit, bestv, smallestPerp, MatS6().unit());
}

DeloneFitResults SellaFitXXXXXX(
   const std::shared_ptr<GenerateDeloneBase>& sptype,
   const S6& s6,
   const MatS6& reductionMatrix) {

   size_t nBest = 0;
   double bestFit = DBL_MAX;
   const std::vector<MatS6>& perps = sptype->GetSellaPerps();
   const std::vector<MatS6>& prjs = sptype->GetSellaProjectors();
   for (size_t i = 0; i < perps.size(); ++i) {
      const S6 perpv = perps[i] * s6;
      const double rawFit = perpv.norm();
      if (bestFit > rawFit) {
         nBest = i;
         bestFit = rawFit;

         //std::cout << " representation   " << i << std::endl;
         //std::cout << "original " << s6 << std::endl;
         //std::cout << "projected  " << bestv << std::endl;
         //std::cout << "perp v " << perpv << std::endl;
         //std::cout << "projected s6 " << bestv << std::endl;
         //std::cout << "distance " << rawFit << std::endl;
      }
   }
   if (bestFit < 1.0E-8) bestFit = 0.0;
   const S6 smallestPerp = perps[nBest] * s6;
   const S6 bestv = prjs[nBest] * s6;
   const MatS6 toCanonicalDeloneType/* = sptypes[nBest]->GetToCanon(nBest)*/;
   //std::cout << "input " << s6 << std::endl;
   //std::cout << "nBest " << nBest << std::endl;
   //std::cout << " bestv " << bestv << std::endl;
   //std::cout << "prj " << std::endl;
   //std::cout << prjs[nBest] << std::endl;

   return DeloneFitResults(bestFit, bestv, smallestPerp, MatS6().unit());
}

static std::vector<DeloneFitResults> SellaFit(
   const std::vector<std::shared_ptr<GenerateDeloneBase> >& sptypes,
   const S6& s6,
   const S6& errors,
   const MatS6& reductionMatrix) {

   std::vector<DeloneFitResults> vDeloneFitResults;

   for (size_t i = 0; i < sptypes.size(); ++i) {
      const std::string name = sptypes[i]->GetName();
      /*if (type.empty() || name.find(type) != std::string::npos) */{  // LCA make type UC
         DeloneFitResults fit = SellaFitXXXXXX(sptypes[i], s6, reductionMatrix);

         const double zscore = Zscore(s6 - fit.GetBestFit(), errors, reductionMatrix) * sqrt(sptypes[i]->GetFreeParams());
         //std::cout << "s6 " << s6 << "---------------------" << std::endl;
         //std::cout << "best fit " << fit.GetBestFit() << std::endl;
         //std::cout << "errors " << errors << std::endl;
         //std::cout << "name " << name << std::endl;
         //std::cout << "free params " << sptypes[i]->GetFreeParams() << std::endl;
         //std::cout << "zscore " << zscore << std::endl;
         //std::cout << "red norm " << reductionMatrix.norm() << std::endl;
         fit.SetZscore(zscore);
         fit.SetLatticeType(name);
         fit.SetReductionMatrix(reductionMatrix);
         fit.SetType(sptypes[i]->GetBravaisType());
         fit.SetGeneralType(sptypes[i]->GetBravaisLatticeGeneral());

         //std::cout << "--------------------" << name << std::endl;
         //std::cout << s6 << std::endl;
         //std::cout << fit.GetBestFit() << std::endl;
         //std::cout << fit.GetDifference() << std::endl;

         fit.SetDifference(s6 - fit.GetBestFit());
         fit.SetOriginalInput(s6);
         vDeloneFitResults.push_back(fit);
      }
   }
   return vDeloneFitResults;
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
        G6 probe;
        Niggli::Reduce(G6(bestFit), probe);
        const G6 perpV = pt->GetPerp() * probe;
        double d = perpV.norm();
        if (d < 1.0E-8) d = 0.0;

        std::cout
            << latticeType
            << " input" << input
            << "\n red " << probe 
            << "  IT=" << pt->GetITNumber()
            << " type=" << pt->GetBravaisType()
            << " d=" << d << std::endl
            //<< "perp       " << perpV << "\n"
			<< "projected  " << LRL_Cell_Degrees(pt->GetPrj() * probe) << "\n\n";
    }
}

int main()
{
   //TestSigmas( );
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
      std::vector<DeloneFitResults> vDeloneFitResults = SellaFit( sptest, vLat[lat], errors[lat], reductionMatrices[lat]);

	
      for (size_t kk = 0; kk < vDeloneFitResults.size(); ++kk) {
          if (vDeloneFitResults[kk].GetRawFit() / vLat[lat].norm() < 0.02) {
              NiggliMatchLatticeType(vDeloneFitResults[kk]);
          }
      }


	
      //for (size_t kk = 0; kk < vDeloneFitResults.size(); ++kk) {
      //   std::cout << vDeloneFitResults[kk] << std::endl;
      //}
      //std::cout << vDeloneFitResults.size() << std::endl;
      //ReportSellaFit(lat, sptest, vDeloneFitResults);
      ReportTypeHeirachy(vDeloneFitResults);
      const std::vector<std::pair<std::string, double> > scores = DeloneFitToScores(vDeloneFitResults);

      std::cout << std::endl << std::endl << "lat " << lat << std::endl << std::endl << std::endl;
      /*std::cout << */BravaisHeirarchy::ProduceSVG(
         input[lat], vLat[lat], scores);
   }

   const int  i19191 = 19191;
}
