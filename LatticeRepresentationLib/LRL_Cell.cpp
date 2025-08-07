#pragma warning (disable: 4189) // Visual Studio -- local variable is initialized but not referenced


#include <algorithm>

#include <cmath>
#include <cstdio>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <string>

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "C3.h"
#include "B4.h"
#include "D7.h"
#include "Delone.h"
#include "G6.h"
#include "P3.h"
#include "LRL_MinMaxTools.h"
#include "LRL_RandTools.h"
#include "MatG6.h"
#include "MatS6.h"
#include "rhrand.h"
#include "S6.h"
#include "Selling.h"
#include "LRL_StringTools.h"

int randSeed1 = 19191;

#ifndef M_PI
static const double M_PI = 4.0 * atan(1.0);
#endif

const double oneeightyDegrees = 180.0;
const double threesixtyDegrees = 360.0;

static const double pi = 4.0 * atan(1.0);
static const double twopi = 2.0 * pi;

double LRL_Cell::randomLatticeNormalizationConstant = 10.0;
double LRL_Cell::randomLatticeNormalizationConstantSquared = randomLatticeNormalizationConstant * randomLatticeNormalizationConstant;

/*  class LRL_Cell
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
A class to implement some common operations for unit cells as usually 
   used with xray crystallography. Conversions from G6 to unit cell and 
   from unit to G6 are included. The angles are ALWAYS in RADIANS.

   LRL_Cell(void)                                  == default constructor
   LRL_Cell( const G6& v )                 == constructor to convert a G6 vector to unit cell
   double Volume(void)                         == return the volume of a unit cell
   double LRL_Cell::operator[](const int& n) const == return the n-th element of a cell (zero-based)
   LRL_Cell LRL_Cell::Inverse( void ) const            == compute the reciprocal cell
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: LRL_Cell()
// Description: default constructor
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LRL_Cell::LRL_Cell(void)
   : m_valid(false)
{
   m_cell.resize(6);
}

LRL_Cell::LRL_Cell(const LRL_Cell& c)
   : m_cell(c.m_cell)
   , m_valid(c.m_valid)
{
}

LRL_Cell::LRL_Cell( const std::vector<double>& v )
   : m_cell( v )
   , m_valid( (*this).m_valid )
{
   for (size_t i = 3; i < 6; ++i)
      m_cell[i] *= 4.0 * atan( 1.0 ) / 180.0;
}

LRL_Cell::LRL_Cell(const std::string& s)
   : m_valid(true)
{
   m_cell = LRL_StringTools::FromString(s);
   m_valid = m_cell.size() == 6 && m_valid && 
      m_cell[3] < oneeightyDegrees && 
      m_cell[4] < oneeightyDegrees && 
      m_cell[5] < oneeightyDegrees && 
      (m_cell[3] + m_cell[4] + m_cell[5]) < threesixtyDegrees;
   if (m_valid) {
      for (size_t i = 3; i < 6; ++i)
         m_cell[i] *= 4.0 * atan(1.0) / 180.0;
   }
}

LRL_Cell::LRL_Cell(const S6& ds)
{
   m_cell.resize( 6 );

   const S6& s( ds );
   double& a = (*this)[0];
   double& b = (*this)[1];
   double& c = (*this)[2];
   double& al = (*this)[3];
   double& be = (*this)[4];
   double& ga = (*this)[5];

   const double asq = -(s[3] + s[2] + s[1]);
   const double bsq = -(s[4] + s[2] + s[0]);
   const double csq = -(s[5] + s[1] + s[0]);
   m_valid = ds.GetValid() && asq > 0.0 && bsq > 0.0 && csq > 0.0;
   if ( m_valid){
      a = sqrt( asq );
      b = sqrt( bsq );
      c = sqrt( csq );

      const double cosal = s[0] / (b * c);
      const double cosbe = s[1] / (a * c);
      const double cosga = s[2] / (a * b);
      m_valid = abs( cosal ) < 1.0 && abs( cosbe ) < 1.0 && abs( cosga ) < 1.0;
      if (m_valid) {
         al = acos( cosal );
         be = acos( cosbe );
         ga = acos( cosga );
      }

      static const double pi = 4.0 * atan( 1.0 );
      static const double twopi = 2.0 * pi;

      m_valid = (al + be + ga) < twopi;
   }
}

LRL_Cell::LRL_Cell(const C3& c3)
{
   *this = S6(c3);
   m_valid = m_valid && c3.GetValid() && GetValid() && m_cell[3] < pi && m_cell[4] < pi && m_cell[5] < pi && (m_cell[3] + m_cell[4] + m_cell[5])< twopi
      && (m_cell[3] + m_cell[4] + m_cell[5] - 2.0 * maxNC(m_cell[3], m_cell[4], m_cell[5]) >= 0.0);
}

LRL_Cell::LRL_Cell(const P3& p) {
   m_cell.resize(6);

   for (size_t i = 0; i < 3; ++i) {
      const double x = p[i].first;
      const double y = p[i].second;
      const double length = std::sqrt(x * x + y * y);
      const double angle  = std::atan2(y, x);  // radians

      m_cell[i]     = length;
      m_cell[i + 3] = angle;
   }

   m_valid = (*this).CheckValid();
}

LRL_Cell::LRL_Cell(const B4& b4)
{
   // Extract all four basis vectors from B4
   const Vector_3& a = b4[0];
   const Vector_3& b = b4[1];
   const Vector_3& c = b4[2];
   const Vector_3& d = b4[3];  // Include the fourth vector

   // Verify that d = -(a+b+c) which should be true for a Delone tetrahedron
   Vector_3 sumCheck = a + b + c + d;
   if (sumCheck.Norm() > 1e-10) {
      std::cerr << "Warning: B4 vectors don't satisfy a+b+c+d=0 constraint" << std::endl;
   }

   // Calculate the magnitudes (cell edges)
   const double a_len = a.Norm();
   const double b_len = b.Norm();
   const double c_len = c.Norm();

   // Calculate the angles (in radians)
   const double alpha = acos(b.Dot(c) / (b_len * c_len));
   const double beta = acos(a.Dot(c) / (a_len * c_len));
   const double gamma = acos(a.Dot(b) / (a_len * b_len));

   // For nearly cubic cells, check if we need to adjust orientation
   if (std::abs(alpha - M_PI / 2) < 0.1 &&
      std::abs(beta - M_PI / 2) < 0.1 &&
      std::abs(gamma - M_PI / 2) < 0.1) {

      // Check the handedness using all four vectors
      Vector_3 crossProduct = a.Cross(b);
      bool rightHanded = (crossProduct.Dot(c) > 0);

      // We need a consistent way to decide which cell representation to use
      // When permutations have been applied
      double adjustedAlpha = alpha;
      double adjustedBeta = beta;
      double adjustedGamma = gamma;

      // If handedness needs to be preserved after permutation,
      // flip two angles (not just one) to maintain an equivalent cell
      if (!rightHanded) {
         // Flip both alpha and beta
         adjustedAlpha = M_PI - alpha;
         adjustedBeta = M_PI - beta;

         // Alternative: flip beta and gamma
         // adjustedBeta = M_PI - beta;
         // adjustedGamma = M_PI - gamma;
      }

      // Return the LRL_Cell with adjusted parameters
      (*this) = LRL_Cell(a_len, b_len, c_len, adjustedAlpha, adjustedBeta, adjustedGamma);
   }
   else {
      // For non-cubic cells, use the standard calculation
      (*this) = LRL_Cell(a_len, b_len, c_len, alpha, beta, gamma);
   }
}

std::ostream& operator<< (std::ostream& o, const LRL_Cell& c) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(5);
   for (size_t i = 0; i < 6; ++i)
      o << std::setw(9) << c.m_cell[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: LRL_Cell()
// Description: constructor to convert load a LRL_Cell from doubles
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LRL_Cell::LRL_Cell( const double a, const double b, const double c,
           const double alpha, const double beta, const double gamma)
   : m_valid(true)
{
   static const double pi = 4.0 * atan(1.0);
   static const double twopi = 2.0 * pi;
   static const double rad = 180.0 / pi;

    bool radianInput = false;

   m_cell.resize( 6 );
   m_cell[0] = a;
   m_cell[1] = b;
   m_cell[2] = c;
   if (alpha < pi && beta < pi && gamma < pi) {
      // radian input
      m_cell[3] = alpha;
      m_cell[4] = beta;
      m_cell[5] = gamma;
      radianInput = true;
   }
   else {
      // angle input, convert to radians
      m_cell[3] = alpha / rad;
      m_cell[4] = beta / rad;
      m_cell[5] = gamma / rad;
      radianInput = false;
   }

   if (alpha < pi || beta < pi || gamma < pi) {
      if (!radianInput) std::cout << "; angles less then pi degrees are unusual\n";
   }

   if (alpha > 175 || beta > 175 || gamma > 175) {
      std::cout << "; angles greater than 175 degrees are unusual\n";
   }

   const double lowerlimit = 0.001;
   const bool b1 = a > lowerlimit && b > lowerlimit && c > lowerlimit;
   const bool b2 = alpha > lowerlimit && beta > lowerlimit && gamma > lowerlimit;
   const bool b3 = alpha < 179.99 && beta < 179.99 && gamma < 179.99;
   const bool b4 = (alpha + beta + gamma) < 360.0;
   const bool b5 = (alpha + beta + gamma - 2.0 * maxNC(alpha, beta, gamma)) >= 0.0 - 0.00001;
   const double xxxx = (alpha + beta + gamma);
   const double xxxxx = 2.0 * maxNC(alpha, beta, gamma);
   m_valid = m_valid && b1 && b2 && b3 && b4 && b5;
   if (!b5) {
      std::cout << "; angles do not satisfy the triangle inequality\n";
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: LRL_Cell()
// Description: constructor to convert an input G6 vector (as a vector of
//              doubles to E3 lengths and angles. The angles are 
//              stored as RADIANS (only). For angles as degrees, use the
//              class LRL_Cell_Degrees.
//              with lengths and angles as DEGREES. For consistency, a
//              "LRL_Cell" object will never contain degrees.
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LRL_Cell::LRL_Cell(const G6& g6)
   : m_valid(true)
{
   static const double pi = 4.0*atan(1.0);
   static const double twopi = 2.0*pi;
   m_cell.resize(6);
   const double lowerlimit = 0.0001;
   if ( (!g6.GetValid()) || g6.norm() < 1.0E-10 || g6[0] <= lowerlimit || g6[1] <= lowerlimit || g6[2] <= lowerlimit) {
      *this = LRL_Cell(0, 0, 0, 0, 0, 0);
      return;
   }
   else {
      m_cell[0] = sqrt(g6[0]);
      m_cell[1] = sqrt(g6[1]) ;
      m_cell[2] = sqrt(g6[2]);

      const double cosalpha(0.5*g6[3] / (m_cell[1] * m_cell[2]));
      const double cosbeta(0.5*g6[4] /  (m_cell[0] * m_cell[2]));
      const double cosgamma(0.5*g6[5] / (m_cell[0] * m_cell[1]));

      if (std::abs(cosalpha) >= 0.9999 || std::abs(cosbeta) >= 0.9999 || std::abs(cosgamma) >= 0.9999) {
         *this = LRL_Cell(0, 0, 0, 0, 0, 0);
         m_valid = false;
         return;
      }
      const double sinalpha(sqrt(1.0 - cosalpha * cosalpha));
      const double sinbeta(sqrt(1.0 - cosbeta * cosbeta));
      const double singamma(sqrt(1.0 - cosgamma * cosgamma));

      // compute the v angles in radians
      m_cell[3] = atan2(sinalpha, cosalpha);
      m_cell[4] = atan2(sinbeta, cosbeta);
      m_cell[5] = atan2(singamma, cosgamma);
      m_valid = m_valid && m_cell[0] > lowerlimit && m_cell[1] > lowerlimit && m_cell[2] > lowerlimit &&
         m_cell[3] > lowerlimit && m_cell[4] > lowerlimit && m_cell[5] > lowerlimit &&
         m_cell[3] < pi && m_cell[4] < pi && m_cell[5] < pi && (m_cell[3] + m_cell[4] + m_cell[5])< twopi
         && (m_cell[3] + m_cell[4] + m_cell[5] - 2.0 * maxNC(m_cell[3], m_cell[4], m_cell[5]) >= 0.0);
   }
}

LRL_Cell::LRL_Cell(const D7& v7)
   : m_valid(v7.GetValid())
{
   (*this) = G6(v7);
}

LRL_Cell::LRL_Cell(const D13& d13) {
   std::cout << "; not implemented" << std::endl;
   m_valid = false;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: LRL_Cell()
// Description: destructor
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LRL_Cell::~LRL_Cell(void)
{
}

void Prepare2CellElements( const double minEdge, const double maxEdge, const size_t i, LRL_Cell& c ) {
   static RHrand r;
   const double range = std::fabs( minEdge - maxEdge );
   const double d1 = r.urand( );
   c[i] = range * d1 + std::sqrt( minEdge * maxEdge );
   const double d2 = r.urand( );
   c[i + 3] = d2 * oneeightyDegrees;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: Volume()
// Description: Return the E3 volume of a LRL_Cell
// follows the formula of Stout and Jensen page 33
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double LRL_Cell::Volume() const {
   if (!IsPhysicallyValid()) {
      std::cout << "⚠️ Volume computation on invalid cell\n";
      return std::numeric_limits<double>::quiet_NaN();
   }
   const double c3 = cos(m_cell[3]);
   const double c4 = cos(m_cell[4]);
   const double c5 = cos(m_cell[5]);
   const double arg = 1.0 - pow(c3, 2) - pow(c4, 2) - pow(c5, 2) + 2.0 * c3 * c4 * c5;
   if (arg <= 0.0) return std::numeric_limits<double>::quiet_NaN();
   return m_cell[0] * m_cell[1] * m_cell[2] * sqrt(arg);
}


bool LRL_Cell::operator== (const LRL_Cell& cl) const {
   return m_cell == cl.m_cell;
}
bool LRL_Cell::operator!= (const LRL_Cell& cl) const {
   return !((*this) == cl);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: operator[]()
// Description: access function for the values in a LRL_Cell object
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double LRL_Cell::operator[](const size_t n) const
{
   const size_t nn( std::max(size_t(0),std::min(size_t(5),n)) );
   return m_cell[nn];
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: operator[]()
// Description: access function for the values in a LRL_Cell object
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double& LRL_Cell::operator[](const size_t n)
{
   const size_t nn( std::max(size_t(0),std::min(size_t(5),n)) );
   return m_cell[nn];
}

double LRL_Cell::DistanceBetween(const LRL_Cell& v1, const LRL_Cell& v2) {
   return (B4(v1) - B4(v2)).norm();
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: Inverse()
// Description: Compute the reciprocal cell (inverse) of a cell
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
LRL_Cell LRL_Cell::Inverse( void ) const
{
   const double& a((*this)[0]);
   const double& b((*this)[1]);
   const double& c((*this)[2]);
   const double& alpha((*this)[3]);
   const double& beta ((*this)[4]);
   const double& gamma((*this)[5]);
   const double cosAlpha(cos(alpha));
   const double cosBeta (cos(beta));
   const double cosGamma(cos(gamma));
   const double sinAlpha(sin(alpha));
   const double sinBeta (sin(beta));
   const double sinGamma(sin(gamma));

   const double v     = (*this).Volume( );
   const double astar = b*c*sin(alpha)/v;
   const double bstar = a*c*sin(beta )/v;
   const double cstar = a*b*sin(gamma)/v;

   const double cosAlphaStar = (cosBeta *cosGamma-cosAlpha)/fabs(sinBeta*sinGamma);
   const double cosBetaStar  = (cosAlpha*cosGamma-cosBeta )/fabs(sinAlpha*sinGamma);
   const double cosGammaStar = (cosAlpha*cosBeta -cosGamma)/fabs(sinAlpha*sinBeta);

   LRL_Cell cell;
   cell.m_cell[0] = astar;
   cell.m_cell[1] = bstar;
   cell.m_cell[2] = cstar;
   cell.m_cell[3] = atan2( sqrt(1.0-pow(cosAlphaStar,2)), cosAlphaStar);
   cell.m_cell[4] = atan2( sqrt(1.0-pow(cosBetaStar ,2)), cosBetaStar );
   cell.m_cell[5] = atan2( sqrt(1.0-pow(cosGammaStar,2)), cosGammaStar);
   cell.m_valid = m_valid;

   return cell;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: LatSymMatG6(const std::string& latsym)
// Description: return the MatG6 matrix for a lattice symbol
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
MatG6 LRL_Cell::LatSymMatG6(const std::string& latsym) const
{
   if (toupper(latsym[0]) == 'P') return MatG6::Eye();
   else if (toupper(latsym[0]) == 'I') return MatG6("1 0 0 0 0 0    0 1 0 0 0 0    .25 .25 .25 .25 .25 .25   0 1 0 .5 0 .5   1 0 0 0 .5 .5   0 0 0 0 0 1"); // for monoclinic, assumes b unique
   else if (toupper(latsym[0]) == 'A') return MatG6("1 0 0 0 0 0   0 1 0 0 0 0   0 .25 .25 .25 0 0    0 1 0 .5 0 0   0 0 0 0 .5 .5   0 0 0 0 0 1"); // for monoclinic, assumes b unique
   else if (toupper(latsym[0]) == 'B') return MatG6("1 0 0 0 0 0   0 1 0 0 0 0   .25 0 .25 0 .25 0   0 0 0 .5 0 .5   1 0 0 0 .5 0   0 0 0 0 0 1"); // for monoclinic, assumes c unique
   else if (toupper(latsym[0]) == 'C') return MatG6("1 0 0 0 0 0   .25 .25 0 0 0 .25   0 0 1 0 0 0    0 0 0 .5 .5 0   0 0 0 0 1 0   1 0 0 0 0 .5"); // for monoclinic, assumes b unique
   else if (toupper(latsym[0]) == 'F') return MatG6(".25 .25 0 0 0 .25     .25 0 .25 0 .25 0     0 .25 .25 .25  0 0    0 0 .5 .25 .25 .25     0 .5 0 .25 .25 .25     .5 0 0 .25 .25 .25");
   else if ((toupper(latsym[0]) == 'R' || toupper(latsym[0]) == 'H')
      && LRL_Cell::IsRhomobhedralAsHex(*this))
      return (1.0 / 9.0)* MatG6("1 1 1 1 -1 -1    4 1 1  1  2  2     1  4  1  -2  -1  2     -4  -4  2  -1  1  -5     2  -4  2  -1  -2  1     -4  2  2  2  1  1");
   else if (toupper(latsym[0]) == 'R' || toupper(latsym[0]) == 'H') return  MatG6::Eye();
   else if (latsym == "CCDC") return (1.0 / 9.0)* MatG6("4  1  1  1  2  2    1  1  1  1 -1 -1     1  4  1 -2 -1  2    2 -4  2 -1 -2  1    -4 -4  2 -1  1 -5    -4  2  2  2  1  1"); // CCDC matrix for R
   else return MatG6::Eye();
}

MatG6 LRL_Cell::G6MakePrimitiveMatrix(const std::string& latsym)
{
   if (toupper(latsym[0]) == 'P') return MatG6::Eye();
   else if (toupper(latsym[0]) == 'I') return MatG6("1 0 0 0 0 0    0 1 0 0 0 0    .25 .25 .25 .25 .25 .25   0 1 0 .5 0 .5   1 0 0 0 .5 .5   0 0 0 0 0 1"); // for monoclinic, assumes b unique
   else if (toupper(latsym[0]) == 'A') return MatG6("1 0 0 0 0 0   0 1 0 0 0 0   0 .25 .25 .25 0 0    0 1 0 .5 0 0   0 0 0 0 .5 .5   0 0 0 0 0 1"); // for monoclinic, assumes b unique
   else if (toupper(latsym[0]) == 'B') return MatG6("1 0 0 0 0 0   0 1 0 0 0 0   .25 0 .25 0 .25 0   0 0 0 .5 0 .5   1 0 0 0 .5 0   0 0 0 0 0 1"); // for monoclinic, assumes c unique
   else if (toupper(latsym[0]) == 'C') return MatG6("1 0 0 0 0 0   .25 .25 0 0 0 .25   0 0 1 0 0 0    0 0 0 .5 .5 0   0 0 0 0 1 0   1 0 0 0 0 .5"); // for monoclinic, assumes b unique
   else if (toupper(latsym[0]) == 'F') return MatG6(".25 .25 0 0 0 .25     .25 0 .25 0 .25 0     0 .25 .25 .25  0 0    0 0 .5 .25 .25 .25     0 .5 0 .25 .25 .25     .5 0 0 .25 .25 .25");
   else if (toupper(latsym[0]) == 'R') return (1.0 / 9.0)* MatG6("1 1 1 1 -1 -1    4 1 1  1  2  2     1  4  1  -2  -1  2     -4  -4  2  -1  1  -5     2  -4  2  -1  -2  1     -4  2  2  2  1  1");
   else if (toupper(latsym[0]) == 'H') return  MatG6::Eye();
   else return MatG6::Eye();
}

MatG6 LRL_Cell::LatSymMatG6( const std::string& latsym, const LRL_Cell& c ) {
   return c.LatSymMatG6( latsym );
}

const MatG6 HexPerp(MatG6::Eye() - (1./3.)*MatG6( " 1 1 0 0 0 -1   1 1 0 0 0 -1   0 0 3 0 0 0   0 0 0 0 0 0   0 0 0 0 0 0   -1 -1 0 0 0 1" ) );
const MatG6 RhmPerp(MatG6::Eye() - (1./3.)*MatG6( " 1 1 1 0 0  0   1 1 1 0 0  0   1 1 1 0 0 0   0 0 0 1 1 1   0 0 0 1 1 1    0  0 0 1 1 1" ) );

bool LRL_Cell::IsRhomobhedralAsHex( void ) const {
   return IsRhomobhedralAsHex( G6(*this));
}

/*static*/ bool LRL_Cell::IsRhomobhedralAsHex( const LRL_Cell& c ) {
   static const double degrees90 = 2.0*atan(1.0);
   static const double degrees120 = 4.0 / 3.0 * degrees90;;
   return (std::abs(c[3] - degrees90) < 1.0E-4 && std::abs(c[4] - degrees90) < 1.0E-4) &&
      (std::abs(c[5] - degrees120) < 1.0E-4);
}

// Assumes the cell EITHER has alpha=beta=gamma or a=b & alpha=beta=90 and gamma=120 (approximately)
/*static*/ bool LRL_Cell::IsRhomobhedralAsHex(const G6& v) {
   return (HexPerp*v).norm() < (RhmPerp*v).norm();
}

static int iseed;

LRL_Cell LRL_Cell::rand() {
   return S6::rand();
}

LRL_Cell LRL_Cell::randDeloneReduced() {//LRL_Cell::randomLatticeNormalizationConstant
   S6 vRan;
   S6 v;

   MatS6 m;
   vRan = LRL_Cell::rand();

   const bool test = Delone::Reduce(vRan, m, v, 0.0);
   while (!Delone::IsReduced(vRan)) {
      vRan = LRL_Cell::rand();
   }
   return LRL_Cell(v);
}

LRL_Cell LRL_Cell::randDeloneUnreduced() {//LRL_Cell::randomLatticeNormalizationConstant
   S6 vRan;

   MatS6 m;
   vRan = LRL_Cell::rand();

   while (Delone::IsReduced(vRan)) {
      vRan = LRL_Cell::rand();
   }
   LRL_Cell celltemp(vRan);
   return LRL_Cell(vRan);
}

LRL_Cell LRL_Cell::rand(const double d) {//LRL_Cell::randomLatticeNormalizationConstant
   return d*rand()/ randomLatticeNormalizationConstant;
}

LRL_Cell LRL_Cell::randDeloneReduced(const double d) {//LRL_Cell::randomLatticeNormalizationConstant
   return d*randDeloneReduced()/ randomLatticeNormalizationConstant;
}

LRL_Cell LRL_Cell::randDeloneUnreduced(const double d) {//LRL_Cell::randomLatticeNormalizationConstant
   return d*randDeloneUnreduced()/ randomLatticeNormalizationConstant;
}

Matrix_3x3 LRL_Cell::Cart() const {
   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);
   return Matrix_3x3::Cart(m_cell[0], m_cell[1], m_cell[2], 
      degreesPerRad*m_cell[3], degreesPerRad*m_cell[4], degreesPerRad*m_cell[5]);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: vvtorow(const double m,
//               const int n1, const int n2,
//               mat33& v1, mat33& v2,
//               const int n3,
//               MatG6& m6)
// Description: Compute one row of a 6x6 matrix from
//              2 rows of 2 3x3 matrices
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


LRL_Cell LRL_Cell::operator* (const double d) const {
   LRL_Cell c(*this);
   c[0] *= d;
   c[1] *= d;
   c[2] *= d;
   return c;
}

LRL_Cell LRL_Cell::operator/ (const double d) const {
   LRL_Cell c(*this);
   c[0] /= d;
   c[1] /= d;
   c[2] /= d;
   return c;
}

LRL_Cell LRL_Cell::operator+ (const LRL_Cell& c) const {
   const G6 v1(*this);
   const G6 v2(c);
   return LRL_Cell(G6(v1 + v2));
}

LRL_Cell LRL_Cell::operator- (const LRL_Cell& c) const {
   const G6 v1(*this);
   const G6 v2(c);
   const G6 diff = v1 - v2;
   if (diff.norm() < 1.0E-10)
      return LRL_Cell(0,0,0,0,0,0);
   else
      return LRL_Cell(G6(v1 - v2));
}

LRL_Cell& LRL_Cell::operator+= (const LRL_Cell& cl) {
   (*this) = (*this) + cl;
   return *this;
}

LRL_Cell& LRL_Cell::operator-= (const LRL_Cell& cl) {
   *this = (*this) - cl;
   return *this;
}

LRL_Cell operator* (const double d, const LRL_Cell& c) {
   return c*d;
}

LRL_Cell& LRL_Cell::operator= (const D7& v) {
   *this = LRL_Cell(v);
   return *this;
}

LRL_Cell& LRL_Cell::operator= (const C3& c3) {
   *this = LRL_Cell(c3);
   return *this;
}

LRL_Cell& LRL_Cell::operator= (const S6& v) {
   *this = LRL_Cell(v);
   return *this;
}

LRL_Cell& LRL_Cell::operator= (const B4& v) {
   *this = LRL_Cell(v);
   return *this;
}

LRL_Cell& LRL_Cell::operator= (const LRL_Cell& v) {
   m_cell = v.m_cell;
   m_valid = v.GetValid();
   return *this;
}

LRL_Cell& LRL_Cell::operator= (const G6& v) {
   *this = LRL_Cell(v);
   m_valid = m_valid && GetValid();
   return *this;
}

LRL_Cell& LRL_Cell::operator= (const std::string& s) {
   *this = LRL_Cell(s);
   m_valid = true;
   return *this;
}

LRL_Cell& LRL_Cell::operator/= (const double d) {
   (*this)[0] /= d;
   (*this)[1] /= d;
   (*this)[2] /= d;
   m_valid = true;
   return *this;
}

LRL_Cell& LRL_Cell::operator*= (const double d) {
   (*this)[0] *= d;
   (*this)[1] *= d;
   (*this)[2] *= d;
   m_valid = true;
   return *this;
}

LRL_Cell LRL_Cell::GetPrimitiveCell(const std::string& latsym, const LRL_Cell& c) {
   return LRL_Cell(GetPrimitiveV6Vector(latsym, c));
}

G6 LRL_Cell::GetPrimitiveV6Vector(const std::string& latsym, const LRL_Cell& c) {
   const MatG6 m66 = c.LatSymMatG6(latsym);
   return m66 * G6(c);
}

bool LRL_Cell::CheckValid() {
   return CheckValid(*this);
}

bool LRL_Cell::CheckValid(const LRL_Cell& cell) {
   return CheckValid(cell[0], cell[1], cell[2], cell[3], cell[4], cell[5]);
}

bool LRL_Cell::CheckValid(const double a, const double b, const double c,
   const double alpha, const double beta, const double gamma) {
   static const double pi = 4.0 * atan(1.0);
   static const double twopi = 2.0 * pi;
   static const double rad = 180.0 / pi;

   const bool radianInput = alpha < pi && beta < pi && gamma < pi;

   const double lowerlimit = 0.001;
   const bool b1 = a > lowerlimit && b > lowerlimit && c > lowerlimit;
   const bool b2 = alpha > lowerlimit && beta > lowerlimit && gamma > lowerlimit;
   const bool b3 = alpha < 179.99 && beta < 179.99 && gamma < 179.99;

   const double anglesum = alpha + beta + gamma;
   const bool b4 = (radianInput) ? (anglesum < twopi) : (anglesum < 360.0);

   // b5 checks the triangle inequality for the angles
   const bool b5 = (alpha + beta + gamma - 2.0 * maxNC(alpha, beta, gamma)) >= 0.0 - 0.00001;

   //std::cout << "in CheckValid " << ((b1 && b2 && b3 && b4 && b5) ? "true" : "false") << std::endl;

   return b1 && b2 && b3 && b4 && b5;
}

