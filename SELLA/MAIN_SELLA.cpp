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
#include "DeloneFitResults.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_CoordinateConversionMatrices.h"
#include "LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "MatS6.h"
#include "Niggli.h"
#include "S6.h"
#include "Sella.h"
#include "Selling.h"
#include "StoreResults.h"

static double g_maxDeltaForMatch = 0.02;
std::string selectBravaisCase = "";

//std::vector<S6> GetInputSellingReducedVectors( const std::vector<LRL_ReadLatticeData>& input ) {
//   std::vector<S6> v;
//   LatticeConverter converter;
//
//   for (size_t i = 0; i < input.size( ); ++i) {
//      const S6 s6 = converter.SellingReduceCell( input[i].GetLattice( ), input[i].GetCell( ) );
//      v.push_back( s6 );
//   }
//   return v;
//}

std::vector<S6> GetInputSellingReducedVectors( const std::vector<LRL_ReadLatticeData>& input, std::vector<MatS6>& vmat ) {
   std::vector<S6> v;
   MatS6 m;
   vmat.clear( );

   for (size_t i = 0; i < input.size(); ++i) {
      const S6 s6 = LatticeConverter::SellingReduceCell(input[i].GetLattice(), input[i].GetCell(), m);
      v.push_back(s6);
      vmat.push_back(m);
   }

   return v;
}
//
//std::string Trim(const std::string type) {
//   for ( size_t i=0; i<type.size(); ++i ) {
//      if (type[i] == ' ') return type.substr(0, i);
//   }
//   return type;
//}

//StoreResults<std::string, S6> storeProblems(10);
//
//StoreResults<std::string, std::string> store_cP(1000);
//StoreResults<std::string, std::string> store_cI(1000);
//StoreResults<std::string, std::string> store_cF(1000);
//void AnalyzePDBCells(const std::vector<LRL_ReadLatticeData>& input) {
//   StoreResults<std::string, std::string> storeGood(20);
//   StoreResults<std::string, std::string> storeLocalProblems(100);
//   StoreResults<double, std::string> storeO3(100);
//   StoreResults<double, std::string> storeM3(100);
//   StoreResults<double, std::string> storeM2B(100);
//   storeO3.SetTitle("O3 O3 O3 O3 O3 O3 O3 O3 ");
//   storeM3.SetTitle("M3 M3 M3 M3 M3 M3 M3 M3 ");
//   storeM2B.SetTitle("M2B M2B M2B M2B M2B M2B ");
//   std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
//   const std::vector<S6> vLat = GetInputSellingReducedVectors(input);
//   Sella sella;
//
//   for (size_t lat = 0; lat < vLat.size( ); ++lat) {
//      const std::string strCell = input[lat].GetStrCell( );
//      if (strCell[0] == 'A' || strCell[0] == 'a') continue;
//      const std::string xtalSystem1 = std::string( 1, strCell[strCell.length( ) - 1] );
//      std::string deloneType = strCell.substr( 0, 3 );
//      deloneType = Trim( deloneType );
//      if (xtalSystem1 == "a") continue;
//      /*for (size_t type = 0; type < typelist.size( ); ++type) */{
//         //const std::string& xtalSystem2 = typelist[type];
//         const std::pair<std::string, double> fit = sella.GetBestFitForCrystalSystem( xtalSystem1, vLat[lat] );
//         //const std::vector<std::pair<std::string, double> > fit = sella.GetVectorOfFits( vLat[lat] );
//         const double normlat = vLat[lat].norm( );
//         if (fit.second > 1.0E-5) storeLocalProblems.Store( fit.first, LRL_ToString( " ", fit.first, "  ", fit.second, " (", normlat, ")  ", strCell, "  ", vLat[lat] ) );
//         if (fit.second < 1.0E-5) storeGood.Store( fit.first, LRL_ToString( " ", fit.first, " ", fit.second, "  ", strCell, "  ", vLat[lat] ) );
//         //std::cout << LRL_ToString( fit.first, "  " ) << LRL_ToString( LRL_ToString( " ", fit.first, " ", fit.second, "  ", strCell, "  ", vLat[lat] ) ) << std::endl;
//      }
//   }
//   storeLocalProblems.ShowResults( );
//   storeGood.ShowResults();
//   exit(0);
//}
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
//
//std::vector<LRL_Cell> CreateCells(const std::vector<LRL_ReadLatticeData>& input) {
//   std::vector<LRL_Cell> cells;
//   for (size_t i = 0; i < input.size(); ++i ) {
//      cells.push_back(input[i].GetCell());
//}
//   return cells;
//}

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
   std::cout << std::endl;
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
      out.emplace_back((*i).second);
   }
   return out;
}

//void NiggliMatchLatticeType(const DeloneFitResults& vDeloneFitResults) {
//
//   const S6 input = vDeloneFitResults.GetOriginalInput();
//	const S6 bestFit = vDeloneFitResults.GetBestFit();
//	const std::string latticeType = vDeloneFitResults.GetType();
//	const std::string latticeGeneral = vDeloneFitResults.GetGeneralType();
//	
//    const std::vector<std::shared_ptr<GenerateNiggliBase> >
//        vglb = GenerateNiggliBase().Select(latticeGeneral);
//
//    for (size_t i = 0; i < vglb.size(); ++i) {
//       const std::shared_ptr<GenerateNiggliBase> pt = vglb[i];
//       if (pt->GetBravaisType()[0] == 'a') continue;
//       G6 probe;
//       Niggli::Reduce(G6(bestFit), probe);
//       const G6 perpV = pt->GetPerp() * probe;
//       double d = perpV.norm();
//       if (d < 1.0E-8) d = 0.0;
//       if ( d/probe.norm() > g_maxDeltaForMatch) continue;
//
//       std::cout
//          << latticeType
//          << " input" << input
//          << "\n red " << probe
//          << "  IT=" << pt->GetITNumber()
//          << " type=" << pt->GetBravaisType()
//          << " d=" << d << std::endl
//          << " ratio " << d / probe.norm() << "\n"
//          //<< "perp       " << perpV << "\n"
//          << "projected  " << LRL_Cell_Degrees(pt->GetPrj() * probe) << "\n";
//
//       std::cout << "centered " << LRL_Cell_Degrees(pt->GetToCenter() * probe)
//          << pt->GetBravaisType() << " d=" << d
//          << std::endl << std::endl;
//    }
//}

int main(int argc, char* argv[])
{
   if (argc > 1) {
      selectBravaisCase = argv[1];
      if (argc > 2) {
         const double d = atof(argv[2]);
         if (d != 0.0) g_maxDeltaForMatch = atof(argv[2]);
      }
   }
   std::cout << "; SELLA\n";

   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   std::vector<MatS6> reductionMatrices;

   const std::vector<S6> vLat = GetInputSellingReducedVectors(inputList, reductionMatrices);
   //const std::vector<LRL_Cell> cells = CreateCells(inputList);
   const std::vector<S6> errors = CreateS6Errors(vLat);
   //const std::vector<LRL_Cell> cellErrors = CreateE3Errors(cells);

//-----------------------------------------------------------------------------------
   std::vector<std::shared_ptr<GenerateDeloneBase> > sptest =
      GenerateDeloneBase().Select(selectBravaisCase);


   for (size_t lat = 0; lat < vLat.size(); ++lat) {
      std::vector<DeloneFitResults> vDeloneFitResults = Sella().SellaFit(sptest, vLat[lat], errors[lat], reductionMatrices[lat]);

      //for (size_t kk = 0; kk < vDeloneFitResults.size(); ++kk) {
      //   const double d = vDeloneFitResults[kk].GetRawFit() / vLat[lat].norm();
      //    //if (vDeloneFitResults[kk].GetRawFit() / vLat[lat].norm() < g_maxDeltaForMatch) {
      //        NiggliMatchLatticeType(vDeloneFitResults[kk]);
      //    //}
      //}

      std::cout << std::endl << "; reported distances and zscores (in A^2)" << std::endl;

      const bool check = BravaisHeirarchy::CheckBravaisChains(vDeloneFitResults);

      std::cout << "; " << inputList[lat].GetStrCell() << " input data" << std::endl;
      //for (size_t kk = 0; kk < vDeloneFitResults.size(); ++kk) {
      //   std::cout << vDeloneFitResults[kk] << std::endl;
      //}
      //std::cout << vDeloneFitResults.size() << std::endl;
      ReportTypeHeirachy(vDeloneFitResults);
      const std::vector<std::pair<std::string, double> > scores = DeloneFitToScores(vDeloneFitResults);





      //std::cout << std::endl << std::endl << "lat " << lat << std::endl << std::endl << std::endl;
      /*std::cout << */BravaisHeirarchy::ProduceSVG(
         inputList[lat], vLat[lat], scores);
   }
}
