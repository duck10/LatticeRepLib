#ifndef R9_H
#define R9_H

#include <array>
#include <vector>

#include "S6.h"

#include <vector>
#include <array>

// Assuming MatR9 is defined as a 9x9 matrix
//using MatR9 = std::array<std::array<int, 9>, 9>;
//
//std::vector<MatR9> create_r9_permutation_matrices() {
//   std::vector<MatR9> permutations;
//
//   auto identity = []() {
//      MatR9 mat{};
//      for (int i = 0; i < 9; ++i) mat[i][i] = 1;
//      return mat;
//      };
//
//   // 1. Identity
//   permutations.push_back(identity());
//
//   // 2. Swap s0 and s1
//   {
//      MatR9 mat = identity();
//      mat[2][2] = -1; mat[2][3] = 0; mat[3][2] = 1; mat[3][3] = 1;
//      mat[6][6] = 0; mat[6][7] = 1; mat[7][6] = 1; mat[7][7] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 3. Swap s0 and s2
//   {
//      MatR9 mat = identity();
//      mat[2][2] = 0; mat[2][3] = 1; mat[3][2] = -1; mat[3][3] = -1;
//      mat[6][6] = 0; mat[6][8] = 1; mat[8][6] = 1; mat[8][8] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 4. Swap s1 and s2
//   {
//      MatR9 mat = identity();
//      mat[2][3] = -1; mat[3][2] = 0; mat[3][3] = -1;
//      mat[7][7] = 0; mat[7][8] = 1; mat[8][7] = 1; mat[8][8] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 5. Swap s3 and s4
//   {
//      MatR9 mat = identity();
//      mat[4][4] = -1; mat[4][5] = 0; mat[5][4] = 1; mat[5][5] = 1;
//      mat[6][6] = 0; mat[6][7] = 1; mat[7][6] = 1; mat[7][7] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 6. Swap s3 and s5
//   {
//      MatR9 mat = identity();
//      mat[4][4] = 0; mat[4][5] = 1; mat[5][4] = -1; mat[5][5] = -1;
//      mat[6][6] = 0; mat[6][8] = 1; mat[8][6] = 1; mat[8][8] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 7. Swap s4 and s5
//   {
//      MatR9 mat = identity();
//      mat[4][5] = -1; mat[5][4] = 0; mat[5][5] = -1;
//      mat[7][7] = 0; mat[7][8] = 1; mat[8][7] = 1; mat[8][8] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 8. Swap s0 and s3
//   {
//      MatR9 mat = identity();
//      mat[6][6] = -1;
//      permutations.push_back(mat);
//   }
//
//   // 9. Swap s0 and s4
//   {
//      MatR9 mat = identity();
//      mat[6][7] = -1; mat[7][6] = -1; mat[6][6] = 0; mat[7][7] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 10. Swap s0 and s5
//   {
//      MatR9 mat = identity();
//      mat[6][8] = -1; mat[8][6] = -1; mat[6][6] = 0; mat[8][8] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 11. Swap s1 and s3
//   {
//      MatR9 mat = identity();
//      mat[6][7] = 1; mat[7][6] = 1; mat[6][6] = 0; mat[7][7] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 12. Swap s1 and s4
//   {
//      MatR9 mat = identity();
//      mat[7][7] = -1;
//      permutations.push_back(mat);
//   }
//
//   // 13. Swap s1 and s5
//   {
//      MatR9 mat = identity();
//      mat[7][8] = -1; mat[8][7] = -1; mat[7][7] = 0; mat[8][8] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 14. Swap s2 and s3
//   {
//      MatR9 mat = identity();
//      mat[6][8] = 1; mat[8][6] = 1; mat[6][6] = 0; mat[8][8] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 15. Swap s2 and s4
//   {
//      MatR9 mat = identity();
//      mat[7][8] = 1; mat[8][7] = 1; mat[7][7] = 0; mat[8][8] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 16. Swap s2 and s5
//   {
//      MatR9 mat = identity();
//      mat[8][8] = -1;
//      permutations.push_back(mat);
//   }
//
//   // 17. Swap (s0,s1) and (s3,s4)
//   {
//      MatR9 mat = identity();
//      mat[2][2] = -1; mat[2][3] = 0; mat[3][2] = 1; mat[3][3] = 1;
//      mat[4][4] = -1; mat[4][5] = 0; mat[5][4] = 1; mat[5][5] = 1;
//      mat[6][6] = 0; mat[6][7] = -1; mat[7][6] = -1; mat[7][7] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 18. Swap (s0,s2) and (s3,s5)
//   {
//      MatR9 mat = identity();
//      mat[2][2] = 0; mat[2][3] = 1; mat[3][2] = -1; mat[3][3] = -1;
//      mat[4][4] = 0; mat[4][5] = 1; mat[5][4] = -1; mat[5][5] = -1;
//      mat[6][6] = 0; mat[6][8] = -1; mat[8][6] = -1; mat[8][8] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 19. Swap (s0,s1,s2) and (s3,s4,s5) cyclically
//   {
//      MatR9 mat = identity();
//      mat[2][2] = 0; mat[2][3] = 1; mat[3][2] = -1; mat[3][3] = -1;
//      mat[4][4] = 0; mat[4][5] = 1; mat[5][4] = -1; mat[5][5] = -1;
//      mat[6][7] = 1; mat[7][8] = 1; mat[8][6] = 1;
//      mat[6][6] = 0; mat[7][7] = 0; mat[8][8] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 20. Swap (s0,s2,s1) and (s3,s5,s4) cyclically
//   {
//      MatR9 mat = identity();
//      mat[2][2] = 0; mat[2][3] = -1; mat[3][2] = 1; mat[3][3] = -1;
//      mat[4][4] = 0; mat[4][5] = -1; mat[5][4] = 1; mat[5][5] = -1;
//      mat[6][8] = 1; mat[7][6] = 1; mat[8][7] = 1;
//      mat[6][6] = 0; mat[7][7] = 0; mat[8][8] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 21. Swap s0 and s3, s1 and s5, s2 and s4
//   {
//      MatR9 mat = identity();
//      mat[6][6] = -1;
//      mat[7][8] = -1; mat[8][7] = -1; mat[7][7] = 0; mat[8][8] = 0;
//      mat[2][2] = 0; mat[2][3] = 1; mat[3][2] = 1; mat[3][3] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 22. Swap s0 and s4, s1 and s3, s2 and s5
//   {
//      MatR9 mat = identity();
//      mat[6][7] = -1; mat[7][6] = -1; mat[6][6] = 0; mat[7][7] = 0;
//      mat[2][2] = 0; mat[2][3] = -1; mat[3][2] = -1; mat[3][3] = 0;
//      mat[8][8] = -1;
//      permutations.push_back(mat);
//   }
//
//   // 23. Swap s0 and s5, s1 and s4, s2 and s3
//   {
//      MatR9 mat = identity();
//      mat[6][8] = -1; mat[8][6] = -1; mat[6][6] = 0; mat[8][8] = 0;
//      mat[7][7] = -1;
//      mat[2][2] = 0; mat[2][3] = 1; mat[3][2] = 1; mat[3][3] = 0;
//      permutations.push_back(mat);
//   }
//
//   // 24. Invert all swaps (equivalent to identity for even number of swaps)
//   permutations.push_back(identity());
//
//   return permutations;
//}

typedef std::array<int, 81> Matrix9x9;


std::vector<Matrix9x9> create_r9_permutation_matrices() {
   return {
      // 1. Identity
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,1,0, 0,0,0,0,0,0,0,0,1},

      // 2. Swap s0 and s1
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,-1,1,0,0,0,0,0, 0,0,1,1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,1,0, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,1},

      // 3. Swap s0 and s2
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,-1,-1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,0,1, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,1,0},

      // 4. Swap s1 and s2
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,-1,0,0,0,0,0, 0,0,0,-1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,1, 0,0,0,0,0,0,0,1,0},

      // 5. Swap s3 and s4
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,-1,1,0,0,0, 0,0,0,0,1,1,0,0,0, 0,0,0,0,0,0,0,1,0, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,1},

      // 6. Swap s3 and s5
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,-1,-1,0,0,0, 0,0,0,0,0,0,0,0,1, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,1,0},

      // 7. Swap s4 and s5
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,1,-1,0,0,0, 0,0,0,0,0,-1,0,0,0, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,1, 0,0,0,0,0,0,0,1,0},

      // 8. Swap s0 and s3
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,-1,0,0, 0,0,0,0,0,0,0,1,0, 0,0,0,0,0,0,0,0,1},

      // 9. Swap s0 and s4
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,-1,0, 0,0,0,0,0,0,-1,0,0, 0,0,0,0,0,0,0,0,1},

      // 10. Swap s0 and s5
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,0,-1, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,-1,0,0},

      // 11. Swap s1 and s3
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,1,0, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,1},

      // 12. Swap s1 and s4
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,-1,0, 0,0,0,0,0,0,0,0,1},

      // 13. Swap s1 and s5
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,-1, 0,0,0,0,0,0,0,-1,0},

      // 14. Swap s2 and s3
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,0,1, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,1,0,0},

      // 15. Swap s2 and s4
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,1, 0,0,0,0,0,0,0,1,0},

      // 16. Swap s2 and s5
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,1,0, 0,0,0,0,0,0,0,0,-1},

      // 17. Swap (s0,s1) and (s3,s4)
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,-1,1,0,0,0,0,0, 0,0,1,1,0,0,0,0,0, 0,0,0,0,-1,1,0,0,0, 0,0,0,0,1,1,0,0,0, 0,0,0,0,0,0,0,-1,0, 0,0,0,0,0,0,-1,0,0, 0,0,0,0,0,0,0,0,1},

      // 18. Swap (s0,s2) and (s3,s5)
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,-1,-1,0,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,-1,-1,0,0,0, 0,0,0,0,0,0,0,0,-1, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,-1,0,0},

      // 19. Swap (s0,s1,s2) and (s3,s4,s5) cyclically
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,-1,-1,0,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,-1,-1,0,0,0, 0,0,0,0,0,0,0,1,0, 0,0,0,0,0,0,0,0,1, 0,0,0,0,0,0,1,0,0},

      // 20. Swap (s0,s2,s1) and (s3,s5,s4) cyclically
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,0,-1,0,0,0,0,0, 0,0,1,-1,0,0,0,0,0, 0,0,0,0,0,-1,0,0,0, 0,0,0,0,1,-1,0,0,0, 0,0,0,0,0,0,0,0,1, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,1,0},

      // 21. Swap s0 and s3, s1 and s5, s2 and s4
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,-1,0,0, 0,0,0,0,0,0,0,0,-1, 0,0,0,0,0,0,0,-1,0},

      // 22. Swap s0 and s4, s1 and s3, s2 and s5
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,0,-1,0,0,0,0,0, 0,0,-1,0,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,-1,0, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,-1},

      // 23. Swap s0 and s5, s1 and s4, s2 and s3
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,0,-1, 0,0,0,0,0,0,0,-1,0, 0,0,0,0,0,0,-1,0,0},

      // 24. Identity (invert all swaps for even number of swaps)
      {1,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0, 0,0,0,0,1,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,1,0, 0,0,0,0,0,0,0,0,1}
   };
}
      
class R9 {
public:
   friend std::ostream& operator<< (std::ostream& o, const R9& obc) {
      o << "R9";
      for (const auto& d : obc.m_data) {
         std::cout << " " << d;
      }
      return o;
   }

   R9() = default;

   R9(double e1, double e2, double e3, double e4, double e5,
      double e6, double e7, double e8, double e9)
      : m_data{ e1, e2, e3, e4, e5, e6, e7, e8, e9 }
   {
   }

   //template<typename T>
   //R9(const T& t) {
   //   const S6 s(t);
   //   m_data = {
   //       s[0] + s[1] + s[2],
   //       s[3] + s[4] + s[5],
   //       s[0] - s[1],
   //       s[1] - s[2],
   //       s[3] - s[4],
   //       s[4] - s[5],
   //       s[0] - s[3],
   //       s[1] - s[4],
   //       s[2] - s[5]
   //   };
   //}
   template<typename T>
   R9(const T& t) {
      const S6 s(t);
      std::cout << "S6 input: ";
      for (int i = 0; i < 6; ++i) {
         std::cout << s[i] << " ";
      }
      std::cout << std::endl;

      m_data = {
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

      std::cout << "Calculated R9: ";
      for (int i = 0; i < 9; ++i) {
         std::cout << m_data[i] << " ";
      }
      std::cout << std::endl;
   }
   double& operator[](size_t index) {
      return m_data[index];
   }

   double operator[](size_t index) const {
      return m_data[index];
   }

   operator S6() const {
      S6 s;
      const std::array<double, 9>& e(m_data);
      s[0] = (e[0] + 2 * e[2] + e[3]) / 3.0;
      s[1] = (e[0] - e[2] + e[3]) / 3.0;
      s[2] = (e[0] - e[2] - 2 * e[3]) / 3.0;
      s[3] = (e[1] + 2 * e[4] + e[5]) / 3.0;
      s[4] = (e[1] - e[4] + e[5]) / 3.0;
      s[5] = (e[1] - e[4] - 2 * e[5]) / 3.0;
      return s;
   }

   double norm() const {
      double sum = 0.0;
      for (const auto& d : m_data) {
         sum += d * d;
      }
      return sqrt(sum);
   }

   R9 operator*(const double d) {
      const auto& e = m_data;
      return R9(
         e[0] * d, 
         e[1] * d, 
         e[2] * d, 
         e[3] * d, 
         e[4] * d, 
         e[5] * d, 
         e[6] * d, 
         e[7] * d, 
         e[8] * d);
   }

   R9 operator-(const R9& f) {
      const auto& e = m_data;
      return R9(
         e[0] - f[0], 
         e[1] - f[1], 
         e[2] - f[2], 
         e[3] - f[3], 
         e[4] - f[4], 
         e[5] - f[5], 
         e[6] - f[6],
         e[7] - f[7],
         e[8] - f[8]);
   }

   R9 operator+(const R9& f) {
      const auto& e = m_data;
      return R9(
         e[0] + f[0],
         e[1] + f[1],
         e[2] + f[2],
         e[3] + f[3],
         e[4] + f[4],
         e[5] + f[5],
         e[6] + f[6],
         e[7] + f[7],
         e[8] + f[8]);
   }

   std::array<double, 9> GetArray() const { return m_data; }
   std::vector<double> GetVector() const {
      std::vector<double> out;
      for (const auto& d : m_data) {
         out.emplace_back(d);
      }
      return out;
   }

   std::string GetName() const { return "R9"; }
   double* data() { return m_data.data(); }

private:
   std::array<double, 9> m_data;

};


static R9 MV(const Matrix9x9& m, const R9& v)
{
   R9 out;
   for (size_t i = 0; i < 9; ++i) {
      double sum = 0;
      for (size_t kk = 9 * i; kk < 9 * i + 9; ++kk) {
         sum += v[i] * m[kk];
      }
      out[i] = sum;
   }
   return out;
}

double r9_lattice_distance(const R9& r9_1, const R9& r9_2, const std::vector<Matrix9x9>& r9_permutations) {
   double min_distance = std::numeric_limits<double>::max();

   for (const auto& Q : r9_permutations) {
      R9 permuted_r9_1 = MV(Q, r9_1);
      double distance = (permuted_r9_1 - r9_2).norm();
      min_distance = std::min(min_distance, distance);
   }

   return min_distance;
}

#endif // R9_H