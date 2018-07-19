#ifndef VECTOR_3D_H_INCLUDED
#define VECTOR_3D_H_INCLUDED

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <climits>
#include <list>
#include <iostream>
#include "cqrlib.h"
#include "TNear.h"
#include "triple.h"
#include <utility>
#include <vector>

#ifdef VECTOR_3D_DEBUG
#include <stdio.h>
#define VECTOR_3D_debugstatement(s) { (s); }
#else
#define VECTOR_3D_debugstatement(s)
#endif


// forward declarations
class Plane_3;
class Line_3;
class Vector_3;
class Matrix_3x3;
template<typename DistanceType> class QR;

// these are friend functions, put here because LINT complains if they are at the end
//#ifdef __cplusplus
//    std::ostream& operator<<     ( std::ostream&, const Vector_3& );
//    std::ostream& operator<<     ( std::ostream&, const Matrix_3x3& );
//#endif
Vector_3 operator* ( const double&, const Vector_3& );
Matrix_3x3 RandomOrientation( int& iseed );
double randf( int& iseed );
Vector_3 randVector( int& iseed );
double Angle ( const Vector_3& a,  const Vector_3& b );
QR<double> randQuaternion( int& iseed );


/* The MatchObject Class carries two atom indices (a1,a2) and the distance between then din */
class MatchObject
    {
    public:
        MatchObject( const double din, const size_t i, const size_t j ) : d(din), a1(i), a2(j)
        {
        };
        double  d;
        size_t a1;
        size_t a2;
        MatchObject& operator= ( const MatchObject& mo )
        {
            d  = mo.d;
            a1 = mo.a1;
            a2 = mo.a2;
            return ( *this );
        }
        bool operator< ( const MatchObject& mo ) const
        {
            return ( d < mo.d );
        }
    };  // end of local class MatchObject

/* The MatchObject_II Class carries three atom indices as a triangle and the
   distances between them.  The atoms are a1, a2 and a3.  The distance of
   the side opposite a1 is d1, the distance of the side opposite a2 is
   d2 and the distance of the side opposite a3 is d3 */
class MatchObject_II
    {
    public:
        MatchObject_II( const double din1, const double din2, const double din3, 
                       const size_t i, const size_t j, const size_t k ) : 
        d1(din1), d2(din2), d3(din3), a1(i), a2(j), a3(k)
        {
        };
        double  d1, d2, d3;
        size_t a1;
        size_t a2;
        size_t a3;
        MatchObject_II& operator= ( const MatchObject_II& mo )
        {
            d1 = mo.d1;
            d2 = mo.d2;
            d3 = mo.d3;
            a1 = mo.a1;
            a2 = mo.a2;
            a3 = mo.a3;
            return ( *this );
        }
        bool operator< ( const MatchObject_II& mo ) const
        {
            return ( d1 < mo.d1 || (d1 == mo.d1 && d2 < mo.d2) || (d1 == mo.d1 && d2 == mo.d2 && d3 < mo.d3)   );
        }
    };  // end of local class MatchObject_II


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
class Vector_3
{
    friend class Matrix_3x3;
    friend class QR<double>;

#ifdef __cplusplus
    friend std::ostream& operator<< ( std::ostream&, const Vector_3& );
#endif
    friend Vector_3 operator* ( const double&, const Vector_3& );
private:
    double v[3];
public:
    //CONSTRUCTORS, DESTRUCTOR
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
// Name: Vector_3()
// Description: default constructor provides bad-looking data
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Vector_3( void )
//-------------------------------------------------------------------------------------
{
    v[0]=v[1]=v[2]=DBL_MAX;
}

//-----------------------------------------------------------------------------
// Name: Vector_3()
// Description: constructor to insert 3 values into a new Vector_3 vector
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline Vector_3( const double a, const double b, const double c )
//-------------------------------------------------------------------------------------
{
    v[0] = a;
    v[1] = b;
    v[2] = c;
}

//-----------------------------------------------------------------------------
// Name: Vector_3()
// Description: constructor to insert 3 identical values into a new Vector_3 vector
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
explicit Vector_3( const double a )
//-------------------------------------------------------------------------------------
{
    v[0] = a;
    v[1] = a;
    v[2] = a;
}

//-----------------------------------------------------------------------------
// Name: Vector_3()
// Description: copy constructor
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Vector_3 ( const Vector_3& o )
//-------------------------------------------------------------------------------------
{
    v[0] = o.v[0];
    v[1] = o.v[1];
    v[2] = o.v[2];
}


//-----------------------------------------------------------------------------
// Name:~ Vector_3()
// Description: destructor
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
~Vector_3( void )
//-------------------------------------------------------------------------------------
{
    v[0]=v[1]=v[2]=DBL_MAX;
}
    //SCALAR OPERATIONS ON VECTORS
//-----------------------------------------------------------------------------
// Name: operator*()
// Description: multiply a vector by a floating point number
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline Vector_3 operator*  ( const double a ) const
//-----------------------------------------------------------------------------
    {
        return ( Vector_3(a*v[0],a*v[1],a*v[2]) );
    }

//-----------------------------------------------------------------------------
// Name: operator/()
// Description: divide a vector by a floating point number
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline Vector_3 operator/  ( const double a ) const
//-----------------------------------------------------------------------------
    {
        return ( Vector_3(v[0]/a,v[1]/a,v[2]/a) );
    }

//-----------------------------------------------------------------------------
// Name: operator+=()
// Description: add two vectors
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline Vector_3& operator+= ( const Vector_3& vv )
//-----------------------------------------------------------------------------
    {
        *this = *this + vv;
        return( *this );
    }

//-----------------------------------------------------------------------------
// Name: operator-=()
// Description: subtract two vectors
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline Vector_3& operator-= ( const Vector_3& vv )
//-----------------------------------------------------------------------------
    {
        (*this) = (*this) - vv;
        return( *this );
    }

    static double        Angle         ( const Vector_3& a,  const Vector_3& b,  const Vector_3& c );
    static double        Torsion        ( const Vector_3& a,  const Vector_3& b,  const Vector_3& c, const Vector_3& d );

    

    //OPERATIONS ONLY ON VECTORS

//-----------------------------------------------------------------------------
// Name: Dot()
// Description: compute the dot product of two vectors = |v1|*|v2|*cos(angle between)
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline double Dot ( const Vector_3& v_other ) const
//-----------------------------------------------------------------------------
    {
        return(
            v[0] * v_other[0]+
            v[1] * v_other[1]+
            v[2] * v_other[2]    );
    }

//-----------------------------------------------------------------------------
// Name: Cross()
// Description: compute the cross product of two vectors. The result is a vector
// perpendicular to the two and with Norm=|v1|*|v2|*sin(angle between)
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline Vector_3 Cross ( const Vector_3& v_other ) const
//-----------------------------------------------------------------------------
    {

        return (Vector_3(    v[1]*v_other[2]-v_other[1]*v[2],
                            -v[0]*v_other[2]+v_other[0]*v[2],
                             v[0]*v_other[1]-v_other[0]*v[1]  ) );
    }

//-----------------------------------------------------------------------------
// Name: DyadProduct()
// Description: compute the dyad product of two vectors
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    Vector_3 DyadProduct ( const Vector_3& v_other ) const
//-----------------------------------------------------------------------------
    {
        return ( Vector_3(v[0]*v_other[0],v[1]*v_other[1],v[2]*v_other[2]) );
    }

    Matrix_3x3 MatrixProduct ( const Vector_3& v_other ) const;

//-----------------------------------------------------------------------------
// Name: operator+()
// Description: add two vectors
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline Vector_3 operator+ ( const Vector_3& v_other ) const
//-----------------------------------------------------------------------------
    {
        return ( Vector_3( v[0]+v_other[0],v[1]+v_other[1],v[2]+v_other[2] ) );
    }

//-----------------------------------------------------------------------------
// Name: operator-()
// Description: subtract two vectors
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline Vector_3 operator- ( const Vector_3& v_other ) const
//-----------------------------------------------------------------------------
    {
        return ( Vector_3(v[0]-v_other[0],v[1]-v_other[1],v[2]-v_other[2]) );
    }

    //-----------------------------------------------------------------------------
    // Name: operator-()
    // Description: Unary minus for a vector
    //
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    Vector_3 operator- ( void ) const
    //-------------------------------------------------------------------------------------
    {
        return( Vector_3( -v[0], -v[1], -v[2] ) );
    }

    inline double Norm ( void ) const
    {
        return( sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] ) );
    }


//-----------------------------------------------------------------------------
// Name: UnitV()
// Description: returns a vector that is the same as the input vector, but scaled to
// a unit vector
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Vector_3 UnitV( void ) const
//-------------------------------------------------------------------------------------
{
            //test against something considerably larger than DBL_MIN
            double mynorm = Norm();
            if ( mynorm < 10.0*DBL_MIN )
            {
                return (Vector_3(0.,0.,0.) );
    }
    else
    {
        return (Vector_3(*this)/Norm( ));  // const, so work on a copy
    }
}

    inline Vector_3& operator=  ( const Vector_3& v_other )
    {
        if (this != &v_other)
        {
            v[0] = v_other.v[0];
            v[1] = v_other.v[1];
            v[2] = v_other.v[2];
        }
        return (*this);
    }

    inline double SquaredLength ( void ) const
    {
        return( v[0]*v[0]+v[1]*v[1]+v[2]*v[2] );
    }

    static Matrix_3x3   Pair            ( const Vector_3& x1, const Vector_3& x2, const Vector_3& y1, const Vector_3& y2 );
    inline static Vector_3     GetXAxis ( void ) { return( Vector_3( 1.0, 0.0, 0.0 ) ); };
    inline static Vector_3     GetYAxis ( void ) { return( Vector_3( 0.0, 1.0, 0.0 ) ); };
    inline static Vector_3     GetZAxis ( void ) { return( Vector_3( 0.0, 0.0, 1.0 ) ); };
    inline static Vector_3     GetZeroVector ( void ) { return( Vector_3( 0.0, 0.0, 0.0 ) ); };

    bool operator== ( const Vector_3& v_other ) const { return( v[0]==v_other.v[0] && v[1]==v_other.v[1] && v[2]==v_other.v[2] ); };
    bool operator!= ( const Vector_3& v_other ) const { return( v[0]!=v_other.v[0] || v[1]!=v_other.v[1] || v[2]!=v_other.v[2] ); };
    static QR<double> QuaternionFromPair( const Vector_3& v1, const Vector_3& v2 );


//-----------------------------------------------------------------------------
// Name: GetCenterOfMass()
// Description: for an input list of points, compute the center of mass
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
template< typename ContainerType > 
static Vector_3 GetCenterOfMass( const ContainerType& lv )
//-------------------------------------------------------------------------------------
{
    Vector_3 cm(0.0);
    if( lv.empty( ) )
    {
        return( cm );
    }
    typename ContainerType::const_iterator it;
    for( it=lv.begin( ); it!=lv.end( ); ++it )
    {
        cm += (*it);
    }
    return( cm / static_cast<double>(lv.size( ) ) );
}
    //OPERATIONS USING MATRICES
    Vector_3 MV( const Matrix_3x3& m) const;
    static std::pair<Matrix_3x3, Vector_3> GeneralRotation( const double angle, const Vector_3& v1, const Vector_3& v2 );
    static std::pair<Matrix_3x3, Vector_3> GeneralRotation( const double angle, const Line_3& L );

    //complex operations
    Matrix_3x3 Rotmat ( const double angle ) const;
    Matrix_3x3 InertiaTensorForPoint ( const double weight ) const;
    double DistanceFromPlane ( const Plane_3& p ) const;
    double DistanceFromLine ( const Line_3& v ) const;

    //access and io functions
    //-----------------------------------------------------------------------------
    // Name: operator[]()
    // Description: access function for the components of a vector
    //
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline double operator[] ( const int& i ) const
    //-------------------------------------------------------------------------------------
    {
        int n = ( i<0 ) ? 0 : i;
        if( i > 2 ) n = 2;
        return (v[n]);
    }

    inline double& operator[] ( const int& i )
    //-------------------------------------------------------------------------------------
    {
        int n = ( i<0 ) ? 0 : i;
        if( i > 2 ) n = 2;
        return (v[n]);
    }

    //-----------------------------------------------------------------------------
    // Name: at()
    // Description: access function for the components of a vector
    //
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline double at ( const int& i ) const
    //-------------------------------------------------------------------------------------
    {
        int n = ( i<0 ) ? 0 : i;
        if( i > 2 ) n = 2;
        return (v[n]);
    }

    // constants
    static const double    MINNORM;
    
    static QR<double> HornQRPlane2Plane( const Vector_3& v1, const Vector_3& v2,const Vector_3& v3,
                                           const Vector_3& t1, const Vector_3& t2, const Vector_3& t3);
}; // end of class vector

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
class Matrix_3x3
{
    friend class Vector_3;
    friend class QR<double>;

#ifdef __cplusplus
    friend std::ostream& operator<< ( std::ostream&, const Matrix_3x3& );
#endif
    friend Matrix_3x3 operator* ( const double&, const Matrix_3x3& );
    double m[9];
public:
    //CONSTRUCTORS, DESTRUCTOR

//-----------------------------------------------------------------------------
// Name: Matrix_3x3()
// Description: constructor
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3( void )
//-------------------------------------------------------------------------------------
{
    for ( int i=0; i<9; i++) m[i] = DBL_MAX;
}

//-----------------------------------------------------------------------------
// Name: Matrix_3x3()
// Description: constructor to fill a matrix with a list of values
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3( const double a, const double b, const double c,
                        const double d, const double e, const double f,
                       const double g, const double h, const double i )
//-------------------------------------------------------------------------------------
{
    m[0] = a;
    m[1] = b;
    m[2] = c;
    m[3] = d;
    m[4] = e;
    m[5] = f;
    m[6] = g;
    m[7] = h;
    m[8] = i;
}

//-----------------------------------------------------------------------------
// Name: Matrix_3x3()
// Description: copy constructor
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3( const Matrix_3x3& o )
//-------------------------------------------------------------------------------------
{
    for ( int i=0; i<9; ++i) m[i] = o.m[i];
}

//-----------------------------------------------------------------------------
// Name: ~Matrix_3x3()
// Description: destructor
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
~Matrix_3x3( void )
//-------------------------------------------------------------------------------------
{
    for ( int i=0;i<9;i++) m[i] = DBL_MAX;
}

double trace( void ) const {
   return m[0]+m[4]+m[8];
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3 adjoint( void ) const {
   return Matrix_3x3(
   m[4]*m[8] - m[7]*m[5],     //m2[0] =
   m[3]*m[8] - m[6]*m[5],     //m2[1] =
   m[3]*m[7] - m[6]*m[4],     //m2[2] =
   m[1]*m[8] - m[7]*m[2],     //m2[3] =
   m[0]*m[8] - m[6]*m[2],     //m2[4] =
   m[0]*m[7] - m[6]*m[1],     //m2[5] =
   m[1]*m[5] - m[4]*m[2],     //m2[6] =
   m[0]*m[5] - m[3]*m[2],     //m2[7] =
   m[0]*m[4] - m[3]*m[1]);    //m2[8] =
}

    //matrix-vector operations
//-----------------------------------------------------------------------------
// Name: MV()
// Description: multiply a vector (from the right) by a matrix
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline Vector_3 MV ( const Vector_3& v ) const
//-------------------------------------------------------------------------------------
{
    return (Vector_3( m[0]*v[0]+m[1]*v[1]+m[2]*v[2],
                      m[3]*v[0]+m[4]*v[1]+m[5]*v[2],
                      m[6]*v[0]+m[7]*v[1]+m[8]*v[2]));
}
    
    Vector_3 operator* ( const Vector_3& v ) const
    {
        return (Vector_3(m[0]*v[0]+m[1]*v[1]+m[2]*v[2],
        m[3]*v[0]+m[4]*v[1]+m[5]*v[2],
        m[6]*v[0]+m[7]*v[1]+m[8]*v[2]));
    }

    //operations on matrices only
    Matrix_3x3 operator*  ( const Matrix_3x3& mm ) const;

//-----------------------------------------------------------------------------
// Name: operator+()
// Description: add two matrices
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline Matrix_3x3 operator+ ( const Matrix_3x3& o ) const
//-------------------------------------------------------------------------------------
{
    return (Matrix_3x3(
        m[0]+o.m[0],
        m[1]+o.m[1],
        m[2]+o.m[2],
        m[3]+o.m[3],
        m[4]+o.m[4],
        m[5]+o.m[5],
        m[6]+o.m[6],
        m[7]+o.m[7],
        m[8]+o.m[8]  ));
}


//-----------------------------------------------------------------------------
// Name: operator-()
// Description: subtract two matrices
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline Matrix_3x3 operator- ( const Matrix_3x3& o ) const
//-------------------------------------------------------------------------------------
{
    return (Matrix_3x3(m[0]-o.m[0],m[1]-o.m[1],m[2]-o.m[2],
        m[3]-o.m[3],m[4]-o.m[4],m[5]-o.m[5],
        m[6]-o.m[6],m[7]-o.m[7],m[8]-o.m[8]));
}

//-----------------------------------------------------------------------------
// Name: operator*()
// Description: multiplies a matrix by a floating point number
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline Matrix_3x3 operator* ( const double d ) const
//-------------------------------------------------------------------------------------
{
    Matrix_3x3 mTemp(*this);
    for( int i=0; i<9; ++i )
    {
        mTemp.m[i] *= d;
    }
    return( mTemp );
}

//-----------------------------------------------------------------------------
// Name: operator/()
// Description: divides a matrix by a floating point number
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3 operator/ ( const double d ) const
//-------------------------------------------------------------------------------------
{
    Matrix_3x3 mTemp(*this);
    for( int i=0; i<9; ++i )
    {
        mTemp.m[i] /= d;
    }
    return( mTemp );
}

//-----------------------------------------------------------------------------
// Name: operator+=()
// Description: add two matrices
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3& operator+= ( const Matrix_3x3& mm )
//-------------------------------------------------------------------------------------
{
    (*this) = (*this) + mm;
    return( *this );
}

//-----------------------------------------------------------------------------
// Name: operator-=()
// Description: subtracts two matrices
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3& operator-= ( const Matrix_3x3& mm )
//-------------------------------------------------------------------------------------
{
    (*this) = (*this) - mm;
    return( *this );
}

//-----------------------------------------------------------------------------
// Name: operator*=()
// Description: multiplies two matrices
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3& operator*= ( const Matrix_3x3& mm )
//-------------------------------------------------------------------------------------
{
    (*this) = (*this) * mm;
    return( *this );
}

//-----------------------------------------------------------------------------
// Name: operator=()
// Description: copy a matrix
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline Matrix_3x3& operator= ( const Matrix_3x3& o )
//-------------------------------------------------------------------------------------
{
    if (this!=&o)
    {
        int i;
        for (i=0;i<9;i++) m[i] = o.m[i];
    }
    return (*this);
}


//-----------------------------------------------------------------------------
// Name: Transpose()
// Description: returns the transpose of the input matrix
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3 Transpose ( void ) const
//-------------------------------------------------------------------------------------
{
    return (Matrix_3x3(m[0],m[3],m[6],
        m[1],m[4],m[7],
        m[2],m[5],m[8]));
}


//-----------------------------------------------------------------------------
// Name: Inver()
// Description: returns the inverse of a matrix
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3 Inver ( void ) const
//-------------------------------------------------------------------------------------
{
    double d(this->Det( ));
    if( d == 0.0 ) d = 1.0;
    if( ::fabs( d ) < 1.0E-20 )
    {
        d = ( d < 0.0 ) ? -d : d;
    }
    const Vector_3 v0( this->m[0], this->m[1], this->m[2] );
    const Vector_3 v1( this->m[3], this->m[4], this->m[5] );
    const Vector_3 v2( this->m[6], this->m[7], this->m[8] );

    const Vector_3 vOut0 = v1.Cross( v2 );
    const Vector_3 vOut1 = v2.Cross( v0 );
    const Vector_3 vOut2 = v0.Cross( v1 );

    const Matrix_3x3 mTemp(
        vOut0.v[0], vOut1.v[0], vOut2.v[0],
        vOut0.v[1], vOut1.v[1], vOut2.v[1],
        vOut0.v[2], vOut1.v[2], vOut2.v[2] );  // computes the ? of a matrix
    return( mTemp/ d );
}

static
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Name: Cart
// Description: Cart is used to generate a matrix that will convert from coordinates
// in a non-orthogonal coordinate system to an orthogonal basis. This is the solution 
// for the common problem in crystallography, where coordinates are often reported in
// coordinates that are measured using the unit cell dimensions. Cart will return
// a matrix that when multiplied by a vector expressed in fractional coordinates will 
// generate the corresponding position in an orthonormal system. NOTE: VERY IMPORTANT:
// The coordinates created by that operation may NOT be comparable to those generated
// by some other conversion software. Each system assumes a particular relationship 
// between the two system, and they are ALL correct. Basically, the conversion matrix
// can be multiplied by ANY rotation matrix at all, and the result will still generate
// correct orthogonal coordinates, just different ones. The inverse of the matrix that
// Cart generates will convert from orthogonal coordinates back to fractional ONLY IF
// the orginal transformation was made with the same convention that Cart uses.

// The convention used in Cart is that the a-crystallographic axis will be aligned
// parallel to x, the b-axis as close as possible to y, and the c-axis by construction
// of a right handed-coordinate system.
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3 Cart(const double a, const double b, const double c,
   const double alpha, const double beta, const double gamma)
   //-------------------------------------------------------------------------------------
{
   const double degreesPerRad = 180.0 / (4.0 * atan(1.0));

   //const double sinAlpha = sin(alpha / degreesPerRad);
   //const double sinBeta  = sin(beta  / degreesPerRad);
   const double sinGamma = sin(gamma / degreesPerRad);
   const double cosAlpha = cos(alpha / degreesPerRad);
   const double cosBeta = cos(beta / degreesPerRad);
   const double cosGamma = cos(gamma / degreesPerRad);

   const double V = a*b*c*sqrt(1.0
      - cosAlpha*cosAlpha - cosBeta*cosBeta - cosGamma*cosGamma +
      2.0*cosAlpha*cosBeta*cosGamma);

   const Matrix_3x3 amat(
      a, b*cosGamma, c*cosBeta,
      0.0, b*sinGamma, c*(cosAlpha - cosBeta*cosGamma) / sinGamma,
      0.0, 0.0, V / (a*b*sinGamma)
   );
   return(amat);
}


//
//static Matrix_3x3 Cart(const double a, const double b, const double c,
//   const double alpha, const double beta, const double gamma) {
//   throw "not implemented--Cart";
//   return Matrix_3x3();
//}

    //-----------------------------------------------------------------------------
// Name: UnitMatrix()
// Description: return a unit matrix
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void UnitMatrix ( void )
//-------------------------------------------------------------------------------------
{
    *this =  Matrix_3x3(
        1.0,0.0,0.0,
        0.0,1.0,0.0,
        0.0,0.0,1.0    );
}

//-----------------------------------------------------------------------------
// Name: Det()
// Description: returns the determinant of a matrix
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double Det( void ) const
//-------------------------------------------------------------------------------------
{
    const double d = m[0]*m[4]*m[8] + m[1]*m[5]*m[6] + m[2]*m[3]*m[7]
    -m[6]*m[4]*m[2] - m[7]*m[5]*m[0] - m[8]*m[3]*m[1];
    return( d );
}

//-----------------------------------------------------------------------------
// Name: Zero()
// Description: insert all zero values into a 3x3 matrix
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void Zero( void )
//-------------------------------------------------------------------------------------
{
    for( int i=0; i<9; ++i )
    {
        this->m[i] = 0.0;
    }
}

    bool Eigen1 ( double& eigenvalue, Vector_3& eigenvector1 ) const;
    bool Eigen3 ( std::vector<double>& eigenvalues, std::vector<Vector_3>& eigenvectors ) const;

//-----------------------------------------------------------------------------
// Name: operator[]()
// Description: access function for the components of a matrix
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double operator[]( const int& i ) const
//-------------------------------------------------------------------------------------
{
    int n = ( i<0 ) ? 0 : i;
    if( i > 8 ) n = 8;
    return (m[n]);
}

double& operator[]( const int& i )
//-------------------------------------------------------------------------------------
{
    int n = ( i<0 ) ? 0 : i;
    if( i > 8 ) n = 8;
    return (m[n]);
}

    Matrix_3x3 UNX( void ) const;
    Matrix_3x3 UNY( void ) const;
    Matrix_3x3 UNZ( void ) const;

}; // end of class Matrix_3x3


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Line_3
//     This is a small class to represent a line. Minimal functionality in Vector_3 is
//     provided for working with lines and planes.
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
class Line_3
{
public:
    Line_3( void );
    Line_3( const Vector_3& direction, const Vector_3& CenterOfMass );
    Vector_3 LineAxis( void ) const;
    Vector_3 BasePoint( void ) const;
    virtual ~Line_3( void );
    double DistanceFromLine( const Vector_3& v ) const;
    std::pair<Matrix_3x3, Vector_3> GeneralRotation( const double angle );

private:
    Vector_3 m_lineAxis;     // the vector parallel to the line
    Vector_3 m_basePoint;    // the point on the line where a vector from the origin intersects at 90 degrees
    Vector_3 m_centerOfMass; // center of mass of the points entered
}; // end of class Line_3

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Plane_3
//     This is a small class to represent a plane. Minimal functionality in Vector_3 is
//     provided for working with lines and planes.
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
class Plane_3
{
public:
    Plane_3( void );
    Plane_3( const Vector_3& direction, const Vector_3& CenterOfMass);
    Vector_3 NormalVector( void ) const;
    Vector_3 BasePoint( void ) const;
    virtual ~Plane_3( void );
    double DistanceFromPlane( const Vector_3& v ) const;
    Line_3 Intersect( const Plane_3& p ) const;

private:
    Vector_3 m_planeNormal;  // the vector normal to the plane
    Vector_3 m_basePoint;    // the point on the plane where a vector from the origin intersects at 90 degrees
    Vector_3 m_centerOfMass; // center of mass of the points entered
}; // end of class Plane_3


template< typename DistanceType=double >
class QR: public CPPQR<DistanceType,Vector_3,Matrix_3x3>
{

    friend class Vector_3;
    friend class Matrix_3x3;

/* private:
    DistanceType w, x, y, z; */

public:
    inline QR() { (*this).Set(DistanceType( 0.0 ), DistanceType( 0.0 ),DistanceType( 0.0 ),DistanceType( 0.0 )); }
    inline QR( const QR& q) {(*this).Set(q.GetW(),q.GetX(),q.GetY(),q.GetZ());}
    inline QR( const CPPQR<double,Vector_3,Matrix_3x3>& q) {(*this).Set(q.GetW(),q.GetX(),q.GetY(),q.GetZ());}
    inline QR( const DistanceType& wi, const DistanceType& xi, const DistanceType& yi, const DistanceType& zi) { (*this).Set(wi, xi,yi,zi); }
    inline QR( const Vector_3& v ) { (*this).Set((DistanceType)0.,(DistanceType)(v.v[0]), (DistanceType)(v.v[1]), (DistanceType)(v.v[2]));}

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline QR& operator= ( const QR& q ) 
    {
        if ( this != &q )
        { (*this).Set(q.GetW(),q.GetX(),q.GetY(),q.GetZ());}
        return( *this );
    }
    inline QR& operator= ( const CPPQR<DistanceType,Vector_3,Matrix_3x3>& q ) 
    {
        if ( this != &q )
    { (*this).Set(q.GetW(),q.GetX(),q.GetY(),q.GetZ());}
        return( *this );
    }

    
    /* RotateByQuaternion -- Rotate a vector by a Quaternion, w = qvq* */
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline Vector_3 RotateByQuaternion( const Vector_3& v )
    {
        Vector_3 w;
        (*this).RotateByQuaternion(w,v);
        return w;
    }


    /* Matrix2Quaterion -- Form the quaternion from a 3x3 rotation matrix R */
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    static inline QR Matrix2Quaternion ( const Matrix_3x3& m )
    {
        QR rotquaternion;

        CPPQR<DistanceType,Vector_3,Matrix_3x3>::Matrix2Quaternion(rotquaternion,m);
        return rotquaternion;
        }


    /* Quaternion2Matrix -- Form the 3x3 rotation matrix from a quaternion */    
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline Matrix_3x3 Quaternion2Matrix( void ) const
    {
        Matrix_3x3 m;
        CPPQR<DistanceType,Vector_3,Matrix_3x3>::Quaternion2Matrix(m,(*this));
        return m;
     } // end Quaternion2Matrix

     /* ImVector -- Recover vector from imaginary part of quaternion */
     /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
     inline Vector_3 ImVector( void ) const
     {
         Vector_3 v = Vector_3((*this).GetX(),(*this).GetY(),(*this).GetZ());
         return v;
     } // end ImVector

}; // end class QR

template< typename DistanceType=double>
class SQR: public QR<DistanceType>
{
public:
inline SQR() { (*this).Set(DistanceType( 0.0 ), DistanceType( 0.0 ),DistanceType( 0.0 ),DistanceType( 0.0 )); }
inline SQR( const SQR<DistanceType>& q) {(*this).Set(q.GetW(),q.GetX(),q.GetY(),q.GetZ());}
inline SQR( const QR<DistanceType>& q) {
    DistanceType qnorm = q.Norm();
    if ( double(qnorm) < DBL_EPSILON ) {
        (*this).Set( 1., 0., 0., 0.);
    } else {
        (*this).Set(q.GetW()/qnorm,q.GetX()/qnorm,q.GetY()/qnorm,q.GetZ()/qnorm);
    }
}
inline SQR( const CPPQR<double,Vector_3,Matrix_3x3>& q) {
    DistanceType qnorm = q.Norm();
    if ( double(qnorm) < DBL_EPSILON ) {   
        (*this).Set( 1., 0., 0., 0.);
    } else {
        (*this).Set(q.GetW()/qnorm,q.GetX()/qnorm,q.GetY()/qnorm,q.GetZ()/qnorm);
    }
}
inline SQR( const DistanceType& wi, const DistanceType& xi, const DistanceType& yi, const DistanceType& zi) {
    DistanceType qnorm = (QR<DistanceType>(wi,xi,yi,zi)).Norm();
    if ( double(qnorm) < DBL_EPSILON ) {
        (*this).Set( 1., 0., 0., 0.);
    } else {
        (*this).Set(wi/qnorm,xi/qnorm,yi/qnorm,zi/qnorm);
    }
 }


// Addition is multiplication on the sphere

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline SQR operator+ ( const SQR& q ) const
{
    QR<DistanceType> asquat = QR<DistanceType>(*this);
    QR<DistanceType> qasquat = QR<DistanceType>(q);
    QR<DistanceType> qresult = asquat*qasquat;
    DistanceType qnorm = qresult.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    return (SQR(qresult/qnorm));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline SQR operator+= ( const SQR& q )
{
    QR<DistanceType> asquat = QR<DistanceType>(*this);
    QR<DistanceType> qasquat = QR<DistanceType>(q);
    QR<DistanceType> qresult = asquat*qasquat;
    DistanceType qnorm = qresult.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    *this = SQR(qresult/qnorm);
    return *this;
}

// Subtraction is division on the sphere

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline SQR operator- ( const SQR& q ) const
{
    QR<DistanceType> asquat = QR<DistanceType>(*this);
    QR<DistanceType> qasquat = QR<DistanceType>(q);
    QR<DistanceType> qresult = asquat/qasquat;
    DistanceType qnorm = qresult.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    return (SQR(qresult/qnorm));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline SQR operator-= ( const SQR& q )
{
    QR<DistanceType> asquat = QR<DistanceType>(*this);
    QR<DistanceType> qasquat = QR<DistanceType>(q);
    QR<DistanceType> qresult = asquat/qasquat;
    DistanceType qnorm = qresult.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    *this = SQR(qresult/qnorm);
    return *this;
}

// Multiplication is taking a power on the sphere

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline SQR operator* ( const DistanceType& p ) const
{
    QR<DistanceType> asquat = (QR<DistanceType>(*this)).pow((double)p);
    DistanceType qnorm = asquat.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    return (SQR(asquat/qnorm));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline SQR operator*= ( const DistanceType&  p )
{
    QR<DistanceType> asquat = (QR<DistanceType>(*this)).pow((double)p);
    DistanceType qnorm = asquat.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    *this = SQR(asquat/qnorm);
    return *this;
}


// Non-integer division is taking a power by the reciprocal on the sphere

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline SQR operator/ ( const DistanceType& p ) const
{
    QR<DistanceType> asquat = (QR<DistanceType>(*this)).pow(1./((double)p));
    DistanceType qnorm = asquat.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    return (SQR(asquat/qnorm));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline SQR operator/= ( const DistanceType&  p )
{
    QR<DistanceType> asquat = (QR<DistanceType>(*this)).pow(1./((double)p));
    DistanceType qnorm = asquat.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    *this = HQR(asquat/qnorm);
    return *this;
}
    
// Integer division is taking a root on the sphere

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline SQR operator/ ( const int& r ) const
{
    QR<DistanceType> asquat = (QR<DistanceType>(*this)).root(r,0);
    DistanceType qnorm = asquat.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    return (SQR(asquat/qnorm));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline SQR operator/= ( const int& r )
{
    QR<DistanceType> asquat = (QR<DistanceType>(*this)).root(r,0);
    DistanceType qnorm = asquat.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    *this = SQR(asquat/qnorm);
    return *this;
}

inline DistanceType Norm( void ) const {
    QR<DistanceType> asquat = QR<DistanceType>(*this);
    double normquat = asquat.Norm();
    if (normquat == 0.) return 0.;
    return ((CPPQR<double,Vector_3,Matrix_3x3>(*this))/normquat)
    .SLERPDist(CPPQR<double,Vector_3,Matrix_3x3>(1.,0.,0.,0.));
}

inline QR<DistanceType> MakeQR( void ) const {
     return(QR<DistanceType>((*this).GetW(),(*this).GetX(),(*this).GetY(),(*this).GetZ()));
}

inline CPPQR<DistanceType> MakeCPPQR( void ) const {
     return(CPPQR<DistanceType>((*this).GetW(),(*this).GetX(),(*this).GetY(),(*this).GetZ()));
}

}; // end of class SQR
template< typename DistanceType=double>
class HQR: public QR<DistanceType>
{
public:
inline HQR() { (*this).Set(DistanceType( 1.0 ), DistanceType( 0.0 ),DistanceType( 0.0 ),DistanceType( 0.0 )); }
inline HQR( const HQR<DistanceType>& q) {(*this).Set(q.GetW(),q.GetX(),q.GetY(),q.GetZ());}
inline HQR( const QR<DistanceType>& q) {
    DistanceType qnorm = q.Norm();
    if (double(qnorm) < DBL_EPSILON ) {
        (*this).Set( 1., 0., 0., 0.);
    } else if ( q.GetW() > 0. ) {
        (*this).Set( q.GetW()/qnorm, q.GetX()/qnorm, q.GetY()/qnorm, q.GetZ()/qnorm);
    } else if ( q.GetW() < 0. ) {
        (*this).Set(-q.GetW()/qnorm,-q.GetX()/qnorm,-q.GetY()/qnorm,-q.GetZ()/qnorm);
    } else if ( q.GetX() > 0. ) {
        (*this).Set(0. , q.GetX()/qnorm, q.GetY()/qnorm, q.GetZ()/qnorm);
    } else if ( q.GetX() < 0. ) {
        (*this).Set(0. ,-q.GetX()/qnorm,-q.GetY()/qnorm,-q.GetZ()/qnorm);
    } else if ( q.GetY() > 0. ) {
        (*this).Set(0. , 0. , q.GetY()/qnorm, q.GetZ()/qnorm);
    } else if ( q.GetY() < 0. ) {
        (*this).Set(0. , 0. ,-q.GetY()/qnorm,-q.GetZ()/qnorm);
    } else if ( q.GetZ() > 0. ) {
        (*this).Set(0. , 0. , 0. , q.GetZ()/qnorm);
    } else {
        (*this).Set(0. , 0. , 0. ,-q.GetZ()/qnorm);
    }
}
inline HQR( const CPPQR<double,Vector_3,Matrix_3x3>& q) {
    DistanceType qnorm = q.Norm();
    if (double(qnorm) < DBL_EPSILON ) {
        (*this).Set( 1., 0., 0., 0.);
    } else if ( q.GetW() > 0. ) {
        (*this).Set( q.GetW()/qnorm, q.GetX()/qnorm, q.GetY()/qnorm, q.GetZ()/qnorm);
    } else if ( q.GetW() < 0. ) {
        (*this).Set(-q.GetW()/qnorm,-q.GetX()/qnorm,-q.GetY()/qnorm,-q.GetZ()/qnorm);
    } else if ( q.GetX() > 0. ) {
        (*this).Set(0. , q.GetX()/qnorm, q.GetY()/qnorm, q.GetZ()/qnorm);
    } else if ( q.GetX() < 0. ) {
        (*this).Set(0. ,-q.GetX()/qnorm,-q.GetY()/qnorm,-q.GetZ()/qnorm);
    } else if ( q.GetY() > 0. ) {
        (*this).Set(0. , 0. , q.GetY()/qnorm, q.GetZ()/qnorm);
    } else if ( q.GetY() < 0. ) {
        (*this).Set(0. , 0. ,-q.GetY()/qnorm,-q.GetZ()/qnorm);
    } else if ( q.GetZ() > 0. ) {
        (*this).Set(0. , 0. , 0. , q.GetZ()/qnorm);
    } else {
        (*this).Set(0. , 0. , 0. ,-q.GetZ()/qnorm);
    }
}
inline HQR( const DistanceType& wi, const DistanceType& xi, const DistanceType& yi, const DistanceType& zi) {
    DistanceType qnorm = (QR<DistanceType>(wi,xi,yi,zi)).Norm();
    if (double(qnorm) < DBL_EPSILON ) {
        (*this).Set( 1., 0., 0., 0.);
    } else if ( wi > 0. ) {
        (*this).Set( wi/qnorm, xi/qnorm, yi/qnorm, zi/qnorm);
    } else if ( wi < 0. ) {
        (*this).Set(-wi/qnorm,-xi/qnorm,-yi/qnorm,-zi/qnorm);
    } else if ( xi > 0. ) {
        (*this).Set(0. , xi/qnorm, yi/qnorm, zi/qnorm);
    } else if ( xi < 0. ) {
        (*this).Set(0. ,-xi/qnorm,-yi/qnorm,-zi/qnorm);
    } else if ( yi > 0. ) {
        (*this).Set(0. , 0. , yi/qnorm, zi/qnorm);
    } else if ( yi < 0. ) {
        (*this).Set(0. , 0. ,-yi/qnorm,-zi/qnorm);
    } else if ( zi > 0. ) {
        (*this).Set(0. , 0. , 0. , zi/qnorm);
    } else {
        (*this).Set(0. , 0. , 0. ,-zi/qnorm);
    }
}

// Addition is multiplication on the sphere

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline HQR operator+ ( const HQR& q ) const
{
    QR<DistanceType> asquat = QR<DistanceType>(*this);
    QR<DistanceType> qasquat = QR<DistanceType>(q);
    QR<DistanceType> qresult = asquat*qasquat;
    DistanceType qnorm = qresult.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    return (HQR(qresult/qnorm));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline HQR operator+= ( const HQR& q )
{
    QR<DistanceType> asquat = QR<DistanceType>(*this);
    QR<DistanceType> qasquat = QR<DistanceType>(q);
    QR<DistanceType> qresult = asquat*qasquat;
    DistanceType qnorm = qresult.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    *this = HQR(qresult/qnorm);
    return *this;
}

// Subtraction is division on the sphere

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline HQR operator- ( const HQR& q ) const
{
    QR<DistanceType> asquat = QR<DistanceType>(*this);
    QR<DistanceType> qasquat = QR<DistanceType>(q);
    QR<DistanceType> qresult = asquat/qasquat;
    DistanceType qnorm = qresult.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    return (HQR(qresult/qnorm));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline HQR operator-= ( const HQR& q )
{
    QR<DistanceType> asquat = QR<DistanceType>(*this);
    QR<DistanceType> qasquat = QR<DistanceType>(q);
    QR<DistanceType> qresult = asquat/qasquat;
    DistanceType qnorm = qresult.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    *this = HQR(qresult/qnorm);
    return *this;
}

// Multiplication is taking a power on the sphere

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline HQR operator* ( const DistanceType& p ) const
{
    QR<DistanceType> asquat = (QR<DistanceType>(*this)).pow((double)p);
    DistanceType qnorm = asquat.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    return (HQR(asquat/qnorm));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline HQR operator*= ( const DistanceType&  p )
{
    QR<DistanceType> asquat = (QR<DistanceType>(*this)).pow((double)p);
    DistanceType qnorm = asquat.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    *this = HQR(asquat/qnorm);
    return *this;
}

// Non-integer division is taking a power by the reciprocal on the sphere

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline HQR operator/ ( const DistanceType& p ) const
{
    QR<DistanceType> asquat = (QR<DistanceType>(*this)).pow(1./((double)p));
    DistanceType qnorm = asquat.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    return (HQR(asquat/qnorm));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline HQR operator/= ( const DistanceType&  p )
{
    QR<DistanceType> asquat = (QR<DistanceType>(*this)).pow(1./((double)p));
    DistanceType qnorm = asquat.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    *this = HQR(asquat/qnorm);
    return *this;
}

// Integer division is taking a root on the sphere

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline HQR operator/ ( const int& r ) const
{
    QR<DistanceType> asquat = (QR<DistanceType>(*this)).root(r,0);
    DistanceType qnorm = asquat.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    return (HQR(asquat/qnorm));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
inline HQR operator/= ( const int& r )
{
    QR<DistanceType> asquat = (QR<DistanceType>(*this)).root(r,0);
    DistanceType qnorm = asquat.Norm();
    if (qnorm < DBL_EPSILON) qnorm = 1.;
    *this = HQR(asquat/qnorm);
    return *this;
}


inline DistanceType Norm( void ) const {
    QR<DistanceType> asquat = QR<DistanceType>(*this);
    double normquat = asquat.Norm();
    if (normquat == 0.) return 0.;
    return ((CPPQR<double,Vector_3,Matrix_3x3>(*this))/normquat)
    .HLERPDist(CPPQR<double,Vector_3,Matrix_3x3>(1.,0.,0.,0.));
}

inline QR<DistanceType> MakeQR( void ) const {
     return(QR<DistanceType>((*this).GetW(),(*this).GetX(),(*this).GetY(),(*this).GetZ()));
}

inline CPPQR<DistanceType> MakeCPPQR( void ) const {
     return(CPPQR<DistanceType>((*this).GetW(),(*this).GetX(),(*this).GetY(),(*this).GetZ()));
}

}; // end of class HQR

/*=========================================================================================*/
/* start of non-class functions */
/*=========================================================================================*/
static Vector_3 UnitV( const Vector_3& v ) { return( v/v.Norm( ) ); }
static Matrix_3x3 UnitMatrix( void ) { return( Matrix_3x3( 1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0 ) ); }

//-----------------------------------------------------------------------------
// Name: BestPlane()
// Description: for a list of points, the best plane is computed and returned.
//
//  The algorithm is that of Andrews and Bernstein, 1976, Acta Cryst, A32, 504-506
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
template< typename ContainerType > 
Plane_3 BestPlane ( const ContainerType& lv )
//-------------------------------------------------------------------------------------
{
    const Vector_3 CenterOfMass( Vector_3::GetCenterOfMass( lv ) );
    Matrix_3x3 inertiaTensor( 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0 );
    typename ContainerType::const_iterator it;

    for( it=lv.begin( ); it!=lv.end( ); ++it )
    {
        inertiaTensor += ( (*it) - CenterOfMass ).InertiaTensorForPoint( 1.0 );
    }

    double eigenValue1;
    Vector_3 eigenVector1;
    inertiaTensor.Eigen1( eigenValue1, eigenVector1 );

    return( Plane_3( eigenVector1, CenterOfMass ) );
}

//-----------------------------------------------------------------------------
// Name: BestLine()
// Description: for a list of points, the best line is computed and returned.
//
//  The algorithm is that of Andrews and Bernstein, 1976, Acta Cryst, A32, 504-506
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
template< typename ContainerType > 
static Line_3 BestLine( const ContainerType& lv )
//-------------------------------------------------------------------------------------
{
    const Vector_3 CenterOfMass( Vector_3::GetCenterOfMass( lv ) );
    Matrix_3x3 inertiaTensor( 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0 );
    typename ContainerType::const_iterator it;

    for( it=lv.begin( ); it!=lv.end( ); ++it )
    {
        inertiaTensor += ( (*it) - CenterOfMass ).InertiaTensorForPoint( 1.0 );
    }

    double eigenValue1;
    Vector_3 eigenVector1;
    inertiaTensor.Inver( ).Eigen1( eigenValue1, eigenVector1 );

    return( Line_3( eigenVector1, CenterOfMass ) );
}


class PairPairDescriptor
{
public:
    PairPairDescriptor( 
        const std::pair<size_t, size_t> mol1, 
        const std::pair<size_t, size_t> mol2 )
        : m_mol1( mol1 )
        , m_mol2( mol2 )
    {
    }
    std::pair<size_t, size_t> GetFirst ( void ) const { return( m_mol1 ); };
    std::pair<size_t, size_t> GetSecond( void ) const { return( m_mol2 ); };
    QR<double> GetQuaternion( void ) const { return( m_quaternion ); };
    void SetQuaternion( const QR<double>& qr ) { m_quaternion = qr; };
    void SetQuaternion( const double angle, const Vector_3& v ) 
    {
        const double cosAng = cos( angle );
        const Vector_3 vSin = v * sin( angle );
        m_quaternion = QR<double>( cosAng, vSin[0], vSin[1], vSin[2] );
    }
    Vector_3 GetAxis( void ) const 
    {
        return( Vector_3( m_quaternion[1], m_quaternion[2], m_quaternion[3] ).UnitV( ) );
    }
private:
    std::pair<size_t, size_t> m_mol1;
    std::pair<size_t, size_t> m_mol2;
    QR<double> m_quaternion;
};

class PairTripleDescriptor {
public:
    PairTripleDescriptor( 
                         const triple<size_t, size_t, size_t> mol1, 
                         const triple<size_t, size_t, size_t> mol2 )
    : m_mol1( mol1 )
    , m_mol2( mol2 )
    {
    }
    triple<size_t, size_t, size_t> GetFirst ( void ) const { return( m_mol1 ); };
    triple<size_t, size_t, size_t> GetSecond( void ) const { return( m_mol2 ); };
    QR<double> GetQuaternion( void ) const { return( m_quaternion ); };
    void SetQuaternion( const QR<double>& qr ) { m_quaternion = qr; };
    void SetQuaternion( const double angle, const Vector_3& v ) 
    {
        const double cosAng = cos( angle );
        const Vector_3 vSin = v * sin( angle );
        m_quaternion = QR<double>( cosAng, vSin[0], vSin[1], vSin[2] );
    }
    Vector_3 GetAxis( void ) const 
    {
        return( Vector_3( m_quaternion[1], m_quaternion[2], m_quaternion[3] ).UnitV( ) );
    }
private:
    triple<size_t, size_t, size_t> m_mol1;
    triple<size_t, size_t, size_t> m_mol2;
    QR<double> m_quaternion;
    
    
};



static bool WithinAngleRange( 
                             PairPairDescriptor& thingToReturn, 
                             const std::pair<double,double> angleRange )
{
    if ( angleRange.first >= 0.0 )
    {
        return( true ); // no angle range specified
    }
    else if ( thingToReturn.GetFirst().first == UINT_MAX )
    {
        return( false ); // no object created by the fractional range check
    }
    else
    {
        // now check that thing to return agrees with the angle limits
        return( true );
    }
} // end of WithinAngleRange

static bool WithinAngleRange_II( 
                             PairTripleDescriptor& thingToReturn, 
                             const std::pair<double,double> angleRange )
{
    if ( angleRange.first >= 0.0 )
    {
        return( true ); // no angle range specified
    }
    else if ( thingToReturn.GetFirst().GetFirst() == UINT_MAX )
    {
        return( false ); // no object created by the fractional range check
    }
    else
    {
        // now check that thing to return agrees with the angle limits
        return( true );
    }
} // end of WithinAngleRange


/*
 FilterTwoListDistanceMatch
 
 The purpose of this is to take two list of atoms that are being used for AB_BestRotation
 and select from each the pairs of atoms that meet the filtering criteria.
 
 For distance and frac match, the distance between two atoms in the first molecule
 and the distance between the putative match atoms in the second must fall
 within the distance and/or frac range specified.
 
 For angle matching, the angle between the normals of the bisecting plane defined
 by the atom pairs in the two lists must fall with the specified range.
 
 */
static
std::list<PairPairDescriptor >  
FilterTwoListDistanceMatch 
( 
       const std::vector<Vector_3>& list1, 
       const std::vector<Vector_3>& list2, 
       const std::pair<double,double> distanceRange,
       const double distRange,
       const std::pair<double,double> angleRange,
       const std::pair<long,long> orddiffrange
)
{
    std::vector< MatchObject > a1List, a2List;
    
    { // makes lists of the allowable pairs based on the distance range
        const double& dmin = distanceRange.first;
        const double& dmax = distanceRange.second;
        const double dminSq = dmin*dmin;
        const double dmaxSq = dmax*dmax;
        
        long list1Accept = 0;
        long list1Reject = 0;
        for( size_t i=0; i!=list1.size( )-1; ++i )
        {
            for( size_t j=i+(1<orddiffrange.first?orddiffrange.first:1); j<list1.size( )&& j<=i+orddiffrange.second; ++j )
            {
                const double dSq = (list1[i]-list1[j]).SquaredLength();
                if( dSq >=dminSq && dSq <=dmaxSq )
                {
                    a1List.push_back( MatchObject( dSq, i, j ) );
                    ++list1Accept;
                    break;
                }
                else
                {
                    ++list1Reject;
                }
            }
        }
        
        for( size_t i=0; i!=list2.size( )-1; ++i )
        {
            for( size_t j=i+(1<orddiffrange.first?orddiffrange.first:1); j<list2.size( )&& j<=i+orddiffrange.second; ++j )
            {
                const double dSq = (list2[i]-list2[j]).SquaredLength();
                if( dSq >=dminSq && dSq <=dmaxSq )
                {
                    a2List.push_back( MatchObject( dSq, i, j ) );
                    break;
                }
            }
        }
        
        std::sort( a1List.begin( ), a1List.end( ) );
        std::sort( a2List.begin( ), a2List.end( ) );
    }
    
    typedef std::pair<size_t, size_t> listPair;
    std::list<PairPairDescriptor > thingToReturnList;
    
    const listPair badPair = std::make_pair( UINT_MAX, UINT_MAX );
    PairPairDescriptor thingToReturn = PairPairDescriptor( badPair, badPair ); // make a bad object to check later
    
    long countFracTotal  = 0;
    long countFracAccepts = 0;
    
    if ( distRange >= 0.0 )
    { // filter the two lists based on distance difference
        
        std::vector<MatchObject>::iterator itmin = a2List.begin( );
        std::vector<MatchObject>::iterator itmax = a2List.begin( );
        
        //for( size_t i1=0; i1!=a1List.size( )-1; ++i1 )
        //{
        //    const MatchObject m = a1List[i1];
        //}
        
        for( size_t i1=0; i1!=a1List.size( )-1; ++i1 )
        {
            const double d1Sq = a1List[i1].d;
            const double lowerSq = pow(sqrt(d1Sq)-distRange,2.0);
            const double upperSq = pow(sqrt(d1Sq)+distRange,2.0);
            
            // find the start and end of the range to check
            itmin = std::lower_bound( itmin, a2List.end( ), MatchObject(lowerSq, UINT_MAX,UINT_MAX) );
            itmax = std::upper_bound( itmax, a2List.end( ), MatchObject(upperSq, UINT_MAX,UINT_MAX) );
            
            //const size_t dist1 = std::distance( itmin, a2List.begin( ) );
            //const size_t dist2 = std::distance( itmin, itmax );
            
            
            
            // test for end()  ????
            for ( std::vector<MatchObject>::const_iterator it=itmin; it!=itmax; ++it )
            {
                ++countFracTotal;
                const MatchObject mo1( a1List[i1] );
                const MatchObject mo2( a1List[i1] );
                thingToReturn = PairPairDescriptor( std::make_pair( mo1.a1, (*it).a1 ), std::make_pair( mo2.a2, (*it).a2 ) );
                if (WithinAngleRange( thingToReturn, angleRange ) )
                {
                    thingToReturnList.push_back( thingToReturn );
                    ++countFracAccepts;
                }
            }
            
        }
    }
    
    
#ifdef VECTOR_3D_DEBUG
    fprintf( stderr, "finished filter code\n" );
    
#endif
    return( thingToReturnList );
} // end of FilterTwoListDistanceMatch


/*
 FilterTwoListDistanceMatch_II
 
 The purpose of this is to take two list of atoms that are being used for AB_BestRotation
 and select from each the triples of atoms that meet the filtering criteria.
 
 For distance and frac match, the distances among three atoms in the first molecule
 and the distances among the putative match atoms in the second must fall
 within the distance and/or frac range specified.
 
 For angle matching, the angle between the normals of the bisecting plane defined
 by the atom pairs in the two lists must fall with the specified range.
// 
// */
//static
//std::list<PairTripleDescriptor >  
//FilterTwoListDistanceMatch_II 
//              ( 
//               const std::vector<Vector_3>& list1, 
//               const std::vector<Vector_3>& list2, 
//               const std::pair<double,double> distanceRange,
//               const double distRange,
//               const std::pair<double,double> angleRange,
//               const std::pair<long,long> orddiffrange
//              )
//{
//    std::vector< MatchObject_II > a1List, a2List;
//    
//    { // makes lists of the allowable pairs based on the distance range
//        const double& dmin = distanceRange.first;
//        const double& dmax = distanceRange.second;
//        
//        CNearTree<Vector_3> treelist1( list1 );
//        CNearTree<Vector_3> treelist2( list2 );
//        std::vector<size_t> jindices;
//        std::vector<Vector_3> jlist;
//        std::vector<size_t> kindices;
//        std::vector<Vector_3> klist;
//        
//        long list1Accept = 0;
//        long list1Reject = 0;
//        for( size_t i=0; i!=list1.size( )-1; ++i )
//        {
//            long jReturn = treelist1.FindInAnnulus(dmin,dmax,jlist,jindices,list1[i]);
//            for ( size_t jind=0; jind < jindices.size(); jind++) {
//                size_t j = jindices[jind];
//                if (j < i+(1<orddiffrange.first?orddiffrange.first:1) || j > i+orddiffrange.second) continue;
//                const double d3 = (list1[i]-list1[j]).Norm();
//                if( d3 >=dmin && d3 <=dmax )
//                {   long kReturn = treelist1.FindInAnnulus(dmin,dmax,klist,kindices,list1[j]);
//                    for ( size_t kind=0; kind < kindices.size(); kind++) {
//                        size_t k = kindices[kind];
//                        if (k < j+(1<orddiffrange.first?orddiffrange.first:1) || k > j+orddiffrange.second) continue;
//                        const double d1 = (list1[j]-list1[k]).Norm();
//                        const double d2 = (list1[i]-list1[k]).Norm();
//                        if (d3 >=  dmin && d3 <=dmax && d1 >=  dmin && d1 <=2.*dmax && d2 < 0.975*(d1+d3) )
//                        {
//                           a1List.push_back( MatchObject_II( d1, d2, d3, i, j, k ) );
//                           ++list1Accept;
//                           break;
//                        }
//                    }
//                }
//                else
//                {
//                    ++list1Reject;
//                }
//            }
//        }
//        
//        for( size_t i=0; i!=list2.size( )-1; ++i )
//        {
//            long jReturn = treelist2.FindInAnnulus(dmin,dmax,jlist,jindices,list2[i]);
//            for ( size_t jind=0; jind < jindices.size(); jind++) {
//                size_t j = jindices[jind];
//                if (j < i+(1<orddiffrange.first?orddiffrange.first:1) || j > i+orddiffrange.second) continue;
//                const double d3 = (list2[i]-list2[j]).Norm();
//                if( d3 >=dmin && d3 <=dmax )
//                {
//                    long kReturn = treelist2.FindInAnnulus(dmin,dmax,klist,kindices,list2[j]);
//                    for ( size_t kind=0; kind < kindices.size(); kind++) {
//                        size_t k = kindices[kind];
//                        if (k < j+(1<orddiffrange.first?orddiffrange.first:1) || k > j+orddiffrange.second) continue;
//                        const double d1 = (list2[j]-list2[k]).Norm();
//                        const double d2 = (list2[i]-list2[k]).Norm();
//                        if (d3 >=  dmin && d3 <=dmax && d1 >=  dmin && d1 <=2.*dmax && d2 < 0.975*(d1+d3) )
//                        {
//                            a2List.push_back( MatchObject_II( d1, d2, d3, i, j, k ) );
//                            break;
//                        }
//                    }
//                }
//            }
//        }
//        
//        if (a1List.size()>0) {
//        std::sort( a1List.begin( ), a1List.end( ) );
//        std::sort( a2List.begin( ), a2List.end( ) );
//        }
//    }
//    
//    typedef triple<size_t, size_t, size_t> listPair;
//    std::list<PairTripleDescriptor > thingToReturnList;
//    if (a1List.size() == 0) return thingToReturnList;
//    
//    const listPair badTriple = make_triple( UINT_MAX, UINT_MAX, UINT_MAX);
//    PairTripleDescriptor thingToReturn = PairTripleDescriptor( badTriple, badTriple ); // make a bad object to check later
//    
//    long countFracTotal  = 0;
//    long countFracAccepts = 0;
//    
//    if ( distRange >= 0.0 )
//    { // filter the two lists based on distance difference
//        
//        std::vector<MatchObject_II>::iterator itmin = a2List.begin( );
//        std::vector<MatchObject_II>::iterator itmax = a2List.begin( );
//
//        for( size_t i1=0; i1!=a1List.size( )-1; ++i1 )
//        {
//            const double d1 = a1List[i1].d1;
//            const double d2 = a1List[i1].d2;
//            const double d3 = a1List[i1].d3;
//            const double lower1 = fabs(d1-distRange);
//            const double upper1 = fabs(d1+distRange);
//            const double lower2 = fabs(d2-distRange);
//            const double upper2 = fabs(d2+distRange);
//            const double lower3 = fabs(d3-distRange);
//            const double upper3 = fabs(d3+distRange);
//            const size_t a1 = a1List[i1].a1;
//            const size_t a2 = a1List[i1].a2;
//            const size_t a3 = a1List[i1].a3;
//            const int sd1 = std::abs((int)a2-(int)a3);
//            const int sd2 = std::abs((int)a1-(int)a3);
//            const int sd3 = std::abs((int)a1-(int)a2);
//            
//            // find the start and end of the range to check
//            itmin = std::lower_bound( itmin, a2List.end( ), MatchObject_II(lower1, lower2, lower3, UINT_MAX, UINT_MAX, UINT_MAX) );
//            itmax = std::upper_bound( itmax, a2List.end( ), MatchObject_II(upper1, upper2, upper3, UINT_MAX, UINT_MAX, UINT_MAX) );
//            
// 
//            for ( std::vector<MatchObject_II>::const_iterator it=itmin; it!=itmax; ++it )
//            {
//                ++countFracTotal;
//                const double d21 = (*it).d1;
//                const double d22 = (*it).d2;
//                const double d23 = (*it).d3;
//                const size_t a21 = (*it).a1;
//                const size_t a22 = (*it).a2;
//                const size_t a23 = (*it).a3;
//                const int sd21 = std::abs((int)a22-(int)a23);
//                const int sd22 = std::abs((int)a21-(int)a23);
//                const int sd23 = std::abs((int)a21-(int)a22);
//                
//                if (fabs(d21-d1)<=0.1 && fabs(d22-d2)<0.1 && fabs(d23-d3)<0.2
//                    &&abs(sd1-sd21) <=0 && abs(sd2-sd22) <=0 && abs(sd3-sd23) <=0) {
//                thingToReturn = PairTripleDescriptor( make_triple( a1, a2, a3), make_triple( a21, a22, a23 ) );
//                if (WithinAngleRange_II( thingToReturn, angleRange ) )
//                {
//                    thingToReturnList.push_back( thingToReturn );
//                    ++countFracAccepts;
//                }
//                }
//            }
//            
//        }
//    }
//    
//    
//#ifdef VECTOR_3D_DEBUG
//    fprintf( stderr, "finished filter code\n" );
//    
//#endif
//    return( thingToReturnList );
//} // end of FilterTwoListDistanceMatch


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// INPUT
// v1                       The Cartesian coordinates of molecule1.
// v2                       The Cartesian coordinates of molecule2.
//
// OUTPUT
// centerOfMass1            The center of mass of molecule1.
// centerOfMass2            The center of mass of molecule2.
// serial1                  The PDB atom serial numbers for molecule1.
// serial2                  The PDB atom serial numbers for molecule2.
// centered_v1              The coordinates of molecule1, translated so its center of mass is at the origin.
// centeredRotated_v2       The coordinates of molecule2 in the position to best match molecule1 after
//                          each has been moved to the origin (and molecule2 rotated).
// quaternionList           The quaternion for each pair that rotates the first onto the second
// axisVectorList           The direction vector of each axis
// axisBaseVectorList       The point on each axis vector nearest the origin
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
class PlaneObject
    {
    public:
        size_t m_ilist1, m_ilist2;
        Plane_3 m_plane;
        PlaneObject( const size_t ilist1, const size_t ilist2, const Plane_3 plane ) :
        m_ilist1( ilist1 ),
        m_ilist2( ilist2 ),
        m_plane ( plane  )
        {
        }
    };


/*  Take two lists of atoms and a list of pairs of pairs of atoms from
    these lists, which each pair of atoms having one atom from each
    list and filter that list limiting the half-angle of rotation
    to [0.,HalfAngleRange] */
template< typename ContainerTypeV, typename ContainerTypeAxis >
ContainerTypeAxis FilterAngleRange( 
                                   const ContainerTypeV& v1, 
                                   const ContainerTypeV& v2,
                                   std::list<PairPairDescriptor > & result,
                                   const double& HalfAngleRange )
{
    std::vector<Vector_3> axisVectorList;
    const double pi = 2.0*atan(1.0);

    long accepted = 0;
    long rejected = 0;

    std::list<PairPairDescriptor >::iterator it;
    for ( it=result.begin(); it!=result.end(); ++it )
    {
        const Vector_3& atom1mol1 = v1[(*it).GetFirst() .first];
        const Vector_3& atom1mol2 = v2[(*it).GetFirst() .second];
        const Vector_3& atom2mol1 = v1[(*it).GetSecond().first];
        const Vector_3& atom2mol2 = v2[(*it).GetSecond().second];
        const Vector_3& atommean = (atom1mol1 + atom1mol2 + atom2mol1 + atom2mol2)/4.;
        const Plane_3 p1 = Plane_3(atom1mol1-atom1mol2,0.5*(atom1mol1+atom1mol2)-atommean);
        const Plane_3 p2 = Plane_3(atom2mol1-atom1mol2,0.5*(atom2mol1+atom2mol2)-atommean);
        const Line_3 l = p1.Intersect(p2);
        if (l.LineAxis().Norm() == 0.0) {
            //reject this one
            ++rejected;
            it = result.erase( it );
            continue;            
        }
       /*  const Vector_3 normal1 = (atom1mol1-atom1mol2);
        const Vector_3 normal2 = (atom2mol1-atom2mol2);
        const Vector_3 crossProduct = normal1.UnitV().Cross(normal2.UnitV());
        const Vector_3 axis = crossProduct.UnitV();
        const double sinAngle = crossProduct.Norm();
        */
        double angle = Vector_3::Angle(l.LineAxis().Cross(atom1mol1-atommean),
                                       Vector_3::GetZeroVector(),
                                       l.LineAxis().Cross(atom1mol2-atommean));
                                       
        if ( fabs(angle) > 2.*  HalfAngleRange )
        {
            //reject this one
            ++rejected;
            it = result.erase( it );
        }
        else
        {
            axisVectorList.insert( axisVectorList.end(), l.LineAxis());
            (*it).SetQuaternion( angle/2., l.LineAxis() );
            ++accepted;
        }
    }
    return( axisVectorList );
}

template< typename ContainerTypeV, typename ContainerTypeQ, typename ContainerTypeAxis >
static QR<double> AB_SubstructureSearchCore( 
                                            const ContainerTypeV& v1, 
                                            const ContainerTypeV& v2,
                                            Vector_3& centerOfMass1,
                                            Vector_3& centerOfMass2,
                                            ContainerTypeV& centered_v1,
                                            ContainerTypeV& centeredRotated_v2,
                                            ContainerTypeQ& quaternionList,
                                            ContainerTypeAxis& axisVectorList,
                                            std::list<PairPairDescriptor >& result
                                            )
//-------------------------------------------------------------------------------------
{
#ifdef VECTOR_3D_DEBUG
    fprintf( stderr, "start filter code\n" );
#endif
    
    QR<double> q;
    const double halfAngleRange = 0.5; // radians
    result =
       FilterTwoListDistanceMatch( v1, v2, std::make_pair( 4.0, 9.0 ), 1.1, 
                               std::make_pair( 0.0, 0.0 ), std::make_pair(1L,7L));
#ifdef VECTOR_3D_DEBUG
    fprintf( stderr, "finished filter code %ld\n", (long int) result.size() );
#endif

    axisVectorList = FilterAngleRange<std::vector<Vector_3>, std::vector<Vector_3> >( v1, v2, result, halfAngleRange );


    //CNearTree<Vector_3> tree( axisVectorList );
    //std::vector<Vector_3> output;
    //CNearTree<Vector_3>::iterator it;
    //for ( it=tree.begin(); it!=tree.end(); ++it )
    //{
    //    std::vector<Vector_3> vClose;
    //    if ( tree.FindInAnnulus( 0.001, 0.035, vClose, *it ) > 3 )
    //    {
    //        output.push_back( *it );
    //    }
    //}

    //for( size_t i=0; i<output.size(); ++i )
    //{
    //    //fprintf( stdout, "  %f,%f,%f\n", output[i][0], output[i][1], output[i][2] );
    //}

    QR<double> qsum( 0.0, 0.0, 0.0, 0.0 ); // Quaternion sum for averaging

   return( qsum );
}

std::pair<double,Vector_3> CircleCenterFromThreePoints( const Vector_3& v1, const Vector_3& v2, const Vector_3& v3 );


#endif  //  vector_3_INCLUDED



