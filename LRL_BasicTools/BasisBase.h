#ifndef BASISBASE_H
#define BASISBASE_H

#include <string>
#include <vector>

class LRL_Cell;
class S6;
class D7;
class B4;
class G6;
class VecN;

template<typename T>
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

   virtual T& operator/= (const double d) = 0;
   virtual T& operator*= (const double d) = 0;
   virtual T operator+ (const T& v) const = 0;
   virtual T operator- (const T& v) const = 0;
   virtual T operator* (const double d) const = 0;
   virtual T operator/ (const double d) const = 0;
   virtual T operator- (void) const = 0; // unary
   virtual T operator+= (const T& v) = 0;
   virtual T operator-= (const T& v) = 0;
   virtual bool operator== (const T& v) const = 0;
   virtual bool operator!= (const T& v) const = 0;

   virtual double operator[](const unsigned long n) const = 0;
   virtual double& operator[](const unsigned long n) = 0;
   virtual double DistanceBetween(const T& v1, const T& v2) = 0;
   virtual unsigned long size(void) const = 0;
   virtual double norm() const = 0;
   virtual double norm(const T& t) const = 0;
   virtual double Norm() const = 0;
   virtual double Norm(const T& t) const = 0;

   virtual std::vector<double> GetVector(void) const = 0;
   virtual void SetVector(const std::vector<double>& v) = 0;
   virtual void SetValid(const bool b) = 0;

   virtual double at(const unsigned long n) const = 0;

   virtual double* data(void) const = 0;
};

#endif  // BASISBASE_H