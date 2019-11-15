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

   const LRL_Cell cel(s);
   const double a = cel[0];
   const double b = cel[1];
   const double c = cel[2];
   const double al = cel[3];
   const double be = cel[4];
   const double ga = cel[5];

   const double& s1 = s[0];
   const double& s2 = s[1];
   const double& s3 = s[2];
   const double& s4 = s[3];
   const double& s5 = s[4];
   const double& s6 = s[5];

   da = sqrt((sq(ds4) + sq(ds3) + sq(ds2)) / sq(2.0 * a));
   db = sqrt((sq(ds5) + sq(ds3) + sq(ds1)) / sq(2.0 * b));
   dc = sqrt((sq(ds6) + sq(ds2) + sq(ds1)) / sq(2.0 * c));

   const double dax = -0.5 * (-ds4 - ds3 - ds2) / sqrt(-s4 - s3 - s2);
    std::cout << "diff in da " << da - dax << std::endl;
    std::cout << " da, dax " << da << "  " << dax << std::endl;

    std::cout << "b * c * ds1 " << b * c * ds1 << std::endl;
    std::cout << "a * c * ds2 " << a * c * ds2 << std::endl;
    std::cout << "a * b * ds3 " << a * b * ds3 << std::endl;
    std::cout << "s1 * db " << s1 * db << std::endl;
    std::cout << "s2 * da " << s2 * da << std::endl;
    std::cout << "s3 * da " << s3 * da << std::endl;
    std::cout << "s1 * dc " << s1 * dc << std::endl;
    std::cout << "s2 * dc " << s2 * dc << std::endl;
    std::cout << "s3 * db " << s3 * db << std::endl;
    std::cout << "sin(al) * (b * c) * (b * c) " << sin(al) * (b * c) * (b * c) << std::endl;
    std::cout << "sin(be) * (a * c) * (a * c) " << sin(be) * (a * c) * (a * c) << std::endl;
    std::cout << "sin(ga) * (a * b) * (a * b) " << sin(ga) * (a * b) * (a * b) << std::endl;



   dal = (b * c * ds1 + s1 * db + s1 * dc) / (sin(al) * (b * c) * (b * c));
   dbe = (a * c * ds2 + s2 * da + s2 * dc) / (sin(be) * (a * c) * (a * c));
   dga = (a * b * ds3 + s3 * da * s3 * db) / (sin(ga) * (a * b) * (a * b));

   std::cout << "input S6 " << s << std::endl;
   std::cout << "input S6 sigmas " << sig << std::endl;
   std::cout << "input cell " << LRL_Cell(s) << std::endl;
   std::cout << "result cell sigs " << cer << std::endl;

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

   return s6errors;
}

void TestSigmas( ) {
   const LRL_Cell cell("10 10 10 90 90 90 ");
   const LRL_Cell errors("  .1 .1 .1 .1 .1 .1");
   std::cout << "input cell " << LRL_Cell(cell) << std::endl;
   std::cout << "cell sigs " << errors << std::endl;
   const S6 s6(cell);
   const S6 s6er = S6ErrorsFromCell(cell, errors);
   const LRL_Cell newCellErrors = CellErrorsFromS6(s6, s6er);
   std::cout << "output cell sigs " << newCellErrors << std::endl;


}

int main()
{
   TestSigmas( );




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
