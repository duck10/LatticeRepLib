#ifndef DC7U_H
#define DC7U_H

#include <vector>

#include "BasisBase.h"

#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"
#include "LRL_indices.h"

class DC7u : private BasisBase<DC7u, double> {
public:
   const static std::vector<Vector_3> vertices;
public:
   friend std::ostream& operator<< (std::ostream&, const DC7u&);

   DC7u(void) ;
   DC7u(const DC7u& v);
   DC7u(const double v[7]);
   DC7u(const LRL_Cell& cell);
   DC7u(const VecN& v);
   DC7u(const G6& v);
   DC7u(const S6& v);
   DC7u(const C3& v);
   DC7u(const D7& v);
   DC7u(const B4& v);
   DC7u(const std::string& t);
   DC7u(const std::vector<double>& v);
   DC7u& operator= (const DC7u& v);
   DC7u& operator= (const std::string& s) { throw; return *this; }
   DC7u& operator= (const G6& v);
   DC7u& operator= (const S6& v);
   DC7u& operator= (const C3& c3);
   DC7u& operator= (const D7& v);
   DC7u& operator= (const B4& v);
   DC7u& operator= (const LRL_Cell& v);
   DC7u& operator/= (const double d);
   DC7u& operator*= (const double d);
   DC7u operator+ (const DC7u& ds) const;
   DC7u operator- (const DC7u& ds) const;
   DC7u& operator+= (const DC7u& ds);
   DC7u& operator-= (const DC7u& ds);
   DC7u operator* (const double d) const;
   DC7u operator/ (const double d) const;
   DC7u operator- (void) const; // unary
   bool operator== (const DC7u& dc7u) const;
   bool operator!= (const DC7u& dc7u) const;
   void FromCellToDC7u(const LRL_Cell& cell);

   double operator[](const size_t n) const { return m_vec[n]; }
   double& operator[](const size_t n) { return m_vec[n]; }
   friend double DistanceBetween( const DC7u& v1, const DC7u& v2 );
   double norm() const { return m_vec.norm(); }
   double norm(const DC7u& dc7u) const { return norm(dc7u); }
   double Norm() const { return m_vec.norm(); }
   double Norm(const DC7u& dc7u) const { return norm(dc7u); }

   std::vector<double> GetVector(void) const { return m_vec.GetVector(); }
   double* data() const { return const_cast<double*>(m_vec.data()); }
   void SetVector(const std::vector<double>& v) { m_vec = v; }
   bool GetValid(void) const { return m_cellIsValid; }
   void SetValid(const bool b) { m_cellIsValid = b; }
   bool IsValid(void) const { return m_cellIsValid; }
   bool IsValid(const DC7u& dc7u) const { return dc7u.m_cellIsValid; }

   std::vector<Vector_3 > GetIndices() const {
      return vertices; 
   }

   std::vector<std::string> GetStringIndices();
   static  std::string GetName()  { return "DC7u"; }
   size_t size() const { return m_vec.size(); }


private:
   std::string m_lattice;
   bool m_cellIsValid;
   size_t m_dim;
   VecN m_vec;

};

#endif  // DC7U_H
