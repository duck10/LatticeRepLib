#include "Dirichlet.h"

#include "DirichletProgramConstants.h"
#include "D7.h"
#include "DirichletCellFaces.h"
#include "GetDate.h"
#include "LatticeConverter.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_DataToSVG.h"
#include "LRL_inverse.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
//#include "ReadDirichletGlobalData.h"
#include "SVG_Tools.h"
#include "TriangleAreaFromSides.h"
#include "LRL_Vector3.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <ctime>

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
   scell.insert(scell.end(), DirichletProgramConstants::note.begin(), DirichletProgramConstants::note.end());

   const std::string header = ImageHeader(DirichletProgramConstants::canvas_x_size, DirichletProgramConstants::canvas_y_size) + "\n";

   const int x = 100;
   const int y = DirichletProgramConstants::imageHeightPx + DirichletProgramConstants::yposition + 60;

   std::ostringstream os;
   os << "<text x = \""
      << x
      << "\" y = \""
      << y
      << "\"  font-size = \"20\" font-family = \"Arial, Helvetica, sans-serif\" >LRL-WEB  DIRICHLET   "
      << GetDate()
      << " </text>\n";

   std::vector<std::string> footer = ImageFooter(scell);
   const std::string output = header + os.str()
      + LRL_StringTools::ConcatanateStrings(stereoImages)
      + os.str()
      + LRL_StringTools::ConcatanateStrings(footer);

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

Vector_3 DirichletCell::CenterOfMassForOneFace(const ANGLELIST& list) {
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
   const Vector_3 centerOfMass = DirichletCell::CenterOfMassForOneFace(list);
   ANGLELIST coordsAtCenterOfMass;
   const Vector_3 cm = DirichletCell::CenterOfMassForOneFace(list);

   for (size_t face = 0; face < list.size(); ++face) {
      coordsAtCenterOfMass.push_back(ANGLE_COORDS(-19192.0, list[face].second - cm));
   }
   return coordsAtCenterOfMass;
}

static ANGLELIST SortAnglesForOneFace(const ANGLELIST& toSort) {
   // bubble sort the result since Vector_3 does not have operator<
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
      const Vector_3& pos1 = anglelist[face - 1].second;
      const Vector_3& pos2 = anglelist[face].second;
      if ((pos1 - pos2).Norm() < 1.0E-4) continue;
      if (abs(angle2 - angle1) > 1.0E-3 && angle2 < 360.0 + initialAngle - 1.0E-3) {
         intermediateOutput.push_back(anglelist[face]);
      }
   }

   const Vector_3 centerOfMass = DirichletCell::CenterOfMassForOneFace(intermediateOutput);
   const double distanceFromOrigin = (centerOfMass - intermediateOutput[0].second).Norm();
   return (intermediateOutput.size() > 3) ? intermediateOutput : ANGLELIST();
}

Vector_3 CenterOfMassForObject(const ANGLESFORFACES& list) {
   Vector_3 cm(0, 0, 0);
   for (size_t face = 0; face < list.size(); ++face) {
      cm += DirichletCell::CenterOfMassForOneFace(list[face]);
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
      const ANGLELIST restoredWithAngles = ComputeAnglesForOneFace(moved, 
         DirichletCell::CenterOfMassForOneFace(faces_in[face]));
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
   for (int i1 = 0; i1 < n - 2; ++i1) {
      for (int i2 = i1 + 1; i2 < n - 1; ++i2) {
         if (i1 == i2) continue;
         for (int i3 = i2 + 1; i3 < n; ++i3) {
            if (i1 == i3 || i2 == i3) continue;
            Intersection intersection = Intersection::FindIntersectionForThreeFaces(dirichletFaces[i1], dirichletFaces[i2], dirichletFaces[i3]);

            const double d = intersection.GetCoord()[0];
            //std::cout << i1 << " " << i2 << " " << i3 << "    "<< intersection.GetCoord() << "    " << d << std::endl;
            const bool b1 = !(d <= 0.0);
            const bool b2 = !(d > 0.0);
            const bool b_both = b1 && b2; // for test for indefinite value

            if (intersection.GetCoord()[0] != DBL_MAX && (!b_both)) {
               const auto it = tree.NearestNeighbor(DBL_MAX, intersection.GetCoord());

               double distanceFromOrigin = DBL_MAX;
               double nearestFoundDistance = DBL_MAX;
               if (it != tree.end()) {
                  const size_t pos = it.get_position();
                  distanceFromOrigin = (intersection.GetCoord() - (*it0)).Norm();
                  nearestFoundDistance = (intersection.GetCoord() - (*it)).Norm();
               }
               //if (intersection.IsValid() ) {
               const bool bFindOrigin = it == it0;
               if (intersection.IsValid() && (bFindOrigin || abs(distanceFromOrigin - nearestFoundDistance < 1.0E-4))) {
                  intersection.SetPlaneList(i1, i2, i3);
                  vvindex.push_back({ size_t(i1), size_t(i2), size_t(i3) });
                  vIntersections.push_back(intersection);
                  dirichletFaces[i1].SetIndicesOfIntersection(i1, i2, i3);
                  const int i19191 = 19191;
               }
            }
         }
      }
   }
   //std::cout << "intersection count " << vvindex.size() << std::endl;
   return std::make_pair(vvindex, vIntersections);
}

static std::vector<Vector_3> CreateVectorOfLatticePoints(const Matrix_3x3& m) {
   std::vector<Vector_3> v;
   const int maxLatticeLimit = DirichletProgramConstants::latticeLimit;
   for (int face = -maxLatticeLimit; face <= maxLatticeLimit; ++face) {
      for (int j = -maxLatticeLimit; j <= maxLatticeLimit; ++j) {
         for (int k = -maxLatticeLimit; k <= maxLatticeLimit; ++k) {
            //if ( face==j==k==0) continue;
            const double di = face;
            const double dj = j;
            const double dk = k;
            Vector_3 v3 = m * Vector_3(di, dj, dk);
            for (size_t pos = 0; pos < 3; ++pos)
               if (abs(v3[pos]) < 1.0E-8) v3[pos] = 0.0;
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

Vector_3 DirichletCell::RecoverIndicesOfOneFace(const Matrix_3x3& minv, const ANGLELIST& list) {
   const Vector_3 cm = DirichletCell::CenterOfMassForOneFace(list);;
   return minv * cm;
}

std::vector<Vector_3> DirichletCell::RecoverIndicesOfFaces(const Matrix_3x3& m_cart, const ANGLESFORFACES& newRinged) {
   const Matrix_3x3 minv(m_cart.Inver());
   std::vector<Vector_3> vout;
   for (size_t ring = 0; ring < newRinged.size(); ++ring) {
      Vector_3 v = (2.0 * DirichletCell::RecoverIndicesOfOneFace(minv, newRinged[ring]));
      for (size_t i = 0; i < 3; ++i) v[i] = (abs(v[i]) < 1.0e-6) ? 0.0 : v[i];
      for (size_t i = 0; i < 3; ++i) v[i] = (abs(abs(v[i]) - 1) < 1.0e-6) ? round(v[i]) : v[i];
      vout.push_back(v);
   }
   return vout;
}

double DirichletCell::AreaOfOneFace(const ANGLELIST& face) {
   const Vector_3 cm = DirichletCell::CenterOfMassForOneFace(face);

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

DirichletCell::DirichletCell(const std::string& strCellAndLattice)
   : m_strCell(strCellAndLattice)
{
   throw("this function needs to Niggli reduced the cell, and it shouldn't be doing this call");
   LRL_ReadLatticeData rcd;
   rcd.CellReader(strCellAndLattice);
   ProcessInputCell(rcd.GetLattice(), rcd.GetCell()); // LCA ERROR !!!! supposed to be reduced cell
}

//typedef std::pair<double, Vector_3 > ANGLE_COORDS;
//typedef std::vector< ANGLE_COORDS > ANGLELIST;
//typedef std::vector<ANGLELIST> ANGLESFORFACES;

std::ostream& operator<< (std::ostream& o, const DirichletCell& dc) {
   const auto xxx = dc.GetAnglesForFaces();
   for (size_t i = 0; i < xxx.size(); ++i)
      for (size_t kkk = 0; kkk < xxx[i].size(); ++kkk)
      {
         o << xxx[i][kkk].first << std::endl;
         o << xxx[i][kkk].second << std::endl << std::endl;
      }
   return o;
}

//DirichletCell::DirichletCell(const LRL_ReadLatticeData& rld)
//{
//   m_cell = rld.GetCell();
//   if (m_strCell.empty()) m_strCell = rld.GetStrCell();
//   ////-------------reduce cell
//   const std::string lattice = rld.GetLattice();
//   const LRL_Cell cell = rld.GetCell();
//   if (DirichletProgramConstants::sellingNiggli == "SELLING")
//      m_reducedCell = LatticeConverter().SellingReduceCell(lattice, cell);
//   else
//      m_reducedCell = LatticeConverter().NiggliReduceCell(lattice, cell);
//
//   if (m_reducedCell.IsValid()) {
//      ProcessInputCell("P", m_reducedCell);
//   }
//   else {
//      std::cout << "In DirichletCell, LatticeConverter().NiggliReducedCell returned " <<
//         m_reducedCell << std::endl;
//      std::cout << "input cell was   " << cell << std::endl;
//      std::cout << "In DirichletCell, LatticeConverter().NiggliReducedCell returned " <<
//         LRL_Cell_Degrees(m_reducedCell) << std::endl;
//      std::cout << std::setw(11) << std::setprecision(6) << "input cell was   " << LRL_Cell_Degrees(cell) << std::endl;
//   }
//}

DirichletCell::DirichletCell(const LatticeCell& lc)
{
   m_cell = lc.getCell();
   if (m_strCell.empty()) m_strCell = lc.GetInput();
   ////-------------reduce cell
   const std::string lattice = lc.getLatticeType();
   const LRL_Cell cell = lc.getCell();
   if (DirichletProgramConstants::sellingNiggli == "SELLING")
      m_reducedCell = LatticeConverter().SellingReduceCell(lattice, cell);
   else
      m_reducedCell = LatticeConverter().NiggliReduceCell(lattice, cell);

   if (m_reducedCell.IsValid()) {
      ProcessInputCell("P", m_reducedCell);
   }
   else {
      std::cout << "In DirichletCell, LatticeConverter().NiggliReducedCell returned " <<
         m_reducedCell << std::endl;
      std::cout << "input cell was   " << cell << std::endl;
      std::cout << "In DirichletCell, LatticeConverter().NiggliReducedCell returned " <<
         LRL_Cell_Degrees(m_reducedCell) << std::endl;
      std::cout << std::setw(11) << std::setprecision(6) << "input cell was   " << LRL_Cell_Degrees(cell) << std::endl;
   }
}


std::vector<DirichletFace> SortTheFaces(const std::vector<DirichletFace>& dirichletFaces) {
   std::vector<DirichletFace> v(dirichletFaces);
   for (size_t i = 0; i < v.size(); ++i) {
      for (size_t kk = 0; kk < v.size() - 1; ++kk) {
         if (v[kk].GetDistance() > v[kk + 1].GetDistance()) {
            std::swap(v[kk], v[kk + 1]);
         }
      }
   }
   return v;
}

std::vector<DirichletFace> RemoveOppositeVectors(const Matrix_3x3& inv, const std::vector<DirichletFace>& dirichletFaces) {
   std::vector<DirichletFace> v(1, dirichletFaces[0]);
   for (size_t i = 0; i < dirichletFaces.size() - 1; ++i) {
      for (size_t kk = 0; kk < dirichletFaces.size(); ++kk) {
         const Vector_3 v1 = inv * dirichletFaces[i].GetCoord();
         const Vector_3 v2 = inv * dirichletFaces[kk].GetCoord();
         const Vector_3 vsum = v1 + v2;
         const double diff = vsum.norm();
         if (diff > 1.0E-6) {
            v.push_back(dirichletFaces[i]);
            break;
         }
      }
   }
   return v;
}

static double CleanNearZero(const double d) {
   const double dret = (abs(d) < 1.0E-6) ? 0.0 : d;
   return dret;
}

static Vector_3 CleanNearZero(const Vector_3& vin) {
   const double d1 = vin[0];
   const double d2 = vin[1];
   const double d3 = vin[2];
   return Vector_3(CleanNearZero(d1), CleanNearZero(d2), CleanNearZero(d3));
}

void ListFaces(const Matrix_3x3& m, const std::vector<DirichletFace>& dirichletFaces) {
   const Matrix_3x3 invCart = m.Inver();
   std::vector<DirichletFace> v = RemoveOppositeVectors(invCart, SortTheFaces(dirichletFaces));

   std::cout << "Dirichlet faces" << std::endl;
   for (size_t i = 0; i < v.size(); ++i) {
      Vector_3 indices = invCart * v[i].GetCoord();
      indices = Vector_3(CleanNearZero(indices[0]), CleanNearZero(indices[1]), CleanNearZero(indices[2]));
      std::cout << i+1 << "   " << v[i].GetDistance() << " " << indices << std::endl;
   }
   std::cout << "end of Dirichlet faces" << std::endl;
}

//typedef std::pair<double, Vector_3 > ANGLE_COORDS;
//typedef std::vector< ANGLE_COORDS > ANGLELIST;
//typedef std::vector<ANGLELIST> ANGLESFORFACES;

std::vector<Vector_3> FindUniqureVertices(const ANGLESFORFACES& AFF) {
   std::vector<Vector_3> unique;

   CNearTree<Vector_3> tree;
   for (size_t aff = 0; aff < AFF.size(); ++aff) {
      const ANGLELIST& anglelist = AFF[aff];
      for (size_t al = 0; al < anglelist.size(); ++al) {
         const Vector_3& coords = anglelist[al].second;
         if (tree.NearestNeighbor(1.03-2, coords) == tree.end()) {
            unique.emplace_back(coords);
            tree.insert(coords);
            break;
         }
      }
   }
   return unique;
}

void DirichletCell::ProcessInputCell(const std::string& lattice, const LRL_Cell& reducedCell) {
   m_reducedCell = reducedCell;
   ////-------------cell create faces
   m_cart = m_reducedCell.Cart();
   m_cellFaces = Cell_Faces(m_reducedCell, m_cart);

   //-------------create Dirichlet faces
   const CNearTree<Vector_3> tree = CreateTreeOfLatticePoints(m_cart);
   std::vector<DirichletFace> dirichletFaces = Cell_Faces::CreateFaces(tree);
   const std::pair<POINT_LIST, std::vector<Intersection> > v_Intersections = ComputeIntersections(tree);







   const ANGLESFORFACES vvPoints = AssignPointsToFaceList(v_Intersections);

   m_facesWithVertices = MakeRings(vvPoints, v_Intersections.second);

   m_vertices = FindUniqureVertices(m_facesWithVertices);
   const std::vector<Vector_3> indices = RecoverIndicesOfFaces(GetCartesianMatrix(), m_facesWithVertices);
   m_indices = ConvertAllVectorIndicesToInt(indices);
   m_strIndices = ConvertAllVectorIndicesToString(indices);

   for (size_t faceIndex = 0; faceIndex < m_facesWithVertices.size(); ++faceIndex) {
      double area = AreaOfOneFace(m_facesWithVertices[faceIndex]);
      m_areas.push_back((area < 1.0E-4) ? 0 : area);
   }
}

std::vector<int> DirichletCell::ConvertVectorIndicesToInt(const Vector_3& v) {
   return{ int(v[0]), int(v[1]), int(v[2]) };
}

std::vector<std::string> DirichletCell::ConvertAllVectorIndicesToString(const std::vector<Vector_3>& v) {
   std::vector<std::string> vout;
   for (size_t i = 0; i < v.size(); ++i) {
      vout.push_back(ConvertVectorIndicesToString(v[i]));
   }
   return vout;
}

std::string DirichletCell::ConvertVectorIndicesToString(const Vector_3& v) {
   std::string s = LRL_ToString(v[0], v[1], v[2]);
   return s;
}

std::vector<std::vector<int> > DirichletCell::ConvertAllVectorIndicesToInt(const std::vector<Vector_3>& v) {
   std::vector<std::vector<int> > vout;
   for (size_t i = 0; i < v.size(); ++i)
      vout.push_back(ConvertVectorIndicesToInt(v[i]));
   return vout;
}

template <typename T>
static std::vector<T> CircularizeVector(const std::vector<T>& vin) {
   std::vector<T> v(vin);
   v.push_back(vin[0]);
   return v;
}

template<typename T>
static int roundNumber(T num)
{
   return num < 0 ? int(num - 0.5) : int(num + 0.5);
}

Vector_3 Dirichlet::round(const Vector_3& vin) {
   Vector_3 vout;
   vout[0] = roundNumber(vin[0]);
   vout[1] = roundNumber(vin[1]);
   vout[2] = roundNumber(vin[2]);
   return vout;
}

std::string Dirichlet::DrawOneDirichletRing(const double scale, const ANGLELIST& ring, const size_t nColor) {
   const double xshift = 30;
   const double yshift = 30;
   ANGLELIST colorTest;
   std::string s;
   const Vector_3 cm = DirichletCell::CenterOfMassForOneFace(ring);

   if (ring.size() >= 3) {
      const std::string color = (cm[2] < 0.0) ? "black" : "black";
      //const std::string color = "black";
      const std::string polygonStrokeWidth = (cm[2] < 0.0) ? "2" : "2";
      s += "  <polygon fill=\"" + /*colors[nColor]*/  std::string(DirichletProgramConstants::faceColor) +
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
      const double y = ring[0].second[1];
      s += "<circle cx = \"" + LRL_ToString(xshift + scale * x) + "\" cy = \"" +
         LRL_ToString(yshift + scale * y) + "\" r = \"2\" stroke = \"blue\" stroke-width = \"1\" fill = \"blue\" />\n";
   }
   return s;
}

ANGLELIST Dirichlet::RotateOneFace(const Matrix_3x3& m, const ANGLELIST& input) {
   ANGLELIST list(input);
   {
      for (size_t point = 0; point < list.size(); ++point)
         list[point].second = m * (list[point].second);
   }
   return list;
}

ANGLESFORFACES Dirichlet::RotateAllFaces(const Matrix_3x3& m, const ANGLESFORFACES& input) {
   ANGLESFORFACES output(input);
   for (size_t face = 0; face < output.size(); ++face) {
      output[face] = RotateOneFace(m, output[face]);
   }
   return output;
}

std::vector<Vector_3> Dirichlet::CentersOfMassForAllFaces(const ANGLESFORFACES& list) {
   std::vector<Vector_3> vcm;
   if (list.size() > 3) {
      for (size_t face = 0; face < list.size(); ++face)
         vcm.push_back(DirichletCell::CenterOfMassForOneFace(list[face]));
   }
   return vcm;
}

std::vector<double> Dirichlet::Z_Angles(const std::vector<Vector_3>& vcm) {
   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);
   std::vector<double> output;
   for (size_t face = 0; face < vcm.size(); ++face) {
      const double angle = degreesPerRad * Vector_3::Angle(Vector_3(0, 0, 10000.0), Vector_3(0, 0, 0), vcm[face]);
      output.push_back(angle);
   }
   return output;
}

ANGLESFORFACES Dirichlet::Z_SortAllFaces(const ANGLESFORFACES& newRinged) {
   ANGLESFORFACES sorted(newRinged);
   const std::vector<Vector_3> vcm = CentersOfMassForAllFaces(newRinged);
   std::vector<double> vAngles = Z_Angles(vcm);

   for (size_t face = 0; face < sorted.size() - 1; ++face) {
      for (size_t k = face + 1; k < sorted.size(); ++k) {
         if (vAngles[face] < vAngles[k]) {
            std::swap(vAngles[face], vAngles[k]);
            std::swap(sorted[face], sorted[k]);
         }
      }
   }
   return sorted;
}

ANGLESFORFACES Dirichlet::RotateObject(const Matrix_3x3& m, const ANGLESFORFACES& ring) {
   ANGLESFORFACES list;
   for (size_t face = 0; face < ring.size(); ++face) {
      list.push_back(RotateOneFace(m, ring[face]));
   }
   return list;
}

std::vector<ANGLESFORFACES> Dirichlet::CreateSeriesOfImages(const ANGLESFORFACES& inputRingsOfFace,
   const size_t nImages, const Matrix_3x3& initialRotation, const Matrix_3x3& rotationBetween) {
   std::vector<ANGLESFORFACES> vout;
   const ANGLESFORFACES oneImage(RotateObject(initialRotation, inputRingsOfFace));
   Matrix_3x3 m2(1, 0, 0, 0, 1, 0, 0, 0, 1);
   for (size_t face = 0; face < nImages; ++face) {
      vout.push_back(RotateObject(m2, oneImage));
      m2 *= rotationBetween;
   }
   return vout;
}

std::vector<Vector_3> Dirichlet::GetVerticesFromFaces(const ANGLESFORFACES& faces) {
   CNearTree<Vector_3> tree;
   double maxCoord = -DBL_MAX;
   std::vector<Vector_3> unique;

   for (size_t face = 0; face < faces.size(); ++face) {
      const ANGLELIST& thisFace = faces[face];
      for (size_t face = 0; face < thisFace.size(); ++face) {
         const ANGLE_COORDS thisVertex = thisFace[face];  //this needs to move out of here so scale doesn't change with rotation
         maxCoord = std::max(maxCoord, thisVertex.second[0]);
         maxCoord = std::max(maxCoord, thisVertex.second[1]);
         maxCoord = std::max(maxCoord, thisVertex.second[2]);
         if (tree.NearestNeighbor(1.0E-4, thisVertex.second) == tree.end()) {
            tree.insert(thisVertex.second);
            unique.emplace_back(thisVertex.second);
         }
      }
   }
   return unique;
}

std::vector<std::string> Dirichlet::DrawDirichletRings(const ANGLESFORFACES& newRinged) {
   const ANGLESFORFACES sorted = Z_SortAllFaces(newRinged);

   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);

   std::vector<std::string> vs;
   const Matrix_3x3 m1 = Vector_3(1, 0, 0).Rotmat(20.0 / degreesPerRad);
   const Matrix_3x3 m2 = Vector_3(0, 1, 0).Rotmat(15.0 / degreesPerRad);
   Matrix_3x3 m(1, 0, 0, 0, 1, 0, 0, 0, 1);

   double maxCoord = -DBL_MAX;
   const ANGLESFORFACES faces(RotateAllFaces(m1, sorted));

   for (size_t face = 0; face < faces.size(); ++face) {
      const ANGLELIST& thisFace = faces[face];
      for (size_t face = 0; face < thisFace.size(); ++face) {
         const ANGLE_COORDS thisVertex = thisFace[face];  //this needs to move out of here so scale doesn't change with rotation
         maxCoord = std::max(maxCoord, thisVertex.second[0]);
         maxCoord = std::max(maxCoord, thisVertex.second[1]);
         maxCoord = std::max(maxCoord, thisVertex.second[2]);
      }
   }

   const double scale = DirichletProgramConstants::imageHeightPx * DirichletProgramConstants::scale / maxCoord;
   for (size_t cycle = 0; cycle < 1; ++cycle) {
      const ANGLESFORFACES faces(RotateAllFaces(m, sorted));
      m = m2 * m;

      std::vector<std::pair<double, size_t> >zandindex;
      for (size_t thisFace = 0; thisFace < faces.size(); ++thisFace) {
         const Vector_3 cm = DirichletCell::CenterOfMassForOneFace(faces[thisFace]);
         zandindex.emplace_back(std::make_pair(cm[3], thisFace));
      }

      std::sort(zandindex.begin(), zandindex.end(), std::greater<>());

      for (size_t i = 0; i < zandindex.size(); ++i) {
         vs.push_back(DrawOneDirichletRing(scale, faces[zandindex[i].second], zandindex[i].second));

      }
   }
   return vs;
}

std::vector<std::string> Dirichlet::DrawSeriesOfObjects(const std::vector<ANGLESFORFACES>& series) {
   std::vector<std::string> s;

   std::vector<std::string> vs;
   for (size_t face = 0; face < series.size(); ++face) {
      vs.push_back(LRL_StringTools::ConcatanateStrings(DrawDirichletRings(series[face])));
   }
   return vs;
}

std::vector<std::string> Dirichlet::MadeStereo(const std::vector<std::string>& vsin) {
   int xplace = 50;
   int xdelta = 200;

   std::vector<std::string> vsout;
   const int yPlace = DirichletProgramConstants::yposition;
   for (size_t face = 0; face < vsin.size(); ++face) {
      const std::vector<std::string> vs = PlaceSubPicture(xplace, yPlace, vsin[face]); // origin shift
      vsout.push_back(LRL_StringTools::ConcatanateStrings(vs));
      xplace += xdelta;
   }
   return vsout;
}

std::string Dirichlet::EdgeLengths(const ANGLELIST& list) {
   std::vector<Vector_3> vin;
   for (size_t i = 0; i < list.size(); ++i) {
      vin.push_back(list[i].second);
   }
   std::vector<Vector_3> v(CircularizeVector(vin));
   std::ostringstream ostr;
   for (size_t i = 0; i < v.size() - 1; ++i) {
      const double d = (v[i] - v[i + 1]).Norm();
      ostr << d << " ";
   }
   return ostr.str();
}

double Dirichlet::CleanNearZero(const double d) {
   return (abs(d) < 1.0E-6) ? 0 : d;
}

Vector_3 Dirichlet::CleanNearZero(const Vector_3& v) {
   return { CleanNearZero(v[0]), CleanNearZero(v[1]), CleanNearZero(v[2]) };
}

std::string Dirichlet::OneFaceRecord(const ANGLELIST& face, const Vector_3& index) {
   const double area = DirichletCell::AreaOfOneFace(face);
   const Vector_3 cm = DirichletCell::CenterOfMassForOneFace(face);
   std::ostringstream ostr;
   //ostr << std::setw(2) << std::setfill('0') << std::hex << r << g << b;
   ostr << "area= " << area << "   " << " index " << index[0] << " " << index[1] << " " << index[2] <<
      "  Center of mass " <<
      CleanNearZero(cm[0]) << " " << CleanNearZero(cm[1]) << " " << CleanNearZero(cm[2]) << std::endl;
   ostr << "   Edge lengths " << EdgeLengths(face) << std::endl;
   return (ostr.str());
}

void Dirichlet::PrintOneFaceRecord(const ANGLELIST& face, const Vector_3& index) {
   std::cout << OneFaceRecord(face, index);
}

std::string Dirichlet::OutputOneFace(const size_t index, const size_t vertices, const std::string& faceRecord) {
   std::ostringstream ostr;
   ostr << "FACE  " << index + 1 << " vertex count= " << vertices << "   " << faceRecord;
   return ostr.str();
}

std::string Dirichlet::FaceRecords(const ANGLESFORFACES& rings, const std::vector<Vector_3>& indices) {
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
      double area = DirichletCell::AreaOfOneFace(rings[faceIndex]);
      if (area > 1.0E-4) {
         ++count;
         //std::cout << count << "  " << area << "  " << indices[faceIndex];
      }
   }
   //std::cout << std::endl;

   ostr << std::endl;
   return ostr.str();
}

std::string Dirichlet::CreateStereoSVGText(const DirichletCell& dc) {
   //------------- create rotated image
   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);
   const Matrix_3x3 m1 = Vector_3(1, 0, 0).Rotmat(DirichletProgramConstants::rotateX / degreesPerRad);
   const Matrix_3x3 m2 = Vector_3(0, 1, 0).Rotmat(DirichletProgramConstants::rotateStereo / degreesPerRad);

   // get the list of faces with their vertices
   const ANGLESFORFACES ringed = dc.GetAnglesForFaces();;
   //-------------create series of images
   const std::vector<ANGLESFORFACES> rings = CreateSeriesOfImages(ringed, DirichletProgramConstants::numberOfImages, m1, m2);
   const std::vector<std::string> series = DrawSeriesOfObjects(rings);
   const std::vector<std::string> stereoImages = MadeStereo(series);
   const auto vertices = GetVerticesFromFaces(ringed);
   const std::vector<Vector_3> indices = DirichletCell::RecoverIndicesOfFaces(dc.GetCartesianMatrix(), ringed);
   const std::string records = FaceRecords(ringed, indices);
   //const std::string constants = ReadCellData::GetConstantsAsString();
   const Cell_Faces cellFaces = dc.GetCellFaces();

   //std::cout << LRL_ToString(indices) << std::endl;
   //std::cout << LRL_ToString(dc.GetAreas()) << std::endl;

   const DirichletSVG dsvg(dc);
   const std::string strCell = dc.GetStrCell();
   const std::string svg = dsvg.OutputSVG(stereoImages, cellFaces,
      records );
   return svg;
}

std::string Dirichlet::HandleOneCell(const DirichletCell& dc) {
   std::string svg = CreateStereoSVGText(dc);
   return svg;
}

std::vector<std::string>Dirichlet::RetrieveCellsAsStringsFromDirichletConstants() const {
   std::vector<std::string> strCells;
   const std::vector<std::string>& cellText = DirichletProgramConstants::cellData;
   for (size_t i = 0; i < cellText.size(); ++i) {
      const std::string s = cellText[i];
      strCells.push_back(s);
   }
   return strCells;
}

std::vector<std::string> Dirichlet::GetUpwardSymmetry(const std::string& s) const {
   // per Grimmer, 2015
   if (s == "aP")  return std::vector<std::string> {"mP", "mS"};
   else if (s == "mP")  return std::vector<std::string> {"oP", "oS"};
   else if (s == "mS")  return std::vector<std::string> {"oS", "oF", "oI", "hR"};
   else if (s == "oP")  return std::vector<std::string> {"tP"};
   else if (s == "oS")  return std::vector<std::string> {"tP", "hP"};
   else if (s == "oF")  return std::vector<std::string> {"tI"};
   else if (s == "oI")  return std::vector<std::string> {"tI"};
   else if (s == "tP")  return std::vector<std::string> {"cP"};
   //else if ( s == "hP") return std::vector<std::string>();
   else if (s == "hR")  return std::vector<std::string> {"cP", "cF", "cI"};
   else if (s == "tI")  return std::vector<std::string> {"cF", "cI"};
   else { return std::vector<std::string>(); }
}

void Dirichlet::ListVertices(const DirichletCell& dc) {

   const ANGLESFORFACES ringed = dc.GetAnglesForFaces();;
   const auto vertices = GetVerticesFromFaces(ringed);

   std::cout << "; vertices" << std::endl;
   for (size_t i = 0; i < vertices.size(); ++i) {
      std::cout << i + 1 << "  " << vertices[i] << std::endl;
   }
   std::cout << std::endl;

}
