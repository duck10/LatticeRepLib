#include <iostream>
#include <vector>
#include <functional>
#include <unordered_set>

// 1. Structure definition (Only declared ONCE at the top)
struct Matrix3x3 {
   int m[3][3];
};

// 2. The self-contained counting function
long long count_matrices_for_det(int target_det) {
   int limit = (target_det == 1) ? 1 : target_det;

   std::vector<int> elements;
   for (int i = -limit; i <= limit; ++i) {
      elements.push_back(i);
   }

   std::unordered_set<int> element_set(elements.begin(), elements.end());
   long long match_count = 0;
   Matrix3x3 mat;

   for (int a : elements) {
      mat.m[0][0] = a;
      for (int b : elements) {
         mat.m[0][1] = b;
         for (int c : elements) {
            mat.m[0][2] = c;

            for (int d : elements) {
               mat.m[1][0] = d;
               for (int e : elements) {
                  mat.m[1][1] = e;
                  for (int f : elements) {
                     mat.m[1][2] = f;

                     int v1 = b * f - c * e;
                     int v2 = c * d - a * f;
                     int v3 = a * e - b * d;

                     for (int g : elements) {
                        mat.m[2][0] = g;
                        for (int h : elements) {
                           mat.m[2][1] = h;

                           int remainder = target_det - (g * v1 + h * v2);

                           if (v3 == 0) {
                              if (remainder == 0) {
                                 match_count += elements.size();
                              }
                           } else {
                              if (remainder % v3 == 0) {
                                 int i = remainder / v3;
                                 if (element_set.count(i)) {
                                    match_count++;
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }
   return match_count;
}

// 3. SINGLE cleanly defined main function
int main() {
   std::cout << "--- Running Determinant Matrix Tests ---\n\n";

   // Run the baseline test you wanted to verify
   std::cout << "Det = 1 (Expected 3480): " << count_matrices_for_det(1) << " matrices\n\n";

   // Run the automated loop for n = 2 through 6
   for (int n = 2; n <= 6; ++n) {
      std::cout << "Det = " << n << " (Elements -" << n << " to " << n << "): "
         << count_matrices_for_det(n) << " matrices\n";
   }

   return 0;
}
