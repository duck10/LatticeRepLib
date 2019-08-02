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


   for ( size_t i=0; i<input.size(); ++i ) {
      const S6 s6 = converter.SellingReduceCell(input[i].GetLattice(), input[i].GetCell());
      v.push_back(s6);
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

void ReportFit2( const size_t i, const DeloneType& type, const std::tuple<double, S6, MatS6>& fit ) {
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

void ReportFit( const size_t n, const DeloneTypeList& types, const std::vector<std::tuple<double, S6, MatS6> >& fit ) {
   for ( size_t i=0; i<fit.size(); ++i )
      ReportFit2( i, types[i], fit[i] );
}

int main()
{
   const std::vector<LRL_ReadLatticeData> input = GetInputCells();
   //std::cout << "input complete" << std::endl;
   //AnalyzePDBCells( input );
   //exit( 0 );
   static const DeloneTypeList deloneList;

   const std::vector<S6> vLat = GetInputSellingReducedVectors(input);

   for (size_t lat = 0; lat < vLat.size(); ++lat) {
      std::cout << "input  " << input[lat].GetStrCell() << "    (" << S6(input[lat].GetCell()) <<")" << std::endl;
      //const std::vector<std::tuple<double, S6, MatS6> >  v = DeloneTypeList::Fit( "T", vLat[lat] );
      const std::vector<std::tuple<double, S6, MatS6> >  v = deloneList.Fit( vLat[lat] );
      ReportFit( lat, deloneList,  v );
      std::cout << "************************************" << std::endl;
   }

   const int  i19191 = 19191;
}
