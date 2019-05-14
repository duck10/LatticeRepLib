#ifndef MATS6_H
#define MATS6_H

#include <string>
#include <vector>

#include "LRL_MatrixBase.h"

//#include "LRL_Cell.h"
//#include "MatG6.h"
#include "MatN.h"
//#include "MatMN.h"
//#include "VecN.h"
//#include "D7.h"

class LRL_Cell;
class S6;
class D7;
class G6;
class B4;
//class MatE3;
class MatG6;

MatS6 operator* (const double d, const MatS6& m);
MatS6 operator/ (const double d, const MatS6& m);

class MatS6 : public LRL_MatrixBase<S6, MatS6> {
public:
   friend class D7;

   MatS6(void);
   MatS6(const double s1, const double s2, const double s3, const double s4, const double s5, const double s6, const double s7, const double s8, const double s9, const double s10, const double s11, const double s12, const double s13, const double s14, const double s15, const double s16, const double s17, const double s18, const double s19, const double s20, const double s21, const double s22, const double s23, const double s24, const double s25, const double s26, const double s27, const double s28, const double s29, const double s30, const double s31, const double s32, const double s33, const double s34, const double s35, const double s36);
   MatS6(const MatG6& m);
   MatS6(const MatS6& m);
   MatS6(const MatD7& m);
   MatS6(const MatN& m);
	MatS6(const MatMN& m);
	//MatS6(const MatE3& m);
	MatS6(const std::string& s);
   MatS6(const std::vector<double>& v);

	static MatS6 e3Tos6(const std::vector<double>& m);

   MatS6& operator= (const MatB4& v);
   MatS6& operator= (const MatS6& v);
   MatS6& operator= (const MatD7& v);
   MatS6& operator= (const MatG6& v);

   MatS6& operator+= (const MatS6& d);
   MatS6& operator-= (const MatS6& d);
   bool operator== (const MatS6& m) const;
   bool operator!= (const MatS6& m) const;
   MatS6& operator*= (const double d);
   MatS6& operator/= (const double d);

   MatS6 operator+ (const MatS6& v) const;
   MatS6 operator- (const MatS6& v) const;
   MatS6 operator- (void) const; // unary
   MatS6 operator* (const MatS6& m2) const;

   MatS6 operator* (const double d) const;
   MatS6 operator/ (const double d) const;

   S6 operator* (const S6& v) const;

   double operator[] (const unsigned long n) const;
   double& operator[] (const unsigned long n);

   double DistanceBetween(const MatS6& v1, const MatS6& v2);
   unsigned long size(void) const;
   double norm() const;
   double norm(const MatS6& t) const;
   double Norm() const;
   double Norm(const MatS6& t) const;

   unsigned long GetRowDim(void) const { return 6; }
   virtual bool IsUnit() const;
   static MatS6 Eye(void);
   static MatS6 Zero(void);
   std::vector<double> GetVector(void) const { return m_mat.GetVector(); }
   void SetVector(const std::vector<double>& v) { m_mat.SetVector(v); }

   MatS6 transpose(const MatS6& m) const;
   void transpose(void);

   double at(const unsigned long n) const;

   MatS6 unit(void);
   static MatS6 unit(const MatS6& m);

   MatS6 Reduce(void) const;
   void ReplaceTwosInRows7x7(MatS6& m);

   MatN GetMatrix(void) const { return m_mat; }

   static std::vector<MatS6> GetReflections();
   static MatS6 GetReflection(const unsigned long n);

   static std::string GetName() { return "MatS6"; }

private:
   MatN m_mat;
   static std::vector<MatS6> vS6_Refl;
};


#endif  //  MATS6_H
