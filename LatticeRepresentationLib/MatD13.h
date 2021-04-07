#ifndef MATD13_H
#define MATD13_H

#include <string>
#include <vector>

#include "D13.h"
#include "LRL_MatrixBase.h"
#include "MatD13.h"
#include "MatN.h"

class MatD13/* : private LRL_MatrixBase<D13, MatD13>*/ {
public:
   friend std::ostream& operator<< (std::ostream&, const MatD13&);

   MatD13();
   void zeros();
   double operator[] (const size_t n) const;
   double& operator[] (const size_t n);
   std::vector<double> GetVector() const { return m_mat.GetVector(); }

   MatD13 operator+ (const MatD13& m) const;
   MatD13 operator- (const MatD13& m) const;
   MatD13 operator- (void) const; // unary

   double norm() const;
   double norm(const MatD13& t) const;
   double Norm() const;
   double Norm(const MatD13& t) const;

   MatD13& operator+= (const MatD13& d);
   MatD13& operator-= (const MatD13& d);

   size_t size() const { return 13 * 13; }

private:
   MatN m_mat;
};


#endif   // MATD13_H