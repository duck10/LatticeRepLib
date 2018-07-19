
#include <cmath>

#include "MatMN.h"
#include "Vec_N_Tools.h"

MatMN::MatMN( const MatMN& m )
: m_mat( m.m_mat )
, m_rowCount( m.m_rowCount )
, m_dim( m.m_dim )
, m_colCount( m.m_colCount ) {
}

MatMN::MatMN( const size_t rowCount, const size_t colCount )
: m_mat( colCount*rowCount )
, m_rowCount( rowCount )
, m_dim( m_mat.size( ) )
, m_colCount( colCount ) {
}

MatMN::MatMN( const std::string& s )
: m_mat()
, m_dim( 0 )
{
   m_mat = Vec_N_Tools::FromString( s );
}

size_t MatMN::LinearIndex( const size_t row, const size_t col) const {
   return row*(*this).m_colCount + col;
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
   std::swap( m.m_rowCount, m.m_colCount );
   const size_t rowsize = ( *this ).m_rowCount;
   for ( size_t count=0; count<rowsize; ++count ) {
      const size_t transposeIndex = count/rowsize + rowsize*( count%rowsize );
      if ( count >= transposeIndex ) {
         m[transposeIndex] = ( *this )[count];
         m[count] = ( *this )[transposeIndex];
      }
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
   VecN vout( 10 );
   vout.zero( );

   for (size_t row=0; row<m.m_rowCount; ++row ) {
      for (size_t col = 0; col<m.m_colCount; ++col )
         vout[col] += v[col] * m.m_mat[LinearIndex( row, col )];
   }

   return vout;
}

MatMN MatMN::inverse( void ) const {
   MatMN I( ( *this ).GetRowDim( ), ( *this ).GetColDim( ) );
   if ( ( *this ).GetRowDim( ) != ( *this ).GetColDim( ) ) throw "inverse requires square matrix";

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

