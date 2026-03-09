#include <cmath>
#include <array>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

#include "LatticeConverter.h"
#include "LRL_Cell.h"

const double M_PI = 4.0 * std::atan(1.0);

//
// ──────────────────────────────────────────────────────────────
//   Distortion calculations
// ──────────────────────────────────────────────────────────────
//

// distortion = (A_actual - A_right) / A_right
double cornerDistortionFromAngles(double theta_uv,
   double theta_vw,
   double theta_wu,
   double h = 1.0)
{
   auto side = [&](double theta) {
      return h * std::sqrt(2.0 - 2.0 * std::cos(theta));
      };

   double a = side(theta_vw);
   double b = side(theta_wu);
   double c = side(theta_uv);

   double A = 0.25 * std::sqrt(
      2 * (a * a * b * b + b * b * c * c + c * c * a * a) -
      (a * a * a * a + b * b * b * b + c * c * c * c)
   );

   double A0 = (std::sqrt(3.0) / 2.0) * h * h;

   return (A - A0) / A0;
}

//
// ──────────────────────────────────────────────────────────────
//   Shape vector from Niggli cell (angles in degrees in LRL_Cell)
// ──────────────────────────────────────────────────────────────
//

std::array<double, 3> ComputeShapeVectorFromNiggliCell(const LRL_Cell& niggli)
{
   // Extract angles in degrees
   const double alpha_deg = niggli[3];
   const double beta_deg = niggli[4];
   const double gamma_deg = niggli[5];

   // Convert to radians
   const double deg2rad = M_PI / 180.0;
   const double alpha = alpha_deg * deg2rad;
   const double beta = beta_deg * deg2rad;
   const double gamma = gamma_deg * deg2rad;

   // The three angles at each corner:
   // Opposite edge a: (alpha, beta, gamma)
   // Opposite edge b: (beta, gamma, alpha)
   // Opposite edge c: (gamma, alpha, beta)

   const double Sa = cornerDistortionFromAngles(alpha, beta, gamma);
   const double Sb = cornerDistortionFromAngles(beta, gamma, alpha);
   const double Sc = cornerDistortionFromAngles(gamma, alpha, beta);

   return { Sa, Sb, Sc };
}

std::array<double, 3> ComputeShapeVector(const std::string& lattice,
   const LRL_Cell& cell)
{
   const LRL_Cell niggli = LatticeConverter::NiggliReduceCell(lattice, cell);
   return ComputeShapeVectorFromNiggliCell(niggli);
}

//
// ──────────────────────────────────────────────────────────────
//   Basic vector type (for earlier face experiments)
// ──────────────────────────────────────────────────────────────
//

struct Vec3 {
   double x, y, z;

   Vec3 operator+(const Vec3& b) const { return { x + b.x, y + b.y, z + b.z }; }
   Vec3 operator-(const Vec3& b) const { return { x - b.x, y - b.y, z - b.z }; }
   Vec3 operator*(double s) const { return { x * s, y * s, z * s }; }
};

inline double dot(const Vec3& a, const Vec3& b) {
   return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline double norm(const Vec3& a) {
   return std::sqrt(dot(a, a));
}

inline double angle_between(const Vec3& u, const Vec3& v) {
   double cu = dot(u, v) / (norm(u) * norm(v));
   if (cu > 1.0) cu = 1.0;
   if (cu < -1.0) cu = -1.0;
   return std::acos(cu);
}

double cornerDistortionFromVectors(const Vec3& u,
   const Vec3& v,
   const Vec3& w)
{
   double tuv = angle_between(u, v);
   double tvw = angle_between(v, w);
   double twu = angle_between(w, u);
   return cornerDistortionFromAngles(tuv, tvw, twu, 1.0);
}

struct FaceSignature {
   std::array<double, 4> d;

   friend std::ostream& operator<<(std::ostream& os, const FaceSignature& fs) {
      os << "[";
      for (size_t i = 0; i < 4; ++i) {
         os << fs.d[i];
         if (i + 1 < 4) os << ", ";
      }
      os << "]";
      return os;
   }
};

FaceSignature faceSignature_ab(const Vec3& a,
   const Vec3& b,
   const Vec3& c)
{
   FaceSignature fs;

   fs.d[0] = cornerDistortionFromVectors(a, b, c);
   fs.d[1] = cornerDistortionFromVectors(a * -1.0, b, c);
   fs.d[2] = cornerDistortionFromVectors(a * -1.0, b * -1.0, c);
   fs.d[3] = cornerDistortionFromVectors(a, b * -1.0, c);

   std::sort(fs.d.begin(), fs.d.end());
   return fs;
}

struct CellSignature {
   FaceSignature ab;
   FaceSignature bc;
   FaceSignature ca;
};

CellSignature fullCellSignature(const Vec3& a,
   const Vec3& b,
   const Vec3& c)
{
   CellSignature cs;
   cs.ab = faceSignature_ab(a, b, c);
   cs.bc = faceSignature_ab(b, c, a);
   cs.ca = faceSignature_ab(c, a, b);
   return cs;
}

std::ostream& operator<<(std::ostream& os, const CellSignature& cs) {
   os << "  Face (a,b): " << cs.ab << "\n";
   os << "  Face (b,c): " << cs.bc << "\n";
   os << "  Face (c,a): " << cs.ca;
   return os;
}

void systematicPerturbationTests(const Vec3& a0,
   const Vec3& b0,
   const Vec3& c0)
{
   std::cout << "\n=== Systematic Perturbation Tests ===\n\n";

   const double eps = 0.01;

   {
      Vec3 a = { a0.x + eps, a0.y, a0.z };
      CellSignature cs = fullCellSignature(a, b0, c0);
      std::cout << "Perturb a.x by +" << eps << "\n" << cs << "\n\n";
   }

   {
      Vec3 b = { b0.x, b0.y + eps, b0.z };
      CellSignature cs = fullCellSignature(a0, b, c0);
      std::cout << "Perturb b.y by +" << eps << "\n" << cs << "\n\n";
   }

   {
      Vec3 c = { c0.x, c0.y, c0.z + eps };
      CellSignature cs = fullCellSignature(a0, b0, c);
      std::cout << "Perturb c.z by +" << eps << "\n" << cs << "\n\n";
   }

   {
      Vec3 a = { a0.x + eps, a0.y - eps, a0.z };
      Vec3 b = { b0.x - eps, b0.y + eps, b0.z };
      Vec3 c = { c0.x, c0.y + eps, c0.z - eps };
      CellSignature cs = fullCellSignature(a, b, c);
      std::cout << "Perturb all vectors\n" << cs << "\n\n";
   }
}

//
// ──────────────────────────────────────────────────────────────
//   Simple angular distortion test driver (unchanged logic)
// ──────────────────────────────────────────────────────────────
//

struct CellABG {
   std::string label;
   double a, b, c;
   double alpha, beta, gamma; // degrees
};

double shapeDistortionFromAngles(double alpha_deg,
   double beta_deg,
   double gamma_deg,
   double h = 1.0)
{
   const double deg2rad = M_PI / 180.0;
   double alpha = alpha_deg * deg2rad;
   double beta = beta_deg * deg2rad;
   double gamma = gamma_deg * deg2rad;

   auto side = [&](double theta) {
      return h * std::sqrt(2.0 - 2.0 * std::cos(theta));
      };

   double a = side(beta);
   double b = side(gamma);
   double c = side(alpha);

   double A = 0.25 * std::sqrt(
      2 * (a * a * b * b + b * b * c * c + c * c * a * a) -
      (a * a * a * a + b * b * b * b + c * c * c * c)
   );

   double A0 = (std::sqrt(3.0) / 2.0) * h * h;

   return (A - A0) / A0;
}

int main()
{
   std::vector<CellABG> cells = {
      {"G_base",  2.000, 4.000, 4.000, 60.000, 79.193, 75.522},
      {"G_1",     1.914, 3.951, 3.933, 58.176, 76.566, 73.300},
      {"G_2",     2.004, 4.009, 3.972, 59.171, 80.112, 75.885},
      {"G_3",     2.009, 3.979, 3.983, 59.840, 78.205, 74.437},
      {"G_4",     1.895, 3.997, 4.031, 61.241, 77.856, 72.861},
      {"G_5",     1.946, 3.996, 3.993, 60.617, 77.099, 73.876},
      {"G_6",     2.011, 4.001, 3.961, 59.501, 77.050, 76.621},
      {"G_7",     2.022, 3.965, 3.984, 59.201, 80.025, 73.820},
      {"G_8",     2.025, 3.960, 3.939, 57.982, 78.521, 75.340},
      {"G_9",     1.883, 3.952, 3.948, 58.116, 76.752, 73.901},
      {"G_10",    2.016, 3.987, 4.064, 60.837, 80.569, 75.457},

      {"C_base", 10.000, 10.000, 10.000, 90.000, 90.000, 90.000},
      {"C_1",    10.030,  9.900, 10.023, 89.717, 90.608, 89.406},
      {"C_2",     9.934, 10.062,  9.921, 90.226, 89.341, 89.955},
      {"C_3",    10.024, 10.024, 10.018, 89.677, 90.042, 90.665},
      {"C_4",    10.032, 10.095,  9.964, 90.098, 89.559, 90.859},
      {"C_5",     9.918, 10.030,  9.938, 89.786, 89.739, 89.831},
      {"C_6",     9.989,  9.950,  9.879, 89.332, 89.365, 90.252},
      {"C_7",    10.030,  9.901, 10.001, 89.412, 90.124, 90.072},
      {"C_8",    10.121,  9.984, 10.024, 90.018, 90.487, 90.231},
      {"C_9",    10.083, 10.015, 10.021, 90.117, 90.679, 89.890},
      {"C_10",    9.892,  9.963,  9.981, 89.703, 89.932, 89.422}
   };

   std::vector<double> D(cells.size());
   for (size_t i = 0; i < cells.size(); ++i) {
      D[i] = shapeDistortionFromAngles(cells[i].alpha,
         cells[i].beta,
         cells[i].gamma);
   }

   std::cout << "Guber-like cell group (shape distortion):\n";
   double D_G_base = D[0];
   for (size_t i = 0; i <= 10; ++i) {
      std::cout << cells[i].label
         << "  D = " << D[i]
         << "  ΔD = " << (D[i] - D_G_base)
         << "\n";
   }
   std::cout << "\n";

   std::cout << "Cubic cell group (shape distortion):\n";
   double D_C_base = D[11];
   for (size_t i = 11; i < cells.size(); ++i) {
      std::cout << cells[i].label
         << "  D = " << D[i]
         << "  ΔD = " << (D[i] - D_C_base)
         << "\n";
   }


   LRL_Cell c1(1, 2, 3, 90, 90, 100);
   LRL_Cell c2(3, 1, 2, 90, 100, 90);

   auto S1 = ComputeShapeVector("P", c1);
   auto S2 = ComputeShapeVector("P", c2);

   std::cout << "Cell 1 shape vector = "
      << S1[0] << " " << S1[1] << " " << S1[2] << "\n";

   std::cout << "Cell 2 shape vector = "
      << S2[0] << " " << S2[1] << " " << S2[2] << "\n";

   return 0;
}
