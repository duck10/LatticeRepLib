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

   double norm() const;
   double norm(const MatDC7u& t) const;
   double Norm() const;
   double Norm(const MatDC7u& t) const;

   MatDC7u& operator+= (const MatDC7u& d);
   MatDC7u& operator-= (const MatDC7u& d);

   size_t size() const { return 7 * 7; }

private:
   MatN m_mat;
};


#endif   // MATDC7u_H
