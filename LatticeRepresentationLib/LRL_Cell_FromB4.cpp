// Conversions between LRL_Cell and B4, plus LRL_Cell::DistanceBetween,
// which is implemented in terms of B4 even though it is not itself a
// conversion. Kept in its own translation unit so that programs which
// never construct an LRL_Cell from a B4 and never call DistanceBetween()
// do not need to compile or link against B4.h.

#include "LRL_Cell.h"
#include "B4.h"

#include <cmath>
#include <iostream>

#ifndef M_PI
static const double M_PI = 4.0 * atan(1.0);
#endif

LRL_Cell::LRL_Cell(const B4& b4)
{
   // Extract all four basis vectors from B4
   const Vector_3& a = b4[0];
   const Vector_3& b = b4[1];
   const Vector_3& c = b4[2];
   const Vector_3& d = b4[3];  // Include the fourth vector

   // Verify that d = -(a+b+c) which should be true for a Delone tetrahedron
   Vector_3 sumCheck = a + b + c + d;
   if (sumCheck.Norm() > 1e-10) {
      std::cerr << "Warning: B4 vectors don't satisfy a+b+c+d=0 constraint" << std::endl;
   }

   // Calculate the magnitudes (cell edges)
   const double a_len = a.Norm();
   const double b_len = b.Norm();
   const double c_len = c.Norm();

   // Calculate the angles (in radians)
   const double alpha = acos(b.Dot(c) / (b_len * c_len));
   const double beta = acos(a.Dot(c) / (a_len * c_len));
   const double gamma = acos(a.Dot(b) / (a_len * b_len));

   // For nearly cubic cells, check if we need to adjust orientation
   if (std::abs(alpha - M_PI / 2) < 0.1 &&
      std::abs(beta - M_PI / 2) < 0.1 &&
      std::abs(gamma - M_PI / 2) < 0.1) {

      // Check the handedness using all four vectors
      Vector_3 crossProduct = a.Cross(b);
      bool rightHanded = (crossProduct.Dot(c) > 0);

      // We need a consistent way to decide which cell representation to use
      // When permutations have been applied
      double adjustedAlpha = alpha;
      double adjustedBeta = beta;
      double adjustedGamma = gamma;

      // If handedness needs to be preserved after permutation,
      // flip two angles (not just one) to maintain an equivalent cell
      if (!rightHanded) {
         // Flip both alpha and beta
         adjustedAlpha = M_PI - alpha;
         adjustedBeta = M_PI - beta;

         // Alternative: flip beta and gamma
         // adjustedBeta = M_PI - beta;
         // adjustedGamma = M_PI - gamma;
      }

      // Return the LRL_Cell with adjusted parameters
      (*this) = LRL_Cell(a_len, b_len, c_len, adjustedAlpha, adjustedBeta, adjustedGamma);
   }
   else {
      // For non-cubic cells, use the standard calculation
      (*this) = LRL_Cell(a_len, b_len, c_len, alpha, beta, gamma);
   }
}

LRL_Cell& LRL_Cell::operator= (const B4& v) {
   *this = LRL_Cell(v);
   return *this;
}

double LRL_Cell::DistanceBetween(const LRL_Cell& v1, const LRL_Cell& v2) {
   return (B4(v1) - B4(v2)).norm();
}
