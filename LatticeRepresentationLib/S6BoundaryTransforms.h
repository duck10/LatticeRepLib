#ifndef S6BOUNDARYTRANSFORMS_H
#define S6BOUNDARYTRANSFORMS_H

#include "MatS6.h"

#include <set>
#include <vector>

class S6BoundaryTransforms {
public:
   S6BoundaryTransforms();
   std::vector<MatS6> GetSixBoundaries() const;
   std::vector<MatS6> GetBoundaries(const size_t n) const;

   std::vector<std::vector<MatS6> >  GetVector() const { return m_vvm; }
   
   std::vector<MatS6> GetVector(const size_t n) const { return m_vvm[n]; }
   std::vector<MatS6> operator[] (const size_t n) { return m_vvm[n]; }

   MatS6 GetOneTransform(const size_t n) const;

   //std::vector<MatS6> Prod(const size_t n, const MatS6& m) const;
   //std::vector<MatS6> Prod(const size_t n, const std::vector<MatS6>& m) const;
   //std::vector<MatS6> Prod(const size_t n1, const size_t n2, const std::vector<MatS6>& m) const;
   //std::vector<MatS6> Prod(const size_t n1, const size_t n2, const size_t n3, const std::vector<MatS6>& m) const;


   std::vector<S6> operator*(const S6& s6);
   friend std::vector<S6> operator*(const size_t n, const S6& s6);
   S6 Transform(const size_t nBound, const size_t nInBound, const S6& s6);
   std::vector<std::vector<S6> > TransformBounds(const S6& s6);


   size_t size() const { return m_vvm.size(); }

private:
   std::vector<std::vector<MatS6> > m_vvm;   
   static std::vector<MatS6> m_SixTransforms;
};

class S6BoundaryMatricesBase {
public:
   virtual std::vector<MatS6> GetVector() const;
protected:
   S6BoundaryMatricesBase();
   
protected:
   std::vector<MatS6> m_mats;
};

class S6BoundaryMatricesZero : public S6BoundaryMatricesBase {
public:
   S6BoundaryMatricesZero();
private:
};

class S6BoundaryMatricesOne : public S6BoundaryMatricesBase {
public:
   S6BoundaryMatricesOne(const size_t n1);
private:
};

class S6BoundaryMatricesTwo : public S6BoundaryMatricesBase {
public:
   S6BoundaryMatricesTwo(const size_t n1, const size_t n2);
   S6BoundaryMatricesTwo(const std::set<size_t>& exclusions, const std::vector<size_t>& zeros);
private:
   void ProcessWithTwoZeros(const std::set<size_t>& exclusions, const std::vector<size_t>& zeros);
};

class S6BoundaryMatricesThree : public S6BoundaryMatricesBase {
public:
   S6BoundaryMatricesThree(const size_t n1, const size_t n2, const size_t n3);
   S6BoundaryMatricesThree(const std::vector<MatS6>& v1, const std::vector<MatS6>& v2);
private:
};






#endif // S6BOUNDARYTRANSFORMS_H
