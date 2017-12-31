#ifndef MatMN_H
#define MatMN_H

#include <string>
#include <vector>

#include "VecN.h"

#define Default6 6
#define Default36 36

class VecN;
class MatMN;
class MatS6;
class MatD7;
class MatG6;

MatMN operator* ( const double d, const MatMN& m );
MatMN operator/ ( const double d, const MatMN& m );

class MatMN {
public:
   friend std::ostream& operator<< ( std::ostream&, const MatMN& );

   MatMN( const unsigned long rowCount, const unsigned long colCount );
   MatMN(const unsigned long rowCount, const unsigned long colCount, const std::string& s );
   MatMN( const MatMN& m );
   MatMN(const MatS6& m6);
   MatMN(const MatD7& m6);
   MatMN(const MatG6& m6);
   ~MatMN( void ) {};

   MatMN operator* ( const MatMN& m2 ) const;
   MatMN operator+ ( const MatMN& m2 ) const;
   MatMN operator- ( const MatMN& m2 ) const;
   MatMN transpose( void ) const;
   MatMN inverse( void ) const;

   unsigned long size( void ) const { return m_dim; }

   double norm( void ) const;

   MatMN operator* ( const double d ) const;
   MatMN operator/ ( const double d ) const;
   double operator[]( const unsigned long i ) const;
   double& operator[]( const unsigned long i );
   MatMN& operator= ( const MatMN& m2 );
   bool operator== (const MatMN& m2) const;
   bool operator!= (const MatMN& m2) const;
   MatMN operator*= ( const double d );
   MatMN operator/= ( const double d );

   MatMN operator* (const MatS6& m6) const;
   MatMN operator* (const MatD7& m6) const;
   MatMN operator* (const MatG6& m6) const;

   void zeros( void );
   MatMN FromString( const std::string& s ) const;

   double operator() ( const unsigned long i );
   double operator() ( const unsigned long row, const unsigned long col );

   VecN operator*( const VecN& v ) const;

   unsigned long GetDim( void ) const { return m_dim; }
   unsigned long GetRowDim( void ) const { return m_rowCount; }
   unsigned long GetColDim( void ) const { return m_colCount; }
   std::vector<double> GetVector(void) const { return m_mat; }

   MatMN Eye( void ) const;

   std::vector<double> GetMatrix( void ) const { return m_mat; }

protected:
   unsigned long LinearIndex( const unsigned long row, const unsigned long col) const;

protected:
   std::vector<double> m_mat;
   unsigned long m_dim;
   unsigned long m_rowCount;
   unsigned long m_colCount;


};

#endif

