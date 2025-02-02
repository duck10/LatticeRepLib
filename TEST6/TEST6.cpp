#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include "TensionSpline.h"



void test_S6_spline() {
   std::cout << "\nTesting TensionSpline with S6\n";
   std::cout << "============================\n";

   // Create points and their x values
   std::vector<double> xvals = { 0.0, 0.2, 0.4, 0.6, 0.8, 1.0 };  // parameter values

   // Create the S6 points (now just the y values)
   std::vector<std::vector<double>> points = {
      {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {0.2, 0.1, 0.1, 0.1, 0.1, 0.1},
      {0.4, 0.3, 0.2, 0.2, 0.2, 0.2},
      {0.6, 0.6, 0.4, 0.3, 0.3, 0.3},
      {0.8, 1.0, 0.7, 0.4, 0.4, 0.4},
      {1.0, 1.5, 1.0, 0.5, 0.5, 0.5}
   };

   // Calculate spline
   std::vector<std::vector<double>> derivatives;
   const double sigma = -1.0;  // Auto-compute slopes
   TensionSpline::calculate(false, xvals, points, derivatives, sigma);

   // Evaluate at some points
   std::cout << "\nSpline values:\n";
   std::cout << "t\tS6 coordinates\n";
   std::cout << "------------------------\n";

   for (double t = 0.0; t <= 1.0; t += 0.1) {
      std::vector<double> pt = TensionSpline::evaluate(t, xvals, points, derivatives, sigma);
      S6 s6pt(pt);
      std::cout << t << "\t";
      std::cout << "     " << s6pt << std::endl;
      //for (const auto& coord : pt) {
      //   std::cout << coord << " ";
      //}
   }
}

void test_closed_S6_spline() {
   // Define x values (parameter values)
   std::vector<double> xvals = { 0.0, 0.25, 0.5, 0.75, 1.0 };

   // Define points (y values only)
   std::vector<std::vector<double>> points = {
      {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {0.2, 0.5, 0.2, 0.1, 0.1, 0.1},
      {0.5, 0.7, 0.5, 0.2, 0.2, 0.2},
      {0.8, 0.5, 0.8, 0.3, 0.3, 0.3},
      {1.0, 0.0, 1.0, 0.4, 0.4, 0.4}
   };

   std::vector<std::vector<double>> derivatives;
   const double sigma = -1.0;
   TensionSpline::calculate(true, xvals, points, derivatives, sigma);

   std::cout << "\nSpline values around the complete loop:\n";
   std::cout << "t\tS6 coordinates\n";
   std::cout << "------------------------\n";

   for (double t = 0.0; t <= 1.0; t += 0.1) {
      std::vector<double> pt = TensionSpline::evaluate(t, xvals, points, derivatives, sigma);
      std::cout << std::fixed << std::setprecision(6)
         << t << "\t";
      // Print all six coordinates from the evaluated point
      for (int i = 0; i < 6; ++i) {     // explicitly print all 6 coordinates
         std::cout << pt[i] << " ";
      }
      std::cout << "\n";
   }
}

int main() {
   //test_closed_S6_spline();
   //exit(0);
   //test_closed_S6_spline();
   //exit(0);
   test_S6_spline();
   exit(0);
   //analyze_parabola_fit();
   //exit(0);
   //test_parabola();
   //exit(0);
   //print_figure8_points();
   //exit(0);
   //test_2d_curves();
   //exit(0);

   //test_dense_sampling();
   std::cout << std::fixed << std::setprecision(6);
   //examine_near_nodes();
   return 0;
}
