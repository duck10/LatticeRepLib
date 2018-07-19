
#include <cmath>
#include <cfloat>

#include <climits>
#include <iostream>
/* remove the next include if using a local version of rand */
#include <stdlib.h>
#ifdef USE_SYSTEM_HEADERS
#include <vector_3d.h>
#else
#include "vector_3d.h"
#endif

// static const member variable assignments

//const Vector_3 Vector_3::GetXAxis( )     ( 1.0, 0.0, 0.0 );
//const Vector_3 Vector_3::GetYAxis( )     ( 0.0, 1.0, 0.0 );
//const Vector_3 Vector_3::GetZAxis( )     ( 0.0, 0.0, 1.0 );
//const Vector_3 Vector_3::zeroVector( 0.0, 0.0, 0.0 );
// const double    Vector_3::MINNORM = 10.0*FLT_MIN;
const double    Vector_3::MINNORM = 1.0E-8;


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static void initrn( int& iseed, int& indx, int& jndx, int& kndx, double buffer[56] )
{
    jndx = iseed;
    for( indx=0; indx<56; ++indx )
    {
        jndx = (jndx*2349 + 14867)%32767;
        buffer[indx] = ::fabs(double(jndx)/32767.0);
    }
    iseed = -iseed;
    if( iseed == 0 ) iseed = -1;
    indx = 55;
    kndx = 54;
    jndx = 31;
}

class Matrix_3x3;

//C**********************************************************************C
//      SUBROUTINE RANORI (ISEED,AMAT)
//C
//C derived from
//C   real function randf( iseed )
//C        if seed .gt. 0 use seed to initialize
//C        returns a rotation matrix for a uniformly distributed
//C        random rotation in 0-360 degrees about a
//C        uniformly distributed random vector
//C
//C    RANORI DOES NOT CALL RANVEC TO GET THE RANDOM VECTOR, SO THAT
//C    THE USERS HAS THE OPTION OF GENERATING SEPARATE, DIFFERENT
//C    STRINGS OF RANDOM ORIENTATIONS AND VECTORS
//C
//
//C  the algorithm is derived from one in J.M.Hammersley and D.C.
//C  Handscomb, "Monte Carlo Methods," Methuen & Co., London and
//C  Wiley & Sons, New York,1964, p47.
//C  The method is to generate a random unit quaterion, and
//C  then to turn it into the corresponding rotation matrix.
//C  Kearsley, Acta Cryst., v. A45, p. 208-210 (1989) solves a
//C  related linear least-squares problem.  The linearity
//C  gives a hint that this method can be used.
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double randf( int& iseed )
{
    //        if seed .gt. 0 use seed to initialize
    //        returns a uniformly distributed random number in (0. < 1.)
    static double randomNumberBuffer[56];
    static int indx = -1;
    static int jndx;
    static int kndx;
    if( iseed >= 0 || indx < 0 )
    {
        initrn( iseed, indx, jndx, kndx, randomNumberBuffer );
    }
    indx = indx%55 + 1;
    jndx = jndx%55 + 1;
    kndx = kndx%55 + 1;
    double dTemp;
    randomNumberBuffer[indx] = modf( randomNumberBuffer[jndx]+randomNumberBuffer[kndx], &dTemp );
    return( randomNumberBuffer[indx] );
}

Matrix_3x3 RandomOrientation( int& iseed )
{

    const int nc = 4; // number of elements in the quaternion
    double sum = 0.0;
    double v[4];
    do
    {
        sum = 0.0;
        for( int i=0; i<nc; ++i )
        {
            v[i] = 2.0 * randf(iseed) - 1.0;
            sum += v[i] * v[i];
            if(sum > 1.0) break;
        }
    } while(sum > 1.0);
    sum = ::sqrt( sum );

    for( int i=0; i<4; ++i )
    {
        v[i] /= sum;
    }

    return( Matrix_3x3(
        v[0]*v[0] + v[1]*v[1] - v[2]*v[2] - v[3]*v[3],
        2.0*(v[1]*v[2] + v[0]*v[3]),
        2.0*(v[1]*v[3] - v[0]*v[2]),
        2.0*(v[1]*v[2] - v[0]*v[3]),
        v[0]*v[0] + v[2]*v[2] - v[1]*v[1] - v[3]*v[3],
        2.0*(v[2]*v[3] + v[0]*v[1]),
        2.0*(v[1]*v[3] + v[0]*v[2]),
        2.0*(v[2]*v[3] - v[0]*v[1]),
        v[0]*v[0] + v[3]*v[3] - v[1]*v[1] - v[2]*v[2] ) );
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Vector_3 randVector( int& iseed )
{
    double v[3];
    do
    {
        for( int i=0; i<3; ++i )
        {
            v[i] = 2.0 * randf(iseed) -1.0;
        }
    } while( v[0]*v[0]+v[1]*v[1]+v[2]*v[2] > 1.0 ); // reject those in the corners
    return( Vector_3( v[0], v[1], v[2] ) );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static double randfg( int& iseed )
{
    //C   real function randfg( iseed )
    //C        if seed .gt. 0 use seed to initialize
    //C        returns a normally distributed random number with unit variance
    //C
    //C   EXACTLY FOLLOWS THE ALGORITHM OF KNUTH, THE ART OF COMPUTER
    //C   PROGRAMMING, V. 22D. ED., 1981, P. 117-118  (THE POLAR METHOD)
    //C   EXCEPT THAT LIMITS ARE INCLUDED BY MAKING THE MINIMUM VALUE OF "S"
    //C   BE EPS
    static double randomNumberBuffer[56];
    static int indx = -1;
    static int jndx;
    static int kndx;
    static double eps;
    double r1 = 1.0;
    double r2 = 0.0;

    if( iseed >= 0 || indx < 0 )
    {
        r1 = 1.0;
        for( int i=0; i<100; ++i )
        {
            eps = r1;
            r1 = r1/2.0;
            if( r1+1.0 == 1.0 ) break;
        }
        initrn( iseed, indx, jndx, kndx, randomNumberBuffer );
    }

    double s;
    do{
        indx = indx%55 + 1;
        jndx = jndx%55 + 1;
        kndx = kndx%55 + 1;
        double dTemp;
        randomNumberBuffer[indx] = modf( randomNumberBuffer[jndx]+randomNumberBuffer[kndx], &dTemp );
        r1 = 2.0 * randomNumberBuffer[indx] - 1.0;
        indx = indx%55 + 1;
        jndx = jndx%55 + 1;
        kndx = kndx%55 + 1;
        randomNumberBuffer[indx] = modf( randomNumberBuffer[jndx]+randomNumberBuffer[kndx], &dTemp );
        r2 = 2.0 * randomNumberBuffer[indx] - 1.0;
        s = r1*r1 + r2*r2;
        if( s >= 1.0 ) continue;
        if( ::fabs(s) < eps ) s = ( s >= 0 )? eps : -eps;
    } while( s >= 1.0 );

    return( r1 * ::sqrt( -2.0 * log10(s)/s ) );
}


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
// Name: Pair()
// Description: returns a matrix that rotates X1 to coincide with Z1
//  and put X2 in the plane of Z1,Z2 as close to Z2 as possible
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3 Vector_3::Pair( const Vector_3& x1, const Vector_3& x2, const Vector_3& z1, const Vector_3& z2 )
//-------------------------------------------------------------------------------------
{
    Matrix_3x3 mTemp( 1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0 );
    const Vector_3 zeroVector = Vector_3::GetZeroVector( );
    const double x1Norm = x1.Norm( );
    const double z1Norm = z1.Norm( );
    if( x1Norm < Vector_3::MINNORM || z1Norm < Vector_3::MINNORM )
    {
        return( mTemp );
    }
    else
    {
        const Vector_3 x1Unit = x1.UnitV( );
        const Vector_3 z1Unit = z1.UnitV( );
        Vector_3 vx1 = x1.Cross( z1 ); // find the rotation axis that is perpendicular to x1 and z1 so we can place x1 on z1
        //if( vx1.SquaredLength( ) < 10.0*DBL_EPSILON *10.0*DBL_EPSILON )
        //{
        //    vx1 = Vector_3::GetXAxis( );
        //}
        //else
        //{
        //    // vx1 = vx1.UnitV( ); // rescale the rotation axis
        //}
        //double angle1 = -Vector_3::Torsion( x1,vx1,zeroVector,z1);
        const double angleTest = Vector_3::Angle( x1, Vector_3::GetZeroVector( ), z1 );
        const Matrix_3x3 bMat = vx1.Rotmat( angleTest );
       //}

        const Vector_3 vx2 = bMat.MV(x2);
        const double angle2 = -Vector_3::Torsion( vx2,z1,zeroVector,z2);
        const Matrix_3x3 cMat = z1.Rotmat( angle2 ); // now do a rotation about the already fixed vector
        //const double angle3 = -Vector_3::Torsion( vx2.MV( cMat ), z1, Vector_3::GetZeroVector( ), z2 );

        //if( angle3 > DBL_EPSILON)
        //{
        //    /* INFO: probably didn't get a good answer */
        //}

        {
            //// check that the point is in the plane
            //const Vector_3 vperp = z1.Cross( z2 );
            //const double error = (vx2.MV( cMat )).Dot( vperp.UnitV( ) );

            //if( error > DBL_EPSILON)
            //{
            //    /* INFO: probably didn't get a good answer */
            //}
        }
        const Matrix_3x3 mResult = cMat*bMat;
            //const Vector_3 x1After_3 = mResult.MV( x1 );
            //const double distSqBefore1_3 = (x1After_3-z1Unit).SquaredLength( );
            //const double distSqBefore2_3 = (x1After_3+z1Unit).SquaredLength( );
            //const double thirdDistSqBefore = (distSqBefore1_3<distSqBefore2_3) ? distSqBefore1_3 : distSqBefore2_3;
        return( mResult );
    }
    return ( mTemp );
}


//-----------------------------------------------------------------------------
// Name: HornQRPlane2Plane()
// Description: Returns a quaternion representing the optimal rotation of
//              three ordered no-colinear points (v1, v2, v3) around their centroid 
//              to bring them into best alignment with three ordered no-colinear 
//              target points (t1, t2, t3) around their centroid.
//
//              The algorithm is due to Horn, "Closed-form solution of absolute 
//              orientation using unit quaternions," Journal of the Optical 
//              Society of America A, Vol, 4, page 629, April 1987.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
QR<double> Vector_3::HornQRPlane2Plane( const Vector_3& v1, const Vector_3& v2,const Vector_3& v3,
                                       const Vector_3& t1, const Vector_3& t2, const Vector_3& t3)
{
    const Vector_3 vcentroid = (v1+v2+v3)/3.;
    const Vector_3 tcentroid = (t1+t2+t3)/3.;
    const Vector_3 rv1 = v1-vcentroid;
    const Vector_3 rv2 = v2-vcentroid;
    const Vector_3 rv3 = v3-vcentroid;
    const Vector_3 rt1 = t1-tcentroid;
    const Vector_3 rt2 = t2-tcentroid;
    const Vector_3 rt3 = t3-tcentroid;
    const Vector_3 nv = (rv1.Cross(rv3)).UnitV();
    const Vector_3 nt = (rt1.Cross(rt3)).UnitV();
    const double ctheta = nv.Dot(nt);
    const Vector_3 axis = (nv.Cross(nt)).UnitV();
    const double stheta = (nv.Cross(nt)).Norm();
    const double angp2p = atan2(stheta,ctheta);
    const QR<double> qa = QR<double>(cos(angp2p/2.),sin(angp2p/2.)*axis.v[0],sin(angp2p/2.)*axis.v[1],sin(angp2p/2.)*axis.v[2]);
    const Vector_3 qv1 = QR<double>(qa*(QR<double>(rv1))*qa.Conjugate()).ImVector();
    const Vector_3 qv2 = QR<double>(qa*(QR<double>(rv2))*qa.Conjugate()).ImVector();
    const Vector_3 qv3 = QR<double>(qa*(QR<double>(rv3))*qa.Conjugate()).ImVector();
    const double C = qv1.Dot(rt1)+ qv2.Dot(rt2)+ qv3.Dot(rt3);
    const double S = (qv1.Cross(rt1)+ qv2.Cross(rt2)+ qv3.Cross(rt3)).Dot(nt);
    const double angp = atan2(S,C);
    const QR<double> qp = QR<double>(cos(angp/2.),sin(angp/2.)*nt.v[0],sin(angp/2.)*nt.v[1],sin(angp/2.)*nt.v[2]);
    return qa*qp;
}



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
// Name: GeneralRotation()
// Original Defect:  Q-3726 larrya
// Description: For a given rotation angle about the line, "L",
//              GeneralRotation computes the transformation such that for some
//              input vector "vIn", an output vector "vOut" can be computed by:
//              vOut = m*vIn + vTrans
//              where m is the matrix returned in the output pair and vTrans
//              is the vector in that pair.
//  
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
std::pair<Matrix_3x3, Vector_3> Vector_3::GeneralRotation( 
   const double angle, 
   const Line_3& L )
{
    return( GeneralRotation( angle, L.BasePoint(), L.BasePoint()+L.LineAxis() ) );
}

//-----------------------------------------------------------------------------
// Name: GeneralRotation()
// Original Defect:  Q-3726 larrya
// Description: For a given rotation angle about the "this" line
//              GeneralRotation computes the transformation such that for some
//              input vector "vIn", an output vector "vOut" can be computed by:
//              vOut = m*vIn + vTrans
//              where m is the matrix returned in the output pair and vTrans
//              is the vector in that pair.
//  
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
std::pair<Matrix_3x3, Vector_3> Line_3::GeneralRotation( const double angle )
{
    return( Vector_3::GeneralRotation( angle, *this ) );
}

//-----------------------------------------------------------------------------
// Name: Eigen1()
// Description: Computes the eigenvector of the largest eigenvalue of a 3x3 matrix
// and also returns that eigenvalue
//
//  The algorithm is that of Andrews and Bernstein, 1976, Acta Cryst, A32, 504-506
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool Matrix_3x3::Eigen1( double& eigenvalue1, Vector_3& eigenvector1 ) const
//-------------------------------------------------------------------------------------
{
    Matrix_3x3 x( *this );
    for( int i=0; i<18; ++i )
    {
        double trace = x[0] + x[4] + x[8];
        if( ::fabs(trace) < Vector_3::MINNORM )
        {
            trace = 1.0;
        }
        x = x / trace; // just to keep things in a reasonable range of values
        x = x*x;
    }
    // by this point in the code, the matrix _should_ have been reduced to a matrix with
    // a single row (and column) that is non-zero (or at least the others are very small)

    // now get the eigenvalue
    const Matrix_3x3 b = (*this) * x; // isolate the eigenvector
    const Vector_3 v0(b[0],b[1],b[2]);
    const Vector_3 v1(b[3],b[4],b[5]);
    const Vector_3 v2(b[6],b[7],b[8]);
    Vector_3 vResult = v0;
    const double v0Norm = v0.Norm( );
    const double v1Norm = v1.Norm( );
    const double v2Norm = v2.Norm( );
    if( v0Norm < v1Norm )
    {
        vResult = v1;
    }
    if( vResult.Norm( ) < v2Norm )
    {
        vResult = v2;
    }
    eigenvector1 = vResult.UnitV( );
    eigenvalue1  = (this->MV(eigenvector1)) .Norm( );

    // test for negative eigenvalues
    if( eigenvector1.Dot( (*this).MV( eigenvector1 )  )< 0.0 )
    {
        eigenvector1 = -eigenvector1;
        eigenvalue1  = -eigenvalue1;
    }


    // just for testing
    //const Vector_3 ev = eigenvector1.UnitV( );
    //const Vector_3 enext = ev.MV( *this );
    //if( (ev-enext.UnitV( )).Norm( ) > 2.0*DBL_EPSILON )
    //{
    //    // probably a problem here
    //    const int i19191 = 19191;
    //}

    return( true );
}

//-----------------------------------------------------------------------------
// Name: Eigen3()
// Description: Computes the eigenvectors and eigenvalues for a 3x3 matrix
//              The vectors are computed sequentially from the largest, which
//              is then removed by deflation, etc.
//
//-----------------------------------------------------------------------------
bool Matrix_3x3::Eigen3( std::vector<double>& eigenValues, std::vector<Vector_3>& eigenVectors ) const
{
    const double MINEIGENVALUE = 1.0E-16;
    Matrix_3x3 m1( *this );

    double evalue1;
    Vector_3 evector1;
    const bool b1 = m1.Eigen1( evalue1, evector1 );
    if( ! b1 ) { /* some problem occurred */ };

    if( ::fabs(evalue1) < MINEIGENVALUE )
    {
        // all three eigenvalues are zero - deal with it!
        eigenValues.push_back( 0.0 );
        eigenValues.push_back( 0.0 );
        eigenValues.push_back( 0.0 );
        eigenVectors.push_back( Vector_3( 1.0, 0.0, 0.0 ) );
        eigenVectors.push_back( Vector_3( 0.0, 1.0, 0.0 ) );
        eigenVectors.push_back( Vector_3( 0.0, 0.0, 1.0 ) );
        return( false );
    }
    else
    {
        eigenValues.push_back( evalue1 );
        eigenVectors.push_back( evector1 );
    }

    const Matrix_3x3 eigMatrix( evalue1*evector1.MatrixProduct( evector1 ) );
    Matrix_3x3 m2( m1-eigMatrix );

    double evalue2;
    Vector_3 evector2;
    const bool b2 = m2.Eigen1( evalue2, evector2 );
    if( ! b2 ) { /* some problem occurred */ };

    if( ::fabs(evalue2) < MINEIGENVALUE )
    {
        // 2nd and 3rd eigenvalues are zero - invent 2 eigenvectors!
        eigenValues.push_back( 0.0 );
        eigenValues.push_back( 0.0 );
        const Vector_3 randomVector( (double)rand( ), (double)rand( ), (double)rand( ) );
        const Vector_3 ev2 = eigenVectors[0].Cross( randomVector );
        eigenVectors.push_back( Vector_3( ev2 ) );
        eigenVectors.push_back( eigenVectors[0].Cross( ev2 ) );
        return( false );
    }
    else
    {
        eigenValues.push_back( evalue2 );
        eigenVectors.push_back(evector2 );
    }

    Matrix_3x3 m3( m2-evalue2*evector2.MatrixProduct( evector2 ) );

    double evalue3;
    Vector_3 evector3;
    const bool b3 = m3.Eigen1( evalue3, evector3 );
    if( ! b3 ) { /* some problem occurred */ };

    if( ::fabs(evalue3) < MINEIGENVALUE )
    {
        // 3rd eigenvalue is zero - calculate the eigenvector from the other two
        eigenValues.push_back( 0.0 );
        eigenVectors.push_back( eigenVectors[0].Cross( eigenVectors[1] ) );
        return( true );
    }
    else
    {
        eigenValues.push_back( evalue3 );
        eigenVectors.push_back( evector3);
    }

    return( true );
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
// Name: MatrixProduct()
// Description: return the tensor product of two vectors
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3 Vector_3::MatrixProduct ( const Vector_3& v_other ) const
//-------------------------------------------------------------------------------------
{
    Matrix_3x3 m;
    m.m[0] = v[0]*v_other[0];
    m.m[1] = v[0]*v_other[1];
    m.m[2] = v[0]*v_other[2];
    m.m[3] = v[1]*v_other[0];
    m.m[4] = v[1]*v_other[1];
    m.m[5] = v[1]*v_other[2];
    m.m[6] = v[2]*v_other[0];
    m.m[7] = v[2]*v_other[1];
    m.m[8] = v[2]*v_other[2];
    return (m);
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

//////////////////////////////////////////////////////////////////////
// Plane_3 Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: Plane_3()
// Description: constructor
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Plane_3::Plane_3( void ) :
    m_planeNormal ( 0.0, 0.0, 0.0 ),
    m_basePoint   ( 0.0, 0.0, 0.0 ),
    m_centerOfMass( 0.0, 0.0, 0.0 )
//-------------------------------------------------------------------------------------
{

}

//-----------------------------------------------------------------------------
// Name: Plane_3()
// Description: destructor
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Plane_3::~Plane_3( void )
//-------------------------------------------------------------------------------------
{

}

//-----------------------------------------------------------------------------
// Name: Plane_3()
// Description: constructor
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Plane_3::Plane_3( const Vector_3& direction, const Vector_3& CenterOfMass) :
   m_planeNormal ( direction.UnitV( ) ),
   m_basePoint   ( CenterOfMass.Dot( m_planeNormal ) * m_planeNormal ),
   m_centerOfMass( CenterOfMass )
//-------------------------------------------------------------------------------------
{
    // center of mass could validly be any point on the plane
}

//-----------------------------------------------------------------------------
// Name: NormalVector()
// Description: returns the vector normal to a determined plane
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Vector_3 Plane_3::NormalVector( void ) const
//-------------------------------------------------------------------------------------
{
    return( m_planeNormal );
}

//-----------------------------------------------------------------------------
// Name: BasePoint()
// Description: returns the point in the plane where a normal to the plane from
// the origin will intersect the plane.
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Vector_3 Plane_3::BasePoint( void ) const
//-------------------------------------------------------------------------------------
{
    return( m_basePoint );
}

//-----------------------------------------------------------------------------
// Name: Line_3()
// Description: constructor
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Line_3::Line_3( const Vector_3& direction, const Vector_3& CenterOfMass ) :
m_lineAxis( direction.UnitV( ) ),
m_basePoint( CenterOfMass ),
m_centerOfMass( CenterOfMass )
//-------------------------------------------------------------------------------------
{
    // Compute the "prep", the perpendicular projector of the line.
    const Matrix_3x3 mPrep = UnitMatrix( ) - LineAxis( ).MatrixProduct( LineAxis( ));
    if ( direction.SquaredLength( ) == 0.0 )
    {
       m_lineAxis = Vector_3( 0.0 ,0.0, 0.0 );
       m_basePoint = Vector_3( 0.0 ,0.0, 0.0 );
       m_centerOfMass = Vector_3( 0.0 ,0.0, 0.0 );
    }
    else
    {
       m_basePoint = mPrep.MV( m_basePoint );
    }
}

//-----------------------------------------------------------------------------
// Name: LineAxis()
// Description: returns the vector direction of a line
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Vector_3 Line_3::LineAxis( void ) const
//-------------------------------------------------------------------------------------
{
    return( m_lineAxis );
}

//-----------------------------------------------------------------------------
// Name: BasePoint()
// Description: returns the point in the line where a normal to the line from
// the origin will intersect the line.
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Vector_3 Line_3::BasePoint( void ) const
//-------------------------------------------------------------------------------------
{
    return( m_basePoint );
}

//-----------------------------------------------------------------------------
// Name: Torsion()
// Description: computes the torsion angle formed by 4 points. This is the angle
// between the plane formed by the first 3 and the plane formed by the last 3.
//
double Vector_3::Torsion( const Vector_3& a, const Vector_3& b, const Vector_3& c, const Vector_3& d )
//-------------------------------------------------------------------------------------
{
    const Vector_3 zero( 0.0 );
    Vector_3 p = a - b;
    Vector_3 q = c - b;
    Vector_3 x = p.Cross( q );
    p = b - c;
    q = d - c;
    Vector_3 y = p.Cross( q );
    double torsionAngle = Vector_3::Angle( x, zero, y );
    q = x.Cross(y);

    if( p.Dot(q.UnitV( )) > 0.0 ) torsionAngle = -torsionAngle;

    return( torsionAngle );
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

//-----------------------------------------------------------------------------
// Name: InertiaTensorForPoint()
// Description: computes the inertia tensor for a SINGLE point. The input also
// includes a weight. Frequently, all the weights are 1.00
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3 Vector_3::InertiaTensorForPoint( const double weight  ) const
//-------------------------------------------------------------------------------------
{
    Matrix_3x3 tensor( 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0 );
    const double am = weight * this->Dot( *this );
    tensor.m[0] += am - this->v[0] * this->v[0];
    tensor.m[4] += am - this->v[1] * this->v[1];
    tensor.m[8] += am - this->v[2] * this->v[2];
    tensor.m[1] -= this->v[0] * this->v[1];
    tensor.m[3]  = tensor.m[1];
    tensor.m[2] -= this->v[0] * this->v[2];
    tensor.m[6]  = tensor.m[2];
    tensor.m[5] -= this->v[1] * this->v[2];
    tensor.m[7]  = tensor.m[5];

    return( tensor );
}

//-----------------------------------------------------------------------------
// Name: UNX()
// Description: Compute the rotated component of the X axis
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3 Matrix_3x3::UNX( void ) const
//-------------------------------------------------------------------------------------
{
    const double denominator = ::fabs(m[4]+m[8]);
    double numerator = m[7]-m[5];
    if ( ::fabs(numerator/denominator) < Vector_3::MINNORM ) numerator = 0.0;
    const double angle = ::atan2( numerator, denominator );
    return( Vector_3::GetXAxis( ).Rotmat( -angle ) );
}

//-----------------------------------------------------------------------------
// Name: UNY()
// Description: Compute the rotated component of the Y axis
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3 Matrix_3x3::UNY( void ) const
//-------------------------------------------------------------------------------------
{
    const double denominator = ::fabs(m[0]+m[8]);
    double numerator = m[6]-m[2];
    if ( ::fabs(numerator/denominator) < Vector_3::MINNORM ) numerator = 0.0;
    const double angle = ::atan2( numerator, denominator );
    return( Vector_3::GetYAxis( ).Rotmat( angle ) );
    // IS THIS REALLY RIGHT X and Z are negative
    // (probably due to change in position of sub determinant of the row/column)
}

//-----------------------------------------------------------------------------
// Name: UNZ()
// Description: Compute the rotated component of the Z axis
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Matrix_3x3 Matrix_3x3::UNZ( void ) const
//-------------------------------------------------------------------------------------
{
    const double denominator = ::fabs(m[0]+m[4]);
    double numerator = m[3]-m[1];
    if ( ::fabs(numerator/denominator) < Vector_3::MINNORM ) numerator = 0.0;
    const double angle = ::atan2( numerator, denominator );
    return( Vector_3::GetZAxis( ).Rotmat( -angle ) );
}

//-----------------------------------------------------------------------------
// Name: Line_3()
// Description: constructor
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Line_3::Line_3( void ) :
    m_lineAxis    ( 0.0 ),
    m_basePoint   ( 0.0 ),
    m_centerOfMass( 0.0 )
//-------------------------------------------------------------------------------------
{

}

//-----------------------------------------------------------------------------
// Name: ~Line_3()
// Description: destructor
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Line_3::~Line_3( void )
//-------------------------------------------------------------------------------------
{

}

//-----------------------------------------------------------------------------
// Name: DistanceFromPlane()
// Description: computes the distance of any point from a plane by subtracting
// the vector to the point from any point in the plane and then projecting onto the
// normal to the plane.
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double Plane_3::DistanceFromPlane( const Vector_3& v ) const
//-------------------------------------------------------------------------------------
{
    return( ::fabs((m_basePoint - v).Dot(m_planeNormal)) );
}

//-----------------------------------------------------------------------------
// Name: DistanceFromPlane()
// Description: computes the distance of any point from a plane
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double Vector_3::DistanceFromPlane( const Plane_3& p ) const
//-------------------------------------------------------------------------------------
{
    return( p.DistanceFromPlane( *this ) );
}

//-----------------------------------------------------------------------------
// Name: DistanceFromLine()
// Description: computes the distance of any point from a line by subtracting the
// point from any point on the line and then projecting onto a normal to the line.
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double Line_3::DistanceFromLine( const Vector_3& v ) const
//-------------------------------------------------------------------------------------
{
    // Compute the "prep", the perpendicular projector of the line.
    const Matrix_3x3 mPrep = UnitMatrix( ) - LineAxis( ).MatrixProduct( LineAxis( ));
    const Vector_3 vTemp = mPrep.MV( v-m_basePoint ); //

    return( vTemp.Norm( ) );
}

//-----------------------------------------------------------------------------
// Name: DistanceFromLine()
// Description: computes the distance of any point from a line
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double Vector_3::DistanceFromLine( const Line_3& vv ) const
//-------------------------------------------------------------------------------------
{
    return( vv.DistanceFromLine( *this ) );
}

QR<double> Vector_3::QuaternionFromPair( const Vector_3& v1, const Vector_3& v2 )
{
   const Vector_3 axis = v1.Cross( v2 ).UnitV( );
   const double angle = Torsion( axis, v1, v2, Vector_3::GetZeroVector( ) );
   QR<double> q(
                cos( angle/2.0 ),
                sin( angle/2.0 ) * axis[0],
                sin( angle/2.0 ) * axis[1],
                sin( angle/2.0 ) * axis[2] 
               );

   return( q );
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
QR<double> randQuaternion( int& iseed )
{
    const int nc = 4; // number of elements in the quaternion
    double sum = 0.0;
    double v[4];
    QR<double> q;
    do
    {
        sum = 0.0;
        for( int i=0; i<nc; ++i )
        {
            v[i] = 2.0 * randf(iseed) - 1.0;
            sum += v[i] * v[i];
            if(sum > 1.0) break;
        }
    } while(sum > 1.0);
    sum = ::sqrt( sum );
    
    for( int i=0; i<4; ++i )
    {
        v[i] /= sum;
    }
    
    q.Set(v[0],v[1],v[2],v[3]);
    return q;
}

// return the radius and the center of the circle
std::pair<double,Vector_3> CircleCenterFromThreePoints( const Vector_3& v1, const Vector_3& v2, const Vector_3& v3 )
{
// WARNINGS FROM THE SOURCE WEB PAGE
    //   * The denominator (mb - ma) is only zero when the lines are parallel 
   //in which case they must be coincident and thus no circle results.

    //* If either line is vertical then the corresponding slope is infinite. 
    //This can be solved by simply rearranging the order of the points so that vertical lines do not occur. 

   std::vector<Vector_3> vlist;
   vlist.push_back( v1 );
   vlist.push_back( v2 );
   vlist.push_back( v3 );
   const Plane_3 plane = BestPlane( vlist );

   int seed = 19192;
   Matrix_3x3 m = Vector_3::Pair( plane.NormalVector( ), randVector(seed), Vector_3::GetZAxis( ), Vector_3::GetYAxis( ) );

   const Vector_3 rot1 = m.MV( v1 );
   const Vector_3 rot2 = m.MV( v2 );
   const Vector_3 rot3 = m.MV( v3 );

   // from http://local.wasp.uwa.edu.au/~pbourke/geometry/circlefrom3/
   const double x1 = rot1[0];
   const double x2 = rot2[0];
   const double x3 = rot3[0];

   const double y1 = rot1[1];
   const double y2 = rot2[1];
   const double y3 = rot3[1];

   if ( ::fabs(x2-x1) == 0.0 || ::fabs(x3-x2) == 0.0 )
   {
      return( CircleCenterFromThreePoints( v3,v2,v1 ) );
   }

   const double ma = (y2-y1) / (x2-x1);
   const double mb = (y3-y2) / (x3-x2);

   if ( ma==mb )
   {
      return( std::make_pair(DBL_MAX, Vector_3( DBL_MAX )) );
   }

   const double x = ma*mb*(y1-y3) + mb*(x1+x2) - ma*(x2+x3) / ( 2.0 * (mb-ma ) );
   const double y = -(1.0/ma)*(x-(x1+x2)/2.0)+ (y1+y2)/2.0;

   const Vector_3 centerInXYPlane = Vector_3( x,y,rot1[2] );  // pick up the z-coordinate of the rotated point
   const Vector_3 centerInOriginal = centerInXYPlane.MV( m.Inver( ) );

   const double radius = (centerInOriginal-v1).Norm( );

   return( std::make_pair( radius, centerInOriginal ) );
}

Line_3 Plane_3::Intersect( const Plane_3& p ) const
{
   const Vector_3& n1 = (*this).m_planeNormal;
   const Vector_3& n2 =       p.m_planeNormal;
   const Vector_3& p1 = (*this).m_basePoint;
   const Vector_3& p2 =       p.m_basePoint;

   const Vector_3 vtest = n1.Cross( n2 );

   Line_3 result;
   if ( vtest.SquaredLength( ) < 0.005*0.005 )
   {
      result = Line_3( Vector_3(0.0,0.0,0.0), Vector_3(0.0,0.0,0.0 ) );
   }
   else
   {
      const double& n11 = n1[0];
      const double& n12 = n1[1];
      const double& n13 = n1[2];
      const double& n21 = n2[0];
      const double& n22 = n2[1];
      const double& n23 = n2[2];

      double x1,x2,x3;
      if ( ::fabs(n13*n21 - n11*n23) > Vector_3::MINNORM &&
           ::fabs(n13*n21 - n11*n23) >= ::fabs(n13*n22 - n12*n23) &&
           ::fabs(n13*n21 - n11*n23) >= ::fabs(n12*n21 - n11*n22) )
      {
         x2 = 0.0;
         x1 =    ( n13*p2.Dot(n2) - n23*p1.Dot(n1)) /  (n13*n21 - n11*n23);
         x3 =    (-n11*p2.Dot(n2) + n21*p1.Dot(n1)) /  (n13*n21 - n11*n23);
      }
      else if ( ::fabs(n13*n22 - n12*n23) > Vector_3::MINNORM &&
                ::fabs(n13*n22 - n12*n23) >= ::fabs(n12*n21 - n11*n22))
      {
         x1 = 0.0;
         x2 =    ( n13*p2.Dot(n2) - n23*p1.Dot(n1)) /  (n13*n22 - n12*n23);
         x3 =    (-n12*p2.Dot(n2) + n22*p1.Dot(n1)) /  (n13*n22 - n12*n23);
      }
      else if ( ::fabs(n12*n21 - n11*n22) > Vector_3::MINNORM )
      {
         x3 = 0.0;
         x1 =    ( n12*p2.Dot(n2) - n22*p1.Dot(n1)) /  (n12*n21 - n11*n22);
         x2 =    (-n11*p2.Dot(n2) + n21*p1.Dot(n1)) /  (n12*n21 - n11*n22);
      }
      else
      {
         // something awful has happened !
         x1 = 0.0;
         x2 = 0.0;
         x3 = 0.0;
      }



      const Vector_3 pointOnLine( Vector_3( x1,x2,x3 ) );

      const Vector_3 vtestUnit = vtest.UnitV( );

      const double dot = vtestUnit.Dot( pointOnLine );

      Vector_3 v = pointOnLine - dot*vtestUnit;

#ifdef DEBUG
     if ( v.Norm( ) > 10000.0 ) {
         const int i19191 = 19191;
     }
#endif

      result = Line_3( vtest.UnitV(), v );
   }

   return( result );
}



