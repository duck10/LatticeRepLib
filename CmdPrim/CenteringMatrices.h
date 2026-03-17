#ifndef CENTERING_MATRICES_H
#define CENTERING_MATRICES_H

#include "Matrix_3x3.h"

#include <string>

// =============================================================================
// Standard centering-to-primitive transformation matrices (3x3, real space).
// Reference: International Tables for Crystallography, Vol. A, Table 5.1.3.1
//
// Convention: if B_centered = [a b c] are the centered basis vectors, then
//   B_primitive = M * B_centered
// i.e. each row of M gives the primitive vector as a linear combination
// of the centered vectors.
//
// Determinants (= volume ratio V_primitive / V_centered):
//   P: 1     A,B,C: 1/2     I: 1/2     F: 1/4     R(obv): 1/3
//
// Note: for P there is no centering, so the identity is returned.
// =============================================================================

namespace CenteringMatrices {

   // P -- primitive: identity, no transformation needed
   inline Matrix_3x3 P() {
      return Matrix_3x3(
         1, 0, 0,
         0, 1, 0,
         0, 0, 1
      );
   }

   // A-centered: centering vector (0, 1/2, 1/2)
   //   a_p =  a
   //   b_p = -b + c      (or the ITA choice: b_p = b/2 - c/2 ... )
   // ITA Table 5.1.3.1:
   //   a_p = a,  b_p = (b+c)/2,  c_p = (-b+c)/2  =>
   //   rows: [1 0 0 | 0 1/2 1/2 | 0 -1/2 1/2]  -- fractional
   // Stored as exact fractions * 2 to keep integers -- but Matrix_3x3 is double,
   // so we use the exact rational values directly.
   inline Matrix_3x3 A() {
      return Matrix_3x3(
         1.0, 0.0, 0.0,
         0.0, 0.5, 0.5,
         0.0, -0.5, 0.5
      );
   }

   // B-centered: centering vector (1/2, 0, 1/2)
   //   a_p = (a+c)/2,  b_p = b,  c_p = (-a+c)/2
   inline Matrix_3x3 B() {
      return Matrix_3x3(
         0.5, 0.0, 0.5,
         0.0, 1.0, 0.0,
         -0.5, 0.0, 0.5
      );
   }

   // C-centered: centering vector (1/2, 1/2, 0)
   //   a_p = (a+b)/2,  b_p = (-a+b)/2,  c_p = c
   inline Matrix_3x3 C() {
      return Matrix_3x3(
         0.5, 0.5, 0.0,
         -0.5, 0.5, 0.0,
         0.0, 0.0, 1.0
      );
   }

   // I-centered (body): centering vector (1/2, 1/2, 1/2)
   //   a_p = (-a+b+c)/2,  b_p = (a-b+c)/2,  c_p = (a+b-c)/2
   inline Matrix_3x3 I() {
      return Matrix_3x3(
         -0.5, 0.5, 0.5,
         0.5, -0.5, 0.5,
         0.5, 0.5, -0.5
      );
   }

   // F-centered (all-face): centering vectors (0,1/2,1/2),(1/2,0,1/2),(1/2,1/2,0)
   //   a_p = (b+c)/2,  b_p = (a+c)/2,  c_p = (a+b)/2  -- one standard choice
   //   (det = 1/4, so four lattice points per conventional cell)
   inline Matrix_3x3 F() {
      return Matrix_3x3(
         0.0, 0.5, 0.5,
         0.5, 0.0, 0.5,
         0.5, 0.5, 0.0
      );
   }

   // R-centered, obverse setting (hexagonal axes):
   //   centering vectors: (2/3, 1/3, 1/3), (1/3, 2/3, 2/3)
   //   a_p = (2a+b+c)/3,  b_p = (-a+b+c)/3,  c_p = (-a-2b+c)/3
   //   (det = 1/3)
   inline Matrix_3x3 R() {
      return Matrix_3x3(
         2.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0,
         -1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0,  // wait -- see note below
         -1.0 / 3.0, -2.0 / 3.0, 1.0 / 3.0
      );
   }
   // Note on R: the obverse setting is the IUCr standard since 1990.
   // If your data uses reverse setting, swap the first two rows.

   // Dispatch by lattice type string (first character used for centering type)
   inline Matrix_3x3 ForLattice(const std::string& lattice) {
      if (lattice.empty()) return P();
      switch (lattice[0]) {
      case 'A': case 'a': return A();
      case 'B': case 'b': return B();
      case 'C': case 'c': return C();
      case 'I': case 'i': return I();
      case 'F': case 'f': return F();
      case 'R': case 'r': return R();
      case 'H': case 'h': return P();   // H is already primitive hexagonal
      default:            return P();   // P, or unknown -> identity
      }
   }

   // Format a 3x3 centering matrix as a compact string for output, e.g.
   //   [1 0 0  0 1/2 1/2  0 -1/2 1/2]
   // Rational fractions are rendered as exact fractions rather than 0.5 etc.
   inline std::string ToString(const Matrix_3x3& M) {
      // Convert a double that is a simple fraction (n/6 family) to a string
      auto frac = [](const double x) -> std::string {
         // Test common crystallographic fractions: 0, ±1, ±1/2, ±1/3, ±2/3, ±1/4, ±3/4
         struct { double val; const char* str; } table[] = {
            {  0.0,       "0"    },
            {  1.0,       "1"    },
            { -1.0,      "-1"    },
            {  0.5,       "1/2"  },
            { -0.5,      "-1/2"  },
            {  1.0 / 3.0,  "1/3"  },
            { -1.0 / 3.0, "-1/3"  },
            {  2.0 / 3.0,  "2/3"  },
            { -2.0 / 3.0, "-2/3"  },
            {  0.25,      "1/4"  },
            { -0.25,     "-1/4"  },
            {  0.75,      "3/4"  },
            { -0.75,     "-3/4"  },
         };
         for (const auto& e : table) {
            if (std::abs(x - e.val) < 1.0e-9) return e.str;
         }
         // fallback: print as decimal
         std::ostringstream oss;
         oss << x;
         return oss.str();
         };

      std::ostringstream oss;
      oss << "["
         << frac(M[0]) << " " << frac(M[1]) << " " << frac(M[2]) << "  "
         << frac(M[3]) << " " << frac(M[4]) << " " << frac(M[5]) << "  "
         << frac(M[6]) << " " << frac(M[7]) << " " << frac(M[8])
         << "]";
      return oss.str();
   }

} // namespace CenteringMatrices

#endif // CENTERING_MATRICES_H
