#include "D13.h"
#include "S6.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <utility>


const std::vector<Vector_3> D13::vertices =
{ Vector_3(0,1,0), Vector_3(1,1,0), Vector_3(1,0,0), Vector_3(1,-1,0),
  Vector_3(-1,1,1), Vector_3(0,1,1), Vector_3(1,1,1), Vector_3(1,0,1),
  Vector_3(1,-1,1), Vector_3(0,-1,1), Vector_3(-1,-1,1), Vector_3(-1,0,1),
  Vector_3(0,0,1) };

std::ostream& operator<< (std::ostream& o, const D13& v) {
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

D13::D13(const LRL_Cell& cell)
   : m_cell(cell)
   , m_lattice("P")
   , m_cellIsValid(cell.IsValid())
{
   m_vec.resize(13);
   m_dim = 13;
   m_cellIsValid = m_cell.IsValid();
}

D13::D13(const std::string& t)
{
   const std::string strInput = t;
   if ((strInput[0] >= 'a' && strInput[0] <= 'z') ||
      (strInput[0] >= 'A' && strInput[0] <= 'Z')) {
      LRL_ReadLatticeData rcd;
      rcd.CellReader(t);
      m_cell = rcd.GetCell();
      m_lattice = rcd.GetLattice();
   }
   else {
      m_cell = LRL_Cell(std::string(t)); // cast to string is necessary for some reason
      m_lattice = "P";
   }
   m_cellIsValid = m_cell.IsValid();
}

std::vector<int> D13::HashIndices(const std::vector<std::vector<int> >& vin) const {
   std::vector<int> v;
   const std::vector<std::vector<int> >& indices = vin;
   for (size_t i = 0; i < indices.size(); ++i) {
      const std::vector<int>& index = indices[i];
      const int hash = HashV3(index);
      std::cout << i << " " << hash << std::endl;
      v.push_back(hash);
   }
   return v;
}

//std::vector<int> D13::HashIndices() const {
//   return HashIndices(m_dc.GetIndices());
//}

//void D13::ConstructHashedAreaList() {
//   const std::vector<std::vector<int> > indList = m_dc.GetIndices();
//   const std::vector<double> areas = m_dc.GetAreas();
//   const std::vector<std::vector<int> > indicesOfAreas = m_dc.GetIndices();
//   std::vector<std::pair<int, std::pair<std::vector<int>, double> > > theList;
//   const std::vector<int> hashedIndices = HashIndices(indList);
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

VecN D13::CreateCompleteListOf13Areas(const std::vector<Vector_3>& allIndices,
   const std::vector<std::vector<int> >& dcIndices,
   const std::vector<double>& dcAreas)
{
   std::vector<int> hashed13(13);

   for (size_t i = 0; i < allIndices.size(); ++i) {
      hashed13[i] = HashV3(allIndices[i]);
      //std::cout << "hash13 " << i << "  " << hashed13[i] << std::endl;
   }
   //std::cout << std::endl;

   for (size_t i = 0; i < dcIndices.size(); ++i) {
      const int hash = HashV3(dcIndices[i]);
      //std::cout << " hash  " << i << "  " << hash << std::endl;
      auto place = std::find(hashed13.begin(), hashed13.end(), hash);
      //if ( place != hashed13.end())
         //std::cout << i << "  " << hash << " " <<  std::endl;

      m_vec.resize(13);
      if (place != hashed13.end()) {
         const int xxxx = place - hashed13.begin();
         //m_dirCellAreas[xxxx] = dcAreas[i];
         m_vec[xxxx] = dcAreas[i];
      }

   }
   //std::cout << LRL_ToString(m_vec) << std::endl;
   return m_vec;
}

D13& D13::operator= (const LRL_Cell& v) {
   *this = D13(v);
   m_cellIsValid = v.IsValid();
   return *this;
}


D13& D13::operator= (const D13& v) {
   m_dirCellAreas = v.m_dirCellAreas;
   m_dim = v.m_dim;
   m_cellIsValid = v.m_cellIsValid;
   m_cell = v.m_cell;
   m_lattice = v.m_lattice;
   m_hashedAreaList = v.m_hashedAreaList;
   m_vec = v.m_vec;
   return *this;
}

D13 D13::operator- (void) const {
   D13 d13;
   d13.m_cellIsValid = false;
   for (size_t i = 0; i < d13.size(); ++i)
      d13[i] = -m_vec[i];
   return d13; // unary
}

D13& D13::operator+= (const D13& d13) {
   for (size_t i = 0; i < d13.size(); ++i)
      m_vec[i] += d13.m_vec[i];
   return *this;
}

D13& D13::operator-= (const D13& D13) {
   for (size_t i = 0; i < D13.size(); ++i)
      m_vec[i] -= D13.m_vec[i];
   return *this;
}

bool D13::operator== (const D13& d13) const {
   return m_vec == d13.m_vec;
}

bool D13::operator!= (const D13& d13) const {
   return !((*this) == d13);
}

D13& D13::operator/= (const double d) {
   (*this).m_vec /= d;
   return *this;
}

D13& D13::operator*= (const double d) {
   (*this).m_vec *= d;
   return *this;
}

D13 D13::operator* (const double d) const {
   D13 dt(*this);
   for (size_t i = 0; i < dt.size(); ++i)
      dt[i] *= d;
   return dt;
}

D13 D13::operator/ (const double d) const {
   D13 dt(*this);
   for (size_t i = 0; i < dt.size(); ++i)
      dt[i] /= d;
   return dt;
}

D13 D13::operator+ (const D13& ds) const {
   D13 v;
   for (size_t i = 0; i < ds.size(); ++i)
      v[i] = (*this)[i] + ds[i];
   return v;
}

D13 D13::operator- (const D13& v) const {
   D13 d(v);
   if ((*this).m_cellIsValid && v.m_cellIsValid) {
      d.m_vec = m_vec - v.m_vec;
   }
   else {
      d.m_cellIsValid = false;
   }
   return d;
}

D13& D13::operator= (const G6& v) {
   *this = D13(LRL_Cell(v));
   return *this;
}

D13& D13::operator= (const S6& v) {
   *this = D13(LRL_Cell(v));
   return *this;
}

D13& D13::operator= (const C3& v) {
   *this = D13(LRL_Cell(v));
   return *this;
}

D13& D13::operator= (const D7& v) {
   *this = D13(LRL_Cell(v));
   return *this;
}

D13& D13::operator= (const B4& v) {
   *this = D13(LRL_Cell(v));
   return *this;
}

double DistanceBetween(const D13& v1, const D13& v2) {
   return (v1 - v2).Norm();
}

std::vector<std::string> D13::GetStringIndices() {
   std::vector<std::string> s;
   for (size_t i = 0; i < vertices.size(); ++i) {
      s.push_back(LRL_ToString(vertices[i], " "));
   }
   return s;
}
