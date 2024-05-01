#ifndef LRL_CELL_H
#define LRL_CELL_H

#ifdef _MSC_VER
#define USE_LOCAL_HEADERS
#endif
#ifndef USE_LOCAL_HEADERS
//#include <rhrand.h>
#else
#include "rhrand.h"
#endif

#include "BasisBase.h"
#include "C3.h"
#include "LRL_Vector3.h"
#include "VecN.h"

#include <string>
#include <vector>

class C3;
class S6;
class D7;
class B4;
class G6;
class MatG6;

class LRL_Cell : private BasisBase<LRL_Cell>
{
public:

   static double randomLatticeNormalizationConstant;
   static double randomLatticeNormalizationConstantSquared;

   friend std::ostream& operator<< (std::ostream& o, const LRL_Cell& c);
   friend LRL_Cell operator* (const double d, const LRL_Cell& v);
   LRL_Cell(void);
   LRL_Cell( const LRL_Cell& c );
   LRL_Cell( const std::vector<double>& v );
   LRL_Cell(const std::string& s); // cell with angles in degrees from text
   LRL_Cell(const double a, const double b, const double c,
      const double alpha, const double beta, const double gamma);
   LRL_Cell(const G6& v6); // specialized for G6
   LRL_Cell(const D7& v7); // specialized for d7
   LRL_Cell(const D13& v13); // specialized for DC13
   LRL_Cell(const S6& dt); // specialized for S6
   LRL_Cell(const C3& c3); // specialized for C3
   LRL_Cell(const B4& ds); // specialized for Delone tetrahedron
   ~LRL_Cell(void);

   bool CheckValid();
   static bool CheckValid(const LRL_Cell& cell);
   static bool CheckValid(const double a, const double b, const double c, 
      const double alpha, const double beta, const double gamma);

   double Volume(void) const;

   LRL_Cell& operator= (const std::string& s);
   LRL_Cell& operator= (const D7& v);
   LRL_Cell& operator= (const C3& c3);
   LRL_Cell& operator= (const S6& v);
   LRL_Cell& operator= (const B4& v);
   LRL_Cell& operator= (const LRL_Cell& v);
   LRL_Cell& operator= (const G6& v);
   LRL_Cell& operator/= (const double d);
   LRL_Cell& operator*= (const double d);
   LRL_Cell operator+ (const LRL_Cell& c) const;
   LRL_Cell operator- (const LRL_Cell& c) const;
   LRL_Cell operator* (const double d) const;
   LRL_Cell operator/ (const double d) const;
   LRL_Cell operator- (void) const { return *this; } // unary
   LRL_Cell& operator+= (const LRL_Cell& cl);
   LRL_Cell& operator-= (const LRL_Cell& cl);
      bool operator== (const LRL_Cell& cl) const;
   bool operator!= (const LRL_Cell& cl) const;

   double operator[] (const size_t n) const;
   double& operator[](const size_t n);
   double DistanceBetween(const LRL_Cell& v1, const LRL_Cell& v2);
   size_t size(void) const { return 6; }
   double norm() const { return (*this).Volume();}
   double norm(const LRL_Cell& cell) const { return cell.Volume();}
   double Norm() const { return (*this).Volume();}
   double Norm(const LRL_Cell& cell) const { return cell.Volume();}

   std::vector<double> GetVector(void) const { return m_cell; }
   double* data() const { return const_cast<double*>(m_cell.data()); }

   void SetSigmas( const LRL_Cell& s ) { m_sigmas = s.GetVector( ); }
   void SetSigmas( const std::vector<double>& s ) { m_sigmas = s; }


   void SetVector(const std::vector<double>& v) { m_cell = v; }
   bool GetValid(void) const { return m_valid; }
   bool IsValid(void) const { return m_valid; }
   void SetValid(const bool b) { m_valid = b; };
   double at(const size_t n) const { return (*this)[n]; }


   LRL_Cell Inverse(void) const;
   MatG6 LatSymMatG6(const std::string& latsym) const;
   static MatG6 LatSymMatG6(const std::string& latsym, const LRL_Cell& c);
   static MatG6 G6MakePrimitiveMatrix(const std::string& latsym);

   static LRL_Cell GetPrimitiveCell(const std::string& latsym, const LRL_Cell& c);
   static G6 GetPrimitiveV6Vector(const std::string& latsym, const LRL_Cell& c);

   bool IsRhomobhedralAsHex(void) const;
   static bool IsRhomobhedralAsHex(const LRL_Cell& c);
   static bool IsRhomobhedralAsHex(const G6& v);

   static LRL_Cell rand();
   static LRL_Cell randDeloneReduced();
   static LRL_Cell randDeloneUnreduced();
   static LRL_Cell rand(const double d);
   static LRL_Cell randDeloneReduced(const double d);
   static LRL_Cell randDeloneUnreduced(const double d);

   static std::string GetName(void) { return "LRL_Cell, unit cell"; }

   static double GetNormalization(void) { return randomLatticeNormalizationConstant; }

   Matrix_3x3 Cart() const;

protected:
   std::vector<double> m_cell;
   std::vector<double> m_sigmas;
   bool m_valid;
};

#endif // LRL_CELL_H
