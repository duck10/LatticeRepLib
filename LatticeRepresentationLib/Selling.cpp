#pragma warning( disable : 4100) //  unreferenced formal parameter

#include "stdafx.h"


#include "Selling.h"

#include <algorithm>
#include <utility>

#include "MatS6.h"
#include "S6Dist.h"

bool Selling::m_debugInstrument = false;

std::vector< S6(*)(const S6&)> FillReduceMatrixVector() {
   static std::vector< S6(*)(const S6&)> vf;
   if (vf.empty()) {
      // THESE ARE TRANSFORMATIONS IN S6 (I THINK), NOT NOT NOT IN G6 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      vf.push_back(&S6Dist::Reduce11);  // g or p
      vf.push_back(&S6Dist::Reduce21);  // h or q
      vf.push_back(&S6Dist::Reduce31);  // k or r
      vf.push_back(&S6Dist::Reduce41);  // l or s
      vf.push_back(&S6Dist::Reduce51);  // m or t
      vf.push_back(&S6Dist::Reduce61);  // n or u
   }
   return vf;
}

bool Selling::Reduce(const S6& in, S6& out) {
   if (m_debugInstrument) std::cout << "Enter Selling::Reduce, input = " << in << std::endl;
   static std::vector< S6(*)(const S6&)> vf = FillReduceMatrixVector();
   unsigned long maxIndex = INT_MAX;
   int cycle = 0;
   out = in;
   long countPositive = 0;
   while (true) {
      double maxScalar = -DBL_MAX;

      for (unsigned long i = 0; i < 6; ++i) {
         if (out[i] > maxScalar) {
            maxIndex = i;
            maxScalar = out[i];
         }
      }
      if (maxScalar <= 0.0) {
         if (m_debugInstrument) std::cout << "Finish Selling::Reduce, positive scalars found = " << countPositive << "  output= " << out << std::endl;
         return true;
      }

      out = vf[maxIndex](out);
      double out1 = out[0];

      if (m_debugInstrument) std::cout << "   cycle = " << cycle << "  current vector = " << out << std::endl;
      ++cycle;
      if (countPositive > 1000)
      {
         if (m_debugInstrument) std::cout << "Finish Selling::Reduce FAILED, positive scalars found = " << countPositive << "  output= " << out << std::endl;
         return false;
      }
      if (maxScalar > 0.0) ++countPositive;
   }
}

   bool Selling::Reduce(const S6& in, MatS6& mReduce, S6& out, const double delta/*=0.0*/) {
      static std::vector<MatS6> vm;
      mReduce = MatS6::Eye();
      if (vm.empty()) {
         // THESE ARE TRANSFORMATIONS IN S6 (I THINK), NOT NOT NOT IN G6 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         vm.push_back(MatS6("-1 0 0  0 0 0   1  1 0 0  0 0   1 0  0 0 1  0   -1 0 0  1 0 0   1  0 1 0  0 0   1 0  0 0 0  1"));  // g or p
         vm.push_back(MatS6(" 1 1 0  0 0 0   0 -1 0 0  0 0   0 1  0 1 0  0    0 1 1  0 0 0   0 -1 0 0  1 0   0 1  0 0 0  1"));  // h or q
         vm.push_back(MatS6(" 1 0 1  0 0 0   0  0 1 1  0 0   0 0 -1 0 0  0    0 1 1  0 0 0   0  0 1 0  1 0   0 0 -1 0 0  1"));  // k or r
         vm.push_back(MatS6(" 1 0 0 -1 0 0   0  0 1 1  0 0   0 1  0 1 0  0    0 0 0 -1 0 0   0  0 0 1  1 0   0 0  0 1 0  1"));  // l or s
         vm.push_back(MatS6(" 0 0 1  0 1 0   0  1 0 0 -1 0   1 0  0 0 1  0    0 0 0  1 1 0   0  0 0 0 -1 0   0 0  0 0 1  1"));  // mReduce or t
         vm.push_back(MatS6(" 0 1 0  0 0 1   1  0 0 0  0 1   0 0  1 0 0 -1    0 0 0  1 0 1   0  0 0 0  1 1   0 0  0 0 0 -1"));  // n or u
      }
      unsigned long maxIndex = INT_MAX;
      out = in;
      long countPositive = 0;
      double prevMaxScalar = -DBL_MAX;
      while (true) {
         double maxScalar = -DBL_MAX;

         for (unsigned long i = 0; i < 6; ++i) {
            if (out[i] > maxScalar) {
               maxIndex = i;
               maxScalar = out[i];
            }
         }
         if (countPositive > 1000) return false;
         if (maxScalar > 0.0 && maxScalar > prevMaxScalar) ++countPositive;
         if (maxScalar <= 0.0) return true;
         S6 ddx = vm[maxIndex] * out;
         mReduce = vm[maxIndex] * mReduce;
         prevMaxScalar = maxScalar;
         out = ddx;
      }
   }

   double Selling::MaxScalar(const S6& s6) {
      const std::vector<double>& v(s6.GetVector());
      return *std::max_element(v.begin(), v.end());
   }

   bool Selling::IsReduced(const S6& s6) {
      return (MaxScalar(s6) <= 0.0);
   }

   bool Selling::IsReduced(const S6& s6, const double delta) {
      return (MaxScalar(s6) <= delta);
   }
