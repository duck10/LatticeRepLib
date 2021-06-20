#include "DC.h"
#include "Niggli.h"
#include "S6.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <utility>

const std::vector<Vector_3> DC::vertices =
{ Vector_3(0,1,0), Vector_3(1,1,0), Vector_3(1,0,0), Vector_3(1,-1,0),
  Vector_3(-1,1,1), Vector_3(0,1,1), Vector_3(1,1,1), Vector_3(1,0,1),
  Vector_3(1,-1,1), Vector_3(0,-1,1), Vector_3(-1,-1,1), Vector_3(-1,0,1),
  Vector_3(0,0,1) };

std::ostream& operator<< (std::ostream& o, const DC& v) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(3);
   for (size_t i = 0; i < v.size(); ++i)
      o << std::setw(9) << v[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

template<typename T>
double HashV3(const T& v) {
   const int hash = v[0] + 4 * v[1] + 16 * v[2] + 2;
   return hash;
}

DC::DC(const LRL_Cell& cell)
   : m_lattice("P")
   , m_cellIsValid(cell.IsValid())
   //, m_dc(m_lattice, cell)
{
   G6 out;
   const bool b = Niggli::Reduce(G6(cell), out);
   m_cell = LRL_Cell(out);
   //m_dirCellAreas = m_dc.GetAreas();
   m_dim = 13;
   m_vec = G6_to_V13(out);

   //m_dc = DirichletCell(m_lattice, m_cell);
   //CreateCompleteListOf13Areas(vertices, m_dc.GetIndices(), m_dc.GetAreas());
}

DC::DC(const std::string& t)
{
   const std::string strInput = t;
   if ((strInput[0] >= 'a' && strInput[0] <= 'z') ||
      (strInput[0] >= 'A' && strInput[0] <= 'Z')) {
      LRL_ReadLatticeData rcd;
      rcd.CellReader(t);
      G6 out;
      const bool b = Niggli::Reduce(G6(rcd.GetCell()), out);
      m_cell = out;
      m_cell = rcd.GetCell();
      m_lattice = rcd.GetLattice();
   }
   else {
      m_cell = LRL_Cell(std::string(t)); // cast to string is necessary for some reason
      G6 out;
      const bool b = Niggli::Reduce(G6(m_cell), out);
      m_cell = out;
      m_lattice = "P";
   }
   m_cellIsValid = m_cell.IsValid();
   //m_dc = DirichletCell(m_lattice, m_cell);
   //CreateCompleteListOf13Areas(vertices, m_dc.GetIndices(), m_dc.GetAreas());
}

//std::vector<int> DC::HashIndices(const std::vector<std::vector<int> >& vin) const {
//   std::vector<int> v;
//   const std::vector<std::vector<int> >& indices = vin;
//   for (size_t i = 0; i < indices.size(); ++i) {
//      const std::vector<int>& index = indices[i];
//      const int hash = HashV3(index);
//      std::cout << i << " " << hash << std::endl;
//      v.push_back(hash);
//   }
//   return v;
//}

//std::vector<int> DC::HashIndices() const {
//   return HashIndices(m_dc.GetIndices());
//}
//
//void DC::ConstructHashedAreaList() {
//   //const std::vector<std::vector<int> > indList = m_dc.GetIndices();
//   //const std::vector<double> areas = m_dc.GetAreas();
//   //const std::vector<std::vector<int> > indicesOfAreas = m_dc.GetIndices();
//   std::vector<std::pair<int, std::pair<std::vector<int>, double> > > theList;
//   //const std::vector<int> hashedIndices = HashIndices(indList);
//   for (size_t i = 0; i < indList.size(); ++i) {
//      theList.push_back(std::make_pair(hashedIndices[i], std::make_pair(indicesOfAreas[i], areas[i])));
//   }
//
//   const std::vector<Vector_3>& v = vertices;
//   std::vector<int> hashTotalIndexList;
//   std::vector < std::pair<int, double> > hashAndArea;
//   for (size_t i = 0; i < vertices.size(); ++i) {
//      hashTotalIndexList.push_back(HashV3(vertices[i]));
//   }
//
//   for (size_t i = 0; i < vertices.size(); ++i) {
//      hashAndArea.push_back(std::make_pair(HashV3(vertices[i]), 0.0));
//   }
//
//   // now fill in the non-zero areas
//   for (size_t i = 0; i < theList.size(); ++i) {
//      hashAndArea.push_back(std::make_pair(HashV3(vertices[i]), 0.0));
//   }
//
//}

//VecN DC::CreateCompleteListOf13Areas(const std::vector<Vector_3>& allIndices,
//   const std::vector<std::vector<int> >& dcIndices,
//   const std::vector<double>& dcAreas)
//{
//   std::vector<int> hasheDC(13);
//
//   for (size_t i = 0; i < allIndices.size(); ++i) {
//      hasheDC[i] = HashV3(allIndices[i]);
//      //std::cout << "hash13 " << i << "  " << hasheDC[i] << std::endl;
//   }
//   //std::cout << std::endl;
//
//   for (size_t i = 0; i < dcIndices.size(); ++i) {
//      const int hash = HashV3(dcIndices[i]);
//      //std::cout << " hash  " << i << "  " << hash << std::endl;
//      auto place = std::find(hasheDC.begin(), hasheDC.end(), hash);
//      //if ( place != hasheDC.end())
//         //std::cout << i << "  " << hash << " " <<  std::endl;
//
//      m_vec.resize(13);
//      if (place != hasheDC.end()) {
//         const int xxxx = place - hasheDC.begin();
//         //m_dirCellAreas[xxxx] = dcAreas[i];
//         m_vec[xxxx] = dcAreas[i];
//      }
//
//   }
//   //std::cout << LRL_ToString(m_vec) << std::endl;
//   return m_vec;
//}

DC& DC::operator= (const LRL_Cell& v) {
   *this = DC(v);
   m_cellIsValid = v.IsValid();
   return *this;
}


DC& DC::operator= (const DC& v) {
   m_dim = v.m_dim;
   m_cellIsValid = v.m_cellIsValid;
   m_cell = v.m_cell;
   m_lattice = v.m_lattice;
   //m_dc = v.m_dc;
   m_hashedAreaList = v.m_hashedAreaList;
   m_vec = v.m_vec;
   return *this;
}

DC DC::operator- (void) const {
   DC DC;
   DC.m_cellIsValid = false;
   for (size_t i = 0; i < DC.size(); ++i)
      DC[i] = -m_vec[i];
   return DC; // unary
}

DC& DC::operator+= (const DC& DC) {
   for (size_t i = 0; i < DC.size(); ++i)
      m_vec[i] += DC.m_vec[i];
   return *this;
}

DC& DC::operator-= (const DC& DC) {
   for (size_t i = 0; i < DC.size(); ++i)
      m_vec[i] -= DC.m_vec[i];
   return *this;
}

bool DC::operator== (const DC& DC) const {
   return m_vec == DC.m_vec;
}

bool DC::operator!= (const DC& DC) const {
   return !((*this) == DC);
}

DC& DC::operator/= (const double d) {
   (*this).m_vec /= d;
   return *this;
}

DC& DC::operator*= (const double d) {
   (*this).m_vec *= d;
   return *this;
}

DC DC::operator* (const double d) const {
   DC dt(*this);
   for (size_t i = 0; i < dt.size(); ++i)
      dt[i] *= d;
   return dt;
}

DC DC::operator/ (const double d) const {
   DC dt(*this);
   for (size_t i = 0; i < dt.size(); ++i)
      dt[i] /= d;
   return dt;
}

DC DC::operator+ (const DC& ds) const {
   DC v;
   for (size_t i = 0; i < ds.size(); ++i)
      v[i] = (*this)[i] + ds[i];
   return v;
}

DC DC::operator- (const DC& v) const {
   DC d(v);
   if ((*this).m_cellIsValid && v.m_cellIsValid) {
      d.m_vec = m_vec - v.m_vec;
   }
   else {
      d.m_cellIsValid = false;
   }
   return d;
}

DC& DC::operator= (const G6& v) {
   *this = DC(LRL_Cell(v));
   return *this;
}

DC& DC::operator= (const S6& v) {
   *this = DC(LRL_Cell(v));
   return *this;
}

DC& DC::operator= (const C3& v) {
   *this = DC(LRL_Cell(v));
   return *this;
}

DC& DC::operator= (const D7& v) {
   *this = DC(LRL_Cell(v));
   return *this;
}

DC& DC::operator= (const B4& v) {
   *this = DC(LRL_Cell(v));
   return *this;
}

std::vector<std::pair<double, Vector_3> > DC::Cell_to_V13(const LRL_Cell& c) {
   std::vector<std::pair<double, Vector_3> > g(13);
   const Matrix_3x3 cart = c.Cart();
   for (size_t i = 0; i < 13; ++i) {
      const double d = (cart * vertices[i]).Norm();
      g[i] = std::make_pair(d, vertices[i]);
   }

   const bool sorted = true;
   if (sorted)
      for (size_t k = 0; k < 13; ++k) {
         for (size_t i = 0; i < 12; ++i) {
            if (g[i].first > g[i + 1].first) std::swap(g[i], g[i + 1]);
         }
      }
   return g;
}

std::vector<double> DC::G6_to_V13(const G6& gin) {
   std::vector<double> g(13);

   const double& g1 = gin[0];
   const double& g2 = gin[1];
   const double& g3 = gin[2];
   const double& g4 = gin[3];
   const double& g5 = gin[4];
   const double& g6 = gin[5];

   g[0] = g1;
   g[1] = g2;
   g[2] = g1 + g2 + g6;
   g[3] = g1 + g2 - g6;
   g[4] = g3;
   g[5] = g2 + g3 + g4;
   g[6] = g2 + g3 - g4;
   g[7] = g1 + g3 + g5;
   g[8] = g1 + g3 - g5;
   g[9] = g1 + g2 + g3 + g4 + g5 + g6;
   g[10] = g1 + g2 + g3 - g4 - g5 + g6;
   g[11] = g1 + g2 + g3 - g4 + g5 - g6;
   g[12] = g1 + g2 + g3 + g4 - g5 - g6;

   const bool sorted = true;
   if (sorted)
      std::sort(g.begin(), g.end());

   return g;
}

double DC::DistanceBetween(const DC& v1, const DC& v2) {

   G6 g1;
   const bool b1 = Niggli::Reduce(G6(v1.m_cell), g1);
   G6 g2;
   const bool b2 = Niggli::Reduce(G6(v2.m_cell), g2);

   const std::vector<double> lengths1 = G6_to_V13(g1);
   const std::vector<double> lengths2 = G6_to_V13(g2);

   double sum = 0.0;
   //for (size_t i = 0; i < 7; ++i)
   //   sum += (lengths1[i] - lengths2[i]) * (lengths1[i] - lengths2[i]);
   return sqrt(
      (lengths1[0] - lengths2[0]) * (lengths1[0] - lengths2[0]) +
      (lengths1[1] - lengths2[1]) * (lengths1[1] - lengths2[1]) +
      (lengths1[2] - lengths2[2]) * (lengths1[2] - lengths2[2]) +
      (lengths1[3] - lengths2[3]) * (lengths1[3] - lengths2[3]) +
      (lengths1[4] - lengths2[4]) * (lengths1[4] - lengths2[4]) +
      (lengths1[5] - lengths2[5]) * (lengths1[5] - lengths2[5]) +
      (lengths1[6] - lengths2[6]) * (lengths1[6] - lengths2[6]));

   //return sqrt(sum);
}
