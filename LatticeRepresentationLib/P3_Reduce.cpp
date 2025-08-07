#include "P3_Reduce.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include "LRL_Cell_Degrees.h"

constexpr double kTolerance = 1.0e-5;

static const Matrix_3x3 identity{ 1,0,0, 0,1,0, 0,0,1 };

static bool IsFullyUpperHalfPlane(const P3& p) {
   for (const auto& [_, y] : p)
      if (y < 0.0) return false;
   return true;
}

Matrix_3x3 ProjectionSubtractionMatrix(const size_t i, const size_t j, const double lambda) {
   Matrix_3x3 M = identity; // Start with identity

   // Modify row i to subtract lambda * row j
   for (size_t col = 0; col < 3; ++col) {
      size_t idx_i = i * 3 + col;
      size_t idx_j = j * 3 + col;
      M[idx_i] -= lambda * M[idx_j];
   }

   return M;
}

Matrix_3x3 CanonicalMatrix(const P3& input) {
   std::array<P3::Vec2, 3> transformed = input.GetVector();
   Matrix_3x3 M_reflect = identity;

   // Step 1: Reflect negative y components
   for (size_t i = 0; i < 3; ++i) {
      const auto& [x, y] = transformed[i];
      if (y < 0.0) {
         transformed[i] = { -x, -y };
         M_reflect[i * 3 + 0] *= -1.0;
         M_reflect[i * 3 + 1] *= -1.0;
      }
   }

   // Step 2: Sort by magnitude
   std::array<std::pair<double, size_t>, 3> mags;
   for (size_t i = 0; i < 3; ++i) {
      const auto& [x, y] = transformed[i];
      mags[i] = { std::sqrt(x * x + y * y), i };
   }

   std::sort(mags.begin(), mags.end());

   Matrix_3x3 M_sort; // permutation matrix
   for (size_t i = 0; i < 3; ++i) {
      size_t src_row = mags[i].second;
      for (size_t j = 0; j < 3; ++j) {
         M_sort[i * 3 + j] = M_reflect[src_row * 3 + j];
      }
   }

   return M_sort;
}




P3 P3_Reduce::EnforceUpperHalfPlane(const P3& p) {
   P3 result = p;
   for (auto& [x, y] : result) {
      if (y < 0.0) {
         x = -x;
         y = -y;
      }
   }
   return result;
}


double P3_Reduce::Cost(const P3& p) {
   const double dot12 = std::abs(Scalar::Dot(p[0], p[1]));
   const double dot13 = std::abs(Scalar::Dot(p[0], p[2]));
   const double dot23 = std::abs(Scalar::Dot(p[1], p[2]));

   const double magSum =
      Scalar::MagnitudeSquared(p[0]) +
      Scalar::MagnitudeSquared(p[1]) +
      Scalar::MagnitudeSquared(p[2]);

   return magSum < 1e-10 ? 0.0 : (dot12 + dot13 + dot23) / magSum;
}

double P3_Reduce::Scalar::Projection(const std::pair<double, double>& pi,
   const std::pair<double, double>& pj) {
   const double dot = Dot(pi, pj);
   const double magSq = MagnitudeSquared(pj);
   return magSq < 1e-10 ? 0.0 : dot / magSq;
}

double P3_Reduce::Scalar::Dot(const std::pair<double, double>& a,
   const std::pair<double, double>& b) {
   return a.first * b.first + a.second * b.second;
}

double P3_Reduce::Scalar::MagnitudeSquared(const std::pair<double, double>& v) {
   return v.first * v.first + v.second * v.second;
}

bool P3_Reduce::CosineSignsUniform(const P3& p) {
   int pos = 0, neg = 0;
   for (const auto& [x, _] : p) {
      if (x < 0) ++neg;
      else       ++pos;
   }
   return (pos == 3 || neg == 3);
}

P3 P3_Reduce::StandardPresentation(const P3& input) {
   std::array<std::pair<double, P3::Vec2>, 3> sorted;
   for (std::size_t i = 0; i < 3; ++i) {
      const auto& [x, y] = input[i];
      const double mag = std::sqrt(x * x + y * y);
      sorted[i] = { mag, { x, y } };
   }

   std::sort(sorted.begin(), sorted.end(),
      [](const auto& a, const auto& b) { return a.first < b.first; });

   P3 result;
   for (std::size_t i = 0; i < 3; ++i)
      result[i] = sorted[i].second;

   return result;
}

P3 P3_Reduce::Reduce(const P3& input) {
   const LRL_Cell cell(input);
   if (!cell.IsValid()) {
      return P3();
   }
   const double originalVolume = cell.Volume();
   P3 current = StandardPresentation(input);
   double bestCost = Cost(current);

   bool improved = true;

   while (improved) {
      improved = false;

      for (size_t i = 0; i < 3; ++i) {
         for (size_t j = 0; j < 3; ++j) {
            if (i == j) continue;

            const double lambda = Scalar::Projection(current[i], current[j]);
            P3 candidate = current;

            candidate[i].first -= lambda * candidate[j].first;
            candidate[i].second -= lambda * candidate[j].second;

            candidate = StandardPresentation(candidate);

            // Geometry filter — skip early if invalid
            if (!LRL_Cell(candidate).CheckValid())
               continue;

            const double newCost = ScalarCost(candidate);
            const double newVolume = LRL_Cell(candidate).Volume();

            const bool accept =
               (newCost + kTolerance < bestCost) &&
               (std::abs(newVolume - originalVolume) < kTolerance);

            if (accept) {
               current = candidate;
               bestCost = newCost;
               improved = true;
               break;
            }
         }
         if (improved) break;
      }
   }

   // Final quadrant enforcement and sorting
   return CanonicalPresentation(current);
}

P3 P3_Reduce::CanonicalPresentation(const P3& input) {
   P3 result;

   // Step 1: Flip all vectors into upper-half-plane
   for (std::size_t i = 0; i < 3; ++i) {
      const auto& [x, y] = input[i];
      result[i] = (y < 0.0) ? P3::Vec2{ -x, -y } : P3::Vec2{ x, y };
   }

   // Step 2: Sort by magnitude (after flipping)
   std::array<std::pair<double, P3::Vec2>, 3> sorted;
   for (std::size_t i = 0; i < 3; ++i) {
      const auto& [x, y] = result[i];
      const double mag = std::sqrt(x * x + y * y);
      sorted[i] = { mag, { x, y } };
   }

   std::sort(sorted.begin(), sorted.end(),
      [](const auto& a, const auto& b) { return a.first < b.first; });

   // Step 3: Output the clean triplet
   for (std::size_t i = 0; i < 3; ++i)
      result[i] = sorted[i].second;

   return result;
}

double P3_Reduce::ScalarCost(const P3& p) {
   return std::abs(Scalar::Dot(p[0], p[1])) +
      std::abs(Scalar::Dot(p[0], p[2])) +
      std::abs(Scalar::Dot(p[1], p[2]));
}


P3 P3_Reduce::ReduceScalarCost(const P3& input) {
   P3 current = StandardPresentation(input);
   const double originalVolume = LRL_Cell(current).Volume();
   double bestCost = ScalarCost(current);  // new cost function

   bool improved = true;
   while (improved) {
      improved = false;

      for (size_t i = 0; i < 3; ++i) {
         for (size_t j = 0; j < 3; ++j) {
            if (i == j) continue;

            const double lambda = Scalar::Projection(current[i], current[j]);
            P3 candidate = current;

            candidate[i].first -= lambda * candidate[j].first;
            candidate[i].second -= lambda * candidate[j].second;

            candidate = StandardPresentation(candidate);

            if (!LRL_Cell(candidate).CheckValid())
               continue;

            const double newCost = ScalarCost(candidate);
            const double newVolume = LRL_Cell(candidate).Volume();



            std::cout << "λ = " << lambda
               << ", ΔVol = " << std::abs(newVolume - originalVolume)
               << ", Vol(candidate) = " << newVolume
               << ", Vol(original) = " << originalVolume << "\n";
            std::cout << "Trying (i,j) = (" << i << "," << j << ")\n";
            std::cout << "λ = " << lambda
               << ", cost = " << newCost << " vs best = " << bestCost
               << ", ΔVolume = " << std::abs(newVolume - originalVolume)
               << ", valid = " << LRL_Cell(candidate).CheckValid() << "\n";



            const double debugVolumeDiff = newVolume - originalVolume;
            const bool accept =
               (newCost + kTolerance < bestCost) &&
               (std::abs(newVolume - originalVolume) < kTolerance);

            if (accept) {
               current = candidate;
               bestCost = newCost;
               improved = true;
               break;  // important: re-run from the top with new current
            }
         }
         if (improved) break;
      }
   }

   return CanonicalPresentation(current);
}

ReducedResult P3_Reduce::MatrixAwareReduce(const P3& input) {
   Matrix_3x3 M_total = identity;
   const LRL_Cell cell(input);
   if (!cell.IsValid()) return { P3(), M_total };

   const double originalVolume = cell.Volume();
   P3 current = StandardPresentation(input);
   double bestCost = ScalarCost(current);

   bool improved = true;
   while (improved) {
      improved = false;
      for (size_t i = 0; i < 3; ++i) {
         for (size_t j = 0; j < 3; ++j) {
            if (i == j) continue;
            double lambda = Scalar::Projection(current[i], current[j]);
            P3 candidate = current;

            candidate[i].first -= lambda * candidate[j].first;
            candidate[i].second -= lambda * candidate[j].second;

            candidate = StandardPresentation(candidate);
            if (!LRL_Cell(candidate).CheckValid()) continue;

            double newCost = Cost(candidate);
            double newVolume = LRL_Cell(candidate).Volume();
            bool accept = (newCost + kTolerance < bestCost) &&
               (std::abs(newVolume - originalVolume) < kTolerance);

            if (accept) {
               Matrix_3x3 M_update = ProjectionSubtractionMatrix(i, j, lambda);
               M_total = M_update * M_total;

               current = candidate;
               bestCost = newCost;
               improved = true;
               break;
            }
         }
         if (improved) break;
      }
   }

   Matrix_3x3 M_canonical = CanonicalMatrix(current);
   M_total = M_canonical * M_total;

   return { CanonicalPresentation(current), M_total };
}
