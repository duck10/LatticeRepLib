//#include "SystematicMatrixGenerator.h"
//#include "TransformationMatrices.h"
//#include <iostream>
//#include "LRL_Vector3.h"
//#include <vector>
//
//int main() {
//   std::cout << "=== MATRIX GENERATION COUNT VERIFICATION ===" << std::endl;
//
//   // Test 1: Verify we can reproduce the 3480 count
//   std::cout << "\n1. Testing {-1, 0, 1} set (should match 3480):" << std::endl;
//   auto set_neg1to1 = SystematicMatrixGenerator::GenerateSet_Neg1To1();
//   auto original3480 = TransformationMatrices::generateUnimodularMatrices();
//
//   std::cout << "Our systematic generation: " << set_neg1to1.size() << " matrices" << std::endl;
//   std::cout << "Original 3480 generation: " << original3480.size() << " matrices" << std::endl;
//   std::cout << "Match: " << (set_neg1to1.size() == original3480.size() ? "YES" : "NO") << std::endl;
//
//   // Test 2: Predict and generate {-2, -1, 0, 1, 2}
//   std::cout << "\n2. Testing {-2, -1, 0, 1, 2} set:" << std::endl;
//   std::vector<double> elements_2 = { -2, -1, 0, 1, 2 };
//   SystematicMatrixGenerator::PredictMatrixCounts(elements_2);
//
//   auto set_neg2to2 = SystematicMatrixGenerator::GenerateSet_Neg2To2();
//   std::cout << "Actual generation result: " << set_neg2to2.size() << " matrices" << std::endl;
//
//   // Test 3: Predict {-3, -2, -1, 0, 1, 2, 3} (might be too large to generate)
//   std::cout << "\n3. Predicting {-3, -2, -1, 0, 1, 2, 3} set:" << std::endl;
//   std::vector<double> elements_3 = { -3, -2, -1, 0, 1, 2, 3 };
//   SystematicMatrixGenerator::PredictMatrixCounts(elements_3);
//
//   // Only generate if reasonable size
//   if (elements_3.size() <= 5) {  // 5^9 = ~2M combinations
//      auto set_neg3to3 = SystematicMatrixGenerator::GenerateSet_Neg3To3();
//      std::cout << "Actual generation result: " << set_neg3to3.size() << " matrices" << std::endl;
//   }
//   else {
//      std::cout << "Skipping generation - too many combinations for quick test" << std::endl;
//   }
//
//   // Summary
//   std::cout << "\n=== SUMMARY ===" << std::endl;
//   std::cout << "The systematic approach should generate many more matrices than pattern-based methods." << std::endl;
//   std::cout << "If {-2,-1,0,1,2} generates thousands of matrices, this explains why our" << std::endl;
//   std::cout << "previous pattern-based approach only found 82 out of a much larger space." << std::endl;
//
//   return 0;
//}
