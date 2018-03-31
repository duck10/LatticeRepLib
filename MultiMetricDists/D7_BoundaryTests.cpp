//#include "stdafx.h"

#include "D7_BoundaryTests.h"

#include "ConstSingleton_T.h"
#include "D7_boundary.h"
#include "D7_ClassifySubboundaries.h"
#include "Delone.h"
#include "D7.h"
#include "G6.h"
#include "B4.h"
#include "MatS6.h"
#include "LRL_Cell.h"
#include "S6.h"
#include "LRL_MaximaTools.h"
#include "LRL_RandTools.h"
#include "StoreResults.h"
#include "SuperFastHash.h"

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

StoreResults< int, std::string> sr(1);

D7_BoundaryList g_blBoundary;

std::pair<unsigned int, std::string> TreatOneProjectedVector(const D7_Boundary& b, const std::string& projectedRandomSubboundaryLabel, const D7& projectedrandomvector) {
   std::ostringstream os;
   unsigned int hash = 0;
   const D7 transformedRandom = (b.GetSubboundary(projectedRandomSubboundaryLabel)).GetTransform() * projectedrandomvector;
   if (Delone::IsReduced(transformedRandom)) {
      std::vector<std::pair<std::string, std::string> > vTransformedVectorLabels = transformedRandom.ClassifyVector(0.00001);
      for (unsigned int i2 = 0; i2 < vTransformedVectorLabels.size(); ++i2) {
         os << "before transform from " << b.GetName() << " using " << projectedRandomSubboundaryLabel
            << D7(projectedrandomvector) << "    after  " << transformedRandom << "  "
            << vTransformedVectorLabels[i2].second << std::endl;
         const std::string stringToHash = projectedRandomSubboundaryLabel + vTransformedVectorLabels[i2].second;
         hash = SuperFastHash(stringToHash.c_str(), (unsigned int) stringToHash.length());
      }
   }
   return std::make_pair(hash, os.str());
}

std::pair<unsigned int, std::string> ScanOneBoundary(const D7_Boundary& b, const D7& reducedvector) {
   std::pair<unsigned int, std::string> dummyresult(std::make_pair(0, ""));
   const D7 projectedrandomvector = D7(b.GetProjector() * reducedvector);
   std::vector<std::pair<std::string, std::string> > vProjectedRandomVectorLabels;

   if (Delone::IsReduced(projectedrandomvector)) {
      vProjectedRandomVectorLabels = projectedrandomvector.ClassifyVector(0.00001);
      for (unsigned int i = 0; i < vProjectedRandomVectorLabels.size(); ++i) {
         dummyresult = TreatOneProjectedVector(b, vProjectedRandomVectorLabels[i].second, D7(projectedrandomvector));
      }
      sr.Store(dummyresult.first, dummyresult.second);

   }
   return dummyresult;
}

void ExamineAllBoundariesForOneVector(const D7& v) {
   for (unsigned int i = 0; i < D7_BoundaryList::size(); ++i) {
      const D7_Boundary b = D7_BoundaryList::instance().GetBoundary(i);
      const std::pair<unsigned int, std::string> vss = ::ScanOneBoundary(b, v);
      if (vss.first != 0) sr.Store(vss.first, vss.second);
   }
}

D7_BoundaryTests::D7_BoundaryTests() {};

void D7_BoundaryTests::ScanAllBoundaries() {
   for (size_t i = 0; i < 10000; ++i) {
      S6 vout;
      MatS6 m;
      const bool b = Delone::Reduce(S6(LRL_Cell::rand()*30.0), m, vout, 0.0);
      if (b) ExamineAllBoundariesForOneVector(D7(vout));
   }
   sr.ShowResults();
};

void D7_BoundaryTests::ListTransformMatrices(void) {

   //D7_BoundaryList bl;
   for (unsigned int i = 0; i<D7_BoundaryList::size(); ++i) {
      const unsigned int subboundarycount = D7_BoundaryList::instance().GetBoundary(i).GetSubboundaryCount();
      for (unsigned int j = 0; j<subboundarycount; ++j) {
         std::cout << D7_BoundaryList::instance().GetBoundary(i).GetSubboundary(j).GetName() << "   Transform  ";
         std::cout << LRL_MaximaTools::MaximaFromMat(g_blBoundary.GetBoundary(i).GetSubboundary(j).GetTransform()) << " ";
         std::cout << std::endl;
      }
      std::cout << std::endl;
   }
}

void D7_BoundaryTests::ListProjectors(void) {
   for (unsigned int i = 0; i < D7_BoundaryList::size(); ++i) {
      const unsigned int subboundarycount = D7_BoundaryList::instance().GetBoundary(i).GetSubboundaryCount();
      std::cout << D7_BoundaryList::instance().GetBoundary(i).GetName() << "   Projector  ";
      std::cout << LRL_MaximaTools::MaximaFromMat(g_blBoundary.GetBoundary(i).GetProjector()) << " ";
      std::cout << std::endl;
      std::cout << std::endl;
   }
}

void D7_BoundaryTests::ListPerps(void) {
   std::vector<D7_Boundary> vb = g_blBoundary.GetVectorOfBoundaries();
   for (unsigned int i = 0; i <vb.size(); ++i) {
      const unsigned int subboundarycount = D7_BoundaryList::instance().GetBoundary(i).GetSubboundaryCount();
      std::cout << D7_BoundaryList::instance().GetBoundary(i).GetName() << "   Perp  ";
      std::cout << LRL_MaximaTools::MaximaFromMat(g_blBoundary.GetBoundary(i).GetPerp()) << " ";
      std::cout << std::endl;
      std::cout << std::endl;
   }
}

bool IsInD7(const D7& d7) {
   const double minvalue = 1.0E-10 * d7.norm();
   const bool b1 = d7[0] + d7[1] >= 0 && d7[2] >= 0 && d7[3] >= 0 && d7[4] >= 0 && d7[5] >= 0 && d7[6] >= 0;
   const bool b2 = std::abs(d7[0] + d7[1] + d7[2] + d7[3] - d7[4] - d7[5] - d7[6]) < minvalue;
   return b1&&b2;
}

void TestNormals() {
   std::vector<D7_Boundary> vb = g_blBoundary.GetVectorOfBoundaries();
   for (unsigned int i = 0; i < D7_BoundaryList::size(); ++i) {
      D7_Boundary& b = vb[i];
      D7 d7 = b.GetExternalNormalUnitVector();
      D7 insideNorm = -d7;
      D7 projectedInsideNorm = b.GetProjector() * insideNorm;
      const std::vector<std::pair<std::string, std::string> > boundstrings = projectedInsideNorm.ClassifyVector(1.0E-5);
      std::cout << "Testing Normal for Case  " << vb[i].GetName() << std::endl;
      for (size_t jj = 0; jj < boundstrings.size(); ++jj) {
         std::cout << boundstrings[jj].first << "  " << boundstrings[jj].second << std::endl;
      }
      std::cout << std::endl;
   }
}

void DeloneTransformTests( const size_t n) {
   size_t errorCount = 0;
   size_t testCount = 0;
   double aa[6];
   G6 g6;
   D7 d7;
   std::vector<D7_Boundary> vb = g_blBoundary.GetVectorOfBoundaries();
   for (unsigned int i = 0; i < n; ++i) {
      d7 = G6(aa);
      if (IsInD7(d7)) {
         for (size_t ibl = 0; ibl < vb.size(); ++ibl) {
            const unsigned int subboundarycount = vb[ibl].GetSubboundaryCount();
            for (unsigned int j = 0; j < subboundarycount; ++j) {
               const MatD7 m7 = (vb[ibl].GetSubboundary(j)).GetTransform();
               const D7 d7Transformed = m7 * d7;
               ++testCount;
               if (!IsInD7(d7Transformed)) {
                  std::cout << std::endl;
                  std::cout << "FAILED TRANSFORM" <<std::endl;
                  std::cout << D7_BoundaryList::instance().GetBoundary(i).GetSubboundary(j).GetName() << "   Transform  ";
                  std::cout << LRL_MaximaTools::MaximaFromMat(g_blBoundary.GetBoundary(i).GetSubboundary(j).GetTransform()) << " ";
                  ++errorCount;
               }
            }
         }
      }
      else {
         std::cout << "GenRandDeloneReducedArray not in D7" << std::endl;
         ++errorCount;
      }
   }
   std::cout << "Random Delone Transforms at boundaries, test count = " << testCount << std::endl;
}

void D7_BoundaryTests::MainTests() {
   D7_Boundary db;

   D7_BoundaryTests bt;
   std::cout << std::endl << std::endl << "ListTransformMatrices************************************************************" << std::endl;
   bt.ListTransformMatrices();
   std::cout << std::endl << std::endl << "ListProjectors************************************************************" << std::endl;
   bt.ListProjectors();
   std::cout << std::endl << std::endl << "ListPerps************************************************************" << std::endl;
   bt.ListPerps();

   TestNormals();

   DeloneTransformTests(50);

   const D7 v8dotx("  85.630   583.255   483.628   419.559   364.999   538.188   668.885 ");
   const D7 v5dot1("188.674   456.494   526.126   399.564   588.238   458.389   524.231");
   //std::vector<std::pair<std::string, std::string> > casenamelist = ClassifySubboundaries::ClassifyVector(v5dot1, 0.0001);
   //std::cout << casenamelist[0].second << "   " << v8dotx << std::endl;

   std::cout << std::endl << std::endl << "ScanAllBoundaries************************************************************" << std::endl;
   bt.ScanAllBoundaries();

   const D7 vfol5("  44.099    44.268    57.722    57.732   101.831   100.014     1.976 ");
   const D7 vfol4("   1.977    44.000    57.833   100.014   101.833    57.823    44.169 ");

}