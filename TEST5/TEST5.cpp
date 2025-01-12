#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

// Additional validation functions
namespace validate {
   void check_monotonic_parameter(const std::vector<std::vector<double>>& points) {
      for (size_t i = 1; i < points.size(); ++i) {
         if (points[i][0] <= points[i - 1][0]) {
            throw std::invalid_argument(
               "First dimension (parameter) must be strictly increasing. "
               "Violation at index " + std::to_string(i)
            );
         }
      }
   }

   void check_finite_values(const std::vector<std::vector<double>>& points, const std::string& name) {
      for (size_t i = 0; i < points.size(); ++i) {
         for (size_t j = 0; j < points[i].size(); ++j) {
            if (!std::isfinite(points[i][j])) {
               throw std::invalid_argument(
                  name + " contains non-finite value at point " +
                  std::to_string(i) + ", dimension " + std::to_string(j)
               );
            }
         }
      }
   }

   void check_arrays_match(const std::vector<std::vector<double>>& arr1,
      const std::vector<std::vector<double>>& arr2,
      const std::string& name1,
      const std::string& name2) {
      if (arr1.size() != arr2.size()) {
         throw std::invalid_argument(
            name1 + " and " + name2 + " have different numbers of points: " +
            std::to_string(arr1.size()) + " vs " + std::to_string(arr2.size())
         );
      }
      if (!arr1.empty() && arr1[0].size() != arr2[0].size()) {
         throw std::invalid_argument(
            name1 + " and " + name2 + " have different dimensions: " +
            std::to_string(arr1[0].size()) + " vs " + std::to_string(arr2[0].size())
         );
      }
   }

   void check_slope_dimensions(const std::vector<double>& slope,
      size_t dim,
      const std::string& name) {
      if (slope.size() != dim) {
         throw std::invalid_argument(
            name + " dimension (" + std::to_string(slope.size()) +
            ") doesn't match point dimension (" + std::to_string(dim) + ")"
         );
      }
      for (size_t i = 0; i < slope.size(); ++i) {
         if (!std::isfinite(slope[i])) {
            throw std::invalid_argument(
               name + " contains non-finite value at dimension " +
               std::to_string(i)
            );
         }
      }
   }
}

// Enhanced N-dimensional CURV1 with additional validation
void curv1_nd(int n,
   const std::vector<std::vector<double>>& points,
   const std::vector<double>& slp1,
   const std::vector<double>& slpn,
   std::vector<std::vector<double>>& yp,
   std::vector<double>& temp,
   double sigma) {
   // Basic input validation
   if (n < 2) {
      throw std::invalid_argument("At least two points required");
   }
   if (points.empty() || points[0].empty()) {
      throw std::invalid_argument("Empty points array or dimension");
   }
   if (points.size() != static_cast<size_t>(n)) {
      throw std::invalid_argument("Number of points doesn't match n");
   }

   // Validate consistent dimensions across all points
   const size_t dim = points[0].size();
   for (size_t i = 1; i < points.size(); ++i) {
      if (points[i].size() != dim) {
         throw std::invalid_argument(
            "Inconsistent dimensions: point " + std::to_string(i) +
            " has dimension " + std::to_string(points[i].size()) +
            ", expected " + std::to_string(dim)
         );
      }
   }

   // Validate slope dimensions if sigma >= 0
   if (sigma >= 0.0) {
      if (slp1.size() != dim) {
         throw std::invalid_argument(
            "Initial slope dimension (" + std::to_string(slp1.size()) +
            ") doesn't match point dimension (" + std::to_string(dim) + ")"
         );
      }
      if (slpn.size() != dim) {
         throw std::invalid_argument(
            "Final slope dimension (" + std::to_string(slpn.size()) +
            ") doesn't match point dimension (" + std::to_string(dim) + ")"
         );
      }
   }

   // Initialize output arrays
   yp.resize(n, std::vector<double>(dim));
   temp.resize(n);

   // Handle n=2 case with auto-calculated slopes
   if (n == 2 && sigma < 0.0) {
      // Use straight line interpolation
      for (size_t d = 0; d < dim; ++d) {
         yp[0][d] = 0.0;
         yp[1][d] = 0.0;
      }
      return;
   }

   // Extract x coordinates (parameter values)
   std::vector<double> x(n);
   for (int i = 0; i < n; ++i) {
      x[i] = points[i][0];
      // Verify strictly increasing
      if (i > 0 && x[i] <= x[i - 1]) {
         throw std::invalid_argument(
            "First dimension (parameter) must be strictly increasing. "
            "Violation at index " + std::to_string(i)
         );
      }
   }

   // Process each dimension separately
   for (size_t d = 0; d < dim; ++d) {
      // Extract current dimension's values
      std::vector<double> y(n);
      for (int i = 0; i < n; ++i) {
         y[i] = points[i][d];
      }

      const int nm1 = n - 1;
      double delx1 = x[1] - x[0];
      double dx1 = (y[1] - y[0]) / delx1;

      // Determine slopes if necessary
      double slpp1, slppn;
      if (sigma < 0.0) {
         if (n == 2) {
            // This case was handled earlier
            continue;
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
         slpp1 = slp1[d];
         slppn = slpn[d];
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
      yp[0][d] = diagin * (dx1 - slpp1);
      double spdiag = sinhin * (sinhs - dels);
      temp[0] = diagin * spdiag;

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
         yp[i][d] = diagin * (dx2 - dx1 - spdiag * yp[i - 1][d]);
         spdiag = sinhin * (sinhs - dels);
         temp[i] = diagin * spdiag;
         dx1 = dx2;
         diag1 = diag2;
      }

      // Final yp calculation
      diagin = 1.0 / (diag1 - spdiag * temp[nm1 - 1]);
      yp[n - 1][d] = diagin * (slppn - dx1 - spdiag * yp[nm1 - 1][d]);

      // Back substitution
      for (int i = nm1 - 1; i >= 0; --i) {
         yp[i][d] = yp[i][d] - temp[i] * yp[i + 1][d];
      }
   }
}
// Enhanced N-dimensional CURV2 with additional validation
// Enhanced N-dimensional CURV2 with complete return paths
std::vector<double> curv2_nd(double t,
   int n,
   const std::vector<std::vector<double>>& points,
   const std::vector<std::vector<double>>& yp,
   double sigma,
   int& it) {
   // Enhanced input validation
   if (!std::isfinite(t)) {
      throw std::invalid_argument("Parameter t must be finite");
   }
   if (n < 2) {
      throw std::invalid_argument("At least two points required");
   }
   if (points.empty() || points[0].empty()) {
      throw std::invalid_argument("Empty points array or dimension");
   }
   if (points.size() != static_cast<size_t>(n)) {
      throw std::invalid_argument("Number of points doesn't match n");
   }

   validate::check_arrays_match(points, yp, "points", "yp");
   validate::check_finite_values(points, "points");
   validate::check_finite_values(yp, "yp");

   const size_t dim = points[0].size();

   // Handle out-of-range t values
   if (t <= points[0][0]) return points[0];
   if (t >= points[n - 1][0]) return points[n - 1];

   // Start search where previously terminated if it != 1
   int i1 = (it == 1) ? 1 : it;

   // Search for interval
   int i;
   for (i = i1; i < n; ++i) {
      if (points[i][0] >= t) break;
   }

   // Check if interval is correct, if not restart search
   if (points[i - 1][0] > t || t > points[i][0]) {
      i1 = 1;
      for (i = i1; i < n; ++i) {
         if (points[i][0] >= t) break;
      }
   }

   // Store interval for next call
   it = i;

   // Set up and perform interpolation
   double del1 = t - points[i - 1][0];
   double del2 = points[i][0] - t;
   double dels = points[i][0] - points[i - 1][0];

   // Denormalize sigma
   double sigmap = std::abs(sigma) * static_cast<double>(n - 1) / (points[n - 1][0] - points[0][0]);

   double exps1 = std::exp(sigmap * del1);
   double sinhd1 = 0.5 * (exps1 - 1.0 / exps1);
   double exps = std::exp(sigmap * del2);
   double sinhd2 = 0.5 * (exps - 1.0 / exps);
   exps = exps1 * exps;
   double sinhs = 0.5 * (exps - 1.0 / exps);

   // Interpolate each dimension
   std::vector<double> result(dim);
   for (size_t d = 0; d < dim; ++d) {
      result[d] = (yp[i][d] * sinhd1 + yp[i - 1][d] * sinhd2) / sinhs +
         ((points[i][d] - yp[i][d]) * del1 +
            (points[i - 1][d] - yp[i - 1][d]) * del2) / dels;
   }

   return result;
}


int main() {
   try {
      std::cout << "\nExtended Tests for N-dimensional CURV implementation\n";

      // Test 1: Linear interpolation test
      std::cout << "\nTest 1: Linear data (should recover straight line)\n";
      {
         int n = 5;
         std::vector<std::vector<double>> points = {
             {0.0, 0.0},
             {1.0, 1.0},
             {2.0, 2.0},
             {3.0, 3.0},
             {4.0, 4.0}
         };
         const size_t dim = points[0].size();
         std::vector<double> slp1 = { 1.0, 1.0 };
         std::vector<double> slpn = { 1.0, 1.0 };
         std::vector<std::vector<double>> yp(n, std::vector<double>(dim));
         std::vector<double> temp(n);
         double sigma = 1.0;

         curv1_nd(n, points, slp1, slpn, yp, temp, sigma);

         std::cout << "Interpolated points (should be linear):\n";
         int it = 1;
         for (double t = 0.0; t <= 4.0; t += 1.0) {
            auto result = curv2_nd(t, n, points, yp, sigma, it);
            std::cout << "t = " << t << ": (" << result[0] << ", " << result[1] << ")\n";
            // Check linearity
            if (std::abs(result[1] - result[0]) > 1e-10) {
               std::cout << "Warning: Non-linear result detected\n";
            }
         }
      }

      // Test 2: High tension test
      std::cout << "\nTest 2: High tension (should approach linear interpolation)\n";
      {
         std::vector<std::vector<double>> points = {
             {0.0, 0.0},
             {1.0, 2.0},
             {2.0, 1.0},
             {3.0, 3.0}
         };
         const size_t dim = points[0].size();
         std::vector<double> slp1 = { 1.0, 2.0 };
         std::vector<double> slpn = { 1.0, 1.0 };
         std::vector<std::vector<double>> yp(points.size(), std::vector<double>(dim));
         std::vector<double> temp(points.size());
         double sigma = 50.0;  // High tension

         curv1_nd(points.size(), points, slp1, slpn, yp, temp, sigma);

         std::cout << "Points with high tension:\n";
         int it = 1;
         for (double t = 0.0; t <= 3.0; t += 0.5) {
            auto result = curv2_nd(t, points.size(), points, yp, sigma, it);
            std::cout << "t = " << std::setw(3) << t << ": ("
               << std::setw(6) << result[0] << ", "
               << std::setw(6) << result[1] << ")\n";
         }
      }

      // Test 3: Low tension test
      std::cout << "\nTest 3: Low tension (should approach cubic spline)\n";
      {
         std::vector<std::vector<double>> points = {
             {0.0, 0.0},
             {1.0, 2.0},
             {2.0, 1.0},
             {3.0, 3.0}
         };
         const size_t dim = points[0].size();
         std::vector<double> slp1 = { 1.0, 2.0 };
         std::vector<double> slpn = { 1.0, 1.0 };
         std::vector<std::vector<double>> yp(points.size(), std::vector<double>(dim));
         std::vector<double> temp(points.size());
         double sigma = 0.001;  // Very low tension

         curv1_nd(points.size(), points, slp1, slpn, yp, temp, sigma);

         std::cout << "Points with low tension:\n";
         int it = 1;
         for (double t = 0.0; t <= 3.0; t += 0.5) {
            auto result = curv2_nd(t, points.size(), points, yp, sigma, it);
            std::cout << "t = " << std::setw(3) << t << ": ("
               << std::setw(6) << result[0] << ", "
               << std::setw(6) << result[1] << ")\n";
         }
      }

      // Test 4: Endpoint behavior test
      std::cout << "\nTest 4: Endpoint behavior with specified slopes\n";
      {
         std::vector<std::vector<double>> points = {
             {0.0, 0.0},
             {1.0, 1.0},
             {2.0, 0.0}
         };
         const size_t dim = points[0].size();
         std::vector<double> slp1 = { 1.0, 2.0 };  // Steep upward slope at start
         std::vector<double> slpn = { 1.0, -2.0 }; // Steep downward slope at end
         std::vector<std::vector<double>> yp(points.size(), std::vector<double>(dim));
         std::vector<double> temp(points.size());
         double sigma = 1.0;

         curv1_nd(points.size(), points, slp1, slpn, yp, temp, sigma);

         std::cout << "Points with specified endpoint slopes:\n";
         int it = 1;
         for (double t = 0.0; t <= 2.0; t += 0.25) {
            auto result = curv2_nd(t, points.size(), points, yp, sigma, it);
            std::cout << "t = " << std::setw(4) << t << ": ("
               << std::setw(6) << result[0] << ", "
               << std::setw(6) << result[1] << ")\n";
         }
      }

      // Test 5: 4D test
      std::cout << "\nTest 5: 4D curve\n";
      {
         std::vector<std::vector<double>> points = {
             {0.0, 0.0, 0.0, 0.0},
             {1.0, 1.0, 0.0, 1.0},
             {2.0, 0.0, 1.0, 0.0},
             {3.0, -1.0, 0.0, 1.0}
         };
         const size_t dim = points[0].size();
         std::vector<double> slp1(dim, 1.0);
         std::vector<double> slpn(dim, -1.0);
         std::vector<std::vector<double>> yp(points.size(), std::vector<double>(dim));
         std::vector<double> temp(points.size());
         double sigma = 1.0;

         curv1_nd(points.size(), points, slp1, slpn, yp, temp, sigma);

         std::cout << "4D curve points:\n";
         int it = 1;
         for (double t = 0.0; t <= 3.0; t += 0.5) {
            auto result = curv2_nd(t, points.size(), points, yp, sigma, it);
            std::cout << "t = " << std::setw(3) << t << ": ("
               << std::setw(7) << result[0] << ", "
               << std::setw(7) << result[1] << ", "
               << std::setw(7) << result[2] << ", "
               << std::setw(7) << result[3] << ")\n";
         }
      }

   }
   catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
   }

   return 0;
}


