// Testbed.cpp : Defines the entry point for the console application.
//

#include "CellInputData.h"
#include "CS6Dist.h"
#include "Delone.h"
#include "CS6Dist.h"
#include "CS6Dist.c"
#include "DeloneTypeList.h"
#include "G6.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_MaximaTools.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "MatS6.h"
#include "NCDist.h"
#include "Niggli.h"
#include "PairReporter.h"
#include "rhrand.h"
#include "S6.h"
#include "S6BoundaryTransforms.h"
#include "S6Dist.h"
#include "TNear.h"

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <set>
#include <sstream>
#include <utility>



class S6ReflectionGenerator {
   /*
   * Rules for allowed reflection (perturbation) operations in S6
   Start with the list of opposite pairs (of the scalars in Delone's terminology). (1,4), (2,5), (3,6).
   Any change must be done on two pairs.
   Each pair must retain the same entries.
   The order of the pairs can be changed by exchanging two of them (at a time).
   Two pairs can have the order of their contents inverted. For instance,
   (1,4), (2,5), (3,6) => (1,4), (5,2), (6,3).
*/
private:
   class OppositeIndexPair {
   private:
      int m_first;
      int m_second;
   public:
      OppositeIndexPair(const int first = 0, const int second = 0) : m_first(first), m_second(second) {}
      void invert() {
         std::swap(m_first, m_second);
      }
      void getMatrixElements(const int row, MatS6& matrix) const {
         matrix[(m_first - 1) * 6 + row] = 1;
         matrix[(m_second - 1) * 6 + (row + 3)] = 1;
      }
   };

   class S6Reflection {
   private:
      OppositeIndexPair m_pairs[3];
   public:
      S6Reflection() : m_pairs{ OppositeIndexPair(1, 4), OppositeIndexPair(2, 5), OppositeIndexPair(3, 6) } {}
      const OppositeIndexPair& operator[](const int index) const {
         return m_pairs[index];
      }
      OppositeIndexPair& operator[](const int index) {
         return m_pairs[index];
      }
      S6Reflection invert(const int i, const int j) const {
         S6Reflection result = *this;
         result[i].invert();
         result[j].invert();
         return result;
      }
      MatS6 toMatrix() const {
         MatS6 matrix = MatS6::Zero();
         for (int i = 0; i < 3; ++i) {
            m_pairs[i].getMatrixElements(i, matrix);
         }
         return matrix;
      }
      static S6Reflection permute(const S6Reflection& base, const int order[3]) {
         S6Reflection result;
         for (int i = 0; i < 3; ++i) {
            result[i] = base[order[i]];
         }
         return result;
      }
   };

   static std::vector<MatS6> generateAllReflections() {
      std::vector<MatS6> reflectionMatrices;
      const S6Reflection base;

      int order[3] = { 0, 1, 2 };
      do {
         const S6Reflection permuted = S6Reflection::permute(base, order);
         reflectionMatrices.push_back(permuted.toMatrix());
         reflectionMatrices.push_back(permuted.invert(0, 1).toMatrix());
         reflectionMatrices.push_back(permuted.invert(0, 2).toMatrix());
         reflectionMatrices.push_back(permuted.invert(1, 2).toMatrix());
      } while (std::next_permutation(order, order + 3));

      return reflectionMatrices;
   }

   static MatS6 createTransformationMatrix(const int i, const int oppositeIndex[6]) {
      MatS6 transform = MatS6::Zero();
      // Steps 1, 2, and 3: Basic transformation
      transform[i * 6 + i] = -1;
      transform[oppositeIndex[i] * 6 + i] = -1;
      transform[oppositeIndex[i] * 6 + oppositeIndex[i]] = 1;
      for (int j = 0; j < 6; ++j) {
         if (j != i && j != oppositeIndex[i]) {
            transform[j * 6 + j] = 1;
            transform[j * 6 + i] = 1;
         }
      }
      return transform;
   }

public:
   static std::vector<MatS6> generate() {
      return generateAllReflections();
   }
};


//class OppositeIndexPair {
//private:
//   int m_first;
//   int m_second;
//public:
//   OppositeIndexPair(const int first = 0, const int second = 0) : m_first(first), m_second(second) {}
//   void invert() {
//      std::swap(m_first, m_second);
//   }
//   void getMatrixElements(const int row, MatS6& matrix) const {
//      matrix[(m_first - 1) * 6 + row] = 1;
//      matrix[(m_second - 1) * 6 + (row + 3)] = 1;
//   }
//   int first() const { return m_first; }
//   int second() const { return m_second; }
//};

static std::vector<MatS6> Generate24S6BoundaryTransforms() {
   class OppositeIndexPair {
   private:
      int m_first;
      int m_second;
   public:
      OppositeIndexPair(const int first = 0, const int second = 0) : m_first(first), m_second(second) {}
      void invert() {
         std::swap(m_first, m_second);
      }
      int first() const { return m_first; }
      int second() const { return m_second; }

      static std::pair<OppositeIndexPair, OppositeIndexPair> interchange(const OppositeIndexPair& a, const OppositeIndexPair& b) {
         return {
             OppositeIndexPair(a.m_first, b.m_second),
             OppositeIndexPair(b.m_first, a.m_second)
         };
      }

      void applyToMatrix(MatS6& matrix, int row) const {
         matrix[(m_first - 1) * 6 + row] = 1;
         matrix[(m_second - 1) * 6 + (row + 3)] = 1;
      }
   };

   const S6 tests6(-3.22250, -0.65950, -1.00050, -0.69900, -27.99200, -30.14000);
   G6 red;
   Niggli::Reduce(tests6, red);
   const G6 testred = red;
   std::cout << LRL_Cell_Degrees(red) << " reduced test cell " << std::endl;
   std::vector<MatS6> bnds24;

   OppositeIndexPair basePairs[3] = {
       OppositeIndexPair(1, 4), OppositeIndexPair(2, 5), OppositeIndexPair(3, 6)
   };

   for (int boundaryPairIndex = 0; boundaryPairIndex < 3; ++boundaryPairIndex) {
      for (int isNegative = 0; isNegative < 2; ++isNegative) {
         int i = boundaryPairIndex + isNegative * 3; // Current boundary index
         OppositeIndexPair boundaryPair = basePairs[boundaryPairIndex];
         if (isNegative) boundaryPair.invert();

         // Identify the two pairs not part of the boundary
         OppositeIndexPair pair1 = basePairs[(boundaryPairIndex + 1) % 3];
         OppositeIndexPair pair2 = basePairs[(boundaryPairIndex + 2) % 3];

         for (int j = 0; j < 4; ++j) {
            MatS6 transform = MatS6::Eye();

            // Set up the basic structure for the boundary transform
            transform[i * 6 + i] = -1;
            transform[boundaryPair.second() - 1 + (boundaryPair.first() - 1) * 6] = -1;
            transform[(boundaryPair.second() - 1) * 6 + (boundaryPair.second() - 1)] = 1;

            // Add 1 to rows not in the boundary pair
            for (int row = 0; row < 6; ++row) {
               if (row != boundaryPair.first() - 1 && row != boundaryPair.second() - 1) {
                  transform[row * 6 + (boundaryPair.first() - 1)] += 1;
               }
            }

            // Apply interchanges
            if (j > 0) {
               auto [newPair1, newPair2] = OppositeIndexPair::interchange(pair1, pair2);
               if (j & 1) std::swap(newPair1, newPair2);
               if (j & 2) {
                  newPair1.invert();
                  newPair2.invert();
               }
               newPair1.applyToMatrix(transform, 1);
               newPair2.applyToMatrix(transform, 2);
            }
            else {
               pair1.applyToMatrix(transform, 1);
               pair2.applyToMatrix(transform, 2);
            }

            bnds24.push_back(transform);

            // Perform the vector test
            Niggli::Reduce(transform * tests6, red);
            if ((red - testred).norm() > 1.0E-1) {
               std::cout << "vector test failed for boundary " << i << " transform " << j << std::endl;
               std::cout << LRL_Cell_Degrees(testred) << " reduced test cell " << std::endl;
               std::cout << LRL_Cell_Degrees(red) << " reduced transformed test cell " << std::endl;
               std::cout << "transform " << std::endl << transform << std::endl << std::endl;
            }

            // Debug output
            std::cout << "Boundary " << i + 1 << " transformation " << j + 1 << ":" << std::endl;
            std::cout << transform << std::endl << std::endl;
         }
      }
   }

   return bnds24;
}


std::vector<MatS6> Generate24S6Reflections() {
   return S6ReflectionGenerator::generate();
}

void TestPermutations() {
   B4 b4(S6::rand());
   std::vector<Vector_3> v(4);
   v[0] = b4[0];
   v[1] = b4[1];
   v[2] = b4[2];
   v[3] = b4[3];

   do {
      std::cout << v[0]<< '\n';
   } while (std::next_permutation(v[0], v[3]));
}

int main() {
   TestPermutations();
   exit(0);
   const S6 tests6(-3.22250, -0.65950, -1.00050, -0.69900, -27.99200, -30.14000);
   const auto bounds = Generate24S6BoundaryTransforms();
   G6 red;
   Niggli::Reduce(tests6, red);

   //std::vector<MatS6> reflections1 = Generate24S6Reflections();
   //std::vector<MatS6> reflections2 = MatS6::GetReflections();

   //std::cout << "Now test the results from Generate24S6Reflections " << std::endl;
   //std::cout << "All output vectors should be the same " << std::endl;
   //std::set<MatS6> matset;
   //for (const auto& r : reflections1) {
   //   Niggli::Reduce(r * tests6, red);
   //   std::cout << LRL_Cell_Degrees(red) << std::endl;
   //   matset.insert(r);
   //}

   //std::cout << LRL_Cell_Degrees(red) << " reduced test cell " << std::endl;
   //for (const auto& r : reflections2) {
   //   Niggli::Reduce(r * tests6, red);
   //   std::cout << LRL_Cell_Degrees(red) << std::endl;
   //   matset.insert(r);
   //}

   //std::cout << "The number of unique matrices in the two sets shoud be 24. Found = "
   //   << matset.size() << std::endl;

   return 0;

}