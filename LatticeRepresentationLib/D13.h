#ifndef D13_H
#define D13_H

#include <vector>

#include "BasisBase.h"

#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"

class D13 : private BasisBase<D13, double> {
public:
   const static std::vector<Vector_3> vertices;
public:
   friend std::ostream& operator<< (std::ostream&, const D13&);

   D13() {}

   D13(const LRL_Cell& cell);
   D13(const std::string& t);
   D13& operator= (const D13& v);
   D13& operator= (const std::string& s) { throw; return *this; }
   D13& operator= (const G6& v);
   D13& operator= (const S6& v);
   D13& operator= (const C3& c3);
   D13& operator= (const D7& v);
   D13& operator= (const B4& v);
   D13& operator= (const LRL_Cell& v);
   D13& operator/= (const double d);
   D13& operator*= (const double d);
   D13 operator+ (const D13& ds) const;
   D13 operator- (const D13& ds) const;
   D13& operator+= (const D13& ds);
   D13& operator-= (const D13& ds);
   D13 operator* (const double d) const;
   D13 operator/ (const double d) const;
   D13 operator- (void) const; // unary
   bool operator== (const D13& d13) const;
   bool operator!= (const D13& d13) const;
   void FromCellToD13(const LRL_Cell& cell);

   double operator[](const size_t n) const { return m_vec[n]; }
   double& operator[](const size_t n) { return m_vec[n]; }
   friend double DistanceBetween( const D13& v1, const D13& v2 );
   double norm() const { return m_vec.norm(); }
   double norm(const D13& d13) const { return norm(d13); }
   double Norm() const { return m_vec.norm(); }
   double Norm(const D13& d13) const { return norm(d13); }

   std::vector<double> GetVector(void) const { return m_vec.GetVector(); }
   double* data() const { return const_cast<double*>(m_vec.data()); }
   void SetVector(const std::vector<double>& v) { m_vec = v; }
   bool GetValid(void) const { return m_cellIsValid; }
   void SetValid(const bool b) { m_cellIsValid = b; }
   bool IsValid(void) const { return m_cellIsValid; }
   bool IsValid(const D13& d13) const { return d13.m_cellIsValid; }

   std::vector<Vector_3 > GetIndices() const {
      return vertices; 
   }

   std::vector<double> GetAreas() const;
   std::vector<std::string> GetStringIndices();
  static  std::string GetName()  { return "D13"; }
   size_t size() const { return m_vec.size(); }

   LRL_Cell GetCell() const { return m_cell; }

private:
   LRL_Cell m_cell;
   std::string m_lattice;
   bool m_cellIsValid;
   VecN m_dirCellAreas;
   size_t m_dim;
   std::vector<std::pair<int, std::pair<std::vector<int>, double> > > m_hashedAreaList;
   VecN m_vec;

private:
   void ConstructHashedAreaList();

   //VecN CreateCompleteListOf13Areas(const std::vector<Vector_3>& allIndices,
   //   const std::vector<std::vector<int> >& dcIndices,
   //   const std::vector<double>& dcAreas);
   //std::vector<int> HashIndices() const;
   //std::vector<int> HashIndices(const std::vector<std::vector<int> >& vin) const;
};

#endif  // D13_h
