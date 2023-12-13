#ifndef DIRICHLET_H
#define DIRICHLET_H

#include "DirichletCellFaces.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_Vector3.h"
#include "TNear.h"

#include <map>
#include <vector>


typedef std::pair<double, Vector_3 > ANGLE_COORDS;
typedef std::vector< ANGLE_COORDS > ANGLELIST;
typedef std::vector<ANGLELIST> ANGLESFORFACES;

class DirichletCell {
public:
   friend std::ostream& operator<< (std::ostream&, const DirichletCell&);
   DirichletCell() {}
   DirichletCell(const std::string& strCellAndLattice);
   DirichletCell(const std::string& lattice, const LRL_Cell& cell);
   void ProcessInputCell(const std::string lattice, const LRL_Cell& cell);

   std::string GetStrCell() const { return m_strCell; }
   LRL_Cell GetCell() const { return m_cell; }
   LRL_Cell GetReducedCell() const { return m_reducedCell; }
   std::vector<std::string> GetStrIndices() const { return m_strIndices; }
   std::vector<std::vector<int> > GetIndices() const { return m_indices; }
   std::vector<double> GetAreas() const { return m_areas; }
   Cell_Faces GetCellFaces() const { return m_cellFaces; }
   Matrix_3x3 GetCartesianMatrix() const { return m_cart; }
   ANGLESFORFACES GetAnglesForFaces() const { return m_facesWithVertices; }
   static double AreaOfOneFace(const ANGLELIST& face);
   static Vector_3 CenterOfMassForOneFace(const ANGLELIST& list);
   static Vector_3 RecoverIndicesOfOneFace(const Matrix_3x3& minv, const ANGLELIST& list);
   static std::vector<Vector_3> RecoverIndicesOfFaces(const Matrix_3x3& m_cart, const ANGLESFORFACES& newRinged);
   std::vector<std::vector<int> > ConvertAllVectorIndicesToInt(const std::vector<Vector_3>& v);
   std::vector<std::string> ConvertAllVectorIndicesToString(const std::vector<Vector_3>& vin);
   std::string ConvertVectorIndicesToString(const Vector_3& vin);
   std::vector<int> ConvertVectorIndicesToInt(const Vector_3& vin);
   std::vector<Vector_3> GetVertices() const { return m_vertices; }

private:
   std::string m_strCell;
   LRL_Cell m_cell;
   LRL_Cell m_reducedCell;
   std::vector<std::string> m_strIndices;
   std::vector<std::vector<int> > m_indices;
   std::vector<double> m_areas;
   Cell_Faces m_cellFaces;
   Matrix_3x3 m_cart;
   ANGLESFORFACES m_facesWithVertices;
   std::vector<Vector_3> m_vertices;
   //const CNearTree<Vector_3> tree = CreateTreeOfLatticePoints(cart);
   //std::vector<DirichletFace> dirichletFaces = Cell_Faces::CreateFaces(tree);
};

class DirichletSVG {
public:
   DirichletSVG(const DirichletCell& dc);
   std::string OutputSVG(const std::vector<std::string>& stereoImages,
      const Cell_Faces& faceRecords, const std::string& extra) const;
private:
   const DirichletCell m_dirCell;
};

#endif  // DIRICHLET

