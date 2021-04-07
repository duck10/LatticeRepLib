
#include <iostream>

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "D13.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"
#include "MatD13.h"
#include "Niggli.h"
#include "PairReporter.h"
#include "S6.h"
#include "Selling.h"

#include <vector>

size_t inputPoint = 0;

//http://www.cplusplus.com/forum/general/55651/  // REFERENCE FOR VECTOR FOR CLASSES

//std::vector<S6> GenerateReflections(const S6 s) {
//   static const std::vector<MatS6> refl = MatS6::GetReflections();
//   std::vector<S6> v;
//   for (size_t i = 0; i < refl.size(); ++i)
//      v.push_back(refl[i] * s);
//   return v;
//}

//S6 Rotate_XY(const S6& s) {
//   G6 g(s);
//   std::swap(g[0], g[1]);
//   std::swap(g[3], g[4]);
//   return g;
//}
//
//S6 Rotate_XZ(const S6& s) {
//   G6 g(s);
//   std::swap(g[0], g[2]);
//   std::swap(g[3], g[5]);
//   return g;
//}
//
//S6 Rotate_YZ(const S6& s) {
//   G6 g(s);
//   std::swap(g[0], g[2]);
//   std::swap(g[4], g[4]);
//   return g;
//}
//
//S6 Rotate_XY_XZ(const S6& s) {
//   return Rotate_XY(Rotate_XZ(s));
//}
//
//S6 Rotate_XY_YZ(const S6& s) {
//   return Rotate_XY(Rotate_YZ(s));
//}
//
//S6 Rotate_XZ_XY(const S6& s) {
//   return Rotate_XZ(Rotate_XY(s));
//}
//
//S6 Rotate_XZ_YZ(const S6& s) {
//   return Rotate_XZ(Rotate_YZ(s));
//}
//
//S6 Rotate_YZ_XY(const S6& s) {
//   return Rotate_XZ(Rotate_XY(s));
//}
//
//S6 Rotate_YZ_XZ(const S6& s) {
//   return Rotate_YZ(Rotate_XZ(s));
//}

std::string CompressMatD13(const MatD13& m) {
   std::string s;
   s.resize(13 * 13);
   for (size_t i = 0; i < 13 * 13; ++i) s[i] = (m[i] == 0) ? '0' : '1';
   return s;
}

inline size_t Index2(const size_t i1, const size_t i2) {
   const size_t index = i1 + 13 * i2;
   return index;
}

void PrintMatD13(const MatD13& m) {
   const std::vector<double> v = m.GetVector();
   for (size_t t = 0; t < 13 * 13; t+=13) {
      for (size_t i = 0; i < 13; ++i) {
         std::cout << v[i + t] << " ";
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
}

MatD13 Differences_To_Matrix13(const D13& d1, const D13& d2) {
   CNearTree<double> tree(d2.GetVector());
   MatD13 m;
   m.zeros();
   m[0] = 0;
   std::vector<double> v(d1.GetVector());
   for (size_t i = 0; i < 13; ++i) {
      if (v[i] > 1.0E-4) {
         const auto it = tree.NearestNeighbor(DBL_MAX, v[i]);
         const long n = it.get_position();
         if (it == tree.end()) {
            const int i19191 = 19191;
            m[Index2(i, i)] = 0;
         }
         else {
            const double diff = abs(v[i] - tree[n]);
            if (diff < 1000.) {
               const size_t i1 = Index2(i, n);
               const size_t i2 = Index2(n, i);
               m[i1] = 1;
               m[i2] = 1;
            }
            else if (v[i] = tree[n]) {
               m[Index2(i, i)] = 1;
            }
            else { // zeros are indeterminant
               m[Index2(i, i)] = -15;
            }
         }

      }
      else { // zeros are indeterminant
         m[Index2(i, i)] = 0;
      }
   }
   //PrintMatD13(m);
   return m;
}


std::pair<double,D13> MeasureD13(const S6& snew, const S6& sendPt, const D13& dendPt) {
   static const std::vector<MatS6> reflectionMatrices = MatS6::GetReflections();
   double minDist = DBL_MAX;
   D13 minD13;

   //D13 revisedEndPt = (dendPt.IsValid()) ? dendPt : D13(sendPt);
   D13 revisedEndPt(sendPt);
   for (size_t i = 0; i < 1; ++i) {
      //std::cout << " in MeasureD31 i = " << i;
      const D13 dnew(reflectionMatrices[i] * snew);
      const double dist = (dnew - revisedEndPt).Norm();
      if (dist < minDist) {
         minDist = dist;
         minD13 = dnew;
      }
      //std::cout << " in MeasureD13 i, dist, minDist " << i << " " << dist << "  " << minDist << std::endl;
   };
   return std::make_pair(minDist, minD13);
}

StoreResults<std::string, std::string > store(5);

void Cycle(const S6& s1, const S6& s2, const size_t steps) {
   const D13 endpt(s2);
   const double diff = (s1 - s2).Norm();
   for (size_t i = 0; i <= steps; ++i) {
      const S6 snew = (double(steps - i) * s1 + double(i)*s2) / double(steps);
      //if (i % 100 == 0) {
      if (i > 28300 && i < 28500 /*&& inputPoint == 5*/) {
         std::cout << "in Cycle, step i = " << i << "  of " << steps <<std::endl;
         std::cout << "\t s " << s1   << " s1" << std::endl;
         std::cout << "\t s " << s2   << " s2 " <<std::endl;
         std::cout << "\t s " << snew << " snew" << std::endl;
         std::cout << "cycles " << Niggli::GetCycles() << std::endl << std::endl;
      }
      const std::pair<double, D13>  dist1 = MeasureD13(snew, s2, D13()/*endpt*/);
      //std::cout << " return from MeasureD13 " << std::endl;

      const D13 start = D13(snew);
      const MatD13 m = Differences_To_Matrix13(start, endpt);
      const std::string s1 = LRL_ToString(start);
      const std::string s2 = LRL_ToString(endpt);
      const std::string s3;
      store.Store(CompressMatD13(m), s1 + "  " + s2);
      //if (i % (steps / 10) == 0) std::cout << i << " A " << dist1.first << std::endl;
      //if (/*i < 30000 && i > 19100 &&*/ inputPoint == 5) std::cout << i << " B " << dist1.first << std::endl;
      //std::cout << i << "  " << dist << " " << snew << std::endl;
      //std::cout << i << std::string("  A  ") << LRL_ToString(dist.second.GetVector()) << std::string("   B\t") << LRL_ToString(D13(snew).GetVector()) << std::endl;
   }
   //store.ShowResults();
}

void FollowerStyleTest(const S6& s1, const S6& s2, const size_t cycles) {
   if ( (s1-s2).Norm() > 1.0E-2) 
      Cycle(s1, s2, cycles);
   else {
      std::cout << "skipping duplicate cells " << std::endl;
      std::cout << s1 << std::endl;
      std::cout << s2 << std::endl;
   }
}

void FollowerStyleTest() {
   const S6 s2(LRL_Cell::rand());
   S6 s1(LRL_Cell::rand());

   //const S6 s1("-300 -200 -400 -100 0 0");
   //const std::vector<S6> refl = GenerateReflections(s1);
   //const S6 s2 = refl[2];

   //const S6 s2("-100 -150 -200  -10 -20 -30");
   //const S6 s2(LRL_Cell::rand());
   //const S6 s1(LRL_Cell::rand());
   //const S6 s1 = refl[1];
   //const S6 s1(" -10 -20 -200 -100 -150 -30");
    //const S6 s1(" -10 -20 -200 -100 -150 -30");
    //const S6 s2("-100 -150 -200  -10 -20 -30");

   Cycle(s2, s1, 10);
   //std::cout << s1 << "   " << s2 << std::endl;
   std::cout << LRL_ToString(D13::vertices) << std::endl;
   //std::cout << LRL_Cell_Degrees(s1) << "   " << LRL_Cell_Degrees(s2) << std::endl;
   //exit(0);
   ////=======================================================================================
   //const size_t limit = 100;
   //std::vector<D13> drefl;
   //for (size_t i = 0; i < refl.size(); ++i)
   //   drefl.push_back(D13(refl[i]));

   //for (size_t i = 0; i < limit + 1; ++i) {
   //   const S6 snew = (double(i) * s2 + double(limit - i) * s1) / double(limit);

   //   double minDist = DBL_MAX;
   //   size_t minIndex = 24;
   //   const D13 d1(snew);
   //   for (size_t k = 0; k < refl.size(); ++k) {
   //      //std::vector<double> v1 = d1.GetVector();
   //      //std::vector<double> v2 = drefl[0].GetVector();
   //      //std::sort(v1.begin(), v1.end());
   //      //std::sort(v2.begin(), v2.end());
   //      //double sum = 0;
   //      //for (size_t i = 0; i < 13; ++i)
   //      //   sum += (v1[i] - v2[i])* (v1[i] - v2[i]);
   //      //minDist = sqrt(sum);
   //      const double dist = DistanceBetween(d1, drefl[k]);
   //      if (dist < minDist) {
   //         minDist = std::min(minDist, DistanceBetween(d1, drefl[k]));
   //         minIndex = k;
   //      }
   //   }
   //   std::cout << i << "  " << minDist << " " << snew << std::endl;
   //   //std::cout << i << std::string("  A  ") << LRL_ToString(drefl[minIndex].GetVector()) << std::string("   B\t") << LRL_ToString(d1.GetVector()) << std::endl;
   //}
   //std::cout << s1 << "   " << s2 << std::endl;
   //std::cout << LRL_ToString(D13::vertices) << std::endl;
   //std::cout << LRL_Cell_Degrees(s1) << "   " << LRL_Cell_Degrees(s2) << std::endl;
}

int main()
{
   //FollowerStyleTest();
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   LRL_Cell cell1 = inputList[0].GetCell();
   LRL_Cell cell2;
   for (inputPoint = 1; inputPoint < inputList.size(); ++inputPoint) {
      std::cout << inputPoint << std::endl;
      cell2 = inputList[inputPoint].GetCell();
      std::cout << "call FollowerStyleTest, inputPoint = " << inputPoint << std::endl;
      FollowerStyleTest(cell1, cell2, 100000);
      cell1 = cell2;
      //const D13 d13(inputList[i].GetStrCell());
      //std::cout << "D13 " << LRL_ToString(d13.GetVector()) << std::endl;
      //std::cout << " " << LRL_ToString(d13.GetStringIndices()) << std::endl;
      //std::cout << "NORM " << d13.Norm() << std::endl;
   }
   store.ShowResults();
   std::cout << "store: unique keys = " << store.size() << std::endl;
}
