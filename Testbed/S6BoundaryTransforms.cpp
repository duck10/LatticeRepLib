// Testbed.cpp : Defines the entry point for the console application.
//

#include "Delone.h"
#include "CS6Dist.h"
#include "CS6Dist.c"
#include "MatS6.h"
#include "S6.h"
#include "S6BoundaryTransforms.h"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>


void S6BoundaryTransforms::CheckMatrix(const MatS6& m) {

   const S6 testS6(-5, -7, -11, -13, -17, -23);
   S6 red;
   Delone::Reduce(testS6, red);
   const S6 testred(red);
   Delone::Reduce(m * testS6, red);
   std::cout << "test for CS6Dist ok " << CS6Dist(red.data(), testred.data());
}


// Function to print a MatS6 matrix
void S6BoundaryTransforms::PrintMatrix(const MatS6& m) {
   for (size_t i = 0; i < 36; ++i) {
      if (m[i] == -1) {
         std::cout << "-1";
      }
      else {
         std::cout << " " << m[i];
      }
      if ((i + 1) % 6 == 0) std::cout << std::endl;
   }
   CheckMatrix(m);
   std::cout << std::endl;
}

// Function to interchange two rows in a MatS6 matrix
MatS6 S6BoundaryTransforms::InterchangeRows(const MatS6& in, const size_t row1, const size_t row2) {
   MatS6 m(in);
   for (size_t i = 0; i < 6; ++i) {
      const size_t index1 = 6 * (row1 - 1) + i;
      const size_t index2 = 6 * (row2 - 1) + i;
      std::swap(m[index1], m[index2]);
   }
   return m;
}

// Function to create pairs of opposite boundaries
std::vector<std::pair<int, int>> S6BoundaryTransforms::CreateOppositePairs() {
   // a list of the pairs of boundaries that are listed by Delone as "opposite"
   // The interchange of matrix rows needs to know which rows are not 
   // involved with the rows belonging to the pair with the changing boundary.

   std::vector<std::pair<int, int>> oppositePairs;
   oppositePairs.emplace_back(0, 0); // dummy zero-th entry so we can index by boundary

   // add extra copies so we don't need to
   // be concerned about overindexing oppositePairs
   // and don't need to do cool %3 and +6 adjusting indices
   oppositePairs.emplace_back(1, 4);
   oppositePairs.emplace_back(2, 5);
   oppositePairs.emplace_back(3, 6);

   oppositePairs.emplace_back(1, 4);
   oppositePairs.emplace_back(2, 5);
   oppositePairs.emplace_back(3, 6);

   oppositePairs.emplace_back(1, 4);
   oppositePairs.emplace_back(2, 5);
   oppositePairs.emplace_back(3, 6);

   return oppositePairs;
}

// Function to generate one boundary transform
MatS6 S6BoundaryTransforms::generateOneBoundaryTransform(const size_t boundary) {
   MatS6 transform(MatS6::Eye());  // Start with unit matrix
   int opposite = (boundary + 3) % 6;
   if (opposite == 0) opposite = 6;

   // Set the boundary column
   for (int i = 0; i < 6; ++i) {
      if ((i != boundary - 1) && (i != opposite - 1)) {
         transform[i * 6 + boundary - 1] = 1;
      }
   }

   // Set the boundary and opposite entries
   transform[(boundary - 1) * 6 + boundary - 1] = -1;
   transform[(opposite - 1) * 6 + boundary - 1] = -1;

   // Determine rows to interchange
   const auto oppositePairs = CreateOppositePairs();
   const int interchangeRow1 = oppositePairs[boundary + 1].first;
   const int interchangeRow2 = (boundary > 3) ? oppositePairs[boundary + 2].first
      : oppositePairs[boundary + 2].second;

   // Perform row interchange
   return InterchangeRows(transform, interchangeRow1, interchangeRow2);
}

// Function to generate all boundary transforms
std::vector<MatS6> S6BoundaryTransforms::generate6BoundaryTransforms() {
   std::vector<MatS6> transforms;
   for (int i = 1; i <= 6; ++i) {
      transforms.push_back(generateOneBoundaryTransform(i));
   }
   return transforms;
}

std::vector<MatS6> S6BoundaryTransforms::GenerateFourTransformsForOneBoundry(const size_t boundary) {
   static const auto refls = MatS6::GetReflections();
   std::vector<MatS6> out;
   S6 base(0, 0, 0, 0, 0, 0);
   base[boundary - 1] = -1;
   const MatS6 trans = generateOneBoundaryTransform(boundary);

   for (const auto& r : refls) {
      const S6 result = r * base;
      if (result[boundary - 1] == -1) {
         out.emplace_back(r * trans);
      }
   }

   return out;
}

std::vector<MatS6> S6BoundaryTransforms::generateFlat24BoundaryTransforms() {
   std::vector<std::vector<MatS6>> twentyfour = generate24BoundaryTransforms();
   std::vector<MatS6> flat;
   for (const auto& v : twentyfour) {
      flat.insert(flat.end(), v.begin(), v.end());
   }
   return flat;
}

std::vector<std::vector<MatS6>> S6BoundaryTransforms::generate24BoundaryTransforms() {
   static const auto refls = MatS6::GetReflections();
   std::vector<std::vector<MatS6>> out;
   for (size_t i = 1; i <= 6; ++i) {
      out.emplace_back(GenerateFourTransformsForOneBoundry(i));
   }
   return out;
}

