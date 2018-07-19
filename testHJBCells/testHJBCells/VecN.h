#ifndef VEC_N_H
#define VEC_N_H

#include <cmath>
#include <string>
#include <vector>
#include "MatN.h"

class MatN;
class Matrix_3x3;
//#define Default6 6

class VecN {
public:
   friend VecN operator* ( const double d, const VecN&v );
   friend std::ostream& operator<< ( std::ostream&, const VecN& );
   friend double dot( const VecN& v1, const VecN& v2 ) { return v1.dot( v2 ); }

   VecN(  ){}
   VecN( const size_t dim );
   VecN( const std::string& s );
   VecN( const Matrix_3x3& m );
   VecN( const VecN& v );
   VecN( const std::vector<double>& v );
   void resize(const size_t n) { m_vec.resize(n); m_dim = n; }
   virtual ~VecN( void );

   virtual std::vector<double> GetVector( void ) const { return m_vec; }
   const double* data() const { return m_vec.data(); }

   virtual VecN operator* ( const VecN& v2 ) const;
   virtual VecN operator+ ( const VecN& v2 ) const;
   virtual VecN operator- ( const VecN& v2 ) const;
   virtual VecN operator*=( const double d );
   virtual VecN operator/=( const double d );
   virtual VecN& operator=( const VecN& v );
   virtual VecN operator* ( const double d ) const;
   virtual VecN operator/ ( const double d ) const;
   virtual double operator[]( const size_t i ) const;
   virtual double& operator[]( const size_t i );
   virtual double at( const size_t i ) const { return ( *this )[i]; }
   virtual bool operator== ( const VecN& v2 ) const;
   virtual bool operator!= ( const VecN& v2 ) const;
   virtual VecN operator- ( void );

   VecN operator- (void) const {
      std::vector<double> v;
      for (size_t i = 0; i < v.size(); ++i)
         v.push_back(-m_vec[i]);
      return *this; // unary
   }

   void SetValid( const bool b ) { m_valid = b; }
   bool GetValid( void ) const { return m_valid; }

   virtual size_t size( void ) const { return (size_t)(m_vec.size( )); }
   virtual void clear( void ) { m_vec.clear( ); }
   virtual double norm( void ) const;
   virtual double Norm( void ) const;
   static double DistanceBetween( const VecN& v1, const VecN& v2 );
   virtual double dot( const VecN& v2 ) const;
   virtual MatN VV_Transpose( void ) const;
   virtual VecN abs( const VecN& v );
   virtual size_t GetDim( void ) const { return m_dim; }
   virtual void zero( void ) { for ( size_t i=0; i<m_dim; ++i ) m_vec[i] = 0.0; }

protected:
   std::vector<double> m_vec;
   size_t m_dim;
   bool m_valid;

};

#endif
