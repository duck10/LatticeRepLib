// TestBases.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <string>

#include "B4.h"
#include "C3.h"
#include "D7.h"
#include "G6.h"
#include "S6.h"
#include "S6Dist.h"
#include "Selling.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "PairReporter.h"
#include "rhrand.h"
#include "StoreResults.h"


StoreResults<unsigned long, PairReporter<S6, LRL_Cell_Degrees> > g_storePositiveCount(4);
StoreResults<unsigned long, S6 > g_storePositiveCountFailed(4);
StoreResults<unsigned long, unsigned long > g_storeChoice(0);

static int s6RandomSeed = 1;
static RHrand rhrand(s6RandomSeed);

template<typename T1, typename T2>
int CheckEquality(const T1& t1, const T2& t2) {
   const double diff = (G6(t1) - G6(t2)).norm();
   int error = 0;
   if (diff / t1.norm() > 1.0E-10) {
      error = 1;
      std::cout << std::endl;
      std::cout << " DIFF " << diff << "  " << diff / t1.norm() << std::endl;
      std::cout << " t1 " << t1.GetName() << "  " << t1 << std::endl;
      std::cout << " t2 " << t2.GetName() << "  " << t2 << std::endl;
   }
   return error;
}

template<typename T>
int CheckFirstConstructors(const T& t, const LRL_Cell& cell) {
   int sum = 0;
   sum += CheckEquality(t, B4(cell));
   sum += CheckEquality(t, C3(cell));
   sum += CheckEquality(t, D7(cell));
   sum += CheckEquality(t, G6(cell));
   sum += CheckEquality(t, S6(cell));
   sum += CheckEquality(t, cell);
   return sum;
}

template<typename T>
int CheckFirstAssignments(const T& t, const LRL_Cell& cell) {
   int sum = 0;

   const B4 xB4 = B4(cell);
   const C3 xC3 = C3(cell);
   const D7 xD7 = D7(cell);
   const G6 xG6 = G6(cell);
   const S6 xS6 = S6(cell);

   sum += CheckEquality(t, xB4);
   sum += CheckEquality(t, xC3);
   sum += CheckEquality(t, xD7);
   sum += CheckEquality(t, xG6);
   sum += CheckEquality(t, xS6);
   sum += CheckEquality(t, cell);
   return sum;
}

int CheckConstructors(const std::string& s) {
   int errorcount = 0;
   const LRL_Cell cell = LRL_Cell(s);
   errorcount += CheckFirstConstructors(B4(cell), cell);
   errorcount += CheckFirstConstructors(C3(cell), cell);
   errorcount += CheckFirstConstructors(D7(cell), cell);
   errorcount += CheckFirstConstructors(G6(cell), cell);
   errorcount += CheckFirstConstructors(S6(cell), cell);
   errorcount += CheckFirstConstructors(cell, cell);
   return errorcount;
}

int CheckAssignments(const std::string& s) {
   int errorcount = 0;
   const LRL_Cell cell = LRL_Cell(s);

  const B4 xB4 = B4(cell);
  const C3 xC3 = C3(cell);
  const D7 xD7 = D7(cell);
  const G6 xG6 = G6(cell);
  const S6 xS6 = S6(cell);

   errorcount += CheckFirstAssignments(xB4, cell);
   errorcount += CheckFirstAssignments(xC3, cell);
   errorcount += CheckFirstAssignments(xD7, cell);
   errorcount += CheckFirstAssignments(xG6, cell);
   errorcount += CheckFirstAssignments(xS6, cell);
   errorcount += CheckFirstAssignments(cell, cell);
   return errorcount;
}

void TestS6_rand() {
   LRL_Cell cell(LRL_Cell().rand());

   for (unsigned long kkkk = 0; kkkk < 1000; ++kkkk) {
      const S6 s6 = S6::rand();
      const double last = s6[5];
      const unsigned long positive = S6::CountPositive(s6);
      const LRL_Cell_Degrees c = LRL_Cell_Degrees(s6);
      const PairReporter<S6, LRL_Cell_Degrees> dataPair(std::make_pair(s6, c));
      if (c.GetValid()) {
         g_storePositiveCount.Store(positive, dataPair);
      }
      else {
         double a = c[0];
         double alpha = c[3];
         if (positive == 0) {
            std::cout << " trouble 0  " << kkkk << std::endl;
            std::cout << dataPair << std::endl;
         }
         g_storePositiveCountFailed.Store(positive, s6);
      }
   }
   g_storePositiveCount.SetTitle("Valid Cells **********************************");
   g_storePositiveCountFailed.SetTitle("Invalid Cells **********************************");
   g_storePositiveCount.SetKeyLabel("#positive");
   g_storePositiveCountFailed.SetKeyLabel("#positive");

   g_storePositiveCount.ShowResults();
   g_storePositiveCountFailed.ShowResults();
}

int TestS6RandomCells() {
   const unsigned long ntest = 1000;
   StoreResults<unsigned long, S6> g_StoreDelone(5);
   g_StoreDelone.SetTitle("Testing randDeloneReduced");
   for (unsigned long i = 0; i < ntest; ++i) {
      const S6 s6(S6::randDeloneReduced());
      g_StoreDelone.Store(S6::CountPositive(s6), s6);
   }
   g_StoreDelone.SetKeyLabel("#positive");
   g_StoreDelone.ShowResults();

   g_StoreDelone.clear();
   g_StoreDelone.SetTitle("Testing randDeloneUnreduced");
   for (unsigned long i = 0; i < ntest; ++i) {
      const S6 s6(S6::randDeloneUnreduced());
      g_StoreDelone.Store(S6::CountPositive(s6), s6);
   }
   g_StoreDelone.SetKeyLabel("#positive");
   g_StoreDelone.ShowResults();

   g_StoreDelone.clear();
   g_StoreDelone.SetKeyLabel("#positive");
   g_StoreDelone.ShowResults();
   g_StoreDelone.SetTitle("Testing randDeloneReduced(20.0)");
   for (unsigned long i = 0; i < ntest; ++i) {
      const S6 s6(S6::randDeloneReduced(20.0));
      g_StoreDelone.Store(S6::CountPositive(s6), s6);
   }
   g_StoreDelone.SetKeyLabel("#positive");
   g_StoreDelone.ShowResults();

   g_StoreDelone.clear();
   g_StoreDelone.SetTitle("Testing randDeloneUnreduced(20.0)");
   for (unsigned long i = 0; i < ntest; ++i) {
      const S6 s6(S6::randDeloneUnreduced(20.0));
      g_StoreDelone.Store(S6::CountPositive(s6), s6);
   }
   g_StoreDelone.SetKeyLabel("#positive");
   g_StoreDelone.ShowResults();
   return -1;
}

int main()
{
   TestS6_rand();
   std::cout << "total errors for TestS6RandomCells " << TestS6RandomCells() << std::endl;
   const std::string cell(" 10 13 17 65 76 85");
   std::cout << "total errors for Constructors " << CheckConstructors(cell) << std::endl;
   std::cout << "total errors for Assignments " << CheckAssignments(cell) << std::endl;

   std::cout << "total errors for TestS6RandomCells " << TestS6RandomCells() << std::endl;
   const C3 c1("1 2 3 4 5 6");
   const C3 c2("6 5 4 3 2 1");
   const C3 c3 = c1 + c2;
   std::cout << c1 << std::endl;
   std::cout << c2 << std::endl;
   std::cout << c3 << std::endl;
   std::cout << c3 - c1 << std::endl;
   return 0;
}

