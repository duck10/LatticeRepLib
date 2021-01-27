
#include <algorithm>
#include <cmath>
#include <iostream>     // std::cout, std::endl
#include <iomanip>      // std::setfill, std::setw
#include <string>
#include <utility>

#include "Dirichlet.h"
#include "DirichletConstants.h"
#include "Faces.h"
#include "FileOperations.h"
#include "LRL_DataStreamToString.h"
#include "LRL_DataToSVG.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"
#include "ReadGlobalData.h"
#include "SVG_Tools.h"
#include "TNear.h"
#include "TriangleAreaFromSides.h"
#include "LRL_Vector3.h"

template <typename T>
std::vector<T> CircularizeVector(const std::vector<T>& vin) {
   std::vector<T> v(vin);
   v.push_back(vin[0]);
   return v;
}

std::string ConcatanateStrings(const std::vector<std::string>& sinput) {
   std::string s;
   for (size_t face = 0; face < sinput.size(); ++face)
      s += sinput[face];
   return s;
}

Vector_3 CenterOfMassForOneFace(const ANGLELIST& list) {
   Vector_3 centerOfMass(0.0, 0.0, 0.0);
   if (list.size() == 1) return list[0].second;
   if (list.empty()) return centerOfMass;
   for (int face = 0; face < list.size(); ++face) {
      const Vector_3& point = list[face].second;
      centerOfMass += point;
   }
   return centerOfMass / double(list.size());
}

std::pair<POINT_LIST, std::vector<Intersection> > ComputeIntersections( std::vector<DirichletFace>& vdf,
   const CNearTree<Vector_3>& tree) {
   const size_t n = vdf.size();
   POINT_LIST vvindex;
   std::vector<Intersection> vIntersections;

   const auto it0 = tree.NearestNeighbor(0.0001, Vector_3(0., 0., 0.));  // origin
   const long n0 = it0.get_position();
   //
   //
   for (size_t i1 = 0; i1 < n; ++i1) {
      for (size_t i2 = 0; i2 < n; ++i2) {
         for (size_t i3 = 0; i3 < n; ++i3) {
            if (i1 == i2 || i1 == i3 || i2 == i3) continue;
            Intersection intersection = Intersection::FindIntersectionForThreeFaces(vdf[i1], vdf[i2], vdf[i3]);

            const double d = intersection.GetCoord()[0];
            const bool b1 = !(intersection.GetCoord()[0] <= 0.0);
            const bool b2 = !(intersection.GetCoord()[0] > 0.0);
            const bool b_both = b1 && b2;



            if (intersection.GetCoord()[0] != DBL_MAX && (!b_both) ) {
               const auto it = tree.NearestNeighbor(DBL_MAX, intersection.GetCoord());
               const bool bFindOrigin = it == it0;

               double distanceFromOrigin = DBL_MAX;
               double nearestFoundDistance = DBL_MAX;
               double size = DBL_MAX;
               double difference = DBL_MAX;
               if (it != tree.end()) {
                  const size_t pos = it.get_position();
                  distanceFromOrigin = (intersection.GetCoord() - (*it0)).Norm();
                  nearestFoundDistance = (intersection.GetCoord() - (*it)).Norm();
                  size = intersection.GetCoord().Norm();
                  difference = distanceFromOrigin - nearestFoundDistance;
                  const int i19191 = 19191;
               }

               const long n = (it).get_position();
               if (n == n0) {
                  int i19191 = 19191;
               }

               if (intersection.IsValid() && (bFindOrigin || abs(distanceFromOrigin - nearestFoundDistance < 1.0E-4))) {
                  intersection.SetPlaneList(i1, i2, i3);
                  vvindex.push_back({ i1, i2, i3 });
                  vIntersections.push_back(intersection);
                  vdf[i1].SetIndicesOfIntersection(i1,i2,i3);
                  const int i19191 = 19191;
               }
            }
         }
      }
   }
   std::cout << "intersection count " << vvindex.size() << std::endl;
   return std::make_pair(vvindex, vIntersections);
}

int CountFaces(const POINT_LIST& p) {
   int max = -INT_MAX;

   for (int face = 0; face < p.size(); ++face) {
      max = std::max(max, int(p[face][0]));
      max = std::max(max, int(p[face][1]));
      max = std::max(max, int(p[face][2]));
   }
   return max+1;
}

Vector_3 CenterOfMassForObject(const ANGLESFORFACES& list) {
   Vector_3 cm(0, 0, 0);
   for (size_t face = 0; face < list.size(); ++face) {
      cm += CenterOfMassForOneFace(list[face]);
   }
   return cm / double(list.size());
}


ANGLESFORFACES AssignPointsToFaceList(const std::pair<POINT_LIST, std::vector<Intersection> >& input) {
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

   if (cm1.Norm() > 1.0E-4) {
      const int i19191 = 19191;
   }


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

template<typename T>
int roundNo(T num)
{
   return num < 0 ? int(num - 0.5) :int( num + 0.5);
}

Vector_3 round(const Vector_3& vin) {
   Vector_3 vout;
   vout[0] = roundNo(vin[0]);
   vout[1] = roundNo(vin[1]);
   vout[2] = roundNo(vin[2]);
   return vout;
}


void PrintIndices(const ANGLESFORFACES& ringed, const Matrix_3x3& cart)
{
   const Matrix_3x3 inverseCart = cart.Inver();

   std::cout << std::endl;
   std::cout << "INDICES FOR ALL FACES " << std::endl;
   for (size_t face = 0; face < ringed.size(); ++face) {
      std::cout << "\nFACE " << face << "   points on face " << ringed[face].size() << std::endl;
      const ANGLELIST v = ringed[face];
      for (size_t k = 0; k < ringed[face].size(); ++k) {

         const Vector_3 cm = CenterOfMassForOneFace(ringed[face]) / double(ringed.size());;
         const Vector_3 facexxx0 = 2.0 * inverseCart * ringed[face][0].second;
         const Vector_3 facexxx1 = 2.0 * inverseCart * ringed[face][1].second;
         const Vector_3 facexxx2 = 2.0 * inverseCart * ringed[face][2].second;
         const Vector_3 faceByIndex0 = round(facexxx0);
         const Vector_3 faceByIndex1 = round(facexxx1);
         const Vector_3 faceByIndex2 = round(facexxx2);
         std::cout << v[k].first << "     " << faceByIndex0 << "  " << faceByIndex1 << "  " << faceByIndex2 << std::endl;
      }
   }
}

ANGLELIST RemoveDuplicatesFromOneAngleList(const ANGLELIST& anglelist) {
   if (anglelist.empty()) return ANGLELIST();
   ANGLELIST intermediateOutput;
   const double initialAngle = anglelist[0].first;
   intermediateOutput.push_back(anglelist[0]);
   for (size_t face = 1; face < anglelist.size(); ++face) {
      const double angle1 = anglelist[face - 1].first;
      const double angle2 = anglelist[face].first;
      if (abs(angle2 - angle1) > 1.0E-3 && angle2 < 360.0+ initialAngle - 1.0E-3) {  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!  still need to handle -180 != 180
         intermediateOutput.push_back(anglelist[face]);
      }
   }

   const Vector_3 centerOfMass = CenterOfMassForOneFace(intermediateOutput);
   ANGLELIST output;

   const double distanceFromOrigin = (centerOfMass - intermediateOutput[0].second).Norm();
   if ((centerOfMass - intermediateOutput[0].second).Norm() < 1.0E-4) return ANGLELIST();
   const int i1919 = 19191;
   const size_t nio = intermediateOutput.size();
   return intermediateOutput;
}

ANGLELIST MoveOneFaceToCenterOfMass(const ANGLELIST& list) {
   const Vector_3 centerOfMass = CenterOfMassForOneFace(list);
   ANGLELIST coordsAtCenterOfMass;
   const Vector_3 cm = CenterOfMassForOneFace(list);

   for (size_t face = 0; face < list.size(); ++face) {
      coordsAtCenterOfMass.push_back(ANGLE_COORDS(-19192.0, list[face].second - cm));
   }
   return coordsAtCenterOfMass;
}

ANGLELIST ComputeAnglesForOneFace(const ANGLELIST& list, const Vector_3& cm) {
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
      angle = std::fmod( angle - angleZero + 3.0*360.0, 360.0);
      const ANGLE_COORDS ac = ANGLE_COORDS(angle, list[face].second + cm);
      restoredWithAngles.push_back(ac);
   }
   return restoredWithAngles;

}

ANGLELIST SortAnglesForOneFace(const ANGLELIST& toSort) {
   //   // bubble sort the result since Vector_3 does not have operator<
   ANGLELIST temp(toSort);
   for (size_t face = 0; face < temp.size(); ++face) {
      for (size_t k = 0; k < temp.size(); ++k) {
         if (temp[face].first < temp[k].first) std::swap(temp[face], temp[k]);
      }
   }
   return temp;
}

ANGLESFORFACES MakeRings(const ANGLESFORFACES& faces_in, const std::vector<Intersection>& vIntersections) {
   ANGLESFORFACES restored_faces_out;

   for (size_t face = 0; face < faces_in.size(); ++face) {
      const ANGLELIST moved = MoveOneFaceToCenterOfMass(faces_in[face]);
      const ANGLELIST restoredWithAngles = ComputeAnglesForOneFace(moved, CenterOfMassForOneFace(faces_in[face]));
      const ANGLELIST sorted = SortAnglesForOneFace(restoredWithAngles);
      const ANGLELIST cleaned = RemoveDuplicatesFromOneAngleList(sorted);
      if ( cleaned.size() > 2) restored_faces_out.push_back(cleaned);
   }

   Vector_3 cm = CenterOfMassForObject(restored_faces_out);
   
   if (cm.Norm() > 1.0E-4) {
      const int i19191 = 19191;
   }

   /*
   Here, the center of mass is NO LONGER in the middle of the figure!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   */





   return restored_faces_out;
}

void PrintIntersections(const ANGLESFORFACES& vin) {
   std::cout << std::endl;
   std::cout << "ALL INTERSECTIONS" << std::endl;
   for (size_t face = 0; face < vin.size(); ++face) {
      std::cout << "FACE " << face << ", angle count " << vin[face].size() << std::endl;
      for (size_t face = 0; face < vin[face].size(); ++face) {
         const double angle = vin[face][face].first;
         const Vector_3 v = vin[face][face].second;
         std::cout << "  angle  " << angle << "     " << v;
      }
   }
   std::cout << std::endl;
}

void PrintRawIntersection(const std::pair<POINT_LIST, std::vector<Intersection> >& v_Intersections) {
   std::cout << std::endl;
   std::cout << "RAW INTERSECTIONS " << std::endl;
   for (size_t face = 0; face < v_Intersections.first.size(); ++face) {
      std::cout
         << v_Intersections.first[face][0] << "  "
         << v_Intersections.first[face][1] << "  "
         << v_Intersections.first[face][2] << "    "

         << v_Intersections.second[face].GetPlaneIndices()[0] << " "
         << v_Intersections.second[face].GetPlaneIndices()[1] << " "
         << v_Intersections.second[face].GetPlaneIndices()[2] << "   "

         << v_Intersections.second[face].GetCoord()[0] << " "
         << v_Intersections.second[face].GetCoord()[1] << " "
         << v_Intersections.second[face].GetCoord()[2] << std::endl;

   }
}

void PrintDirichletFaces(const std::vector<DirichletFace>& dirichletFaces) {
   for (size_t face = 0; face < dirichletFaces.size(); ++face) {
      const std::vector<size_t> indices = dirichletFaces[face].GetIndicesOfIntersection();
      std::cout << "face " << face << "      " << indices[0] << " " << indices[1] << " " << indices[2] << std::endl;
   }
}

bool Z_DepthTest(const Vector_3& v) {
   return v[0] > 0 && v[1] > 0 && v[2] > 0;
}

bool Z_DepthTest(const ANGLELIST& ring) {
   for (size_t face = 0; face < ring.size(); ++face) {
      if (Z_DepthTest(ring[face].second)) return true;
   }
   return false;
}

std::string DrawOneDirichletRing(const double scale, const ANGLELIST& ring, const size_t nColor) {
   static const std::vector<std::string> colors = { "aqua", "blue", "orange", "darkgray", "lightblue", "yellow", "purple", "teal", "indigo", "violet", "maroon", "yellowgreen" };

   const double xshift = 30;
   const double yshift = 30;
   ANGLELIST colorTest;
   std::string s;
   const Vector_3 cm = CenterOfMassForOneFace(ring);

   const double dotToZAxis = cm.Dot(Vector_3(0,0,1));

   if (ring.size() >= 3) {
      const std::string color = (dotToZAxis> 0.0) ? DirichletConstants::hiddenLineColor : "black";
      const std::string polygonStrokeWidth = (dotToZAxis > 0.0) ? "1" : "2";
      s += "  <polygon fill=\"" + /*colors[nColor]*/  std::string("none") + 
         " \" stroke=\"" + color + "\" stroke-width=\"" + polygonStrokeWidth + "\" points=\"";
      for (size_t point = 0; point < ring.size(); ++point) {
         const Vector_3& v = ring[point].second;
         s += LRL_DataToSVG(xshift + scale * v[0], ", ", yshift + scale * v[1], " ");
      }
      s += "\" />\n";
   }
   else if (ring.size() == 1)
   {
      const double x = ring[0].second[0];
      const double y=ring[0].second[1];
      s += "<circle cx = \"" + LRL_ToString(xshift+scale*x) + "\" cy = \"" + 
         LRL_ToString(yshift+scale*y) + "\" r = \"2\" stroke = \"blue\" stroke-width = \"1\" fill = \"blue\" />\n";
   }
   return s;
}

ANGLELIST RotateOneFace( const Matrix_3x3& m, const ANGLELIST& input) {
   ANGLELIST list(input);
   {
      for (size_t point = 0; point < list.size(); ++point)
         list[point].second = m * (list[point].second);
   }
   return list;
}

ANGLESFORFACES RotateAllFaces(const Matrix_3x3& m, const ANGLESFORFACES& input) {
   ANGLESFORFACES output(input);
   for (size_t face = 0; face < output.size(); ++face) {
      output[face] = RotateOneFace(m, output[face]);
   }
   return output;
}

std::vector<Vector_3> CentersOfMassForAllFaces(const ANGLESFORFACES& list) {
   std::vector<Vector_3> vcm;
   if (list.size() > 3) {
      for (size_t face = 0; face < list.size(); ++face)
         vcm.push_back(CenterOfMassForOneFace(list[face]));
   }
   return vcm;
}

std::vector<double> Z_Angles(const std::vector<Vector_3>& vcm) {
   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);
   std::vector<double> output;
   for (size_t face = 0; face < vcm.size(); ++face) {
      const double angle = degreesPerRad * Vector_3::Angle(Vector_3(0, 0, 10000.0), Vector_3(0, 0, 0), vcm[face]);
      output.push_back(angle);
   }
   return output;
}

ANGLESFORFACES Z_SortAllFaces(const ANGLESFORFACES& newRinged) {
   ANGLESFORFACES sorted(newRinged);
   const std::vector<Vector_3> vcm = CentersOfMassForAllFaces(newRinged);
   std::vector<double> vAngles = Z_Angles(vcm);

   for (size_t face = 0; face < sorted.size() - 1; ++face) {
      for (size_t k = face+1; k < sorted.size(); ++k) {
         if (vAngles[face] < vAngles[k]) {
            std::swap(vAngles[face], vAngles[k]);
            std::swap(sorted[face], sorted[k]);
         }
      }
   }

   //ANGLESFORFACES output;
   //for (size_t face = 0; face < vAngles.size(); ++face) {
   //   if (vAngles[face] < 90.0) output.push_back(sorted[face]);
   //}
   return sorted;
}

ANGLESFORFACES RotateObject(const Matrix_3x3& m, const ANGLESFORFACES& ring) {
   ANGLESFORFACES list;
   for (size_t face = 0; face < ring.size(); ++face) {
      list.push_back(RotateOneFace(m, ring[face]));
   }
   return list;
}

std::vector<ANGLESFORFACES> CreateSeriesOfImages(const ANGLESFORFACES& inputRingsOgFace, 
   const size_t nImages, const Matrix_3x3& initialRotation, const Matrix_3x3& rotationBetween) {
   std::vector<ANGLESFORFACES> vout;
   const ANGLESFORFACES oneImage(RotateObject(initialRotation, inputRingsOgFace));
   Matrix_3x3 m2(1,0,0,  0,1,0,  0,0,1);
   for (size_t face = 0; face < nImages; ++face) {
      vout.push_back(RotateObject(m2, oneImage));
      m2 *= rotationBetween;
   }
   return vout;
}

std::vector<std::string> DrawDirichletRings( const ANGLESFORFACES& newRinged) {
   const ANGLESFORFACES sorted = Z_SortAllFaces(newRinged);

   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);

   std::vector<std::string> vs;
   const Matrix_3x3 m1 = Vector_3(1, 0, 0).Rotmat(20.0/degreesPerRad);
   const Matrix_3x3 m2 = Vector_3(0, 1, 0).Rotmat(15.0/degreesPerRad);
   Matrix_3x3 m(1, 0, 0, 0, 1, 0, 0, 0, 1);

   double maxCoord = -DBL_MAX;
   const ANGLESFORFACES faces(RotateAllFaces(m1, sorted));
   for (size_t face = 0; face < faces.size(); ++face) {
      const ANGLELIST& thisFace = faces[face];
      for (size_t face = 0; face < thisFace.size(); ++face) {
         const ANGLE_COORDS& thisVertex = thisFace[face];  //this needs to move out of here so scale doesn't change with rotation
         maxCoord = std::max(maxCoord, thisVertex.second[0]);
         maxCoord = std::max(maxCoord, thisVertex.second[1]);
         maxCoord = std::max(maxCoord, thisVertex.second[2]);
      }
   }

   const double scale = DirichletConstants::imageHeightPx / maxCoord;
   for (size_t cycle = 0; cycle < 1; ++cycle) {
      const ANGLESFORFACES faces(RotateAllFaces(m, sorted));
      m = m2 * m;
      for (size_t pass = 0; pass < 2; ++pass) {

         for (size_t face = 0; face < faces.size(); ++face) {
            const Vector_3 cm = CenterOfMassForOneFace(faces[face]);
            const double dotToZAxis = cm.Dot(Vector_3(0, 0, 1));
            if ((pass == 0 && dotToZAxis > 0.0) ||( pass == 1 && dotToZAxis <= 0.0))
               vs.push_back(DrawOneDirichletRing(scale, faces[face], face));
         }
      }
   }
   return vs;
}

std::string OutputSVG(const std::string& s, const Cell_Faces& cell, const std::string& addedInfo) {
   const std::string stringCell = 
      LRL_ToString( "\nReduced Cell ", LRL_Cell_Degrees(cell.GetCell()), "\n  S6  ", S6(cell.GetCell()), "\n");
   std::vector<std::string> scell;
   scell.push_back(stringCell);
   scell.push_back(addedInfo);
   scell.push_back(DirichletConstants::note);



   const std::string header = ImageHeader(DirichletConstants::canvas_x_size, DirichletConstants::canvas_y_size) + "\n";
   std::vector<std::string> footer = ImageFooter(scell);

   std::string output(header);
   output += s;
   for (size_t face = 0; face < footer.size(); ++face) {
      output += footer[face];
   }
   std::cout << output << std::endl;
   const int i19191 = 19191;
   return output;
}

std::vector<std::string> DrawSeriesOfObjects(const std::vector<ANGLESFORFACES>& series) {
   std::vector<std::string> s;

   std::vector<std::string> vs;
   for (size_t face = 0; face < series.size(); ++face) {
      vs.push_back(ConcatanateStrings(DrawDirichletRings(series[face])));
   }
   return vs;
}

std::vector<std::string> MadeStereo(const std::vector<std::string>& vsin) {
   int xplace = 50;
   int xdelta = 200;
   const int yPlace = 300;

   std::vector<std::string> vsout;
   for (size_t face = 0; face < vsin.size(); ++face) {
      const std::vector<std::string> vs = PlaceSubPicture(xplace, yPlace, vsin[face]); // origin shift
      vsout.push_back(ConcatanateStrings(vs));
      xplace += xdelta;
   }
   return vsout;
}

const int maxLatticeLimit = 1;

std::vector<Vector_3> CreateVectorOfLatticePoints(const Matrix_3x3& m) {
   std::vector<Vector_3> v;
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

ANGLESFORFACES CreateMarkersForCmOfFaces(const ANGLESFORFACES& list) {
   ANGLESFORFACES output;
   const std::vector<Vector_3> vcm = CentersOfMassForAllFaces(list);
   for (size_t i = 0; i < vcm.size(); ++i) {
      const ANGLE_COORDS coord = std::make_pair(-19193.0, vcm[i]);
      ANGLELIST angl;
      angl.push_back(coord);
      output.push_back(angl);
   }
   return output;
}

ANGLELIST CreateOriginMarker1(const double scale) {
   ANGLELIST output;
   const std::vector<Vector_3> vertices = {
      Vector_3(0,0,0), Vector_3(1,0,0),Vector_3(0,1,0),Vector_3(-1,0,0),Vector_3(0,-1,0),Vector_3(1,0,0), Vector_3(0,0,0),
      Vector_3(0,0,1),Vector_3(1,0,0),Vector_3(0,0,-1),Vector_3(-1,0,0),Vector_3(0,0,1) };

   for (size_t i = 0; i < vertices.size(); ++i) {
      output.push_back(std::make_pair(-19192.0, vertices[i]));
   }
   return output;
}

ANGLELIST CreateOriginMarker2(const double scale) {
   ANGLELIST output;
   output.push_back(std::make_pair(-19192.0, Vector_3(0, 0, 0)));
   return output;
}

CNearTree<Vector_3> CreateTreeOfLatticePoints(const Matrix_3x3& m) {
   const std::vector<Vector_3> v = CreateVectorOfLatticePoints(m);
   return v;
}

Vector_3 RecoverIndicesOfOneFace(const Matrix_3x3& minv, const ANGLELIST& list) {
   const Vector_3 cm = CenterOfMassForOneFace(list);
   return minv * cm;
}

std::vector<Vector_3> RecoverIndicesOfFaces(const Matrix_3x3& m_cart, const ANGLESFORFACES& newRinged) {
   const Matrix_3x3 minv(m_cart.Inver());
   std::vector<Vector_3> vout;
   for (size_t i = 0; i < newRinged.size(); ++i) {
      Vector_3 v = (2.0 * RecoverIndicesOfOneFace(minv, newRinged[i]));
      for (size_t i = 0; i < 3; ++i) v[i] = (abs(v[i]) < 1.0e-6) ? 0.0 : v[i];
      for (size_t i = 0; i < 3; ++i) v[i] = (abs(abs(v[i])-1) < 1.0e-6) ? round(v[i]) : v[i];
      vout.push_back(v);
   }
   return vout;
}

std::string IndicesAsString(const std::vector<Vector_3>& v) {
   std::string s;
   for (size_t i = 0; i < v.size(); ++i) {
      s += "Face # " + LRL_ToString(i + 1) + "    " +
         LRL_ToString(v[i][0]) + " " + LRL_ToString(v[i][1]) + " " + LRL_ToString(v[i][2]) + "\n";
   }
   return s;
}

void PrintIndices(const std::vector<Vector_3>& v) {
   for (size_t i = 0; i < v.size(); ++i) {
      std::cout << "Face # " << i + 1 << "    " << v[i][0] << " " << v[i][1] << " " << v[i][2] << std::endl;
   }
}

double AreaOfOneFace(const ANGLELIST& face) {
   const Vector_3 cm = CenterOfMassForOneFace(face);

   double totalArea = 0.0;
   for (size_t i = 0; i < face.size(); ++i)
   {
      const Vector_3 v1 = face[i].second;
      const Vector_3 v2 = (i==face.size()-1) ? face[0].second : face[i + 1].second;
      const Vector_3 v3 = v2-v1;

      const double side1 = (v1 - cm).norm();
      const double side2 = (v2 - cm).norm();
      const double side3 = v3.Norm();

      const double area = TriangleAreaFromSides(side1, side2, side3); // Heron's formula for 3 sides
      totalArea += area;
   }
   return totalArea;
}

std::string EdgeLengths(const ANGLELIST& list) {
   std::vector<Vector_3> vin;
   for (size_t i = 0; i < list.size(); ++i) {
      vin.push_back(list[i].second);
   }
   std::vector<Vector_3> v(CircularizeVector(vin));
   std::ostringstream ostr;
   for (size_t i = 0; i < v.size()-1; ++i) {
      const double d = (v[i] - v[i + 1]).Norm();
      ostr << d << " ";
   }
   return ostr.str();
}

double CleanNearZero(const double d) {
   return (abs(d) < 1.0E-6) ? 0 : d;
}

std::string OneFaceRecord(const ANGLELIST& face, const Vector_3& index) {
   const double area = AreaOfOneFace(face);
   const Vector_3 cm = CenterOfMassForOneFace(face);
   std::ostringstream ostr;
   //ostr << std::setw(2) << std::setfill('0') << std::hex << r << g << b;
   ostr << "area= " << area << "   " << " index " << index[0] << " " << index[1] << " " << index[2] <<
      "  Center of mass " <<
      CleanNearZero(cm[0]) << " " << CleanNearZero(cm[1]) << " " << CleanNearZero(cm[2]) << std::endl;
   ostr << "   Edge lengths " << EdgeLengths(face) << std::endl;
   return (ostr.str());
}

void PrintOneFaceRecord(const ANGLELIST& face, const Vector_3& index) {
   std::cout << OneFaceRecord(face, index);
}

std::string OutputOneFace(const size_t index, const size_t vertices, const std::string& faceRecord) {
   std::ostringstream ostr;
   ostr << "FACE  " << index + 1 << " vertex count= " << vertices << "   " << faceRecord;
   return ostr.str();
}

std::string FaceRecords(const ANGLESFORFACES& rings, const std::vector<Vector_3>& indices) {
   std::ostringstream ostr;
   for (size_t faceIndex = 0; faceIndex < rings.size()/2; ++faceIndex) {
      const ANGLELIST face1 = rings[faceIndex];
      const ANGLELIST face2 = rings[rings.size() - faceIndex - 1];
      const size_t vertexCount1 = face1.size();
      const size_t vertexCount2 = face2.size();
      const std::string record1 = OneFaceRecord(face1, indices[faceIndex]);
      const std::string record2 = OneFaceRecord(face2, indices[rings.size() - faceIndex - 1]);
      const std::string testString1 = OutputOneFace(faceIndex, vertexCount1, record1);
      const std::string testString2 = OutputOneFace(rings.size() - faceIndex-1, vertexCount2, record2);
      ostr << testString1.c_str();
      ostr << testString2.c_str();
      ostr << std::endl;
   }
   ostr << std::endl;
   return ostr.str();
}

void PrintFaceRecords(const ANGLESFORFACES& rings, const std::vector<Vector_3>& indices) {
   std::cout << FaceRecords(rings, indices) << std::endl;
}

int main() {

   LRL_ReadLatticeData rld;
   rld.CellReader("p 10 10 10  90 90 90");
   ReadGlobalData();


   const std::vector<std::string> constants = DirichletConstants::cellData;

   Dirichlet dc(Dirichlet::ReadAndReduceCell());
   const std::vector<Dirichlet_Intersections> filteredIntersections = dc.GenerateIntersectionList();
   const Dirichlet_Faces faces(filteredIntersections);
   const Dirichlet_Faces filteredFaces(faces);
   dc.GetCell();

   //=========================================================================================================

   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);

   const LRL_Cell reducedCell = dc.GetCell();
   const Matrix_3x3 cart(reducedCell.Cart());
   const Cell_Faces cell(reducedCell, cart);
   const CNearTree<Vector_3> tree = CreateTreeOfLatticePoints(cart);

   std::vector<DirichletFace> dirichletFaces = Cell_Faces::CreateFaces(tree);
   const std::pair<POINT_LIST, std::vector<Intersection> > v_Intersections = ComputeIntersections(dirichletFaces, tree);
   //PrintRawIntersection(v_Intersections);
   const ANGLESFORFACES vvPoints = AssignPointsToFaceList(v_Intersections);

   const ANGLESFORFACES ringed = MakeRings(vvPoints, v_Intersections.second);

   const Matrix_3x3 m1 = Vector_3(1, 0, 0).Rotmat(DirichletConstants::rotateX / degreesPerRad);
   const Matrix_3x3 m2 = Vector_3(0, 1, 0).Rotmat(DirichletConstants::rotateY / degreesPerRad);
   std::vector<ANGLESFORFACES> rings = CreateSeriesOfImages(ringed, DirichletConstants::numberOfImages, m1, m2);

   const std::vector<std::string> series = DrawSeriesOfObjects(rings);
   const std::vector<std::string> stereoImages = MadeStereo(series);

   const std::vector<Vector_3> indices = RecoverIndicesOfFaces(cart, ringed);
   const std::string records = FaceRecords(ringed, indices);
   const std::string svg = OutputSVG(ConcatanateStrings(stereoImages), cell, records);

   exit(0); 
}