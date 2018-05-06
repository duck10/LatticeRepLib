


#include <cmath>
#include <cfloat>

#include <climits>
#include <iostream>
/* remove the next include if using a local version of rand */
#include <stdlib.h>
#ifdef USE_SYSTEM_HEADERS
#include <LRL_Vector3.h>
#else
#include "LRL_Vector3.h"
#endif

const double    Vector_3::MINNORM = 1.0E-8;

class Matrix_3x3;


//This is a translation of an old fortran library of vector algebra code. It
//was designed to be versatile and easy to use. Surprisingly, even in 2005, it
//is in quite common use in the scientific community. 
//
//Conventions:
//   vector are internally arrays of 3 doubles
//   matrices are internally arrays of 9 doubles, indexed as follows
//      0 1 2
//      3 4 5
//      6 7 8
//


//-----------------------------------------------------------------------------
// Name: GeneralRotation()
// Original Defect:  Q-3726 larrya
// Description: For a given rotation angle about the line from v1 to v2,
//              GeneralRotation computes the transformation such that for some
//              input vector "vIn", an output vector "vOut" can be computed by:
//              vOut = m*vIn + vTrans
//              where m is the matrix returned in the output pair and vTrans
//              is the vector in that pair.
//  
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
std::pair<Matrix_3x3, Vector_3> Vector_3::GeneralRotation( 
   const double angle, 
   const Vector_3& v1, 
   const Vector_3& v2 )
{
    const Vector_3 x = v2 - v1;
    const Matrix_3x3 m = x.Rotmat( angle );
    const Vector_3 y = m.MV( v1 );
    const Vector_3 tran = v1 - y;
    return( std::make_pair( m, tran ) );
}


//-----------------------------------------------------------------------------
// Name: Rotmat()
// Description: Rotmat computes the matrix that would rotate an arbitrary vector
// about a specified vector by a specified angle.
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3 Vector_3::Rotmat( const double angle ) const
//-------------------------------------------------------------------------------------
{/*   from Wikipedia   http://en.wikipedia.org/wiki/Rotation_matrix
        R = \begin{bmatrix}
        u_x^2+(1-u_x^2)c  & u_x u_y(1-c)-u_zs & u_x u_z(1-c)+u_ys
        u_x u_y(1-c)+u_zs & u_y^2+(1-u_y^2)c  & u_y u_z(1-c)-u_xs
        u_x u_z(1-c)-u_ys & u_y u_z(1-c)+u_xs & u_z^2+(1-u_z^2)c
        \end{bmatrix},

where

    c = \cos\theta,  s = \sin\theta.
    */

    const double d = (*this).SquaredLength( );
    Vector_3 vtemp;
    if ( d != 1.0 )
    {
        vtemp = (*this) / sqrt( d );
    }
    else if ( d == 1.0 )
    {
        vtemp = (*this);
    }
    else if ( d == 0.0 )
    {
        vtemp = Vector_3::GetXAxis( );
    }
    // otherwise it was already a unit vector

    const double& ux = vtemp.v[0];
    const double& uy = vtemp.v[1];
    const double& uz = vtemp.v[2];

    const double ux2 = ux*ux;
    const double uy2 = uy*uy;
    const double uz2 = uz*uz;

    const double uxy = ux*uy;
    const double uxz = ux*uz;
    const double uyz = uy*uz;

    const double s = ::sin(angle);
    const double c = ::cos(angle);

    const Matrix_3x3 mtemp(
                                ux2+(1.0-ux2)*c,  uxy*(1.0-c)-uz*s, uxz*(1.0-c)+uy*s,
                                uxy*(1.0-c)+uz*s, uy2+(1-uy2)*c,    uyz*(1.0-c)-ux*s,
                                uxz*(1.0-c)-uy*s, uyz*(1.0-c)+ux*s, uz2+(1.0-uz2)*c

                          );
    return (mtemp);
}


//-----------------------------------------------------------------------------
// Name: MV
// Description: multiply a vector by a matrix
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Vector_3 Vector_3::MV( const Matrix_3x3& m ) const
//-------------------------------------------------------------------------------------
{
    return (Vector_3(
        m.m[0]*v[0]+m.m[1]*v[1]+m.m[2]*v[2],
        m.m[3]*v[0]+m.m[4]*v[1]+m.m[5]*v[2],
        m.m[6]*v[0]+m.m[7]*v[1]+m.m[8]*v[2]    ));
}



//-----------------------------------------------------------------------------
// Name: operator*()
// Description: multiply two matrices
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline Matrix_3x3 Matrix_3x3::operator* ( const Matrix_3x3& o ) const
//-------------------------------------------------------------------------------------
{
    return(Matrix_3x3(  m[0]*o.m[0] + m[1]*o.m[3] + m[2]*o.m[6],
        m[0]*o.m[1] + m[1]*o.m[4] + m[2]*o.m[7],
        m[0]*o.m[2] + m[1]*o.m[5] + m[2]*o.m[8],
        m[3]*o.m[0] + m[4]*o.m[3] + m[5]*o.m[6],
        m[3]*o.m[1] + m[4]*o.m[4] + m[5]*o.m[7],
        m[3]*o.m[2] + m[4]*o.m[5] + m[5]*o.m[8],
        m[6]*o.m[0] + m[7]*o.m[3] + m[8]*o.m[6],
        m[6]*o.m[1] + m[7]*o.m[4] + m[8]*o.m[7],
        m[6]*o.m[2] + m[7]*o.m[5] + m[8]*o.m[8] ) );
}


//NON MEMBER FUNCTIONS START HERE

#ifdef __cplusplus
//-----------------------------------------------------------------------------
// Name: operator<<()
// Description: stream a vector to the output stream
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::ostream& operator<< ( std::ostream& o, const Vector_3& v )
//-------------------------------------------------------------------------------------
{
    o<<v.v[0]<<" "<<v.v[1]<<" "<<v.v[2]<<std::endl;
    return o;
}

//-----------------------------------------------------------------------------
// Name: operator<<()
// Description: stream a matrix to the output stream
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::ostream& operator<< ( std::ostream& o, const Matrix_3x3& m )
//-------------------------------------------------------------------------------------
{
    o << m.m[0] << ", " << m.m[1] << ", " << m.m[2] << std::endl;
    o << m.m[3] << ", " << m.m[4] << ", " << m.m[5] << std::endl;
    o << m.m[6] << ", " << m.m[7] << ", " << m.m[8];
    return o;
}
#endif

//-----------------------------------------------------------------------------
// Name: operator*()
// Description: multiples a vector by a floating point number
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Vector_3 operator* ( const double& a, const Vector_3& v )
//-------------------------------------------------------------------------------------
{
    return ( Vector_3(a*v.v[0], a*v.v[1], a*v.v[2]) );
}

//-----------------------------------------------------------------------------
// Name: operator*()
// Description: multiples a matrix by a floating point number
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3 operator* ( const double& a, const Matrix_3x3& m )
//-------------------------------------------------------------------------------------
{
    return ( Matrix_3x3(
        a*m.m[0], a*m.m[1], a*m.m[2],
        a*m.m[3], a*m.m[4], a*m.m[5],
        a*m.m[6], a*m.m[7], a*m.m[8]
    )
        );
}


//-----------------------------------------------------------------------------
// Name: Angle()
// Description: angle between the ends of three vectors. In many cases, the
// middle vector will be the origin.
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double Vector_3::Angle( const Vector_3& a, const Vector_3& b, const Vector_3& c )
//-------------------------------------------------------------------------------------
{
     const Vector_3 x = a - b;
     const Vector_3 y = c - b;
     const Vector_3 z = x.Cross(y);
     const double xNorm = x.Norm( );
     const double yNorm = y.Norm( );
     const double zNorm = z.Norm( );
     const double dotProduct = x.Dot( y );
     double angle;
     if (xNorm > Vector_3::MINNORM && yNorm > Vector_3::MINNORM) {
         const double cosAngle = dotProduct/(xNorm*yNorm);
         const double sinAngle = zNorm/(xNorm*yNorm);
         angle = ::atan2( sinAngle, cosAngle );
     }
     else
     {
        angle = 0.0;
     }
     return( angle );
}

double Angle( const Vector_3& v1, const Vector_3& v2 )
{
   return Vector_3::Angle( v1, Vector_3::GetZeroVector( ), v2 );
}

