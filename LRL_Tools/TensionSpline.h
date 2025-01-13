#ifndef TENSION_SPLINE_H
#define TENSION_SPLINE_H

#include <vector>
#include <cmath>
#include <stdexcept>

#include "S6.h"

class TensionSpline {
public:
   // Main interface matching Fortran CURV1
   // n - number of points (n >= 2)
   // x - array of increasing abscissae 
   // y - array of function values at x
   // slp1, slpn - endpoint slopes (used if sigma >= 0)
   // yp - output array for derivative information
   // sigma - tension factor (sign indicates if slopes are used)




   static void curv1(
      const std::vector<double>& x,
      const std::vector<double>& y,
      std::vector<double>& yp,
      double sigma,
      double slp1 = -1.0,  // Default values for auto-compute mode
      double slpn = -1.0) {

      const int n = x.size();
      if (n < 2) throw std::invalid_argument("At least two points required");

      // Create temp array internally
      std::vector<double> temp(n);
      double slpp1, slppn;

      // Determine slopes if sigma negative
      if (sigma >= 0.0) {
         slpp1 = slp1;
         slppn = slpn;
      }
      else {
         // Use polynomial interpolation for negative sigma
         if (n == 2) {
            yp[0] = yp[1] = 0.0;
            return;
         }

         // Calculate endpoints using second order polynomial
         const double delx1 = x[1] - x[0];
         const double delx2 = x[2] - x[1];
         const double delx12 = x[2] - x[0];
         //std::cout << "Start slope calculation:\n";
         //std::cout << "delx1 = " << delx1 << " (x[1] - x[0])\n";
         //std::cout << "delx2 = " << delx2 << " (x[2] - x[1])\n";
         //std::cout << "delx12 = " << delx12 << " (x[2] - x[0])\n";

         const double c1 = -(delx12 + delx1) / (delx12 * delx1);
         const double c2 = delx12 / (delx1 * delx2);
         const double c3 = -delx1 / (delx12 * delx2);
         //std::cout << "coefficients: c1=" << c1 << " c2=" << c2 << " c3=" << c3 << "\n";
         //std::cout << "y values: y[0]=" << y[0] << " y[1]=" << y[1] << " y[2]=" << y[2] << "\n";
         slpp1 = c1 * y[0] + c2 * y[1] + c3 * y[2];
         //std::cout << "calculated slpp1 = " << slpp1 << "\n";

         // End point calculation - use points before the end
         const double deln = x[n - 1] - x[n - 2];       // Last interval
         const double delnm1 = x[n - 2] - x[n - 3];     // Second to last interval
         const double delnn = x[n - 1] - x[n - 3];      // Total span

         //std::cout << "\nEnd slope calculation (using points around x=2):\n";
         //std::cout << "Points used: (" << x[n - 3] << "," << y[n - 3] << "), "
         //   << "(" << x[n - 2] << "," << y[n - 2] << "), "
         //   << "(" << x[n - 1] << "," << y[n - 1] << ")\n";
         //std::cout << "Intervals: deln=" << deln << " delnm1=" << delnm1
         //   << " delnn=" << delnn << "\n";

         // Calculate coefficients
         const double c1_end = (delnn + delnm1) / (delnn * delnm1);
         const double c2_end =-delnn / (delnm1 * deln);
         const double c3_end = delnm1 / (delnn * deln);
         //std::cout << "coefficients: c1_end=" << c1_end << " c2_end=" << c2_end << " c3_end=" << c3_end << "\n";

         slppn = c3_end * y[n - 3] + c2_end * y[n - 2] + c1_end * y[n - 1];
         //std::cout << "calculated slppn = " << slppn << "\n";
      }

      // Denormalize tension factor
      double sigmap = std::abs(sigma) * (n - 1) / (x[n - 1] - x[0]);

      // Forward elimination
      double delx1 = x[1] - x[0];
      double dx1 = (y[1] - y[0]) / delx1;
      double dels = sigmap * delx1;
      double exps = std::exp(dels);
      double sinhs = 0.5 * (exps - 1.0 / exps);
      double sinhin = 1.0 / (delx1 * sinhs);
      double diag1 = sinhin * (dels * 0.5 * (exps + 1.0 / exps) - sinhs);
      double diagin = 1.0 / diag1;

      //std::cout << "\nTridiagonal system setup:\n";
      //std::cout << "sigmap = " << sigmap << "\n";
      //std::cout << "delx1 = " << delx1 << "\n";
      //std::cout << "dx1 = " << dx1 << "\n";
      //std::cout << "dels = " << dels << "\n";
      //std::cout << "exps = " << exps << "\n";
      //std::cout << "sinhs = " << sinhs << "\n";
      //std::cout << "sinhin = " << sinhin << "\n";
      //std::cout << "diag1 = " << diag1 << "\n";
      //std::cout << "diagin = " << diagin << "\n";

      yp[0] = diagin * (dx1 - slpp1);
      //std::cout << "yp[0] initial = " << yp[0] << "\n";

      double spdiag = sinhin * (sinhs - dels);
      temp[0] = diagin * spdiag;

      // Interior points
      if (n > 2) {
         for (int i = 1; i < n - 1; i++) {
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
      }

      // Last point
      diagin = 1.0 / (diag1 - spdiag * temp[n - 2]);
      yp[n - 1] = diagin * (slppn - dx1 - spdiag * yp[n - 2]);

      //std::cout << "\nBefore back substitution:\n";
      //for (int i = 0; i < n; i++) {
      //   std::cout << "yp[" << i << "] = " << yp[i] << ", temp[" << i << "] = " << temp[i] << "\n";
      //}

      // Back substitution
      for (int i = n - 2; i >= 0; i--) {
         //std::cout << "Back step " << i << ": yp[" << i << "] = " << yp[i]
         //   << " - " << temp[i] << " * " << yp[i + 1] << " = ";
         yp[i] = yp[i] - temp[i] * yp[i + 1];
         //std::cout << yp[i] << "\n";
      }
   }




   static double curv2(double t,
      int n,
      const std::vector<double>& x,
      const std::vector<double>& y,
      const std::vector<double>& yp,
      double sigma) {

      double s = x[n - 1] - x[0];
      double sigmap = std::abs(sigma) * (n - 1) / s;

      //std::cout << "\nCURV2 calculation at t = " << t << ":\n";
      //std::cout << "sigmap = " << sigmap << "\n";

      // Find interval containing t
      int i = 1;
      while (i < n) {
         double diff = x[i] - t;
         if (diff < 0.0) {
            i++;
            continue;
         }
         break;
      }

      if (i >= n) i = n - 1;

      //std::cout << "Selected interval: i = " << i << "\n";

      if (t <= x[0]) return y[0];
      if (t >= x[n - 1]) return y[n - 1];

      double del1 = t - x[i - 1];
      double del2 = x[i] - t;
      double dels = x[i] - x[i - 1];

      //std::cout << "del1 = " << del1 << " del2 = " << del2 << " dels = " << dels << "\n";

      double expsi = std::exp(sigmap * del1);
      double sinhd1 = 0.5 * (expsi - 1.0 / expsi);

      double exps = std::exp(sigmap * del2);
      double sinhd2 = 0.5 * (exps - 1.0 / exps);

      exps = expsi * exps;
      double sinhs = 0.5 * (exps - 1.0 / exps);

      //std::cout << "expsi = " << expsi << " sinhd1 = " << sinhd1 << "\n";
      //std::cout << "exps = " << exps << " sinhd2 = " << sinhd2 << "\n";
      //std::cout << "sinhs = " << sinhs << "\n";

      double tension_term = (yp[i] * sinhd1 + yp[i - 1] * sinhd2) / sinhs;
      double linear_term = ((y[i] - yp[i]) * del1 + (y[i - 1] - yp[i - 1]) * del2) / dels;

      //std::cout << "tension_term = " << tension_term << "\n";
      //std::cout << "linear_term = " << linear_term << "\n";
      //std::cout << "final result = " << tension_term + linear_term << "\n";

      return tension_term + linear_term;
   }




   static void calculate(bool closed,
      const std::vector<std::vector<double>>& points,
      std::vector<std::vector<double>>& derivatives,
      double sigma) {

      const int n = points.size();
      if (n < 2) throw std::invalid_argument("At least two points required");

      // Basic validation
      if (points[0].empty()) throw std::invalid_argument("Points cannot be empty");

      const int dim = points[0].size();
      // Check all points have same dimension
      for (const auto& p : points) {
         if (p.size() != dim) {
            throw std::invalid_argument("All points must have same dimension");
         }
      }

      // Extract x coordinates (parameter)
      std::vector<double> x(n);
      for (int i = 0; i < n; i++) {
         x[i] = points[i][0];
      }

      // Check x monotonicity for open curves
      if (!closed) {
         for (int i = 1; i < n; i++) {
            if (x[i] <= x[i - 1]) {
               throw std::runtime_error("Points must be strictly monotonic in x");
            }
         }
      }

      // Initialize derivatives array
      derivatives.resize(n);
      for (auto& d : derivatives) {
         d.resize(dim);
      }

      // Handle each component after x
      for (int d = 1; d < dim; d++) {
         std::vector<double> y(n);
         std::vector<double> yp(n);
         for (int i = 0; i < n; i++) {
            y[i] = points[i][d];
         }

         if (!closed) {
            curv1(x, y, yp, sigma);
         }
         else {
            kurvp1(x, y, yp, sigma);
         }
         // Store results
         for (int i = 0; i < n; i++) {
            derivatives[i][d] = yp[i];
         }
      }
   }

   static std::vector<double> evaluate(double t,
      const std::vector<std::vector<double>>& points,
      const std::vector<std::vector<double>>& derivatives,
      double sigma) {

      int n = static_cast<int>(points.size());
      if (n == 0) throw std::invalid_argument("No points provided");
      if (derivatives.size() != n) throw std::invalid_argument("Derivatives array size mismatch");

      const int dim = static_cast<int>(points[0].size());
      if (dim < 2) throw std::invalid_argument("Points must be at least 2D");

      // Extract x coordinates
      std::vector<double> x(n);
      for (int i = 0; i < n; i++) {
         x[i] = points[i][0];
      }

      // Prepare result vector
      std::vector<double> result(dim);
      result[0] = t;  // First component is parameter value

      // Evaluate each component
      for (int d = 1; d < dim; d++) {
         // Extract component values and derivatives
         std::vector<double> y(n), yp(n);
         for (int i = 0; i < n; i++) {
            y[i] = points[i][d];
            yp[i] = derivatives[i][d];
         }

         // Use existing curv2
         result[d] = curv2(t, n, x, y, yp, sigma);
      }

      return result;
   }



   template<typename PointType>
   static void calculate(bool closed,
      const std::vector<PointType>& points,
      std::vector<PointType>& derivatives,
      double sigma) {

      if (points.size() < 2) throw std::invalid_argument("At least two points required");

      // Initialize derivatives
      derivatives.resize(points.size());

      // Handle each coordinate after x
      const int dim = points[0].size();
      for (int d = 1; d < dim; d++) {
         std::vector<double> x(points.size());
         std::vector<double> y(points.size());
         std::vector<double> yp(points.size());

         for (size_t i = 0; i < points.size(); i++) {
            x[i] = points[i][0];
            y[i] = points[i][d];
         }

         // Only check monotonicity for open curves
         if (!closed) {
            for (size_t i = 1; i < x.size(); i++) {
               if (x[i] <= x[i - 1]) {
                  throw std::runtime_error("Points must be strictly monotonic in x");
               }
            }
            curv1(x, y, yp, sigma);
         }
         else {
            kurvp1(x, y, yp, sigma);
         }

         // Store results
         for (size_t i = 0; i < points.size(); i++) {
            derivatives[i][d] = yp[i];
         }
      }
   }


   template<typename PointType>
   static PointType evaluate_forward(double t,  // t is between 0 and 1
      const std::vector<PointType>& points,
      const std::vector<PointType>& derivatives,
      double sigma) {

      const int n = points.size();
      if (n == 0) throw std::invalid_argument("No points provided");
      if (derivatives.size() != n) throw std::invalid_argument("Derivatives array size mismatch");

      // Extract x coordinates
      std::vector<double> x(n);
      for (int i = 0; i < n; i++) {
         x[i] = points[i][0];
      }

      PointType result;
      result[0] = t;

      // Evaluate each coordinate
      const int dim = points[0].size();
      for (int d = 1; d < dim; d++) {
         std::vector<double> y(n), yp(n);
         for (int i = 0; i < n; i++) {
            y[i] = points[i][d];
            yp[i] = derivatives[i][d];
         }

         result[d] = curv2(t, x.size(), x, y, yp, sigma);
      }

      return result;
   }

   template<typename PointType>
   static PointType evaluate(double t,  // t is between 0 and 1
      const std::vector<PointType>& points,
      const std::vector<PointType>& derivatives,
      double sigma) {

      if (t <= 0.5) {
         // First half - use t*2 to map [0,0.5] to [0,1]
         return evaluate_forward(t * 2.0, points, derivatives, sigma);
      }
      else {
         // Second half - use (1-t)*2 to map [0.5,1] to [1,0]
         return evaluate_forward((1.0 - t) * 2.0, points, derivatives, sigma);
      }
   }





   static void kurvp1(const std::vector<double>& x,
      const std::vector<double>& y,
      std::vector<double>& yp,
      double sigma) {

      const int n = x.size();
      if (n < 2) throw std::invalid_argument("At least two points required");

      // Workspace array
      std::vector<double> temp(2 * n);  // Double size needed for closed curves

      // Calculate initial derivatives and accumulate arclength
      double s = 0.0;
      std::vector<double> dels(n);

      // First point
      double delx1 = x[1] - x[0];
      double dely1 = y[1] - y[0];
      dels[0] = std::sqrt(delx1 * delx1 + dely1 * dely1);
      s = dels[0];
      yp[0] = dely1 / dels[0];

      // Interior points
      for (int i = 1; i < n - 1; i++) {
         double delx2 = x[i + 1] - x[i];
         double dely2 = y[i + 1] - y[i];
         dels[i] = std::sqrt(delx2 * delx2 + dely2 * dely2);
         s += dels[i];
         yp[i] = dely2 / dels[i] - yp[i - 1];
      }

      // Last point connecting back to start
      double delxn = x[0] - x[n - 1];
      double delyn = y[0] - y[n - 1];
      dels[n - 1] = std::sqrt(delxn * delxn + delyn * delyn);
      s += dels[n - 1];
      yp[n - 1] = delyn / dels[n - 1] - yp[n - 2];

      // Denormalize tension factor for closed curve
      double sigmap = std::abs(sigma) * n / s;

      // Forward elimination with periodic boundary
      double dels1 = sigmap * dels[0];
      double exps = std::exp(dels1);
      double sinhs = 0.5 * (exps - 1.0 / exps);
      double sinhin = 1.0 / (dels[0] * sinhs);
      double diag1 = sinhin * (dels1 * 0.5 * (exps + 1.0 / exps) - sinhs);
      double diagin = 1.0 / diag1;
      double spdiag = sinhin * (sinhs - dels1);
      temp[0] = diagin * spdiag;

      // Interior points
      double diag2 = 0.0;
      for (int i = 1; i < n; i++) {
         dels1 = sigmap * dels[i];
         exps = std::exp(dels1);
         sinhs = 0.5 * (exps - 1.0 / exps);
         sinhin = 1.0 / (dels[i] * sinhs);
         diag2 = sinhin * (dels1 * 0.5 * (exps + 1.0 / exps) - sinhs);

         if (i < n - 1) {
            diagin = 1.0 / (diag1 + diag2 - spdiag * temp[i - 1]);
            yp[i] = diagin * (yp[i] - spdiag * yp[i - 1]);
            temp[n + i] = -diagin * temp[n + i - 1] * spdiag;
            if (i == 1) temp[n + 1] = -diagin * spdiag;
            spdiag = sinhin * (sinhs - dels1);
            temp[i] = diagin * spdiag;
            diag1 = diag2;
         }
      }

      // Special handling for last point connecting back to first
      temp[n - 1] = temp[2 * n - 1] - temp[n - 1];
      diagin = 1.0 / (diag1 + diag2 + spdiag * temp[0] + spdiag * temp[n - 1]);
      yp[n - 1] = diagin * (yp[n - 1] - spdiag * yp[0] - spdiag * yp[n - 2]);

      // Back substitution
      for (int i = 0; i < n - 1; i++) {
         yp[i] = yp[i] + temp[i] * yp[n - 1];
      }
   }


};

#endif // TENSION_SPLINE_H

