#ifndef NC_MAT_H
#define NC_MAT_H

#include <string>
#include <vector>

#include "VectorTools.h"

//#include "NC_Vec.h"

#define Default6 6
#define Default36 36

class G6;
class Mat7;
class Mat66;

Mat66 operator* ( const double d, const Mat66& m );
Mat66 operator/ ( const double d, const Mat66& m );

class Mat66 {
public:
   friend std::ostream& operator<< ( std::ostream&, const Mat66& );

   Mat66( );
   Mat66( const std::string& s );
   Mat66( const Mat66& m );
   Mat66( const Mat7& m7 );
   Mat66( const MatN& m );
   ~Mat66( void ) {};

   Mat66 operator* ( const Mat66& m2 ) const;
   Mat66 operator+ ( const Mat66& m2 ) const;
   Mat66 operator- ( const Mat66& m2 ) const;
   Mat66 transpose( const Mat66& m2 ) const;
   Mat66 inverse( const Mat66& m2 ) const;

   G6 operator* ( const G6& v ) const;
   double norm( void ) const;

   Mat66 operator* ( const double d ) const;
   Mat66 operator/ ( const double d ) const;
   double operator[]( const size_t i ) const;
   double& operator[]( const size_t i );
   Mat66& operator= ( const Mat66& m2 );
   bool operator== ( const Mat66& m2 ) const;
   void zeros( void );

   static Mat66 Eye( void );
   bool IsUnit( void ) const;

   std::vector<double> m_mat;

protected:

};

#endif

