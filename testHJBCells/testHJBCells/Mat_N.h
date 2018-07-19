#ifndef MAT_N_H
#define MAT_N_H
#ifdef _MSC_VER
#pragma once
#endif

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "Vec_N.h"

class VecN;
class MatN;

MatN operator* ( const double d, const MatN& m );
MatN operator/ ( const double d, const MatN& m );
MatN Eye( const MatN& m );

class MatN {
public:
   friend std::ostream& operator<< ( std::ostream&, const MatN& );


   MatN( const unsigned int dim );
   MatN( const std::string& s );
   MatN( const MatN& m );
   virtual ~MatN( void ) {};

   virtual MatN operator* ( const MatN& m2 ) const;
   virtual MatN operator*= ( const MatN& m2 );
   virtual MatN operator+ ( const MatN& m2 ) const;
   virtual MatN operator- ( const MatN& m2 ) const;
   virtual MatN transpose( void ) const;
   virtual MatN inverse( void ) const;

   virtual unsigned int size( void ) const { return m_dim; }

   virtual VecN operator* ( const VecN& v ) const;
   virtual double norm( void ) const;

   virtual MatN operator* ( const double d ) const;
   virtual MatN operator/ ( const double d ) const;
   virtual double operator[]( const unsigned int i ) const;
   virtual double& operator[]( const unsigned int i );
   virtual MatN& operator= ( const MatN& m2 );
   virtual bool operator== ( const MatN& m2 ) const;
   virtual void zeros( void );
   virtual MatN FromString( const std::string& s ) const;
   virtual unsigned int GetDim( void ) const { return m_dim; }
   virtual unsigned int GetRowDim( void ) const { return m_rowdim; }
   virtual unsigned int GetColDim( void ) const { return m_rowdim; }

   virtual MatN Eye( void ) const;
   static MatN Eye( const unsigned int n );
   
   virtual double operator() ( const unsigned int i );
   virtual double operator() ( const unsigned int row, const unsigned int col );
   virtual bool IsUnit( void ) const;

   virtual std::vector<double> GetMatrix( void ) const { return m_mat; }
protected:
   std::vector<double> m_mat;
   unsigned int m_dim;
   unsigned int m_rowdim;


};

#endif

