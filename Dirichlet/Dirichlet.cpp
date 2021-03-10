#include "Dirichlet.h"

#include "DirichletConstants.h"
#include "LatticeConverter.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "TriangleAreaFromSides.h"

#include <cmath>

static const std::string letters( "V,G,D,S,P,A,B,C,I,F,R,C3,G6,S6,B4,D7,H");


LRL_Cell Dirichlet::ParseAndReduceStringCellWithLattice(const std::string& strCell) {
   return ParseAndReduceCell(std::string(1, strCell[0]), strCell.substr(2));
}

LRL_Cell Dirichlet::ParseAndReduceCell(const std::string& lattice, const std::string& strCell) {
   LRL_Cell cell;
   if ((!lattice.empty()) && (static_cast<char>(letters.find(toupper(lattice[0]))) != std::string::npos)) {

      if (DirichletConstants::sellingNiggli == "SELLING")
         cell = LatticeConverter().SellingReduceCell(lattice, LRL_Cell(strCell));
      else
         cell = LatticeConverter().NiggliReduceCell(lattice, LRL_Cell(strCell));
   }
   return cell;
}

LRL_Cell Dirichlet::ReadAndReduceCell() {
   std::string lattice;
   LRL_ReadLatticeData rcdA;
   const LRL_ReadLatticeData rcd = rcdA.read();
   lattice = rcd.GetLattice();
   if ((!lattice.empty()) && (static_cast<char>(letters.find(toupper(lattice[0]))) != std::string::npos)) {
      LRL_Cell cell = LatticeConverter::NiggliReduceCell(lattice, rcd.GetCell());
      if (DirichletConstants::sellingNiggli == "SELLING")
         cell = LatticeConverter().SellingReduceCell(lattice, rcd.GetCell());
      else
         cell = LatticeConverter().NiggliReduceCell(lattice, rcd.GetCell());
      return cell;
   }
   else {
      throw;
   }
}

//    static LRL_Cell SellingReduceCell( const std::string& lattice, const LRL_Cell& cell, MatS6& mat );

double FixZero(const double d) {
   return (abs(d) < 1.0E-8) ? 0.0 : d;
}

CNearTree<Vector_3> Dirichlet::CreateTreeOfLatticePoints() {
   return CreateVectorOfLatticePoints();
}

Dirichlet_Intersection Dirichlet::CalculateOneIntersection(const size_t i, const size_t j, const size_t k, const std::vector<Vector_3>& vLattice)
{
   const Vector_3& v1 = vLattice[i];
   const Vector_3& v2 = vLattice[j];
   const Vector_3& v3 = vLattice[k];

   const double norm1 = v1.Norm();
   const double norm2 = v2.Norm();
   const double norm3 = v3.Norm();

   if ( norm1<1.0E-6 || norm2 < 1.0E-6 || norm3 < 1.0E-6) return Dirichlet_Intersection();

   const Vector_3 a = v1 / v1.Norm();
   const Vector_3 b = v2 / v2.Norm();
   const Vector_3 c = v3 / v3.Norm();
    // prelude
      const double cos1 = abs(a.Dot(b));
      const double cos2 = abs(a.Dot(c));
      const double cos3 = abs(b.Dot(c));

      // is some pair of planes parallel?
      if ((abs(cos1) > 0.99999) || (abs(cos2) > 0.99999) || (abs(cos3) > 0.99999)) {
         return Dirichlet_Intersection();
      }
   

   const Matrix_3x3 const_m(Matrix_3x3(
      a[0], a[1], a[2],
      b[0], b[1], b[2],
      c[0], c[1], c[2]));
   const double constDet = const_m.Det();

   if (abs(constDet) < 1.0E-4) {
      return Dirichlet_Intersection();
   }

   Vector_3 v;
   Dirichlet_Intersection intersection;
   { // the real calcularions
      Matrix_3x3 m;
      intersection.SetIsValid(true);
      // Cramer's rule
      for (int i = 0; i < 3; ++i) {
         m = const_m;
         m[i] = v1.Norm() / 2;
         m[i + 3] = v2.Norm() / 2;
         m[i + 6] = v3.Norm() / 2;

         const double det = m.Det();
         v[i] = det / constDet;
      }
   }

   if (!(v[0] > -DBL_MAX)) {
      const int i19191 = 19191;
   }

   intersection.SetCoordinates(v);
   intersection.SetFacesIndices({ i,j,k });
   return intersection;
}


std::vector<Vector_3> Dirichlet::CreateVectorOfLatticePoints() {
   std::vector<Vector_3> v;
   const int maxLatticeLimit = DirichletConstants::latticeLimit;
   for (int face = -maxLatticeLimit; face <= maxLatticeLimit; ++face) {
      for (int j = -maxLatticeLimit; j <= maxLatticeLimit; ++j) {
         for (int k = -maxLatticeLimit; k <= maxLatticeLimit; ++k) {
            const double& di = face;
            const double& dj = j;
            const double& dk = k;
            if (di == 0 && dj == 0 && dk == 0) continue;
            Vector_3 v3 = m_cartesianMatrix * Vector_3(di, dj, dk);
            for (size_t face = 0; face < 3; ++face)
               if (abs(v3[face]) < 1.0E-8) v3[face] = 0.0;
            v.push_back(v3);
         }
      }
   }
   return v;
}


/*   std::vector<Dirichlet_Vertex> m_vertexList; // ARE THESE THE SAME THING??????????????
   std::vector<size_t> m_VerticesInThisFace;;
*/

void Dirichlet::InsertFaceIndices( const Dirichlet_Intersection& di, const size_t n) {
   const std::vector<size_t> v = di.GetFaceIndices();
   //const size_t nmax = std::max(std::max(v[0],v[1]), std::max(v[2], m_faces.size()))+1;
   //if (nmax > m_faces.size()) m_faces.resize(nmax);
   m_faces[v[0]].AddVertex(n);
   m_faces[v[1]].AddVertex(n);
   m_faces[v[2]].AddVertex(n);
}

void Dirichlet::CreateFaceList(const std::vector<Vector_3>& vertices, const std::vector<Dirichlet_Intersection>& v) {
   int count = 0;
   m_faces.resize(vertices.size());
   for (size_t i = 0; i < v.size(); ++i) {
      Dirichlet_Face face;
      const double normFace = v[i].GetCoordinates().Norm();
      face.SetNormal(v[i].GetCoordinates() / normFace);
      face.SetDistance(normFace);
      InsertFaceIndices(v[i], i);
      face.SetVertexIndexList(v[i].GetFaceIndices());
      if (face.size() > 3) m_faces[i] = face;
      if (face.size() > 3) ++count;
   }

   count = 0;
   int minVertices = INT_MAX;
   int maxVertices = 0;
   for (size_t i = 0; i < m_faces.size(); ++i) {
      if (m_faces[i].size() > 3) ++count;
      if (maxVertices < m_faces[i].size()) maxVertices = m_faces[i].size();
      if (minVertices > m_faces[i].size()) minVertices = m_faces[i].size();
   }
   count;
   const int i19191 = 19191;
}

/*               double distanceFromOrigin = DBL_MAX;
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
                  vdf[i1].SetIndicesOfIntersection(i1,i2,i3);
                  const int i19191 = 19191;
               }
*/
std::vector<Dirichlet_Intersection> Dirichlet::CreateAllIntersections(const std::vector<Vector_3>& vLattice) {
   std::vector<Dirichlet_Intersection> vout;
   const CNearTree<Vector_3> tree(vLattice);
   const auto it0 = tree.NearestNeighbor(DBL_MAX, vLattice[0]);

   for (size_t i = 0; i < vLattice.size()-2; ++i) {
      for (size_t j = i+1; j < vLattice.size()-1; ++j) {
         if (i == j) continue;
         for (size_t k = j+1; k < vLattice.size(); ++k) {
            if (i == k || j == k) continue;
            Dirichlet_Intersection inter = CalculateOneIntersection(i, j, k, vLattice);

            double distanceFromOrigin = DBL_MAX;
            double nearestFoundDistance = DBL_MAX;
               const auto itX = tree.NearestNeighbor(DBL_MAX, inter.GetCoordinates());
            if (itX != tree.end()) {
               distanceFromOrigin = (inter.GetCoordinates() - (*it0)).Norm();
               nearestFoundDistance = (inter.GetCoordinates() - (*itX)).Norm();
            }
            if (inter.IsValid()) {

               if ((it0 == itX) || (abs(distanceFromOrigin - nearestFoundDistance < 1.0E-4))) {
                  inter.SetFacesIndices(std::vector<size_t>({ i,j,k }));
                  vout.push_back(inter);
               }
            }
         }
      }
   }
   return vout;
}

std::vector<std::pair<double, size_t> > RemoveDuplicatefaceAngles( const std::vector<std::pair<double, size_t> >  angleWithIndex ) {
   std::vector<std::pair<double, size_t> > out;
   out.push_back(angleWithIndex[0]);
   for (size_t i = 1; i < angleWithIndex.size() - 1; ++i) {
      if (abs(angleWithIndex[i].first - angleWithIndex[i + 1].first) > 1.0E-4) out.push_back(angleWithIndex[i + 1]);
   }
   return out;
}

void Dirichlet::ProcessOneFace(const Dirichlet_Face& face, const std::vector<Dirichlet_Intersection>& intersIn) {
   Vector_3 cm(0, 0, 0);
   std::vector<Dirichlet_Intersection> inters(intersIn);
   std::vector<size_t> vin = face.GetVertexIndexList();
   for (size_t i = 0; i < vin.size(); ++i) {
      cm += inters[vin[i]].GetCoordinates();
   }
   cm = cm / double(vin.size());

   vin.push_back(vin[0]);
   //inters.push_back(inters[0]);
   std::vector<std::pair<double, size_t> >  angleWithIndex;
   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);
   for (size_t i = 0; i < vin.size()-1; ++i) {
      const Vector_3 inter1 = inters[vin[i]].GetCoordinates();
      const Vector_3 inter2 = inters[vin[i+1]].GetCoordinates();
      double angle = degreesPerRad * Vector_3::Angle(inter1, cm, inter2);
      if (angle < 1.0E-4) angle = 0;
      angleWithIndex.push_back(std::make_pair(angle, i));
   }


   std::sort(angleWithIndex.begin(), angleWithIndex.end());

   std::vector<std::pair<double, size_t> > out = RemoveDuplicatefaceAngles(angleWithIndex);
   std::cout << std::endl;

   for (size_t i = 0; i < out.size()-1; ++i) {
      std::cout << out[i].first << "  " << out[i].second << std::endl;
   }
   const int i19191 = 19191;
}

void Dirichlet::ProcessFaces(const std::vector<Dirichlet_Intersection>& inters) {
   for (size_t i = 0; i < m_faces.size(); ++i) {
      if ( m_faces[i].size() > 0) ProcessOneFace(m_faces[i], inters);
   }
}

//std::vector<Dirichlet_Intersection> Dirichlet::CleanIntersectionList(const std::vector<Dirichlet_Intersection>& vInter) {
//   CNearTree<Vector_3> tree = Dirichlet::CreateTreeOfLatticePoints();
//   tree.insert(Vector_3(0, 0, 0));
//   const auto it_Origin = tree.NearestNeighbor(0.0001, Vector_3(0., 0., 0.));  // origin
//
//   std::vector<Dirichlet_Intersection> vout;
//   for (size_t i = 0; i < vInter.size(); ++i) {
//      const auto it = tree.NearestNeighbor(DBL_MAX, vInter[i].GetCoordinates());
//      if (it == it_Origin) {
//         vout.push_back(vInter[i]);
//      }
//   }
//   return vInter;
//}

//std::vector<Dirichlet_Intersection> Dirichlet::GenerateIntersectionList() {
//   const std::vector<Vector_3> vLattice = CreateVectorOfLatticePoints();
//   const std::vector<Dirichlet_Intersection> vInter = CreateAllIntersections(vLattice);
// 
//   const std::vector<Dirichlet_Intersection> vout = CleanIntersectionList(vInter);
//
//   return vout;
//}

Dirichlet_Intersection::Dirichlet_Intersection(const Vector_3& v, const std::vector<size_t>& planes)
   : m_IsValid(true)
   , m_coord(v)
   , m_faces(planes)
{
}

Dirichlet_Intersection::Dirichlet_Intersection(const std::vector<size_t>& planes, const Vector_3& v)
   : m_IsValid(true)
   , m_coord(v)
   , m_faces(planes)
{
}

Dirichlet_Faces::Dirichlet_Faces(const std::vector<Dirichlet_Intersection>& intersections)
   : m_intersections(intersections)
{
   std::vector< std::vector<size_t> > faces;

   for (size_t ith_Intersection = 0; ith_Intersection < intersections.size(); ++ith_Intersection) {
      const size_t n0 = intersections[ith_Intersection][0];
      const size_t n1 = intersections[ith_Intersection][1];
      const size_t n2 = intersections[ith_Intersection][2];
      const size_t maxIndex = std::max(std::max(n0, n1), n2);
      if (maxIndex+1 > faces.size()) faces.resize(maxIndex + 1);
     faces[n0].push_back(ith_Intersection);
     faces[n1].push_back(ith_Intersection);
     faces[n2].push_back(ith_Intersection);
   }

   for (size_t i = 0; i < faces.size(); ++i) {
      const std::vector<size_t> merged = GetAngleSortedAndMergedIndices(faces[i]);
         //if (vAngles.size() > 2) m_faces.push_back(faces[i]);
      if ( merged.size() > 2 ) m_faces.push_back(merged);
   }
   const std::vector<double> vAreas = GetFaceAreas();
   std::cout << LRL_ToString(vAreas) << std::endl;
}

Vector_3 Dirichlet_Faces::CenterOfMassForOneFace(const std::vector<size_t>& list) const {
   Vector_3 centerOfMass(0.0, 0.0, 0.0);

   for (int face = 0; face < list.size(); ++face) {
      const Vector_3& point = m_intersections[list[face]].GetCoordinates();
      centerOfMass += point;
   }
   return centerOfMass / double(list.size());
}

std::vector<double> Dirichlet_Faces::ComputeAnglesForOneFace(const std::vector<size_t>& vi) const {
   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);
   // So here we will take the first point and compute a unit vector 
   // (the points are already at the center of mass.) The second basis
   // vector is the plane normal. The third will be the cross product
   // of those two vectors. The vector from the origin to the center
   // of mass is the plane normal.
   std::vector<double> angleList;
   if (vi.size() < 3 ) return angleList;
   const Vector_3 cm = CenterOfMassForOneFace(vi);
   const double distanceFromCenterOfMass = (m_intersections[vi[0]].GetCoordinates() - cm).Norm();
   if (distanceFromCenterOfMass < 1.0E-5) return angleList;

   const Vector_3 firstVectorInList = m_intersections[ vi[0]].GetCoordinates();
   const Vector_3 basis1 = firstVectorInList / firstVectorInList.Norm();
   const Vector_3 basis2 = cm / cm.Norm();
   const Vector_3 basis3 = basis1.Cross(basis2);
   const double angleZero = degreesPerRad * 
      atan2(m_intersections[vi[0]].GetCoordinates().Dot(basis1), m_intersections[vi[0]].GetCoordinates().Dot(basis3));;

   for (size_t face = 0; face < vi.size(); ++face) {
      double angle = degreesPerRad * 
         atan2(m_intersections[vi[face]].GetCoordinates().Dot(basis1), m_intersections[vi[face]].GetCoordinates().Dot(basis3));
      if ( face==0 || angle < 1.0E-5) angle = std::fmod(angle - angleZero + 3.0 * 360.0, 360.0);
      angleList.push_back(angle);
   }
   return angleList;
}

std::vector<double> Dirichlet_Faces::GetFaceAreas() const {
   std::vector<double> output;

   for (size_t i = 0; i < m_faces.size(); ++i) {
      const double area = GetAreaOfOneFace(i);
      output.push_back(area);
   }
   return output;
}

double Dirichlet_Faces::GetAreaOfOneFace(const size_t n) const {
   const std::vector<size_t> intersections( m_faces[n]);
   const Vector_3 cm = CenterOfMassForOneFace(m_faces[0]);

   double totalArea = 0.0;
   for (size_t i = 0; i < intersections.size(); ++i) {

      const Vector_3 v1 = m_intersections[intersections[i]].GetCoordinates();
      const Vector_3 v2 = (i==intersections.size()-1) ? 
      m_intersections[intersections[0]].GetCoordinates():
      m_intersections[intersections[i + 1]].GetCoordinates();
      const Vector_3 v3 = v1-v2;

      const double side1 = (v1 - cm).norm();
      const double side2 = (v2 - cm).norm();
      const double side3 = v3.Norm();

      const double area = TriangleAreaFromSides(side1, side2,side3); // Heron's formula for 3 sides
      totalArea += area;
   }
   return totalArea;
}

std::pair<std::vector<double>, std::vector<size_t> > Dirichlet_Faces::SortByAngle(const std::pair<std::vector<double>, std::vector<size_t> >& input) const {
   std::pair<std::vector<double>, std::vector<size_t> > sorted(input);
   for (size_t i = 1; i < sorted.second.size(); ++i) {
      for (size_t i = 1; i < sorted.second.size(); ++i) {
         if (sorted.first[i] < sorted.first[i - 1]) std::swap(sorted.first[i], sorted.first[i - 1]);
      }
   }
   return sorted;
}

std::vector<size_t> Dirichlet_Faces::GetAngleMergedIndices(const std::pair<std::vector<double>, std::vector<size_t> >& input) const {
   std::vector<size_t> output;
   output.push_back(input.second[0]);
   const std::vector<double>& angles(input.first);

   for (size_t i = 0; i < angles.size()-1; ++i) {
      const double d1 = angles[i];
      const size_t kk = i + 1;
      const double d2 = angles[kk];
      const double diff = abs(d1-d2);
      if (diff > 1.0e-4) {
         output.push_back(input.second[kk]);
      }
   }
   return (output.size()>2) ? output : std::vector<size_t>();
}

std::vector<size_t> Dirichlet_Faces::GetAngleSortedAndMergedIndices(const std::vector<size_t>& input) const {
   const std::vector<double> angles = ComputeAnglesForOneFace(input);
   const size_t angleCount = angles.size();
   if (angleCount < 3) return std::vector<size_t>();
   const std::pair<std::vector<double>, std::vector<size_t> > paired(angles, input);
   const std::pair<std::vector<double>, std::vector<size_t> > sorted = SortByAngle(paired);
   const std::vector<size_t> merged = GetAngleMergedIndices(sorted);
   return merged;
}

Dirichlet_Vertex::Dirichlet_Vertex::Dirichlet_Vertex()
   : m_coord()
   , m_faceAngleForSorting(-DBL_MAX)
   , m_zSort()
   , m_indicesOfFaces()
{}

Dirichlet_Vertex::Dirichlet_Vertex(const Dirichlet_Intersection& di)
   : m_coord()
   , m_faceAngleForSorting(-DBL_MAX)
   , m_zSort()
   , m_indicesOfFaces()
{}


Dirichlet_Face::Dirichlet_Face(const Dirichlet_Intersection& inter) 
   : m_VerticesInThisFace(3)
{
   const size_t currentFaceCount = m_VerticesInThisFace.size();
   const size_t maxIndex = std::max(std::max(inter[0], inter[1]), inter[2]);
   if (maxIndex > m_VerticesInThisFace.size() - 1) m_VerticesInThisFace.resize(maxIndex + 1);

   for (size_t i = 0; i < inter.size(); ++i) {
   }
   const Vector_3 coord(inter.GetCoordinates());
   SetCoord(coord);
   SetNormal(coord / coord.Norm());
   SetDistance(coord.Norm());
   SetVertexIndexList(inter.GetFaceIndices());
}

void Dirichlet_Face::AddVertex(const size_t n) {
   m_VerticesInThisFace.push_back(n);
}


std::vector<double> GetAreas() {
   //return DiricheltFaces::GetFaceAreas();
   throw;
   return std::vector<double>();
}