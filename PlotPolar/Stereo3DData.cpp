#include "Stereo3DData.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


Stereo3DData::Stereo3DData(const std::vector<Polar>& polarData) {
   extractProjections(polarData);
}

void Stereo3DData::extractProjections(const std::vector<Polar>& polarData) {
   m_projection1.clear();
   m_projection2.clear();

   for (const Polar& polar : polarData) {
      // First projection: (p1[0], p2[0], p3[0])
      Vector_3 proj1(polar[0][0], polar[1][0], polar[2][0]);
      m_projection1.push_back(proj1);

      // Second projection: (p1[1], p2[1], p3[1])
      Vector_3 proj2(polar[0][1], polar[1][1], polar[2][1]);
      m_projection2.push_back(proj2);
   }
}



Matrix_3x3 Stereo3DData::createYRotationMatrix(double angleDegrees) const {
   double angleRadians = angleDegrees * M_PI / 180.0;

   // Use GeneralRotation to create Y-axis rotation
   // Rotate around Y-axis: from X-axis toward Z-axis
   Vector_3 v1(1.0, 0.0, 0.0);  // X-axis
   Vector_3 v2(0.0, 0.0, 1.0);  // Z-axis

   auto [rotMatrix, translation] = Vector_3::GeneralRotation(angleRadians, v1, v2);
   return rotMatrix;
}

std::pair<std::vector<Vector_3>, std::vector<Vector_3>>
Stereo3DData::generateStereoPair(int projectionIndex, double stereoAngleDegrees) const {

   const auto& sourceData = (projectionIndex == 1) ? m_projection1 : m_projection2;

   Matrix_3x3 leftRotation = createYRotationMatrix(-stereoAngleDegrees / 2.0);
   Matrix_3x3 rightRotation = createYRotationMatrix(+stereoAngleDegrees / 2.0);

   std::vector<Vector_3> leftEye, rightEye;

   for (const Vector_3& point : sourceData) {
      // Use MV method: point.MV(matrix)
      leftEye.push_back(point.MV(leftRotation));
      rightEye.push_back(point.MV(rightRotation));
   }

   return { leftEye, rightEye };
}
