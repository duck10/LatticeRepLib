#ifndef MATG6_H
#define MATG6_H

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

MatG6 operator* (const double d, const MatG6& m);
MatG6 operator/ (const double d, const MatG6& m);

class MatG6 : public LRL_MatrixBase<G6, MatG6> {
public:
   friend class D7;

   MatG6(void);
   MatG6(const MatG6& m);
   MatG6(const MatS6& m);
   MatG6(const MatD7& m);
   MatG6(const MatN& m);
   MatG6(const MatMN& m);
   MatG6(const std::string& s);
   MatG6(const std::vector<double>& v);

   MatG6& operator= (const MatB4& m);
   MatG6& operator= (const MatS6& m);
   MatG6& operator= (const MatD7& m);
   MatG6& operator= (const MatG6& m);

   MatG6& operator+= (const MatG6& d);
   MatG6& operator-= (const MatG6& d);
   bool operator== (const MatG6& m) const;
   bool operator!= (const MatG6& m) const;
   MatG6& operator*= (const double d);
   MatG6& operator/= (const double d);

   MatG6 operator+ (const MatG6& m) const;
   MatG6 operator- (const MatG6& m) const;
   MatG6 operator- (void) const; // unary
   MatG6 operator* (const MatG6& m2) const;

   MatG6 operator* (const double d) const;
   MatG6 operator/ (const double d) const;

   G6 operator* (const G6& v) const;

   double operator[] (const unsigned long n) const;
   double& operator[] (const unsigned long n);

   double DistanceBetween(const MatG6& v1, const MatG6& v2);
   unsigned long size(void) const;
   double norm() const;
   double norm(const MatG6& t) const;
   double Norm() const;
   double Norm(const MatG6& t) const;

   unsigned long GetRowDim(void) const { return 6; }
   virtual bool IsUnit() const;
   static MatG6 Eye(void);
   static MatG6 Zero(void);
   std::vector<double> GetVector(void) const { return m_mat.GetVector(); }
   void SetVector(const std::vector<double>& v) { m_mat.SetVector(v); }

   MatG6 transpose(const MatG6& m) const;
   void transpose(void);

   double at(const unsigned long n) const;

   MatG6 unit(void);
   static MatG6 unit(const MatG6& m);

   void ReplaceTwosInRows7x7(MatG6& m);

   MatN GetMatrix(void) const { return m_mat; }

   static std::vector<MatG6> GetReflections();
   static MatG6 GetReflection(const unsigned long n);

private:
   MatN m_mat;
   static std::vector<MatG6> vG6_Refl;
};

#endif  //  MATG6_H