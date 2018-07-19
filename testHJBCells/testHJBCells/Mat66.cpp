#include <cstdlib>
#include <cstdio>
#include <iostream>


#include <cmath>
#include <sstream>

#include "Mat66.h"
#include "Mat7.h"
#include "MatMN.h"
#include "G6.h"
#include "VectorTools.h"

Mat66::Mat66( )
: m_mat( Default36 )                                                                                                                                                                                           {
  for( size_t i=0; i<Default36; ++i )
      m_mat[i] = 19191.0;
}

Mat66::Mat66( const Mat66& m )
: m_mat(Default36)
{
   for( size_t i=0; i<Default36; ++i ) m_mat[i] = m.m_mat[i];
}

Mat66::Mat66( const Mat7& m7 )
{
   MatMN m77( 7,7 );
   for ( size_t i=0; i<49; ++i ) m77[i] = m7[i];
   const MatMN mn = VectorTools::g_m7To6 * m77 * VectorTools::g_m6To7;
   m_mat.resize(36);
   for( size_t i=0; i<36; ++i )
      m_mat[i] = mn[i];
}
/*
Mat7::Mat7( const Mat66& m6 )
: MatN( Default49 ) {
   MatMN m66( 6,6 );
   for( size_t i=0; i<36; ++i ) m66[i] = m6[i];
   MatMN m67 = m66 * g_m7To6;
   MatMN mupTo7Rows(7,6);
   *this = g_m6To7 * m66 * g_m7To6;
*/

Mat66::Mat66( const MatN& m ) {
   if ( m.GetDim()!=36 || m.GetRowDim() != 6 ) throw;
   m_mat = m.GetMatrix();
}

Mat66::Mat66( const std::string& s )
: m_mat()
{
   std::istringstream istr(s);
   double d;
   for( size_t i=0; i<Default36; ++i ) {
         istr >> d;
         m_mat.push_back( d );
   }
}

Mat66 Mat66::operator* ( const Mat66& m2 ) const {
   Mat66 m;
   m.zeros();
   const Mat66& m1(*this);
   for ( int count=0; count<Default36; ++count ){
      for (int i=0; i<Default6; ++i ) {
         m[count] += m1[Default6*(count/Default6)+i] *m2[(count%Default6)+Default6*i];
      }
   }
   return m;
}

Mat66 Mat66::operator+ ( const Mat66& m2 ) const {
   Mat66 m;
   for( size_t i=0; i<Default36; ++i )
      m[i] = (*this)[i]+m2[i];
   return m;
}

Mat66 Mat66::operator- ( const Mat66& m2 ) const {
   Mat66 m(*this);
   for( size_t i=0; i<Default36; ++i )
      m[i] -= m2[i];
   return m;
}

Mat66 Mat66::transpose(const Mat66& m2) const {
   //  transpose a symmetrical matrix
   const int size = Default6;
   Mat66 m;
   for(int count=0; count<size*size; ++count) {
      const int transposeIndex = count/size + size*(count%size); //'=int(rowindex/6) +6*colIndex)
      if(count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
   return m;
}

bool Mat66::IsUnit( void ) const {
   const Mat66& m(*this);
   for( size_t i=0; i<36; i+=7)
      if ( m[i] !=1.0 || m[7]!=1.0) return false;
   double sum = 0.0;
   for( size_t i=0; i<36; ++i ) sum += std::fabs(m[i]);
   return sum == 6.0;
}

inline size_t LinearIndex(const size_t row,const size_t col,const size_t sizeOfRow) {
   return row*sizeOfRow + col;
}

Mat66 Mat66::inverse   ( const Mat66& m2 ) const {
   Mat66 m( (*this) );
   Mat66 I = m.Eye();
   const size_t rowsize = Default6;
   for( size_t k=0;k<rowsize;k++) {
      const double temp=m[LinearIndex(k,k,rowsize)];
      for( size_t j=0;j<rowsize;j++) {
         const size_t index = LinearIndex(k,j,rowsize);
         m[index]/=temp;
         I[index]/=temp;
      }
      for( size_t i=0;i<rowsize;i++) {
         if(i==k) continue;
         const double temp2=m[LinearIndex(i,k,rowsize)];
         for( size_t j=0;j<rowsize;j++) {
            const size_t indexij = LinearIndex(i,j,rowsize);
            const size_t indexkj = LinearIndex(k,j,rowsize);
            m[indexij]-=m[indexkj]*temp2;
            I[indexij]-=m[indexkj]*temp2;
         }
      }
   }
   return I;
}

G6 Mat66::operator* ( const G6& v ) const {
   G6 vout;
   for( size_t i=0; i<Default6; ++i ) vout[i] =0.0;

   int count = 0;
   for( size_t im=0; im<Default36; im+=Default6) {
      double d = 0.0;
      for( size_t j=0; j<Default6; ++j) {
         d += (*this)[im+j]* v[j];
      }
//      std::cout << "d " << d << std::endl << std::endl;
      vout[count] = d ;
      ++count;
   }

   return vout;
}

double Mat66::norm( void ) const {
   double sum = 0.0;
   for( size_t i=0; i<Default36; ++i )
      sum += std::pow(m_mat[i], 2);
   return std::sqrt(sum);
}

Mat66 Mat66::operator*(const double d) const {
   Mat66 m2;
   for( size_t i=0; i<Default36; ++i )
      m2.m_mat[i] = d*m_mat[i];
  return m2;
}

Mat66 Mat66::operator/(const double d) const {
   Mat66 m2;
   for( size_t i=0; i<Default36; ++i )
      m2.m_mat[i] = m_mat[i]/d;
  return m2;
}

Mat66 operator*(const double d, const Mat66& m) {
   return m*d;
}

Mat66 operator/(const double d, const Mat66& m) {
   return m/d;
}

Mat66 Mat66::Eye( ) {
   Mat66 m;
   for( size_t i=0; i<Default36; ++i )
         m.m_mat[i] = 0.0;
   for( size_t i=0; i<Default36; i+=7 )
      m.m_mat[i] = 1.0;
   return m;
}

double Mat66::operator[]( const size_t i ) const{
   return m_mat[i];
}

double& Mat66::operator[]( const size_t i ){
   return m_mat[i];
}

Mat66& Mat66::operator= (const Mat66& m2 ) {
   for( size_t i=0; i<Default36; ++i ) m_mat[i] = m2.m_mat[i];
   return *this;
}

void Mat66::zeros( ) {
   for( size_t i=0; i<Default36; ++i ) m_mat[i] = 0.0;
}

bool Mat66::operator== (const Mat66& m2 ) const {
   bool b = true;
   for( size_t i=0; i<Default36; ++i) b = b && m2[i]==(*this)[i];
   return b;
}

std::ostream& operator<< ( std::ostream& o, const Mat66& v ) {
   for( size_t i=0; i<36; ++i ) {
      if ( i > 0 && i%6==0 ) o << std::endl;
      o << v[i] << " ";
   }
   return o;
}
