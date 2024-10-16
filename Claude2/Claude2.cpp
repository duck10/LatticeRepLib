#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <vector>
#include <set>
#include <iostream>
#include <iomanip>
#include "S6.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "S6Dist.h"
#include "MatS6.h"

class TMBDistance {
public:
   using S6C = std::array<double, 6>;

   static double latticeDistance(const S6C& a, const S6C& b) {
      initializeMatrices();
      return tmbDistance(a, b);
   }

private:
   static constexpr int NUM_BOUNDARIES = 6;
   static std::vector<MatS6> permutationMatrices;
   static std::vector<MatS6> boundaryMatrices;

   static void initializeMatrices() {
      if (!permutationMatrices.empty()) return;

      auto reflections = Generate24S6Reflections();
      permutationMatrices = std::vector<MatS6>(reflections.begin(), reflections.end());
      boundaryMatrices = std::vector<MatS6>(Generate6S6BoundaryTransforms().begin(), Generate6S6BoundaryTransforms().end());
   }

   static std::set<MatS6> Generate24S6Reflections() {
      const MatS6 m1(0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0);
      const MatS6 m2(0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0);
      std::set<MatS6> sm;
      sm.insert(m1);
      sm.insert(m2);
      for (size_t i = 0; i < 3; ++i)
      {
         for (const auto& s : sm) {
            sm.insert(s * m1);
            sm.insert(s * m2);
         }
      }
      return sm;
   }

   static std::vector<MatS6> Generate24S6BoundaryTransforms() {
      const std::set<MatS6> refls = Generate24S6Reflections();
      const MatS6 bound{ -1,0,0,0,0,0,1,1,0,0,0,0,1,0,0,0,1,0,-1,0,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0,1 };
      std::vector<MatS6> bnds;
      for (const auto& r : refls) {
         bnds.emplace_back(r * bound);
      }
      return bnds;
   }

   static std::vector<MatS6> Generate6S6BoundaryTransforms() {
      const std::vector<MatS6> bnds = Generate24S6BoundaryTransforms();
      std::vector<MatS6> bnds6;
      const S6 id(-1, -3, -7, -13, -19, -31);

      for (const auto& b : bnds) {
         S6 temp(id);
         const S6 prod = b * temp;
         for (size_t i = 0; i < 6; ++i) {
            if (prod[i] == -id[i]) {
               if (std::find_if(bnds6.begin(), bnds6.end(),
                  [i, &id](const MatS6& m) { return (m * S6(id))[i] == -id[i]; }) == bnds6.end()) {
                  bnds6.push_back(b);
                  break;
               }
            }
         }
         if (bnds6.size() == 6) break;  // Stop once we have 6 matrices
      }

      if (bnds6.size() != 6) {
         std::cerr << "Warning: Generated " << bnds6.size() << " boundary matrices instead of 6." << std::endl;
      }

      return bnds6;
   }
   static double tmbDistance(const S6C& a, const S6C& b) {
      double minDistance = std::numeric_limits<double>::max();

      for (const auto& M : permutationMatrices) {
         S6C permA = matrixMultiply(M, a);
         for (const auto& N : permutationMatrices) {
            S6C permB = matrixMultiply(N, b);

            S6C redA = sellingReduce(permA);
            S6C redB = sellingReduce(permB);

            double dist = s6Distance(redA, redB);
            minDistance = std::min(minDistance, dist);
         }
      }

      return minDistance;
   }

   static S6C sellingReduce(const S6C& input, const int maxIterations = 100) {
      S6C current = input;
      for (int iteration = 0; iteration < maxIterations; ++iteration) {
         bool reduced = true;
         for (size_t i = 0; i < boundaryMatrices.size(); ++i) {
            if (current[i] > 0) {
               current = matrixMultiply(boundaryMatrices[i], current);
               reduced = false;
               break;
            }
         }
         if (reduced) {
            break;  // If no positive scalars, the reduction is complete
         }
      }
      return current;
   }

   static double s6Distance(const S6C& a, const S6C& b) {
      double directDist = euclideanDistance(a, b);
      double boundaryDist = boundaryDistance(a, b);
      return std::min(directDist, boundaryDist);
   }

   static double boundaryDistance(const S6C& a, const S6C& b) {
      double minDist = std::numeric_limits<double>::max();

      for (size_t bd_up = 0; bd_up < boundaryMatrices.size(); ++bd_up) {
         for (size_t bd_dwn = 0; bd_dwn < boundaryMatrices.size(); ++bd_dwn) {
            S6C mirrorA = computeMirrorPoint(a, b, bd_up);
            S6C mirrorB = computeMirrorPoint(b, a, bd_dwn);

            double dist = euclideanDistance(a, mirrorA) +
               euclideanDistance(mirrorA, mirrorB) +
               euclideanDistance(mirrorB, b);
            minDist = std::min(minDist, dist);
         }
      }

      return minDist;
   }

   static S6C computeMirrorPoint(const S6C& v1, const S6C& v2, size_t boundary) {
      double alpha = std::abs(v1[boundary]) / (std::abs(v1[boundary]) + std::abs(v2[boundary]));
      S6C mirror;
      for (int i = 0; i < 6; ++i) {
         mirror[i] = v1[i] + alpha * (v2[i] - v1[i]);
      }
      mirror[boundary] = 0;
      return mirror;
   }

   static S6C matrixMultiply(const MatS6& m, const S6C& v) {
      S6C result;
      for (int i = 0; i < 6; ++i) {
         result[i] = 0;
         for (int j = 0; j < 6; ++j) {
            result[i] += m[i * 6 + j] * v[j];  // Corrected access to MatS6 elements
         }
      }
      return result;
   }

   static double euclideanDistance(const S6C& a, const S6C& b) {
      double sumSq = 0.0;
      for (int i = 0; i < 6; ++i) {
         double diff = a[i] - b[i];
         sumSq += diff * diff;
      }
      return std::sqrt(sumSq);
   }
};

// Static member initialization
std::vector<MatS6> TMBDistance::permutationMatrices;
std::vector<MatS6> TMBDistance::boundaryMatrices;

int main() {
   const int NUM_TESTS = 5;

   for (int test = 0; test < NUM_TESTS; ++test) {
      S6 randomS6A = S6::rand();
      S6 randomS6B = S6::rand();

      TMBDistance::S6C testA, testB;
      for (int i = 0; i < 6; ++i) {
         testA[i] = randomS6A[i];
         testB[i] = randomS6B[i];
      }

      double distanceTMB = TMBDistance::latticeDistance(testA, testB);
      double distanceCS6 = CS6Dist(testA.data(), testB.data());
      S6Dist s6dist;
      double distanceS6 = s6dist.DistanceBetween(randomS6A, randomS6B);

      // Output the results
      std::cout << "Test " << test + 1 << ":" << std::endl;
      std::cout << "Vector A: ";
      for (const auto& val : testA) {
         std::cout << std::setw(10) << std::setprecision(5) << std::fixed << val << " ";
      }
      std::cout << std::endl;

      std::cout << "Vector B: ";
      for (const auto& val : testB) {
         std::cout << std::setw(10) << std::setprecision(5) << std::fixed << val << " ";
      }
      std::cout << std::endl;

      std::cout << "Distances - TMBDistance: " << std::setw(10) << std::setprecision(6) << distanceTMB
         << "  CS6Dist: " << std::setw(10) << distanceCS6
         << "  S6Dist: " << std::setw(10) << distanceS6 << std::endl;

      std::cout << std::endl;
   }

   return 0;
}