#ifndef STEREO3D_DATA_H
#define STEREO3D_DATA_H

#include "LRL_Vector3.h"
#include "Polar.h"
#include <vector>

class Stereo3DData {
public:
   Stereo3DData(const std::vector<Polar>& polarData);

   // Get 3D points for first projection (p1[0], p2[0], p3[0])
   const std::vector<Vector_3>& getProjection1() const { return m_projection1; }

   // Get 3D points for second projection (p1[1], p2[1], p3[1])
   const std::vector<Vector_3>& getProjection2() const { return m_projection2; }

   // Generate stereo pair with rotation angle
   std::pair<std::vector<Vector_3>, std::vector<Vector_3>>
      generateStereoPair(int projectionIndex, double stereoAngleDegrees) const;

private:
   std::vector<Vector_3> m_projection1;  // (p1[0], p2[0], p3[0])
   std::vector<Vector_3> m_projection2;  // (p1[1], p2[1], p3[1])

   void extractProjections(const std::vector<Polar>& polarData);

   // Create Y-axis rotation matrix using GeneralRotation
   Matrix_3x3 createYRotationMatrix(double angleDegrees) const;
};

#endif // STEREO3D_DATA_H
