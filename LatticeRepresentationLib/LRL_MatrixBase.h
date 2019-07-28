#ifndef LRL_MATRIXBASE_H
#define LRL_MATRIXBASE_H

#include <string>

class MatG6;
class MatN;
class MatMN;
class VecN;

class LRL_Cell;
class S6;
class D7;
class B4;
class G6;

class MatB4;
class MatS6;
class MatD7;
class MatG6;

#include <iostream>

template< typename TVEC, typename TMAT, typename TELEMENT=double>
class LRL_MatrixBase {
public:

   friend std::ostream& operator<< (std::ostream& o, const TMAT& m) {
      for (size_t i = 0; i<m.size(); ++i) {
         if (i > 0 && (i % (m.GetRowDim()) == 0)) o << std::endl;
         o << m[i] << " ";
      }
      return o;
   }

   virtual ~LRL_MatrixBase() {}

   virtual TMAT& operator= (const MatB4& m) = 0;
   virtual TMAT& operator= (const MatS6& m) = 0;
   virtual TMAT& operator= (const MatD7& m) = 0;
   //virtual TMAT& operator= (const LRL_Cell& v) = 0;
   virtual TMAT& operator= (const MatG6& m) = 0;

   virtual TMAT& operator+= (const TMAT& d) = 0;
   virtual TMAT& operator-= (const TMAT& d) = 0;
   virtual TMAT& operator*= (const TELEMENT d) = 0;
   virtual TMAT& operator/= (const TELEMENT d) = 0;

   virtual bool operator== (const TMAT& m) const = 0;
   virtual bool operator!= (const TMAT& m) const = 0;

   virtual TMAT operator+ (const TMAT& m) const = 0;
   virtual TMAT operator- (const TMAT& m) const = 0;
   virtual TMAT operator- (void) const = 0;  // unary
   virtual TMAT operator* (const TMAT& m2) const = 0;

   virtual TMAT operator* (const TELEMENT d) const = 0;
   virtual TMAT operator/ (const TELEMENT d) const = 0;

   virtual TVEC operator* (const TVEC& v) const = 0;

   virtual TELEMENT operator[] (const size_t n) const = 0;
   virtual TELEMENT& operator[] (const size_t n) = 0;

   virtual double DistanceBetween(const TMAT& v1, const TMAT& v2) = 0;
   virtual size_t size(void) const = 0;
   virtual double norm() const = 0;
   virtual double norm(const TMAT& t) const = 0;
   virtual double Norm() const = 0;
   virtual double Norm(const TMAT& t) const = 0;

   virtual size_t GetRowDim(void) const = 0;
   virtual bool IsUnit() const = 0;

   virtual std::vector<TELEMENT> GetVector(void) const = 0;
   virtual TMAT transpose(const TMAT& m) const = 0;
   virtual void transpose(void) = 0;

   virtual TELEMENT at(const size_t n) const = 0;

};

#endif  // LRL_MATRIXBASE_H

