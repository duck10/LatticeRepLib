
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Cell.h"
#include "D6.h"
#include "G6.h"
#include "Mat66.h"
#include "Vec_N_Tools.h"
#include "D7.h"
#include "vector_3d.h"

#include "Vec_N_Tools.h"

#define Default6 6

G6::G6( void )
: VecN( 6 ) {
   m_dim = 6;
   m_valid = true;
}

G6::G6(const G6& v)
: VecN(6) {
   m_dim = 6;
    m_vec = v.m_vec;
   m_valid = v.m_valid;
}

G6::G6(const double v[6])
: VecN(6) {
    m_dim = 6;
    m_vec[0] = v[0];
    m_vec[1] = v[1];
    m_vec[2] = v[2];
    m_vec[3] = v[3];
    m_vec[4] = v[4];
    m_vec[5] = v[5];
}

G6::G6(const Cell& c)
: VecN(6) {
    m_vec[0] = c[0] * c[0];
    m_vec[1] = c[1] * c[1];
    m_vec[2] = c[2] * c[2];
    m_vec[3] = 2.0 * c[1] * c[2] * cos(c[3]);
    m_vec[4] = 2.0 * c[0] * c[2] * cos(c[4]);
    m_vec[5] = 2.0 * c[0] * c[1] * cos(c[5]);
}


G6::G6( const D6& ds ) {
    m_dim = 6;
    m_vec.resize(6);
    const double& p = ds[0];
    const double& q = ds[1];
    const double& r = ds[2];
    const double& s = ds[3];
    const double& t = ds[4];
    const double& u = ds[5];
    
    double& g1 = m_vec[0];
    double& g2 = m_vec[1];
    double& g3 = m_vec[2];
    double& g4 = m_vec[3];
    double& g5 = m_vec[4];
    double& g6 = m_vec[5];
    
    g1 = -s-r-q;
    g2 = -t-r-p;
    g3 = -u-q-p;
    g4 = 2.0*p;
    g5 = 2.0*q;
    g6 = 2.0*r;
}

G6::G6(const DeloneTetrahedron& dt)
{
   (*this) = D6(dt);
}

G6::G6( const VecN& v )
: VecN(6) {
    if (m_dim != 6) {
        G6 v6;
        v6.m_dim = 6;
        m_vec = v6.m_vec;
    }
    else {
        m_vec = v.GetVector();
    }
    m_valid = true;
}

G6::G6( const D7& v7 )
: VecN( 6 ) {
   double& g1 = m_vec[0];
   double& g2 = m_vec[1];
   double& g3 = m_vec[2];
   double& g4 = m_vec[3];
   double& g5 = m_vec[4];
   double& g6 = m_vec[5];
   const double& d1 = v7[0];
   const double& d2 = v7[1];
   const double& d3 = v7[2];
   const double& d4 = v7[3];
   const double& d5 = v7[4];
   const double& d6 = v7[5];
   const double& d7 = v7[6];
   g1 = d1;
   g2 = d2;
   g3 = d3;
   g4 = d5 - d2 - d3;
   g5 = d6 - d1 - d3;
   g6 = d7 - d1 - d2;
//   g4 = (d4 -g1 -g2 -g3 -g5 -g6 + d5 - g2 -g3)/2.0; 
   m_valid = true;
}

G6::G6( const std::string& s ) 
: VecN(6)
{
  m_vec = Vec_N_Tools::FromString( s );
  m_dim = m_vec.size();
   m_valid = true;
  if ( m_dim != 6 ) throw "bad dimension in G6 from a string";
}

G6::G6( const std::vector<double>& v ) 
: VecN( v.size() )
{
  m_dim = v.size();
   if ( m_dim != 6 ) throw "bad dimension in G6 from a vector";
  m_vec = v;
   m_valid = true;
}

double G6::DistanceBetween( const G6& v1, const G6& v2 ) {
   double sum = 0.0;
   for( size_t i=0; i<6; ++i )  sum += (v1[i]-v2[i])*(v1[i]-v2[i]);
   return sqrt( sum );
}
G6& G6::operator= ( const G6& v ) {
   m_vec = v.m_vec;
   m_dim = v.m_dim;
   m_valid = v.m_valid;
   return *this;
}

G6& G6::operator= (const std::string& s)
{
   (*this) = G6(s);
   return *this;
}

G6& G6::operator= (const D7& v)
{
   (*this) = G6(v);
   return *this;
}

G6& G6::operator= (const D6& v)
{
   (*this) = G6(v);
   return *this;
}


G6& G6::operator= (const DeloneTetrahedron& v)
{
   (*this) = G6(v);
   return *this;
}


G6& G6::operator= (const Cell& v)
{
   (*this) = G6(v);
   return *this;
}



G6 G6::InverseG6Vector( void ) const {
   G6 v( *this );

   double a = sqrt( v[0] );
   double b = sqrt( v[1] );
   double c = sqrt( v[2] );
   double cosalpha = 0.5*v[3]/v[1]/v[2];
   double cosbeta  = 0.5*v[4]/v[0]/v[2];
   double cosgamma = 0.5*v[5]/v[0]/v[1];

   const double sinalpha = sqrt( 1.0-cosalpha*cosalpha );
   const double sinbeta  = sqrt( 1.0-cosbeta *cosbeta );
   const double singamma = sqrt( 1.0-cosgamma*cosgamma );

   const double volume = a*b*c*sqrt( 1.0 - cosalpha*cosalpha - cosbeta*cosbeta - cosgamma*cosgamma ) -
      2.0*cosalpha*cosbeta*cosgamma;

   const double astar = b*c*sinalpha/volume;
   const double bstar = a*c*sinbeta /volume;
   const double cstar = a*b*singamma/volume;

   const double cosalphastar = ( cosbeta *cosgamma - cosalpha ) / std::fabs( sinbeta *singamma );
   const double cosbetastar  = ( cosalpha*cosgamma - cosbeta ) / std::fabs( sinalpha*singamma );
   const double cosgammastar = ( cosalpha*cosbeta  - cosgamma ) / std::fabs( sinalpha*sinbeta );

   G6 vout;
   vout[0] = astar*astar;
   vout[1] = bstar*bstar;
   vout[2] = cstar*cstar;
   vout[3] = 2.0*bstar*cstar*cosalphastar;
   vout[4] = 2.0*astar*cstar*cosbetastar;
   vout[5] = 2.0*astar*bstar*cosgammastar;

   return vout;
}

std::ostream& operator<< ( std::ostream& o, const G6& v ) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed <<std::setprecision(3);
   for( size_t i=0; i<v.size( ); ++i )
      o << std::setw(9) << v[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

/*

std::ostream& operator<< ( std::ostream& o, const G6& v ) {
   for( size_t i=0; i<6; ++i )
      o << v[i] << " ";
   return o;
}

G6::G6( )
: m_vec( ) {
   m_vec.clear( );
   m_vec = std::vector<double>( Default6, 19191.0 );
}

G6::G6( const Matrix_3x3& m ) {
   m_vec.resize( 6 );
   m_vec[0] = m[0];
   m_vec[1] = m[4];
   m_vec[2] = m[8];

   ( *this )[3] = m[5] + m[7];
   ( *this )[4] = m[2] + m[6];
   ( *this )[5] = m[1] + m[3];
}

G6 G6::InverseG6Vector( void ) const {
   G6 v( *this );

   double a = sqrt( v[0] );
   double b = sqrt( v[1] );
   double c = sqrt( v[2] );
   double cosalpha = 0.5*v[3]/v[1]/v[2];
   double cosbeta  = 0.5*v[4]/v[0]/v[2];
   double cosgamma = 0.5*v[5]/v[0]/v[1];

   const double sinalpha = sqrt( 1.0-cosalpha*cosalpha );
   const double sinbeta  = sqrt( 1.0-cosbeta *cosbeta );
   const double singamma = sqrt( 1.0-cosgamma*cosgamma );

   const double volume = a*b*c*sqrt( 1.0 - cosalpha*cosalpha - cosbeta*cosbeta - cosgamma*cosgamma ) -
      2.0*cosalpha*cosbeta*cosgamma;

   const double astar = b*c*sinalpha/volume;
   const double bstar = a*c*sinbeta /volume;
   const double cstar = a*b*singamma/volume;

   const double cosalphastar = ( cosbeta *cosgamma - cosalpha ) / std::fabs( sinbeta *singamma );
   const double cosbetastar  = ( cosalpha*cosgamma - cosbeta ) / std::fabs( sinalpha*singamma );
   const double cosgammastar = ( cosalpha*cosbeta  - cosgamma ) / std::fabs( sinalpha*sinbeta );

   G6 vout;
   vout[0] = astar*astar;
   vout[1] = bstar*bstar;
   vout[2] = cstar*cstar;
   vout[3] = 2.0*bstar*cstar*cosalphastar;
   vout[4] = 2.0*astar*cstar*cosbetastar;
   vout[5] = 2.0*astar*bstar*cosgammastar;

   return vout;
}

G6::G6( const G6& v )
: m_vec( v.m_vec )                                                                                                                                    {
 }

G6::G6( const std::string& s )
: m_vec(Default6)
{
   const size_t vsize = m_vec.size();
   G6 v(Vec_N_Tools::FromString( s ));
   for( size_t i=0; i<Default6; ++i ) m_vec[i] = v.m_vec[i];
}

G6::~G6( ) {
   const int i19191 = 19191;
}

G6 G6::operator* ( const G6& v2 ) const {
   return G6();
}

G6 G6::operator+ ( const G6& v2 ) const {
   G6 v;
   for( size_t i=0; i<Default6; ++i )
      v[i] = (*this)[i]+v2[i];
   return v;
}

G6 G6::operator- ( const G6& v2 ) const {
   G6 v;
   for( size_t i=0; i<Default6; ++i )
      v[i] = (*this)[i]-v2[i];
   return v;
}

G6 G6::operator*=( const double d ) {
   for( size_t i=0; i<Default6; ++i )
      m_vec[i] *= d;
   return *this;
}
*/
/* friend */
/*
G6 operator*( const double d, const G6& v) {
   G6 vv(v);
   vv *= d;
   return vv;
}
*/
/*
double G6::norm( void ) const {
   double sum = 0.0;
   for( size_t i=0; i<Default6; ++i ) sum += std::pow(m_vec[i], 2);
   return std::sqrt(sum);
}

G6 G6::operator* ( const double d ) const {
   G6 temp;
   for( size_t i=0; i<Default6; ++i ) temp[i] = (*this)[i]*d;
   return temp;
}

double G6::operator[]( const size_t i ) const{
   return m_vec[i];
}

double& G6::operator[]( const size_t i ){
   return m_vec[i];
}


G6& G6::operator= (const G6& v2 ) {
   m_vec = v2.m_vec;
   const size_t size1 = m_vec.size();
   const size_t size2 = v2.m_vec.size();
   return *this;
}

double G6::dot( const G6& v2 ) const {
   const G6& v1((*this));
   if ( v2.size() != v1.size()) throw "VecN dot dimensions not equal";
   double sum = 0.0;
   for( size_t i=0; i<6; ++i ) sum += v1[i]*v2[i];
   return sum;
}

double dot( const G6& v1, const G6& v2 ) {
   double d = 0.0;
   for( size_t i=0; i<Default6; ++i ) d += v1[i]*v2[i];
   return d;
}

Mat66 G6::VV_Transpose( void ) const {
   const G6& v(*this);
   Mat66 m;
   m[0] = v[0]*v[0];
   m[1]=m[6] = v[0]*v[1];
   m[2]=m[12] = v[0]*v[2];
   m[3]=m[18] = v[0]*v[3];
   m[4]=m[24] = v[0]*v[4];
   m[5]=m[30] = v[0]*v[5];

   m[7] = v[1]*v[1];
   m[8]=m[13] = v[1]*v[2];
   m[9]= m[19] = v[1]*v[3];
   m[10]=m[25] = v[1]*v[4];
   m[11]=m[31] = v[1]*v[5];

   m[14] = v[2]*v[2];
   m[15]=m[20] = v[2]*v[3];
   m[16]=m[26] = v[2]*v[4];
   m[17]=m[32] = v[2]*v[5];

   m[21] = v[3]*v[3];
   m[22]=m[27] = v[3]*v[4];
   m[23]=m[33] = v[3]*v[5];

   m[28] = v[4]*v[4];
   m[29]=m[34] = v[4]*v[5];

   m[35] = v[5]*v[5];
   return m;
}

G6 G6::abs( const G6& v ) {
   G6 vtemp;
   for( size_t i=0; i<6; ++i ) vtemp[i] = std::abs(v[i]);
   return vtemp;
}

   G6 G6::operator/ (const double d) const {
      G6 r(*this);
      for( size_t i=0; i<6; ++i)
         r[i] /=d;
      return r;
   }
   

   */
