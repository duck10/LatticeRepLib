#ifndef S6BOUNDARYTRANSFORMS_H
#define S6BOUNDARYTRANSFORMS_H

#include "MatS6.h"
#include "S6.h"


class S6BoundaryTransforms {
public:

   template<typename T>
   static  void PrintMatrix(const T& vm) {
      for (const auto& v : vm) {
         PrintMatrix(v);
      }
   }
   // Function to print a MatS6 matrix
   static void PrintMatrix(const MatS6& m);
   static void CheckMatrix(const MatS6& m);
   // Function to generate one boundary transform
   static MatS6 generateOneBoundaryTransform(const size_t boundary);
   // Function to generate all boundary transforms
   static std::vector<MatS6> generate6BoundaryTransforms();
   static std::vector<MatS6> GenerateFourTransformsForOneBoundry(const size_t boundary);
   static std::vector<std::vector<MatS6>> generate24BoundaryTransforms();
   static std::vector<MatS6> generateFlat24BoundaryTransforms();

private:

   // Function to interchange two rows in a MatS6 matrix
   static MatS6 InterchangeRows(const MatS6& in, const size_t row1, const size_t row2);

   // Function to create pairs of opposite boundaries
   static std::vector<std::pair<int, int>> CreateOppositePairs();

};

#endif // S6BOUNDARYTRANSFORMS_H