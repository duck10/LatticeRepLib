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
class MatG6;

MatS6 operator* (const double d, const MatS6& m);
MatS6 operator/ (const double d, const MatS6& m);

class MatS6 : public LRL_MatrixBase<S6, MatS6> {
public:
   friend class D7;

   MatS6(void);
   MatS6(const MatG6& m);
   MatS6(const MatS6& m);
   MatS6(const MatD7& m);
   MatS6(const MatN& m);
   MatS6(const MatMN& m);
   MatS6(const std::string& s);
   MatS6(const std::vector<double>& v);

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

   static std::vector<MatS6> GetReflections();
   static MatS6 GetReflection(const unsigned long n);

private:
   MatN m_mat;
   static std::vector<MatS6> vS6_Refl;
};


#endif  //  MATS6_H
