#include "DC7u.h"
#include "Niggli.h"
#include "S6.h"
#include "LRL_indices.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <utility>

const std::vector<Vector_3> DC7u::vertices =
{ Vector_3(0,1,0), Vector_3(1,1,0), Vector_3(1,0,0), Vector_3(1,-1,0),
  Vector_3(-1,1,1), Vector_3(0,1,1), Vector_3(1,1,1), Vector_3(1,0,1),
  Vector_3(1,-1,1), Vector_3(0,-1,1), Vector_3(-1,-1,1), Vector_3(-1,0,1),
  Vector_3(0,0,1) };

std::ostream& operator<< (std::ostream& o, const DC7u& v) {
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

void DC7u::FromCellToDC7u(const LRL_Cell& cell) {
   G6 red;
   double delta;
   int error;
   const bool b = Niggli::Reduce(G6(cell), red);
   m_vec.resize(7);
   m_vec[DC7u_AA_idx] = red[G6_AA_idx];
   m_vec[DC7u_BB_idx] = red[G6_BB_idx];
   m_vec[DC7u_CC_idx] = red[G6_CC_idx];
   m_vec[DC7u_MIN_BC_diagsq_idx] = red[G6_BB_idx]+red[G6_CC_idx]-std::fabs(red[G6_2BC_idx]);
   m_vec[DC7u_MIN_AC_diagsq_idx] = red[G6_AA_idx]+red[G6_CC_idx]-std::fabs(red[G6_2AC_idx]);
   m_vec[DC7u_MIN_AB_diagsq_idx] = red[G6_AA_idx]+red[G6_BB_idx]-std::fabs(red[G6_2AB_idx]);
   m_vec[DC7u_MIN_ABC_diagsq_idx] = red[G6_AA_idx]+red[G6_BB_idx]+red[G6_CC_idx]
     -std::fabs(red[G6_2BC_idx])-std::fabs(red[G6_2AC_idx])-std::fabs(red[G6_2AB_idx]);
   delta=std::fabs(m_vec[DC7u_AA_idx])*1.e-10;
   error=0;
   for (size_t ii=0; ii < 7; ii++) {
     if (m_vec[ii] < delta) error++;
   }
   if (error > 0) throw std::invalid_argument( "invalid unsorted DC7 cell" );
   if (red[G6_2BC_idx] > delta && red[G6_2AC_idx] > delta && red[G6_2AB_idx] > delta ) {
      m_vec[DC7u_MIN_ABC_diagsq_idx] += 2.*std::min(std::min(red[G6_2BC_idx],red[G6_2AC_idx]),red[G6_2AB_idx]);
   }
   m_dim = 7;
}

DC7u::DC7u(const LRL_Cell& cell)
   : m_dim(7)
   , m_cell(cell)
   , m_cellIsValid(cell.IsValid())
   , m_lattice("P")
{
   FromCellToDC7u(cell);
}

DC7u::DC7u(const DC7u& u)
   : m_dim(7)
   , m_cellIsValid(u.m_cellIsValid)
   , m_vec(u.m_vec)
{
   m_cellIsValid = IsValid();
}

DC7u::DC7u( void )
   : m_dim(0)
   , m_cellIsValid(false)
{
   m_vec.resize(7);
}

DC7u::DC7u(const VecN& v)
   : m_dim(7)
   , m_vec(v)
{
  m_cellIsValid = IsValid();
}

DC7u::DC7u(const std::vector<double>& v)
   : m_dim(7)
   , m_vec(v)
{
  m_cellIsValid = IsValid();
}

DC7u::DC7u(char const* ch) 
{
   *this = DC7u(std::string(ch));
}

DC7u::DC7u(const std::string& t)
{
   const std::string strInput = t;
   LRL_Cell x_cell;

   if ((strInput[0] >= 'a' && strInput[0] <= 'z') ||
      (strInput[0] >= 'A' && strInput[0] <= 'Z')) {
      LRL_ReadLatticeData rcd;
      rcd.CellReader(t);
      x_cell = rcd.GetCell();
      FromCellToDC7u(x_cell);
   }
   else {
      x_cell = LRL_Cell(std::string(t)); // cast to string is necessary for some reason
      FromCellToDC7u(x_cell);
   }
   m_cellIsValid = x_cell.IsValid();
   m_dim = 7;
}

DC7u& DC7u::operator= (const LRL_Cell& v) {
   *this = DC7u(v);
   m_cellIsValid = v.IsValid();
   return *this;
}


DC7u& DC7u::operator= (const DC7u& v) {
   m_dim = v.m_dim;
   m_cellIsValid = v.m_cellIsValid;
   m_cell = v.m_cell;
   m_lattice = v.m_lattice;
   m_vec = v.m_vec;
   return *this;
}

DC7u DC7u::operator- (void) const {
   DC7u dc7u;
   dc7u.m_cellIsValid = false;
   for (size_t i = 0; i < dc7u.size(); ++i)
      dc7u[i] = -m_vec[i];
   return dc7u; // unary
}

DC7u& DC7u::operator+= (const DC7u& dc7u) {
   for (size_t i = 0; i < dc7u.size(); ++i)
      m_vec[i] += dc7u.m_vec[i];
   return *this;
}

DC7u& DC7u::operator-= (const DC7u& DC7u) {
   for (size_t i = 0; i < DC7u.size(); ++i)
      m_vec[i] -= DC7u.m_vec[i];
   return *this;
}

bool DC7u::operator== (const DC7u& dc7u) const {
   return m_vec == dc7u.m_vec;
}

bool DC7u::operator!= (const DC7u& dc7u) const {
   return !((*this) == dc7u);
}

DC7u& DC7u::operator/= (const double d) {
   (*this).m_vec /= d;
   return *this;
}

DC7u& DC7u::operator*= (const double d) {
   (*this).m_vec *= d;
   return *this;
}

DC7u DC7u::operator* (const double d) const {
   DC7u dt(*this);
   for (size_t i = 0; i < dt.size(); ++i)
      dt[i] *= d;
   return dt;
}

DC7u DC7u::operator/ (const double d) const {
   DC7u dt(*this);
   for (size_t i = 0; i < dt.size(); ++i)
      dt[i] /= d;
   return dt;
}

DC7u DC7u::operator+ (const DC7u& ds) const {
   DC7u d(ds);
   if ((*this).m_cellIsValid && ds.m_cellIsValid) {
      d.m_vec = m_vec + ds.m_vec;
   }
   else {
      d.m_cellIsValid = false;
   }
   return d;
}

DC7u DC7u::operator- (const DC7u& v) const {
   DC7u d(v);
   if ((*this).m_cellIsValid && v.m_cellIsValid) {
      d.m_vec = m_vec - v.m_vec;
   }
   else {
      d.m_cellIsValid = false;
   }
   return d;
}

//DC7u& DC7u::operator= (const G6& v) {
//   *this = DC7u(LRL_Cell(v));
//   return *this;
//}
//
//DC7u& DC7u::operator= (const S6& v) {
//   *this = DC7u(LRL_Cell(v));
//   return *this;
//}
//
//DC7u& DC7u::operator= (const C3& v) {
//   *this = DC7u(LRL_Cell(v));
//   return *this;
//}
//
//DC7u& DC7u::operator= (const D7& v) {
//   *this = DC7u(LRL_Cell(v));
//   return *this;
//}
//
//DC7u& DC7u::operator= (const B4& v) {
//   *this = DC7u(LRL_Cell(v));
//   return *this;
//}

double DistanceBetween(const DC7u& v1, const DC7u& v2) {
   return (v1 - v2).Norm();
}

DC7u operator* (const double d, const DC7u& dc7u) {
   return dc7u * d;
}

bool DC7u::IsReduced() const {
   const DC7u& dc7u = *this;
   int error = 0;
   const double delta = std::fabs(dc7u[DC7u_AA_idx]) * 1.e-10;
   for (size_t ii = 0; ii < 7; ii++) {
      if (dc7u[ii] < delta) error++;
   }
   if (dc7u[DC7u_AA_idx] > dc7u[DC7u_BB_idx] + delta ||
      dc7u[DC7u_BB_idx] > dc7u[DC7u_CC_idx] + delta) error++;
   return error == 0;
}