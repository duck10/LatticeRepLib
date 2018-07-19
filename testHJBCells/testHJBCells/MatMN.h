#ifndef MatMN_H
#define MatMN_H

#include <string>
#include <vector>

#include "VecN.h"

#define Default6 6
#define Default36 36

class VecN;
class MatMN;

MatMN operator* ( const double d, const MatMN& m );
MatMN operator/ ( const double d, const MatMN& m );

class MatMN {
public:
   friend std::ostream& operator<< ( std::ostream&, const MatMN& );

   MatMN( const size_t rowCount, const size_t colCount );
   MatMN( const std::string& s );
   MatMN( const MatMN& m );
   ~MatMN( void ) {};

   MatMN operator* ( const MatMN& m2 ) const;
   MatMN operator+ ( const MatMN& m2 ) const;
   MatMN operator- ( const MatMN& m2 ) const;
   MatMN transpose( void ) const;
   MatMN inverse( void ) const;

   size_t size( void ) const { return m_dim; }

   double norm( void ) const;

   MatMN operator* ( const double d ) const;
   MatMN operator/ ( const double d ) const;
   double operator[]( const size_t i ) const;
   double& operator[]( const size_t i );
   MatMN& operator= ( const MatMN& m2 );
   bool operator== ( const MatMN& m2 ) const;
   MatMN operator*= ( const double d );
   MatMN operator/= ( const double d );
   void zeros( void );
   MatMN FromString( const std::string& s ) const;

   double operator() ( const size_t i );
   double operator() ( const size_t row, const size_t col );

   VecN operator*( const VecN& v ) const;

   size_t GetDim( void ) const { return m_dim; }
   size_t GetRowDim( void ) const { return m_rowCount; }
   size_t GetColDim( void ) const { return m_colCount; }

   MatMN Eye( void ) const;

   std::vector<double> GetMatrix( void ) const { return m_mat; }

protected:
   size_t LinearIndex( const size_t row, const size_t col) const;

protected:
   std::vector<double> m_mat;
   size_t m_dim;
   size_t m_rowCount;
   size_t m_colCount;


};

#endif

