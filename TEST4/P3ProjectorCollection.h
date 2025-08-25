#ifndef P3_PROJECTOR_COLLECTION_H
#define P3_PROJECTOR_COLLECTION_H

#include "P3.h"
#include "P3_Reduce.h"
#include "LRL_Cell.h"
#include "Polar.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <map>

class P3ProjectorCollection {
private:
   // Use P3's built-in flat vector functionality
   static std::vector<double> P3ToVector(const P3& p3) {
      return p3.GetFlatVector();
   }

   // Use P3's 6-double constructor
   static P3 VectorToP3(const std::vector<double>& v) {
      return P3(v[0], v[1], v[2], v[3], v[4], v[5]);
   }

public:
   // DATA: Determined projection matrices from SVD analysis

   // single_angle_p2_p3_90deg (p2⊥p3) - VERIFIED
   static inline std::vector<std::vector<double>> P_p2_orth_p3 = {
      {1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
      {0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000},
      {0.0000, 0.0000, 0.8580, 0.2668, -0.2045, -0.0938},
      {0.0000, 0.0000, 0.2668, 0.4986, 0.3844, 0.1763},
      {0.0000, 0.0000, -0.2045, 0.3844, 0.7053, -0.1352},
      {0.0000, 0.0000, -0.0938, 0.1763, -0.1352, 0.9380}
   };

   // single_angle_p1_p3_90deg (p1⊥p3) - VERIFIED  
   static inline std::vector<std::vector<double>> P_p1_orth_p3 = {
      {0.8021, 0.3249, 0.0000, 0.0000, -0.2032, -0.1089},
      {0.3249, 0.4665, 0.0000, 0.0000, 0.3337, 0.1787},
      {0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000},
      {0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000},
      {-0.2032, 0.3337, 0.0000, 0.0000, 0.7913, -0.1118},
      {-0.1089, 0.1787, 0.0000, 0.0000, -0.1118, 0.9401}
   };

   // single_mag_p1_eq_p2 (|p1|=|p2|) - VERIFIED
   static inline std::vector<std::vector<double>> P_mag_p1_eq_p2 = {
      {0.8280, -0.1676, 0.3086, 0.1381, 0.0000, 0.0000},
      {-0.1676, 0.8367, 0.3007, 0.1346, 0.0000, 0.0000},
      {0.3086, 0.3007, 0.4462, -0.2479, 0.0000, 0.0000},
      {0.1381, 0.1346, -0.2479, 0.8890, 0.0000, 0.0000},
      {0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000},
      {0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000}
   };

   // PREDICTED: single_angle_p1_p2_90deg (p1⊥p2)
   // Pattern: preserve p3 [4,5], mix p1,p2 [0,1,2,3] with orthogonal coupling
   static inline std::vector<std::vector<double>> P_p1_orth_p2 = {
      {0.8580, 0.2668, -0.2045, -0.0938, 0.0000, 0.0000},
      {0.2668, 0.4986, 0.3844, 0.1763, 0.0000, 0.0000},
      {-0.2045, 0.3844, 0.7053, -0.1352, 0.0000, 0.0000},
      {-0.0938, 0.1763, -0.1352, 0.9380, 0.0000, 0.0000},
      {0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000},
      {0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000}
   };

   // PREDICTED: single_mag_p1_eq_p3 (|p1|=|p3|)
   // Pattern: preserve p2 [2,3], mix p1,p3 [0,1,4,5] with magnitude coupling
   static inline std::vector<std::vector<double>> P_mag_p1_eq_p3 = {
      {0.8280, -0.1676, 0.0000, 0.0000, 0.3086, 0.1381},
      {-0.1676, 0.8367, 0.0000, 0.0000, 0.3007, 0.1346},
      {0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000},
      {0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000},
      {0.3086, 0.3007, 0.0000, 0.0000, 0.4462, -0.2479},
      {0.1381, 0.1346, 0.0000, 0.0000, -0.2479, 0.8890}
   };

   // PREDICTED: single_mag_p2_eq_p3 (|p2|=|p3|)
   // Pattern: preserve p1 [0,1], mix p2,p3 [2,3,4,5] with magnitude coupling
   static inline std::vector<std::vector<double>> P_mag_p2_eq_p3 = {
      {1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
      {0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000},
      {0.0000, 0.0000, 0.8280, -0.1676, 0.3086, 0.1381},
      {0.0000, 0.0000, -0.1676, 0.8367, 0.3007, 0.1346},
      {0.0000, 0.0000, 0.3086, 0.3007, 0.4462, -0.2479},
      {0.0000, 0.0000, 0.1381, 0.1346, -0.2479, 0.8890}
   };

   // Map boundary types to their projection matrices
   static std::map<std::string, std::vector<std::vector<double>>*> GetProjectorMap() {
      static std::map<std::string, std::vector<std::vector<double>>*> projectorMap = {
         {"single_angle_p2_p3_90deg", &P_p2_orth_p3},
         {"single_angle_p1_p3_90deg", &P_p1_orth_p3},
         {"single_angle_p1_p2_90deg", &P_p1_orth_p2},
         {"single_mag_p1_eq_p2", &P_mag_p1_eq_p2},
         {"single_mag_p1_eq_p3", &P_mag_p1_eq_p3},
         {"single_mag_p2_eq_p3", &P_mag_p2_eq_p3}
      };
      return projectorMap;
   }

   // Apply projection matrix to P3
   static P3 ApplyProjector(const P3& input, const std::string& boundaryType) {
      auto projectorMap = GetProjectorMap();
      auto it = projectorMap.find(boundaryType);

      if (it == projectorMap.end()) {
         std::cout << "Unknown boundary type: " << boundaryType << std::endl;
         return input;
      }

      const auto& projMatrix = *(it->second);
      std::vector<double> v_input = P3ToVector(input);
      std::vector<double> v_output(6, 0.0);

      // Matrix multiplication: v_output = projMatrix * v_input
      for (size_t i = 0; i < 6; ++i) {
         for (size_t j = 0; j < 6; ++j) {
            v_output[i] += projMatrix[i][j] * v_input[j];
         }
      }

      P3 result = VectorToP3(v_output);
      return P3_Reduce::StandardPresentation(result);
   }

   // Test constraint satisfaction
   static double TestConstraint(const P3& p3, const std::string& boundaryType) {
      if (boundaryType == "single_angle_p2_p3_90deg") {
         return p3[1].first * p3[2].first + p3[1].second * p3[2].second;  // p2·p3
      } else if (boundaryType == "single_angle_p1_p3_90deg") {
         return p3[0].first * p3[2].first + p3[0].second * p3[2].second;  // p1·p3
      } else if (boundaryType == "single_angle_p1_p2_90deg") {
         return p3[0].first * p3[1].first + p3[0].second * p3[1].second;  // p1·p2
      } else if (boundaryType == "single_mag_p1_eq_p2") {
         double mag1 = std::sqrt(p3[0].first * p3[0].first + p3[0].second * p3[0].second);
         double mag2 = std::sqrt(p3[1].first * p3[1].first + p3[1].second * p3[1].second);
         return mag1 - mag2;  // |p1| - |p2|
      } else if (boundaryType == "single_mag_p1_eq_p3") {
         double mag1 = std::sqrt(p3[0].first * p3[0].first + p3[0].second * p3[0].second);
         double mag3 = std::sqrt(p3[2].first * p3[2].first + p3[2].second * p3[2].second);
         return mag1 - mag3;  // |p1| - |p3|
      } else if (boundaryType == "single_mag_p2_eq_p3") {
         double mag2 = std::sqrt(p3[1].first * p3[1].first + p3[1].second * p3[1].second);
         double mag3 = std::sqrt(p3[2].first * p3[2].first + p3[2].second * p3[2].second);
         return mag2 - mag3;  // |p2| - |p3|
      }
      return 0.0;
   }

   // Verify a single projector
   static void VerifyProjector(const std::string& boundaryType, int numTests = 20) {
      std::cout << "\n--- Verifying " << boundaryType << " ---" << std::endl;

      int validCells = 0;
      int improvedConstraints = 0;
      double totalImprovement = 0.0;

      for (int test = 0; test < numTests; ++test) {
         // Generate random P3
         P3 testInput(Polar::rand());
         P3 testInputStd = P3_Reduce::StandardPresentation(testInput);

         // Apply projector
         P3 projected = ApplyProjector(testInputStd, boundaryType);

         // Check if result produces valid unit cell
         LRL_Cell projCell(projected);
         bool isValid = projCell.IsValid() && projCell.Volume() > 1.0;
         if (isValid) validCells++;

         // Check constraint improvement
         double constraintBefore = std::abs(TestConstraint(testInputStd, boundaryType));
         double constraintAfter = std::abs(TestConstraint(projected, boundaryType));

         if (constraintAfter < constraintBefore) {
            improvedConstraints++;
            if (constraintBefore > 1e-10) {
               totalImprovement += constraintAfter / constraintBefore;
            }
         }
      }

      std::cout << "Valid unit cells: " << validCells << "/" << numTests << std::endl;
      std::cout << "Improved constraints: " << improvedConstraints << "/" << numTests << std::endl;

      if (improvedConstraints > 0) {
         double avgRatio = totalImprovement / improvedConstraints;
         std::cout << "Average improvement ratio: " << std::fixed << std::setprecision(3) << avgRatio << std::endl;
      }

      if (validCells >= numTests * 0.7 && improvedConstraints >= numTests * 0.5) {
         std::cout << "✓ Projector VERIFIED" << std::endl;
      } else {
         std::cout << "✗ Projector needs refinement" << std::endl;
      }
   }

   // Verify all projectors
   static void VerifyAllProjectors() {
      std::cout << "\n" << std::string(80, '=') << std::endl;
      std::cout << "VERIFYING ALL P3 PROJECTORS" << std::endl;
      std::cout << std::string(80, '=') << std::endl;

      auto projectorMap = GetProjectorMap();
      int verifiedCount = 0;

      for (const auto& pair : projectorMap) {
         VerifyProjector(pair.first, 20);
         // Note: This is a simplified check - could be made more sophisticated
         verifiedCount++;  // Assume verified for now - check output manually
      }

      std::cout << "\n" << std::string(60, '-') << std::endl;
      std::cout << "SUMMARY: " << verifiedCount << "/" << projectorMap.size() << " projectors tested" << std::endl;
      std::cout << "Review individual results to confirm verification status" << std::endl;
   }

   // Display a projection matrix
   static void DisplayProjector(const std::string& boundaryType) {
      auto projectorMap = GetProjectorMap();
      auto it = projectorMap.find(boundaryType);

      if (it == projectorMap.end()) {
         std::cout << "Unknown boundary type: " << boundaryType << std::endl;
         return;
      }

      const auto& projMatrix = *(it->second);

      std::cout << "\n--- " << boundaryType << " Projection Matrix ---" << std::endl;
      for (size_t i = 0; i < 6; ++i) {
         std::cout << "  [";
         for (size_t j = 0; j < 6; ++j) {
            std::cout << std::setw(8) << std::fixed << std::setprecision(4) << projMatrix[i][j];
            if (j < 5) std::cout << ", ";
         }
         std::cout << "]" << std::endl;
      }
   }
};

#endif // P3_PROJECTOR_COLLECTION_H