#ifndef MAT_N_H
#define MAT_N_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "VecN.h"

class VecN;
class MatN;

MatN operator* ( const double d, const MatN& m );
MatN operator/ ( const double d, const MatN& m );
MatN Eye( const MatN& m );

class MatN {
public:
   friend std::ostream& operator<< ( std::ostream&, const MatN& );

   MatN( const unsigned long dim );
   MatN( const std::string& s );
   MatN( const MatN& m );
   virtual ~MatN( void ) {};

   virtual MatN operator* ( const MatN& m2 ) const;
   virtual MatN operator*= ( const MatN& m2 );
   virtual MatN operator+ ( const MatN& m2 ) const;
   virtual MatN operator- (const MatN& m2) const;
   virtual MatN operator- (void) const; // unary
   virtual MatN transpose( void ) const;
   virtual MatN inverse( void ) const;

   virtual unsigned long size( void ) const { return m_dim; }

   virtual VecN operator* ( const VecN& v ) const;
   virtual double norm( void ) const;

   virtual MatN operator* ( const double d ) const;
   virtual MatN operator/ ( const double d ) const;
   virtual double operator[]( const unsigned long i ) const;
   virtual double& operator[]( const unsigned long i );
   virtual MatN& operator= ( const MatN& m2 );
   virtual bool operator== ( const MatN& m2 ) const;
   virtual void zeros( void );
   virtual MatN FromString( const std::string& s );
   virtual unsigned long GetDim(void) const { return m_dim; }
   virtual unsigned long GetRowDim(void) const { return m_rowdim; }
   virtual unsigned long GetColDim(void) const { return m_rowdim; }
   virtual void SetDim(const unsigned long n) { m_dim = n; }
   virtual void SetRowDim(const unsigned long n) { m_rowdim = n; }
   virtual void SetColDim(const unsigned long n) { m_rowdim = n; }

   virtual MatN Eye( void ) const;
   static MatN Eye( const unsigned long n );
   std::vector<double> GetVector(void) const { return m_mat; }
   void SetVector(const std::vector<double>& m) { m_mat = m; }

   virtual double operator() ( const unsigned long i );
   virtual double operator() ( const unsigned long row, const unsigned long col );
   virtual bool IsUnit( void ) const;

   virtual std::vector<double> GetMatrix( void ) const { return m_mat; }
   virtual void resize(const unsigned long n) {
      m_mat.resize(n);
      m_dim = n;
      m_rowdim = (unsigned long)(sqrt(double(n)));
   }

protected:
   unsigned long m_dim;
   std::vector<double> m_mat;
   unsigned long m_rowdim;


};

#endif

