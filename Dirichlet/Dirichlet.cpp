#include "Dirichlet.h"

#include "DirichletConstants.h"
#include "D7.h"
#include "Faces.h"
#include "LatticeConverter.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "SVG_Tools.h"
#include "TriangleAreaFromSides.h"

#include <cmath>

DirichletSVG::DirichletSVG(const DirichletCell& dc)
   : m_dirCell(dc)
{ }

std::string DirichletSVG::OutputSVG(const std::vector<std::string>& stereoImages,
   const Cell_Faces& faceRecords, const std::string& extra) const {
   const std::string stringCell =
      LRL_ToString("\nReduced Cell \n", LRL_Cell_Degrees(m_dirCell.GetCell()), "\n  S6  ", S6(m_dirCell.GetCell()), "\n  G6  "
         , G6(m_dirCell.GetCell()), "\n  D7  ", D7(m_dirCell.GetCell()), "\n\n");

   std::vector<std::string> scell;

   scell.push_back(stringCell);
   scell.push_back(extra);
   scell.insert(scell.end(), DirichletConstants::note.begin(), DirichletConstants::note.end());

   const std::string header = ImageHeader(DirichletConstants::canvas_x_size, DirichletConstants::canvas_y_size) + "\n";
   std::vector<std::string> footer = ImageFooter(scell);

   const std::string output = header +
      LRL_StringTools::ConcatanateStrings(stereoImages) + LRL_StringTools::ConcatanateStrings(footer);
   return output;
}

static ANGLELIST ComputeAnglesForOneFace(const ANGLELIST& list, const Vector_3& cm) {
   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);
   // So here we will take the first point and compute a unit vector 
   // (the points are already at the center of mass.) The second basis
   // vector is the plane normal. The third will be the cross product
   // of those two vectors. The vector from the origin to the center
   // of mass is the plane normal.
   if (false || list.empty() || ((list[0].second - cm).Norm() < 1.0E-5)) return ANGLELIST();
   const Vector_3 firstVectorInList = list[0].second;
   const Vector_3 basis1 = firstVectorInList / firstVectorInList.Norm();
   const Vector_3 basis2 = cm / cm.Norm();
   const Vector_3 basis3 = basis1.Cross(basis2);
   const double angleZero = degreesPerRad * atan2(list[0].second.Dot(basis1), list[0].second.Dot(basis3));;
   ANGLELIST restoredWithAngles;

   for (size_t face = 0; face < list.size(); ++face) {
      double angle = degreesPerRad * atan2(list[face].second.Dot(basis1), list[face].second.Dot(basis3));
      angle = std::fmod(angle - angleZero + 3.0 * 360.0, 360.0);
      const ANGLE_COORDS ac = ANGLE_COORDS(angle, list[face].second + cm);
      restoredWithAngles.push_back(ac);
   }
   return restoredWithAngles;

}

static Vector_3 CenterOfMassForOneFace(const ANGLELIST& list) { // LCA duplicate!!!!!!!!!!!!!!!!!!!
   Vector_3 centerOfMass(0.0, 0.0, 0.0);
   if (list.size() == 1) return list[0].second;
   if (list.empty()) return centerOfMass;
   for (int face = 0; face < list.size(); ++face) {
      const Vector_3& point = list[face].second;
      centerOfMass += point;
   }
   return centerOfMass / double(list.size());
}


static ANGLELIST MoveOneFaceToCenterOfMass(const ANGLELIST& list) {
   const Vector_3 centerOfMass = CenterOfMassForOneFace(list);
   ANGLELIST coordsAtCenterOfMass;
   const Vector_3 cm = CenterOfMassForOneFace(list);

   for (size_t face = 0; face < list.size(); ++face) {
      coordsAtCenterOfMass.push_back(ANGLE_COORDS(-19192.0, list[face].second - cm));
   }
   return coordsAtCenterOfMass;
}

static ANGLELIST SortAnglesForOneFace(const ANGLELIST& toSort) {
   //   // bubble sort the result since Vector_3 does not have operator<
   ANGLELIST temp(toSort);
   for (size_t face = 0; face < temp.size(); ++face) {
      for (size_t k = 0; k < temp.size(); ++k) {
         if (temp[face].first < temp[k].first) std::swap(temp[face], temp[k]);
      }
   }
   return temp;
}

static ANGLELIST RemoveDuplicatesFromOneAngleList(const ANGLELIST& anglelist) {
   if (anglelist.empty()) return ANGLELIST();
   ANGLELIST intermediateOutput;
   const double initialAngle = anglelist[0].first;
   intermediateOutput.push_back(anglelist[0]);
   for (size_t face = 1; face < anglelist.size(); ++face) {
      const double& angle1 = anglelist[face - 1].first;
      const double& angle2 = anglelist[face].first;
      if (abs(angle2 - angle1) > 1.0E-3 && angle2 < 360.0 + initialAngle - 1.0E-3) {  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!  still need to handle -180 != 180
         intermediateOutput.push_back(anglelist[face]);
      }
   }

   const Vector_3 centerOfMass = CenterOfMassForOneFace(intermediateOutput);
   const double distanceFromOrigin = (centerOfMass - intermediateOutput[0].second).Norm();
   //if (distanceFromOrigin < 1.0E-4) return ANGLELIST();

   const size_t nio = intermediateOutput.size();
   if (intermediateOutput.size() != anglelist.size()) {
      int i19191 = 19191;
   }
   return (intermediateOutput.size() > 3) ? intermediateOutput : ANGLELIST();
}

Vector_3 CenterOfMassForObject(const ANGLESFORFACES& list) {
   Vector_3 cm(0, 0, 0);
   for (size_t face = 0; face < list.size(); ++face) {
      cm += CenterOfMassForOneFace(list[face]);
   }
   return cm / double(list.size());
}

static ANGLESFORFACES MakeRings(const ANGLESFORFACES& faces_in, const std::vector<Intersection>& vIntersections) {
   ANGLESFORFACES restored_faces_out;
   //
   // THIS SHOULD BE SIMPLIFIED
   //
   for (size_t face = 0; face < faces_in.size(); ++face) {
      const ANGLELIST moved = MoveOneFaceToCenterOfMass(faces_in[face]);
      const ANGLELIST restoredWithAngles = ComputeAnglesForOneFace(moved, CenterOfMassForOneFace(faces_in[face]));
      const ANGLELIST sorted = SortAnglesForOneFace(restoredWithAngles);
      const ANGLELIST cleaned = RemoveDuplicatesFromOneAngleList(sorted);
      if (cleaned.size() > 2) restored_faces_out.push_back(cleaned);
   }

   Vector_3 cm = CenterOfMassForObject(restored_faces_out);

   if (cm.Norm() > 1.0E-4) {
      const int i19191 = 19191;
   }
   return restored_faces_out;
}

static int CountFaces(const POINT_LIST& p) {
   int max = -INT_MAX;

   for (int face = 0; face < p.size(); ++face) {
      max = std::max(max, int(p[face][0]));
      max = std::max(max, int(p[face][1]));
      max = std::max(max, int(p[face][2]));
   }
   return max + 1;
}

static ANGLESFORFACES AssignPointsToFaceList(const std::pair<POINT_LIST, std::vector<Intersection> >& input) {
   ANGLESFORFACES output;

   const POINT_LIST& inputIndices = input.first;
   const std::vector<Intersection>& angleCoords = input.second;
   // make list of faces with points

   const int nFaces = CountFaces(inputIndices);
   ANGLESFORFACES anglesForFaces(nFaces);
   for (int face = 0; face < inputIndices.size(); ++face) {
      const std::vector<size_t>& v_index = inputIndices[face];
      const size_t& i1 = v_index[0];
      const size_t& i2 = v_index[1];
      const size_t& i3 = v_index[2];
      const Vector_3& v_Coords_out1 = angleCoords[face].GetCoord();
      const int i19191 = 19191;
      const ANGLE_COORDS anglec(-19191.0, v_Coords_out1);
      anglesForFaces[i1].push_back(anglec);
      anglesForFaces[i2].push_back(anglec);
      anglesForFaces[i3].push_back(anglec);
   }

   const Vector_3 cm1 = CenterOfMassForObject(anglesForFaces);

   for (size_t face = 0; face < anglesForFaces.size(); ++face) {
      if (anglesForFaces[face].size() > 2) output.push_back(anglesForFaces[face]);
      if (anglesForFaces[face].size() == 1) output.push_back(anglesForFaces[face]);
   }

   const Vector_3 cm = CenterOfMassForObject(output);

   if (cm.Norm() > 1.0E-4) {
      const int i19191 = 19191;
   }

   return output;
}

static std::pair<POINT_LIST, std::vector<Intersection> > ComputeIntersections(const CNearTree<Vector_3>& tree) {
   std::vector<DirichletFace> dirichletFaces = Cell_Faces::CreateFaces(tree);
   const size_t n = dirichletFaces.size();
   POINT_LIST vvindex;
   std::vector<Intersection> vIntersections;

   const auto it0 = tree.NearestNeighbor(0.0001, Vector_3(0., 0., 0.));  // origin
   const long n0 = it0.get_position();
   //
   //
   for (size_t i1 = 0; i1 < n - 2; ++i1) {
      for (size_t i2 = i1 + 1; i2 < n - 1; ++i2) {
         if (i1 == i2) continue;
         for (size_t i3 = i2 + 1; i3 < n; ++i3) {
            if (i1 == i3 || i2 == i3) continue;
            Intersection intersection = Intersection::FindIntersectionForThreeFaces(dirichletFaces[i1], dirichletFaces[i2], dirichletFaces[i3]);

            const double d = intersection.GetCoord()[0];
            const bool b1 = !(intersection.GetCoord()[0] <= 0.0);
            const bool b2 = !(intersection.GetCoord()[0] > 0.0);
            const bool b_both = b1 && b2; // for test for indefinite value

            if (intersection.GetCoord()[0] != DBL_MAX && (!b_both)) {
               const auto it = tree.NearestNeighbor(DBL_MAX, intersection.GetCoord());
               const bool bFindOrigin = it == it0;

               double distanceFromOrigin = DBL_MAX;
               double nearestFoundDistance = DBL_MAX;
               if (it != tree.end()) {
                  const size_t pos = it.get_position();
                  distanceFromOrigin = (intersection.GetCoord() - (*it0)).Norm();
                  nearestFoundDistance = (intersection.GetCoord() - (*it)).Norm();
               }
               //if (intersection.IsValid() ) {
               if (intersection.IsValid() && (bFindOrigin || abs(distanceFromOrigin - nearestFoundDistance < 1.0E-4))) {
                  intersection.SetPlaneList(i1, i2, i3);
                  vvindex.push_back({ i1, i2, i3 });
                  vIntersections.push_back(intersection);
                  dirichletFaces[i1].SetIndicesOfIntersection(i1, i2, i3);
                  const int i19191 = 19191;
               }
            }
         }
      }
   }
   std::cout << "intersection count " << vvindex.size() << std::endl;
   return std::make_pair(vvindex, vIntersections);
}

static std::vector<Vector_3> CreateVectorOfLatticePoints(const Matrix_3x3& m) {
   std::vector<Vector_3> v;
   const int maxLatticeLimit = DirichletConstants::latticeLimit;
   for (int face = -maxLatticeLimit; face <= maxLatticeLimit; ++face) {
      for (int j = -maxLatticeLimit; j <= maxLatticeLimit; ++j) {
         for (int k = -maxLatticeLimit; k <= maxLatticeLimit; ++k) {
            double di = face;
            double dj = j;
            double dk = k;
            Vector_3 v3 = m * Vector_3(di, dj, dk);
            for (size_t face = 0; face < 3; ++face)
               if (abs(v3[face]) < 1.0E-8) v3[face] = 0.0;
            v.push_back(v3);
         }
      }
   }
   return v;
}

static CNearTree<Vector_3> CreateTreeOfLatticePoints(const Matrix_3x3& m) {
   const std::vector<Vector_3> v = CreateVectorOfLatticePoints(m);
   return v;
}

static Vector_3 RecoverIndicesOfOneFace(const Matrix_3x3& minv, const ANGLELIST& list) {
   const Vector_3 cm = CenterOfMassForOneFace(list);; // LCA duplicate !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   return minv * cm;
}

static std::vector<Vector_3> RecoverIndicesOfFaces(const Matrix_3x3& m_cart, const ANGLESFORFACES& newRinged) {
   const Matrix_3x3 minv(m_cart.Inver()); // LCA duplicate !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   std::vector<Vector_3> vout;
   for (size_t i = 0; i < newRinged.size(); ++i) {
      Vector_3 v = (2.0 * RecoverIndicesOfOneFace(minv, newRinged[i]));
      for (size_t i = 0; i < 3; ++i) v[i] = (abs(v[i]) < 1.0e-6) ? 0.0 : v[i];
      for (size_t i = 0; i < 3; ++i) v[i] = (abs(abs(v[i]) - 1) < 1.0e-6) ? round(v[i]) : v[i];
      vout.push_back(v);
   }
   return vout;
}

static double AreaOfOneFace(const ANGLELIST& face) {
   const Vector_3 cm = CenterOfMassForOneFace(face);

   double totalArea = 0.0;
   for (size_t i = 0; i < face.size(); ++i)
   {
      const Vector_3 v1 = face[i].second;
      const Vector_3 v2 = (i == face.size() - 1) ? face[0].second : face[i + 1].second;
      const Vector_3 v3 = v2 - v1;

      const double side1 = (v1 - cm).norm();
      const double side2 = (v2 - cm).norm();
      const double side3 = v3.Norm();

      const double area = TriangleAreaFromSides(side1, side2, side3); // Heron's formula for 3 sides
      totalArea += area;
   }
   return totalArea;
}

DirichletCell::DirichletCell(const std::string& strCell)
   : m_strCell(strCell)
{
   //-------------decode unit cell
   LRL_ReadLatticeData rdc;
   rdc.CellReader(strCell);
   m_cell = rdc.GetCell();

   ////-------------reduce cell
   if (DirichletConstants::sellingNiggli == "SELLING")
      m_reducedCell = LatticeConverter().SellingReduceCell(rdc.GetLattice(), rdc.GetCell());
   else
      m_reducedCell = LatticeConverter().NiggliReduceCell(rdc.GetLattice(), rdc.GetCell());

   ////-------------cell create faces
   m_cart = m_reducedCell.Cart();
   m_cellFaces = Cell_Faces(m_reducedCell, m_cart);

   //-------------create Dirichlet faces
   const CNearTree<Vector_3> tree = CreateTreeOfLatticePoints(m_cart);
   std::vector<DirichletFace> dirichletFaces = Cell_Faces::CreateFaces(tree);
   const std::pair<POINT_LIST, std::vector<Intersection> > v_Intersections = ComputeIntersections(tree);
   const ANGLESFORFACES vvPoints = AssignPointsToFaceList(v_Intersections);

   m_facesWithVertices = MakeRings(vvPoints, v_Intersections.second);
   const std::vector<Vector_3> indices = RecoverIndicesOfFaces(GetCartesianMatrix(), m_facesWithVertices);
   m_indices = ConvertAllVectorIndicesToInt(indices);
   m_strIndices = ConvertAllVectorIndicesToString(indices);

   for (size_t faceIndex = 0; faceIndex < m_facesWithVertices.size(); ++faceIndex) {
      double area = AreaOfOneFace(m_facesWithVertices[faceIndex]);
      m_areas.push_back((area < 1.0E-4) ? 0 : area);
   }
}
