
#include <iostream>
#include <cmath>
#include <sstream>

#include "G6.h"

#include "Mat66.h"
#include "Vec_N_Tools.h"
#include "vector_3d.h"
#include "VectorTools.h"

#define Default7 7

#include "Mat7.h"
#include "D7.h"

#define Default49 49

Mat7::Mat7( void )
: MatN( Default49 ) {
}

Mat7::Mat7( const Mat7& m )
: MatN( Default49 ) {
   m_mat = m.m_mat;
   m_dim = m.m_dim;
   m_rowdim = m.m_rowdim;
}

Mat7::Mat7( const Mat66& m6 )
: MatN( Default49 ) {
   MatMN m66( 6,6 );
   for( size_t i=0; i<36; ++i ) m66[i] = m6[i];
   *this = VectorTools::g_m6To7 * m66 * VectorTools::g_m7To6;
}

Mat7::Mat7( const MatN& m )
: MatN( m.size( ) ) {
   if ( m.size( ) != 49 || m.GetRowDim( ) != 7 ) throw "bad dimension in Mat7";
   ( *this ).m_mat = m.GetMatrix( );
}


Mat7::Mat7( const MatMN& m )
: MatN( Default49 ) {
   if ( m.size( ) != 49 || m.GetRowDim( ) != 7 ) throw "bad dimension in Mat7";
   ( *this ).m_mat = m.GetMatrix( );
}

Mat7::Mat7( const std::string& s ) 
: MatN(Default49)
{
  m_mat = Vec_N_Tools::FromString( s );
  m_dim = m_mat.size();
  if ( m_dim != Default49 ) throw "bad dimension in Mat7 from a string";
  m_rowdim = Default7;
}

Mat7::Mat7( const std::vector<double>& v ) 
: MatN( v.size() )
{
  m_dim = v.size();
  if ( m_dim != Default49 ) throw "bad dimension in Mat7 from a string";
  m_rowdim = Default7;
  m_mat = v;
}

Mat7 Mat7::operator= ( const Mat7& m ) {
   ( *this ).m_mat = m.GetMatrix( );
   m_dim = m.m_dim;
   m_rowdim = m.m_rowdim;
   return *this;
}

Mat7 Mat7::operator* ( const Mat7& m ) const {
   return MatN(*this)*MatN(m);
}

//static const Mat7 mReducer( "0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0");
static std::vector<Mat7> vReducer;

bool IsBetter( const Mat7& m1, const Mat7& m2 ) {
   return m1.norm() < m2.norm();
}


Mat7 Mat7::Reduce( void ) const {
   if ( vReducer.empty( ) ) {
      vReducer.push_back( Mat7( "1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0" ) );
      vReducer.push_back( Mat7( "0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0" ) );
      vReducer.push_back( Mat7( "0 0 0 0 0 0 0   0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0" ) );
      vReducer.push_back( Mat7( "0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0" ) );
      vReducer.push_back( Mat7( "0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0   0 0 0 0 0 0 0" ) );
      vReducer.push_back( Mat7( "0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0" ) );
      vReducer.push_back( Mat7( "0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1" ) );
   }
   Mat7 mToReduce( *this );
   double mNorm = mToReduce.norm( );
   const double testValue = 1.0E-6 * mNorm;
   double lastNorm = mNorm;

   int count = 0;

   while (count <1 || mNorm - lastNorm > testValue) {
      ++count;
      for( size_t nloops = 1; nloops < 4; ++nloops ) {
         for( size_t i = 0; i < vReducer.size(); ++i) {
            const Mat7 m1 = mToReduce - vReducer[i];
            const Mat7 m2 = mToReduce + vReducer[i];

            if (IsBetter( m1, mToReduce ))
               mToReduce = m1;
            if (IsBetter( m2, mToReduce ))
               mToReduce = m2;
         }
      }
   }

   return mToReduce;
}

//const double t1[10] = { 2, 0, 0, 0, 0, 1, 1, 1, 0, 0 };
//const double t2[10] = { 0, 2, 0, 0, 1, 0, 1, 0, 1, 0 };
//const double t3[10] = { 0, 0, 2, 0, 1, 1, 0, 0, 0, 1 };
//const double t4[10] = { 0, 0, 0, 2, 0, 0, 0, 1, 1, 1 };
//
//const double tx[10] = { 1, 1, 1, -1, 1, 1, 1, 0, 0, 0 };

void FixColumnMinus7( MatN& m, const size_t start, const double v[7] ) {
   for( size_t i=start, j=0; i<start+7; ++i, ++j )
      m[i] += v[j];
}
void FixColumnPlus7( MatN& m, const size_t start, const double v[7] ) {
   for( size_t i=start, j=0; i<start+7; ++i, ++j )
      m[i] -= v[j];
}

//// special function for simplifying 7-space transformation matrices
//void Mat7::ReplaceTwosInRows7x7( Mat7& m ) {
//   for( size_t i=0; i<49; i = i+7 ) {
//      if ( m[i+0] == -2.0 ) FixColumnMinus( m, i, t1 );
//      if ( m[i+1] == -2.0 ) FixColumnMinus( m, i, t2 );
//      if ( m[i+2] == -2.0 ) FixColumnMinus( m, i, t3 );
//      if ( m[i+3] == -2.0 ) FixColumnMinus( m, i, t4 );
//      if ( m[i+0] == 2.0 ) FixColumnPlus( m, i, t1 );
//      if ( m[i+1] == 2.0 ) FixColumnPlus( m, i, t2 );
//      if ( m[i+2] == 2.0 ) FixColumnPlus( m, i, t3 );
//      if ( m[i+3] == 2.0 ) FixColumnPlus( m, i, t4 );
//
//      if ( m[i]==1.0 && m[i+1]==1.0 && m[i+2]==1.0 && m[i+3]==0.0 && m[i+4]==1.0 && m[i+5]==1.0 && m[i+6]==1.0 ) FixColumnPlus( m, i, tx );
//   }
//}

