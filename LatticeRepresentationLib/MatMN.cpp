


#include <cmath>

#include "MatS6.h"
#include "MatD7.h"
#include "MatG6.h"
#include "MatMN.h"
#include "LRL_StringTools.h"

MatMN::MatMN( const MatMN& m )
: m_mat( m.m_mat )
, m_rowCount( m.m_rowCount )
, m_dim( m.m_dim )
, m_colCount( m.m_colCount ) {
}

MatMN::MatMN( const size_t rowCount, const size_t colCount )
: m_mat( colCount*rowCount )
, m_rowCount( rowCount )
, m_dim( (size_t)m_mat.size( ) )
, m_colCount( colCount ) {
}

MatMN::MatMN(const MatS6& m6)
   : m_mat(m6.GetVector())
   , m_rowCount(m6.GetRowDim())
   , m_dim((size_t)m_mat.size())
   , m_colCount(m6.GetRowDim())
{
}

MatMN::MatMN(const MatD7& m7)
   : m_mat(m7.GetVector())
   , m_rowCount(m7.GetRowDim())
   , m_dim((size_t)m_mat.size())
   , m_colCount(m7.GetRowDim())
{
}

MatMN::MatMN(const MatG6& m6)
   : m_mat(m6.GetVector())
   , m_rowCount(m6.GetRowDim())
   , m_dim((size_t)m_mat.size())
   , m_colCount(m6.GetRowDim())
{
}

MatMN::MatMN(const size_t rowCount, const size_t colCount, const std::string& s )
   : m_mat()
   , m_rowCount(rowCount)
   , m_dim(colCount*rowCount)
   , m_colCount(colCount) {
   m_mat = LRL_StringTools::FromString( s );
}

size_t MatMN::LinearIndex( const size_t row, const size_t col) const {
   const size_t n = row*(*this).m_colCount + col;
   return n;
}

double  MatMN::operator[]( const size_t i ) const {
   return m_mat[i];
}

double& MatMN::operator[]( const size_t i ) {
   return m_mat[i];
}

MatMN MatMN::transpose( void ) const {
   //  transpose a symmetrical matrix
   MatMN m( *this );
   for (size_t i = 0; i < m_dim; ++i) m.m_mat[i] = 19191;
   std::swap( m.m_rowCount, m.m_colCount );

   for (size_t ij = 0; ij < m_dim; ++ij) {
      const size_t i = ij % m_colCount;
      const size_t j = ij / m_colCount;
      const size_t n = i * m.m_colCount + j;
      m[n] = (*this)[ij];
   }
   return m;
}

MatMN MatMN::operator*( const MatMN& m2 ) const {
   const MatMN& m1( *this );
   if ( m1.m_colCount != m2.m_rowCount ) throw;
   MatMN m3( m_rowCount, m2.m_colCount );

   for (size_t i_outputRowNum=0; i_outputRowNum<m3.m_rowCount; ++i_outputRowNum ) {
      for (size_t j_outputColNum=0; j_outputColNum<m3.m_colCount; ++j_outputColNum ) {
         const size_t index3 = m3.LinearIndex( i_outputRowNum, j_outputColNum );
         m3[index3]=0;
         for (size_t k_inputRowNum=0; k_inputRowNum<m2.m_rowCount; ++k_inputRowNum ) {
            const size_t index1 = LinearIndex( i_outputRowNum, k_inputRowNum );
            const size_t index2 = m2.LinearIndex( k_inputRowNum, j_outputColNum );
            m3[index3] += m1[index1]*m2[index2];
         }
      }
   }
   return m3;
}

VecN MatMN::operator*( const VecN& v ) const {
   const MatMN& m( *this );
   const size_t vdim = v.GetDim( );
   if ( vdim != m.m_colCount ) throw "Vector Dimension not same as matrix";
   VecN vout( m.GetRowDim() );
   vout.zero( );

   for (size_t row=0; row<m.m_rowCount; ++row ) {
      for (size_t col = 0; col < m.m_colCount; ++col) {
         const size_t index = LinearIndex(row, col);
         const double currentMatValue = m.m_mat[LinearIndex(row, col)];
         const double d = v[col] * m.m_mat[LinearIndex(row, col)];
         const double sum = vout[row] + d;
         vout[row] += d;
      }
   }

   return vout;
}

MatMN MatMN::inverse( void ) const {
   MatMN I( ( *this ).GetRowDim( ), ( *this ).GetColDim( ) );
   if ( ( *this ).GetRowDim( ) != ( *this ).GetColDim( ) ) throw "inverse requires square matrix";
   throw;
   return I;
}

MatMN& MatMN::operator= ( const MatMN& m2 ) {
   m_mat = m2.m_mat;

   if ( m2.m_dim <= 0 ) {
   }
   else {
      m_dim = m2.m_dim;
      m_rowCount = m2.m_rowCount;
      m_colCount = m2.m_colCount;
   }
   return *this;
}

bool MatMN::operator== (const MatMN& m2) const {
   if ((*this).size() != m2.size()) throw;
   if ((*this).m_rowCount != m2.m_rowCount) throw;
   return m_mat == m2.m_mat;
}

bool MatMN::operator!= (const MatMN& m2) const {
   return !((*this) == m2);
}

MatMN  MatMN::operator+ ( const MatMN& m2 ) const {
   const MatMN& m1( *this );
   if ( m1.m_colCount != m2.m_colCount || m1.m_rowCount != m2.m_rowCount ) throw;
   std::vector<double> v( m_dim );

   MatMN matout( m1 );
   for ( size_t i=0; i<m_dim; ++i )
      matout.m_mat[i] = m1.m_mat[i] + m2.m_mat[i];

   return matout;
}

MatMN  MatMN::operator- ( const MatMN& m2 ) const {
   const MatMN& m1( *this );
   if ( m1.m_colCount != m2.m_colCount || m1.m_rowCount != m2.m_rowCount ) throw;
   std::vector<double> v( m_dim );

   MatMN matout( m1 );
   for ( size_t i=0; i<m_dim; ++i )
      matout.m_mat[i] = m1.m_mat[i] - m2.m_mat[i];

   return matout;
}


std::ostream& operator<< ( std::ostream& o, const MatMN& m ) {
   const size_t dim = m.GetDim( );
   const size_t n = m.GetRowDim( );
   for ( size_t i=0; i<dim; ++i ) {
      if ( i > 0 && i%m.GetColDim( )==0 ) o << std::endl;
      o << ( std::fabs( m[i] ) < 1.0E-20? 0.0: m[i] ) << " ";
   }
   return o;
}

MatMN MatMN::operator* ( const double d ) const {
   MatMN m2( *this );
   for ( size_t i=0; i<m_dim; ++i )
      m2[i] = d*m2[i];
   return m2;
}

MatMN operator* ( const double d, const MatMN& m1 ) {
   MatMN m2( m1 );
   for ( size_t i=0; i<m2.GetDim( ); ++i )
      m2[i] = d*m2[i];
   return m2;
}

MatMN MatMN::Eye( void ) const {
   MatMN m( *this );
   for (size_t row=0; row<m_rowCount; ++row ) {
      for (size_t col=0; col<m_colCount; ++col ) {
         const size_t index = LinearIndex( row, col );
         m.m_mat[index] = ( row!=col ) ? 0 : 1;
      }
   }
   return m;
}

MatMN MatMN::operator*= ( const double d ){
   for ( size_t i=0; i<m_dim; ++i )
      m_mat[i] *= d;
   return *this;
}

MatMN MatMN::operator/ ( const double d ) const{
   MatMN m(*this);
   for ( size_t i=0; i<m_dim; ++i )
      m.m_mat[i] /= d;
   return m;
}

MatMN MatMN::operator/= ( const double d ){
   for ( size_t i=0; i<m_dim; ++i )
      m_mat[i] /= d;
   return *this;
}

double MatMN::operator() ( const size_t i ) {
   return m_mat[i];
}

double MatMN::operator() ( const size_t row, const size_t col ) {
   return m_mat[LinearIndex(  row, col )];
}

MatMN MatMN::operator* (const MatS6& m6) const {
   return (*this) * MatMN(m6);
}

MatMN MatMN::operator* (const MatD7& m7) const {
   return (*this) * MatMN(m7);
}

MatMN MatMN::operator* (const MatG6& m6) const {
   return (*this) * MatMN(m6);
}
