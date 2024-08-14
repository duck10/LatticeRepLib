#include <iostream>
#include <array>
#include <cmath>
#include <iomanip>

using S6 = std::array<double, 6>;
using R9 = std::array<double, 9>;

R9 embed_S6_to_R9(const S6& s) {
   return {
       s[0] + s[1] + s[2],
       s[3] + s[4] + s[5],
       s[0] - s[1],
       s[1] - s[2],
       s[3] - s[4],
       s[4] - s[5],
       s[0] - s[3],
       s[1] - s[4],
       s[2] - s[5]
   };
}

S6 recover_S6_from_R9(const R9& e) {
   S6 s;
   s[0] = (e[0] + 2 * e[2] + e[3]) / 3.0;
   s[1] = (e[0] - e[2] + e[3]) / 3.0;
   s[2] = (e[0] - e[2] - 2 * e[3]) / 3.0;
   s[3] = (e[1] + 2 * e[4] + e[5]) / 3.0;
   s[4] = (e[1] - e[4] + e[5]) / 3.0;
   s[5] = (e[1] - e[4] - 2 * e[5]) / 3.0;
   return s;
}

double recovery_error(const S6& original, const S6& recovered) {
   double sum_sq = 0.0;
   for (size_t i = 0; i < 6; ++i) {
      double diff = original[i] - recovered[i];
      sum_sq += diff * diff;
   }
   return std::sqrt(sum_sq);
}

template<typename Vector>
void print_vector(const Vector& v, const std::string& name) {
   std::cout << name << ": [";
   for (size_t i = 0; i < v.size(); ++i) {
      std::cout << std::setprecision(15) << v[i];
      if (i < v.size() - 1) std::cout << ", ";
   }
   std::cout << "]\n";
}

void test_embedding_and_recovery(const S6& s6) {
   std::cout << "\nTesting S6: ";
   print_vector(s6, "Original S6");

   R9 r9 = embed_S6_to_R9(s6);
   print_vector(r9, "Embedded R9");

   S6 recovered_s6 = recover_S6_from_R9(r9);
   print_vector(recovered_s6, "Recovered S6");

   double error = recovery_error(s6, recovered_s6);
   std::cout << "Recovery error: " << std::setprecision(15) << error << "\n";
}

int main() {
   std::cout << std::fixed;

   S6 s6_1 = { -20, -30, -40, -50, -60, -70 };
   test_embedding_and_recovery(s6_1);

   S6 s6_2 = { -0.01, -20, -30, -40, -50, -60 };
   test_embedding_and_recovery(s6_2);

   S6 s6_3 = { 0, 0, -30, -40.01, -39.99, -50 };
   test_embedding_and_recovery(s6_3);

   return 0;
}