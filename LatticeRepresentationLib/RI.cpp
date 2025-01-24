#pragma warning( disable : 4189) //  local variable is initialized but not referenced

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <utility>
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
#include "RI.h" // Rename G6 to RI
#include "S6.h"
#include "Selling.h"
#include "LRL_StringTools.h"
#include "LRL_Vector3.h"

static const std::vector<MatS6> refl_one = MatS6::GetReflections();

RI::RI(void) 
   :m_dim(6)
   , m_valid(true)
{
   m_vec.resize(6);
}

RI::RI(const RI& v)
   : RI()
{
   m_vec = v.m_vec;
   m_valid = v.m_valid;
}

RI::RI(const double v[6])
   : RI()
{
   m_dim = 6;
   m_vec.resize(6);
   const LRL_ReadLatticeData input;
   const std::pair<S6, std::string> pss = MakeRI(input, v);
   m_vec = pss.first.GetVector();
}

RI::RI(const double d1, const double d2, const double d3, const double d4, const double d5, const double d6)
   :m_dim(6)
{
   m_vec = std::vector<double>{ d1,d2,d3,d4,d5,d6 };
}

RI::RI(const LRL_Cell& c) {
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
   const LRL_ReadLatticeData input;
   const std::pair<S6, std::string> pss = MakeRI(input, c);
   m_vec = pss.first.GetVector();
}

RI::RI(const S6& ds)
{
   const LRL_ReadLatticeData input;
   const std::pair<S6, std::string> pss = MakeRI(input, -ds);
   m_vec = pss.first.GetVector();

   //throw; // this needs to be implemented differently
   // here is the S6L code
   //S6L::S6L(const S6 & ds)
   //   :m_dim(6)
   //   , m_valid(true)
   //{
   //   S6 s6out;
   //   const bool b = Selling::Reduce(ds, (s6out));
   //   const double s1 = sqrt(-s6out[0]);
   //   const double s2 = sqrt(-s6out[1]);
   //   const double s3 = sqrt(-s6out[2]);
   //   const double s4 = sqrt(-s6out[3]);
   //   const double s5 = sqrt(-s6out[4]);
   //   const double s6 = sqrt(-s6out[5]);
   //   m_vec = std::vector<double>{ s1, s2, s3, s4, s5, s6 };
   //}

}

RI::RI(const G6& ds)
{
   const LRL_ReadLatticeData input;
   const std::pair<S6, std::string> pss = MakeRI(input, ds);
   m_vec = pss.first.GetVector();
}

std::vector<S6> RI::GenerateReflections(const S6& s6) {
   std::vector<S6> out;
   for (size_t i = -0; i < refl_one.size(); ++i) {
      out.emplace_back(refl_one[i] * s6);
   }
   return out;
}

RI::RI(const VecN& v) {
   m_dim = 0;
   m_valid = false;
   if (v.size() == 6) {
      m_dim = 6;
      m_valid = true;
      const LRL_ReadLatticeData input;
      const std::pair<S6, std::string> pss = MakeRI(input, v);
      m_vec = pss.first.GetVector();
   }
}

RI::RI(const std::vector<double>& v) {
   m_dim = 6;
   if (v.size() == 6) {
      m_valid = true;
      m_vec = v;
   }
   else {
      m_valid = false;
      m_vec.resize(6);

   }
}

RI& RI::operator=(const RI& v)
{
   m_vec = v.m_vec;
   m_dim = v.m_dim;
   m_valid = v.m_valid;
   return *this;
}

std::string RI::Format_V1(const S6& s) {
   //std::cout << "; Root Invariant for V2 " << LRL_ToString("{", s6[1], "(", s6[2], s6[3], s6[4], ")", , s6[5], "}") << std::endl;
   const double& s1 = s[0];
   const double& s2 = s[1];
   const double& s3 = s[2];
   const double& s4 = s[3];
   const double& s5 = s[4];
   const double& s6 = s[5];
   //; Root Invariant for V2 { 1.62541 ( 2.86617 2.24793 5.37153 ) 4.4525 }
   const std::string descr = " Root Invariant for V1 ";
   const std::string ss1 = LRL_DataToSVG("{", s1, ", ");
   const std::string ss2 = LRL_DataToSVG(s2, ", ");
   const std::string ss3 = LRL_DataToSVG(s3, ", ");
   const std::string ss4 = LRL_DataToSVG(s4, ", ");
   const std::string ss5 = LRL_DataToSVG(s5, ", ");
   const std::string ss6 = LRL_DataToSVG(s6, "}");
   const std::string out(descr + ss1 + ss2 + ss3 + ss4 + ss5 + ss6);
   return out;
}

std::string RI::Format_V2(const S6& s) {
   //std::cout << "; Root Invariant for V2 " << LRL_ToString("{", s6[1], "(", s6[2], s6[3], s6[4], ")", , s6[5], "}") << std::endl;
   //const double& s1 = s[0];
   const double& s2 = s[1];
   const double& s3 = s[2];
   const double& s4 = s[3];
   const double& s5 = s[4];
   const double& s6 = s[5];
   //; Root Invariant for V2 { 1.62541 ( 2.86617 2.24793 5.37153 ) 4.4525 }
   const std::string descr = " Root Invariant for V2 ";
   const std::string ss2 = LRL_DataToSVG("(", s2, ", ");
   const std::string ss3 = LRL_DataToSVG(s3, ", ");
   const std::string ss4 = LRL_DataToSVG("{", s4, ", ");
   const std::string ss5 = LRL_DataToSVG(s5, "), ");
   const std::string ss6 = LRL_DataToSVG(s6, "}");
   const std::string out(descr + ss4 + ss2 + ss3 + ss5 + ss6);
   return out;
}

std::string RI::Format_V3(const S6& s) {
   //const double& s1 = s[0];
   const double& s2 = s[1];
   const double& s3 = s[2];
   const double& s5 = s[4];
   const double& s6 = s[5];
   //; Root Invariant for V3
   const std::string descr = " Root Invariant for V3 ";
   const std::string ss3 = LRL_DataToSVG("{", s2, ", ");
   const std::string ss4 = LRL_DataToSVG(s3, ", ");
   const std::string ss5 = LRL_DataToSVG(s5, ", ");
   const std::string ss6 = LRL_DataToSVG(s6, "}");
   const std::string out(descr + ss3 + ss4 + ss5 + ss6);
   return out;
}

std::string RI::Format_V4(const S6& s) {
   //std::cout << "; Root Invariant for V2 " << LRL_ToString("{", s6[1], "(", s6[2], s6[3], s6[4], ")", , s6[5], "}") << std::endl;
   const double& s1 = s[0];
   const double& s2 = s[1];
   const double& s3 = s[2];
   const double& s4 = s[3];
   const double& s5 = s[4];
   const double& s6 = s[5];
   std::vector<double> vd{ s3, s4, s5 };
   std::sort(vd.begin(), vd.end());
   S6 out;
   out[0] = s1;
   out[1] = s2;
   out[2] = vd[0];
   out[3] = vd[1];
   out[4] = vd[2];
   out[5] = s6;
   //; Root Invariant for V4
   const std::string descr = " Root Invariant for V4 ";
   const std::string ss3 = LRL_DataToSVG("{(", out[2], ", ");
   const std::string ss4 = LRL_DataToSVG(out[3], ", ");
   const std::string ss5 = LRL_DataToSVG(out[4], "), ");
   const std::string ss6 = LRL_DataToSVG(s6, "}");
   const std::string strout(descr + ss3 + ss4 + ss5 + ss6);
   return strout;
}

std::string RI::Format_V5(const S6& s) {
   //const double& s1 = s[0];
   //const double& s2 = s[1];
   const double& s4 = s[3];
   const double& s5 = s[4];
   const double& s6 = s[5];
   //; Root Invariant for V2 { 1.62541 ( 2.86617 2.24793 5.37153 ) 4.4525 }
   const std::string descr = " Root Invariant for V5 ";
   const std::string ss4 = LRL_DataToSVG("{", s4, ", ");
   const std::string ss5 = LRL_DataToSVG(s5, ", ");
   const std::string ss6 = LRL_DataToSVG(s6, "}");
   const std::string out(descr + ss4 + ss5 + ss6);
   return out;
}

std::vector<S6> RI::ResortFirstElement(const std::vector<S6>& vs) {
   std::vector<S6> out;
   for (size_t i = 0; i < vs.size(); ++i)
   {
      S6 s = vs[i];
      const double d = s[0] - s[1];
      if (d < 1.0E-6) {
         const double smin = std::min(s[3], s[4]);
         const double smax = std::max(s[3], s[4]);
         s[3] = smin;
         s[4] = smax;
      }
      out.emplace_back(s);
   }
   return out;
}

std::string  RI::OutputRootInvariants(const S6& s6) {
   const int zeroCount = CountZeros(s6);
   std::string out;
   if (zeroCount == 0) {
      out = Format_V1(s6);
   }
   else if (zeroCount == 1) {
      out = Format_V2(Resort_V2(s6));
   }
   else if (zeroCount == 2 && s6[1] == 0.0) {
      out = Format_V4(s6);
   }
   else if (zeroCount == 2) {
      out = Format_V3(s6);
   }
   else if (zeroCount == 3) {
      out = Format_V5(s6);
   }
   else {
      //throw; "not supposed to happen";
   }
   //std::cout << out << std::endl;
   return out;
}

void RI::OutputVector(const std::vector<S6>& vs) {
   for (size_t i = 0; i < vs.size(); ++i) {
      std::cout << vs[i] << std::endl;
   }
}

std::pair<S6, std::string>   RI::MakeRI(const LRL_ReadLatticeData& input, const S6& positiveRed) {
   //// this is the calculation (at least my attempt) to compute Bright and Kurlin's "Root Invariant"

   // generate 24 reflections
   const std::vector<S6> allRefls = RI::GenerateReflections(positiveRed);
   const std::vector<S6> sqrted = DoSqrt(allRefls);
   const std::vector<S6> resetZeros1 = ResetZeros(sqrted);

   // treat all zero vectors
   if (resetZeros1.empty()) {
      return std::make_pair(S6{0, 0, 0, 0, 0, 0 }, "");
   }
   // remove unsort examples
   const std::vector<S6> firstElementDmin = KeepDminFirst(resetZeros1);
   // treat all zero vectors
   if (firstElementDmin.empty()) {
      return std::make_pair(S6{ 0, 0, 0, 0, 0, 0 }, "");
   }
   const std::vector<S6> secondElement = KeepSecondMin(firstElementDmin);
   // set near zeros to zero (probably not needed)
   if (secondElement.empty()) {
      return std::make_pair(S6{ 0, 0, 0, 0, 0, 0 }, "");
   }
   const std::vector<S6> resetZeros2 = ResetZeros(secondElement);
   if (resetZeros2.empty()) {
      return std::make_pair(S6{ 0, 0, 0, 0, 0, 0 }, "");
   }
   // remove actual near duplicates
   const std::vector<S6> allSorted = ResortElements_1_3(resetZeros2);
   // remove actual near duplicates, because sorting values can generate them
   const std::vector<S6> noDups2 = RI::EliminateDuplicates(allSorted);
   std::vector<S6> likeRI(noDups2);

   if (likeRI.size() > 1 && likeRI[0][0] == 0.0 && likeRI[0][3] == 0) {
      const S6 s6 = RI::Resort_V3(likeRI[0]);
      likeRI.clear();
      likeRI.emplace_back(s6);
   }
   else if (likeRI.size() > 1 && CountZeros(likeRI[0]) == 0) {
      const S6 s6 = RI::Resort_V1(likeRI[0]);
      likeRI.clear();
      likeRI.emplace_back(s6);
   }
   else if (likeRI.size() > 1 && likeRI[0][0] == 0.0 && CountZeros(likeRI[0]) == 1) {
      const S6 s6 = RI::Resort_V2(likeRI[0]);
      likeRI.clear();
      likeRI.emplace_back(s6);
   }

   if (likeRI.size() > 1 || likeRI.empty()) {
      //std::cout << input.GetStrCell() << std::endl;
      RI::OutputVector(likeRI);
      throw; "bad likeRI";
   }
   std::vector<S6> summary;
   //summary.emplace_back(likeRI[0]);

   for (size_t i = 0; i < likeRI.size(); ++i) {
      const std::string RI = RI::OutputRootInvariants(likeRI[i]);
      //const std::string line = "; SL " + LRL_ToString(likeRI[i], RI);
   }
   const std::pair<S6, std::string> pss = std::make_pair(likeRI[0], RI::OutputRootInvariants(likeRI[0]));

   return pss;
}

RI RI::rand() {
   const S6 s6rand{ S6::randDeloneReduced() };
   const std::pair<S6, std::string> pss = MakeRI(LRL_ReadLatticeData(), -s6rand);
   const RI ri = pss.first;
   return ri;
}

std::vector<S6> RI::ResetZeros(const std::vector<S6>& vs) {
   std::vector<S6> out;
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

S6 RI::ResetZeros(const S6& vs) {
   S6 s6;
   for (size_t i = 0; i < 6; ++i) {
      s6[i] = (abs(vs[i]) < 1.0E-6) ? 0 : vs[i];
   }
   return s6;
}

std::vector<S6> RI::DoSqrt(const std::vector<S6>& vs) {
   std::vector<S6> out;
   for (size_t i = 0; i < vs.size(); ++i) {
      const S6& s(vs[i]);
      S6 temp(sqrt(s[0]), sqrt(s[1]), sqrt(s[2]), sqrt(s[3]), sqrt(s[4]), sqrt(s[5]));
      out.emplace_back(temp);
   }
   return out;
}

std::vector<S6> RI::KeepDminFirst(const std::vector<S6>& vs) {
   std::vector<S6> out;
   const std::vector<double> vd = vs[0].GetVector();
   const double dmin = minNC(vd[0], vd[1], vd[2], vd[3], vd[4], vd[5]);

   for (size_t i = 0; i < vs.size(); ++i)
   {
      const S6 s = vs[i];
      const double d = s[0] - dmin;
      if (d < 1.0E-6) {
         out.emplace_back(s);
      }
   }
   return out;
}

std::vector<S6> RI::KeepSecondMin(const std::vector<S6>& vs) {
   std::vector<S6> out;
   const std::vector<double> vd = vs[0].GetVector();
   const double dmin = minNC(vd[1], vd[2], vd[4], vd[5]);

   for (size_t i = 0; i < vs.size(); ++i)
   {
      const S6 s = vs[i];
      const double d = s[1] - dmin;
      if (d < 1.0E-6) {
         out.emplace_back(s);
      }
   }
   return out;
}


std::vector<S6> RI::ResortSecondElement(const std::vector<S6>& vs) {
   std::vector<S6> out;
   for (size_t i = 0; i < vs.size(); ++i)
   {
      S6 s = vs[i];
      const double d = s[2] - s[3];
      if (d < 1.0E-6) {
         const double smin = std::min(s[4], s[5]);
         const double smax = std::max(s[4], s[5]);
         s[4] = smin;
         s[5] = smax;
      }
      out.emplace_back(s);
   }
   return out;
}

std::vector<S6> RI::ResortElements_1_3(const std::vector<S6>& vs) {
   const std::vector<S6> sort1 = RI::ResortFirstElement(vs);
   const std::vector<S6> sort2 = RI::ResortSecondElement(sort1);
   const std::vector<S6> sort3 = RI::ResortFirstElement(sort2);
   const std::vector<S6> sort4 = RI::ResortFirstElement(sort3);
   return sort4;
}

S6 RI::MakeAllPositive(const S6& s) {
   S6 out(s);
   for (size_t i = 0; i < 6; ++i) {
      out[i] = -out[i];
   }
   return out;
}

int RI::CountZeros(const S6& s6) {
   int count = 0;
   for (size_t i = 0; i < 6; ++i)
      if (s6[i] == 0.0) ++count;
   return count;
}


std::vector<S6> RI::EliminateDuplicates(const std::vector<S6>& vs) {
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

S6 RI::Resort_V3(const S6& s6) {

   S6 out(s6);
   std::vector<double> vd{ out[1], out[2], out[4], out[5] };
   std::sort(vd.begin(), vd.end());
   out[1] = vd[0];
   out[2] = vd[1];
   out[4] = vd[2];
   out[5] = vd[3];
   return out;
}

S6 RI::Resort_V1(const S6& s6) {

   S6 out(s6);
   if (abs(out[0] - out[3]) < 1.0E-5 && out[3] > out[4])
   {
      std::swap(out[3], out[4]);

   }
   else if (abs(out[1] - out[4]) < 1.0E-5 && out[4] > out[5])
   {
      std::swap(out[4], out[5]);
   }
   return out;
}

S6 RI::Resort_V2(const S6& s6) {

   S6 out;
   //std::cout << " s6 entering Resort_V2  " << s6 << std::endl;
   std::vector<double> vd{ s6[1], s6[2], s6[4] };
   std::sort(vd.begin(), vd.end());
   //std::cout << "vd in Resort_V2 " << LRL_ToString(vd ) << std::endl;
   out[0] = s6[0];
   out[1] = vd[0]; //this is wrong and incomplete !!!!!!!!!!!!!!!!!!!!!!
   out[2] = vd[1];
   out[3] = s6[3];
   out[4] = vd[2];
   out[5] = s6[5];
   //std::cout << " final out in Resort_V2 " << out << std::endl << std::endl;
   return out;
}



RI RI::operator* (const double d) const {
   RI RI(*this);
   RI.m_vec *= d;
   return RI;
}

RI RI::operator/ (const double d) const {
   RI RI(*this);
   RI.m_vec /= d;
   return RI;
}

RI operator*(const double d, const RI& ri) {
   return ri*d;
}

RI operator/(const double d, const RI& ri) {
   return ri / d;
}

std::ostream& operator<< (std::ostream& o, const RI& v) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(3);
   for (size_t i = 0; i < v.size(); ++i)
      o << std::setw(9) << v[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

