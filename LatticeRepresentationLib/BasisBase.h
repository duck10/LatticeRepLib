#ifndef BASISBASE_H
#define BASISBASE_H

#include <string>
#include <vector>

class LRL_Cell;
class C3;
class S6;
class D7;
class D13;
class B4;
class G6;
class VecN;

template<typename T>
T CreateUnitOrthogonalVector(const T& t) {
   // assume t is not null and rand is not parallel to t
   T base = t / t.norm();
   T temp(T::rand());
   temp /= temp.norm();
   const double d = dot(VecN(base.GetVector()), VecN(temp.GetVector()));
   const T ortho = temp - d * base;
   return ortho / ortho.norm();
}

template<typename T1, typename T2>
double dot(const T1& t1, const T2& t2) {
   const T1 t1x(t2);
   const size_t n1 = t1.size();
   const size_t n2 = t2.size();
   if (n1 != n2) return -19199;
   double sum = 0.0;
   for (size_t i = 0; i < n1; ++i)
      sum += t1[i] * t1x[i];
   return sum;
}

template<typename T, typename TELEMENT=double>
class BasisBase {
public:

   BasisBase() {}
   virtual ~BasisBase(void) {}

   virtual T& operator= (const std::string& s) = 0;
   virtual T& operator= (const D7& v) = 0;
   virtual T& operator= (const S6& v) = 0;
   virtual T& operator= (const B4& v) = 0;
   virtual T& operator= (const LRL_Cell& v) = 0;
   virtual T& operator= (const G6& v) = 0;
   virtual T& operator= (const C3& v) = 0;
   //virtual T& operator= (const D13& v) = 0;

   virtual T& operator/= (const double d) = 0;
   virtual T& operator*= (const double d) = 0;
   virtual T operator+ (const T& v) const = 0;
   virtual T operator- (const T& v) const = 0;
   virtual T operator* (const double d) const = 0;
   virtual T operator/ (const double d) const = 0;
   virtual T operator- (void) const = 0; // unary
   virtual T& operator+= (const T& v) = 0;
   virtual T& operator-= (const T& v) = 0;
   virtual bool operator== (const T& v) const = 0;
   virtual bool operator!= (const T& v) const = 0;

   virtual TELEMENT operator[](const size_t n) const = 0;
   virtual TELEMENT& operator[](const size_t n) = 0;
   //virtual double DistanceBetween(const T& v1, const T& v2) = 0; // this needs to be a friend
   virtual size_t size(void) const = 0;
   virtual double norm() const = 0;
   virtual double norm(const T& t) const = 0;
   virtual double Norm() const = 0;
   virtual double Norm(const T& t) const = 0;

   virtual std::vector<TELEMENT> GetVector(void) const = 0;
   virtual void SetVector(const std::vector<TELEMENT>& v) = 0;
   virtual void SetValid(const bool b) = 0;

   virtual TELEMENT at(const size_t n) const {
      return (*this)[n];
   }

   virtual TELEMENT* data(void) const = 0;
};

#endif  // BASISBASE_H
