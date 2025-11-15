#pragma warning( disable : 4100) //  unreferenced formal parameter




#include "Selling.h"
#include "EnhancedS6.h"

#include <algorithm>
#include <array>
#include <utility>

#include "C3.h"
#include "MatS6.h"
#include "S6Dist.h"
#include "LRL_Vector3.h"

const int limitReductionCycles = 10000;

bool Selling::m_debugInstrument = false;
size_t Selling::m_ReductionCycleCount = 0;

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
   size_t maxIndex = INT_MAX;
   m_ReductionCycleCount = 0;
   out = in;
   if (out[0] <= 0.0 && out[1] <= 0.0 && out[2] <= 0.0 && out[3] <= 0.0 && out[4] <= 0.0 && out[5] <= 0.0) return true;

   //ListSteps(out);

   while (S6::CountPositive(out) != 0) {
      double maxScalar = -DBL_MAX;

      for (size_t i = 0; i < 6; ++i) {
         if (out[i] > maxScalar) {
            maxIndex = i;
            maxScalar = out[i];
         }
      }
      //std::cout << "maxIndex in Selling::Reduce " << maxIndex << " at count " << m_ReductionCycleCount << std::endl;
      out = reductionFunctions[maxIndex](out);
      //std::cout << "S6 after step: " << out << std::endl;

      //ListSteps(out);

      ++m_ReductionCycleCount;
      if (m_ReductionCycleCount > limitReductionCycles || S6::NegativeSumOfScalars(out) < 0.0 ) return false;
   }
   return true;
}

bool Selling::Reduce(const S6& in, MatS6& mReduce, S6& out, const double delta/*=0.0*/) {
      static std::vector<MatS6> vm;
      mReduce = MatS6::Eye();
      if (vm.empty()) {
         // THESE ARE BOUNDARY TRANSFORMATIONS IN S6
         vm.emplace_back(MatS6("-1 0 0  0 0 0   1  1 0 0  0 0   1 0  0 0 1  0   -1 0 0  1 0 0   1  0 1 0  0 0   1 0  0 0 0  1"));  // g or p or s1
         vm.emplace_back(MatS6(" 1 1 0  0 0 0   0 -1 0 0  0 0   0 1  0 1 0  0    0 1 1  0 0 0   0 -1 0 0  1 0   0 1  0 0 0  1"));  // h or q or s2
         vm.emplace_back(MatS6(" 1 0 1  0 0 0   0  0 1 1  0 0   0 0 -1 0 0  0    0 1 1  0 0 0   0  0 1 0  1 0   0 0 -1 0 0  1"));  // k or r or s3
         vm.emplace_back(MatS6(" 1 0 0 -1 0 0   0  0 1 1  0 0   0 1  0 1 0  0    0 0 0 -1 0 0   0  0 0 1  1 0   0 0  0 1 0  1"));  // l or s or s4
         vm.emplace_back(MatS6(" 0 0 1  0 1 0   0  1 0 0 -1 0   1 0  0 0 1  0    0 0 0  1 1 0   0  0 0 0 -1 0   0 0  0 0 1  1"));  // mReduce or t or s5
         vm.emplace_back(MatS6(" 0 1 0  0 0 1   1  0 0 0  0 1   0 0  1 0 0 -1    0 0 0  1 0 1   0  0 0 0  1 1   0 0  0 0 0 -1"));  // n or u or s6
      }
      size_t maxIndex = INT_MAX;
      out = in;
      long countPositive = 0;
      double prevMaxScalar = -DBL_MAX;
      while (true) {
         double maxScalar = -DBL_MAX;

         for (size_t i = 0; i < 6; ++i) {
            if (out[i] > maxScalar) {
               maxIndex = i;
               maxScalar = out[i];
            }
         }
         if (countPositive > limitReductionCycles) return false;
         if (maxScalar > 0.0 && maxScalar > prevMaxScalar) ++countPositive;
         if (maxScalar <= 0.0) return true;
         S6 ddx = vm[maxIndex] * out;
         mReduce = vm[maxIndex] * mReduce;
         prevMaxScalar = maxScalar;
         out = ddx;
      }
   }

const std::vector<std::pair<MatS6, Matrix_3x3>> vmSellingPairs{
 { MatS6("-1 0 0 0 0 0   1 1 0 0 0 0   1 0 0 0 1 0   -1 0 0 1 0 0   1 0 1 0 0 0   1 0 0 0 0 1"),
   Matrix_3x3(-1, -1, 0,  0, 1, 0,  0, 0, -1) },
 { MatS6("1 1 0 0 0 0   0 -1 0 0 0 0   0 1 0 1 0 0   0 1 1 0 0 0   0 -1 0 0 1 0   0 1 0 0 0 1"),
   Matrix_3x3(1, 0, 0,  -1, -1, 0,  0, 0, -1) },
 { MatS6("1 0 1 0 0 0   0 0 1 1 0 0   0 0 -1 0 0 0   0 1 1 0 0 0   0 0 1 0 1 0   0 0 -1 0 0 1"),
   Matrix_3x3(1, 0, 0,  0, -1, 0,  -1, 0, -1) },
 { MatS6("1 0 0 -1 0 0   0 0 1 1 0 0   0 1 0 1 0 0   0 0 0 -1 0 0   0 0 0 1 1 0   0 0 0 1 0 1"),
   Matrix_3x3(1, 0, 0,  -1, -1, 0,  -1, 0, -1) },
 { MatS6("0 0 1 0 1 0   0 1 0 0 -1 0   1 0 0 0 1 0   0 0 0 1 1 0   0 0 0 0 -1 0   0 0 0 0 1 1"),
   Matrix_3x3(-1, -1, 0,  0, 1, 0,  0, -1, -1) },
 { MatS6("0 1 0 0 0 1   1 0 0 0 0 1   0 0 1 0 0 -1   0 0 0 1 0 1   0 0 0 0 1 1   0 0 0 0 0 -1"),
   Matrix_3x3(-1, 0, -1,  0, -1, -1,  0, 0, 1) }
};

void Selling::Write_S6_E3_MatrixPairs() {
   std::cout << R"(const std::vector<std::pair<MatS6, Matrix_3x3>> vmSellingPairs{
    { MatS6("-1 0 0 0 0 0   1 1 0 0 0 0   1 0 0 0 1 0   -1 0 0 1 0 0   1 0 1 0 0 0   1 0 0 0 0 1"),
      Matrix_3x3(-1, -1, 0,  0, 1, 0,  0, 0, -1) },
    { MatS6("1 1 0 0 0 0   0 -1 0 0 0 0   0 1 0 1 0 0   0 1 1 0 0 0   0 -1 0 0 1 0   0 1 0 0 0 1"),
      Matrix_3x3(1, 0, 0,  -1, -1, 0,  0, 0, -1) },
    { MatS6("1 0 1 0 0 0   0 0 1 1 0 0   0 0 -1 0 0 0   0 1 1 0 0 0   0 0 1 0 1 0   0 0 -1 0 0 1"),
      Matrix_3x3(1, 0, 0,  0, -1, 0,  -1, 0, -1) },
    { MatS6("1 0 0 -1 0 0   0 0 1 1 0 0   0 1 0 1 0 0   0 0 0 -1 0 0   0 0 0 1 1 0   0 0 0 1 0 1"),
      Matrix_3x3(1, 0, 0,  -1, -1, 0,  -1, 0, -1) },
    { MatS6("0 0 1 0 1 0   0 1 0 0 -1 0   1 0 0 0 1 0   0 0 0 1 1 0   0 0 0 0 -1 0   0 0 0 0 1 1"),
      Matrix_3x3(-1, -1, 0,  0, 1, 0,  0, -1, -1) },
    { MatS6("0 1 0 0 0 1   1 0 0 0 0 1   0 0 1 0 0 -1   0 0 0 1 0 1   0 0 0 0 1 1   0 0 0 0 0 -1"),
      Matrix_3x3(-1, 0, -1,  0, -1, -1,  0, 0, 1) }
};)" << std::endl;
}

bool Selling::ReduceWithtransforms(const S6& in, MatS6& mReduce, S6& out, Matrix_3x3& m33, const double delta) {

   out = in;
   MatS6 cumulativeTransform = MatS6::Eye();  // Start with identity matrix
   Matrix_3x3 cumulative3d = UnitMatrix();

   const int limitReductionCycles = 10000;
   int cycleCount = 0;

   while (true) {
      // Find the maximum positive scalar
      double maxScalar = out[0];  // Initialize with out[0]
      size_t maxIndex = 0;
      for (size_t i = 1; i < 6; ++i) {
         if (out[i] > maxScalar) {
            maxScalar = out[i];
            maxIndex = i;
         }
      }

      //std::cout << "maxIndex in ReduceWithTransforms " << maxIndex << " for cycle " << cycleCount << std::endl;
      //std::cout << "S6 after step: " << out << std::endl;
      if (maxScalar <= 0) break;  // All scalars are non-positive, reduction complete

      // Apply reduction step
      const MatS6 reductionMatrix = vmSellingPairs[maxIndex].first;
      out = reductionMatrix * out;
      cumulativeTransform = reductionMatrix * cumulativeTransform;
      cumulative3d = vmSellingPairs[maxIndex].second * cumulative3d;
      ++cycleCount;
      if (cycleCount > limitReductionCycles || S6::NegativeSumOfScalars(out) < 0.0) {
         return false;  // Reduction failed
      }
   }

   // Update the final transformation matrix
   mReduce = cumulativeTransform;
   m33 = cumulative3d;
   return true;

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

bool Selling::Reduce(const EnhancedS6& in, EnhancedS6& out) {
   out = in;
   MatS6 cumulativeTransform = MatS6::Eye();  // Start with identity matrix

   const int limitReductionCycles = 10000;
   int cycleCount = 0;

   // Define the reduction matrices
   static const std::array<MatS6, 6> reductionMatrices = { {
       MatS6("-1 0 0 0 0 0   1 1 0 0 0 0   1 0 0 0 1 0   -1 0 0 1 0 0   1 0 1 0 0 0   1 0 0 0 0 1"),
       MatS6("1 1 0 0 0 0   0 -1 0 0 0 0   0 1 0 1 0 0   0 1 1 0 0 0   0 -1 0 0 1 0   0 1 0 0 0 1"),
       MatS6("1 0 1 0 0 0   0 0 1 1 0 0   0 0 -1 0 0 0   0 1 1 0 0 0   0 0 1 0 1 0   0 0 -1 0 0 1"),
       MatS6("1 0 0 -1 0 0   0 0 1 1 0 0   0 1 0 1 0 0   0 0 0 -1 0 0   0 0 0 1 1 0   0 0 0 1 0 1"),
       MatS6("0 0 1 0 1 0   0 1 0 0 -1 0   1 0 0 0 1 0   0 0 0 1 1 0   0 0 0 0 -1 0   0 0 0 0 1 1"),
       MatS6("0 1 0 0 0 1   1 0 0 0 0 1   0 0 1 0 0 -1   0 0 0 1 0 1   0 0 0 0 1 1   0 0 0 0 0 -1")
   } };

   while (true) {
      // Find the maximum positive scalar
      double maxScalar = out[0];
      size_t maxIndex = 0;
      for (size_t i = 1; i < 6; ++i) {
         if (out[i] > maxScalar) {
            maxScalar = out[i];
            maxIndex = i;
         }
      }

      if (maxScalar <= 0) break;  // All scalars are non-positive, reduction complete

      // Apply reduction step
      const MatS6& reductionMatrix = reductionMatrices[maxIndex];
      out = reductionMatrix * out;
      cumulativeTransform = reductionMatrix * cumulativeTransform;

      ++cycleCount;
      if (cycleCount > limitReductionCycles || S6::NegativeSumOfScalars(out) < 0.0) {
         return false;  // Reduction failed
      }
   }

   // Update the final transformation matrix
   out.setTransformMatrix(cumulativeTransform * in.getTransformMatrix());

   return true;
}