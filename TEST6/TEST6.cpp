//
//#include <algorithm>
//#include <cmath>
//#include <iomanip>
//#include <iostream>
//#include <sstream>
//#include <string>
//#include <vector>
//
//#include "B4.h"
//#include "G6.h"
//#include "LRL_Cell_Degrees.h"
//#include "TransformationMatrices.h"
//#include "InputHandler.h"
//#include "ProgramSetup.h"
//#include "Test6Controls.h"
//#include "LRL_Vector3.h"
//
//bool EigenSolver3x3(const Matrix_3x3& A, Vector_3& eigenvalues, Matrix_3x3& eigenvectors, double tol = 1e-10, int maxIter = 50) {
//   Matrix_3x3 D = A; // Working copy
//   eigenvectors = Matrix_3x3::Identity();
//
//   for (int iter = 0; iter < maxIter; ++iter) {
//      // Find largest off-diagonal element
//      int p = 0, q = 1;
//      double maxVal = fabs(D(0, 1));
//      for (int i = 0; i < 3; ++i) {
//         for (int j = i + 1; j < 3; ++j) {
//            if (fabs(D(i, j)) > maxVal) {
//               maxVal = fabs(D(i, j));
//               p = i; q = j;
//            }
//         }
//      }
//      if (maxVal < tol) break;
//
//      double theta = 0.5 * atan2(2 * D(p, q), D(q, q) - D(p, p));
//      double c = cos(theta), s = sin(theta);
//
//      // Jacobi rotation
//      Matrix_3x3 J = Matrix_3x3::Identity();
//      J(p, p) = c; J(q, q) = c;
//      J(p, q) = s; J(q, p) = -s;
//
//      D = J.transpose() * D * J;
//      eigenvectors = eigenvectors * J;
//   }
//
//   eigenvalues = { D(0,0), D(1,1), D(2,2) };
//   return true;
//}
//
//struct BravaisSignature {
//   std::string name;
//   Vector_3 target; // Normalized eigenvalue pattern
//   double tolerance;
//};
//
//Vector_3 NormalizeEigenvalues(const Vector_3& lambda) {
//   double sum = lambda[0] + lambda[1] + lambda[2];
//   return { lambda[0] / sum, lambda[1] / sum, lambda[2] / sum };
//}
//
//bool MatchesBravais(const Vector_3& normEig, const BravaisSignature& sig) {
//   Vector_3 diff = normEig - sig.target;
//   return diff.norm() < sig.tolerance;
//}
//
//std::string ClassifyBravais(const Matrix_3x3& metric) {
//   Vector_3 lambda;
//   Matrix_3x3 eigVecs;
//   if (!EigenSolver3x3(metric, lambda, eigVecs)) return "Solver failed";
//
//   Vector_3 normEig = NormalizeEigenvalues(lambda);
//
//   for (const auto& sig : bravaisSignatures) {
//      if (MatchesBravais(normEig, sig)) return sig.name;
//   }
//   return "Unclassified";
//}
//
//std::vector<BravaisSignature> bravaisSignatures = {
//    { "Cubic", {1.0 / 3, 1.0 / 3, 1.0 / 3}, 0.01 },
//    { "Tetragonal", {0.4, 0.4, 0.2}, 0.02 },
//    { "Orthorhombic", {0.5, 0.3, 0.2}, 0.05 },
//    { "Hexagonal", {0.45, 0.45, 0.1}, 0.02 },
//    { "Rhombohedral", {1.0 / 3, 1.0 / 3, 1.0 / 3}, 0.01 },
//    { "Monoclinic", {0.6, 0.3, 0.1}, 0.05 },
//    { "Triclinic", {0.7, 0.2, 0.1}, 0.1 }
//};
int main () {}