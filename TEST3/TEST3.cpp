#include <array>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>


struct S6 {
   std::array<double, 6> components;
};

struct R13 {
   std::array<double, 13> components;
};

double r13Distance(const R13& a, const R13& b) {
   double sum = 0.0;
   for (int i = 0; i < 13; ++i) {
      double diff = a.components.at(i) - b.components[i];
      sum += diff * diff;
   }
   return std::sqrt(sum);
}


R13 calculateR13(const S6& s6) {
   R13 r13;

   r13.components[0] = s6.components[0] + s6.components[1] + s6.components[2];
   r13.components[1] = s6.components[3] + s6.components[4] + s6.components[5];
   r13.components[2] = s6.components[0] - s6.components[1];
   r13.components[3] = s6.components[1] - s6.components[2];
   r13.components[4] = s6.components[3] - s6.components[4];
   r13.components[5] = s6.components[4] - s6.components[5];
   r13.components[6] = s6.components[0] - s6.components[3];
   r13.components[7] = s6.components[1] - s6.components[4];
   r13.components[8] = s6.components[2] - s6.components[5];
   r13.components[9] = s6.components[0] + s6.components[3];
   r13.components[10] = s6.components[1] + s6.components[4];
   r13.components[11] = s6.components[2] + s6.components[5];
   r13.components[12] = s6.components[0] + s6.components[1] + s6.components[2] +
      s6.components[3] + s6.components[4] + s6.components[5];

   return r13;
}

// R13 to S6 inverse transformation
S6 r13ToS6(const R13& r13) {
   S6 s6;
   s6.components[0] = (r13.components[0] + 2 * r13.components[2] + r13.components[3]) / 3.0;
   s6.components[1] = (r13.components[0] - r13.components[2] + r13.components[3]) / 3.0;
   s6.components[2] = (r13.components[0] - r13.components[2] - 2 * r13.components[3]) / 3.0;
   s6.components[3] = (r13.components[1] + 2 * r13.components[4] + r13.components[5]) / 3.0;
   s6.components[4] = (r13.components[1] - r13.components[4] + r13.components[5]) / 3.0;
   s6.components[5] = (r13.components[1] - r13.components[4] - 2 * r13.components[5]) / 3.0;
   return s6;
}

// Function to apply a permutation to an R13 vector
R13 applyPermutation(const R13& r13, const std::array<int, 13>& perm) {
   R13 result;
   for (int i = 0; i < 13; ++i) {
      result.components[i] = r13.components[perm[i]];
   }
   return result;
}

using Permutation = std::array<int, 13>;

std::vector<Permutation> generateR13Permutations() {
   std::vector<Permutation> permutations;

   // Identity permutation
   permutations.push_back({ {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12} });   // Permutations based on S6 swaps
   permutations.push_back({ 0, 1, 3, 2, 4, 5, 7, 6, 8, 9, 10, 11, 12 });   // Swap s0 and s1
   permutations.push_back({ 0, 1, 2, 3, 5, 4, 6, 8, 7, 9, 11, 10, 12 });   // Swap s3 and s4
   permutations.push_back({ 0, 1, 6, 7, 4, 5, 2, 3, 8, 10, 9, 11, 12 });   // Swap s0 and s3
   permutations.push_back({ 0, 1, 7, 6, 5, 4, 3, 2, 8, 11, 10, 9, 12 });   // Swap s1 and s4
   permutations.push_back({ 0, 1, 8, 7, 5, 4, 6, 3, 2, 11, 10, 9, 12 });   // Swap s2 and s5

   // Cyclic permutations
   permutations.push_back({ 0, 1, 3, 8, 5, 4, 7, 2, 6, 10, 11, 9, 12 });   // Cycle s0 -> s1 -> s2
   permutations.push_back({ 0, 1, 8, 6, 5, 4, 2, 7, 3, 11, 9, 10, 12 });   // Cycle s0 -> s2 -> s1
   permutations.push_back({ 1, 0, 2, 3, 6, 8, 4, 7, 5, 10, 9, 11, 12 });   // Cycle s3 -> s4 -> s5
   permutations.push_back({ 1, 0, 2, 3, 8, 5, 4, 7, 6, 11, 9, 10, 12 });   // Cycle s3 -> s5 -> s4

   // Combinations of swaps
   permutations.push_back({ 0, 1, 6, 8, 5, 4, 2, 7, 3, 11, 9, 10, 12 });   // Swap s0-s3, s1-s4
   permutations.push_back({ 0, 1, 7, 8, 5, 4, 6, 2, 3, 11, 10, 9, 12 });   // Swap s0-s4, s1-s3
   permutations.push_back({ 0, 1, 8, 6, 4, 5, 3, 7, 2, 10, 11, 9, 12 });   // Swap s0-s5, s2-s3

   // More complex permutations
   permutations.push_back({ 1, 0, 6, 8, 4, 5, 2, 3, 7, 10, 9, 11, 12 });   // s0-s3, s1-s4, s2-s5 swaps
   permutations.push_back({ 1, 0, 7, 6, 5, 4, 3, 8, 2, 11, 10, 9, 12 });   // s0-s4, s1-s3, s2-s5 swaps
   permutations.push_back({ 1, 0, 8, 7, 4, 5, 6, 2, 3, 10, 11, 9, 12 });   // s0-s5, s1-s4, s2-s3 swaps

   // Inversion-related permutations
   permutations.push_back({ 0, 1, 2, 3, 4, 5, 8, 7, 6, 9, 10, 11, 12 });   // Invert s2 and s5
   permutations.push_back({ 0, 1, 3, 2, 5, 4, 7, 8, 6, 9, 11, 10, 12 });   // Invert s1, s2, s4, s5
   permutations.push_back({ 0, 1, 6, 7, 5, 4, 2, 3, 8, 10, 11, 9, 12 });   // Invert s0, s3, s1, s4

   // Additional permutations to reach 24
   permutations.push_back({ 0, 1, 8, 3, 6, 5, 2, 7, 4, 11, 9, 10, 12 });
   permutations.push_back({ 0, 1, 7, 6, 4, 5, 3, 2, 8, 10, 11, 9, 12 });
   permutations.push_back({ 0, 1, 6, 8, 4, 5, 7, 3, 2, 10, 11, 9, 12 });
   permutations.push_back({ 0, 1, 8, 7, 6, 4, 2, 3, 5, 11, 9, 10, 12 });
   permutations.push_back({ 0, 1, 3, 6, 5, 4, 8, 2, 7, 10, 11, 9, 12 });

   return permutations;
}
R13 s6ToR13(const std::array<double, 6>& s6) {
   R13 r13;
   r13.components[0] = s6[0] + s6[1] + s6[2];
   r13.components[1] = s6[3] + s6[4] + s6[5];
   r13.components[2] = s6[0] - s6[1];
   r13.components[3] = s6[1] - s6[2];
   r13.components[4] = s6[3] - s6[4];
   r13.components[5] = s6[4] - s6[5];
   r13.components[6] = s6[0] - s6[3];
   r13.components[7] = s6[1] - s6[4];
   r13.components[8] = s6[2] - s6[5];
   r13.components[9] = s6[0] + s6[3];
   r13.components[10] = s6[1] + s6[4];
   r13.components[11] = s6[2] + s6[5];
   r13.components[12] = s6[0] + s6[1] + s6[2] + s6[3] + s6[4] + s6[5];
   return r13;
}

double minR13Distance(const R13& a, const R13& b, const std::vector<Permutation>& permutations) {
   double minDist = std::numeric_limits<double>::max();
   for (const auto& perm : permutations) {
      R13 permuted_a;
      for (int i = 0; i < 13; ++i) {
         permuted_a.components[i] = a.components[perm[i]];
      }
      double dist = r13Distance(permuted_a, b);
      minDist = std::min(minDist, dist);
   }
   return minDist;
}

// Main function to test
int main() {
   std::array<double, 6> s6_1 = { -0.331760, -12.833980, -21.667890, -0.172080, -0.269790, -1.031350 };
   std::array<double, 6> s6_2 = { -0.055070, -0.702780, -0.030500, -13.551970, -23.125080, -0.797570 };

   R13 r13_1 = s6ToR13(s6_1);
   R13 r13_2 = s6ToR13(s6_2);

   auto permutations = generateR13Permutations();  // Assume this function is defined
   double minDist = minR13Distance(r13_1, r13_2, permutations);

   std::cout << "Minimum R13 distance after permutations: " << minDist << std::endl;

   return 0;
}