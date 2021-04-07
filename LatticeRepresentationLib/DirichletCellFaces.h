#ifndef FACES_H
#define FACES_H

#include <vector>
#include "LRL_Cell.h"
#include "LRL_Vector3.h"
#include "TNear.h"
#include "LRL_Vector3.h"


typedef std::vector<int> POINT;
typedef std::vector<POINT> POINT_LIST; 
typedef std::vector< std::vector<POINT>> FACES;

class DirichletFace;

class Intersection {
public:
   Intersection() : m_isValid(false) {}

   static Intersection FindIntersectionForThreeFaces(const DirichletFace& a, const DirichletFace& b, const DirichletFace& c);
   Vector_3 GetCoord() const { return m_coordinates; }
   void SetCoord(const Vector_3 v) { m_coordinates = v; }
   void SetPlaneList(const size_t i1, const size_t i2, const size_t i3) { m_PlanesThatMakeThisIntersection = { i1, i2, i3}; }
   bool IsValid() const{ return m_isValid; }
   std::vector<size_t> GetPlaneIndices() const { return m_PlanesThatMakeThisIntersection; }
private:
   bool m_isValid;
   Vector_3 m_coordinates;
   std::vector<size_t> m_PlanesThatMakeThisIntersection;
};

class DirichletFace {
public:
   DirichletFace() {}
   DirichletFace(const Vector_3& v3, const double d);
   std::vector< DirichletFace> ReadFaces();

   static DirichletFace FindIntersectionForThreeFaces(const DirichletFace& a, const DirichletFace& b, const DirichletFace& c);
   static void SeekFaces(const double rmax, const size_t iplane, const std::vector<DirichletFace>& faces);

   double GetLength()  const { return m_distance * m_normal.Norm(); }
   double GetDistance() const { return m_distance; }
   void SetDistance(const double d) { m_distance = d; }
   void SetDistance(const Vector_3& v) { m_distance = v.Norm(); }
   void SetNormal(const Vector_3& v) { m_normal = v/v.Norm(); }
   Vector_3 GetNormal() const { return m_normal; }
   Vector_3 GetCoord() const { return m_distance * m_normal; }
   std::vector<size_t> GetIndicesOfIntersection() const { return m_PlanesThatMakeThisIntersection; }
   void SetIndicesOfIntersection(const std::vector<size_t>& indices) { m_PlanesThatMakeThisIntersection = indices; }
   void SetIndicesOfIntersection(const size_t i1, const size_t i2, const size_t i3) { m_PlanesThatMakeThisIntersection = { i1, i2, i3 }; }
   Vector_3 GetCenterOfMass() const { return m_CenterOfMass; }
private:
   Vector_3 m_CenterOfMass;
   Vector_3 m_normal;
   double m_distance;
   std::vector<size_t> m_PlanesThatMakeThisIntersection;
};

class Cell_Faces {
public:
   Cell_Faces() {};
   Cell_Faces(const Cell_Faces& cell) { *this = cell; }
   Cell_Faces(const LRL_Cell& cell, const Matrix_3x3& m) :m_cell(cell), m_cart(m) {};
   static std::vector<DirichletFace> CreateFaces(const CNearTree<Vector_3>& tree);
   LRL_Cell GetCell() const { return m_cell; }
   Matrix_3x3 GetCartesianMatrix() const { return m_cart; }

private:
   LRL_Cell m_cell;
   Matrix_3x3 m_cart;
};

#endif // FACES_H

