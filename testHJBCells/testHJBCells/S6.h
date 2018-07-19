#ifndef S6_H
#define S6_H

#include "VecN.h"

#include <ostream>
#include <string>

class S6 {
public:
   friend std::ostream& operator<< (std::ostream&, const S6&);
   friend S6 operator* (const double d, const S6& ds);
   friend class D7;

   S6(void);
   S6(const double v[6]);
   S6(const S6& s6);
   S6(const VecN& v);
   S6(const std::vector<double>& v);
   S6(const double d1, const double d2, const double d3, const double d4, const double d5, const double d6);
   ~S6(void) {}


   S6& operator= (const S6& v);
   S6& operator/= (const double d);
   S6& operator*= (const double d);
   S6 operator+ (const S6& ds) const;
   S6 operator- (const S6& ds) const;
   S6& operator+= (const S6& ds);
   S6& operator-= (const S6& ds);
   S6 operator* (const double d) const;
   S6 operator/ (const double d) const;
   S6 operator- (void) const; // unary
   bool operator== (const S6& s6) const;
   bool operator!= (const S6& s6) const;

   double operator[](const size_t n) const { return m_vec[n]; }
   double& operator[](const size_t n) { return m_vec[n]; }
   double DistanceBetween(const S6& v1, const S6& v2);
   size_t size(void) const { return 6; }
   double norm() const { return m_vec.norm(); }
   double norm(const S6& s6) const { return norm(s6); }
   double Norm() const { return m_vec.norm(); }
   double Norm(const S6& s6) const { return norm(s6); }

   std::vector<double> GetVector(void) const { return m_vec.GetVector(); }
   double* data() const { return const_cast<double*>(m_vec.data()); }
   void SetVector(const std::vector<double>& v) { m_vec = v; }
   bool GetValid(void) const { return m_valid; }
   void SetValid(const bool b) {m_valid = b; }
   bool IsValid(void) const;

   double at(const size_t n) const { return m_vec[n]; }

   static S6 rand();
   static S6 randDeloneReduced();
   static S6 randDeloneUnreduced();
   static S6 rand(const double d);
   static S6 randDeloneReduced(const double d);
   static S6 randDeloneUnreduced(const double d);

   bool IsAllMinus() const;
   static std::string GetName(void) { return "S6, Selling scalars"; }
   static S6 InvertCoord(const size_t n, const S6& din);
   S6 InvertCoord(const size_t n) const;
   static std::string Signature(const S6& s6);

   static std::vector< S6(*)(const S6&)> SetReduceFunctions();
   static S6 Reduce11(const S6& din);
   static S6 Reduce12(const S6& din);
   static S6 Reduce21(const S6& din);
   static S6 Reduce22(const S6& din);
   static S6 Reduce31(const S6& din);
   static S6 Reduce32(const S6& din);
   static S6 Reduce41(const S6& din);
   static S6 Reduce42(const S6& din);
   static S6 Reduce51(const S6& din);
   static S6 Reduce52(const S6& din);
   static S6 Reduce61(const S6& din);
   static S6 Reduce62(const S6& din);

   static std::vector< S6(*)(const S6&)> SetUnreduceFunctions();
   static S6 Unreduce11(const S6& din);
   static S6 Unreduce12(const S6& din);
   static S6 Unreduce21(const S6& din);
   static S6 Unreduce22(const S6& din);
   static S6 Unreduce31(const S6& din);
   static S6 Unreduce32(const S6& din);
   static S6 Unreduce41(const S6& din);
   static S6 Unreduce42(const S6& din);
   static S6 Unreduce51(const S6& din);
   static S6 Unreduce52(const S6& din);
   static S6 Unreduce61(const S6& din);
   static S6 Unreduce62(const S6& din);
   static size_t CountPositive(const S6& s6);
   size_t CountZeros(void) const;

   static std::vector< S6(*)(const S6&)> SetRelectionFunctions();
   static S6 Relection01(const S6& din);
   static S6 Relection02(const S6& din);
   static S6 Relection03(const S6& din);
   static S6 Relection04(const S6& din);
   static S6 Relection05(const S6& din);
   static S6 Relection06(const S6& din);
   static S6 Relection07(const S6& din);
   static S6 Relection08(const S6& din);
   static S6 Relection09(const S6& din);
   static S6 Relection10(const S6& din);
   static S6 Relection11(const S6& din);
   static S6 Relection12(const S6& din);
   static S6 Relection13(const S6& din);
   static S6 Relection14(const S6& din);
   static S6 Relection15(const S6& din);
   static S6 Relection16(const S6& din);
   static S6 Relection17(const S6& din);
   static S6 Relection18(const S6& din);
   static S6 Relection19(const S6& din);
   static S6 Relection20(const S6& din);
   static S6 Relection21(const S6& din);
   static S6 Relection22(const S6& din);
   static S6 Relection23(const S6& din);
   static S6 Relection24(const S6& din);

private:
   static S6 RandomUnreduceOne(const S6& s6);
   static S6 RandomUnreduceTwo(const S6& s6);
   static S6 RandomUnreduceThree(const S6& s6);

   VecN m_vec;
   size_t m_dim;
   bool m_valid;
};


#endif
