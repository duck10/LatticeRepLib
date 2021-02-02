#ifndef DIRICHLET_H
#define DIRICHLET_H

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_Vector3.h"
#include "TNear.h"

#include <map>
#include <vector>

class Dirichlet_Intersections {
public:
   Dirichlet_Intersections() : m_IsValid(false) {}
   Dirichlet_Intersections(const Vector_3& v, const std::vector<size_t>& planes);
   Dirichlet_Intersections(const std::vector<size_t>& planes, const Vector_3& v);
   void SetIsValid(const bool b) { m_IsValid = b; }
   void SetCoordinates(const Vector_3& v) { m_coord = v; }
   void SetFacesIndices(const std::vector<size_t>& v) { m_faces = v; }
   bool IsValid() const { return m_IsValid; }
   Vector_3 GetCoordinates() const { return m_coord; }
   std::vector<size_t> GetFaceIndices() const { return m_faces; }
   size_t size() const { return m_faces.size(); }
   size_t operator[] (const size_t n) const { return m_faces[n]; }
   void resize(const size_t n) { m_faces.resize(n); }

private:
   bool m_IsValid;
   Vector_3 m_coord;
   std::vector<size_t> m_faces;
};

class Dirichlet_Vertex {
public:
   Dirichlet_Vertex();
private:
   Vector_3 m_coord;
   Vector_3 m_normal;
   double m_faceAngleForSorting;
   double m_zSort;
   std::vector<double> m_indicesOfFaces;
};

class Dirichlet_Face {
public:
   Dirichlet_Face() {}
   Dirichlet_Face(const Dirichlet_Intersections& inter);
   Vector_3 GetCenterOfMass() const { return   m_centerOfMass; }
   Vector_3 GeCoord        () const { return   m_coord; }
   Vector_3 GetNormal      () const { return   m_normal; }
   double   GetDistance    () const { return   m_distance; }
   std::vector<Dirichlet_Vertex> GetVertexList() const { return m_vertexList; }
   std::vector<size_t> GetVertexIndexList() const { return m_VerticesInThisFace; }

   void SetCenterOfMass(const Vector_3& v) { m_centerOfMass = v; }
   void SetCoord        (const Vector_3& v) { m_coord = v; }
   void SetNormal      (const Vector_3& v) { m_normal = v; }
   void SetDistance    (const double d   ) { m_distance = d; }
   void SetVertexIndexList(const std::vector<size_t>& v) { m_VerticesInThisFace = v; }

private:
   Vector_3 m_centerOfMass;
   Vector_3 m_coord;
   Vector_3 m_normal;
   double m_distance;
   std::vector<Dirichlet_Vertex> m_vertexList; // ARE THESE THE SAME THING??????????????
   std::vector<size_t> m_VerticesInThisFace;;
};

class Dirichlet_Faces {
public:
   Dirichlet_Faces() {}
   Dirichlet_Faces(const std::vector<Dirichlet_Intersections>& intersections);
   Dirichlet_Faces FilteredFaces(const Dirichlet_Faces& inFaces) const;
   size_t size() const { return m_faces.size(); }
   void Add(const Dirichlet_Face& face);

   std::vector<double> ComputeAnglesForOneFace(const std::vector<size_t>& vi) const;
   std::pair<std::vector<double>, std::vector<size_t> > SortByAngle(const std::pair<std::vector<double>, std::vector<size_t> >& input) const;
   std::vector<size_t> GetAngleSortedAndMergedIndices(const std::vector<size_t>& indices) const;
   std::vector<size_t> GetAngleMergedIndices(const std::pair<std::vector<double>, std::vector<size_t> >& input) const;
   double GetAreaOfOneFace(const size_t n) const;
   std::vector<double> GetFaceAreas() const;

   std::vector<size_t> GetVectorOfIndicesForOneFace(const size_t n) const { return m_faces[n]; }
   Vector_3 CenterOfMassForOneFace(const std::vector<size_t>& list) const;
private:
   const std::vector<Dirichlet_Intersections> m_intersections;
   std::vector< std::vector<size_t> > m_faces;
};

class Dirichlet {
public:
   template<typename T>
   Dirichlet(const T& cell)
      : m_cell(cell)
      , m_cellDegrees(m_cell)
      , m_cartesianMatrix(Matrix_3x3::Cart(
         m_cellDegrees[0], m_cellDegrees[1], m_cellDegrees[2], 
         m_cellDegrees[3], m_cellDegrees[4], m_cellDegrees[5]))
      , m_faces()
      , m_maxDiameter(0)
   {
   }

   Matrix_3x3 GetMatrix() const { return m_cartesianMatrix; }
   LRL_Cell GetCell() const { return m_cell; }
   LRL_Cell_Degrees GetCellDegrees() const { return m_cellDegrees; }
   std::vector<Dirichlet_Face> GetFaceList() const { return m_faces; }
   double GetMaxDiameter() const { return m_maxDiameter; }
   std::vector<Dirichlet_Intersections> CreateAllIntersections(const std::vector<Vector_3>& vLattice);
   Dirichlet_Intersections CalculateOneIntersection(const size_t i, const size_t j, const size_t k, const std::vector<Vector_3>& vLattice);
   CNearTree<Vector_3> CreateTreeOfLatticePoints();


   static LRL_Cell ReadAndReduceCell();
   static LRL_Cell ParseAndReduceStringCellWithLattice(const std::string& strCell);
   static LRL_Cell ParseAndReduceCell(const std::string& lattice, const std::string& strCell);
   std::vector<Vector_3> CreateVectorOfLatticePoints();
   //std::vector<Dirichlet_Intersections> GenerateIntersectionList();
   //std::vector<Dirichlet_Intersections> CleanIntersectionList(const std::vector<Dirichlet_Intersections>& vInter);
private:
   LRL_Cell m_cell;
   LRL_Cell_Degrees m_cellDegrees;
   Matrix_3x3 m_cartesianMatrix;
   std::vector<Dirichlet_Face> m_faces;
   double m_maxDiameter;
};

#endif  // DIRICHLET

