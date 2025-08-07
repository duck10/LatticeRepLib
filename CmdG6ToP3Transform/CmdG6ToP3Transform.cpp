#include <iostream>
#include <iomanip>
#include <vector>
#include "LRL_Cell.h"
#include "B4.h"
#include "P3.h"
#include "P3_Reduce.h"
#include "P3Dist.h"
#include "LRL_Vector3.h"
#include "TransformationMatrices.h"

// Use canonical base vectors for transformation
static std::vector<Vector_3> GetBaseVectors(const LRL_Cell& cell) {
   const B4 b4(cell);
   return { b4[0], b4[1], b4[2] };
}

static LRL_Cell GetCellFromBaseVectors(const std::vector<Vector_3>& vecs) {
   LRL_Cell out;
   for (size_t i = 0; i < 3; ++i)
      out[i] = vecs[i].norm();

   const Vector_3 zero(0.0, 0.0, 0.0);
   out[3] = Angle(vecs[1], vecs[2]);  // angle between b and c
   out[4] = Angle(vecs[0], vecs[2]);  // angle between a and c
   out[5] = Angle(vecs[0], vecs[1]);  // angle between a and b
   out.SetValid(out.CheckValid());
   return out;
}


static LRL_Cell ApplyMatrixToCell(const LRL_Cell& cell, const Matrix_3x3& m) {
   const std::vector<Vector_3> base = GetBaseVectors(cell);
   std::vector<Vector_3> transformed;
   for (const Vector_3& v : base)
      transformed.push_back(m * v);
   return GetCellFromBaseVectors(transformed);
}

int main() {
   std::cout << "; CmdG6ToP3Transform — Apply crystallographic transformations and compare P3\n";

   const double tolerance = 1e-3;
   std::string inputLine;
   std::getline(std::cin, inputLine);
   const LRL_Cell inputCell(inputLine);
   if (!inputCell.IsValid()) {
      std::cerr << "; ERROR: Invalid input cell\n";
      return 1;
   }

   std::cout << "input cell " << inputCell << std::endl;

   const P3 p3Original = P3_Reduce::Reduce(P3(inputCell));

   std::vector<Matrix_3x3> matrices = TransformationMatrices::getAllPermutationMatrices();
   std::cout << "; Loaded " << matrices.size() << " crystallographic transformations\n";
   std::cout << "first matrix in list\n" << matrices[0] << std::endl;
   size_t matchCount = 0;
   for (size_t i = 0; i < matrices.size(); ++i) {
      const LRL_Cell transformedCell = ApplyMatrixToCell(inputCell, matrices[i]);
      std::cout << "; Matrix #" << i + 1 << ": Valid = " << transformedCell.IsValid() << std::endl;

      if (!transformedCell.IsValid()) {
         std::cout << "; ⚠️ Transformation produced invalid cell\n";
         std::cout << "; Transformed cell: " << transformedCell << std::endl;
         continue;
      }

      //if (!transformedCell.IsValid()) continue;

      const P3 p3Transformed = P3_Reduce::Reduce(P3(transformedCell));

      std::cout << "; Original P3: " << p3Original << std::endl;
      std::cout << "; Transformed P3: " << p3Transformed << std::endl;



      const double dist = P3Dist::ReducedSymmetric(p3Original, p3Transformed);

      std::cout << std::fixed << std::setprecision(6);
      std::cout << "; Matrix #" << std::setw(3) << i + 1
         << " → P3Dist = " << std::setw(10) << dist;

      if (dist < tolerance) {
         std::cout << " ✅ P3-equivalent" << std::endl;
         ++matchCount;
      }
      else {
         std::cout << std::endl;
      }
   }

   std::cout << "; Done. " << matchCount << " out of " << matrices.size()
      << " matrices preserve scalar form in P3.\n";

   return 0;
}
