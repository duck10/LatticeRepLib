#include "Dirichlet.h"

#include "LatticeConverter.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "TriangleAreaFromSides.h"

#include <cmath>

static const std::string letters( "V,G,D,S,P,A,B,C,I,F,R,C3,G6,S6,B4,D7,H");

LRL_Cell Dirichlet::ReadAndReduceCell() {
   std::string lattice;
   LRL_ReadLatticeData rcdA;
   const LRL_ReadLatticeData rcd = rcdA.read();
   lattice = rcd.GetLattice();
   if ((!lattice.empty()) && (static_cast<char>(letters.find(toupper(lattice[0]))) != std::string::npos)) {
      LRL_Cell cell = LatticeConverter::SellingReduceCell(lattice, rcd.GetCell());
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

std::vector<Vector_3> Dirichlet::CreateVectorOfLatticePoints() {
   std::vector<Vector_3> v;
   std::vector<std::vector<size_t> > indexList;
   static const int latticeLimit = 2;
   for (int i = -latticeLimit + 1; i < latticeLimit; ++i) {
      for (int j = -latticeLimit + 1; j < latticeLimit; ++j) {
         for (int k = -latticeLimit + 1; k < latticeLimit; ++k) {
            std::vector<size_t> indices(3);
            indices[0] = i;
            indices[1] = j;
            indices[2] = k;
            indexList.push_back(indices);
            const Vector_3 vx = GetMatrix() * Vector_3(double(i),double(j),double(k));
            v.push_back(Vector_3(FixZero(vx[0]), FixZero(vx[1]), FixZero(vx[2])));
         }
      }
   }
   return v;
}


CNearTree<Vector_3> Dirichlet::CreateTreeOfLatticePoints() {
   return CreateVectorOfLatticePoints();
}

Dirichlet_Intersections Dirichlet::CalculateOneIntersection(const size_t i, const size_t j, const size_t k, const std::vector<Vector_3>& vLattice)
{
   const Vector_3& v1 = vLattice[i];
   const Vector_3& v2 = vLattice[j];
   const Vector_3& v3 = vLattice[k];

   const double norm1 = v1.Norm();
   const double norm2 = v2.Norm();
   const double norm3 = v3.Norm();

   if ( norm1<1.0E-6 || norm2 < 1.0E-6 || norm3 < 1.0E-6) return Dirichlet_Intersections();

   const Vector_3 a = v1 / v1.Norm();
   const Vector_3 b = v2 / v2.Norm();
   const Vector_3 c = v3 / v3.Norm();
    // prelude
      const double cos1 = abs(a.Dot(b));
      const double cos2 = abs(a.Dot(c));
      const double cos3 = abs(b.Dot(c));

      // is some pair of planes parallel?
      if ((abs(cos1) > 0.99999) || (abs(cos2) > 0.99999) || (abs(cos3) > 0.99999)) {
         return Dirichlet_Intersections();
      }
   

   const Matrix_3x3 const_m(Matrix_3x3(
      a[0], a[1], a[2],
      b[0], b[1], b[2],
      c[0], c[1], c[2]));
   const double constDet = const_m.Det();

   if (abs(constDet) < 1.0E-4) {
      return Dirichlet_Intersections();
   }

   Vector_3 v;
   Dirichlet_Intersections intersection;
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


std::vector<Dirichlet_Intersections> Dirichlet::CreateAllIntersections(const std::vector<Vector_3>& vLattice) {
   std::vector<Dirichlet_Intersections> vout;
   for (size_t i = 0; i < vLattice.size(); ++i) {
      for (size_t j = 0; j < vLattice.size(); ++j) {
         for (size_t k = 0; k < vLattice.size(); ++k) {
            Dirichlet_Intersections inter = CalculateOneIntersection(i, j, k, vLattice);
            if (inter.IsValid()) {
               inter.SetFacesIndices(std::vector<size_t>({ i,j,k }));
               vout.push_back(inter);
            }
         }
      }
   }
   return vout;
}

std::vector<Dirichlet_Intersections> Dirichlet::CleanIntersectionList(const std::vector<Dirichlet_Intersections>& vInter) {
   CNearTree<Vector_3> tree = Dirichlet::CreateTreeOfLatticePoints();
   tree.insert(Vector_3(0, 0, 0));
   const auto it_Origin = tree.NearestNeighbor(0.0001, Vector_3(0., 0., 0.));  // origin

   std::vector<Dirichlet_Intersections> vout;
   for (size_t i = 0; i < vInter.size(); ++i) {
      const auto it = tree.NearestNeighbor(DBL_MAX, vInter[i].GetCoordinates());
      if (it == it_Origin) {
         vout.push_back(vInter[i]);
      }
   }
   return vout;
}

std::vector<Dirichlet_Intersections> Dirichlet::GenerateIntersectionList() {
   const std::vector<Vector_3> vLattice = CreateVectorOfLatticePoints();
   const std::vector<Dirichlet_Intersections> vInter = CreateAllIntersections(vLattice);
 
   const std::vector<Dirichlet_Intersections> vout = CleanIntersectionList(vInter);

   return vout;
}

Dirichlet_Intersections::Dirichlet_Intersections(const Vector_3& v, const std::vector<size_t>& planes)
   : m_IsValid(true)
   , m_coord(v)
   , m_faces(planes)
{
}

Dirichlet_Intersections::Dirichlet_Intersections(const std::vector<size_t>& planes, const Vector_3& v)
   : m_IsValid(true)
   , m_coord(v)
   , m_faces(planes)
{
}

Dirichlet_Faces::Dirichlet_Faces(const std::vector<Dirichlet_Intersections>& intersections)
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

Dirichlet_Vertex::Dirichlet_Vertex()
   : m_coord()
   , m_normal()
   , m_faceAngleForSorting(-DBL_MAX)
   , m_zSort()
   , m_indicesOfFaces()
{}

Dirichlet_Face::Dirichlet_Face(const Dirichlet_Intersections& inter) 
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
