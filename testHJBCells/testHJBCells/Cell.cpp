#include <algorithm>

#include <cmath>
#include <cstdio>
#include <iomanip>

#include "Cell.h"
#include "rhrand.h"
#include "Vec_N_Tools.h"

/*  class Cell
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
A class to implement some common operations for unit cells as usually 
   used with xray crystallography. Conversions from G6 to unit cell and 
   from unit to G6 are included. The angles are ALWAYS in RADIANS.

   Cell(void)                                  == default constructor
   Cell( const G6& v )                 == constructor to convert a G6 vector to unit cell
   double Volume(void)                         == return the volume of a unit cell
   G6 Cell::CellWithDegrees            == return the unit cell as a vector with the angles as DEGREES
   double Cell::operator[](const int& n) const == return the n-th element of a cell (zero-based)
   Cell Cell::Inverse( void ) const            == compute the reciprocal cell
   G6 Cell::Cell2V6( void ) const      == return the G6 vector corresponding to a unit cell
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: Cell()
// Description: default constructor
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Cell::Cell(void)
{
   m_cell.resize(6);
}

Cell::Cell(const Cell& c)
: m_cell(c.m_cell)
{
}

Cell::Cell(const std::string& s)
{
   m_cell = Vec_N_Tools::FromString(s);
   for (size_t i = 3; i < 6; ++i)
      m_cell[i] *= 4.0*atan(1.0) / 180.0;
}

Cell::Cell(const D6& ds)
{
   (*this) = G6(ds);
}

Cell::Cell(const DeloneTetrahedron& dt)
{
   (*this) = G6(dt);
}

std::ostream& operator<< (std::ostream& o, const Cell& c) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(5);
   for (size_t i = 0; i < 6; ++i)
      o << std::setw(9) << c.m_cell[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: Cell()
// Description: constructor to convert load a Cell from doubles
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Cell::Cell( const double a, const double b, const double c,
           const double alpha, const double beta, const double gamma)
{
   m_cell.resize( 6 );
   m_cell[0] = a;
   m_cell[1] = b;
   m_cell[2] = c;
   m_cell[3] = alpha / 57.2957795130823;
   m_cell[4] = beta / 57.2957795130823;
   m_cell[5] = gamma / 57.2957795130823;
}

std::vector<double> V62Cell(const G6& v) {
   std::vector<double> vd;
   vd.resize(6);
   vd[0] = sqrt(v[0]);
   vd[1] = sqrt(v[1]);
   vd[2] = sqrt(v[2]);

   const double cosalpha(0.5*v[3] / (vd[1] * vd[2]));
   const double cosbeta(0.5*v[4] / (vd[0] * vd[2]));
   const double cosgamma(0.5*v[5] / (vd[0] * vd[1]));
   const double sinalpha(sqrt(std::max(0.0, 1.0 - cosalpha*cosalpha)));
   const double sinbeta(sqrt(std::max(0.0, 1.0 - cosbeta *cosbeta)));
   const double singamma(sqrt(std::max(0.0, 1.0 - cosgamma*cosgamma)));
   
   // compute the vd angles in radians
   vd[3] = atan2(sinalpha, cosalpha);
   vd[4] = atan2(sinbeta, cosbeta);
   vd[5] = atan2(singamma, cosgamma);

   return vd;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: Cell()
// Description: constructor to convert an input G6 vector (as a vector of
//              doubles to E3 lengths and angles. The angles are 
//              stored as RADIANS (only). For angles as degrees, use the
//              function CellWithDegrees to obtain a VECTOR (G6)
//              with lengths and angles as DEGREES. For consistency, a
//              "Cell" object will never contain degrees.
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Cell::Cell( const G6& v )
{
   m_cell = V62Cell(v);
}

Cell::Cell(const D7& v7)
{
   (*this) = G6(v7);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: Cell()
// Description: destructor
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Cell::~Cell(void)
{
}

const double thirtyDegrees = 30.0 / 180.0 * 4 * atan( 1.0 );
const double sixtyDegrees = 2.0*thirtyDegrees;
const double ninetyDegrees = 3.0*thirtyDegrees;
const double oneeightyDegrees = 6.0*thirtyDegrees;

void Prepare2CellElements( const double minEdge, const double maxEdge, const size_t i, Cell& c ) {
   static RHrand r;
   const double range = std::fabs( minEdge - maxEdge );
   const double d1 = r.urand( );
   c[i] = range * d1 + std::sqrt( minEdge * maxEdge );
   const double d2 = r.urand( );
   c[i + 3] = d2 * oneeightyDegrees;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: RandCell()
// Description: Generate a cell with random edge lengths within the specified
//              range and angles (in radians) in 60-120 degrees
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Cell Cell::RandCell( const double minEdge, const double maxEdge )
{
   Cell c;
   Prepare2CellElements( minEdge, maxEdge, 0, c );
   Prepare2CellElements( minEdge, maxEdge, 1, c );
   Prepare2CellElements( minEdge, maxEdge, 2, c );
   return( c );
}

Cell Cell::RandCell( const double minEdgeA, const double maxEdgeA, const double minEdgeB, const double maxEdgeB, const double minEdgeC, const double maxEdgeC ) {
   Cell c;
   Prepare2CellElements( minEdgeA, maxEdgeA, 0, c );
   Prepare2CellElements( minEdgeB, maxEdgeB, 1, c );
   Prepare2CellElements( minEdgeC, maxEdgeC, 2, c );
   return c;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: Volume()
// Description: Return the E3 volume of a Cell
// follows the formula of Stout and Jensen page 33
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double Cell::Volume( void ) const
{
   const G6& c( m_cell );
   const double c3(cos(c[3]));
   const double c4(cos(c[4]));
   const double c5(cos(c[5]));
   const double volume( c[0]*c[1]*c[2] * sqrt( 1.0-pow(c3,2)-pow(c4,2)-pow(c5,2)
       + 2.0*c3*c4*c5 ) );
   return volume;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: CellWithDegrees()
// Description: Return the E3 cell with the angles as DEGREES in an
//              G6 vector of doubles.
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
G6 Cell::CellWithDegrees( void ) const
{
   G6 v;
   v[0] = m_cell[0];
   v[1] = m_cell[1];
   v[2] = m_cell[2];
   v[3] = 57.2957795130823 * m_cell[3];
   v[4] = 57.2957795130823 * m_cell[4];
   v[5] = 57.2957795130823 * m_cell[5];

   return v;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: CellWithDegrees()
// Description: Return the E3 cell with the angles as DEGREES in an
//              G6 vector of doubles.
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
G6 Cell::CellWithDegrees( const Cell& c )
{
   G6 v;
   v[0] = c.m_cell[0];
   v[1] = c.m_cell[1];
   v[2] = c.m_cell[2];
   v[3] = 57.2957795130823 *c.m_cell[3];
   v[4] = 57.2957795130823 *c.m_cell[4];
   v[5] = 57.2957795130823 *c.m_cell[5];

   return v;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: operator[]()
// Description: access function for the values in a Cell object
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double Cell::operator[](const size_t n) const
{
   const size_t nn( std::max(size_t(0),std::min(size_t(5),n)) );
   return m_cell[nn];
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: operator[]()
// Description: access function for the values in a Cell object
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double& Cell::operator[](const size_t n)
{
   const size_t nn( std::max(size_t(0),std::min(size_t(5),n)) );
   return m_cell[nn];
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: Inverse()
// Description: Compute the reciprocal cell (inverse) of a cell
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Cell Cell::Inverse( void ) const
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

   Cell cell;
   cell.m_cell[0] = astar;
   cell.m_cell[1] = bstar;
   cell.m_cell[2] = cstar;
   cell.m_cell[3] = atan2( sqrt(1.0-pow(cosAlphaStar,2)), cosAlphaStar);
   cell.m_cell[4] = atan2( sqrt(1.0-pow(cosBetaStar ,2)), cosBetaStar );
   cell.m_cell[5] = atan2( sqrt(1.0-pow(cosGammaStar,2)), cosGammaStar);

   return cell;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: Cell2V6()
// Description: return the G6 vector of an input cell
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
G6 Cell::Cell2V6( void ) const
{
   const Cell& c( *this );
   G6 v;
   v[0] = c[0]*c[0];
   v[1] = c[1]*c[1];
   v[2] = c[2]*c[2];
   v[3] = 2.0*c[1]*c[2]*cos(c[3]);
   v[4] = 2.0*c[0]*c[2]*cos(c[4]);
   v[5] = 2.0*c[0]*c[1]*cos(c[5]);
   for ( size_t i=3; i<6; ++i ) if ( std::fabs(v[i]) < 1.0E-10 ) v[i] = 0.0;

   return v;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: Cell2V6()
// Description: return the G6 vector of an input cell
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
G6 Cell::Cell2V6( const Cell& c )
{
   G6 v;
   v[0] = c[0]*c[0];
   v[1] = c[1]*c[1];
   v[2] = c[2]*c[2];
   v[3] = 2.0*c[1]*c[2]*cos(c[3]);
   v[4] = 2.0*c[0]*c[2]*cos(c[4]);
   v[5] = 2.0*c[0]*c[1]*cos(c[5]);

   return v;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: LatSymMat66(const std::string& latsym)
// Description: return the Mat66 matrix for a lattice symbol
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Mat66 Cell::LatSymMat66( const std::string& latsym ) const
{
   if ( toupper( latsym[0] ) == 'P' ) return Mat66( ).Eye( );
   else if ( toupper( latsym[0] ) == 'I' ) return Mat66( "1 0 0 0 0 0    0 1 0 0 0 0    .25 .25 .25 .25 .25 .25   0 1 0 .5 0 .5   1 0 0 0 .5 .5   0 0 0 0 0 1" ); // for monoclinic, assumes b unique
   else if ( toupper( latsym[0] ) == 'A' ) return Mat66( "1 0 0 0 0 0   0 1 0 0 0 0   0 .25 .25 .25 0 0    0 1 0 .5 0 0   0 0 0 0 .5 .5   0 0 0 0 0 1" ); // for monoclinic, assumes b unique
   else if ( toupper( latsym[0] ) == 'B' ) return Mat66( "1 0 0 0 0 0   0 1 0 0 0 0   .25 0 .25 0 .25 0   0 0 0 .5 0 .5   1 0 0 0 .5 0   0 0 0 0 0 1" ); // for monoclinic, assumes c unique
   else if ( toupper( latsym[0] ) == 'C' ) return Mat66( "1 0 0 0 0 0   .25 .25 0 0 0 .25   0 0 1 0 0 0    0 0 0 .5 .5 0   0 0 0 0 1 0   1 0 0 0 0 .5" ); // for monoclinic, assumes b unique
   else if ( toupper( latsym[0] ) == 'F' ) return Mat66( ".25 .25 0 0 0 .25     .25 0 .25 0 .25 0     0 .25 .25 .25  0 0    0 0 .5 .25 .25 .25     0 .5 0 .25 .25 .25     .5 0 0 .25 .25 .25" );
   else if ( toupper( latsym[0] ) == 'R' && Cell::IsRhomobhedralAsHex( *this ) )
      return (1.0 / 9.0)* Mat66( "1 1 1 1 -1 -1    4 1 1  1  2  2     1  4  1  -2  -1  2     -4  -4  2  -1  1  -5     2  -4  2  -1  -2  1     -4  2  2  2  1  1 " );
   else if ( toupper( latsym[0] ) == 'R' ) return  Mat66().Eye();
   else if ( latsym == "CCDC" ) return (1.0 / 9.0)* Mat66( "4  1  1  1  2  2    1  1  1  1 -1 -1     1  4  1 -2 -1  2    2 -4  2 -1 -2  1    -4 -4  2 -1  1 -5    -4  2  2  2  1  1 " ); // CCDC matrix for R
   else return Mat66( ).Eye( );
}

Mat66 Cell::LatSymMat66( const std::string& latsym, const Cell& c ) {
   return c.LatSymMat66( latsym );
}

const Mat66 HexPerp(Mat66().Eye() - Mat66( " 1 1 0 0 0 -1   1 1 0 0 0 -1   0 0 3 0 0 0   0 0 0 0 0 0   0 0 0 0 0 0   -1 -1 0 0 0 1 " ) );
const Mat66 RhmPerp(Mat66().Eye() - Mat66( " 1 1 1 0 0 0   1 1 1 0 0 0   1 1 1 0 0 0   0 0 0 1 1 1    0 0 0 1 1 1    0 0 0 1 1 1 " ) );

bool Cell::IsRhomobhedralAsHex( void ) const {
   return IsRhomobhedralAsHex( (*this).Cell2V6() );
}

/*static*/ bool Cell::IsRhomobhedralAsHex( const Cell& c ) {
   return IsRhomobhedralAsHex( c.Cell2V6() );
}


// Assumes the cell EITHER has alpha=beta=gamma or a=b & alpha=beta=90 and gamma=120 (approximately)
/*static*/ bool Cell::IsRhomobhedralAsHex( const G6& v ) {
   return (HexPerp*v).norm() < (RhmPerp*v).norm();
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: vvtorow(const double m,
//               const int n1, const int n2,
//               mat33& v1, mat33& v2,
//               const int n3,
//               Mat66& m6)
// Description: Compute one row of a 6x6 matrix from
//              2 rows of 2 3x3 matrices
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


Cell Cell::operator* ( const double d ) const {
   Cell c(*this);
   c[0] *= d;
   c[1] *= d;
   c[2] *= d;
   return c;
}

Cell Cell::operator+ (const Cell& c) const {
   const G6 v1((*this).Cell2V6());
   const G6 v2(c.Cell2V6());
   return Cell(G6(v1 + v2));
}

Cell Cell::operator- (const Cell& c) const {
   const G6 v1((*this).Cell2V6());
   const G6 v2(c.Cell2V6());
   return Cell(G6(v1 - v2));
}

Cell operator* ( const double d, const Cell& c ) {
   return c*d;
}

Cell& Cell::operator= (const G6& v) {
   m_cell = V62Cell(v);
   return *this;
}

Cell& Cell::operator= (const std::string& s) {
   (*this) = Cell(s);
   return *this;
}

Cell Cell::GetPrimitiveCell(const std::string& latsym, const Cell& c) {
   return Cell(GetPrimitiveV6Vector(latsym, c));
}

G6 Cell::GetPrimitiveV6Vector(const std::string& latsym, const Cell& c) {
   const Mat66 m66 = c.LatSymMat66(latsym);
   return m66 * c.Cell2V6();
}

bool Cell::operator== (const Cell& c)
{
   bool breturn = true;
   for (size_t i = 0; i < 6; ++i)
      breturn = breturn && (*this)[i] == c[i];
   return breturn;
}

bool Cell::operator!= (const Cell& c)
{
   return !((*this) == c);
}