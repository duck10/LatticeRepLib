#include <iostream>
#include <string>
#include <type_traits>
#include <algorithm>
#include <iomanip>

#include "B4.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "LatticeCell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_Cell.h"
#include "LRL_Vector3.h"
#include "S6.h"

// Function 1: Convert from Type2 to Type1
template <typename Type1, typename Type2>
Type1 convert(const Type2& input) {
   return Type1(input);
}

// Function for converting to string
template <typename Type1>
std::string toString(const Type1& value) {
   if constexpr (std::is_arithmetic_v<Type1>) {
      return std::to_string(value);
   }
   else {
      // For other types, you might need specialized conversions
      static_assert(std::is_convertible_v<Type1, std::string>,
         "No conversion from this type to string available");
      return std::string(value);
   }
}

// Function 2: Print a value as Type2, with input type deduced
template <typename Type2, typename Type1>
void printAs(const Type1& value, std::ostream& os = std::cout) {
   if constexpr (std::is_same_v<Type2, std::string>) {
      os << toString(value);
   }
   else {
      os << Type2(value);
   }
}

void printB4(const B4& b4) {
   std::cout << b4[0] << std::endl;
   std::cout << b4[1] << std::endl;
   std::cout << b4[2] << std::endl;
   std::cout << b4[3] << std::endl;
}

void printComparisonResults(const std::string& testName, bool equal) {
   std::cout << testName << ": " << (equal ? "EQUAL" : "DIFFERENT") << std::endl;
}

void printHighPrecisionCell(const LRL_Cell& cell) {
   std::cout << "Cell parameters: ";
   for (size_t i = 0; i < 6; ++i) {
      std::cout << std::fixed << std::setprecision(15) << cell[i] << " ";
   }
   std::cout << std::endl;
}

void detailedCompareB4(const B4& a, const B4& b) {
   std::cout << "B4 vector norms comparison:" << std::endl;
   for (size_t i = 0; i < 4; ++i) {
      std::cout << "  Vector " << i << ": " << std::fixed << std::setprecision(15)
         << a[i].norm() << " vs " << b[i].norm()
         << " (diff: " << a[i].norm() - b[i].norm() << ")" << std::endl;
   }

   std::cout << "B4 dot products comparison:" << std::endl;
   for (size_t i = 0; i < 4; ++i) {
      for (size_t j = i + 1; j < 4; ++j) {
         double dotA = a[i].Dot(a[j]);
         double dotB = b[i].Dot(b[j]);
         std::cout << "  Dot(" << i << "," << j << "): " << std::fixed << std::setprecision(15)
            << dotA << " vs " << dotB
            << " (diff: " << dotA - dotB << ")" << std::endl;
      }
   }
}

void analyzeTransformationPath(const B4& originalB4, const Matrix_3x3& permMatrix) {
   // Create permuted B4
   Vector_3 pv1 = permMatrix.MV(originalB4[0]);
   Vector_3 pv2 = permMatrix.MV(originalB4[1]);
   Vector_3 pv3 = permMatrix.MV(originalB4[2]);
   Vector_3 pv4 = -(pv1 + pv2 + pv3);
   B4 permutedB4(pv1, pv2, pv3, pv4);

   std::cout << "=== Step 1: Original vs Permuted B4 ===" << std::endl;
   std::cout << "Original B4: " << originalB4 << std::endl;
   std::cout << "Permuted B4: " << permutedB4 << std::endl;
   detailedCompareB4(originalB4, permutedB4);

   // Convert to LRL_Cell
   std::cout << "\n=== Step 2: Conversion to LRL_Cell ===" << std::endl;
   LRL_Cell origCell(originalB4);
   LRL_Cell permCell(permutedB4);

   std::cout << "Original LRL_Cell: " << std::endl;
   printHighPrecisionCell(origCell);
   std::cout << "Permuted LRL_Cell: " << std::endl;
   printHighPrecisionCell(permCell);

   // Direct comparison of the raw parameters
   std::cout << "\nDirect parameter comparison:" << std::endl;
   for (size_t i = 0; i < 6; ++i) {
      std::cout << "  Parameter " << i << ": " << std::fixed << std::setprecision(15)
         << origCell[i] << " vs " << permCell[i]
         << " (diff: " << origCell[i] - permCell[i] << ")" << std::endl;
   }

   // Recreate B4 from LRL_Cell
   std::cout << "\n=== Step 3: Recreate B4 from LRL_Cell ===" << std::endl;
   B4 recreatedOrigB4(origCell);
   B4 recreatedPermB4(permCell);

   std::cout << "Recreated Original B4: " << recreatedOrigB4 << std::endl;
   std::cout << "Recreated Permuted B4: " << recreatedPermB4 << std::endl;

   // Compare recreated B4s
   std::cout << "\nComparing recreated B4s:" << std::endl;
   detailedCompareB4(recreatedOrigB4, recreatedPermB4);

   // Compare original to recreated
   std::cout << "\n=== Step 4: Original vs Recreated B4 Comparison ===" << std::endl;
   std::cout << "Original B4 vs Recreated from LRL_Cell:" << std::endl;
   detailedCompareB4(originalB4, recreatedOrigB4);

   std::cout << "\nPermuted B4 vs Recreated from LRL_Cell:" << std::endl;
   detailedCompareB4(permutedB4, recreatedPermB4);
}
int main() {
   // Create a nearly cubic B4
   Vector_3 v1(10.5, 0, 0);
   Vector_3 v2(-0.0174533, 9.99998, 0);
   Vector_3 v3(-0.0174533, -0.0174838, 9.99997);
   Vector_3 v4 = -(v1 + v2 + v3);
   B4 nearlyCubicB4(v1, v2, v3, v4);

   // Define permutation matrix
   Matrix_3x3 permMatrix(
      0, 0, 1,
      1, 0, 0,
      0, 1, 0
   );

   std::cout << "=== Testing with Nearly Cubic Cell ===" << std::endl;
   analyzeTransformationPath(nearlyCubicB4, permMatrix);

   return 0;
}

