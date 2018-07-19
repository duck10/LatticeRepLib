#include "Cell.h"
#include "D6.h"
#include "DeloneTetrahedron.h"
#include "G6.h"
#include "D7.h"
#include "vector_3d.h"

std::ostream& operator<< (std::ostream& o, const DeloneTetrahedron& dt) {
   for (size_t i = 0; i < 4; ++i)
      o << dt.m_vec[i];
   return o;
}

DeloneTetrahedron::DeloneTetrahedron(void) {
   m_vec.resize(4);
}

//                   DeloneTetrahedron(const DeloneTetrahedron& dy);
DeloneTetrahedron::DeloneTetrahedron(const DeloneTetrahedron& dt) 
   : m_vec(dt.m_vec)
{
   m_vec.resize(4);
}

DeloneTetrahedron::DeloneTetrahedron(const G6& v6) {
   m_vec.resize(4);
   (*this) = DeloneTetrahedron(Cell(v6));
}

DeloneTetrahedron::DeloneTetrahedron(const D7& v7) {
   m_vec.resize(4);
   (*this) = DeloneTetrahedron( Cell(v7));
}

DeloneTetrahedron::DeloneTetrahedron(const std::string& v7) {
   m_vec.resize(4);
   throw;
}

const double rad = 180.0 / (4 * atan(1.0));
DeloneTetrahedron::DeloneTetrahedron(const Cell& c) {
   m_vec.resize(4);
   const Matrix_3x3 amat = Matrix_3x3::Cart(c[0], c[1], c[2], c[3] * rad, c[4] * rad, c[5] * rad);

   (*this).m_vec[0] = amat * Vector_3(1, 0, 0);
   (*this).m_vec[1] = amat * Vector_3(0, 1, 0);
   (*this).m_vec[2] = amat * Vector_3(0, 0, 1);
   (*this).m_vec[3] = -(m_vec[0]+m_vec[1]+m_vec[2]);
}

DeloneTetrahedron::DeloneTetrahedron(const D6& ds) {
   m_vec.resize(4);
   (*this) = DeloneTetrahedron(Cell(ds));
}

Vector_3 DeloneTetrahedron::operator[] (const size_t n) const {
   return m_vec[n];
}

Vector_3& DeloneTetrahedron::operator[] (const size_t n) {
   return m_vec[n];
}

DeloneTetrahedron& DeloneTetrahedron::operator= (const std::string& s)
{
   throw;
   return *this;
}

DeloneTetrahedron DeloneTetrahedron::operator+ (const DeloneTetrahedron& dt) const {
   DeloneTetrahedron temp;
   for (size_t i = 0; i<4; ++i)
      temp[i] = (*this)[i] + dt[i];
   return temp;
}

DeloneTetrahedron DeloneTetrahedron::operator- (const DeloneTetrahedron& dt) const {
   DeloneTetrahedron temp;
   for ( size_t i=0; i<4; ++i )
   temp[i] = (*this)[i] - dt[i];
   return temp;
}

DeloneTetrahedron DeloneTetrahedron::operator* (const double d) const
{
   DeloneTetrahedron dt;
   for (size_t i = 0; i < dt.size(); ++i)
      dt[i] = ((*this)[i]) * d;
   return dt;
}

DeloneTetrahedron operator* (const double d, const DeloneTetrahedron& dt)
{
   return (dt*d);
}

bool DeloneTetrahedron::operator== (const DeloneTetrahedron& dt)
{
   bool breturn = true;
   for (size_t i = 0; i < 4; ++i)
      breturn = breturn && (*this)[i] == dt[i];
   return breturn;
}

bool DeloneTetrahedron::operator!= (const DeloneTetrahedron& dt)
{
   return !((*this) == dt);
}

double DistanceBetween(const DeloneTetrahedron& v1, const DeloneTetrahedron& v2) {
   return (v1 - v2).norm();
}


double DeloneTetrahedron::norm(void) const {
   return norm(*this);
}

double DeloneTetrahedron::norm(const DeloneTetrahedron& dt) const {
   const double norm1 = dt[0].Norm();
   const double norm2 = dt[1].Norm();
   const double norm3 = dt[2].Norm();
   const double norm4 = dt[3].Norm();
   return norm1 + norm2 + norm3 + norm4;
}
