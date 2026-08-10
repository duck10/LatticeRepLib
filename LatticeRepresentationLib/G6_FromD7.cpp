// Conversions between G6 and D7.
// Kept in its own translation unit so that programs which never construct
// a G6 from a D7 do not need to compile or link against D7.h (and
// whatever D7.h pulls in transitively).

#include "G6.h"
#include "D7.h"

G6::G6(const D7& v7) {
   m_vec.resize(6);
   double& g1 = m_vec[0];
   double& g2 = m_vec[1];
   double& g3 = m_vec[2];
   double& g4 = m_vec[3];
   double& g5 = m_vec[4];
   double& g6 = m_vec[5];
   const double& d1 = v7[0];
   const double& d2 = v7[1];
   const double& d3 = v7[2];
   const double& b4 = v7[3];
   const double& d5 = v7[4];
   const double& s6 = v7[5];
   const double& d7 = v7[6];
   g1 = d1;
   g2 = d2;
   g3 = d3;
   g4 = d5 - d2 - d3;
   g5 = s6 - d1 - d3;
   g6 = d7 - d1 - d2;
   //   g4 = (b4 -g1 -g2 -g3 -g5 -g6 + d5 - g2 -g3)/2.0; 
   m_valid = g1 > 0.001 && g2 > 0.001 && g3 > 0.001 && v7.GetValid();
   m_dim = 6;
}

G6& G6::operator= (const D7& v)
{
   (*this) = G6(v);
   m_vec.SetValid(v.GetValid());
   return *this;
}
