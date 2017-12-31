#ifndef MatD7XH
#define MatD7XH

#include <cmath>
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

MatD7 operator* (const double d, const MatD7& m);
MatD7 operator/ (const double d, const MatD7& m);

class MatD7 : public LRL_MatrixBase<D7,MatD7> {
public:
   friend class D7;

   MatD7(void);
   MatD7(const MatG6& m);
   MatD7(const MatS6& m);
   MatD7(const MatD7& m);
   MatD7(const MatN& m);
   MatD7(const MatMN& m);
   MatD7(const std::string& s);
   MatD7(const std::vector<double>& v);

   MatD7& operator= (const MatB4& v);
   MatD7& operator= (const MatS6& v);
   MatD7& operator= (const MatD7& v);
   MatD7& operator= (const MatG6& v);

   MatD7& operator+= (const MatD7& d);
   MatD7& operator-= (const MatD7& d);
   bool operator== (const MatD7& m) const;
   bool operator!= (const MatD7& m) const;
   MatD7& operator*= (const double d);
   MatD7& operator/= (const double d);

   MatD7 operator+ (const MatD7& v) const;
   MatD7 operator- (const MatD7& v) const;
   MatD7 operator- (void) const; // unary
   MatD7 operator* (const MatD7& m2) const;

   MatD7 operator* (const double d) const;
   MatD7 operator/ (const double d) const;

   D7 operator* (const D7& v) const;

   double operator[] (const unsigned long n) const;
   double& operator[] (const unsigned long n);

   double DistanceBetween(const MatD7& v1, const MatD7& v2);
   unsigned long size(void) const;
   double norm() const;
   double norm(const MatD7& t) const;
   double Norm() const;
   double Norm(const MatD7& t) const;

   unsigned long GetRowDim(void) const { return 7; }
   virtual bool IsUnit() const;
   static MatD7 Eye(void);
   static MatD7 Zero(void);

   std::vector<double> GetVector(void) const { return m_mat.GetVector(); }
   void SetVector(const std::vector<double>& v) { m_mat.SetVector(v); }
   MatD7 transpose(const MatD7& m) const;
   void transpose(void);

   double at(const unsigned long n) const;

   MatD7 unit(void);
   static MatD7 unit(const MatD7& m);

   MatD7 Reduce(void) const;
   void ReplaceTwosInRows7x7(MatD7& m);
   static std::vector<MatD7> GetReflections();
   static MatD7 GetReflection(const unsigned long n);

private:
   MatN m_mat;
   static std::vector<MatD7> vD7_Refl;
};

#endif

