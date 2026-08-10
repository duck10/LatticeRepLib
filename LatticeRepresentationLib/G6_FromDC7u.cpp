// Conversion from DC7u to G6.
// Kept in its own translation unit so that programs which never construct
// a G6 from a DC7u do not need to compile or link against DC7u.h (and
// whatever DC7u.h pulls in transitively, e.g. Niggli.h).

#include "G6.h"
#include "DC7u.h"

#include <cmath>
#include <iostream>

G6::G6(const DC7u& dc7u)
: m_dim(6)
{
   double delta;
   double u, v, w, testsign;
   int error, ii;
   m_vec.resize(6);
   error=0;
   delta=std::fabs(dc7u[DC7u_AA_idx])*1.e-10;
   for (ii=0;ii<7;ii++) {
     if (dc7u[ii]<delta) error++;
   }
   if (dc7u[DC7u_AA_idx] > dc7u[DC7u_BB_idx]+delta ||
       dc7u[DC7u_BB_idx] > dc7u[DC7u_CC_idx]+delta ) error++;
   if (error > 0) {
     std::cout <<  "unreduced DC7 cell" << std::endl;
     //m_valid=false;
   } 
     m_vec[G6_AA_idx]=dc7u[DC7u_AA_idx];
     m_vec[G6_BB_idx]=dc7u[DC7u_BB_idx];
     m_vec[G6_CC_idx]=dc7u[DC7u_CC_idx];
     u=dc7u[DC7u_MIN_BC_diagsq_idx]
       -dc7u[DC7u_BB_idx]-dc7u[DC7u_CC_idx];
     v=dc7u[DC7u_MIN_AC_diagsq_idx]
       -dc7u[DC7u_AA_idx]-dc7u[DC7u_CC_idx];
     w=dc7u[DC7u_MIN_AB_diagsq_idx]
       -dc7u[DC7u_AA_idx]-dc7u[DC7u_BB_idx];
     testsign=dc7u[DC7u_MIN_ABC_diagsq_idx]
             -(m_vec[G6_AA_idx]
              +m_vec[G6_BB_idx]
              +m_vec[G6_CC_idx]+u+v+w);
     if ((std::fabs(testsign)<=delta)
        || (std::fabs(u)<=delta)
        || (std::fabs(v)<=delta)
        || (std::fabs(w)<=delta)) {
        m_vec[G6_2BC_idx]=u;
        m_vec[G6_2AC_idx]=v;
        m_vec[G6_2AB_idx]=w;
      } else {
        m_vec[G6_2BC_idx]=-u;
        m_vec[G6_2AC_idx]=-v;
        m_vec[G6_2AB_idx]=-w;
      }
      if (dc7u[DC7u_AA_idx]>dc7u[DC7u_BB_idx]+delta ||
        dc7u[DC7u_BB_idx]>dc7u[DC7u_CC_idx]+delta ) error++;
      if (error > 0) {
        //for (ii=0;ii<6;ii++) m_vec[ii]=0.;
        // m_valid=false;
      } else {
        m_vec[G6_AA_idx]=dc7u[DC7u_AA_idx];
        m_vec[G6_BB_idx]=dc7u[DC7u_BB_idx];
        m_vec[G6_CC_idx]=dc7u[DC7u_CC_idx];
        u=dc7u[DC7u_MIN_BC_diagsq_idx]
          -dc7u[DC7u_BB_idx]-dc7u[DC7u_CC_idx];
        v=dc7u[DC7u_MIN_AC_diagsq_idx]
          -dc7u[DC7u_AA_idx]-dc7u[DC7u_CC_idx];
        w=dc7u[DC7u_MIN_AB_diagsq_idx]
          -dc7u[DC7u_AA_idx]-dc7u[DC7u_BB_idx];
        testsign=dc7u[DC7u_MIN_ABC_diagsq_idx]
             -(m_vec[G6_AA_idx]
              +m_vec[G6_BB_idx]
              +m_vec[G6_CC_idx]+u+v+w);
        if ((std::fabs(testsign)<=delta)
            || (std::fabs(u)<=delta)
            || (std::fabs(v)<=delta)
            || (std::fabs(w)<=delta)) {
          m_vec[G6_2BC_idx]=u;
          m_vec[G6_2AC_idx]=v;
          m_vec[G6_2AB_idx]=w;
        } else {
          m_vec[G6_2BC_idx]=-u;
          m_vec[G6_2AC_idx]=-v;
          m_vec[G6_2AB_idx]=-w;
        }
        m_valid=true;
      }
}
