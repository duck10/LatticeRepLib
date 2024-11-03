#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <vector>
#include <set>
#include <iostream>
#include <iomanip>

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "Delone.h"
#include"LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "MatS6.h"
#include "S6.h"
#include "S6BoundaryTransforms.h"
#include "S6Dist.h"


template<typename T>
std::vector<S6> MVV(const std::vector<MatS6>& mv, const std::vector<S6>& sv) {

}


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
   static std::vector<MatS6> boundaryMatrices6;
   static std::vector<MatS6> boundaryMatrices24;

   static void initializeMatrices() {
      if (!permutationMatrices.empty()) return;

      auto reflections =  MatS6::Generate24S6Reflections();
      permutationMatrices = std::vector<MatS6>(reflections.begin(), reflections.end());
      boundaryMatrices = std::vector<MatS6>(S6BoundaryTransforms::generateFlat24BoundaryTransforms());
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
         for (size_t i = 0; i < boundaryMatrices6.size(); ++i) {
            if (current[i] > 0) {
               current = matrixMultiply(boundaryMatrices6[i], current);
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

      for (size_t bd_up = 0; bd_up < boundaryMatrices24.size(); ++bd_up) {
         for (size_t bd_dwn = 0; bd_dwn < boundaryMatrices24.size(); ++bd_dwn) {
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
std::vector<MatS6> TMBDistance::boundaryMatrices6;
std::vector<MatS6> TMBDistance::boundaryMatrices24;

int main() {

   //; Path generator
   //   p 10 20 30 44 55 66
   //   p 30 20 10 95 105 110
   //   end

   // raw path
   //   S  431.60388 172.07293  81.34733 - 353.42026 - 912.95121 - 1503.67681   segment 1  point 1 count 1
   //   S  407.97046 158.92984  66.26525 - 367.30036 - 874.23571 - 1424.79504   segment 1  point 2 count 2
   //   S  384.33704 145.78676  51.18318 - 381.18046 - 835.52021 - 1345.91327   segment 1  point 3 count 3
   //   S  360.70361 132.64367  36.10111 - 395.06057 - 796.80472 - 1267.03149   segment 1  point 4 count 4
   //   S  337.07019 119.50058  21.01903 - 408.94067 - 758.08922 - 1188.14972   segment 1  point 5 count 5
   //   S  313.43677 106.35750   5.93696 - 422.82077 - 719.37372 - 1109.26795   segment 1  point 6 count 6
   //   S  289.80334  93.21441 - 9.14512 - 436.70087 - 680.65823 - 1030.38618   segment 1  point 7 count 7
   //   S  266.16992  80.07133 - 24.22719 - 450.58097 - 641.94273 - 951.50441   segment 1  point 8 count 8
   //   S  242.53650  66.92824 - 39.30927 - 464.46108 - 603.22723 - 872.62263   segment 1  point 9 count 9
   //   S  218.90308  53.78515 - 54.39134 - 478.34118 - 564.51174 - 793.74086   segment 1  point 10 count 10
   //   S  195.26965  40.64207 - 69.47342 - 492.22128 - 525.79624 - 714.85909   segment 1  point 11 count 11
   //   S  171.63623  27.49898 - 84.55549 - 506.10138 - 487.08074 - 635.97732   segment 1  point 12 count 12
   //   S  148.00281  14.35589 - 99.63756 - 519.98149 - 448.36524 - 557.09554   segment 1  point 13 count 13
   //   S  124.36939   1.21281 - 114.71964 - 533.86159 - 409.64975 - 478.21377   segment 1  point 14 count 14
   //   S  100.73596 - 11.93028 - 129.80171 - 547.74169 - 370.93425 - 399.33200   segment 1  point 15 count 15
   //   S   77.10254 - 25.07337 - 144.88379 - 561.62179 - 332.21875 - 320.45023   segment 1  point 16 count 16
   //   S   53.46912 - 38.21645 - 159.96586 - 575.50189 - 293.50326 - 241.56845   segment 1  point 17 count 17
   //   S   29.83570 - 51.35954 - 175.04794 - 589.38200 - 254.78776 - 162.68668   segment 1  point 18 count 18
   //   S    6.20227 - 64.50263 - 190.13001 - 603.26210 - 216.07226 - 83.80491   segment 1  point 19 count 19
   //   S - 17.43115 - 77.64571 - 205.21209 - 617.14220 - 177.35677 - 4.92314   segment 1  point 20 count 20


// raw path as cells
//  P    10.000    20.000    30.000    44.000    55.000    66.000
//  P    11.921    20.000    29.290    45.858    62.923    73.863
//  P    13.572    20.000    28.562    47.716    67.909    79.131
//  P    15.044    20.000    27.815    49.579    71.519    83.109
//  P    16.384    20.000    27.048    51.457    74.355    86.322
//  P    17.622    20.000    26.258    53.356    76.711    89.035
//  P    18.778    20.000    25.443    55.284    78.750    91.395
//  P    19.868    20.000    24.602    57.252    80.572    93.496
//  P    20.901    20.000    23.731    59.269    82.245    95.396
//  P    21.885    20.000    22.827    61.348    83.819    97.138
//  P    22.827    20.000    21.885    63.504    85.333    98.753
//  P    23.731    20.000    20.901    65.758    86.822   100.262
//  P    24.602    20.000    19.868    68.132    88.317   101.683
//  P    25.443    20.000    18.778    70.661    89.855   103.029
//  P    26.258    20.000    17.622    73.392    91.477   104.310
//  P    27.048    20.000    16.384    76.390    93.244   105.535
//  P    27.815    20.000    15.044    79.764    95.240   106.711
//  P    28.562    20.000    13.572    83.690    97.613   107.845
//  P    29.290    20.000    11.921    88.509   100.646   108.939
//  P    30.000    20.000    10.000    95.000   105.000   110.000


   // niggli reduced as cells
   // P    10.000    18.366    18.850    96.713    92.820    95.829
   // P    11.921    19.968    20.000    81.607    73.863    78.013
   // P    13.572    20.000    21.027    80.853    71.701    79.131
   // P    15.044    20.000    21.267    95.301   107.564    96.891
   // P    16.384    20.000    21.388    98.459   106.323    93.678
   // P    17.622    20.000    21.508   101.609   105.365    90.965
   // P    18.778    20.000    21.627    75.240    75.403    88.605
   // P    19.868    20.000    21.747    72.079    76.031    86.504
   // P    20.000    20.901    21.865    76.558    68.895    84.604
   // P    20.000    21.885    21.983    77.006    65.676    82.862
   // P    20.000    21.885    22.827    94.667    98.753   116.496
   // P    20.000    20.901    23.731    93.178   100.262   114.242
   // P    19.868    20.000    24.602   101.683    91.683   111.868
   // P    18.778    20.000    25.443   103.029    90.145   109.339
   // P    17.622    20.000    26.258    75.690    88.523    73.392
   // P    16.384    20.000    27.048    74.465    86.756    76.390
   // P    15.044    20.000    27.815    73.289    84.760    79.764
   // P    13.572    20.000    28.562    72.155    82.387    83.690
   // P    11.921    20.000    29.290    71.061    79.354    88.509
   // P    10.000    20.000    28.274   108.279    90.998    95.000

   // delone reduced as cells
   // P    10.000    18.366    18.850    96.713    92.820    95.829
   // P    20.238    19.968    26.118   130.750   127.926    91.255
   // P    21.145    20.000    26.616   128.743   126.765    92.123
   // P    15.044    20.000    21.267    95.301   107.564    96.891
   // P    16.384    20.000    21.388    98.459   106.323    93.678
   // P    17.622    20.000    21.508   101.609   105.365    90.965
   // P    24.812    20.000    25.443   124.717   114.753   101.750
   // P    25.672    20.000    24.602   122.748   114.212   102.325
   // P    23.731    20.901    26.504   126.643   113.788    97.755
   // P    22.827    21.885    27.310   128.342   113.479    96.181
   // P    20.000    21.885    22.827    94.667    98.753   116.496
   // P    20.000    20.901    23.731    93.178   100.262   114.242
   // P    19.868    20.000    24.602   101.683    91.683   111.868
   // P    18.778    20.000    25.443   103.029    90.145   109.339
   // P    17.622    22.562    28.808   106.205   100.075   121.848
   // P    16.384    22.676    29.014   107.979    96.284   120.996
   // P    15.044    22.790    29.219   109.729    91.988   120.276
   // P    29.955    20.000    29.422   112.474   134.635   104.028
   // P    29.513    20.000    29.625   110.745   134.194   108.156
   // P    10.000    20.000    28.274   108.279    90.998    95.000
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   const int NUM_TESTS = 25;



   for (int test = 0; test < inputList.size()-1; ++test) {
      const std::string lattice = inputList[0].GetLattice();
      const S6 g6A = LatticeConverter().NiggliReduceCell(lattice, inputList[test].GetCell());
      const S6 g6B = LatticeConverter().NiggliReduceCell(lattice, inputList[test+1].GetCell());


      S6 redA = g6A;
      S6 redB = g6B;
      Delone::Reduce(g6A, redA);
      Delone::Reduce(g6B, redB);

      TMBDistance::S6C testA, testB;
      for (int i = 0; i < 6; ++i) {
         testA[i] = redA[i];
         testB[i] = redB[i];
      }

      double distanceTMB = TMBDistance::latticeDistance(testA, testB);
      double distanceCS6 = CS6Dist(testA.data(), testB.data());
      S6Dist s6dist;
      double distanceS6 = s6dist.DistanceBetween(redA, redB);

      if ( abs(distanceTMB- distanceCS6) > 0.5)
      {
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
   }

   return 0;
}