#pragma warning( disable : 4189) //  local variable is initialized but not referenced


#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "LRL_Cell.h"
#include "Delone.h"
#include "D7.h"
#include "DC7u.h"
#include "G6.h"
#include "LRL_DataToSVG.h"
#include "LRL_RandTools.h"
#include "MatS6.h"
#include "MatD7.h"
//#include "MatRI.h" // Rename MatG6 to MatRI
#include "rhrand.h"
#include "S6L.h" // Rename G6 to S6L
#include "S6.h"
#include "Selling.h"
#include "LRL_StringTools.h"
#include "LRL_Vector3.h"

static const std::vector<MatS6> refl_one = MatS6::GetReflections();

S6L::S6L(void) {
   m_vec.resize(6);
   m_dim = 6;
   m_valid = true;
}

S6L::S6L(const S6L& v)
   : S6L()
{
   m_vec = v.m_vec;
   m_valid = v.m_valid;
}

S6L::S6L(const double v[6])
   : S6L()
{
   m_dim = 6;
   m_vec.resize(6);
   throw; // "needs to implemented"
}

S6L::S6L(const double d1, const double d2, const double d3, const double d4, const double d5, const double d6)
   :m_dim(6)
{
   m_vec = std::vector<double>{ d1,d2,d3,d4,d5,d6 };
}

S6L::S6L(const LRL_Cell& c) {
   static const double pi = 4.0 * atan(1.0);
   static const double twopi = 2.0 * pi;
   m_dim = 6;
   m_vec.resize(6);
   throw; // "needs to implemented"

   bool b1 = c.GetValid();
   bool b2 = c[3] < pi;
   bool b3 = c[4] < pi;
   bool b4 = c[5] < pi;
   bool b5 = (c[3] + c[4] + c[5]) < twopi;
   m_valid = b1 && b2 && b3 && b4 && b5;
   m_vec.SetValid(m_valid);
}

S6L::S6L(const S6& ds)
   :m_dim(6)
   , m_valid(true)
{
   S6 s6out;
   const bool b = Selling::Reduce(ds, s6out);

   const double s1 = sqrt(-s6out[0]);
   const double s2 = sqrt(-s6out[1]);
   const double s3 = sqrt(-s6out[2]);
   const double s4 = sqrt(-s6out[3]);
   const double s5 = sqrt(-s6out[4]);
   const double s6 = sqrt(-s6out[5]);
   m_vec = std::vector<double>{s1, s2, s3, s4, s5, s6};
}

S6L& S6L::operator= (const S6L& s6l) {
   const  S6L out(s6l[0], s6l[1], s6l[2], s6l[3], s6l[4], s6l[5]);
   m_vec = out.m_vec;
   m_dim = out.m_dim;
   m_valid = out.m_valid;
   return *this;
}

std::vector<S6> S6L::GenerateReflections(const S6& s6) {
   std::vector<S6> out;
   for (size_t i = -0; i < refl_one.size(); ++i) {
      out.emplace_back(refl_one[i] * s6);
   }
   return out;
}

S6L::S6L(const VecN& v) {
   m_dim = 0;
   m_valid = false;
   if (v.size() == 6) {
      m_dim = 6;
      m_valid = true;
      m_vec = v;
   }
}

S6L::S6L(const std::vector<double>& v) {
   m_dim = 0;
   m_valid = false;
   if (v.size() == 6) {
      m_dim = 6;
      m_valid = true;
      m_vec = v;
   }
}

void S6L::OutputVector(const std::vector<S6>& vs) {
   for (size_t i = 0; i < vs.size(); ++i) {
      std::cout << vs[i] << std::endl;
   }
}

std::vector<S6L> S6L::ResetZeros(const std::vector<S6L>& vs) {
   std::vector<S6L> out;
   for (size_t kkk = 0; kkk < vs.size(); ++kkk)
   {
      S6 s6;
      for (size_t i = 0; i < 6; ++i) {
         s6[i] = (abs(vs[kkk][i]) < 1.0E-6) ? 0 : vs[kkk][i];
      }
      out.emplace_back(s6);
   }
   return out;
}

S6 S6L::ResetZeros(const S6L& vs) {
   S6 s6;
   for (size_t i = 0; i < 6; ++i) {
      s6[i] = (abs(vs[i]) < 1.0E-6) ? 0 : vs[i];
   }
   return s6;
}

std::vector<S6> S6L::DoSqrt(const std::vector<S6>& vs) {
   std::vector<S6> out;
   for (size_t i = 0; i < vs.size(); ++i) {
      const S6& s(vs[i]);
      S6 temp(sqrt(s[0]), sqrt(s[1]), sqrt(s[2]), sqrt(s[3]), sqrt(s[4]), sqrt(s[5]));
      out.emplace_back(temp);
   }
   return out;
}

std::vector<S6> S6L::EliminateDuplicates(const std::vector<S6>& vs) {
   std::vector<S6> out(1, vs[0]);
   for (size_t i = 1; i < vs.size(); ++i) {
      std::vector<S6> toAdd(1, vs[i]);
      for (size_t kkk = 0; kkk < out.size(); ++kkk)
      {
         const double d = (vs[i] - out[kkk]).norm();
         if (d < 1.0E-6) {
            toAdd.clear();
            break;
         }
      }
      out.insert(out.end(), toAdd.begin(), toAdd.end());
   }
   return out;
}

S6L S6L::operator* (const double d) const {
   S6L S6L(*this);
   S6L.m_vec *= d;
   return S6L;
}

S6L S6L::operator/ (const double d) const {
   S6L S6L(*this);
   S6L.m_vec /= d;
   return S6L;
}

S6L operator*(const double d, const S6L& ri) {
   return ri * d;
}

S6L operator/(const double d, const S6L& ri) {
   return ri / d;
}

std::ostream& operator<< (std::ostream& o, const S6L& v) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(3);
   for (size_t i = 0; i < v.size(); ++i)
      o << std::setw(9) << v[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

