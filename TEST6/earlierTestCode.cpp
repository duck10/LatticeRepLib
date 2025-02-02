//void examine_near_nodes() {
//   // Test data
//   std::vector<double> x = { 0.0, 1.0, 2.0, 3.0, 4.0 };
//   std::vector<double> y = { 0.0, 1.0, 0.0, 1.0, 0.0 };
//   std::vector<double> yp(x.size());
//   const double sigma = -1.0;
//   const double slp1 = +1.0;
//   const double slpn = -1.0;
//
//   // Calculate spline parameters
//   TensionSpline::curv1(static_cast<int>(x.size()), x, y, slp1, slpn, yp, sigma);
//
//   std::cout << "\nDetailed Node Behavior\n";
//   std::cout << "=====================\n";
//
//   // Test offsets from nodes
//   std::vector<double> offsets = {
//       -0.01, -0.001, -0.0001, -0.00001,
//       0.0,
//       0.00001, 0.0001, 0.001, 0.01
//   };
//
//   // For each node
//   for (size_t i = 0; i < x.size(); i++) {
//      std::cout << "\nAround node x = " << x[i] << " (y = " << y[i] << "):\n";
//      std::cout << "         t          y\n";
//      std::cout << "--------------------\n";
//
//      for (double offset : offsets) {
//         double t = x[i] + offset;
//         double val = TensionSpline::curv2(t, static_cast<int>(x.size()), x, y, yp, sigma);
//         std::cout << std::setw(10) << t << " "
//            << std::setw(12) << val;
//         if (offset < 0) std::cout << " (before)";
//         else if (offset > 0) std::cout << " (after)";
//         else std::cout << " (at node)";
//         std::cout << "\n";
//      }
//   }
//}

//void test_dense_sampling() {
//   std::cout << "\nTesting with Dense Sampling\n";
//   std::cout << "=========================\n";
//
//   // More points, same oscillating pattern
//   std::vector<double> x = { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0 };
//   std::vector<double> y = { 0.0, 0.5, 1.0, 0.5, 0.0, 0.5, 1.0, 0.5, 0.0 };
//   std::vector<double> yp(x.size());
//   const double sigma = -1.0;  // Negative to compute slopes
//
//   std::cout << "\nInput points:\n";
//   for (size_t i = 0; i < x.size(); i++) {
//      std::cout << "Point " << i << ": (" << x[i] << ", " << y[i] << ")\n";
//   }
//
//   // Add debug output for start slope calculation
//   std::cout << "\nStart slope calculation:\n";
//   double delx1 = x[1] - x[0];
//   double delx2 = x[2] - x[1];
//   double delx12 = x[2] - x[0];
//
//   std::cout << "delx1 = " << delx1 << " (x[1] - x[0])\n";
//   std::cout << "delx2 = " << delx2 << " (x[2] - x[1])\n";
//   std::cout << "delx12 = " << delx12 << " (x[2] - x[0])\n";
//
//   double c1 = -(delx12 + delx1) / (delx12 * delx1);
//   double c2 = delx12 / (delx1 * delx2);
//   double c3 = -delx1 / (delx12 * delx2);
//
//   std::cout << "\nCoefficients:\n";
//   std::cout << "c1 = " << c1 << "\n";
//   std::cout << "c2 = " << c2 << "\n";
//   std::cout << "c3 = " << c3 << "\n";
//
//   double expected_slp1 = c1 * y[0] + c2 * y[1] + c3 * y[2];
//   std::cout << "\nExpected start slope:\n";
//   std::cout << "slp1 = " << c1 << " * " << y[0] << " + "
//      << c2 << " * " << y[1] << " + "
//      << c3 << " * " << y[2] << " = " << expected_slp1 << "\n";
//
//   // Calculate spline
//   TensionSpline::curv1(static_cast<int>(x.size()), x, y, 0.0, 0.0, yp, sigma);
//
//   // Print the computed derivatives
//   std::cout << "\nComputed derivatives at nodes:\n";
//   for (size_t i = 0; i < x.size(); i++) {
//      std::cout << "x[" << i << "] = " << x[i] << ": yp = " << yp[i] << "\n";
//   }
//
//   // Test near the start point
//   std::cout << "\nBehavior near start:\n";
//   std::cout << "         t          y\n";
//   std::cout << "--------------------\n";
//   for (double t : {-0.001, 0.0, 0.001, 0.01, 0.1}) {
//      double val = TensionSpline::curv2(t, static_cast<int>(x.size()), x, y, yp, sigma);
//      std::cout << std::setw(10) << t << " " << std::setw(10) << val << "\n";
//   }
//
//   // Calculate numerical slope at start
//   double h = 0.001;
//   double numeric_slope = (TensionSpline::curv2(h, static_cast<int>(x.size()), x, y, yp, sigma) -
//      TensionSpline::curv2(0.0, static_cast<int>(x.size()), x, y, yp, sigma)) / h;
//   std::cout << "\nNumerical slope at start = " << numeric_slope << "\n";
//
//   //for (double t = 0; t <= 4.000001; t+= 0.01) {
//   //   double val = TensionSpline::curv2(t, x.size(), x, y, yp, sigma);
//   //   std::cout << std::setw(10) << t << " " << std::setw(10) << val << "\n";
//   //}
//
//}
//
//void test_2d_curves() {
//   std::cout << "\nTesting 2D Curves\n";
//   std::cout << "================\n";
//
//   // Test Case 1: Simple monotonic x (quarter circle-like)
//   std::vector<std::vector<double>> points1 = {
//       {0.0, 0.0},    // (x,y) coordinates
//       {0.5, 0.7},
//       {1.0, 1.0},
//       {1.5, 0.7},
//       {2.0, 0.0}
//   };
//
//   std::vector<std::vector<double>> derivatives1;
//   const double sigma = -1.0;  // Auto-compute slopes
//
//   std::cout << "\nTest Case 1 - Quarter circle-like curve:\n";
//   std::cout << "Input points:\n";
//   for (const auto& p : points1) {
//      std::cout << "(" << p[0] << ", " << p[1] << ")\n";
//   }
//
//   // Calculate spline
//   TensionSpline::calculate(false, points1, derivatives1, sigma);
//
//   // Test evaluations
//   std::cout << "\nEvaluating at intervals:\n";
//   for (double t = 0.0; t <= 2.0; t += 0.25) {
//      auto pt = TensionSpline::evaluate(t, points1, derivatives1, sigma);
//      std::cout << "t = " << t << ": (" << pt[0] << ", " << pt[1] << ")\n";
//   }
//
//   // Test Case 2: More complex curve (figure-8 like)
//   // Here we'll need to use a parameter other than x
//   std::vector<std::vector<double>> points2 = {
//       {0.0, 0.0},
//       {1.0, 1.0},
//       {0.0, 2.0},
//       {-1.0, 1.0},
//       {0.0, 0.0}  // Back to start for closed curve
//   };
//
//   std::cout << "\nTest Case 2 - Figure-8 curve:\n";
//   std::cout << "Input points:\n";
//   for (const auto& p : points2) {
//      std::cout << "(" << p[0] << ", " << p[1] << ")\n";
//   }
//
//   // First, parameterize by approximate arc length
//   double total_length = 0.0;
//   std::vector<double> params{ 0.0 };  // Start at 0
//   for (size_t i = 1; i < points2.size(); i++) {
//      double dx = points2[i][0] - points2[i - 1][0];
//      double dy = points2[i][1] - points2[i - 1][1];
//      total_length += std::sqrt(dx * dx + dy * dy);
//      params.push_back(total_length);
//   }
//
//   // Normalize parameters to [0,1]
//   for (auto& t : params) {
//      t /= total_length;
//   }
//
//   // Create parameterized points
//   std::vector<std::vector<double>> param_points2(points2.size());
//   for (size_t i = 0; i < points2.size(); i++) {
//      param_points2[i] = { params[i], points2[i][0], points2[i][1] };
//   }
//
//   std::vector<std::vector<double>> derivatives2;
//   std::cout << "\nParameterized points:\n";
//   for (const auto& p : param_points2) {
//      std::cout << "t = " << p[0] << ": (" << p[1] << ", " << p[2] << ")\n";
//   }
//
//   // Calculate spline
//   TensionSpline::calculate(false, param_points2, derivatives2, sigma);
//
//   // Test evaluations
//   std::cout << "\nEvaluating at intervals:\n";
//   for (double t = 0.0; t <= 1.0; t += 0.125) {
//      auto pt = TensionSpline::evaluate(t, param_points2, derivatives2, sigma);
//      std::cout << "t = " << t << ": (" << pt[1] << ", " << pt[2] << ")\n";
//   }
//}
//
//void print_figure8_points() {
//   std::cout << "\nFigure-8 Curve - 100 Points\n";
//   std::cout << "=========================\n";
//
//   // Define figure-8 control points with more points to better define the crossover
//   std::vector<std::vector<double>> points = {
//       {0.0, 0.0},      // Center
//       {0.5, 0.5},      // First quadrant
//       {1.0, 0.0},      // Right point
//       {0.5, -0.5},     // Fourth quadrant
//       {0.0, 0.0},      // Center (crossing point)
//       {-0.5, 0.5},     // Second quadrant
//       {-1.0, 0.0},     // Left point
//       {-0.5, -0.5},    // Third quadrant
//       {0.0, 0.0}       // Back to center
//   };
//
//
//
//   // Parameterize by arc length
//   double total_length = 0.0;
//   std::vector<double> params{ 0.0 };
//   for (size_t i = 1; i < points.size(); i++) {
//      double dx = points[i][0] - points[i - 1][0];
//      double dy = points[i][1] - points[i - 1][1];
//      total_length += std::sqrt(dx * dx + dy * dy);
//      params.push_back(total_length);
//   }
//
//   // Create parameterized points
//   std::vector<std::vector<double>> param_points(points.size());
//   for (size_t i = 0; i < points.size(); i++) {
//      param_points[i] = { params[i] / total_length, points[i][0], points[i][1] };
//   }
//
//   // Calculate spline
//   std::vector<std::vector<double>> derivatives;
//   const double sigma = -1.0;
//   TensionSpline::calculate(false, param_points, derivatives, sigma);
//
//   // Generate 100 points
//   std::cout << "\nt,x,y\n";
//   for (int i = 0; i <= 100; i++) {
//      double t = i / 100.0;
//      auto pt = TensionSpline::evaluate(t, param_points, derivatives, sigma);
//      std::cout << t << "," << pt[1] << "," << pt[2] << "\n";
//   }
//}
//
//void test_parabola() {
//   // Define parabola points y = x^2
//   std::vector<std::vector<double>> points = {
//       {-2.0, 4.0},
//       {-1.5, 2.25},
//       {-1.0, 1.0},
//       {-0.5, 0.25},
//       {0.0, 0.0},
//       {0.5, 0.25},
//       {1.0, 1.0},
//       {1.5, 2.25},
//       {2.0, 4.0}
//   };
//
//   std::cout << "\nParabola Test (y = x^2)\n";
//   std::cout << "Expected slopes = 2x:\n";
//   for (const auto& pt : points) {
//      std::cout << "x = " << pt[0] << ": expected slope = " << 2 * pt[0] << "\n";
//   }
//
//   std::vector<std::vector<double>> derivatives;
//   const double sigma = -1.0;  // Auto-compute slopes
//   TensionSpline::calculate(false, points, derivatives, sigma);
//
//   std::cout << "\nCalculated slopes:\n";
//   for (size_t i = 0; i < points.size(); i++) {
//      std::cout << "x = " << points[i][0] << ": calculated slope = " << derivatives[i][1] << "\n";
//   }
//
//   // Print points near x = 2
//   std::cout << "\nDetailed output near x = 2:\n";
//   for (double x = 1.8; x <= 2.0; x += 0.05) {
//      auto pt = TensionSpline::evaluate(x, points, derivatives, sigma);
//      std::cout << "x = " << x << ": y = " << pt[1] << "\n";
//   }
//
//   //  Points along the spline for the parabola
//   for (double x = -2.0; x <= 2.0; x += 0.04) {
//      auto pt = TensionSpline::evaluate(x, points, derivatives, sigma);
//      std::cout << "x = " << x << " " << pt[1] << "\n";
//   }
//}
//
//void analyze_parabola_fit() {
//   // Define parabola points y = x^2
//   std::vector<std::vector<double>> points = {
//       {-2.0, 4.0},
//       {-1.5, 2.25},
//       {-1.0, 1.0},
//       {-0.5, 0.25},
//       {0.0, 0.0},
//       {0.5, 0.25},
//       {1.0, 1.0},
//       {1.5, 2.25},
//       {2.0, 4.0}
//   };
//
//   // Calculate spline
//   std::vector<std::vector<double>> derivatives;
//   const double sigma = -1.0;  // Auto-compute slopes
//   TensionSpline::calculate(false, points, derivatives, sigma);
//
//   std::cout << "Analyzing parabola fit:\n";
//   std::cout << "x\ty=x^2\tspline\tdiff\tapprox_slope\n";
//
//   double prev_x = -2.04;
//   double prev_y = prev_x * prev_x;  // Exact y
//
//   // Sample points
//   for (double x = -2.0; x <= 2.0; x += 0.04) {
//      auto pt = TensionSpline::evaluate(x, points, derivatives, sigma);
//      double spline_y = pt[1];
//      double exact_y = x * x;
//      double diff = spline_y - exact_y;
//
//      // Approximate slope using central difference
//      double slope = (spline_y - prev_y) / (x - prev_x);
//
//      std::cout << std::fixed << std::setprecision(6)
//         << x << "\t" << exact_y << "\t" << spline_y
//         << "\t" << diff << "\t" << slope << "\n";
//
//      prev_x = x;
//      prev_y = spline_y;
//   }
//}
