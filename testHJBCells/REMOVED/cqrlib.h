/*
 *  cqrlib.h
 *  
 *
 *  Created by Herbert J. Bernstein on 2/15/09.
 *  Copyright 2009 Herbert J. Bernstein. All rights reserved.
 *
 *  Revised, 8 July 2009 for CQR_FAR macro -- HJB
 */

/*  Work supported in part by NIH NIGMS under grant 1R15GM078077-01 and DOE 
 under grant ER63601-1021466-0009501.  Any opinions, findings, and 
 conclusions or recommendations expressed in this material are those of the   
 author(s) and do not necessarily reflect the views of the funding agencies.
 */

/**********************************************************************
 *                                                                    *
 * YOU MAY REDISTRIBUTE THE CQRlib API UNDER THE TERMS OF THE LGPL    *
 *                                                                    *
 **********************************************************************/

/************************* LGPL NOTICES *******************************
 *                                                                    *
 * This library is free software; you can redistribute it and/or      *
 * modify it under the terms of the GNU Lesser General Public         *
 * License as published by the Free Software Foundation; either       *
 * version 2.1 of the License, or (at your option) any later version. *
 *                                                                    *
 * This library is distributed in the hope that it will be useful,    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  *
 * Lesser General Public License for more details.                    *
 *                                                                    *
 * You should have received a copy of the GNU Lesser General Public   *
 * License along with this library; if not, write to the Free         *
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,    *
 * MA  02110-1301  USA                                                *
 *                                                                    *
 **********************************************************************/

/* A utility library for quaternion arithmetic and
 quaternion rotation math.  See
 
 "Quaternions and spatial rotation", Wikipedia
 http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
 
 K. Shoemake, "Quaternions", Department of Computer Science,
 University of Pennsylvania, Philadelphia, PA 19104,
 ftp://ftp.cis.upenn.edu/pub/graphics/shoemake/quatut.ps.Z
 
 K. Shoemake, "Animating rotation with quaternion curves",
 ACM SIGGRAPH Computer Graphics, Vol 19, No. 3, pp 245--254,
 1985.
 
 */

#ifndef CQRLIB_H_INCLUDED
#define CQRLIB_H_INCLUDED

#ifdef __cplusplus
#include <climits>
#include <cmath>
#include <cfloat>
template< typename DistanceType=double, typename VectorType=double[3], typename MatrixType=double[9] >
class CPPQR
{
private:
DistanceType w, x, y, z;

public:
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR( ) : // default constructor
w( DistanceType( 0.0 ) ),
x( DistanceType( 0.0 ) ),
y( DistanceType( 0.0 ) ),
z( DistanceType( 0.0 ) )
{
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR( const CPPQR& q ) // copy constructor
{
    if ( this != &q )
    {
        w = q.w;
        x = q.x;
        y = q.y;
        z = q.z;
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR(                  // constructor
          const DistanceType& wi,
          const DistanceType& xi, 
          const DistanceType& yi, 
          const DistanceType& zi 
          ) :
w( wi ),
x( xi ),
y( yi ),
z( zi )
{
}

/* Set -- set the values of an existing quaternion = w +ix+jy+kz */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline void Set ( const DistanceType& wi,
                 const DistanceType& xi, 
                 const DistanceType& yi, 
                 const DistanceType& zi 
                 ) 
{
    w = wi;
    x = xi;
    y = yi;
    z = zi;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline DistanceType GetW( void ) const
{
    return( w );
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline DistanceType GetX( void ) const
{
    return( x );
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline DistanceType GetY( void ) const
{
    return( y );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

inline DistanceType GetZ( void ) const
{
    return( z );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

inline CPPQR GetIm( void ) const
{
    return( CPPQR(0.,x,y,z) );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

inline CPPQR GetAxis( void ) const
{
    DistanceType anorm=sqrt((double)(x*x + y*y + z*z));
    if (anorm < DBL_MIN) return(CPPQR(0.,0.,0.,0.));
    return( CPPQR(0.,x/anorm,y/anorm,z/anorm) );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

inline double GetAngle( void ) const
{
    double cosangle, sinangle;
    DistanceType radius, anorm;
    radius = sqrt( (double) (w*w + x*x + y*y + z*z) );
    anorm = sqrt( (double) (x*x + y*y + z*z) );
    if ( anorm <= DBL_MIN) {
        return 0.;
    }
    cosangle = w/radius;
    sinangle = anorm/radius;
    return(atan2(sinangle,cosangle));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

inline CPPQR log( void ) const
{
    CPPQR axis = (*this).GetAxis();
    double PI;
    double ipnormsq;
    double angle = (*this).GetAngle();
    if ( w < 0. ) {
        ipnormsq = (double) (x*x + y*y + z*z);
        if (ipnormsq <= DBL_MIN ) {
            PI = std::atan2(1.,1.)*4.;
            axis = CPPQR(0.,1.,0.,0.);
            angle = PI;
        }
    }
    return (CPPQR(std::log((double)((*this).Norm())),axis.x*angle,axis.y*angle,axis.z*angle));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

inline CPPQR exp( void ) const
{
    const CPPQR impart = (*this).GetIm( );
    const double angle = (double)impart.Norm();
    if (angle <= DBL_MIN) { 
        return (CPPQR(cos(angle)*std::exp(w),0.,0.,0.));
    }
    const double rat = std::exp(w)*sin(angle)/angle;
    return(CPPQR(cos(angle)*std::exp(w),rat*x,rat*y,rat*z));
}

template <typename powertype>
inline CPPQR pow( const powertype p) const {
    
    return(((*this).log()*p).exp());
}


inline CPPQR pow( const int p) const {
    
    CPPQR qtemp, qaccum;
    unsigned int ptemp;
    
    if ( p == 0 ) return (CPPQR(1.0,0.,0.,0.));
    else if ( p > 0 ) {
        qtemp = *this;
        ptemp = p;
    } else {
        qtemp = (*this).Inverse();
        ptemp = -p;
    }
    qaccum = CPPQR(1.0,0.,0.,0.);
    while(1) {
        if ((ptemp&1)!= 0) {
            qaccum *= qtemp;
        }
        ptemp >>= 1;
        if (ptemp==0) break;
        qtemp *= qtemp;
    }
    
    return qaccum;
    
}

/* root -- take the given integer root  of a quaternion, returning
 the indicated mth choice from among multiple roots.
 For reals the cycle runs through first the i-based
 roots, then the j-based roots and then the k-based roots,
 out of the infinite number of possible roots of negative reals. */

inline CPPQR root( const int r, const int m) const
{

    const double PI = 4.*atan2(1.,1.);
    CPPQR qlog,qlogoverr,qaxis;
    double recip, qaxisnormsq;
    int cycle;
    
    if (r == 0) return CPPQR(DBL_MAX,DBL_MAX,DBL_MAX,DBL_MAX);
    recip = 1./((double)r);
    qlog = (*this).log();
    
    if (m != 0) {
        qaxis =(*this).GetAxis();
        qaxisnormsq = qaxis.Normsq();
        if (qaxisnormsq <= DBL_MIN) {
            cycle = (m/r)%3;
            switch (cycle) {
                case 1: 
                    qaxis = CPPQR(0.,0.,1.,0.);
                    break;
                case 2: 
                    qaxis = CPPQR(0.,0.,0.,1.);
                    break;
                default: 
                    qaxis = CPPQR(0.,1.,0.,0.);
                    break;
            }
        }
        qaxis *= 2.*PI*((double)m);
        qlog += qaxis;
    }
    qlogoverr = qlog*recip;
    return qlogoverr.exp();    
}


/* Dot product of 2 quaternions as 4-vectors */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

inline DistanceType Dot( const CPPQR& q) const
{
    return (w*q.w+x*q.x+y*q.y+z*q.z);
}

/* Add -- add a quaternion (q1) to a quaternion (q2) */   
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR operator+ ( const CPPQR& q ) const
{
    CPPQR temp;
    temp.w = w + q.w;
    temp.x = x + q.x;
    temp.y = y + q.y;
    temp.z = z + q.z;
    return( temp );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR& operator+= ( const CPPQR& q )
{
    w += q.w;
    x += q.x;
    y += q.y;
    z += q.z;
    return *this;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR& operator-= ( const CPPQR& q )
{
    w -= q.w;
    x -= q.x;
    y -= q.y;
    z -= q.z;
    return *this;
}

/* Subtract -- subtract a quaternion (q2) from a quaternion (q1)  */  
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR operator- ( const CPPQR& q ) const
{
    CPPQR temp;
    temp.w = w - q.w;
    temp.x = x - q.x;
    temp.y = y - q.y;
    temp.z = z - q.z;
    return( temp );
}

/* Unary minus -- negate a quaterion  */  
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR operator- ( void ) const
{
    CPPQR temp;
    temp.w = -w;
    temp.x = -x;
    temp.y = -y;
    temp.z = -z;
    return( temp );
}


/* Multiply -- multiply a quaternion (q1) by quaternion (q2)  */    
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR operator* ( const CPPQR& q ) const // multiply two quaternions
{
    CPPQR temp;
    temp.w = -z*q.z - y*q.y - x*q.x + w*q.w;
    temp.x =  y*q.z - z*q.y + w*q.x + x*q.w;
    temp.y = -x*q.z + w*q.y + z*q.x + y*q.w;
    temp.z =  w*q.z + x*q.y - y*q.x + z*q.w;
    return( temp );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR& operator*= ( const CPPQR& q )
{
    CPPQR temp;
    temp.w = -z*q.z - y*q.y - x*q.x + w*q.w;
    temp.x =  y*q.z - z*q.y + w*q.x + x*q.w;
    temp.y = -x*q.z + w*q.y + z*q.x + y*q.w;
    temp.z =  w*q.z + x*q.y - y*q.x + z*q.w;
    w = temp.w;
    x = temp.x;
    y = temp.y;
    z = temp.z;
    return *this;
}

/* Divide -- Divide a quaternion (q1) by quaternion (q2)  */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR operator/ ( const CPPQR& q2 ) const
{
    const DistanceType norm2sq = q2.w*q2.w + q2.x*q2.x + q2.y*q2.y + q2.z*q2.z;
    
    if ( norm2sq == 0.0 )
    {
        return( CPPQR( DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX ) );
    }
    
    CPPQR q;
    q.w =  z*q2.z + y*q2.y + x*q2.x + w*q2.w;
    q.x = -y*q2.z + z*q2.y - w*q2.x + x*q2.w;
    q.y =  x*q2.z - w*q2.y - z*q2.x + y*q2.w;
    q.z = -w*q2.z - x*q2.y + y*q2.x + z*q2.w;
    
    return( q / norm2sq );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR& operator/= ( const CPPQR& q2 )
{
    const DistanceType norm2sq = q2.w*q2.w + q2.x*q2.x + q2.y*q2.y + q2.z*q2.z;
    
    if ( norm2sq == 0.0 )
    {
        return( CPPQR( DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX ) );
    }
    
    CPPQR q;
    q.w =  z*q2.z + y*q2.y + x*q2.x + w*q2.w;
    q.x = -y*q2.z + z*q2.y - w*q2.x + x*q2.w;
    q.y =  x*q2.z - w*q2.y - z*q2.x + y*q2.w;
    q.z = -w*q2.z - x*q2.y + y*q2.x + z*q2.w;
    
    w = q.w/norm2sq;
    x = q.x/norm2sq;
    y = q.y/norm2sq;
    z = q.z/norm2sq;
    
    return *this;
}


/* ScalarMultiply -- multiply a quaternion (q) by scalar (s)  */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR operator* ( const DistanceType& d ) const // multiply by a constant
{
    CPPQR temp;
    temp.w = w*d;
    temp.x = x*d;
    temp.y = y*d;
    temp.z = z*d;
    return( temp );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR& operator*= ( const DistanceType& d )
{    
    w *= d;
    x *= d;
    y *= d;
    z *= d;
    return *this;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR& operator/= ( const DistanceType& d )
{
    if ( std::fabs((double)d) <= DBL_MIN ) {
        w = DBL_MAX;
        x = DBL_MAX;
        y = DBL_MAX;
        z = DBL_MAX;
        
    } else {
        
        w /= d;
        x /= d;
        y /= d;
        z /= d;
    }
    return *this;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR operator/ ( const DistanceType& d ) const // divide by a constant
{
    CPPQR temp;
    temp.w = w/d;
    temp.x = x/d;
    temp.y = y/d;
    temp.z = z/d;
    return( temp );
}

/* Conjugate -- Form the conjugate of a quaternion qconj */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR Conjugate ( void ) const
{
    CPPQR conjugate;
    conjugate.w =  w;
    conjugate.x = -x;
    conjugate.y = -y;
    conjugate.z = -z;
    return( conjugate );
}

/* Normsq -- Form the normsquared of a quaternion */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline DistanceType Normsq ( void ) const
{
    return( w*w + x*x + y*y + z*z );
}

/* Norm -- Form the norm of a quaternion */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline DistanceType Norm ( void ) const
{
    return sqrt( w*w + x*x + y*y + z*z );
}


/* Distsq -- Form the distance squared from a quaternion */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline DistanceType Distsq ( const CPPQR& q ) const
{
    return( (w-q.w)*(w-q.w) + (x-q.x)*(x-q.x) + (y-q.y)*(y-q.y) + (z-q.z)*(z-q.z) );
}

/* Dist -- Form the distance from a quaternion */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline DistanceType Dist ( const CPPQR& q ) const
{
    return sqrt( (w-q.w)*(w-q.w) + (x-q.x)*(x-q.x) + (y-q.y)*(y-q.y) + (z-q.z)*(z-q.z)  );
}

/* Inverse -- Form the inverse of a quaternion */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR Inverse ( void ) const
{
    CPPQR inversequaternion = (*this).Conjugate();
    const DistanceType normsq = (*this).Normsq( );
    
    if ( normsq > DistanceType( 0.0 ) )
    {
        inversequaternion = inversequaternion * DistanceType( 1.0 ) / normsq;
    }
    else
    {
        inversequaternion = CPPQR( DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX );
    }
    return( inversequaternion );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR& operator= ( const CPPQR& q ) 
{
    if ( this != &q )
    {
        w = q.w;
        x = q.x;
        y = q.y;
        z = q.z;
    }
    return( *this );
}

/* Equal */    
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline bool operator== ( const CPPQR& q ) const
{
    return( w==q.w && x==q.x && y==q.y && z==q.z );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline bool operator!= ( const CPPQR& q ) const
{
    return( (w!=q.w) || (x!=q.x) || (y!=q.y) | (z!=q.z) );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline VectorType& operator* ( const VectorType& v )
{
    return( RotateByQuaternion( v ) );
}

/* RotateByQuaternion -- Rotate a vector by a Quaternion, w = qvq* */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline void RotateByQuaternion(VectorType &w, const VectorType v )
{
    CPPQR vquat( 0.0, v[0], v[1], v[2] );
    const CPPQR wquat = (*this)*vquat;
    const CPPQR qconj = (*this).Conjugate( );
    vquat = wquat * qconj;
    w[0] = vquat.x; w[1] = vquat.y; w[2] = vquat.z;
    return;
}

inline VectorType& RotateByQuaternion(const VectorType v )
{
    CPPQR vquat( 0.0, v[0], v[1], v[2] );
    const CPPQR wquat = (*this)*vquat;
    const CPPQR qconj = (*this).Conjugate( );
    vquat = wquat * qconj;
    return VectorType(vquat.x, vquat.y, vquat.z);
}

/* Axis2Quaternion -- Form the quaternion for a rotation around axis v  by angle theta */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static inline CPPQR Axis2Quaternion ( const DistanceType& angle, const VectorType v )
{
    return( Axis2Quaternion( v, angle ) );
}

/* Axis2Quaternion -- Form the quaternion for a rotation around axis v  by angle theta */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static inline CPPQR Axis2Quaternion ( const VectorType v, const DistanceType& angle  )
{
    const DistanceType norm2sq = v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
    
    if ( norm2sq == DistanceType(0.0) )
    {
        return( CPPQR( DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX ) );
    }
    
    const DistanceType sinOverNorm = sin(angle/2.0)/sqrt(norm2sq);
    const CPPQR q( cos(angle/2.0), sinOverNorm*v[0], sinOverNorm*v[1], sinOverNorm*v[2]);
    
    return( q );
}

/* Matrix2Quaterion -- Form the quaternion from a 3x3 rotation matrix R */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static inline void Matrix2Quaternion ( CPPQR& rotquaternion, const MatrixType m )
{
    const DistanceType trace = m[0] + m[4] + m[8];
    
    if ( trace > -0.75 )
    {
        rotquaternion.w = sqrt( (1.0+trace)) / 2.0;
        const DistanceType recip = 0.25 / rotquaternion.w;
        rotquaternion.z = (m[3]-m[1]) * recip;
        rotquaternion.y = (m[2]-m[6]) * recip;
        rotquaternion.x = (m[7]-m[5]) * recip;
        return;
    }
    
    const DistanceType fourxsq = 1.0 +  m[0] - m[4] - m[8];
    
    if ( fourxsq >= 0.25 )
    {
        rotquaternion.x = sqrt( fourxsq ) / 2.0;
        const DistanceType recip = 0.25 /rotquaternion.x;
        rotquaternion.y = (m[3] + m[1])*recip;
        rotquaternion.z = (m[2] + m[6])*recip;
        rotquaternion.w = (m[7] - m[5])*recip;
        return;
    }
    
    const DistanceType fourysq = 1.0 + m[4] - m[0] - m[8];
    
    if ( fourysq >= 0.25 )
    {
        rotquaternion.y = sqrt( fourysq ) / 2.0;
        const DistanceType recip = 0.25 / rotquaternion.y;
        rotquaternion.x = (m[3] + m[1])*recip;
        rotquaternion.w = (m[2] - m[6])*recip;
        rotquaternion.z = (m[7] + m[5])*recip;
        return;
    }
    
    const DistanceType fourzsq = 1. + m[8] - m[0] - m[4];
    
    if ( fourzsq >= 0.25 )
    {
        rotquaternion.z = sqrt( fourzsq ) / 2.0;
        const DistanceType recip = 0.25 / rotquaternion.z;
        rotquaternion.w = (m[3] - m[1])*recip;
        rotquaternion.x = (m[2] + m[6])*recip;
        rotquaternion.y = (m[7] + m[5])*recip;
        return;
    }
    
    rotquaternion.x =  rotquaternion.y =  rotquaternion.z =  rotquaternion.w = 0;
    return;
}

static inline void Matrix2Quaternion (CPPQR& rotquaternion, const DistanceType R[3][3] )
{
    const DistanceType trace = R[0][0] + R[1][1] + R[2][2];
    
    if ( trace > -0.75 )
    {
        rotquaternion.w = sqrt( (1.0+trace)) / 2.0;
        const DistanceType recip = 0.25 / rotquaternion.w;
        rotquaternion.z = (R[1][0]-R[0][1]) * recip;
        rotquaternion.y = (R[0][2]-R[2][0]) * recip;
        rotquaternion.x = (R[2][1]-R[1][2]) * recip;
        return;
    }
    
    const DistanceType fourxsq =  1.0 + R[0][0] - R[1][1] - R[2][2];
    
    if ( fourxsq >= 0.25 )
    {
        rotquaternion.x = sqrt( fourxsq ) / 2.0;
        const DistanceType recip = 0.25 /rotquaternion.x;
        rotquaternion.y = (R[1][0]+R[0][1])*recip;
        rotquaternion.z = (R[0][2]+R[2][0])*recip;
        rotquaternion.w = (R[2][1]-R[1][2])*recip;
        return;
    }
    
    const DistanceType fourysq = 1.0 + R[1][1] - R[0][0] - R[2][2];
    
    if ( fourysq >= 0.25 )
    {
        rotquaternion.y = sqrt( fourysq ) / 2.0;
        const DistanceType recip = 0.25 / rotquaternion.y;
        rotquaternion.x = (R[1][0]+R[0][1])*recip;
        rotquaternion.w = (R[0][2]-R[2][0])*recip;
        rotquaternion.z = (R[2][1]+R[1][2])*recip;
        return;
    }
    
    const DistanceType fourzsq = 1. + R[2][2] - R[0][0] - R[1][1];
    
    if ( fourzsq >= 0.25 )
    {
        rotquaternion.z = sqrt( fourzsq ) / 2.0;
        const DistanceType recip = 0.25 / rotquaternion.z;
        rotquaternion.w = (R[1][0]-R[0][1])*recip;
        rotquaternion.x = (R[0][2]+R[2][0])*recip;
        rotquaternion.y = (R[2][1]+R[1][2])*recip;
        return;
    }
    
    rotquaternion.x =  rotquaternion.y =  rotquaternion.z =  rotquaternion.w = 0;
    return;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
DistanceType operator[] ( const int k ) const
{
    const int i = (k<0) ? 0 : ( (k>3)?3:k ) ;
    if ( i==0 ) return w;
    if ( i==1 ) return x;
    if ( i==2 ) return y;
    if ( i==3 ) return z;
    return( 0 ); // just to keep compilers happy
}

/* Quaternion2Matrix -- Form the 3x3 rotation matrix from a quaternion */    
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static inline void Quaternion2Matrix(MatrixType& m, const CPPQR q )
{
    const DistanceType ww = q.w*q.w;
    const DistanceType xx = q.x*q.x;
    const DistanceType yy = q.y*q.y;
    const DistanceType zz = q.z*q.z;
    
    const DistanceType  twoxy = 2.0 * q.x*q.y;
    const DistanceType  twoyz = 2.0 * q.y*q.z;
    const DistanceType  twoxz = 2.0 * q.x*q.z;
    const DistanceType  twowx = 2.0 * q.w*q.x;
    const DistanceType  twowy = 2.0 * q.w*q.y;
    const DistanceType  twowz = 2.0 * q.w*q.z;
    
    m[0] = ww+xx-yy-zz;   m[1] = twoxy - twowz; m[2] = twoxz + twowy;
    m[3] = twoxy + twowz; m[4] = ww-xx+yy-zz;   m[5] = twoyz - twowx;
    m[6] = twoxz - twowy; m[7] = twoyz + twowx; m[8] = ww-xx-yy+zz;
    
    return;

 }

static inline void Quaternion2Matrix( DistanceType m[3][3], const CPPQR q )
{
    const DistanceType ww = q.w*q.w;
    const DistanceType xx = q.x*q.x;
    const DistanceType yy = q.y*q.y;
    const DistanceType zz = q.z*q.z;
    
    const DistanceType  twoxy = 2.0 * q.x*q.y;
    const DistanceType  twoyz = 2.0 * q.y*q.z;
    const DistanceType  twoxz = 2.0 * q.x*q.z;
    const DistanceType  twowx = 2.0 * q.w*q.x;
    const DistanceType  twowy = 2.0 * q.w*q.y;
    const DistanceType  twowz = 2.0 * q.w*q.z;
    
    m[0][0] = ww+xx-yy-zz;   m[0][1] = twoxy - twowz; m[0][2] = twoxz + twowy;
    m[1][0] = twoxy + twowz; m[1][1] = ww-xx+yy-zz;   m[0][2] = twoyz - twowx;
    m[2][0] = twoxz - twowy; m[2][1] = twoyz + twowx; m[2][2] = ww-xx-yy+zz;
    
    return;
        
}

// end Quaternion2Matrix

/* Get a unit quaternion from a general one */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline CPPQR UnitQ( void ) const
{
    const DistanceType normsq = Normsq( );
    if ( normsq == 0.0 )
    {
        return( CPPQR( DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX ) );
    }
    else
    {
        return( (*this) / sqrt( Normsq( ) ) );
    }
} // end UnitQ

/* Quaternion2Angles -- Convert a Quaternion into Euler Angles for Rz(Ry(Rx))) convention */  
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline bool Quaternion2Angles ( DistanceType& rotX, DistanceType& rotY, DistanceType& rotZ ) const
{
    const DistanceType PI = 4.0 * atan( 1.0 );
    
    const DistanceType rmx0 = w*w + x*x - y*y - z*z;
    const DistanceType rmx1 = 2.0 * ( x*y - w*z );
    const DistanceType rmy0 = 2.0 * ( x*y + w*z );
    const DistanceType rmy1 = w*w - x*x + y*y - z*z;
    const DistanceType rmz0 = 2.0 * (x*z - w*y );
    const DistanceType rmz1 = 2.0 * (w*x + y*z );
    const DistanceType rmz2 = w*w - x*x - y*y + z*z;
    DistanceType srx, sry, srz, trX, trY, trZ;
    
    if ( rmz0 >= 1.0 )
    {
        sry = -0.5 * PI;
    }
    else if ( rmz0 <= -1.0 )
    {
        sry = 0.5 * PI;
    }
    else
    {
        sry = asin( -rmz0 );
    }
    
    if ( rmz0 > 0.9999995 )
    {
        srx = atan2( -rmx1, rmy1 );
        srz = 0.0;
    }
    else if ( rmz0 < -0.9999995 )
    {
        srx = atan2( rmx1, rmy1 );
        srz = 0.0;
    }
    else
    {
        srx = atan2( rmz1, rmz2 );
        srz = atan2( rmy0, rmx0 );
    }
    
    trX = PI + srx;
    if ( trX > 2.0 * PI ) trX -= 2.0 * PI;
    trY = PI + sry;
    if ( trY > 2.0 * PI ) trY -= 2.0 * PI;
    trZ = PI + srz;
    if ( trZ > 2.0 * PI ) trZ -= 2.0 * PI;
    
    const DistanceType nsum  =    fabs( cos(srx)-cos(rotX)) + fabs( sin(srx)-sin(rotX))
    + fabs( cos(sry)-cos(rotY)) + fabs( sin(sry)-sin(rotY))
    + fabs( cos(srz)-cos(rotZ)) + fabs( sin(srz)-sin(rotZ));
    
    const DistanceType tsum  =    fabs( cos(trX)-cos(rotX)) + fabs( sin(trX)-sin(rotX))
    + fabs( cos(trY)-cos(rotY)) + fabs( sin(trY)-sin(rotY))
    + fabs( cos(trZ)-cos(rotZ)) + fabs( sin(trZ)-sin(rotZ));
    
    if ( nsum < tsum )
    {
        rotX = srx; rotY = sry; rotZ = srz;
    }
    else
    {
        rotX = trX; rotY = trY; rotZ = trZ;
    }
    
    return( true );
}  // end Quaternion2Angles

/* Angles2Quaternion -- Convert Euler Angles for Rz(Ry(Rx))) convention into a quaternion */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static inline CPPQR Angles2Quaternion ( const DistanceType& rotX, const DistanceType& rotY, const DistanceType& rotZ )
{
    const DistanceType cx = cos( rotX / 2.0 );
    const DistanceType sx = sin( rotX / 2.0 );
    
    const DistanceType cy = cos( rotY / 2.0 );
    const DistanceType sy = sin( rotY / 2.0 );
    
    const DistanceType cz = cos( rotZ / 2.0 );
    const DistanceType sz = sin( rotZ / 2.0 );
    
    const CPPQR q( cx*cy*cz + sx*sy*sz,
               sx*cy*cz - cx*sy*sz,
               cx*sy*cz + sx*cy*sz,
               cx*cy*sz - sx*sy*cz 
               ); 
    return( q );
} // end Angles2Quaternion

static inline CPPQR Point2Quaternion( const DistanceType v[3] )
{
    return( CPPQR( 0.0, v[0], v[1], v[2] ) );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*  SLERP -- Spherical Linear Interpolation   
 Take two quaternions and two weights and combine them
 following a great circle on the unit quaternion 4-D sphere
 and linear interpolation between the radii
 
 This version keeps a quaternion separate from the negative
 of the same quaternion and is not appropriate for
 quaternions representing rotations.  Use CQRHLERP
 to apply SLERP to quaternions representing rotations
 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

inline CPPQR SLERP (const CPPQR& q, DistanceType w1, DistanceType w2) const
{
    CPPQR s1;
    CPPQR s2;
    CPPQR st1;
    CPPQR st2;
    CPPQR sout;
    DistanceType normsq;
    const DistanceType norm1sq=(*this).Normsq();
    const DistanceType norm2sq=q.Normsq();
    DistanceType r1,r2;
    DistanceType cosomega,sinomega;
    DistanceType omega;
    DistanceType t, t1, t2;
    
    
    t = w1/(w1+w2);
    
    if (norm1sq <= DBL_MIN) return q*(1-t);
    
    if (norm2sq <= DBL_MIN) return (*this)*t;
    
    if (fabs(norm1sq-1.)<= DBL_MIN) {
        r1 = 1.;
        s1 = *this;
    } else {
        r1 = sqrt(norm1sq);
        s1 = (*this)*(1/r1);
    }
    
    if (fabs(norm2sq-1.)<= DBL_MIN) {
        r2 = 1.;
        s2 = q;
    } else {
        r2 = sqrt(norm1sq);
        s2 = q*(1./r2);
    }
    
    cosomega = s1.Dot(s2);
    if (cosomega>=1. || cosomega<=-1.) {
        sinomega = 0.;
    } else {
        sinomega=sqrt(1.-cosomega*cosomega);
    }
    
    omega=atan2(sinomega,cosomega);
    
    if (sinomega <= 0.05) {
        t1=t*(1-t*t*omega*omega/6.);
        t2=(1-t)*(1.-(1-t)*(1-t)*omega*omega/6.);
        st1=s1*t1;
        st2=s2*t2;        
        if (cosomega >=0.) {
            sout=st1+st2;
        } else {
            if (sinomega <= 0.00001) {
                sout=CPPQR(-st1.x,st1.w,st1.z,-st1.y)-CPPQR(-st2.x,st2.w,st2.z,-st2.y);
            } else {
                sout = s1+s2;
            }
            sout=sout*(1/sout.Norm());
            if (t >= 0.5) {
                sout=sout.SLERP(s1,2-2.*t,2.*t-1.);
            }else {
                sout=sout.SLERP(s2,2.*t,1.-2.*t);
            }
        }
        normsq = sout.Normsq();
        if (normsq <= DBL_MIN) {
            return CPPQR(0.,0.,0.,0.);
        } else {
            return CPPQR(sout*(t*r1+(1-t)*r2)/sqrt(normsq));
        }
    }
    
    t1 = sin(t*omega);
    t2 = sin((1-t)*omega);
    st1=s1*t1;
    st2=s2*t2;
    sout=st1+st2;
    normsq = sout.Normsq();
    return CPPQR(sout*((r1*t+r2*(1-t))/sqrt(normsq)));
    
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*  HLERP -- Hemispherical Linear Interpolation   
 Take two quaternions and two weights and combine them
 following a great circle on the unit quaternion 4-D sphere
 and linear interpolation between the radii
 
 This is the hemispherical version, for use with quaternions
 representing rotations.  Use SLERP for full
 spherical interpolation.
 
 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

inline CPPQR HLERP (const CPPQR& q, DistanceType w1, DistanceType w2) const
{
    CPPQR s1;
    CPPQR s2;
    CPPQR st1;
    CPPQR st2;
    CPPQR sout;
    DistanceType normsq;
    const DistanceType norm1sq=(*this).Normsq();
    const DistanceType norm2sq=q.Normsq();
    DistanceType r1,r2;
    DistanceType cosomega,sinomega;
    DistanceType omega;
    DistanceType t, t1, t2;
    
    
    t = w1/(w1+w2);
    
    if (norm1sq <= DBL_MIN) return q*(1-t);
    
    if (norm2sq <= DBL_MIN) return (*this)*t;
    
    if (fabs(norm1sq-1.)<= DBL_MIN) {
        r1 = 1.;
        s1 = *this;
    } else {
        r1 = sqrt(norm1sq);
        s1 = (*this)*(1/r1);
    }
    
    if (fabs(norm2sq-1.)<= DBL_MIN) {
        r2 = 1.;
        s2 = q;
    } else {
        r2 = sqrt(norm1sq);
        s2 = q*(1./r2);
}

    cosomega = s1.Dot(s2);
    if (cosomega>=1. || cosomega<=-1.) {
        sinomega = 0.;
    } else {
        sinomega=sqrt(1.-cosomega*cosomega);
    }
    
    if (cosomega < 0.) {
        if (t < 0.5) {
            s1.w=-s1.w;s1.x=-s1.x;s1.y=-s1.y;s1.z=-s1.z; 
        } else {
            s2.w=-s2.w;s2.x=-s2.x;s2.y=-s2.y;s2.z=-s2.z; 
        }
        cosomega = -cosomega;
    }
    
    omega=atan2(sinomega,cosomega);
    
    if (sinomega <= 0.05) {
        t1=t*(1-t*t*omega*omega/6.);
        t2=(1-t)*(1.-(1-t)*(1-t)*omega*omega/6.);
        st1=s1*t1;
        st2=s2*t2;
        sout=st1+st2;
        if (sout.w < 0.) {
            sout.w = -sout.w;
            sout.x = -sout.x;
            sout.y = -sout.y;
            sout.z = -sout.z;
        }
        normsq = sout.Normsq();
        if (normsq <= DBL_MIN) {
            return CPPQR(0.,0.,0.,0.);
        } else {
            return CPPQR(sout*(t*r1+(1-t)*r2)/sqrt(normsq));
        }
    }
    
    t1 = sin(t*omega);
    t2 = sin((1-t)*omega);
    st1=s1*t1;
    st2=s2*t2;
    sout=st1+st2;
    if (sout.w < 0.) {
        sout = -sout;
    }
    normsq = sout.Normsq();
    return CPPQR(sout*((r1*t+r2*(1-t))/sqrt(normsq)));
    
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*  SLERPDist -- Spherical Linear Interpolation distance
 Form the distance between two quaternions by summing
 the difference in the magnitude of the radii and
 the great circle distance along the sphere of the
 smaller quaternion.
 
 This version keeps a quaternion separate from the negative
 of the same quaternion and is not appropriate for
 quaternions representing rotations.  Use CQRHLERPDist
 to apply SLERPDist to quaternions representing rotations
 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

inline DistanceType SLERPDist (const CPPQR& q) const
{
    CPPQR s1;
    CPPQR s2;
    CPPQR st1;
    CPPQR st2;
    CPPQR sout;
    const DistanceType norm1sq=(*this).Normsq();
    const DistanceType norm2sq=q.Normsq();
    DistanceType r1,r2;
    DistanceType cosomega,sinomega;
    DistanceType omega;
    
    if (norm1sq <= DBL_MIN) return sqrt(norm2sq);
    
    if (norm2sq <= DBL_MIN) return sqrt(norm1sq);
    
    if (fabs(norm1sq-1.)<= DBL_MIN) {
        r1 = 1.;
        s1 = *this;
    } else {
        r1 = sqrt(norm1sq);
        s1 = (*this)*(1/r1);
    }
    
    if (fabs(norm2sq-1.)<= DBL_MIN) {
        r2 = 1.;
        s2 = q;
    } else {
        r2 = sqrt(norm1sq);
        s2 = q*(1./r2);
    }
    
    cosomega = s1.Dot(s2);
    if (cosomega>=1. || cosomega<=-1.) {
        sinomega = 0.;
    } else {
        sinomega=sqrt(1.-cosomega*cosomega);
    }
    
    omega=atan2(sinomega,cosomega);
    if (r1 <= r2) return (r2-r1)+r1*fabs(omega);
    else return (r1-r2)+r2*fabs(omega);
    
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*  HLERPDist -- Hemispherical Linear Interpolation distance
 Form the distance between two quaternions by summing
 the difference in the magnitude of the radii and
 the great circle distance along the sphere of the
 smaller quaternion.
 
 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

inline DistanceType HLERPDist (const CPPQR& q) const
{
    CPPQR s1;
    CPPQR s2;
    CPPQR st1;
    CPPQR st2;
    CPPQR sout;
    const DistanceType norm1sq=(*this).Normsq();
    const DistanceType norm2sq=q.Normsq();
    DistanceType r1,r2;
    DistanceType cosomega,sinomega;
    DistanceType omega;
    
    if (norm1sq <= DBL_MIN) return sqrt(norm2sq);
    
    if (norm2sq <= DBL_MIN) return sqrt(norm1sq);
    
    if (fabs(norm1sq-1.)<= DBL_MIN) {
        r1 = 1.;
        s1 = *this;
    } else {
        r1 = sqrt(norm1sq);
        s1 = (*this)*(1/r1);
    }
    
    if (fabs(norm2sq-1.)<= DBL_MIN) {
        r2 = 1.;
        s2 = q;
    } else {
        r2 = sqrt(norm1sq);
        s2 = q*(1./r2);
    }
    
    cosomega = s1.Dot(s2);
    if (cosomega>=1. || cosomega<=-1.) {
        sinomega = 0.;
    } else {
        sinomega=sqrt(1.-cosomega*cosomega);
    }
    
    if (cosomega < 0.) {
        cosomega = -cosomega;
    }
    
    omega=atan2(sinomega,cosomega);
    if (r1 <= r2) return (r2-r1)+r1*fabs(omega);
    else return (r1-r2)+r2*fabs(omega);
    
}



}; // end class CPPQR

#ifndef CQR_NOCCODE
extern "C" {
#endif
#endif

#ifndef __cplusplus
#ifndef CQR_NOCCODE
#include <math.h>
#include <float.h>
#endif
#endif

#ifndef CQR_NOCCODE
#ifdef CQR_USE_FAR
#include <malloc.h>
#define CQR_FAR __far
#define CQR_MALLOC _fmalloc
#define CQR_FREE _ffree
#define CQR_MEMSET _fmemset
#define CQR_MEMMOVE _fmemmove
#else
#include <stdlib.h>
#define CQR_FAR
#define CQR_MALLOC malloc
#define CQR_FREE free
#define CQR_MEMSET memset
#define CQR_MEMMOVE memmove
#endif
    
#define CQR_FAILED       4
#define CQR_NO_MEMORY    2
#define CQR_BAD_ARGUMENT 1
#define CQR_SUCCESS      0
    
    typedef struct {
        double w;
        double x;
        double y;
        double z; } CQRQuaternion;
    
    typedef CQRQuaternion CQR_FAR * CQRQuaternionHandle;
    
    /* CQR Macros */
    
#define CQRMIm(impart,q) \
(impart).w = 0.; (impart).x = (q).x; (impart).y = (q).y; (impart).z = (q).z;
    
#define CQRMCopy(copy,orig) \
(copy).w = (orig).w; (copy).x = (orig).x; (copy).y = (orig).y; (copy).z = (orig).z;

#define CQRMSet(q,qw,qx,qy,qz) \
(q).w = (qw); (q).x = (qx); (q).y = (qy); (q).z = (qz);

#define CQRMAdd(sum,q1,q2) \
(sum).w = (q1).w + (q2).w; (sum).x = (q1).x + (q2).x; (sum).y = (q1).y + (q2).y; (sum).z = (q1).z + (q2).z;

#define CQRMSubtract(sum,q1,q2) \
(sum).w = (q1).w - (q2).w; (sum).x = (q1).x - (q2).x; (sum).y = (q1).y - (q2).y; (sum).z = (q1).z - (q2).z;

#define CQRMMultiply(product,q1,q2 ) \
(product).w = -(q1).z*(q2).z - (q1).y*(q2).y - (q1).x*(q2).x + (q1).w*(q2).w; \
(product).x =  (q1).y*(q2).z - (q1).z*(q2).y + (q1).w*(q2).x + (q1).x*(q2).w; \
(product).y = -(q1).x*(q2).z + (q1).w*(q2).y + (q1).z*(q2).x + (q1).y*(q2).w; \
(product).z =  (q1).w*(q2).z + (q1).x*(q2).y - (q1).y*(q2).x + (q1).z*(q2).w;        

#define CQRMDot(dotprod,q1,q2 ) \
dotprod = (q1).w*(q2).w + (q1).x*(q2).x + (q1).y*(q2).y + (q1).z*(q2).z; 
    
#define CQRMScalarMultiply(product,q,s ) \
(product).w = (q).w*s; \
(product).x = (q).x*s; \
(product).y = (q).y*s; \
(product).z = (q).z*s;

#define CQRMConjugate(conjugate,q ) \
(conjugate).w = (q).w; \
(conjugate).x = -(q).x; \
(conjugate).y = -(q).y; \
(conjugate).z = -(q).z;

#define CQRMNormsq(normsq,q) \
normsq = (q).w*(q).w + (q).x*(q).x + (q).y*(q).y + (q).z*(q).z;

#define CQRMNorm(norm,q) \
norm = sqrt((q).w*(q).w + (q).x*(q).x + (q).y*(q).y + (q).z*(q).z);

#define CQRMDistsq(distsq,q1,q2) \
distsq = ((q1).w-(q2).w)*((q1).w-(q2).w) + ((q1).x-(q2).x)*((q1).x-(q2).x) + ((q1).y-(q2).y)*((q1).y-(q2).y) +  ((q1).z-(q2).z)*((q1).z-(q2).z);
    
#define CQRMDist(dist,q1,q2) \
dist = sqrt(((q1).w-(q2).w)*((q1).w-(q2).w) + ((q1).x-(q2).x)*((q1).x-(q2).x) + ((q1).y-(q2).y)*((q1).y-(q2).y) +  ((q1).z-(q2).z)*((q1).z-(q2).z));
    
#define CQRMInverse(inverseq,q) \
{ double normsq; \
CQRMConjugate(inverseq,q); \
CQRMNormsq(normsq,q); \
if (normsq > 0.) { \
CQRMScalarMultiply(inverseq,inverseq,1./normsq); \
} \
}
    
    
    /* CQRCreateQuaternion -- create a quaternion = w +ix+jy+kz */
    
    int CQRCreateQuaternion(CQRQuaternionHandle CQR_FAR * quaternion, double w, double x, double y, double z); 
    
    /* CQRCreateEmptyQuaternion -- create a quaternion = 0 +i0+j0+k0 */
    
    int CQRCreateEmptyQuaternion(CQRQuaternionHandle CQR_FAR * quaternion) ;
    
    /* CQRFreeQuaternion -- free a quaternion  */
    
    int CQRFreeQuaternion(CQRQuaternionHandle CQR_FAR * quaternion);        
    
    /* CQRSetQuaternion -- create an existing quaternion = w +ix+jy+kz */
    
    int CQRSetQuaternion( CQRQuaternionHandle quaternion, double w, double x, double y, double z);

    /* CQRGetQuaternionW -- get the w component of a quaternion */
    
    int CQRGetQuaternionW( double CQR_FAR * qw, CQRQuaternionHandle q );
    
    /* CQRGetQuaternionX -- get the x component of a quaternion */
    
    int CQRGetQuaternionX( double CQR_FAR * qx, CQRQuaternionHandle q );
    
    /* CQRGetQuaternionY -- get the y component of a quaternion */
    
    int CQRGetQuaternionY( double CQR_FAR * qy, CQRQuaternionHandle q );
    
    /* CQRGetQuaternionZ -- get the z component of a quaternion */
    
    int CQRGetQuaternionZ( double CQR_FAR * qz, CQRQuaternionHandle q );
    
    /* CQRGetQuaternionIm -- get the imaginary component of a quaternion */
    
    int CQRGetQuaternionIm( CQRQuaternionHandle quaternion, CQRQuaternionHandle q );
    
    /* CQRGetQuaternionAxis -- get the axis for the polar representation of a quaternion */
    
    int CQRGetQuaternionAxis( CQRQuaternionHandle quaternion, CQRQuaternionHandle q );
    
    /* CQRGetQuaternionAngle -- get the angular component of the polar representation
     of aquaternion */
    
    double CQRGetQuaternionAngle( double CQR_FAR * angle, CQRQuaternionHandle q );
    
    /* CQRLog -- get the natural logarithm of a quaternion */
    
    int CQRLog( CQRQuaternionHandle quaternion, CQRQuaternionHandle q );
    
    /* CQRExp -- get the exponential (exp) of a quaternion */
    
    int CQRExp( CQRQuaternionHandle quaternion, CQRQuaternionHandle q ); 
    
    /* CQRQuaternionPower -- take a quarernion to a quaternion power */
    
    int CQRQuaternionPower( CQRQuaternionHandle quaternion, CQRQuaternionHandle q, CQRQuaternionHandle p);
    
    /* CQRDoublePower -- take a quarernion to a double power */
    
    int CQRDoublePower( CQRQuaternionHandle quaternion, CQRQuaternionHandle q, double p);
    
    /* CQRIntegerPower -- take a quaternion to an integer power */
    
    int CQRIntegerPower( CQRQuaternionHandle quaternion, CQRQuaternionHandle q, int p);
    
    /* CQRIntegerRoot -- take the given integer root  of a quaternion, returning
     the indicated mth choice from among multiple roots.
     For reals the cycle runs through first the i-based
     roots, then the j-based roots and then the k-based roots,
     out of the infinite number of possible roots of reals. */
    
    int CQRIntegerRoot( CQRQuaternionHandle quaternion, CQRQuaternionHandle q, int r, int m);
    
    /*  CQRAdd -- add a quaternion (q1) to a quaternion (q2) */
    
    int CQRAdd (CQRQuaternionHandle quaternion,  CQRQuaternionHandle q1, CQRQuaternionHandle q2 );
    
    /*  CQRSubtract -- subtract a quaternion (q2) from a quaternion (q1)  */
    
    int CQRSubtract (CQRQuaternionHandle quaternion,  CQRQuaternionHandle q1, CQRQuaternionHandle q2 );
    
    /*  CQRMultiply -- multiply a quaternion (q1) by quaternion (q2)  */
    
    int CQRMultiply (CQRQuaternionHandle quaternion,  CQRQuaternionHandle q1, CQRQuaternionHandle q2 );
    
    /*  CQRDot -- dot product of quaternion (q1) by quaternion (q2) as 4-vectors  */
    
    int CQRDot (double CQR_FAR * dotprod,  CQRQuaternionHandle q1, CQRQuaternionHandle q2 );    
    
    /*  CQRDivide -- Divide a quaternion (q1) by quaternion (q2)  */
    
    int CQRDivide (CQRQuaternionHandle quaternion,  CQRQuaternionHandle q1, CQRQuaternionHandle q2 );

    /*  CQRScalarMultiply -- multiply a quaternion (q) by scalar (s)  */
    
    int CQRScalarMultiply (CQRQuaternionHandle quaternion,  CQRQuaternionHandle q, double s );

    /*  CQREqual -- return 0 if quaternion q1 == q2  */
    
    int CQREqual (CQRQuaternionHandle q1, CQRQuaternionHandle q2 );
    
    /*  CQRConjugate -- Form the conjugate of a quaternion qconj */

    int CQRConjugate (CQRQuaternionHandle qconjgate, CQRQuaternionHandle quaternion);
    
    /*  CQRNormsq -- Form the normsquared of a quaternion */
    
    int CQRNormsq (double CQR_FAR * normsq, CQRQuaternionHandle quaternion ) ;
    
    /*  CQRNorm -- Form the norm of a quaternion */
    
    int CQRNorm (double CQR_FAR * norm, CQRQuaternionHandle quaternion ) ;
    
    /*  CQRDistsq -- Form the distance squared between two quaternions */
    
    int CQRDistsq (double CQR_FAR * distsq, CQRQuaternionHandle q1, CQRQuaternionHandle q2) ;
    
    /*  CQRDist -- Form the distance between two quaternions */
    
    int CQRDist (double CQR_FAR * dist, CQRQuaternionHandle q1, CQRQuaternionHandle q2 ) ;
    
    /*  CQRInverse -- Form the inverse of a quaternion */
    
    int CQRInverse (CQRQuaternionHandle inversequaternion, CQRQuaternionHandle quaternion );
    
    /* CQRRotateByQuaternion -- Rotate a vector by a Quaternion, w = qvq* */
    
    int CQRRotateByQuaternion(double CQR_FAR * w, CQRQuaternionHandle rotquaternion, double CQR_FAR * v);        
    
    /* CQRAxis2Quaternion -- Form the quaternion for a rotation around axis v  by angle theta */
    
    int CQRAxis2Quaternion (CQRQuaternionHandle rotquaternion, double CQR_FAR * v, double theta);
    
    /* CQRMatrix2Quaterion -- Form the quaternion from a 3x3 rotation matrix R */
    
    int CQRMatrix2Quaternion (CQRQuaternionHandle rotquaternion, double R[3][3]);
    
    /* CQRQuaternion2Matrix -- Form the 3x3 rotation matrix from a quaternion */
    
    int CQRQuaternion2Matrix (double R[3][3], CQRQuaternionHandle rotquaternion);
    
    /* CQRQuaternion2Angles -- Convert a Quaternion into Euler Angles for Rz(Ry(Rx))) convention */
    
    int CQRQuaternion2Angles (double CQR_FAR * RotX, double CQR_FAR * RotY, double CQR_FAR * RotZ, CQRQuaternionHandle rotquaternion);
    
    /* CQRAngles2Quaternion -- Convert Euler Angles for Rz(Ry(Rx))) convention into a quaternion */
    
    int CQRAngles2Quaternion (CQRQuaternionHandle rotquaternion, double RotX, double RotY, double RotZ );
    
    /* Represent a 3-vector as a quaternion with w=0 */
    
    int CQRPoint2Quaternion( CQRQuaternionHandle quaternion, double v[3] );

    
    /*  SLERP -- Spherical Linear Interpolation   
     Take two quaternions and two weights and combine them
     following a great circle on the unit quaternion 4-D sphere
     and linear interpolation between the radii
     
     This version keeps a quaternion separate from the negative
     of the same quaternion and is not appropriate for
     quaternions representing rotations.  Use CQRHLERP
     to apply SLERP to quaternions representing rotations
     */
    
    int CQRSLERP (CQRQuaternionHandle quaternion, const CQRQuaternionHandle q1, const CQRQuaternionHandle q2,
                  const double w1, const double w2);
    
    /*  HLERP -- Hemispherical Linear Interpolation   
     Take two quaternions and two weights and combine them
     following a great circle on the unit quaternion 4-D sphere
     and linear interpolation between the radii
     
     This is the hemispherical version, for use with quaternions
     representing rotations.  Use SLERP for full
     spherical interpolation.
     
     */
    
    int CQRHLERP (CQRQuaternionHandle quaternion, const CQRQuaternionHandle q1, const CQRQuaternionHandle q2,
                  const double w1, const double w2);
    
    /*  SLERPDist -- Spherical Linear Interpolation distance
     Form the distance between two quaternions by summing
     the difference in the magnitude of the radii and
     the great circle distance along the sphere of the
     smaller quaternion.
     
     This version keeps a quaternion separate from the negative
     of the same quaternion and is not appropriate for
     quaternions representing rotations.  Use CQRHLERPDist
     to apply SLERPDist to quaternions representing rotations
     */
    
    int CQRSLERPDist (double CQR_FAR * dist, const CQRQuaternionHandle q1, const CQRQuaternionHandle q2);
    
    /*  HLERPDist -- Hemispherical Linear Interpolation distance
     Form the distance between two quaternions by summing
     the difference in the magnitude of the radii and
     the great circle distance along the sphere of the
     smaller quaternion.
     
     This version keeps a quaternion separate from the negative
     of the same quaternion and is not appropriate for
     quaternions representing rotations.  Use CQRHLERPDist
     to apply SLERPDist to quaternions representing rotations
     */
    
    int CQRHLERPDist (double CQR_FAR * dist, const CQRQuaternionHandle q1, const CQRQuaternionHandle q2);
    
#ifdef __cplusplus
    
}

#endif
#endif

#endif
