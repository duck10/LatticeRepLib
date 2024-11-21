#include "Distance.h"
#include "R9.h"
#include "S6.h"
#include "Selling.h"
#include "Niggli.h"

#include <cmath>
#include <iostream>

//double Distance_CS6Dist::dist(const S6& v1, const S6& v2) const {
//   S6 s6_1u(v1.data());
//   S6 s6_2u(v2.data());
//
//   // Add debug output
//   std::cout << "Before Selling reduction: " << std::endl;
//   std::cout << "s6_1: " << s6_1u << std::endl;
//   std::cout << "s6_2: " << s6_2u << std::endl;
//
//   // Apply Selling reduction
//   S6 s6_1;
//   S6 s6_2;
//   Selling::Reduce(s6_1u, s6_1);
//   Selling::Reduce(s6_2u, s6_2);
//
//   // Add more debug output
//   std::cout << "After Selling reduction: " << std::endl;
//   std::cout << "s6_1: " << s6_1 << std::endl;
//   std::cout << "s6_2: " << s6_2 << std::endl;
//
//   double sum = 0.0;
//   for (size_t i = 0; i < 6; ++i) {
//      double diff = s6_1[i] - s6_2[i];
//      sum += diff * diff;
//   }
//
//   double result = std::sqrt(sum);
//   std::cout << "CS6Dist result: " << result << std::endl;
//
//   return result;
//}
//double Distance_NCDist::dist(const S6& v1, const S6& v2) const {
//   G6 g6_1(S6(v1.data()));
//   G6 g6_2(S6(v2.data()));
//
//   // Add debug output
//   std::cout << "Before Niggli reduction: " << std::endl;
//   std::cout << "g6_1: " << g6_1 << std::endl;
//   std::cout << "g6_2: " << g6_2 << std::endl;
//
//   // Apply Niggli reduction
//   Niggli::Reduce(g6_1, g6_1);
//   Niggli::Reduce(g6_2, g6_2);
//
//   // Add more debug output
//   std::cout << "After Niggli reduction: " << std::endl;
//   std::cout << "g6_1: " << g6_1 << std::endl;
//   std::cout << "g6_2: " << g6_2 << std::endl;
//
//   double sum = 0.0;
//   for (size_t i = 0; i < 3; ++i) {
//      double diff = g6_1[i] - g6_2[i];
//      sum += diff * diff;
//   }
//   for (size_t i = 3; i < 6; ++i) {
//      double diff = g6_1[i] - g6_2[i];
//      sum += 0.5 * diff * diff;
//   }
//
//   double result = std::sqrt(sum);
//   std::cout << "NCDist result: " << result << std::endl;
//
//   return result;
//}
//double Distance_V7Dist::dist(const S6& v1, const S6& v2) const {
//   // Implement V7 distance calculation
//   return 0.0; // Placeholder
//}
//
//double Distance_R9Dist::dist(const S6& v1, const S6& v2) const {
//   G6 g6_1;
//   G6 g6_2;
//   Niggli::Reduce(v1, g6_1);
//   Niggli::Reduce(v2, g6_2);
//
//   // Convert to R9
//   R9 r9_1(g6_1);
//   R9 r9_2(g6_2);
//   double sum = 0.0;
//   for (size_t i = 0; i < 9; ++i) {
//      double diff = r9_1[i] - r9_2[i];
//      sum += diff * diff;
//   }
//   return std::sqrt(sum);
//}
