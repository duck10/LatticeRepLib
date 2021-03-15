#ifndef D13_H
#define D13_H

#include <vector>

#include "Dirichlet.h"

#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"

class D13 {

   const static std::vector<Vector_3> vertices;
public:
   D13() {}

   template<typename T>
   D13(const T& t)
      : m_cell(LRL_Cell(t))
      , m_cellIsValid(m_cell.IsValid())
      , m_dc()
   {
      const Matrix_3x3 cartesian(m_cell.Cart());
      m_dc = DirichletCell("P", m_cell);
      //v13.resize(13);
      //for (size_t i = 0; i < vertices.size(); ++i) {
      //   v13[i] = (cartesian * vertices[i]).Norm();
      //}
   }

   D13(const std::string& strCellAndLattice) {
      LRL_ReadLatticeData rdc;
      rdc.CellReader(strCellAndLattice);
      m_dc = DirichletCell(rdc.GetLattice(), rdc.GetCell());
   }

   std::vector<std::vector<int> > GetIndices() const { return m_dc.GetIndices(); }

   std::vector<double> GetAreas() const { return m_dc.GetAreas(); }

   std::vector<std::string> GetStringIndices() const { return m_dc.GetStrIndices(); }
   //std::vector<Vector_3> GetIndices();

   //std::string GetIndex(const size_t n);

private:
   LRL_Cell m_cell;
   bool m_cellIsValid;
   DirichletCell m_dc;
   //std::vector<Vector_3> indices;
   //std::vector<std::string> sIndices;
   //std::vector<double> areas;
};

#endif  // D13_h
