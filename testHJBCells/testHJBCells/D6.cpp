#include "Cell.h"
#include "D6.h"
#include "DeloneTetrahedron.h"
#include "Vec_N_Tools.h"
#include "vector_3d.h"



#include <iomanip>
#include <string>

D6::D6(void)
   : VecN(6) {
   m_dim = 6;
   m_valid = true;
}

D6::D6(const D6& v)
   : VecN(6) {
   m_dim = 6;
   m_vec = v.m_vec;
   m_valid = v.m_valid;
}

D6::D6(const double v[6]) {
   double& p = (*this)[0];
   double& q = (*this)[1];
   double& r = (*this)[2];
   double& s = (*this)[3];
   double& t = (*this)[4];
   double& u = (*this)[5];

   const double& g1 = v[0];
   const double& g2 = v[1];
   const double& g3 = v[2];
   const double& g4 = v[3];
   const double& g5 = v[4];
   const double& g6 = v[5];

   p = g4 / 2.0;
   q = g5 / 2.0;
   r = g6 / 2.0;
   s = (-2.0*g1 - g6 - g5) / 2.0;
   t = (-g6 - 2.0*g2 - g4) / 2.0;
   u = (-g5 - g4 - 2.0*g3) / 2.0;
}

D6::D6(const G6& v6) 
   : VecN(6) {

   const double& g1 = v6[0];
   const double& g2 = v6[1];
   const double& g3 = v6[2];
   const double& g4 = v6[3];
   const double& g5 = v6[4];
   const double& g6 = v6[5];

    double& p = (*this)[0];
    double& q = (*this)[1];
    double& r = (*this)[2];
    double& s = (*this)[3];
    double& t = (*this)[4];
    double& u = (*this)[5];
    
    p = g4 / 2.0;
    q = g5 / 2.0;
    r = g6 / 2.0;
    s = (-2.0*g1 - g6 - g5) / 2.0;
    t = (-g6 - 2.0*g2 - g4) / 2.0;
    u = (-g5 - g4 - 2.0*g3) / 2.0;
}


D6::D6(const D7& v7) {
   m_dim = 6;
   (*this) = G6(v7);
   m_valid = true;
}

D6::D6( const DeloneTetrahedron& del ) {

   const Vector_3 a = del[size_t(0)];
   const Vector_3 b = del[size_t(1)];
   const Vector_3 c = del[size_t(2)];
   const Vector_3 d = del[size_t(3)];
   double& p = (*this)[0];
   double& q = (*this)[1];
   double& r = (*this)[2];
   double& s = (*this)[3];
   double& t = (*this)[4];
   double& u = (*this)[5];

   p = b.Dot(c);
   q = a.Dot(c);
   r = a.Dot(b);
   s = a.Dot(d);
   t = b.Dot(d);
   u = c.Dot(d);
}

D6::D6(const Cell& c) {
   (*this) = G6(c.Cell2V6());
}

D6::D6(const VecN& v)
   : VecN(v) {
   if (v.size() == 6) {
      G6 g6(v);
      const D6 v6(g6);
      m_dim = 6;
      m_vec.resize(m_dim);
      for (size_t i = 0; i<m_dim; ++i) m_vec[i] = v6[i];
}
   m_valid = true;
}

D6::D6(const std::string& s)
   : VecN(6)
{
   m_vec = Vec_N_Tools::FromString(s);
   m_dim = m_vec.size();
   m_valid = true;
   if (m_dim != 6) throw "bad dimension in D6 from a string";
}

D6::D6(const std::vector<double>& v)
   : VecN(v.size())
{
   m_dim = v.size();
   if (m_dim != 6) throw "bad dimension in D7 from a string";
   m_vec = v;
   m_valid = true;
}

double D6::DistanceBetween( const D6& v1, const D6& v2 ) {
   double sum = 0.0;
   const size_t size1 = sizeof(v1);
   const size_t size2 = sizeof(v1[0]);
   for( size_t i=0; i<6; ++i )  sum += (v1[i]-v2[i])*(v1[i]-v2[i]);
   return sqrt( sum );
}

D6& D6::operator= ( const D6& v ) {
   const double d = v[0];
   m_vec = v.m_vec;
   m_dim = v.m_dim;
   m_valid = v.m_valid;
   return *this;
}
D6& D6::operator= (const std::string& s)
{
   (*this) = D6(s);
   return *this;
}

std::ostream& operator<< (std::ostream& o, const D6& dc) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(5);
   for( size_t i = 0; i < dc.size(); ++i)
      o << std::setw(9) << dc[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}
//
//D6 D6::operator- (const D6& ds) const {
//   if (ds.size() != 6) throw;
//   if ((*this).size() != 6) throw;
//   D6 v;
//   for (size_t i = 0; i < 6; ++i)
//      v[i] = (*this)[i] - ds[i];
//   return v;
//}
//
//D6 D6::operator+ (const D6& ds) const {
//   if (ds.size() != 6) throw;
//   if ((*this).size() != 6) throw;
//   D6 v;
//   for (size_t i = 0; i < 6; ++i)
//      v[i] = (*this)[i] + ds[i];
//   return v;
//}
//
//D6 operator* (const double d, const D6& ds)
//{
//   return ds * d;
//}
//
//D6 D6::operator* (const double& d) const
//{
//   D6 dt(*this);
//   for (size_t i = 0; i < 6; ++i)
//      dt[i] *= d;
//   return dt;
//}
//
//
//double D6::norm(const D6& ds) const {
//   if (ds.size() != 6) throw;
//   if ((*this).size() != 6) throw;
//   double d=0.0;
//   for (size_t i = 0; i < 6; ++i)
//      d += std::abs(ds[i]);
//   return d;
//}
//
//double D6::norm(void) const {
//   if ((*this).size() != 6) throw;
//   double d=0.0;
//   for (size_t i = 0; i < 6; ++i)
//      d += std::abs((*this)[i]);
//   return d;
//}
