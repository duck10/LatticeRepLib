// this is Cline's ACM Algorithm 476, spline under tension

#include <vector>
#include <cmath>
#include <iostream>
#include <stdexcept>

/** Implementation Notes:
 *
 * Algorithm Details:
 * 1. For curv1:
 *    - If no derivatives are given (sigma < 0), uses second order polynomial
 *      interpolation on input data for values at endpoints
 *    - For n = 2 with no derivatives, uses straight line
 *    - Sets up a tridiagonal system for the second derivatives
 *    - Performs forward elimination followed by back substitution
 *
 * 2. For curv2:
 *    - Searches for the interval containing the target point
 *    - Uses the tension spline formula for interpolation within the interval
 *    - Implements efficient searching when points are ordered
 *
 * Reference: ACM Algorithm 476
 * Source: https://dl.acm.org/doi/pdf/10.1145/360924.360974
 */


// CURV1: Determines parameters necessary to compute an interpolatory spline under tension
void curv1(int n,
   const std::vector<double>& x,
   const std::vector<double>& y,
   double slp1,
   double slpn,
   std::vector<double>& yp,
   std::vector<double>& temp,
   double sigma) {
   /**
    * @brief Determines the parameters necessary to compute an interpolatory spline under tension
    *        through a sequence of functional values. The slopes at the two ends of the curve may
    *        be specified or omitted. For actual computation of points on the curve, curv2()
    *        should be called.
    *
    * @param n     The number of values to be interpolated (n >= 2)
    * @param x     Array of n increasing abscissae of the functional values
    * @param y     Array of n ordinates of the values (y[k] is the functional value corresponding to x[k])
    * @param slp1  Desired value for the first derivative of the curve at x[0]
    *              (ignored if sigma is negative)
    * @param slpn  Desired value for the first derivative of the curve at x[n-1]
    *              (ignored if sigma is negative)
    * @param yp    Output array of length n containing values proportional to the second
    *              derivative of the curve at the given nodes
    * @param temp  Scratch storage array of length n
    * @param sigma Tension factor (non-zero). Indicates the curviness desired:
    *             - If |sigma| is nearly zero (e.g., 0.001), the result is approximately a cubic spline
    *             - If |sigma| is large (e.g., 50), the result is nearly a polygonal line
    *             - The sign of sigma indicates whether derivative information has been input:
    *               - If sigma is negative, endpoint derivatives will be determined internally
    *               - A standard value is approximately 1 in absolute value
    *
    * @note The function assumes x values are strictly increasing
    * @note n, x, y, slp1, slpn and sigma are unaltered by the function
    */
    // Input validation
   if (n < 2) {
      throw std::invalid_argument("At least two points required");
   }
   if (x.size() != static_cast<size_t>(n) || y.size() != static_cast<size_t>(n)) {
      throw std::invalid_argument("Array sizes don't match n");
   }
   // Validate x is increasing
   for (int i = 1; i < n; ++i) {
      if (x[i] <= x[i - 1]) {
         throw std::invalid_argument("X values must be strictly increasing");
      }
   }

   const int nm1 = n - 1;
   const int np1 = n + 1;

   double delx1 = x[1] - x[0];
   double dx1 = (y[1] - y[0]) / delx1;

   // Determine slopes if necessary
   double slpp1, slppn;
   if (sigma < 0.0) {
      if (n == 2) {
         // If only two points and no derivatives are given, use straight line
         yp[0] = 0.0;
         yp[1] = 0.0;
         return;
      }

      // Use second order polynomial interpolation for endpoint derivatives
      double delx2 = x[2] - x[1];
      double delx12 = x[2] - x[0];
      double c1 = -(delx12 + delx1) / delx12 / delx1;
      double c2 = delx12 / delx1 / delx2;
      double c3 = -delx1 / delx12 / delx2;
      slpp1 = c1 * y[0] + c2 * y[1] + c3 * y[2];

      double deln = x[n - 1] - x[n - 2];
      double delnm1 = x[n - 2] - x[n - 3];
      double delnn = x[n - 1] - x[n - 3];
      c1 = (delnn + deln) / delnn / deln;
      c2 = -delnn / deln / delnm1;
      c3 = deln / delnn / delnm1;
      slppn = c3 * y[n - 3] + c2 * y[n - 2] + c1 * y[n - 1];
   }
   else {
      slpp1 = slp1;
      slppn = slpn;
   }

   // Denormalize tension factor
   double sigmap = std::abs(sigma) * static_cast<double>(n - 1) / (x[n - 1] - x[0]);

   // Set up right hand side and tridiagonal system for yp
   double dels = sigmap * delx1;
   double exps = std::exp(dels);
   double sinhs = 0.5 * (exps - 1.0 / exps);
   double sinhin = 1.0 / (delx1 * sinhs);
   double diag1 = sinhin * (dels * 0.5 * (exps + 1.0 / exps) - sinhs);
   double diagin = 1.0 / diag1;
   yp[0] = diagin * (dx1 - slpp1);
   double spdiag = sinhin * (sinhs - dels);
   temp[0] = diagin * spdiag;

   // Handle n=2 case
   if (n == 2) {
      diagin = 1.0 / (diag1 - spdiag * temp[0]);
      yp[1] = diagin * (slppn - dx1 - spdiag * yp[0]);
      return;
   }

   // Forward elimination
   for (int i = 1; i < nm1; ++i) {
      double delx2 = x[i + 1] - x[i];
      double dx2 = (y[i + 1] - y[i]) / delx2;
      dels = sigmap * delx2;
      exps = std::exp(dels);
      sinhs = 0.5 * (exps - 1.0 / exps);
      sinhin = 1.0 / (delx2 * sinhs);
      double diag2 = sinhin * (dels * 0.5 * (exps + 1.0 / exps) - sinhs);
      diagin = 1.0 / (diag1 + diag2 - spdiag * temp[i - 1]);
      yp[i] = diagin * (dx2 - dx1 - spdiag * yp[i - 1]);
      spdiag = sinhin * (sinhs - dels);
      temp[i] = diagin * spdiag;
      dx1 = dx2;
      diag1 = diag2;
   }

   // Final yp calculation
   diagin = 1.0 / (diag1 - spdiag * temp[nm1 - 1]);
   yp[n - 1] = diagin * (slppn - dx1 - spdiag * yp[nm1 - 1]);

   // Back substitution
   for (int i = 2; i <= n; ++i) {
      int ibak = np1 - i;
      yp[ibak - 1] = yp[ibak - 1] - temp[ibak - 1] * yp[ibak];
   }
}

// CURV2: Interpolates a curve at a given point using a spline under tension
double curv2(double t,
   int n,
   const std::vector<double>& x,
   const std::vector<double>& y,
   const std::vector<double>& yp,
   double sigma,
   int& it) {
   /**
    * @brief Interpolates a curve at a given point using a spline under tension.
    *        The function curv1() should be called earlier to determine necessary parameters.
    *
    * @param t     Value to be mapped onto the interpolating curve
    * @param n     Number of points which were interpolated to determine the curve
    * @param x     Array containing the abscissae of the interpolated points
    * @param y     Array containing the ordinates of the interpolated points
    * @param yp    Array with values proportional to the second derivative of the curve at the nodes
    *              (output from curv1)
    * @param sigma Tension factor (its sign is ignored)
    * @param it    Search optimization parameter:
    *              - If it is not 1, indicates the function has been called previously
    *                (with n, x, y, yp, and sigma unaltered) and that this value of t
    *                exceeds the previous value
    *              - If it is 1, the search for the interval (x[k],x[k+1]) containing t
    *                starts with k=0
    *              For efficiency when interpolating at a sequence of points, order the
    *              values increasing and set it to the index of the call
    *
    * @return The interpolated value:
    *         - For t < x[0], returns y[0]
    *         - For t > x[n-1], returns y[n-1]
    *         - Otherwise returns the interpolated value
    *
    * @note The parameters n, x, y, yp and sigma should be input unaltered from the output of curv1
    * @note None of the input parameters are altered except it
    */
    
    // Input validation
   if (n < 2) {
      throw std::invalid_argument("At least two points required");
   }
   if (x.size() != static_cast<size_t>(n) ||
      y.size() != static_cast<size_t>(n) ||
      yp.size() != static_cast<size_t>(n)) {
      throw std::invalid_argument("Array sizes don't match n");
   }

   // Handle out-of-range t values
   if (t <= x[0]) return y[0];
   if (t >= x[n - 1]) return y[n - 1];

   double s = x[n - 1] - x[0];
   // Denormalize sigma
   double sigmap = std::abs(sigma) * static_cast<double>(n - 1) / s;

   // Start search where previously terminated if it != 1
   int i1 = (it == 1) ? 1 : it;

   // Search for interval
   int i;
   for (i = i1; i < n; ++i) {
      if (x[i] >= t) break;
   }

   // Check if interval is correct
   if (x[i - 1] > t || t > x[i]) {
      // Restart search if it was incorrect
      i1 = 1;
      for (i = i1; i < n; ++i) {
         if (x[i] >= t) break;
      }
   }

   // Set up and perform interpolation
   double del1 = t - x[i - 1];
   double del2 = x[i] - t;
   double dels = x[i] - x[i - 1];
   double exps1 = std::exp(sigmap * del1);
   double sinhd1 = 0.5 * (exps1 - 1.0 / exps1);
   double exps = std::exp(sigmap * del2);
   double sinhd2 = 0.5 * (exps - 1.0 / exps);
   exps = exps1 * exps;
   double sinhs = 0.5 * (exps - 1.0 / exps);

   // Store interval for next call
   it = i;

   return (yp[i] * sinhd1 + yp[i - 1] * sinhd2) / sinhs +
      ((y[i] - yp[i]) * del1 + (y[i - 1] - yp[i - 1]) * del2) / dels;
}

int main() {
   try {
      // Test data: simple curve with 5 points
      std::vector<double> x = { 0.0, 1.0, 2.0, 3.0, 4.0 };
      std::vector<double> y = { 0.0, 1.0, 0.0, 1.0, 0.0 };
      int n = x.size();

      // Initialize arrays for CURV1
      std::vector<double> yp(n);
      std::vector<double> temp(n);

      // Parameters
      double slp1 = 1.0;  // Initial slope
      double slpn = -1.0; // Final slope
      double sigma = 1.0; // Tension factor

      // Calculate spline parameters
      curv1(n, x, y, slp1, slpn, yp, temp, sigma);

      // Test interpolation at various points
      std::cout << "Interpolated values:\n";
      int it = 1;  // Initialize search index
      for (double t = -0.5; t <= 4.5; t += 0.5) {
         double val = curv2(t, n, x, y, yp, sigma, it);
         std::cout << "t = " << t << ": " << val << std::endl;
      }

      // Test with automatically determined slopes
      std::cout << "\nWith automatic slopes:\n";
      sigma = -1.0;  // Negative sigma for automatic slope calculation
      curv1(n, x, y, slp1, slpn, yp, temp, sigma);
      it = 1;
      for (double t = -0.5; t <= 4.5; t += 0.5) {
         double val = curv2(t, n, x, y, yp, sigma, it);
         std::cout << "t = " << t << ": " << val << std::endl;
      }
   }
   catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
   }

   return 0;
}