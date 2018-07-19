#ifndef MAT7_H
#define MAT7_H

#include <string>
#include <vector>

#include "Mat66.h"
#include "MatN.h"
#include "MatMN.h"
#include "VecN.h"
#include "D7.h"

class Mat66;
class G6;

class Mat7: public MatN {
public:
   Mat7( void );
   Mat7( const Mat66& m );
   Mat7( const Mat7& m );
   Mat7( const MatN& m );
   Mat7( const MatMN& m );
   Mat7( const std::string& s );
   Mat7( const std::vector<double>& v );

   Mat7 Reduce( void ) const;
   Mat7 operator= ( const Mat7& m );
   Mat7 operator* ( const Mat7& m2 ) const;

   D7 operator* (const D7& v) const { return MatN(*this)*v; }

   double operator[] (const size_t n) const { return MatN::m_mat[n]; }
   double& operator[] (const size_t n) { return MatN::m_mat[n]; }

   void ReplaceTwosInRows7x7( Mat7& m );

};

#endif

