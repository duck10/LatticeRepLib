#ifndef CELL_H
#define CELL_H

#ifdef _MSC_VER
#define USE_LOCAL_HEADERS
#endif
#ifndef USE_LOCAL_HEADERS
//#include <rhrand.h>
#else
#include "rhrand.h"
#endif

#include "D6.h"
#include "Mat66.h"
#include "G6.h"

#include <vector>

class Mat66;
class G6;

class Cell
{
public:
   Cell(void);
   Cell(const Cell& c);
   Cell(const std::string& s); // cell with angles in degrees from text
   Cell(const double a, const double b, const double c,
      const double alpha, const double beta, const double gamma);
   explicit Cell(const G6& v6); // specialized for G6
   explicit Cell(const D7& v7); // specialized for d7
   explicit Cell(const D6& dt); // specialized for d7
   explicit Cell(const DeloneTetrahedron& ds); // specialized for Delone tetrahedron
   ~Cell(void);

   double Volume(void) const;
   G6 CellWithDegrees(void) const;
   static G6 CellWithDegrees(const Cell& c); // specialized for G6
   double operator[] (const size_t n) const;
   double& operator[](const size_t n);
   G6 Cell2V6(void) const;
   static G6 Cell2V6(const Cell& c);

   Cell Inverse(void) const;
   static Cell RandCell(const double minEdge, const double maxEdge);
   static Cell RandCell(const double minEdgeA, const double maxEdgeA, const double minEdgeB, const double maxEdgeB, const double minEdgeC, const double maxEdgeC);
   Mat66 LatSymMat66(const std::string& latsym) const;
   static Mat66 LatSymMat66(const std::string& latsym, const Cell& c);

   static Cell GetPrimitiveCell(const std::string& latsym, const Cell& c);
   static G6 GetPrimitiveV6Vector(const std::string& latsym, const Cell& c);

   Cell operator* (const double d) const;
   Cell operator+ (const Cell& c) const;
   Cell operator- (const Cell& c) const;
   Cell& operator= (const G6& v);
   Cell& operator= (const std::string& s);
   bool operator== (const Cell& c);
   bool operator!= (const Cell& c);

   bool IsRhomobhedralAsHex(void) const;
   static bool IsRhomobhedralAsHex(const Cell& c);
   static bool IsRhomobhedralAsHex(const G6& v);

   friend Cell operator* (const double d, const Cell& c);
   static size_t size(void) { return 6; }

   friend std::ostream& operator<< (std::ostream& o, const Cell& c);

   template<typename T>
   Cell operator+ (const Cell& c) const {
      const T v1((*this).Cell2V6());
      const T v2(c.Cell2V6());
      return Cell(v1 + v2);
   }

   template<typename T>
   Cell operator- (const Cell& c) const {
      const T v1((*this).Cell2V6());
      const T v2(c.Cell2V6());
      return Cell(v1 + v2);
   }

private:
   std::vector<double> m_cell;
};

#endif // CELL_H
