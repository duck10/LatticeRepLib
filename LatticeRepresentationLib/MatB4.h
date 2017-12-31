#ifndef MatB4_H
#define MatB4_H

#include <string>
#include <vector>

#include "B4.h"
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

MatB4 operator* (const double d, const MatB4& m);
MatB4 operator/ (const double d, const MatB4& m);

class MatB4 : public LRL_MatrixBase<B4, MatB4> {
public:

   MatB4(void);
   MatB4(const MatG6& m);
   MatB4(const MatS6& m);
   MatB4(const MatD7& m);
   MatB4(const MatB4& m);
   MatB4(const MatN& m);
   //MatB4(const MatMN& m);
   MatB4(const std::string& s);
   MatB4(const std::vector<double>& v);

   MatB4& operator= (const MatB4& v);
   MatB4& operator= (const MatS6& v);
   MatB4& operator= (const MatD7& v);
   MatB4& operator= (const MatG6& v);

   MatB4& operator+= (const MatB4& d);
   MatB4& operator-= (const MatB4& d);
   MatB4& operator*= (const double d);
   MatB4& operator/= (const double d);
   bool operator== (const MatB4& m) const;
   bool operator!= (const MatB4& m) const;

   MatB4 operator+ (const MatB4& v) const;
   MatB4 operator- (const MatB4& v) const;
   MatB4 operator- (void) const; // unary
   MatB4 operator* (const MatB4& m2) const;

   MatB4 operator* (const double d) const;
   MatB4 operator/ (const double d) const;

   B4 operator* (const B4& v) const;

   double operator[] (const unsigned long n) const;
   double& operator[] (const unsigned long n);

   double DistanceBetween(const MatB4& v1, const MatB4& v2);
   unsigned long size(void) const;
   double norm() const;
   double norm(const MatB4& t) const;
   double Norm() const;
   double Norm(const MatB4& t) const;

   unsigned long GetRowDim(void) const { return 4; }
   virtual bool IsUnit() const;
   static MatB4 Eye(void);
   static MatB4 Zero(void);
   std::vector<double> GetVector(void) const { return m_mat.GetVector(); }

   MatB4 transpose(const MatB4& m) const;
   void transpose(void);

   double at(const unsigned long n) const;

   MatB4 unit(void);
   static MatB4 unit(const MatB4& m);

   std::vector<double> GetVector(void) { return m_mat.GetMatrix(); }
   void SetVector(const std::vector<double>& v) { m_mat.SetVector(v); }
 
   static MatB4 GetReflection(const unsigned long n);
   static std::vector<MatB4>  GetReflections();

private:
   MatN m_mat;
   static std::vector<MatB4> vB4_Refl;
};


#endif  // MatB4_H

