#ifndef DC7U_H
#define DC7U_H

#include <vector>

#include "BasisBase.h"

#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"
#include "LRL_indices.h"
#include "Niggli.h"

class DC7u {
public:
   const static std::vector<Vector_3> vertices;
public:
   friend std::ostream& operator<< (std::ostream&, const DC7u&);
   friend DC7u operator* (const double d, const DC7u& dc7u);
   template<typename T> explicit DC7u(const T& t)
      : m_dim(7)
      , m_cell(LRL_Cell(t))
      , m_lattice("P") //LCA assumes primitive
   {
      G6 red;
      double delta;
      int error;
      const bool b = Niggli::Reduce(G6(t).data(), red);
      m_vec.resize(7);
      m_vec[DC7u_AA_idx] = red[G6_AA_idx];
      m_vec[DC7u_BB_idx] = red[G6_BB_idx];
      m_vec[DC7u_CC_idx] = red[G6_CC_idx];
      m_vec[DC7u_MIN_BC_diagsq_idx] = red[G6_BB_idx] + red[G6_CC_idx] - std::fabs(red[G6_2BC_idx]);
      m_vec[DC7u_MIN_AC_diagsq_idx] = red[G6_AA_idx] + red[G6_CC_idx] - std::fabs(red[G6_2AC_idx]);
      m_vec[DC7u_MIN_AB_diagsq_idx] = red[G6_AA_idx] + red[G6_BB_idx] - std::fabs(red[G6_2AB_idx]);
      m_vec[DC7u_MIN_ABC_diagsq_idx] = red[G6_AA_idx] + red[G6_BB_idx] + red[G6_CC_idx]
         - std::fabs(red[G6_2BC_idx]) - std::fabs(red[G6_2AC_idx]) - std::fabs(red[G6_2AB_idx]);
      delta = std::fabs(m_vec[DC7u_AA_idx]) * 1.e-10;
      error = 0;
      for (size_t ii = 0; ii < 7; ii++) {
         if (m_vec[ii] < delta) error++;
      }
      if (error > 0) throw std::invalid_argument("invalid unsorted DC7 cell");
      if (red[G6_2BC_idx] > delta && red[G6_2AC_idx] > delta && red[G6_2AB_idx] > delta) {
         m_vec[DC7u_MIN_ABC_diagsq_idx] += 2. * std::min(std::min(red[G6_2BC_idx], red[G6_2AC_idx]), red[G6_2AB_idx]);
      }
      m_cellIsValid = m_cell.IsValid();
   }

   template<typename T> DC7u operator=(const T& t) {
      *this = t;
      return *this;
   }


   DC7u(void) ;
   DC7u(const DC7u& v);
   DC7u(const double v[7]);
   DC7u(const LRL_Cell& cell);
   DC7u(const VecN& v);
   explicit DC7u( char const * ch); // access to construct as std::string
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

   std::vector<double> GetAreas() const;
   std::vector<std::string> GetStringIndices();
   static  std::string GetName()  { return "DC7u"; }
   size_t size() const { return m_vec.size(); }

   LRL_Cell GetCell() const { return m_cell; }
   size_t GetDim() const { return m_dim; }

private:
   size_t m_dim;
   LRL_Cell m_cell;
   std::string m_lattice;
   bool m_cellIsValid;
   VecN m_dirCellAreas;
   std::vector<std::pair<int, std::pair<std::vector<int>, double> > > m_hashedAreaList;
   VecN m_vec;

};

#endif  // DC7U_H
