
#include <algorithm>
#include <cmath>
#include <iostream>     // std::cout, std::endl
#include <iomanip>      // std::setfill, std::setw
#include <string>
#include <utility>

#include "D7.h"
#include "Dirichlet.h"
#include "DirichletConstants.h"
#include "Faces.h"
#include "FileOperations.h"
#include "G6.h"
#include "LatticeConverter.h"
#include "LRL_CreateFileName.h"
#include "LRL_DataStreamToString.h"
#include "LRL_DataToSVG.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
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
//
//void PrintIndices(const ANGLESFORFACES& ringed, const Matrix_3x3& cart)
//{
//   const Matrix_3x3 inverseCart = cart.Inver();
//
//   std::cout << std::endl;
//   std::cout << "INDICES FOR ALL FACES " << std::endl;
//   for (size_t face = 0; face < ringed.size(); ++face) {
//      std::cout << "\nFACE " << face << "   points on face " << ringed[face].size() << std::endl;
//      const ANGLELIST v = ringed[face];
//      for (size_t k = 0; k < ringed[face].size(); ++k) {
//
//         const Vector_3 cm = CenterOfMassForOneFace(ringed[face]) / double(ringed.size());;
//         const Vector_3 facexxx0 = 2.0 * inverseCart * ringed[face][0].second;
//         const Vector_3 facexxx1 = 2.0 * inverseCart * ringed[face][1].second;
//         const Vector_3 facexxx2 = 2.0 * inverseCart * ringed[face][2].second;
//         const Vector_3 faceByIndex0 = round(facexxx0);
//         const Vector_3 faceByIndex1 = round(facexxx1);
//         const Vector_3 faceByIndex2 = round(facexxx2);
//         std::cout << v[k].first << "     " << faceByIndex0 << "  " << faceByIndex1 << "  " << faceByIndex2 << std::endl;
//      }
//   }
//}

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

//void PrintDirichletFaces(const std::vector<DirichletFace>& dirichletFaces) {
//   for (size_t face = 0; face < dirichletFaces.size(); ++face) {
//      const std::vector<size_t> indices = dirichletFaces[face].GetIndicesOfIntersection();
//      std::cout << "face " << face << "      " << indices[0] << " " << indices[1] << " " << indices[2] << std::endl;
//   }
//}

//bool Z_DepthTest(const Vector_3& v) {
//   return v[0] > 0 && v[1] > 0 && v[2] > 0;
//}
//
//bool Z_DepthTest(const ANGLELIST& ring) {
//   for (size_t face = 0; face < ring.size(); ++face) {
//      if (Z_DepthTest(ring[face].second)) return true;
//   }
//   return false;
//}

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
      s += "  <polygon fill=\"" + /*colors[nColor]*/  std::string(DirichletConstants::faceColor) + 
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

   const double scale = DirichletConstants::imageHeightPx * DirichletConstants::scale / maxCoord;
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

std::vector<std::string> DrawSeriesOfObjects(const std::vector<ANGLESFORFACES>& series) {
   std::vector<std::string> s;

   std::vector<std::string> vs;
   for (size_t face = 0; face < series.size(); ++face) {
      vs.push_back(LRL_StringTools::ConcatanateStrings(DrawDirichletRings(series[face])));
   }
   return vs;
}

std::vector<std::string> MadeStereo(const std::vector<std::string>& vsin) {
   int xplace = 50;
   int xdelta = 200;

   std::vector<std::string> vsout;
   const int yPlace = DirichletConstants::yposition;
   for (size_t face = 0; face < vsin.size(); ++face) {
      const std::vector<std::string> vs = PlaceSubPicture(xplace, yPlace, vsin[face]); // origin shift
      vsout.push_back(LRL_StringTools::ConcatanateStrings(vs));
      xplace += xdelta;
   }
   return vsout;
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
   for (size_t faceIndex = 0; faceIndex < rings.size() / 2; ++faceIndex) {
      const ANGLELIST face1 = rings[faceIndex];
      const ANGLELIST face2 = rings[rings.size() - faceIndex - 1];
      const size_t vertexCount1 = face1.size();
      const size_t vertexCount2 = face2.size();
      const std::string record1 = OneFaceRecord(face1, indices[faceIndex]);
      const std::string record2 = OneFaceRecord(face2, indices[rings.size() - faceIndex - 1]);
      const std::string testString1 = OutputOneFace(faceIndex, vertexCount1, record1);
      const std::string testString2 = OutputOneFace(rings.size() - faceIndex - 1, vertexCount2, record2);
      ostr << testString1.c_str();
      ostr << testString2.c_str();
      ostr << std::endl;
   }

   size_t count = 0;
   for (size_t faceIndex = 0; faceIndex < rings.size(); ++faceIndex) {
      double area = AreaOfOneFace(rings[faceIndex]);
      if (area > 1.0E-4) {
         ++count;
         std::cout << count << "  " << area << "  " << indices[faceIndex];
      }
   }
   std::cout << std::endl;

   ostr << std::endl;
   return ostr.str();
}

void PrintFaceRecords(const ANGLESFORFACES& rings, const std::vector<Vector_3>& indices) {
   std::cout << FaceRecords(rings, indices) << std::endl;
}

std::vector<std::vector<int> > DirichletCell::ConvertAllVectorIndicesToInt(const std::vector<Vector_3>& v) {
   std::vector<std::vector<int> > vout;
   for (size_t i = 0; i < v.size(); ++i)
      vout.push_back(ConvertVectorIndicesToInt(v[i]));
   return vout;
}
//
//std::vector<int> DirichletCell::ConvertVectorIndicesToInt(const Vector_3& v) {
//   return{ int(v[0]), int(v[1]), int(v[2]) };
//}
//
//std::vector<std::string> DirichletCell::ConvertAllVectorIndicesToString(const std::vector<Vector_3>& v) {
//   std::vector<std::string> vout;
//   for (size_t i = 0; i < v.size(); ++i) {
//      vout.push_back(ConvertVectorIndicesToString(v[i]));
//   }
//   return vout;
//}
//
//std::string DirichletCell::ConvertVectorIndicesToString(const Vector_3& v) {
//   return LRL_ToString(v);
//}

std::string HandleOneCell(const std::string& strCell) {

   DirichletCell dc(strCell);

   //------------- create rotated image
   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);
   const Matrix_3x3 m1 = Vector_3(1, 0, 0).Rotmat(DirichletConstants::rotateX / degreesPerRad);
   const Matrix_3x3 m2 = Vector_3(0, 1, 0).Rotmat(DirichletConstants::rotateStereo / degreesPerRad);

   // get the list of faces with their vertices
   const ANGLESFORFACES ringed = dc.GetAnglesForFaces();;
   //-------------create series of images
   const std::vector<ANGLESFORFACES> rings = CreateSeriesOfImages(ringed, DirichletConstants::numberOfImages, m1, m2);
   const std::vector<std::string> series = DrawSeriesOfObjects(rings);
   const std::vector<std::string> stereoImages = MadeStereo(series);

   const std::vector<Vector_3> indices = RecoverIndicesOfFaces(dc.GetCartesianMatrix(), ringed);
   const std::string records = FaceRecords(ringed, indices);
   const std::string constants = ReadGlobalData::GetConstantsAsString();
   const Cell_Faces cellFaces = dc.GetCellFaces();

   std::cout << LRL_ToString(indices) << std::endl;
   std::cout << LRL_ToString(dc.GetAreas()) << std::endl;

   const DirichletSVG dsvg(dc);
   const std::string svg = dsvg.OutputSVG(stereoImages, cellFaces,
      records + "\nGlobal Constants\n" + constants);
   return svg;
}

std::vector<std::string> RetrieveCellsAsStringsFromDirichletConstants() {
   std::vector<std::string> strCells;
   const std::vector<std::string>& cellText = DirichletConstants::cellData;
   for (size_t i = 0; i < cellText.size(); ++i) {
      const std::string s = cellText[i];
      strCells.push_back(s);
   }
   return strCells;
}

int main() {
   ReadGlobalData();

   std::vector<std::string> strCells = RetrieveCellsAsStringsFromDirichletConstants();

   const std::string filePrefix =
      LRL_ToString(LRL_CreateFileName::Create(DirichletConstants::fileNamePrefix, "", 
         DirichletConstants::timestamp));

   for ( size_t whichCell =0; whichCell<strCells.size(); ++whichCell) {
      const std::string svg = HandleOneCell(strCells[whichCell]);
      const std::string fileName = filePrefix + LRL_ToString(whichCell) + ".svg";
      if ( ! svg.empty()) FileOperations::Write(fileName, svg);
   }
   exit(0); 
}