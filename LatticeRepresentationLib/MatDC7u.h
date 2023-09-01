#ifndef MATDC7u_H
#define MATDC7u_H

#include <string>
#include <vector>

#include "DC7u.h"
#include "LRL_MatrixBase.h"
#include "MatDC7u.h"
#include "MatN.h"

class MatDC7u/* : private LRL_MatrixBase<DC7u, MatDC7u>*/ {
public:
   friend std::ostream& operator<< (std::ostream&, const MatDC7u&);

   MatDC7u();
   void zeros();
   double operator[] (const size_t n) const;
   double& operator[] (const size_t n);
   std::vector<double> GetVector() const { return m_mat.GetVector(); }

   MatDC7u operator+ (const MatDC7u& m) const;
   MatDC7u operator- (const MatDC7u& m) const;
   MatDC7u operator- (void) const; // unary
   MatDC7u operator* (const MatDC7u& m2) const;
   double norm() const;
   double norm(const MatDC7u& t) const;
   double Norm() const;
   double Norm(const MatDC7u& t) const;

   size_t GetRowDim(void) const { return 6; }
   size_t GetColDim(void) const { return 6; }
   virtual bool IsUnit() const;
   static MatDC7u Eye(void);
   static MatDC7u Zero(void);
   void SetVector(const std::vector<double>& v) { m_mat.SetVector(v); }
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

   double at(const size_t n) const;

   MatDC7u unit(void);
   static MatDC7u unit(const MatDC7u& m);
   MatN GetMatrix(void) const { return m_mat; }
   static std::string GetName() { return "MatDC7u"; }
private:
   MatN m_mat;
   static std::vector<MatDC7u> vDU_Refl;
};


#endif   // MATDC7u_H
