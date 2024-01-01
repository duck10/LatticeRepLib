#ifndef RI_H
#define RI_H


#ifdef _MSC_VER

#define USE_LOCAL_HEADERS

#endif

#ifndef USE_LOCAL_HEADERS

//#include <rhrand.h>

#else

#include "rhrand.h"

#endif


#include "BasisBase.h"

#include "VecN.h"
#include "LRL_indices.h"

class LRL_Cell;
class B4;
class C3;
class D7;
class G6;
class S6;
class DC7u;

#include <ostream>
#include <string>

class RI /*: private BasisBase<RI> */{
public:
   friend std::ostream& operator<< (std::ostream&, const RI&);
   friend RI operator* (const double d, const RI& v);
   friend RI operator/ (const double d, const RI& v);
   friend class S6;

   RI(void);
   RI(const RI& v);
   RI(const double v[6]);
   RI(const double d1, const double d2, const double d3, const double d4, const double d5, const double d6);
   RI(const D7& v);
   RI(const DC7u& dc7u);
   RI(const D13& d13);
   RI(const LRL_Cell& c);
   RI(const VecN& v);
   RI(const S6& ds);
   RI(const C3& c3);
   RI(const B4& dt);
   RI(const std::string& s);
   RI(const std::vector<double>& v);

   RI& operator= (const RI& v);
   RI& operator= (const std::string& s);
   RI& operator= (const D7& v);
   RI& operator= (const DC7u& dc7u);
   RI& operator= (const S6& v);
   RI& operator= (const C3& c3);
   RI& operator= (const B4& v);
   RI& operator= (const LRL_Cell& v);
   RI& operator/= (const double d);
   RI& operator*= (const double d);
   RI& operator+= (const RI& v);
   RI& operator-= (const RI& v);
   RI operator+ (const RI& v) const;
   RI operator- (const RI& v) const;
   RI operator* (const double d) const;
   RI operator/ (const double d) const;
   RI operator- (void) const; // unary
   bool operator== (const RI& ri) const;
   bool operator!= (const RI& ri) const;

   double operator[](const size_t n) const { return m_vec[n]; }
   double& operator[](const size_t n) { return m_vec[n]; }
   double DistanceBetween(const RI& v1, const RI& v2);
   std::vector<std::pair<std::string, std::string> > ClassifyVector(const double delta) const;
   bool TestBoundaryDetection(void);
   size_t size(void) const { return 6; }
   double norm() const { return m_vec.norm(); }
   static double norm(const RI& ri) { return ri.norm(); }
   double Norm() const { return m_vec.norm(); }
   static double Norm(const RI& ri) { return ri.norm(); }

   RI InverseRIVector(void) const;
   RI sort(const RI& ri);
   std::vector<double> GetVector(void) const { return m_vec.GetVector(); }
   double* data() const { return const_cast<double*>(m_vec.data()); }
   void SetVector(const std::vector<double>& v) { m_vec = v; }
   bool GetValid(void) const { return m_valid; }
   bool IsValid(void) const { return m_valid; }
   void SetValid(const bool b) { m_valid = b; }

   double at(const size_t n) const { return m_vec[n]; }
   static std::string GetName(void) { return "RI"; }

   static RI rand();
   static RI randDeloneReduced();
   static RI randDeloneUnreduced();
   static RI rand(const double d);
   static RI randDeloneReduced(const double d);
   static RI randDeloneUnreduced(const double d);
   static std::vector<S6> ResetZeros(const std::vector<S6>& vs);
   static S6 ResetZeros(const S6& vs);
   static S6 AllPositive(const S6& s);
   static int CountZeros(const S6& s6);
   static std::pair<S6, std::string>   MakeRI(const LRL_ReadLatticeData& input, const S6& positiveRed);


protected:
   VecN m_vec;
   size_t m_dim;
   bool m_valid;

   static std::vector<S6> DoSqrt(const std::vector<S6>& vs);
   static std::vector<S6> GenerateReflections(const S6& s6);
   static std::vector<S6> KeepDminFirst(const std::vector<S6>& vs);
   static std::vector<S6> KeepSecondMin(const std::vector<S6>& vs);
   static std::vector<S6> ResortSecondElement(const std::vector<S6>& vs);
   static std::vector<S6> ResortElements_1_3(const std::vector<S6>& vs);
   static std::vector<S6> ResortFirstElement(const std::vector<S6>& vs);
   static std::vector<S6> EliminateDuplicates(const std::vector<S6>& vs);
   static S6 Resort_V3(const S6& s6);
   static S6 Resort_V1(const S6& s6);
   static S6 Resort_V2(const S6& s6);
   static void OutputVector(const std::vector<S6>& vs);
   static std::string OutputRootInvariants(const S6& s6);
   static std::string Format_V1(const S6& s);
   static std::string Format_V2(const S6& s);
   static std::string Format_V4(const S6& s);
   static std::string Format_V3(const S6& s);
   static std::string Format_V5(const S6& s);


   };

#endif
