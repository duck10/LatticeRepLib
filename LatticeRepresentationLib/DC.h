#ifndef DC_H
#define DC_H

#include <vector>

#include "BasisBase.h"

#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"

class DC : private BasisBase<DC, double> {
public:
   const static std::vector<Vector_3> vertices;
public:
   friend std::ostream& operator<< (std::ostream&, const DC&);

   DC() {}

   DC(const LRL_Cell& cell);
   DC(const std::string& t);
   DC& operator= (const DC& v);
   DC& operator= (const std::string& s) { throw; return *this; }
   DC& operator= (const G6& v);
   DC& operator= (const S6& v);
   DC& operator= (const C3& c3);
   DC& operator= (const D7& v);
   DC& operator= (const B4& v);
   DC& operator= (const LRL_Cell& v);
   DC& operator/= (const double d);
   DC& operator*= (const double d);
   DC operator+ (const DC& ds) const;
   DC operator- (const DC& ds) const;
   DC& operator+= (const DC& ds);
   DC& operator-= (const DC& ds);
   DC operator* (const double d) const;
   DC operator/ (const double d) const;
   DC operator- (void) const; // unary
   bool operator== (const DC& DC) const;
   bool operator!= (const DC& DC) const;

   double operator[](const size_t n) const { return m_vec[n]; }
   double& operator[](const size_t n) { return m_vec[n]; }
   static double DistanceBetween(const DC& v1, const DC& v2, const int length=7);
   double norm() const { return m_vec.norm(); }
   double norm(const DC& DC) const { return norm(DC); }
   double Norm() const { return m_vec.norm(); }
   double Norm(const DC& DC) const { return norm(DC); }

   std::vector<double> GetVector(void) const;
   double* data() const { return const_cast<double*>(m_vec.data()); }
   void SetVector(const std::vector<double>& v) { m_vec = v; }
   bool GetValid(void) const { return m_cellIsValid; }
   void SetValid(const bool b) { m_cellIsValid = b; }
   bool IsValid(void) const { return m_cellIsValid; }
   bool IsValid(const DC& DC) const { return DC.m_cellIsValid; }

   //std::vector<std::vector<int> > GetIndices() const {
   //   return m_dc.GetIndices();
   //}
   //std::vector<double> GetAreas() const;
   //std::vector<std::string> GetStringIndices() const { return m_dc.GetStrIndices(); }
   std::string GetName() const { return "DC"; }
   size_t size() const { return m_vec.size(); }
   static std::vector<double> G6_to_V13(const G6& gin);
   static std::vector<std::pair<double, Vector_3> > Cell_to_V13(const LRL_Cell& c);

private:
   LRL_Cell m_cell;
   std::string m_lattice;
   bool m_cellIsValid;
   size_t m_dim;
   std::vector<std::pair<int, std::pair<std::vector<int>, double> > > m_hashedAreaList;
   VecN m_vec;

private:

   //VecN CreateCompleteListOf13Areas(const std::vector<Vector_3>& allIndices,
   //   const std::vector<std::vector<int> >& dcIndices,
   //   const std::vector<double>& dcAreas);
   //std::vector<int> HashIndices(const std::vector<std::vector<int> >& vin) const;
};

#endif  // DC_h
