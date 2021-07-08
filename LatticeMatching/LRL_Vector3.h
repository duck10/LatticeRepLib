#ifndef VECTOR_3D_H_INCLUDED
#define VECTOR_3D_H_INCLUDED

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <climits>
#include <list>
#include <iostream>
#include <utility>
#include <vector>

#pragma warning( disable : 4505) // unreferenced local function has been removed

// forward declarations
class Vector_3;
class Matrix_3x3;

// these are friend functions, put here because LINT complains if they are at the end
//#ifdef __cplusplus
//    std::ostream& operator<<     ( std::ostream&, const Vector_3& );
//    std::ostream& operator<<     ( std::ostream&, const Matrix_3x3& );
//#endif
Vector_3 operator* ( const double&, const Vector_3& );
double Angle ( const Vector_3& a,  const Vector_3& b );



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
class Vector_3
{
    friend class Matrix_3x3;

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

    inline double norm(void) const
    {
       return(sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]));
    }

    inline double Norm(void) const
    {
       return(sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]));
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

    //complex operations
    Matrix_3x3 Rotmat ( const double angle ) const;

    //access and io functions
    //-----------------------------------------------------------------------------
    // Name: operator[]()
    // Description: access function for the components of a vector
    //
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline double operator[] ( const size_t i ) const
    //-------------------------------------------------------------------------------------
    {
       size_t n = ( i<0 ) ? 0 : i;
        if( i > 2 ) n = 2;
        return (v[n]);
    }

    inline double& operator[] ( const size_t i )
    //-------------------------------------------------------------------------------------
    {
       size_t n = ( i<0 ) ? 0 : i;
        if( i > 2 ) n = 2;
        return (v[n]);
    }

    //-----------------------------------------------------------------------------
    // Name: at()
    // Description: access function for the components of a vector
    //
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    inline double at ( const size_t i ) const
    //-------------------------------------------------------------------------------------
    {
       size_t n = ( i<0 ) ? 0 : i;
        if( i > 2 ) n = 2;
        return (v[n]);
    }

    // constants
    static const double    MINNORM;
    
}; // end of class vector

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
class Matrix_3x3
{
    friend class Vector_3;

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

size_t GetRowDim() const { return 3; }

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

   if (std::abs(sinGamma) < 1.0E-10) {
      return Matrix_3x3(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
   }
   else {
      const Matrix_3x3 amat(
         a, b*cosGamma, c*cosBeta,
         0.0, b*sinGamma, c*(cosAlpha - cosBeta*cosGamma) / sinGamma,
         0.0, 0.0, V / (a*b*sinGamma)
      );

      return(amat);
   }
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


}; // end of class Matrix_3x3


/*=========================================================================================*/
/* start of non-class functions */
/*=========================================================================================*/
static Vector_3 UnitV( const Vector_3& v ) { return( v/v.Norm( ) ); }
static Matrix_3x3 UnitMatrix( void ) { return( Matrix_3x3( 1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0 ) ); }


#endif  //  vector_3_INCLUDED



