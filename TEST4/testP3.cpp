//#include <cmath>
//#include <iomanip>
//#include <iostream>
//#include <random>
//#include <vector>
//
//#include "CmdPathControls.h"
//#include "B4.h"
//#include "G6.h"
//#include "P3.h"
//#include "P3_Reduce.h"
//#include "LatticeConverter.h"
//#include "LRL_ReadLatticeData.h"
//#include "Niggli.h"
//#include "Polar.h"
//#include "ProgramSetup.h"
//#include "Vector_2.h"
//
//// Define M_PI if not already defined (Visual Studio compatibility)
//#ifndef M_PI
//#define M_PI 3.14159265358979323846
//#endif
//
//Vector_2 MultiplyP3MatrixByVector2(const Matrix_3x3& M, const Vector_2& v) {
//   const Vector_3 v3(v[0], v[1], 0.0);           // promote to 3D
//   const Vector_3 result3 = M * v3;              // use existing operator
//   return Vector_2(result3[0], result3[1]);      // project back to 2D
//}
//
//
//static void TestCanonicalProjectedTriplet() {
//   using Vec2 = std::pair<double, double>;
//   const Vec2 v1 = { 1.0, 2.0 };
//   const Vec2 v2 = { 3.0, 4.0 };
//   const Vec2 v3 = { 5.0, 6.0 };
//
//   const std::array<int, 2> signs = { +1, -1 };
//   int failures = 0;
//
//   for (int s1 : signs) {
//      for (int s2 : signs) {
//         for (int s3 : signs) {
//            P3 input;
//            input[0] = v1;
//            input[1] = { v2.first, s1 * v2.second };
//            input[2] = { v3.first, s2 * v3.second };
//
//            P3 result = P3_Reduce::CanonicalPresentation(input);
//
//            std::cout << "Test: signs = (" << s1 << ", " << s2 << ", " << s3 << ")\n";
//            std::cout << "Result: " << result << "\n";
//
//            for (std::size_t i = 0; i < 3; ++i) {
//               const double y = result[i].second;
//               if (y < 0.0) {
//                  ++failures;
//                  std::cout << "❌ Failure: y[" << i << "] = " << y << " is negative\n";
//               }
//            }
//
//            std::cout << std::endl;
//         }
//      }
//   }
//
//   std::cout << "Test complete. Failures = " << failures << "\n";
//}
//
//
//static bool IsFullyUpperHalfPlane(const P3& triplet) {
//   for (const auto& [x, y] : triplet) {
//      if (y < 0.0) return false;
//   }
//   return true;
//}
//
//
//
//std::vector<S6> CreatePathBetweenTwoPoints(const int npts, const S6& delta, const S6& s1, const S6& s2) {
//   std::vector<S6> out(1, s1);
//   for (int i = 0; i < npts - 1; ++i) {
//      const S6 diff = double(i + 1) * delta;
//      const S6 temp = s1 + diff;
//      out.push_back(temp);
//   }
//   return out;
//}
//
//#include <map>
//
//#include <map>
//
//void CompareP3AndNiggli() {
//   double maxdiffMethods = -DBL_MAX;
//   double maxdiffReduction = -DBL_MAX;
//   const size_t count = 1000000;
//
//   const double binSize = 2.0;
//   const size_t numBins = 10;
//
//   size_t zeroBin = 0;
//   std::vector<size_t> histogram(numBins, 0);
//
//   for (size_t i = 0; i < count; ++i) {
//      const LRL_Cell rancell = LRL_Cell(Polar::rand());
//      const G6 rang6(rancell);
//      const P3 p(rancell);
//      const P3 pred = P3_Reduce::Reduce(p);
//      G6 gred;
//      Niggli::ReduceWithoutMatrices(rang6, gred, 1.0E-6);
//      const P3 p3OfNiggliReduce(gred);
//      const double diff = (pred - p3OfNiggliReduce).norm();
//      std::cout << diff << std::endl;
//   }
//   exit(0);
//}
//
//void RandomReduceComparison() {
//   double maxdiffMethods = -DBL_MAX;
//   double maxdiffReduction = -DBL_MAX;
//   const size_t count = 1000000;
//
//   const double binSize = 2.0;
//   const size_t numBins = 10;
//
//   size_t zeroBin = 0;
//   std::vector<size_t> histogram(numBins, 0);
//
//   for (size_t i = 0; i < count; ++i) {
//      const LRL_Cell rancell = LRL_Cell(Polar::rand());
//      const P3 p(rancell);
//
//      const double diffMethods = (P3_Reduce::Reduce(p) - P3_Reduce::ReduceScalarCost(p)).norm();
//      const double diffReduction = (p - P3_Reduce::ReduceScalarCost(p)).norm();
//
//      maxdiffMethods = std::max(maxdiffMethods, diffMethods);
//      maxdiffReduction = std::max(maxdiffReduction, diffReduction);
//
//      if (diffReduction == 0.0) {
//         ++zeroBin;
//      }
//      else {
//         const size_t binIndex = std::min(static_cast<size_t>(diffReduction / binSize), numBins - 1);
//         ++histogram[binIndex];
//      }
//   }
//
//   std::cout << "\nHistogram of P3 input vs reduced form:\n";
//   std::cout << "[    =0.0]: " << zeroBin << "\n";
//   for (size_t i = 0; i < numBins; ++i) {
//      const double binStart = binSize * i;
//      const double binEnd = binStart + binSize;
//      std::cout << "[" << std::setw(6) << binStart << ", " << std::setw(6) << binEnd << "): "
//         << histogram[i] << "\n";
//   }
//
//   std::cout << "\nFor " << count
//      << " samples, the maximum difference between the two reduction methods is " << maxdiffMethods << "\n";
//   std::cout << "For " << count
//      << " samples, the maximum difference between input and reduced form is " << maxdiffReduction << "\n";
//}
//
//void writeReduced() {
//   const int count = 10;
//   std::vector<LRL_Cell> vcells;
//   for (size_t i = 0; i < count; ++i) {
//      const LRL_Cell rancell = LRL_Cell(Polar::rand());
//      vcells.emplace_back(rancell);
//   }
//
//
//   for (size_t i = 0; i < count; ++i) {
//      std::cout << "P  " << LRL_Cell_Degrees(vcells[i]) << std::endl;
//   }
//
//   std::cout << std::endl << " here are the P3 reduced vectors " << std::endl << std::endl;
//   for (size_t i = 0; i < count; ++i) {
//      const P3 pred = P3_Reduce::ReduceScalarCost(P3(vcells[i]));
//      std::cout << "P " << LRL_Cell_Degrees(LRL_Cell(pred)) << std::endl;
//   }
//   exit(0);
//}
//
//
//typedef std::pair<double, double> Vec2;
//
//double Dot(const Vec2& a, const Vec2& b) {
//   return a.first * b.first + a.second * b.second;
//}
//
//double NormSquared(const Vec2& v) {
//   return v.first * v.first + v.second * v.second;
//}
//
//Vec2 SubtractProjection(const Vec2& vi, const Vec2& vj) {
//   double lambda = Dot(vi, vj) / NormSquared(vj);
//   return { vi.first - lambda * vj.first, vi.second - lambda * vj.second };
//}
//
//void TestProjectionSubtraction() {
//   const Vec2 vi = { 4.0, 3.0 };  // arbitrary input
//   const Vec2 vj = { 1.0, -1.0 }; // projection direction
//
//   const double lambda = Dot(vi, vj) / NormSquared(vj);
//   const Vec2 viNew = SubtractProjection(vi, vj);
//   const double dotCheck = Dot(viNew, vj);
//
//   std::cout << "Original vi · vj = " << Dot(vi, vj) << "\n";
//   std::cout << "Lambda = " << lambda << "\n";
//   std::cout << "viNew = (" << viNew.first << ", " << viNew.second << ")\n";
//   std::cout << "viNew · vj = " << dotCheck << "\n";
//
//   if (std::abs(dotCheck) < 1e-10)
//      std::cout << "✅ Projection subtraction successful — orthogonality achieved\n";
//   else
//      std::cout << "❌ Projection subtraction failed — dot = " << dotCheck << "\n";
//}
//
//void TestP3RoundTripConversion() {
//   constexpr size_t trials = 5;
//   constexpr double epsilon = 1e-10;
//
//   for (size_t i = 0; i < trials; ++i) {
//      const LRL_Cell cellOriginal = Polar::rand();
//      const P3 p3Original(cellOriginal);
//
//      const LRL_Cell cellBack(p3Original);
//      const P3 p3Recovered(cellBack);
//
//      const double diff = (p3Original - p3Recovered).norm();
//
//      std::cout << "Trial " << i + 1 << "\n";
//      std::cout << "Original P3:   " << p3Original << "\n";
//      std::cout << "Recovered P3:  " << p3Recovered << "\n";
//      std::cout << "Δ(norm) = " << diff << "\n";
//
//      if (diff < epsilon)
//         std::cout << "✅ Round-trip successful — conversion stable\n\n";
//      else
//         std::cout << "❌ Round-trip drift detected — Δ exceeds tolerance\n\n";
//   }
//}
//
//
//double UniformRand() {
//   static std::mt19937 rng(std::random_device{}());
//   static std::uniform_real_distribution<double> dist(0.0, 1.0);
//   return dist(rng);
//}
//
//void AnglePerturbationVolumeTest() {
//   constexpr size_t trials = 10000;
//   constexpr double deltaAngleDeg = 1e-4;  // Small angular perturbation
//   constexpr double degToRad = M_PI / 180.0;
//
//   for (size_t i = 0; i < trials; ++i) {
//      // Randomly select edge lengths
//      const double a = 3.0 + 2.0 * UniformRand();  // 3–5 Å
//      const double b = 3.0 + 2.0 * UniformRand();
//      const double c = 3.0 + 2.0 * UniformRand();
//
//      // Choose angle class: tiny or near 90°
//      const bool useTinyAngles = (UniformRand() < 0.5);
//
//      const double alpha = useTinyAngles ? 0.01 + 0.99 * UniformRand() : 89.99 - 0.99 * UniformRand();
//      const double beta = useTinyAngles ? 0.01 + 0.99 * UniformRand() : 89.99 - 0.99 * UniformRand();
//      const double gamma = useTinyAngles ? 0.01 + 0.99 * UniformRand() : 89.99 - 0.99 * UniformRand();
//
//      const LRL_Cell cellOriginal(a, b, c, alpha, beta, gamma);
//      const double originalVolume = cellOriginal.Volume();
//
//      // Perturb one angle slightly
//      const LRL_Cell cellPerturbed(a, b, c, alpha + deltaAngleDeg, beta, gamma);
//      const double perturbedVolume = cellPerturbed.Volume();
//
//      const double volChange = std::abs(perturbedVolume - originalVolume);
//
//      std::cout << "Angle class: " << (useTinyAngles ? "tiny" : "near 90")
//         << ", α = " << alpha << "° → " << alpha + deltaAngleDeg << "°"
//         << ", ΔVol = " << volChange << "\n";
//   }
//}
//
//void TestProjectionVolumeStability() {
//   constexpr size_t trials = 1000;
//   constexpr double volumeTolerance = 1e-5;
//
//   for (size_t t = 0; t < trials; ++t) {
//      const P3 original = P3(Polar::rand());
//      if (!LRL_Cell(original).CheckValid()) continue;
//
//      const size_t i = 0, j = 1; // test fixed pair
//      const double lambda = P3_Reduce::Scalar::Projection(original[i], original[j]);
//
//      P3 candidate = original;
//      candidate[i].first -= lambda * candidate[j].first;
//      candidate[i].second -= lambda * candidate[j].second;
//
//      candidate = P3_Reduce::StandardPresentation(candidate);
//
//      const double volOriginal = LRL_Cell(original).Volume();
//      const double volCandidate = LRL_Cell(candidate).Volume();
//      const double deltaVol = std::abs(volCandidate - volOriginal);
//
//      if (deltaVol > volumeTolerance) {
//         std::cout << "ΔVol = " << deltaVol
//            << ", λ = " << lambda
//            << ", Original P3: " << original
//            << ", Candidate P3: " << candidate << "\n";
//      }
//   }
//}
//
//void TestGruberMatrixOnP3() {
//   const G6 g6Original{ 4., 16., 16., 16., 3., 4. }; // Niggli reduced reference
//
//   // Construct B4 from G6
//   const B4 b4Original(g6Original);
//
//   // Choose a Gruber transformation matrix (e.g. R6_Plus_3D)
//   const Matrix_3x3 gruberMatrix(1.0, 0.0, 0.0,
//      0.0, 1.0, 0.0,
//      -1.0, 0.0, 1.0);
//
//   // Transform the first three basis vectors a, b, c
//   Vector_3 transformed[4];
//   for (size_t i = 0; i < 3; ++i)
//      transformed[i] = gruberMatrix * b4Original[i];
//
//   // Compute d = -(a + b + c) for closure
//   transformed[3] = -(transformed[0] + transformed[1] + transformed[2]);
//
//   const B4 b4Transformed(transformed[0], transformed[1], transformed[2], transformed[3]);
//
//   // Convert both B4 forms to LRL_Cell, then to P3
//   const LRL_Cell cellOriginal(b4Original);
//   const LRL_Cell cellTransformed(b4Transformed);
//
//   const P3 p3Original(cellOriginal);
//   const P3 p3Transformed(cellTransformed);
//
//   // Validate geometry
//   const double volOriginal = cellOriginal.Volume();
//   const double volTransformed = cellTransformed.Volume();
//   const double deltaVol = std::abs(volTransformed - volOriginal);
//
//   std::cout << "Original Volume:   " << volOriginal << "\n";
//   std::cout << "Transformed Volume:" << volTransformed << "\n";
//   std::cout << "ΔVol = " << deltaVol << "\n";
//
//   std::cout << "Original P3:   " << p3Original << "\n";
//   std::cout << "Transformed P3:" << p3Transformed << "\n";
//
//   const bool originalValid = LRL_Cell(p3Original).CheckValid();
//   const bool transformedValid = LRL_Cell(p3Transformed).CheckValid();
//
//   std::cout << "Original valid:   " << originalValid << "\n";
//   std::cout << "Transformed valid:" << transformedValid << "\n";
//
//   if (deltaVol < 1e-5 && transformedValid)
//      std::cout << "✅ Gruber matrix maps cleanly into P3 — symmetry preserved\n";
//   else
//      std::cout << "❌ Transformation disrupts lattice geometry\n";
//}
//
//Matrix_3x3 EstimateP3Transformation(
//   const std::vector<Vector_2>& original,
//   const std::vector<Vector_2>& transformed) {
//
//   if (original.size() != 3 || transformed.size() != 3) {
//      std::cerr << "EstimateP3Transformation requires three original and three transformed vectors\n";
//      return Matrix_3x3(); // fallback to identity or zero
//   }
//
//   // Build original P3 matrix as 3x3
//   Matrix_3x3 V_orig(
//      original[0][0], original[0][1], 0.0,
//      original[1][0], original[1][1], 0.0,
//      original[2][0], original[2][1], 0.0
//   );
//
//   // Build transformed P3 matrix as 3x3
//   Matrix_3x3 V_trans(
//      transformed[0][0], transformed[0][1], 0.0,
//      transformed[1][0], transformed[1][1], 0.0,
//      transformed[2][0], transformed[2][1], 0.0
//   );
//
//   if (std::abs(V_orig.Determinant()) < 1e-12) {
//      std::cerr << "Original P3 matrix is not invertible\n";
//      return Matrix_3x3(); // fallback or signal failure
//   }
//
//   const Matrix_3x3 M = V_trans * V_orig.Inverse();
//   return M;
//}
//
//
//
//
//
//
//int main(int argc, char* argv[]) {
//   //TestGruberMatrixOnP3();
//   //TestProjectionVolumeStability();
//   //AnglePerturbationVolumeTest();
//   //TestP3RoundTripConversion();
//   //TestProjectionSubtraction();
//   writeReduced();
//   //CompareP3AndNiggli();
//   //TestCanonicalProjectedTriplet();
//   //RandomReduceComparison();
//
//   std::cout << "; Path generator" << std::endl;
//
//   //for (size_t i = 0; i < 1000; ++i) {
//   //   const LRL_Cell cell1(Polar::rand());
//   //   const P3 p3(cell1);
//   //   const LRL_Cell cell2(p3);
//   //   const P3 stdp3(P3_Reduce::CanonicalPresentation(p3));
//   //   const LRL_Cell cell3(stdp3);
//   //   if ( !cell3.IsValid())
//   //   {
//   //      const int i19191 = 19191;
//   //   }
//   //}
//
//
//   try {
//      // Get path length from command line if provided
//      int commandLineLength = 20;
//      if (argc > 1) {
//         commandLineLength = atoi(argv[1]);
//         if (commandLineLength == 0) commandLineLength = 20;
//      }
//
//      CmdPathControls controls(commandLineLength);
//      const BasicProgramInput<CmdPathControls> dc_setup("CmdPath", controls);
//
//      if (dc_setup.getInputList().empty()) {
//         throw std::runtime_error("; No input vectors provided");
//      }
//
//      if (controls.shouldShowControls()) {
//         std::cout << controls << std::endl;
//      }
//
//      std::vector<S6> s6Cells;
//      for (const auto& input : dc_setup.getInputList()) {
//         s6Cells.push_back(input.getCell());
//      }
//
//      // Handle single input case
//      if (s6Cells.size() == 1) {
//         const std::string lattice = dc_setup.getInputList()[0].getLatticeType();
//         const G6 g6red = LatticeConverter().NiggliReduceCell(lattice, s6Cells[0]);
//
//         if (G6(s6Cells[0] - g6red).norm() > 1.0) {
//            s6Cells.emplace_back(g6red);
//            std::cout << "; Path Generator found only one point. Using Niggli reduced input for 2nd point" << std::endl;
//         }
//         else {
//            std::cout << "; Path Generator found only one point. Using random cell for 2nd point" << std::endl;
//            s6Cells.emplace_back(S6::rand());
//         }
//      }
//      std::vector<S6> path;
//      const int pathLength = controls.getPathLength();
//
//      //std::vector<S6> s6Cells;
//      for (size_t i = 0; i < s6Cells.size() - 1; ++i) {
//         const S6 delta = (s6Cells[i + 1] - s6Cells[i]) / double(pathLength - 1);
//         const std::vector<S6> pathStep = CreatePathBetweenTwoPoints(pathLength, delta, s6Cells[i], s6Cells[i + 1]);
//         path.insert(path.end(), pathStep.begin(), pathStep.end());
//      }
//
//      std::vector<P3> p3path;
//      for (const auto& p : path) {
//         p3path.emplace_back(p);
//      }
//
//      std::cout << "P3 as input " << std::endl;
//      for (const auto& input : p3path) {
//         std::cout << (input) << std::endl;
//      }
//      std::cout << std::endl << std::endl;
//
//      std::cout << "P3 reduced " << std::endl;
//      std::vector<P3> P3reducedPath;
//      std::vector<P3> P3CostreducedPath;
//      for (const auto& input : p3path) {
//         P3 reduced = P3_Reduce::Reduce(input);
//         std::cout << "P3 " << reduced << std::endl;
//         P3reducedPath.emplace_back(reduced);
//         P3CostreducedPath.emplace_back(P3_Reduce::ReduceScalarCost(input));
//      }
//      std::cout << std::endl << std::endl;
//
//      std::vector<double> pathDiffs;
//      for (size_t i = 0; i < P3reducedPath.size()-1; ++i) {
//         pathDiffs.emplace_back((P3reducedPath[i] - P3reducedPath[i + 1]).norm());
//      }
//
//      for (const auto& p : pathDiffs) {
//         std::cout << p << std::endl;
//      }
//
//      std::cout << "Comparing the two reduction models" << std::endl;
//      double maxdiff = -DBL_MAX;
//      for (size_t i = 0; i < P3CostreducedPath.size(); ++i) {
//         std::cout << P3reducedPath[i] << std::endl;
//         std::cout << P3CostreducedPath[i] << std::endl << std::endl;
//         maxdiff = (P3reducedPath[i]- P3CostreducedPath[i]).norm();
//      }
//      std::cout << "For " << P3CostreducedPath.size() 
//         << " samples,the maximum difference between the two reduction methods is " << maxdiff << std::endl;
//
//
//
//      return 0;
//   }
//   catch (const std::exception& e) {
//      std::cerr << "; An error occurred: " << e.what() << std::endl;
//      return 1;
//   }
//}