#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <set>
#include <vector>

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "G6.h"
#include "LRL_ReadLatticeData.h"
#include "NCDist.h"
#include "Niggli.h"
#include "Polar.h"
#include "S6Dist.h"
#include "Selling.h"

class S6C {
public:
   friend std::ostream& operator<< (std::ostream& o, const S6C& dc) {
      std::streamsize oldPrecision = o.precision();
      o << std::fixed << std::setprecision(5);
      const size_t n = dc.size();
      for (int i = 0; i < n; ++i) {
         const double d = dc[i];
         o << std::setw(9) << d << " ";
      }
      o << std::setprecision(oldPrecision);
      o.unsetf(std::ios::floatfield);
      return o;
   }

   explicit operator S6(void) const {
      S6 s;
      const S6C& t (*this);
      s[0] = t[0];
      s[1] = t[1];
      s[2] = t[2];
      s[3] = t[3];
      s[4] = t[4];
      s[5] = t[5];

      return s;
   }


   std::array<double, 6> data;
   static size_t size() { return 6; }

   S6C() : data{ 0,0,0,0,0,0 } {}
   S6C(double s1, double s2, double s3, double s4, double s5, double s6)
      : data{ s1, s2, s3, s4, s5, s6 } {}
   explicit S6C(const S6& s)
      : data{ s[0], s[1], s[2], s[3], s[4], s[5] }
   { }

   double& operator[](int index) { return data[index]; }
   const double& operator[](int index) const { return data[index]; }

   bool operator<(const S6C& other) const {
      return data < other.data;
   }
};

// Matrix multiplication for S6
S6C matrixMultiply(const std::array<std::array<int, 6>, 6>& matrix, const S6C& s) {
   S6C result;
   //std::cout << "matrixMultiply" << std::endl;
   for (int i = 0; i < 6; ++i) {
      result[i] = 0;
      for (int j = 0; j < 6; ++j) {
         result[i] += matrix[i][j] * s[j];
         //std::cout << matrix[i][j] << std::endl;
      }
      //std::cout << std::endl;
   }
   return result;
}
// Function to calculate the covering radius for an S6 vector
double calculateCoveringRadius(const S6C& s) {
   // In S6 space, we can estimate the covering radius as half the minimum
   // absolute non-zero value of the S6 vector components
   double minNonZero = std::numeric_limits<double>::max();
   for (int i = 0; i < 6; ++i) {
      if (std::abs(s[i]) > 1e-10 && std::abs(s[i]) < minNonZero) {
         minNonZero = std::abs(s[i]);
      }
   }
   return minNonZero / 2.0;
}
// Euclidean distance between two S6 vectors
double euclideanDistance(const S6C& a, const S6C& b) {
   double sum = 0.0;
   for (int i = 0; i < 6; ++i) {
      double diff = a[i] - b[i];
      sum += diff * diff;
   }
   return std::sqrt(sum);
}

using Matrix6x6 = std::array<std::array<int, 6>, 6>;
using ReductionMatrices = std::array<Matrix6x6, 6>;

ReductionMatrices initializeReductionMatrices() {
   ReductionMatrices matrices;

   matrices[0] = { {
       {-1, 0, 0, 0, 0, 0},
       {1, 1, 0, 0, 0, 0},
       {1, 0, 0, 0, 1, 0},
       {-1, 0, 0, 1, 0, 0},
       {1, 0, 1, 0, 0, 0},
       {1, 0, 0, 0, 0, 1}
   } };

   matrices[1] = { {
       {1, 1, 0, 0, 0, 0},
       {0, -1, 0, 0, 0, 0},
       {0, 1, 0, 1, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, -1, 0, 0, 1, 0},
       {0, 1, 0, 0, 0, 1}
   } };

   matrices[2] = { {
       {1, 0, 1, 0, 0, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 0, -1, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 0, 1, 0, 1, 0},
       {0, 0, -1, 0, 0, 1}
   } };

   matrices[3] = { {
       {1, 0, 0, -1, 0, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 1, 0, 1, 0, 0},
       {0, 0, 0, -1, 0, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 1, 0, 1}
   } };

   matrices[4] = { {
       {0, 0, 1, 0, 1, 0},
       {0, 1, 0, 0, -1, 0},
       {1, 0, 0, 0, 1, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 0, -1, 0},
       {0, 0, 0, 0, 1, 1}
   } };

   matrices[5] = { {
       {0, 1, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 1},
       {0, 0, 1, 0, 0, -1},
       {0, 0, 0, 1, 0, 1},
       {0, 0, 0, 0, 1, 1},
       {0, 0, 0, 0, 0, -1}
   } };

   return matrices;
}

// Selling reduction function
S6C sellingReduce(const S6C& input, const int maxIterations = 100) {
   static const ReductionMatrices reductionMatrices = initializeReductionMatrices();
   S6C current = input;

   for (int iteration = 0; iteration < maxIterations; ++iteration) {
      bool reduced = true;
      for (int i = 0; i < 6; ++i) {
         if (current[i] > 0) {
            current = matrixMultiply(reductionMatrices[i], current);
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


// Function to check if an S6 vector is reduced
bool isReduced(const S6C& s) {
   for (int i = 0; i < 6; ++i) {
      if (s[i] > 0) return false;
   }
   return true;
}

// Function to estimate the number of shells needed

// Function to estimate the number of shells based on covering radius
int estimateRequiredShells(const S6C& s, const S6C& target) {
   double coveringRadius = calculateCoveringRadius(s);
   double maxDistance = 0.0;
   for (int i = 0; i < 6; ++i) {
      maxDistance += (s[i] - target[i]) * (s[i] - target[i]);
   }
   maxDistance = std::sqrt(maxDistance);

   // We add 1 to ensure we cover slightly beyond the estimated maximum distance
   return static_cast<int>(std::ceil(maxDistance / coveringRadius)) + 1;
}

// Generate permutations
std::set<S6C> generatePermutations(const S6C& input) {
   std::set<S6C> permutations;

   const std::array<std::array<int, 6>, 24> perms = {
       //{{0,1,2,3,4,5}, {0,2,1,3,5,4}, {0,4,5,3,1,2}, {0,5,4,3,2,1},
       // {1,0,5,4,3,2}, {1,2,0,4,5,3}, {1,3,2,4,0,5}, {1,5,3,4,2,0},
       // {2,0,4,5,3,1}, {2,1,0,5,4,3}, {2,3,1,5,0,4}, {2,4,3,5,1,0},
       // {3,0,5,1,4,2}, {3,1,4,0,5,2}, {3,2,0,1,5,4}, {3,4,2,0,1,5},
       // {4,0,2,5,1,3}, {4,1,5,2,0,3}, {4,3,0,2,5,1}, {4,5,1,2,3,0},
       // {5,0,3,4,1,2}, {5,1,2,3,0,4}, {5,2,1,3,4,0}, {5,3,4,1,0,2}}
      // the matrices below were generated from (0,-1,-2,-3,-4,-5), applying
      // cmdS6Refl, and simple text editing. They do not agree with
      // the above matrices that Claude generated on Sept 18,2024
{{0,1,2,3,4,5}, {0,2,1,3,5,4}, {0,4,5,3,1,2}, {0,5,4,3,2,1},
 {1,0,2,4,3,5}, {1,2,0,4,5,3}, {1,3,5,4,0,2}, {1,5,3,4,2,0},
 {2,0,1,5,3,4}, {2,1,0,5,4,3}, {2,3,4,5,0,1}, {2,4,3,5,1,0},
 {3,1,5,0,4,2}, {3,2,4,0,5,1}, {3,4,2,0,1,5}, {3,5,1,0,2,4},
 {4,0,5,1,3,2}, {4,2,3,1,5,0}, {4,3,2,1,0,5}, {4,5,0,1,2,3},
 {5,0,4,2,3,1}, {5,1,3,2,4,0}, {5,3,1,2,0,4}, {5,4,0,2,1,3}}
   };

   for (const auto& perm : perms) {
      S6C p;
      for (int i = 0; i < 6; ++i) {
         p[i] = input[perm[i]];
      }
      permutations.insert(p);
   }

   return permutations;
}

using Matrix6x6 = std::array<std::array<int, 6>, 6>;
using BoundaryMatrices = std::array<Matrix6x6, 6>;

BoundaryMatrices initializeBoundaryMatrices() {
   BoundaryMatrices matrices;

   matrices[0] = { {
       {-1, 0, 0, 0, 0, 0},
       {1, 1, 0, 0, 0, 0},
       {1, 0, 0, 0, 1, 0},
       {-1, 0, 0, 1, 0, 0},
       {1, 0, 1, 0, 0, 0},
       {1, 0, 0, 0, 0, 1}
   } };

   matrices[1] = { {
       {1, 1, 0, 0, 0, 0},
       {0, -1, 0, 0, 0, 0},
       {0, 1, 0, 1, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, -1, 0, 0, 1, 0},
       {0, 1, 0, 0, 0, 1}
   } };

   matrices[2] = { {
       {1, 0, 1, 0, 0, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 0, -1, 0, 0, 0},
       {0, 1, 1, 0, 0, 0},
       {0, 0, 1, 0, 1, 0},
       {0, 0, -1, 0, 0, 1}
   } };

   matrices[3] = { {
       {1, 0, 0, -1, 0, 0},
       {0, 0, 1, 1, 0, 0},
       {0, 1, 0, 1, 0, 0},
       {0, 0, 0, -1, 0, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 1, 0, 1}
   } };

   matrices[4] = { {
       {0, 0, 1, 0, 1, 0},
       {0, 1, 0, 0, -1, 0},
       {1, 0, 0, 0, 1, 0},
       {0, 0, 0, 1, 1, 0},
       {0, 0, 0, 0, -1, 0},
       {0, 0, 0, 0, 1, 1}
   } };

   matrices[5] = { {
       {0, 1, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 1},
       {0, 0, 1, 0, 0, -1},
       {0, 0, 0, 1, 0, 1},
       {0, 0, 0, 0, 1, 1},
       {0, 0, 0, 0, 0, -1}
   } };

   return matrices;
}

// Function to generate boundary transformations
std::vector<S6C> generateBoundaryTransformations(const S6C& input) {
   static const BoundaryMatrices boundaryMatrices = initializeBoundaryMatrices();
   std::vector<S6C> transformations;
   transformations.push_back(input);
   for (size_t i = 0; i < boundaryMatrices.size(); ++i) {
      //if (abs(input[i]) > 1.0E-4)
         transformations.push_back(matrixMultiply(boundaryMatrices[i], input));
      //else
      //   const int i19191 = 19191;
   }

   return transformations;
}

std::vector<S6C> generateBoundaryTransformations(const std::vector<S6C>& input) {
   std::vector<S6C> out;
   for (const auto& s6 : input) {
      const std::vector<S6C> temp = generateBoundaryTransformations(s6);
      out.insert(out.end(), temp.begin(), temp.end());
   }
   return out;
}


void TestTransforms(const S6C& testvector, const std::vector<S6C>& toBeTested, const std::string& s="") {
   //std::cout << "TestTransforms" << std::endl;
   std::cout << s << std::endl;
   for (size_t i = 0; i < toBeTested.size(); ++i) {
      S6 outS6A;
      Selling::Reduce(S6(testvector), outS6A);
      S6 outS6B;
      Selling::Reduce(S6(toBeTested[i]), outS6B);
      const double distS6 = CS6Dist(outS6A.data(), outS6B.data());
      //if (distS6 > 1.0E-4) {
      //   std::cout << "error in TestTransforms" << std::endl;
      //   G6 out1;
      //   G6 out2;
      //   Niggli::Reduce(G6(S6(testvector)), out1);
      //   Niggli::Reduce(G6(S6(toBeTested[i])), out2);
      //   const double distG6 = NCDist(out1.data(), out2.data());
      //   std::cout << " testvector (S6C) " << testvector << std::endl;
      //   std::cout << " toBeTested[" << i << "] (S6C) " << toBeTested[i] << std::endl;
      //   std::cout << "CS6Dist " << distS6 << "  NCDist " << distG6 << std::endl;
      //   std::cout << " Selling reduce outS6A (testvector, S6) " << outS6A << std::endl;
      //   std::cout << " Selling reduce outS6B (toBeReduced[i], S6 " << outS6B << std::endl << std::endl;
      //   std::cout << " Niggli reduce out1, (G6) " << out1 << std::endl;
      //   std::cout << " Niggli reduce out2, (G6) " << out2 << std::endl << std::endl;




      //   const int i19191 = 19191;
      //}
   }
}




// Apply both boundary transformations and permutations
std::set<S6C> applyTransformationsAndPermutations(const S6C& input) {
   std::set<S6C> results;

   // Apply boundary transformations
    static const auto boundaryTransforms = generateBoundaryTransformations(input);
    //TestTransforms(input, boundaryTransforms, "from applyTransformationsAndPermutations--generateBoundaryTransformations");

   // For each boundary transformation, apply all permutations
    for (size_t i = 0; i < boundaryTransforms.size(); ++i) {
      const auto permutations = generatePermutations(boundaryTransforms[i]);
      //TestTransforms(input, permutations, "from applyTransformationsAndPermutations after generatePermutations 1");
      results.insert(permutations.begin(), permutations.end());
      //TestTransforms(input, results);
      //TestTransforms(input, permutations, "from applyTransformationsAndPermutations after generatePermutations 2");
      //std::cout << " permutations of boundary transforms in applyTransformationsAndPermutations" << std::endl;
      //for (const auto& r : results) {
      //   std::cout << "S6 " << r << std::endl;;
      //}
   }

   return results;
}

const int maximumNumberOfShellsToAllow = 6;

double latticeDistance(const S6C& a, const S6C& b) {
   //std::cout << "in latticeDistance, input = " << a << "\n " << b << std::endl;
   const S6C reducedA = sellingReduce(a);
   const S6C reducedB = sellingReduce(b);
   //std::cout << "in latticeDistance, Claude reduced input = \n" << reducedA << " " << reducedB << std::endl;

   const S6C aSelling(sellingReduce(S6C(a)));
   const S6C bSelling(sellingReduce(S6C(b)));
   //std::cout << "in latticeDistance, Selling reduced input = \n" << aSelling << "\n " << bSelling << std::endl;


   int maxShells = estimateRequiredShells(reducedB, reducedA);
   maxShells = std::min(maxShells, maximumNumberOfShellsToAllow);

   double minDistance = std::numeric_limits<double>::max();
   std::set<S6C> currentShell = generatePermutations(reducedB);  // Using set to avoid duplicates

   S6C amin;
   S6C bmin;
   std::set<S6C> alreadySeen;
   double distmin = DBL_MAX;
   for (int shell = 0; shell <= maxShells; ++shell) {
      //std::cout << " shell " << shell << " of " << maxShells << std::endl;
      std::set<S6C> nextShell;

      for (const auto& s : currentShell) {
         const double distance = euclideanDistance(reducedA, s);
         if (distance < minDistance) {
            minDistance = distance;
            const int i19191 = 19191;
            amin = reducedA;
            bmin = s;
         }

         const bool found = alreadySeen.find(s) != alreadySeen.end();
         if (!found) {
            alreadySeen.insert(s);
            const auto transformedAndPermuted = applyTransformationsAndPermutations(s);
            //double distminShell = DBL_MAX;
            //int pos = -1;
            //for (size_t i = 0; i < transformedAndPermuted.size(); ++i ) {
            //   const double dist = CS6Dist(S6(reducedA).data(), S6(transformedAndPermuted[i]).data());
            //   distmin = std::min(dist, distmin);
            //   distminShell = std::min(dist, distminShell);
            //   const int i19191 = 19191;
            //}
            nextShell.insert(s);  // had no effect
            nextShell.insert(transformedAndPermuted.begin(), transformedAndPermuted.end());
         }
         else
         {
            const int i19191 = 19191;
         }
      }

      if (nextShell.empty() || shell == maxShells) {
         break;  // Stop if we've reached the maximum shells or no new points are generated
      }

      currentShell = std::move(nextShell);
   }
   //std::cout << std::endl;
   //std::cout << "best distance   " << minDistance << std::endl;
   //std::cout << "S6 " << amin << " amin in latticeDistance" << std::endl;
   //std::cout << "S6 " << bmin << " bmin in latticeDistance" << std::endl;

   return minDistance;
}

int TestReduction(const S6C& s) {
   std::cout << "TestReduction" << std::endl;
   S6 s6red;
   S6C s6cred = sellingReduce(s);
   Selling::Reduce(S6(s), s6red);
   const double d = CS6Dist(s6red.data(), S6(s6cred).data());
   int count = 0;
   if (d > 1.0E-4) {
      std::cout << " ERROR " << std::endl;
      std::cout << "incoming " << s << std::endl;
      std::cout << "Selling::Reduce" << s6red << std::endl;
      std::cout << "sellingReduce" << s6cred << std::endl;
      G6 out;
      Niggli::Reduce(G6(S6(s)), out);
      std::cout << "Niggli reduced input " << out << std::endl;
      Niggli::Reduce(G6(s6red), out);
      std::cout << "Niggli reduced s6red " << out << std::endl;
      Niggli::Reduce(G6(S6(s6cred)), out);
      std::cout << "Niggli reduced s6cred " << out << std::endl;
      std::cout << std::endl;
      ++count;
   }

   return count;
}

// Main function for testing
int main() {
   //const auto inputList = LRL_ReadLatticeData().ReadLatticeData();
   //for (const auto& input : inputList) {

   int count = 0;
   const S6C test1(-4.77577, -1.17355, -2.12290, -1.41245, -0.66383, -57.01491);
   const S6C test2(2.12290, -0.66547, -1.41206, -57.01244, -1.17394, -4.77413);
   latticeDistance(test1, test2);
   count += TestReduction(S6C(-4.77577, -1.17355, -2.12290, -1.41245, -0.66383, -57.01491));
   count += TestReduction(S6C(-2.12290, -0.66547, -1.41206, -57.01244, -1.17394, -4.77413));
   for (size_t i = 0; i < 1; ++i) {
      count += TestReduction(S6C(S6::rand()));
   }
   std::cout << count << " errors were found" << std::endl;
   //exit(0);

   for ( size_t i=0; i<100; ++i ) {
      const S6 Aunreduced(Polar().rand());
      const S6 Bunreduced(Polar().rand());
      //const S6 Aunreduced(-2.12290, -0.66547, -1.41206, -57.01244, -1.17394, -4.77413);
      //const S6 Bunreduced(-4.77577, -1.17355, -2.12290, -1.41245, -0.66383, -57.01491);
      //std::cout << "S6 " << Aunreduced << " A unreduced input" << std::endl;
      //std::cout << "S6 " << Bunreduced << " B unreduced input" << std::endl;
      S6 out;
      Selling::Reduce(Aunreduced, out);
      const S6 Ared(out);
      Selling::Reduce(Bunreduced, out);
      const S6 Bred(out);

      const S6C ac(Ared);
      const S6C bc(Bred);

      const S6C aSelling(sellingReduce(S6C(Aunreduced)));
      const S6C bSelling(sellingReduce(S6C(Bunreduced)));

      const double distanceClaude = latticeDistance(S6C(Ared), S6C(Bred));

      //std::cout << Bred << std::endl;
      //std::cout << S6C(Bred) << std::endl;

      const double cs6dist = CS6Dist(Ared.data(), Bred.data());
      S6Dist s6d;
      const double csdist = s6d.DistanceBetween(Ared, Bred);
      if (abs(distanceClaude - cs6dist) > 3) {
         std::cout << "S6 " << Ared << " Ared in main" << std::endl;
         std::cout << "S6 " << Bred << " Bred in main" << std::endl;
         std::cout << "S6 " << aSelling << " aSelling in main" << std::endl;
         std::cout << "S6 " << bSelling << " bSelling in main" << std::endl;

         std::cout << "Claude  " << distanceClaude;
         std::cout << " CS6Dist " << cs6dist << "  ";
         std::cout << " S6Dist " << csdist << std::endl << std::endl;
         std::cout << std::endl;
      }

      if (distanceClaude < 6.0) {
         const int i19191 = 19191;
      }
   }
   exit(1);


   // TESTING
   S6C latticeA(1, 2, -4, -100, -110, -120);
   S6C latticeB(2, 1, -4, -100, -110, -120);

   std::cout << "Covering radius of lattice A: " << calculateCoveringRadius(latticeA) << std::endl;
   std::cout << "Covering radius of lattice B: " << calculateCoveringRadius(latticeB) << std::endl;
   std::cout << "Estimated required shells: without reduction!! " << estimateRequiredShells(latticeA, latticeB) << std::endl;

   double distance = latticeDistance(latticeB, latticeA);

   std::cout << "Distance between lattices: " << distance << std::endl;

   return 0;
}