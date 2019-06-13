// Testbed.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include "ColorTables.h"

#include "Allman.h"
#include "B4.h"
#include "CellInputData.h"
#include "LRL_CreateFileName.h"
#include "CS6Dist.h"
#include "D7.h"
#include "G6.h"
#include "D7Dist_.hpp"
#include "Delone.h"
#include "GenerateRandomLattice.h"
#include "DeloneTypeList.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_CreateFileName.h"
#include "LRL_DataToSVG.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "LRL_MaximaTools.h"
#include "MatS6.h"
#include "LRL_MaximaTools.h"
#include "NCDist.h"
#include "Niggli.h"
#include "PairReporter.h"
#include "S6.h"
#include "V7Dist.h"

#include "S6Dist.h"
#include "Selling.h"
#include "StoreResults.h"
#include "TNear.h"
#include "LRL_Vector3.h"

#include <ctime>
#include <functional>


#define CS6M_S6toD7 (s6vec, d7vec) { \
    d7vec[0] = -s6vec[3] -s6vec[2] -s6vec[1];\
    d7vec[1] = -s6vec[4] -s6vec[2] -s6vec[0];\
    d7vec[2] = -s6vec[5] -s6vec[1] -s6vec[0];\
    d7vec[3] = -s6vec[3] -s6vec[4] -s6vec[5];\
    d7vec[4] = -s6vec[4] -s6vec[2] -s6vec[5] -s6vec[1];\
    d7vec[5] = -s6vec[3] -s6vec[2] -s6vec[5] -s6vec[0];\
    d7vec[6] = -s6vec[3] -s6vec[1] -s6vec[4] -s6vec[0];\
}

#define CS6M_D7toS6(d7vec, s6vec) { \
    s6vec[0] = -(-d7vec[4]+d7vec[2]+d7vec[1])/2;\
    s6vec[1] = -(-d7vec[5]+d7vec[2]+d7vec[0])/2;\
    s6vec[2] = (-d7vec[5]-d7vec[4]+d7vec[3]+d7vec[2])/2;\
    s6vec[3] = -(-d7vec[4]+d7vec[3]+d7vec[0])/2;\
    s6vec[4] = -(-d7vec[5]+d7vec[3]+d7vec[1])/2;\
    s6vec[5] = (-d7vec[5]-d7vec[4]+d7vec[1]+d7vec[0])/2;\
}


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

static const double piTestbed = 4.0*atan(1.0);

bool BadTetrahedron(const S6& s6) {
   B4 b4(s6);
   const double angle_ad = Vector_3::Angle(b4[0], Vector_3::GetZeroVector(), b4[1]);
   const double angle_bd = Vector_3::Angle(b4[0], Vector_3::GetZeroVector(), b4[2]);
   const double angle_cd = Vector_3::Angle(b4[0], Vector_3::GetZeroVector(), b4[3]);

   if (angle_ad < 0.01) return true;
   if (angle_bd < 0.01) return true;
   if (angle_cd < 0.01) return true;
   if (angle_ad > 0.999*piTestbed) return true;
   if (angle_bd > 0.999*piTestbed) return true;
   if (angle_cd > 0.999*piTestbed) return true;
   return false;
}

StoreResults<std::string, S6> g_storeRand(10);
StoreResults<int, LRL_Cell> g_store(10);
StoreResults<std::string, LRL_Cell> g_storeStr(10);
StoreResults<std::string, std::string> g_storeStrStr(10);
StoreResults<unsigned long, unsigned long> g_storeReduceCount(0);

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
         if (cell[k] > 0.999*piTestbed) ++invalids;  // 180 degree angle
      }

      if (cell[3] + cell[4] + cell[5] > 2.0*piTestbed) ++invalids; // cell angles sum to more than 360 degrees
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
      const std::vector<std::pair<std::string, LRL_Cell> > v = g_storeStr.GetResult(keys[i]);
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
   std::list<LRL_Cell> vcell;
   std::list<LRL_Cell>::iterator itcell;

   GenerateRandomLattice<S6> grl(19191);
   grl.SetLengthLimits(10.0, 90.0);
   for (unsigned long i = 0; i < 100000; ++i) {
      //const S6 s6 = S6::randDeloneUnreduced();
      const S6 s6 = G6::rand();
      vcell.push_back(s6);
   }

   LRL_Cell dummy;
   std::clock_t start;

   {
      g_storeReduceCount.clear();
      start = std::clock();
      S6 red1;
      for (itcell = vcell.begin(); itcell != vcell.end(); ++itcell) {
         const bool b = Selling::Reduce(*itcell, red1);
         dummy = red1;
         g_storeReduceCount.Store(Selling::GetCycles(), 0);
      }
      dummy *= 2.0; // to try to get the optimizer to keep calculating dummy as unit cell parmeters
      g_storeReduceCount.SetKeyLabel("Random Selling Reduction Cycles");
      g_storeReduceCount.ShowResultsByKeyAscending();
      std::cout << LRL_CreateFileName::Create("Selling_", "") << "  " << std::clock() - start << " msec" << std::endl;
   }

   {
      start = std::clock();
      G6 red6;
      for (itcell = vcell.begin(); itcell != vcell.end(); ++itcell) {
         const bool b = Niggli::Reduce(*itcell, red6);
         dummy = red6;
      }
      dummy *= 2.0; // to try to get the optimizer to keep calculating dummy as unit cell parmeters
      std::cout << LRL_CreateFileName::Create("Niggli_", "") << "  " << std::clock() - start << " msec" << std::endl;
   }

   {
      start = std::clock();
      S6 red1;
      for (itcell = vcell.begin(); itcell != vcell.end(); ++itcell) {
         const bool b = Delone::Reduce(*itcell, red1);
         dummy = red1;
      }
      dummy *= 2.0; // to try to get the optimizer to keep calculating dummy as unit cell parmeters
      std::cout << LRL_CreateFileName::Create("Delone_", "") << "  " << std::clock() - start << " msec" << std::endl;
   }

   exit(0);

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
      const int key = int(deg[3] + deg[4] + deg[5]);
      sr_rand.Store(key, deg);
      S6 out;
      Selling::Reduce(cell, out);
      LRL_Cell_Degrees celld(out);
      const double celld1 = celld[0];
      const int keyDelone = int(celld[3] + celld[4] + celld[5]);
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

unsigned long cycles = 100000;
StoreResults<int, int> store(0);

class TestStaticSpeed {
public:
   TestStaticSpeed() {
      for (unsigned long i = 0; i < 10000000; ++i) { v.push_back(double(i)); }
   }

   static double mulStatic(const double a, std::vector<double>& v) {
      double d[4];
      for (unsigned long j = 0; j < cycles; ++j)
         for (unsigned long k = 0; k < 10 * cycles; ++k) {
            for (unsigned long i = 0; i < v.size(); ++i) {
               d[k % 4] = a * v[i];
            }
            if (k == 0) store.Store(k, 0);
         }
      double test = d[0];
      return test;
   }

   double mulPlain(const double a, std::vector<double>& vin) {
      double d[4];
      for (unsigned long j = 0; j < cycles; ++j)
         for (unsigned long k = 0; k < 10 * cycles; ++k) {
            for (unsigned long i = 0; i < vin.size(); ++i) {
               d[k % 4] = a * vin[i];
            }
            if (k == 0) store.Store(k, 0);
         }
      double test = d[0];
      return test;
   }


public:
   std::vector<double> v;
};

void TestStatic() {
   double test1, test2;
   std::vector<double> v;
   TestStaticSpeed tss;
   test1 = TestStaticSpeed::mulStatic(1.0, v);
   store.ShowResults();
   store.clear();
   test2 = tss.mulPlain(1.0, v);
   std::cout << test1 << " " << test2 << std::endl;
   store.ShowResults();
   exit(0);
}

int seed = 19191;
void TestCS6Dist1() {
   GenerateRandomLattice<S6> generator(seed);
   for (unsigned long i = 0; i < 1000; ++i) {
      const S6 s6 = generator.randSellingReduced();
      const double d = CS6Dist(s6.data(), s6.data());
      if (d > 1.0) {
         std::cout << i << "   d=" << d << "      " << s6 << "      " << C3(s6) << std::endl;
      }
   }
   //exit(0);
}

void TestCS6Dist2() {
   double ar[] = { 0,3,5,7,11,13,17,19 };
   for (unsigned long i = 0; i < 100; ++i) {
      double s6[6];
      for (unsigned long k = 0; k < 6; ++k) {
         s6[k] = -ar[rand() % 8];
      }
      printf("\n  i = %d\n", i);
      printf("before %8f  %8f  %8f  %8f  %8f  %8f \n", s6[0], s6[1], s6[2], s6[3], s6[4], s6[5]);
      const double d = CS6Dist(s6, s6);
      if (d > 1.0) {
         printf("after  %8f  %8f  %8f  %8f  %8f  %8f        %8f  *****************\n", s6[0], s6[1], s6[2], s6[3], s6[4], s6[5], d);
         const int i19191 = 19191;;
      }
   }
   exit(0);
}

void TestCS6Dist3() {
   std::vector< S6(*)(const S6&)> refl = S6::SetRelectionFunctions();
   GenerateRandomLattice<S6> generator(seed);
   for (unsigned long i = 0; i < 1000; ++i) {
      S6 s6red;
      Delone::Reduce(generator.randSellingReduced(), s6red);
      for (unsigned long ir = 0; ir < 24; ++ir) {
         const S6 s6 = refl[ir](s6red);
         const double d = CS6Dist(s6.data(), s6.data());
         if (d > 1.0) {
            double ar[7];
            std::vector<double> v = s6.GetVector();
            std::cout << i << "  " << ir << "   d=" << d << "      " << s6 << "      " << C3(s6) << std::endl;
         }
      }
   }
   exit(0);
}

void TestCS6Dist4() {
   GenerateRandomLattice<S6> generator(seed);
   StoreResults<double, std::string> storeLocal(1);
   S6Dist s6dist(1);
   std::vector<S6> voutside;
   S6 red1, red2;
   for (unsigned long i2 = 0; i2 < 1000; ++i2) {
      const S6 s1(generator.randSellingUnreduced());
      Selling::Reduce(s1, red1);
      voutside = s6dist.Generate24Reflections(s1); // for outside, generate the 24 reflections for each unreduced cell
      for (unsigned long i1 = 0; i1 < 24; ++i1) {
         Selling::Reduce(voutside[i1], red2);
         const double d1 = CS6Dist(red2.data(), red1.data());
         const double d2 = s6dist.DistanceBetween(red2.data(), red1.data());
         const double dfinal = (d1 < 1.0E-4) ? 0.0 : d1;
         const std::string s = LRL_ToString(" ", " ", red2, "\n", red1, "\n", LRL_Cell_Degrees(red2), "\n");
         storeLocal.Store(dfinal, s);
      }
   }
   storeLocal.ShowResults();
   exit(0);
}

void TestCS_Reflections() {
   CNearTree<MatS6> tree;
   std::vector<MatS6> vm(24);
   MatS6 closest;
   const double radius = 0.1;
   for (unsigned long im = 0; im < 24; ++im) {
      for (unsigned long i = 0; i < 36; ++i) {
         vm[im][i] = **(S6Refl + i);
      }
      tree.insert(vm[im]);
      std::cout << std::endl << im << std::endl;
      std::cout << vm[im] << std::endl;
   }
   const double d = S6Refl[0][0];
   exit(0);
}

void TestReflVCP() {
   StoreResults<unsigned long, std::string> storeLocal(5);
   storeLocal.SetKeyLabel("Reflection#");
   const static std::vector< std::pair<S6(*)(const S6&), S6(*)(const S6&)> > unredfun = S6Dist::SetUnreduceFunctionPairs();
   const static std::vector< S6(*)(const S6&)> reflfun = S6::SetRelectionFunctions();
   int seed1 = 19;
   GenerateRandomLattice<S6> grl(seed1);
   for (unsigned long i = 0; i < 2; ++i) grl.randSellingReduced();
   const S6 start = grl.randSellingReduced();
   std::cout << "the initial inside point is: " << start << std::endl << std::endl;
   S6 in = unredfun[0].first(start);
   in[0] = 0.0001;

   std::cout << "the initial outside point is: " << in << std::endl << std::endl;

   for (unsigned long i = 0; i < 100000; ++i) {
      S6 svar = grl.randSellingReduced();
      svar[0] = -0.0001;
      if (svar.IsValid()) {
         double dmin = DBL_MAX;
         unsigned long kmin = ULONG_MAX;
         S6 ref;
         for (unsigned long k = 0; k < 24; ++k) {
            ref = reflfun[k](svar);
            if ((ref - in).norm() < dmin) {
               dmin = (ref - in).norm();
               kmin = k;
            }
         }
         storeLocal.Store(kmin, LRL_ToString(ref));
      }
   }

   storeLocal.ShowResults();
   exit(0);
}

void TestSellingTiming(const std::vector<S6>& testData) {

   for (unsigned long i = 0; i < testData.size(); ++i) {
      S6 s6out;
      Selling::Reduce(testData[i], s6out);
   }
}

void TestNiggliTiming(const std::vector<G6>& testData, const bool doSelling) {
   for (unsigned long i = 0; i < testData.size(); ++i) {
      G6 g6out;
      Niggli::Reduce(testData[i], g6out, doSelling);
   }
}

void TestCS6DistancesTiming(const std::vector<S6>& testData) {
   const unsigned long ntests = (unsigned long)(testData.size());
   const double distA = CS6Dist(testData[0].data(), testData[ntests - 1].data());
   for (unsigned long i = 0; i < ntests - 1; ++i) {
      const double dist = CS6Dist(testData[i].data(), testData[i + 1].data());
   }
}

void TestD7DistancesTiming(const std::vector<S6>& testData) {
   const unsigned long ntests = (unsigned long)(testData.size());
   const double distA = D7Dist(testData[0].data(), testData[ntests - 1].data());
   for (unsigned long i = 0; i < ntests - 1; ++i) {
      const double dist = D7Dist(testData[i].data(), testData[i + 1].data());
   }
}

void TestS6DistancesTiming(const std::vector<S6>& testData) {
   const unsigned long ntests = (unsigned long)(testData.size());
   S6Dist s6dist(1);
   const double distA = s6dist.DistanceBetween(testData[0], testData[ntests - 1]);
   for (unsigned long i = 0; i < ntests - 1; ++i) {
      const double dist = s6dist.DistanceBetween(testData[i], testData[i + 1]);
   }
}

void TestG6DistancesTiming(const std::vector<G6>& testData) {
   const unsigned long ntests = (unsigned long)(testData.size());
   const double distA = NCDist(testData[0].data(), testData[ntests - 1].data());
   for (unsigned long i = 0; i < ntests - 1; ++i) {
      NCDist(testData[i].data(), testData[i + 1].data());
   }
}

void TestV7DistancesTiming(const std::vector<G6>& testData) {
   const unsigned long ntests = (unsigned long)(testData.size());
   const double distA = V7Dist(testData[0].data(), testData[ntests - 1].data());
   for (unsigned long i = 0; i < ntests - 1; ++i) {
      V7Dist(testData[i].data(), testData[i + 1].data());
   }
}

void CheckReduceAndDistanceTiming() {
   StoreResults < std::string, double> storeLocal(100);
   const unsigned long nReduceSamples = 1000;
   std::cout << "number of test samples " << nReduceSamples << std::endl;
   int seed2(19191);
   GenerateRandomLattice<G6> grl(seed2);

   std::vector<S6> testDataS6REDUCED;
   for (unsigned long i = 0; i < nReduceSamples; ++i) {
      testDataS6REDUCED.push_back(grl.randSellingReduced());
   }

   std::vector<G6> testDataG6REDUCED;
   for (unsigned long i = 0; i < nReduceSamples; ++i) {
      G6 g6out;
      testDataG6REDUCED.push_back(G6(testDataS6REDUCED[i]));
   }

   std::vector<S6> unreducedTestDataS6;
   for (unsigned long i = 0; i < nReduceSamples; ++i) {
      unreducedTestDataS6.push_back(grl.GenerateExtreme());
   }

   std::vector<G6> unreducedTestDataG6;
   for (unsigned long i = 0; i < nReduceSamples; ++i) {
      G6 g6out;
      unreducedTestDataG6.push_back(G6(unreducedTestDataS6[i]));
   }

   auto start = std::clock();
   double endTime;

   for (unsigned long cycle = 0; cycle < 10; ++cycle) {
      std::cout << " cycle " << cycle << std::endl;
      //---------------------------------- test reduced reductions
      {
         start = std::clock();
         TestSellingTiming(testDataS6REDUCED);
         endTime = std::clock() - start;
         storeLocal.Store("Reduced Input Selling", endTime);

         start = std::clock();
         TestNiggliTiming(testDataG6REDUCED, true);
         endTime = std::clock() - start;
         storeLocal.Store("Reduced Input Niggli preSelling", endTime);

         start = std::clock();
         TestNiggliTiming(testDataG6REDUCED, false);
         endTime = std::clock() - start;
         storeLocal.Store("Reduced Input Niggli only Niggli", endTime);
      }

      //---------------------------------- test unreduced reductions
      {

         start = std::clock();
         TestSellingTiming(unreducedTestDataS6);
         endTime = std::clock() - start;
         storeLocal.Store("NOT Reduced Input Selling", endTime);

         start = std::clock();
         TestNiggliTiming(unreducedTestDataG6, true);
         endTime = std::clock() - start;
         storeLocal.Store("NOT Reduced Input Niggli, preSelling", endTime);

         start = std::clock();
         TestNiggliTiming(unreducedTestDataG6, false);
         endTime = std::clock() - start;
         storeLocal.Store("NOT Reduced Input Niggli, NOT preSelling", endTime);
      }

      {
         //---------------------------------- test S6 distances
         start = std::clock();
         TestS6DistancesTiming(testDataS6REDUCED);
         endTime = std::clock() - start;
         storeLocal.Store("Reduced S6Dist ", endTime);

         //---------------------------------- test CS6 distances
         start = std::clock();
         TestCS6DistancesTiming(testDataS6REDUCED);
         endTime = std::clock() - start;
         storeLocal.Store("Reduced CS6Dist ", endTime);

         //---------------------------------- test G6 distances
         start = std::clock();
         TestG6DistancesTiming(testDataG6REDUCED);
         endTime = std::clock() - start;
         storeLocal.Store("Reduced NCDist ", endTime);

         //---------------------------------- test D7 distances
         start = std::clock();
         TestD7DistancesTiming(testDataS6REDUCED);
         endTime = std::clock() - start;
         storeLocal.Store("Reduced D7Dist ", endTime);

         //---------------------------------- test V7 distances
         start = std::clock();
         TestV7DistancesTiming(testDataG6REDUCED);
         endTime = std::clock() - start;
         storeLocal.Store("Reduced V7Dist ", endTime);
      }
   }

   storeLocal.ShowResults();
   exit(0);
}

void LookAtReductions() {
   int seed4(19191);
   GenerateRandomLattice<G6> grl(seed4);
   S6 test;
   S6 s6out;
   G6 g6out;
   S6 deout;
   const unsigned long nReduceSamples = 10;
   for (unsigned long i = 0; i < nReduceSamples; ++i) {
      test = grl.GenerateExtreme();
      Selling::Reduce(test, s6out);
      Delone::Reduce(test, deout);
      Niggli::Reduce(G6(test), g6out);

      std::cout << "input (extreme) " << LRL_Cell_Degrees(test) << std::endl;
      std::cout << "Niggli reduced " << LRL_Cell_Degrees(g6out) << std::endl;
      std::cout << "Selling reduced " << LRL_Cell_Degrees(s6out) << std::endl;
      std::cout << "Delone reduced " << LRL_Cell_Degrees(deout) << std::endl;
      std::cout << "Allman " << LRL_Cell_Degrees(Allman::Reduce(g6out)) << std::endl << std::endl;
   }
   exit(0);
}

void VerifyIsNiggli() {
   StoreResults<bool, std::string> storeLocal(10);
   int seed6 = 9;
   GenerateRandomLattice<G6> grl(seed6);
   G6 vout;
   const unsigned long ntest = 10000;
   for (unsigned long i = 0; i < ntest; ++i) {
      const G6 g6 = grl.GenerateExtreme();
      storeLocal.Store(Niggli::IsNiggli(vout), LRL_ToString(LRL_Cell_Degrees(g6), "  ", LRL_Cell_Degrees(vout)));
   }
   storeLocal.ShowResults();
   exit(0);
}

void TimingForNewNiggliReduce() {
   std::vector<G6> v;
   const unsigned long nv = 100000;
   std::cout << "number of distances = " << nv << std::endl;
   int seed5 = 10;
   GenerateRandomLattice<G6> grl(seed5);
   for (unsigned long i = 0; i <= nv; ++i)
      v.push_back(grl.GenerateExtreme());

   auto start = std::clock();
   double elapsedTime;

   G6 vout;
   start = std::clock();
   for (unsigned long i = 0; i <= nv; ++i)
      bool b = Niggli::Reduce(v[i], vout);
   elapsedTime = std::clock() - start;
   std::cout << elapsedTime << std::endl;

   start = std::clock();
   for (unsigned long i = 0; i <= nv; ++i)
      bool b = Niggli::ReduceWithoutMatrices(v[i], vout, 0.0);
   elapsedTime = std::clock() - start;
   std::cout << elapsedTime << std::endl;
   exit(0);
}

void VerifyNiggli() {
   StoreResults<double, std::string> storeLocal(10);
   int seed3 = 9;
   //GenerateRandomLattice<G6> grl(seed3);
   //const unsigned long ntest = 100000;
   //for (unsigned long i = 0; i < ntest; ++i) {
   //   const G6 g6 = grl.GenerateExtreme();
   const std::vector<CellInputData> cellData = LRL_ReadLatticeData::ReadAllLatticeDataAndMakePrimitive(seed3);  // read the data

   const unsigned long n = (unsigned long)(cellData.size());
   std::cout << "number of cells read   " << n << std::endl;
   G6 vout1;
   G6 vout2;

   for (unsigned long i = 0; i < n; ++i) {
      LRL_Cell cell = cellData[i].GetCell();

      if (vout1 == vout2) storeLocal.Store(0, LRL_ToString(LRL_Cell_Degrees(vout1), "  ", LRL_Cell_Degrees(vout2)));
      else if ((vout1 - vout2).norm() < 1.0E-9) storeLocal.Store(1, LRL_ToString(LRL_Cell_Degrees(vout1), "  ", LRL_Cell_Degrees(vout2)));
      else storeLocal.Store(2, LRL_ToString((vout1 - vout2).norm(), "  ", LRL_ToString(LRL_Cell_Degrees(vout1), "  ", LRL_Cell_Degrees(vout2))));
   }
   storeLocal.ShowResults();
   exit(0);
}

/*
}

std::vector<S6> GetInputSellingReducedVectors(const std::vector<LRL_ReadLatticeData>& input) {
   std::vector<S6> v;
   LatticeConverter converter;


   for ( unsigned long i=0; i<input.size(); ++i ) {
      const LRL_ReadLatticeData& rcd = input[i];
      const S6 s6 = converter.SellingReduceCell(input[i].GetLattice(), input[i].GetCell());
      v.push_back(s6);
   }
   return v;
}

void AnalyzePDBCells(const std::vector<LRL_ReadLatticeData>& input) {
   StoreResults<std::string, std::string> storeGood(1);
   StoreResults<std::string, std::string> storeProblems(100);
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
      if (fit.second > 1.0E-5) storeProblems.Store(fit.first, LRL_ToString(" ", fit.first, "  ", fit.second, " (", normlat, ")  ",  strCell, "  ", vLat[lat]));
      if (fit.second < 1.0E-5) storeGood.Store(fit.first, LRL_ToString(" ", fit.first, " ", fit.second, "  ", strCell, "  ", vLat[lat]));
 //     std::cout << LRL_ToString(fit.first, "  ") << LRL_ToString( LRL_ToString(" ", fit.first, " ", fit.second, "  ", strCell, "  ", vLat[lat])) << std::endl;
   }
   storeProblems.ShowResults();
   storeGood.ShowResults();
   exit(0);
}
*/

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

LRL_Cell PDB_ReduceTimingTest() {
   const std::vector<LRL_ReadLatticeData> input = GetInputCells();
   std::vector<LRL_Cell> vcell;
   std::vector<std::string> vlattice;
   for (unsigned long i = 0; i < input.size(); ++i) {
      vcell.push_back(input[i].GetCell());
      vlattice.push_back(input[i].GetLattice());
   }

   LatticeConverter converter;
   LRL_Cell dummy;
   std::clock_t start;

   {
      start = std::clock();
      S6 red1;
      for (unsigned long i = 0; i < vcell.size(); ++i) {
         const LRL_Cell cell = converter.MakePrimitiveCell(vlattice[i], vcell[i]);
         const bool b = Selling::Reduce(cell, red1);
         dummy = red1;
         g_storeReduceCount.Store(Selling::GetCycles(), 0);
      }
      dummy *= 2.0; // to try to get the optimizer to keep calculating dummy as unit cell parmeters
      g_storeReduceCount.SetKeyLabel("PDB Selling Reduction Cycles");
      g_storeReduceCount.ShowResultsByKeyAscending();
      std::cout << LRL_CreateFileName::Create("PDB_Selling_", "") << "  " << std::clock() - start << " msec" << std::endl;
   }

   {
      start = std::clock();
      G6 red6;
      for (unsigned long i = 0; i < vcell.size(); ++i) {
         const LRL_Cell cell = converter.MakePrimitiveCell(vlattice[i], vcell[i]);
         const bool b = Niggli::Reduce(cell, red6);
         dummy = red6;
      }
      dummy *= 2.0; // to try to get the optimizer to keep calculating dummy as unit cell parmeters
      std::cout << LRL_CreateFileName::Create("PDB_Niggli_", "") << "  " << std::clock() - start << " msec" << std::endl;
   }

   {
      start = std::clock();
      S6 red1;
      for (unsigned long i = 0; i < vcell.size(); ++i) {
         const LRL_Cell cell = converter.MakePrimitiveCell(vlattice[i], vcell[i]);
         const bool b = Delone::Reduce(cell, red1);
         dummy = red1;
      }
      dummy *= 2.0; // to try to get the optimizer to keep calculating dummy as unit cell parmeters
      std::cout << LRL_CreateFileName::Create("PDB_Delone_", "") << "  " << std::clock() - start << " msec" << std::endl;
   }
   dummy *= 2.0;
   return dummy;
}

void F_makeS6UnPrimitiveMatrices() {
   const MatG6 fmat = LRL_Cell::G6MakePrimitiveMatrix("F");
   const MatN fmatinv = fmat.GetMatrix().inverse();

   const MatG6 s6Tog6(" 2 0 0 0 0 0   0 2 0 0 0 0   0 0 2 0 0 0 0 -1 -1 -1 0 0   -1 0 -1 0 -1 0   -1 -1 0 0 0 -1");
   const MatS6 g6Tos6 = s6Tog6.GetMatrix().inverse();
   //const MatS6 g6Tos6("0 0 0 .5 0 0   0 0 0 0 .5 0   0 0 0 0 0 .5   -1 0 0 0 -.5 -.5   0 -1 0 -.5 0 -.5   0 0 -1 -.5 -.5 0");
   std::cout << "should be unit matrix " << std::endl << s6Tog6 * g6Tos6 << std::endl << std::endl;
   std::cout << 1.0 - (s6Tog6 * g6Tos6).norm() << std::endl;
   const LRL_Cell f_G6_centered_cell("10 10 10  90 90 90");
   std::cout << "G6 input centered vector " << G6(f_G6_centered_cell) << std::endl;
   const S6 f_S6_centered_cell(f_G6_centered_cell);
   std::cout << "S6 input centered vector " << f_S6_centered_cell << std::endl << std::endl;

   std::cout << "G6  centered vector  " << f_G6_centered_cell << std::endl;


   std::cout << "S6  centered vector  " << S6(f_G6_centered_cell) << std::endl << std::endl;

   const MatS6 s6primconverter = s6Tog6 * fmat* g6Tos6;
   const S6 primcell = fmat * f_G6_centered_cell;
   std::cout << "primitive S6 using original matrix" << primcell << std::endl;
   std::cout << "prim cell from original " << LRL_Cell_Degrees(primcell) << std::endl << std::endl;
   std::cout << "primitive S6 with new matrix" << s6primconverter * f_S6_centered_cell << std::endl;
   std::cout << "prim cell from new matrix " << LRL_Cell_Degrees(s6primconverter * f_S6_centered_cell) << std::endl;
}
#include "S6.cpp"
int main(int argc, char *argv[])
{
	//std::vector<double> e3(9);
	//e3[0] = 1;
	//e3[1] = 1;
	//e3[2] = 0;

	//e3[3] = -1;
	//e3[4] = 1;
	//e3[5] = 0;

	//e3[6] = 1;
	//e3[7] = 1;
	//e3[8] = 2;
	//std::cout << "E3 matrix for P to F for cF  " << std::endl;
	//for (unsigned long i = 0; i < 9; ++i) {
	//	if (i % 3 == 0) std::cout << std::endl;
	//	std::cout << e3[i];
	//}
	
	std::cout << std::endl << std::endl;
	std::vector<std::pair<std::string, std::vector<double> > > vcenters = DeloneTypeList::Make3dCenteringMatrices();
	std::cout << "size" << vcenters.size() << std::endl;
	for (unsigned long i=0; i<vcenters.size(); ++i ) {
	   const MatS6 m6 = MatS6::e3Tos6(vcenters[i].second);
		std::cout << std::endl << "\"" << vcenters[i].first << "\", MatS6(" << std::endl << m6 << std::endl;
	}
	exit(0);
	//   std::cout << "MatS6 matrix for P to F for cF  " << std::endl << m6 << std::endl << std::endl;
	//   LRL_Cell cell(10, 10, 10,60,60,60);
	//   std::cout << "input cell " << LRL_Cell_Degrees(cell) << std::endl << std::endl;
	//S6 s6red;
	//Selling::Reduce(S6(cell), s6red);

	//std::cout << " result cell  " << LRL_Cell_Degrees(m6*s6red) << std::endl;
	//exit(0);


   PDB_ReduceTimingTest();
   // TimingForNewNiggliReduce();
   TestReductionTiming();

   exit(0);
   const std::vector<std::pair<MatS6, MatS6> > unred = S6::SetUnreductionMatrices();
   for (unsigned long i = 0; i < 24; ++i) std::cout << LRL_MaximaTools::MaximaFromMat(unred[i].first) << "   " <<
      LRL_MaximaTools::MaximaFromMat(unred[i].second) << std::endl;

   const std::vector<MatS6> vmats6 = MatS6::GetReflections();
   for (unsigned long i = 0; i < 24; ++i) std::cout << LRL_MaximaTools::MaximaFromMat(vmats6[i]) << std::endl;
   exit(0);
   VerifyNiggli();
   VerifyIsNiggli();
   S6 out;
   //LookAtReductions();
   CheckReduceAndDistanceTiming();
   TestReflVCP();
   //TestCS_Reflections();
   TestCS6Dist4();
   //TestCS6Dist1();
   TestCS6Dist2();
   //TestCS6Dist3();
   TestStatic();
   C3 c3 = G6("100 100 100 100 100 100");
   C3 c3a = 1.0*c3;
   C3 c3aa = c3 * 1.0;
   C3 c3b(c3);
   c3b *= 1.0;
   C3 c3c(1.0*c3);
   std::string svg = LRL_DataToSVG(1.0, 2.0, 3UL);
   exit(0);
   //ListS6ReductionSteps();
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

