// Testbed.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include "B4.h"
#include "LRL_CreateFileName.h"
#include "D7.h"
#include "G6.h"
#include "D7Dist_.hpp"
#include "GenerateRandomLattice.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_MinMaxTools.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "LRL_MaximaTools.h"
#include "Niggli.h"
#include "PairReporter.h"
#include "S6.h"

#include "S6Dist.h"
#include "Selling.h"
#include "StoreResults.h"
#include "LRL_Vector3.h"

#include <functional>


//void TransformAndTestVector( const D7& testvec, const D7_Subboundary& sb ) {
//   const D7 transformedRandom = sb.GetTransform( ) * testvec;
//   const std::pair<int, std::string> id = D7_IdentifyNearbyBoundaries( transformedRandom, 10 );
//   boundarymap[sb.GetName( )] = std::make_pair( sb.GetName( ) + " transforms to  " + id.second, "" );
//}

template<typename T>
void TestToType() {
   T a, b;
   if (a.size() == 6) {
      a = T("10 11 12 90 90 90");          // from string
      b = T(G6("10 10 10  10 10 10 "));     // from G6
   }
   else if (a.size() == 7)
   {
      a = T("10 11 12 13 14 15 16");          // from string
      b = T("10 10 10  60 30 30 30 ");     // from G6
   }
   else if (a.size() == 4)
   {
      a = T(LRL_Cell("10 11 12 90 90 90"));
      b = T(G6("10 10 10  10 10 10 "));
   }
   else
   {
      throw;
   }
   D7 d7(b);
   T c(d7);                           // from D7
   T d(c);                            // from LRL_Cell
   S6 ds("5 5 5 -20 -20 -20");
   T e(ds);                           // from DeloneScalars (D6)
   B4 dt(G6("10 10 10  10 10 10 "));
   T f(dt);

   std::cout << a << std::endl;
   std::cout << b << std::endl;
   std::cout << c << std::endl;
   std::cout << d << std::endl;
   std::cout << e << std::endl;
   std::cout << f << std::endl;

   std::cout << std::endl;

   std::cout << LRL_ToString(a) << std::endl;
   std::cout << LRL_ToString(b) << std::endl;
   std::cout << LRL_ToString(c) << std::endl;
   std::cout << LRL_ToString(d) << std::endl;
   std::cout << LRL_ToString(e) << std::endl;
   std::cout << LRL_ToString(f) << std::endl;

}


int Negatives(const S6& s) {
   int n = 0;
   for (int i = 0; i < 6; ++i) n += (s[i] <= 0.0) ? 1 : 0;
   return n;
}

static const double pi = 4.0*atan(1.0);

bool BadTetrahedron(const S6& s6) {
   B4 b4(s6);
   const double angle_ad = Vector_3::Angle(b4[0], Vector_3::GetZeroVector(), b4[1]);
   const double angle_bd = Vector_3::Angle(b4[0], Vector_3::GetZeroVector(), b4[2]);
   const double angle_cd = Vector_3::Angle(b4[0], Vector_3::GetZeroVector(), b4[3]);

   if (angle_ad < 0.01) return true;
   if (angle_bd < 0.01) return true;
   if (angle_cd < 0.01) return true;
   if (angle_ad > 0.999*pi) return true;
   if (angle_bd > 0.999*pi) return true;
   if (angle_cd > 0.999*pi) return true;
   return false;
}

StoreResults<std::string, S6> g_storeRand(10);
StoreResults<int, LRL_Cell> g_store(10);
StoreResults<std::string, LRL_Cell> g_storeStr(10);
StoreResults<std::string, std::string> g_storeStrStr(10);

void Reflects() {
   std::vector<MatD7> refl;
   MatD7 ref;
   for (unsigned long i = 0; i < 24; ++i) {
      for (unsigned long k = 0; k < 49; ++k) {
         ref[k] = D7Refl[i][k];
      }
      refl.push_back(ref);
   }
   for (unsigned long i = 0; i < 1000; ++i) {  // 100000000
      S6 s6(S6::rand());
      int n = Negatives(s6);
      LRL_Cell cell(s6);
      int invalids = 0;
      for (int k = 0; k < 6; ++k) {
         invalids += (cell[k] < 1.0E20) ? 0 : 1; // testing for invalid FP number
      }

      for (int k = 3; k < 6; ++k) {
         if (cell[k] < 0.01) ++invalids; // zero degree angle
         if (cell[k] > 0.999*pi) ++invalids;  // 180 degree angle
      }

      if (cell[3] + cell[4] + cell[5] > 2.0*pi) ++invalids; // cell angles sum to more than 360 degrees
      if (invalids == 0 && BadTetrahedron(s6)) ++invalids;
      const std::string signature = LRL_ToString(n) + S6::Signature(s6);
      if (invalids == 0) {
         g_storeRand.Store(signature, s6);
         S6 s6red;
         Selling::Reduce(s6, s6red);
         g_storeStrStr.Store(signature, LRL_ToString(s6) + "   " + LRL_ToString(s6red));
      }
      if (!g_storeStr.HasKey(signature) && invalids == 0) {
         for (unsigned long kkkk = 0; kkkk < 24; ++kkkk) {
            //g_store.Store(n, refl[kkkk]*cell);
            g_storeStr.Store(signature, refl[kkkk] * cell);
         }
      }
   }

   g_store.ShowResultsByKeyDescending();
   g_storeStrStr.ShowResultsByKeyDescending();
   std::cout << g_storeStr.GetHerald() << std::endl;
   std::vector < std::string > keys = g_storeStr.GetKeys();

   for (int i = 0; i < keys.size(); ++i) {
      const std::vector<std::pair<std::string, LRL_Cell> > v = g_storeStr.FindResult(keys[i]);
      /*if (v.size() > 0) */std::cout << "key " << keys[i] << "    samples = " << v.size() << "   count  " << g_storeStr.GetItemCount(keys[i]) << std::endl;
      for (int k = 0; k < v.size(); ++k) {
         std::cout << LRL_Cell_Degrees(v[k].second) << "    " << S6(v[k].second) << std::endl;
      }
      std::cout << std::endl;
   }
   g_storeRand.ShowResultsByKeyDescending();
}

void TestUnreducePairs() {
   // tests to show that the reduce and unreduce pairs plus the 24 reflections generate the same set of cells

   const S6 s6(LRL_Cell::randDeloneReduced());
   S6Dist s6dist(DBL_MAX);

   static std::vector< std::pair<S6(*)(const S6&), S6(*)(const S6&)> > unreducers = S6Dist::SetUnreduceFunctionPairs();

   std::vector<S6> vinside;
   std::vector<S6> voutside;

   for (unsigned long i = 0; i < unreducers.size(); ++i) {
      vinside.push_back(unreducers[i].first(s6));  // generate using the first of each unreduce pair of functions
      voutside.push_back(unreducers[i].second(s6));  // generate using the second of each unreduce pair of functions
   }

   voutside = s6dist.Generate24Reflections(voutside); // for outside, generate the 24 reflections for each unreduced cell

   std::cout << "vinside   " << vinside.size() << std::endl;
   for (unsigned long i = 0; i < vinside.size(); ++i) {
      std::cout << vinside[i] << std::endl;
   }
   std::cout << std::endl;

   std::cout << "voutside   " << voutside.size() << std::endl;
   for (unsigned long i = 0; i < voutside.size(); ++i) {
      std::cout << voutside[i] << std::endl;
   }
   std::cout << std::endl;

   std::cout << "vinside   Cell, unreduced angles in degrees       " << vinside.size() << std::endl;
   for (unsigned long i = 0; i < vinside.size(); ++i) {
      std::cout << LRL_Cell_Degrees(LRL_Cell(vinside[i])) << std::endl;
   }
   std::cout << std::endl;

   std::cout << "vinside   NIGGLI REDUCED --  G6        " << vinside.size() << std::endl;
   for (unsigned long i = 0; i < vinside.size(); ++i) {
      S6 s6out;
      Niggli::Reduce<S6>((vinside[i]), s6out);
      std::cout << G6(s6out) << std::endl;
   }
   std::cout << std::endl;

   const CNearTree<S6> tree = voutside;

   for (unsigned long i = 0; i < vinside.size(); ++i) {
      CNearTree<S6>::iterator it = tree.NearestNeighbor(0.0001, vinside[i]);
      std::cout << i << "  " << ((it == tree.end()) ? "bad" : "good ") << std::endl;
   }
}

void TestSellingReduceUnreduceFunctions() {
   // test of functions for Selling reduce/unreduce
   //std::vector< std::pair<S6(*)(const S6&), S6(*)(const S6&)> > vf =S6Dist::SetUnreduceFunctionPairs();
   //S6 temp;
   //temp[0] = 1;
   //temp[1] = 3;
   //temp[2] = 5;
   //temp[3] = 7;
   //temp[4] = 11;
   //temp[5] = 13;
   //std::cout <<  vf[0].first(temp) << std::endl;;
   //std::cout <<  vf[0].second(temp) << std::endl;

   //exit(0);

   //const std::vector<std::pair<MatS6, MatS6> > vRed = S6Dist::SetunreductionReductionMatricesFromReductionMatrices();
   //for (unsigned long i = 0; i < vRed.size(); ++i) {
   //   std::cout << "For scale " << i << "1" << std::endl;
   //   std::cout << MaximaTools::MaximaFromMat(vRed[i].first) << std::endl << std::endl;
   //   std::cout << "For scale " << i << "2" << std::endl;
   //   std::cout << MaximaTools::MaximaFromMat(vRed[i].second) << std::endl << std::endl;
   //}
   //exit(0);
   //std::list<S6> vs6;
   //for (unsigned long i = 0; i < 5000000; ++i) {
   //   Cell cl = Cell::rand();
   //   vs6.push_back(S6(cl));;
   //}
}

void TestReductionTiming() {
   //std::list<S6>::iterator it;

   //S6 red2;
   //std::cout << CreateFileName::Create("", "") << std::endl;;
   //unsigned long trials1 = vs6.size()/100;
   //std::cout << trials1 << std::endl;
   //for (it = vs6.begin(); it != vs6.end(); ++it) {
   //   S6 red1 = Delone::Reduce(*it);
   //}
   //std::cout << CreateFileName::Create("", "") << std::endl;;

   //unsigned long trials2 = vs6.size();
   //std::cout << trials2 << std::endl;
   //for (it = vs6.begin(); it != vs6.end(); ++it) {
   //   Delone::ReduceUnsortedByFunction(*it, red2);
   //}
   //std::cout << CreateFileName::Create("", "") << std::endl;;
   //exit(0);

}

void TestConversions() {
   //-------------------------------------------------------------------------------------------
   LRL_Cell c;
   c[0];
   std::cout << LRL_Cell::GetName() << std::endl;
   TestToType<LRL_Cell>();

   std::cout << std::endl << std::endl;
   std::cout << G6::GetName() << std::endl;
   TestToType<G6>();

   std::cout << std::endl << std::endl;
   std::cout << D7::GetName() << std::endl;
   TestToType<D7>();

   std::cout << std::endl << std::endl;
   std::cout << S6::GetName() << std::endl;
   TestToType<S6>();

   std::cout << std::endl << std::endl;
   std::cout << B4::GetName() << std::endl;
   TestToType<B4>();
}

void CompareBothUnreduces() {
   // tests to show that the reduce and unreduce pairs plus the 24 reflections generate the same set of cells

   const S6 s6(LRL_Cell::randDeloneReduced());
   std::cout << " initial REDUCED input  " << LRL_MaximaTools::MaximaFromString(LRL_ToString(s6)) << std::endl;
   S6Dist s6dist(DBL_MAX);

   // get the functions that do the pairs of unreduces
   static std::vector< std::pair<S6(*)(const S6&), S6(*)(const S6&)> > unreducers = S6Dist::SetUnreduceFunctionPairs();

   std::vector<S6> vinside;
   std::vector<S6> voutside;

   // unreduce by each of the 6 unreduces of the two types
   for (unsigned long i = 0; i < unreducers.size(); ++i) {
      vinside.push_back(unreducers[i].first(s6));  // generate using the first of each unreduce pair of functions
      voutside.push_back(unreducers[i].second(s6));  // generate using the second of each unreduce pair of functions
   }

   // for outside, generate the 24 reflections for each unreduced cell
   voutside = s6dist.Generate24Reflections(voutside);

   std::cout << "vinside   " << vinside.size() << std::endl;
   for (unsigned long i = 0; i < vinside.size(); ++i) {
      std::cout << LRL_MaximaTools::MaximaFromString(LRL_ToString(vinside[i])) << std::endl;
   }
   std::cout << std::endl;

   std::cout << "voutside   " << voutside.size() << std::endl;
   for (unsigned long i = 0; i < voutside.size(); ++i) {
      std::cout << LRL_MaximaTools::MaximaFromString(LRL_ToString(voutside[i])) << std::endl;
   }
   std::cout << std::endl;


   const CNearTree<S6> tree = voutside;

   // put a bad one in
   vinside.push_back(1.0001 * vinside[0]);

   // see if every one of the insides is in outside
   // this can never be true for the different unreduce targets, so we get
   // no spurious results
   // MAKE SURE THAT THE LAST ONE IS BAD !!!!!
   for (unsigned long i = 0; i < vinside.size(); ++i) {
      CNearTree<S6>::iterator it = tree.NearestNeighbor(0.0001, vinside[i]);
      std::cout << i << "  " << ((it == tree.end()) ? "bad" : "good ") << std::endl;
   }
}

void TestDistanceWhenReducing() {
   static std::vector< std::pair<S6(*)(const S6&), S6(*)(const S6&)> > unreducers = S6Dist::SetUnreduceFunctionPairs();

   for (unsigned long i = 0; i < 10; ++i) {
      const S6 sB(S6::randDeloneReduced());
      const S6 sA(S6::randDeloneReduced());
      const S6 sAu(unreducers[0].first(sA));
      const S6 sBu(unreducers[0].first(sB));
      const double normA = (sA - sB).norm();
      const double normB = (sAu - sBu).norm();
      std::cout << sA << "   " << sA.norm() << std::endl;
      std::cout << sB << "   " << sB.norm() << std::endl;
      std::cout << std::endl;
      std::cout << sAu << "   " << sAu.norm() << std::endl;
      std::cout << sBu << "   " << sBu.norm() << std::endl;
      std::cout << normA << "   " << normB << std::endl;
      std::cout << normA / normB << std::endl;
      std::cout << std::endl;
      std::cout << std::endl;

   }
}
void TestRandCell() {
   StoreResults<int, LRL_Cell_Degrees> sr_rand(5);
   StoreResults<int, LRL_Cell_Degrees> sr_Delone(5);
   StoreResults<int, LRL_Cell_Degrees> sr_failed(20);
   sr_failed.SetTitle("FAILED CELLS");
   sr_failed.SetFooter("***********************************************");
   GenerateRandomLattice<S6> grl(19191);
   grl.SetLengthLimits(10.0, 90.0);
   for (unsigned long i = 0; i < 10000; ++i) {
      const LRL_Cell cell = grl.Generate();
      LRL_Cell_Degrees deg(cell);
      const bool b = deg.GetValid();
      const int key = (deg[3] + deg[4] + deg[5]);
      sr_rand.Store(key, deg);
      S6 out;
      Selling::Reduce(cell, out);
      LRL_Cell_Degrees celld(out);
      const double celld1 = celld[0];
      const int keyDelone = (celld[3] + celld[4] + celld[5]);
      sr_Delone.Store(keyDelone, LRL_Cell_Degrees(out));
      if (celld1 == 0.0) sr_failed.Store(0, cell);
   }
   sr_failed.ShowResults();
   if (sr_failed.size() > 19) exit(0);
   //sr_rand.ShowResults();
   sr_Delone.ShowResults();
   exit(0);
}


void TestGenerateRandomLattice() {
   std::cout << "testing GenerateRandomLattice" << std::endl;
   TestRandCell();
   GenerateRandomLattice<S6> grl(19191);

   StoreResults<int, PairReporter<S6, LRL_Cell> > store(5);
   for (unsigned long i = 0; i < 4000; ++i) {
      S6 sa = grl.Generate();
      //std::cout << " " << sa << "    " << LRL_Cell_Degrees(sa) << std::endl;
      LRL_Cell cell(sa);
      const double ratio = maxNC(cell[0], cell[1], cell[2]) / minNC(cell[0], cell[1], cell[2]);
      PairReporter<S6, LRL_Cell> p(sa, LRL_Cell(sa));
      store.Store(int(10.0*ratio), p);
   }
   store.ShowResultsByKeyAscending();
   std::cout << "finish testing GenerateRandomLattice" << std::endl;
}

void TestCellParams() {
   std::cout << S6(LRL_Cell(" 10 10 10 85 85 85")) << std::endl;
   std::cout << S6(LRL_Cell(" 10 10 20 85 85 85")) << std::endl;
   std::cout << S6(LRL_Cell(" 10 10 30 85 85 85")) << std::endl;
   std::cout << S6(LRL_Cell(" 10 10 40 85 85 85")) << std::endl;
   std::cout << S6(LRL_Cell(" 10 10 50 85 85 85")) << std::endl;
   std::cout << S6(LRL_Cell(" 10 10 60 85 85 85")) << std::endl;
   std::cout << S6(LRL_Cell(" 10 10 70 85 85 85")) << std::endl;
   std::cout << S6(LRL_Cell(" 10 10 80 85 85 85")) << std::endl;
}

void Test_ompPS6Speed() {
   S6Dist s6dist(1);
   std::cout << LRL_CreateFileName::Create("", "") << std::endl;
   std::vector<S6> v;
   for (unsigned long i = 0; i < 10000; ++i) {
      v.push_back(S6::randDeloneUnreduced());
   }
   //#pragma omp parallel
   {
      //#pragma omp for schedule(dynamic)
      for (unsigned long i = 0; i < v.size() - 1; ++i) {
         s6dist.DistanceBetween(v[i], v[i + 1]);
      }
   }
   std::cout << LRL_CreateFileName::Create("", "") << std::endl;
}

void TestStdGenerate() {
   const int numberToGenerate = 30;
   std::cout << "TestStdGenerate" << std::endl;
   std::vector<S6> v(numberToGenerate);
   GenerateRandomLattice<S6> grl(1);
   std::generate_n(v.begin(), v.size(), std::bind(std::mem_fun(&GenerateRandomLattice<S6>::rand), &grl));
   std::cout << LRL_ToString(v) << std::endl;
   std::cout << LRL_ToString(v) << std::endl;
   // std::bind(std::mem_fun(&GenerateRandomLattice<S6>::rand), &grl);
   // std::bind(std::mem_fun(&TaskFactory::getTask), &factory)
}

void TestGetBestPairS6() {
   S6Dist s6dist(1);
   std::cout << S6Dist::ReportS6Best(s6dist) << std::endl;
   GenerateRandomLattice<S6> grl(19191);
   grl.SetLengthLimits(10.0, 90.0);

   const double d1 = s6dist.DistanceBetween(grl.Generate(), grl.Generate());
   std::cout << S6Dist::ReportS6Best(s6dist) << std::endl;

   const double d2 = s6dist.DistanceBetween(grl.Generate(), grl.Generate());
   std::cout << S6Dist::ReportS6Best(s6dist) << std::endl;
}

void TestS6Speed() {
   std::vector<S6> v;
   const unsigned long nv = 1000000;
   std::cout << "number of distances = " << nv << std::endl;
   int seed = 10;
   GenerateRandomLattice<S6> grl(seed);
   for (unsigned long i = 0; i <= nv; ++i)
      v.push_back(grl.GenerateExtreme());

   std::cout << LRL_CreateFileName().Create("", "") << std::endl;
   S6Dist s6dist(10);
   for (unsigned long i = 0; i < nv; ++i)
      s6dist.DistanceBetween(v[i], v[i + 1]);
   std::cout << LRL_CreateFileName().Create("", "") << std::endl;
   exit(0);
}

void TestFollowStarter()
{

   //TestS6Speed();
   StoreResults<int, LRL_Cell> store(330);
   for (unsigned long i = 0; i < 100000; ++i) {
      const S6 s6 = GenerateRandomLattice<S6>::GenerateExtreme();
      LRL_Cell cell = LRL_Cell(s6);
      double d = cell[0];
      //if (d < 1.e-6) std::cout << s6 << std::endl;

      store.Store(S6::CountPositive(s6), LRL_Cell_Degrees(cell));
      bool test = s6.IsValid();
      if (!test) {
         std::cout << i << "                   " << s6.GetValid() << "      " << cell.GetValid() << "  " << s6 <<
            "  " << LRL_Cell_Degrees(cell) << std::endl << std::endl;
         store.Store(-1, LRL_Cell_Degrees(cell));
      }
   }
   store.ShowResults();
   exit(0);
}

void TestDataForHJB() {
   std::vector< C3(*)(const C3&)> fnRefl = C3::SetReflections();
   const C3 checkValue = S6(" 0 -10 -11 -12 -13 -14");

   for (unsigned long i = 0; i < fnRefl.size(); ++i) {
      S6 s6refl = fnRefl[i](checkValue);
      for (unsigned long k = 0; k < 6; ++k) {
         if (s6refl[k] == 0.0) s6refl[k] = -0.1;
      }
      std::cout << " " << LRL_MaximaTools::MaximaFromString(LRL_ToString(s6refl)) << std::endl;
   }
   exit(0);
}

double PositiveDistance( const S6& s6 ) {
   double sum = 0;
   double sqsum = 0;
   for (unsigned long i = 0; i < 6; ++i) {
      sum += (s6[i] > 0.0) ? s6[i] * s6[i] : 0.0;
      sqsum += s6[i] * s6[i];
   }
   return sqrt(sum / sqsum);
}

void CatalogByReductionSteps( void ) {
   GenerateRandomLattice<S6> grl(19191);
   StoreResults<unsigned long, std::string> storeExtreme(3);
   storeExtreme.SetTitle("Extreme Unreduced S6 Generation -- >1000 cycles means reduction failed");
   storeExtreme.SetKeyLabel("cycles");
   StoreResults<unsigned long, std::string> storesimple(3);
   storesimple.SetTitle("S6 Selling Unreduced Generation -- >1000 cycles means reduction failed");
   storesimple.SetKeyLabel("cycles");
   S6 s6in, out;
   bool b;

   for (unsigned long i = 0; i < 10000; ++i) {
      s6in = grl.GenerateExtreme();
      if (s6in.GetValid()) {
         b = Selling::Reduce(s6in, out);
         storeExtreme.Store(Selling::GetCycles(), LRL_ToString(" ", s6in, " ", Selling::GetCycles(), " ", PositiveDistance(s6in)));
      }

      s6in = grl.randSellingUnreduced();
      LRL_Cell cell = LRL_Cell(s6in);
      if (cell.GetValid()) {
         double test = std::abs(39.4918 - s6in[0]);
         b = Selling::Reduce(s6in, out);
         LRL_Cell cellout = LRL_Cell(out);
         storesimple.Store(Selling::GetCycles(), LRL_ToString(" ", s6in, " ", Selling::GetCycles(), " ", PositiveDistance(s6in)));
      }

   }
   storesimple.ShowResultsByKeyAscending();
   std::cout << std::endl;
   storeExtreme.ShowResultsByKeyAscending();
   exit(0);
}

void ListS6ReductionSteps() {
   Selling::SetListSteps(true);
   std::cout << "begin ListS6ReductionSteps " << std::endl;
   const S6 s6(" 39.49181 -151.76318  93.02526 -9821.60744 -173.91829  67.90784");
   //const S6 s6("  7.45210 -41.26590   2.45653 -30.68298 -11.34161 -44.64461");
   //const S6 s6("  972.88190 1376.50614 4087.55999 -13378.54258 -8221.08084 -2699.18238 "); 
   std::cout << s6 << std::endl;
   std::cout << LRL_Cell_Degrees(s6) << "   valid= " << LRL_Cell(s6).GetValid() << std::endl << std::endl;
   S6 out;
   S6Dist s6dist(1);
   const bool b = Selling::Reduce(s6, out);
   std::cout << "cycles " << Selling::GetCycles() << std::endl;
   std::cout << "input cell  " << LRL_Cell_Degrees(s6) << std::endl;
   std::cout << "output cell  " << LRL_Cell_Degrees(out) << std::endl;
   std::cout << "end ListS6ReductionSteps " << std::endl;
   Selling::SetListSteps(false);
   exit(0);
}

int main(int argc, char *argv[])
{
   //ListS6ReductionSteps();
   CatalogByReductionSteps();
   TestDataForHJB();
   TestFollowStarter();
   //ProgressStep ps;
   //ps.AddType<S6>();
   //ProgressPoint<S6> pp(5, 0.0);
   //pp.SetBest(S6("1 2 3 4 5 6"), S6());
   //TestGetBestPairS6();

   exit(0);
   TestStdGenerate();
   Test_ompPS6Speed();
   TestGenerateRandomLattice();
   TestCellParams();

   TestDistanceWhenReducing();
   for (unsigned long i = 0; i < 10000000; ++i) {
      const S6 s6(S6::randDeloneUnreduced());
      if (!s6.GetValid()) {
         throw;
      }
   }
   exit(0);
   std::cout << LRL_ToString(S6("1 2 3 4 5 6")) << std::endl;
   std::cout << LRL_ToString(G6("1 2 3 4 5 6")) << std::endl;
   std::cout << LRL_ToString(D7("1 2 3 4 5 6 7")) << std::endl;
   std::cout << LRL_ToString(LRL_Cell("10 20 40 60 70 80")) << std::endl;
   std::cout << LRL_ToString(Vector_3(1, 2, 3)) << std::endl;

   MatB4 b4("1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6");
   MatS6 s6;
   MatG6 g6;
   MatD7 d7;


   std::cout << "MatB4" << std::endl << LRL_ToString(MatB4("1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6")) << std::endl;
   std::cout << "MatS6" << std::endl << LRL_ToString(MatS6()) << std::endl;
   std::cout << "MatG6" << std::endl << LRL_ToString(MatG6()) << std::endl;
   std::cout << "MatD7" << std::endl << LRL_ToString(MatD7()) << std::endl;

   std::cout << "main file name *** " << __FILE__ << " ***" << std::endl;

   CompareBothUnreduces();
   TestUnreducePairs();

   //Selling::SetDebug(true);

   TestConversions();

   LRL_Cell c1("10 20 40 60 70 80");
   S6 s1(c1);
   Reflects();
   exit(0);
   return 0;
}

