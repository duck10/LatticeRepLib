#pragma warning( disable : 4100) //  unreferenced formal parameter




#include "Selling.h"

#include <algorithm>
#include <utility>

#include "C3.h"
#include "MatS6.h"
#include "S6Dist.h"

bool Selling::m_debugInstrument = false;
unsigned long Selling::m_ReductionCycleCount = 0;

std::vector< S6(*)(const S6&)> FillReduceFunctionArray() {
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

bool Selling::listSteps = false;

void Selling::ListSteps( const S6& in ) {
   if (listSteps) {
      C3 cin(in);
      std::cout << " " << in << "  |  " <<
         cin[0].real() << " " << cin[0].imag() << " " <<
         cin[1].real() << " " << cin[1].imag() << " " <<
         cin[2].real() << " " << cin[2].imag() << " " <<
         "     " << S6::NegativeSumOfScalars(in) <<
         std::endl;
   }
}

bool Selling::Reduce(const S6& in, S6& out, const bool sellingFirst) {
   // dummy function to have the same signature as one in Niggli
   return Reduce(in, out);
}

bool Selling::Reduce(const S6& in, S6& out) {
   static const std::vector< S6(*)(const S6&)> reductionFunctions = FillReduceFunctionArray();
   unsigned long maxIndex = INT_MAX;
   m_ReductionCycleCount = 0;
   out = in;

   ListSteps(out);

   while (S6::CountPositive(out) != 0) {
      double maxScalar = -DBL_MAX;

      for (unsigned long i = 0; i < 6; ++i) {
         if (out[i] > maxScalar) {
            maxIndex = i;
            maxScalar = out[i];
         }
      }
      
      out = reductionFunctions[maxIndex](out);

      ListSteps(out);

      ++m_ReductionCycleCount;
      if (m_ReductionCycleCount > 1000 || S6::NegativeSumOfScalars(out) < 0.0 ) return false;
   }
   return true;
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
