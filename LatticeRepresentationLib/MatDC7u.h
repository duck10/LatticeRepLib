#ifndef MATDC7u_H
#define MATDC7u_H

#include <string>
#include <vector>

#include "DC7u.h"
#include "LRL_MatrixBase.h"
#include "MatDC7u.h"
#include "MatN.h"

class MatDC7u : private LRL_MatrixBase<DC7u, MatDC7u> {
public:
   //friend std::ostream& operator<< (std::ostream&, const MatDC7u&);
   friend MatDC7u operator* (const double d, const MatDC7u& m);
   //{
   //   return m * d;
   //}

   MatDC7u();
   MatDC7u(const std::string& s);
   MatDC7u(const std::vector<double>& v);

   MatDC7u& operator= (const MatB4& m);
   MatDC7u& operator= (const MatG6& m);
   MatDC7u& operator= (const MatS6& m);
   MatDC7u& operator= (const MatD7& m);
   MatDC7u& operator= (const MatDC7u& m);
   void zeros();
   double operator[] (const size_t n) const;
   double& operator[] (const size_t n);

   double DistanceBetween(const MatDC7u& v1, const MatDC7u& v2);
   std::vector<double> GetVector() const { return m_mat.GetVector(); }

   MatDC7u operator+ (const MatDC7u& m) const;
   MatDC7u operator- (const MatDC7u& m) const;
   MatDC7u operator- (void) const; // unary
   MatDC7u operator* (const MatDC7u& m2) const;
   double norm() const;
   double norm(const MatDC7u& t) const;
   double Norm() const;
   double Norm(const MatDC7u& t) const;

   size_t GetRowDim(void) const { return 7; }
   size_t GetColDim(void) const { return 7; }
   virtual bool IsUnit() const;
   static MatDC7u Eye(void);
   static MatDC7u Zero(void);
   void SetVector(const std::vector<double>& v) { m_mat.SetVector(v); }
   bool GetValid(void) const { return m_cellIsValid; }
   void SetValid(const bool b) { m_cellIsValid = b; }
   MatDC7u& operator+= (const MatDC7u& d);
   MatDC7u& operator-= (const MatDC7u& d);
   bool operator== (const MatDC7u& m) const;
   bool operator!= (const MatDC7u& m) const;
   MatDC7u& operator/= (const double d);
   MatDC7u& operator*= (const double d);

   MatDC7u operator* (const double d) const;
   MatDC7u operator/ (const double d) const;

   DC7u operator* (const DC7u& v) const;


   size_t size() const { return 7 * 7; }

   MatDC7u transpose(const MatDC7u& m) const;
   void transpose(void);

   double at(const size_t n) const {  return m_mat[n];
   }

   MatDC7u unit(void);
   static MatDC7u unit(const MatDC7u& m);
   MatN GetMatrix(void) const { return m_mat; }

   char GetFtype() const { return m_Ftype; }
   void SetFtype(const char c) { m_Ftype = c; }
   std::string GetDescr() const { return m_descr; }
   void SetDescr(const std::string& s) { m_descr = s; }

   static std::vector<MatDC7u> MakeBoundaryMatrices();
   bool ShouldApply(const G6& g) const;
   std::string GetName() const { return m_name; }
   void SetName(const std::string& s) { m_name = s; }

private:
   MatN m_mat;
   static std::vector<MatDC7u> vDU_Refl;
   bool m_cellIsValid;
   char m_Ftype = '0'; // 3 choices can be used
                        // '+'  for +++ matrix that applies to cells
                        // '-'  for --- matrix that applies to cells
                        // '0' for matrices that can be used with both
   std::string m_descr;
   std::string m_name;
};


#endif   // MATDC7u_H
