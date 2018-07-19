/*
 *  cqrlib.c
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
 * YOU MAY REDISTRIBUTE THE CQRlib API UNDER THE TERMS OF THE LGPL   *
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


#ifdef __cplusplus

extern "C" {
    
#endif
    
#include "cqrlib.h"
    
    /* CQRCreateQuaternion -- create a quaternion = w +ix+jy+kz */
    
    int CQRCreateQuaternion(CQRQuaternionHandle CQR_FAR * quaternion, double w, double x, double y, double z) {
        
        *quaternion = (CQRQuaternionHandle)CQR_MALLOC(sizeof(CQRQuaternion));
        
        if (!*quaternion) return CQR_NO_MEMORY;
        
        (*quaternion)->w = w;
        (*quaternion)->x = x;
        (*quaternion)->y = y;
        (*quaternion)->z = z;
        
        return CQR_SUCCESS;
        
    }
    
    /* CQRCreateEmptyQuaternion -- create a quaternion = 0 +i0+j0+k0 */
    
    int CQRCreateEmptyQuaternion(CQRQuaternionHandle CQR_FAR * quaternion) {
        
        *quaternion = (CQRQuaternionHandle)CQR_MALLOC(sizeof(CQRQuaternion));
        
        if (!*quaternion) return CQR_NO_MEMORY;
        
        
        (*quaternion)->w = 0;
        (*quaternion)->x = 0;
        (*quaternion)->y = 0;
        (*quaternion)->z = 0;
        
        return CQR_SUCCESS;
        
    }
    
    /* CQRFreeQuaternion -- free a quaternion  */
    
    int CQRFreeQuaternion(CQRQuaternionHandle CQR_FAR * quaternion) {
        
        if (!quaternion) return CQR_BAD_ARGUMENT;
        
        CQR_FREE(*quaternion);
        
        *quaternion = NULL;
        
        return CQR_SUCCESS;
        
    }
    
    /* CQRSetQuaternion -- create an existing quaternion = w +ix+jy+kz */
    
    int CQRSetQuaternion( CQRQuaternionHandle quaternion, double w, double x, double y, double z) {
        
        if (!quaternion) return CQR_BAD_ARGUMENT;
        
        quaternion->w = w;
        quaternion->x = x;
        quaternion->y = y;
        quaternion->z = z;
        
        return CQR_SUCCESS;
        
    }
    
    /* CQRGetQuaternionW -- get the w component of a quaternion */
    
    int CQRGetQuaternionW( double CQR_FAR * qw, CQRQuaternionHandle q ) {
        
        if (!qw || ! q) return CQR_BAD_ARGUMENT;
        
        *qw = q->w;
        
        return CQR_SUCCESS;
    
    }

    /* CQRGetQuaternionX -- get the x component of a quaternion */
    
    int CQRGetQuaternionX( double CQR_FAR * qx, CQRQuaternionHandle q ) {
        
        if (!qx || ! q) return CQR_BAD_ARGUMENT;
        
        *qx = q->x;
        
        return CQR_SUCCESS;
        
    }

    /* CQRGetQuaternionY -- get the y component of a quaternion */
    
    int CQRGetQuaternionY( double CQR_FAR * qy, CQRQuaternionHandle q ) {
        
        if (!qy || ! q) return CQR_BAD_ARGUMENT;
        
        *qy = q->y;
        
        return CQR_SUCCESS;
        
    }

    /* CQRGetQuaternionZ -- get the z component of a quaternion */
    
    int CQRGetQuaternionZ( double CQR_FAR * qz, CQRQuaternionHandle q ) {
        
        if (!qz || ! q) return CQR_BAD_ARGUMENT;
        
        *qz = q->z;
        
        return CQR_SUCCESS;
        
    }
    
    /* CQRGetQuaternionIm -- get the imaginary component of a quaternion */
    
    int CQRGetQuaternionIm( CQRQuaternionHandle quaternion, CQRQuaternionHandle q ) {
        
        if (!quaternion || ! q) return CQR_BAD_ARGUMENT;
        
        quaternion->w = 0.;
        quaternion->x = q->x;
        quaternion->y = q->y;
        quaternion->z = q->z;
        
        return CQR_SUCCESS;
        
    }
    
    /* CQRGetQuaternionAxis -- get the axis for the polar representation of a quaternion */
    
    int CQRGetQuaternionAxis( CQRQuaternionHandle quaternion, CQRQuaternionHandle q ) {
        
        double anorm;
        
        if (!quaternion || ! q) return CQR_BAD_ARGUMENT;
        
        anorm = sqrt((q->x)*(q->x)+(q->y)*(q->y)+(q->z)*(q->z));
        
        if (anorm < DBL_MIN) {
            
            quaternion->w = 0.;
            quaternion->x = 0.;
            quaternion->y = 0.;
            quaternion->z = 0.;
            
            
        } else {
        
            quaternion->w = 0.;
            quaternion->x = q->x/anorm;
            quaternion->y = q->y/anorm;
            quaternion->z = q->z/anorm;

        }
        
        return CQR_SUCCESS;
        
    }
    
    
    /* CQRGetQuaternionAngle -- get the angular component of the polar representation
       of aquaternion */
    
    double CQRGetQuaternionAngle( double CQR_FAR * angle, CQRQuaternionHandle q ) {
        
        double cosangle, sinangle;
        double radius, anorm;
        
        if (!angle || ! q) return CQR_BAD_ARGUMENT;
        
        radius = sqrt( (q->w)*(q->w) + (q->x)*(q->x) + (q->y)*(q->y) + (q->z)*(q->z) );
        anorm = sqrt( (q->x)*(q->x) + (q->y)*(q->y) + (q->z)*(q->z) );
        if ( anorm <= DBL_MIN) {
            return 0.;
        }
        cosangle = (q->w)/radius;
        sinangle = anorm/radius;
        *angle = atan2(sinangle,cosangle);
        
        return CQR_SUCCESS;
        
    }
    
    /* CQRLog -- get the natural logarithm of a quaternion */
    
    int CQRLog( CQRQuaternionHandle quaternion, CQRQuaternionHandle q ) {
        
        CQRQuaternion axis;
        double angle;
        double ipnormsq;
        double PI;
        
        if (!quaternion || ! q) return CQR_BAD_ARGUMENT;
        
        CQRGetQuaternionAxis(&axis,q);
        CQRGetQuaternionAngle(&angle,q);
        
        if (q->w < 0.) {
            ipnormsq = (q->x)*(q->x) + (q->y)*(q->y) + (q->z)*(q->z);
            if (ipnormsq <= DBL_MIN ) {
                PI = atan2(1.,1.)*4.;
                CQRMSet(axis,0.,1.,0.,0.);
                angle = PI;
            }
            
        }
        
        quaternion->w = log(sqrt((q->w)*(q->w) + (q->x)*(q->x) + (q->y)*(q->y) + (q->z)*(q->z) ));
        quaternion->x = axis.x*angle;
        quaternion->y = axis.y*angle;
        quaternion->z = axis.z*angle;
        
        return CQR_SUCCESS;
        
    }
    
    /* CQRExp -- get the exponential (exp) of a quaternion */
                            
    int CQRExp( CQRQuaternionHandle quaternion, CQRQuaternionHandle q ) {
                            
        CQRQuaternion impart;
        double angle;
        double rat;
                            
        if (!quaternion || ! q) return CQR_BAD_ARGUMENT;
                            
        CQRGetQuaternionIm(&impart,q);
        CQRMNorm(angle,impart);
        
        if (angle <= DBL_MIN) {
                            
            quaternion->w = cos(angle)*exp(q->w);
            quaternion->x = 0.;
            quaternion->y = 0.;
            quaternion->z = 0.;
            
        } else {
            
            rat = exp(q->w)*sin(angle)/angle;
            quaternion->w = cos(angle)*exp(q->w);
            quaternion->x = rat*q->x;
            quaternion->y = rat*q->y;
            quaternion->z = rat*q->z;
            
        }
        
        return CQR_SUCCESS;
                                                
    }

    /* CQRQuaternionPower -- take a quaternion to a quaternion power */
    
    int CQRQuaternionPower( CQRQuaternionHandle quaternion, CQRQuaternionHandle q, CQRQuaternionHandle p) {
        
        CQRQuaternion qlog,qlogtimesp;
        
        CQRLog(&qlog,q);
        CQRMMultiply(qlogtimesp,qlog,*p);
        CQRExp(quaternion,&qlogtimesp);
        
        return CQR_SUCCESS;
        
    }

    /* CQRDoublePower -- take a quarernion to a double power */
    
    int CQRDoublePower( CQRQuaternionHandle quaternion, CQRQuaternionHandle q, double p) {
        
        CQRQuaternion qlog,qlogtimesp;
        
        CQRLog(&qlog,q);
        CQRMScalarMultiply(qlogtimesp,qlog,p);
        CQRExp(quaternion,&qlogtimesp);
        
        return CQR_SUCCESS;
        
    }
 
    /* CQRIntegerPower -- take a quaternion to an integer power */
    
    int CQRIntegerPower( CQRQuaternionHandle quaternion, CQRQuaternionHandle q, int p) {
        
        CQRQuaternion qtemp, qsq, qprod;
        unsigned int ptemp;
        
        CQRMSet(*quaternion,1.0,0.,0.,0.);
        if ( p == 0 ) return CQR_SUCCESS;
        else if ( p > 0 ) {
            CQRMCopy(qtemp,*q);
            ptemp = p;
        } else {
            CQRMInverse(qtemp,*q);
            ptemp = -p;
        }
        while(1) {
            if ((ptemp&1)!= 0) {
                CQRMMultiply(qprod,*quaternion,qtemp);
                CQRMCopy(*quaternion,qprod);
            }
            ptemp >>= 1;
            if (ptemp==0) break;
            CQRMMultiply(qsq,qtemp,qtemp);
            CQRMCopy(qtemp,qsq)
        }
        
        return CQR_SUCCESS;
        
    }

    /* CQRIntegerRoot -- take the given integer root  of a quaternion, returning
                         the indicated mth choice from among multiple roots.
                         For reals the cycle runs through first the i-based
                         roots, then the j-based roots and then the k-based roots,
                         out of the infinite number of possible roots of reals. */
    
    int CQRIntegerRoot( CQRQuaternionHandle quaternion, CQRQuaternionHandle q, int r, int m) {
        
        
        double PI;
        CQRQuaternion qlog,qlogoverr,qaxis;
        double recip, qaxisnormsq;
        int cycle;
        
        PI  = 4.*atan2(1.,1.);
        if (r == 0) return CQR_BAD_ARGUMENT;
        recip = 1./((double)r);
        CQRLog(&qlog,q);
        
        if (m != 0) {
            CQRGetQuaternionAxis(&qaxis,q);
            CQRMNormsq(qaxisnormsq,qaxis);
            if (qaxisnormsq <= DBL_MIN) {
                cycle = (m/r)%3;
                switch (cycle) {
                    case 1: 
                        CQRMSet(qaxis,0.,0.,1.,0.);
                        break;
                    case 2: 
                        CQRMSet(qaxis,0.,0.,0.,1.);
                        break;
                    default: 
                        CQRMSet(qaxis,0.,1.,0.,0.);
                        break;
                }
            }
            CQRMScalarMultiply(qaxis,qaxis,2.*PI*((double)m));
            CQRMAdd(qlog,qlog,qaxis);
        }
        CQRMScalarMultiply(qlogoverr,qlog,recip);
        CQRExp(quaternion,&qlogoverr);
        
        return CQR_SUCCESS;
        
    }
    
    /*  CQRAdd -- add a quaternion (q1) to a quaternion (q2) */
    
    int CQRAdd (CQRQuaternionHandle quaternion,  CQRQuaternionHandle q1, CQRQuaternionHandle q2 ) {
        
        if (!quaternion || !q1 || !q2 ) return CQR_BAD_ARGUMENT;
        
        CQRMAdd(*quaternion,*q1,*q2);
        
        return CQR_SUCCESS;
        
    }
    
    /*  CQRSubtract -- subtract a quaternion (q2) from a quaternion (q1)  */
    
    int CQRSubtract (CQRQuaternionHandle quaternion,  CQRQuaternionHandle q1, CQRQuaternionHandle q2 ) {
        
        if (!quaternion || !q1 || !q2 ) return CQR_BAD_ARGUMENT;
        
        CQRMSubtract(*quaternion,*q1,*q2);
        
        return CQR_SUCCESS;
        
    }
    
    /*  CQRMultiply -- multiply a quaternion (q1) by quaternion (q2)  */
    
    int CQRMultiply (CQRQuaternionHandle quaternion,  CQRQuaternionHandle q1, CQRQuaternionHandle q2 ) {
        
        if (!quaternion || !q1 || !q2 ) return CQR_BAD_ARGUMENT;
        
        CQRMMultiply(*quaternion,*q1,*q2);
                
        return CQR_SUCCESS;
        
    }
    
    /*  CQRDot -- dot product of quaternion (q1) by quaternion (q2) as 4-vectors   */
    
    int CQRDot (double CQR_FAR * dotprod,  CQRQuaternionHandle q1, CQRQuaternionHandle q2 ) {
        
        if (!dotprod || !q1 || !q2 ) return CQR_BAD_ARGUMENT;
        
        *dotprod = q1->w*q2->w + q1->x*q2->x + q1->y*q2->y +q1->z*q2->z;
        
        return CQR_SUCCESS;
        
    }
    
    /*  CQRDivide -- divide a quaternion (q1) by quaternion (q2)  */
    
    int CQRDivide (CQRQuaternionHandle quaternion,  CQRQuaternionHandle q1, CQRQuaternionHandle q2 ) {
        
        double norm2sq;
        CQRQuaternion q;
        
        if (!quaternion || !q1 || !q2 ) return CQR_BAD_ARGUMENT;
        
        norm2sq = q2->w*q2->w+q2->x*q2->x+q2->y*q2->y+q2->z*q2->z;
        
        if (norm2sq==0.) return CQR_BAD_ARGUMENT;
        
        q.w =  q1->z*q2->z + q1->y*q2->y + q1->x*q2->x + q1->w*q2->w;
        q.x = -q1->y*q2->z + q1->z*q2->y - q1->w*q2->x + q1->x*q2->w;
        q.y =  q1->x*q2->z - q1->w*q2->y - q1->z*q2->x + q1->y*q2->w;
        q.z = -q1->w*q2->z - q1->x*q2->y + q1->y*q2->x + q1->z*q2->w;
        
        quaternion->w=q.w/norm2sq;
        quaternion->x=q.x/norm2sq;
        quaternion->y=q.y/norm2sq;
        quaternion->z=q.z/norm2sq;
        
        return CQR_SUCCESS;
        
    }
    
    /*  CQRScalarMultiply -- multiply a quaternion (q) by scalar (s)  */
    
    int CQRScalarMultiply (CQRQuaternionHandle quaternion,  CQRQuaternionHandle q, double s ) {
        
        if (!quaternion || !q ) return CQR_BAD_ARGUMENT;
        
        CQRMScalarMultiply(*quaternion,*q,s);
        
        return CQR_SUCCESS;
        
    }
    
    /*  CQREqual -- return 0 if quaternion q1 == q2  */
    
    int CQREqual (CQRQuaternionHandle q1, CQRQuaternionHandle q2 ) {

        if ( !q1 || !q2 ) return CQR_BAD_ARGUMENT;
        
        return ((q1->w==q2->w)&&(q1->x==q2->x)&&(q1->y==q2->y)&&(q1->z==q2->z))?CQR_SUCCESS:CQR_FAILED;

    }
    
    
    /*  CQRConjugate -- Form the conjugate of a quaternion qconj */
    
    int CQRConjugate (CQRQuaternionHandle qconjugate, CQRQuaternionHandle quaternion) {
        
        if (!quaternion || !qconjugate ) return CQR_BAD_ARGUMENT;
        
        CQRMConjugate(*qconjugate,*quaternion);
                
        return CQR_SUCCESS;
        
    }
    
    /*  CQRNormsq -- Form the normsquared of a quaternion */
    
    int CQRNormsq (double CQR_FAR * normsq, CQRQuaternionHandle quaternion ) {
        
        if (!quaternion || !normsq ) return CQR_BAD_ARGUMENT;
        
        CQRMNormsq(*normsq,*quaternion);
        
        return CQR_SUCCESS;
        
    }
    
    /*  CQRNorm -- Form the norm of a quaternion */
    
    int CQRNorm (double CQR_FAR * norm, CQRQuaternionHandle quaternion ) {
        
        if (!quaternion || !norm ) return CQR_BAD_ARGUMENT;
        
        CQRMNorm(*norm,*quaternion);
                
        return CQR_SUCCESS;
        
    }

    /*  CQRDistsq -- Form the distance squared between two quaternions */
    
    int CQRDistsq (double CQR_FAR * distsq, CQRQuaternionHandle q1,  CQRQuaternionHandle q2 ) {
        
        if (!q1 || !q2 || !distsq ) return CQR_BAD_ARGUMENT;
        
        CQRMDistsq(*distsq,*q1,*q2);
        
        return CQR_SUCCESS;
        
    }
    
    /*  CQRDist -- Form the distance between two quaternions */
    
    int CQRDist (double CQR_FAR * dist, CQRQuaternionHandle q1,  CQRQuaternionHandle q2) {
        
        if (!q1 || !q2 || !dist ) return CQR_BAD_ARGUMENT;
        
        CQRMDist(*dist,*q1,*q2);
        
        return CQR_SUCCESS;
        
    }
    
    /*  CQRInverse -- Form the inverse of a quaternion */
    
    int CQRInverse (CQRQuaternionHandle inversequaternion, CQRQuaternionHandle quaternion ) {
        
        double normsq;
        
        if (!quaternion || !inversequaternion ) return CQR_BAD_ARGUMENT;
        
        CQRMConjugate(*inversequaternion,*quaternion);
        CQRMNormsq(normsq,*quaternion);
        if (normsq > 0.) {
            CQRMScalarMultiply(*inversequaternion,*inversequaternion,1./normsq);
            return CQR_SUCCESS;
        } else return CQR_BAD_ARGUMENT;
        
    }
    
    /* CQRRotateByQuaternion -- Rotate a vector by a Quaternion, w = qvq* */
    
    int CQRRotateByQuaternion(double CQR_FAR * w, CQRQuaternionHandle rotquaternion, double CQR_FAR * v) {
        
        CQRQuaternion vquat, wquat, qconj;
        
        if (!w || !rotquaternion || !v ) return CQR_BAD_ARGUMENT;
        
        CQRMSet(vquat,0,v[0],v[1],v[2]);
       
        CQRMMultiply(wquat,*rotquaternion,vquat);
        CQRMConjugate(qconj,*rotquaternion);
        CQRMMultiply(vquat,wquat,qconj);
        
        w[0] = vquat.x;
        w[1] = vquat.y;
        w[2] = vquat.z;
        
        return CQR_SUCCESS;
        
    }
    
    /* CQRAxis2Quaternion -- Form the quaternion for a rotation around axis v  by angle theta */
    
    int CQRAxis2Quaternion (CQRQuaternionHandle rotquaternion, double CQR_FAR * v, double theta) {
        
        double normsq, norm;
        
        if (!rotquaternion || !v ) return CQR_BAD_ARGUMENT;
        
        normsq = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
        
        if (normsq == 0.) return CQR_BAD_ARGUMENT;
        
        if (normsq == 1.) {
            
            CQRMSet(*rotquaternion,cos(theta/2),sin(theta/2)*v[0],sin(theta/2)*v[1],sin(theta/2)*v[2]);
            
        } else {
            
            norm = sqrt(normsq);
            
            CQRMSet(*rotquaternion,cos(theta/2),sin(theta/2)*v[0]/norm,sin(theta/2)*v[1]/norm,sin(theta/2)*v[2]/norm);
        }
        
        return CQR_SUCCESS;
    }
    
    /* CQRMatrix2Quaternion -- Form the quaternion from a 3x3 rotation matrix R */
    
    int CQRMatrix2Quaternion (CQRQuaternionHandle rotquaternion, double R[3][3]) {
        
        double trace, recip, fourxsq, fourysq, fourzsq;
        
        trace = R[0][0] + R[1][1] + R[2][2];
        
        if (trace > -.75) {
            
            rotquaternion->w = sqrt((1.+trace))/2.;
            
            recip = .25/rotquaternion->w;
            
            rotquaternion->z = (R[1][0]-R[0][1])*recip;
            
            rotquaternion->y = (R[0][2]-R[2][0])*recip;
            
            rotquaternion->x = (R[2][1]-R[1][2])*recip;
            
            return CQR_SUCCESS;
            
        }
        
        fourxsq = 1.+ R[0][0] - R[1][1] - R[2][2];
        
        if (fourxsq >= .25) {
            
            rotquaternion->x = sqrt(fourxsq)/2.;
            
            recip = .25/rotquaternion->x;
            
            rotquaternion->y = (R[1][0]+R[0][1])*recip;
            
            rotquaternion->z = (R[0][2]+R[2][0])*recip;
            
            rotquaternion->w = (R[2][1]-R[1][2])*recip;
            
            return CQR_SUCCESS;
            
        }
        
        fourysq = 1.+ R[1][1] - R[0][0] - R[2][2];
        
        if (fourysq >= .25) {
            
            rotquaternion->y = sqrt(fourysq)/2.;
            
            recip = .25/rotquaternion->y;
            
            rotquaternion->x = (R[1][0]+R[0][1])*recip;
            
            rotquaternion->w = (R[0][2]-R[2][0])*recip;
            
            rotquaternion->z = (R[2][1]+R[1][2])*recip;
            
            return CQR_SUCCESS;
            
        }
        
        fourzsq = 1.+ R[2][2] - R[0][0] - R[1][1];
        
        if (fourzsq >= .25) {
            
            rotquaternion->z = sqrt(fourzsq)/2.;
            
            recip = .25/rotquaternion->z;
            
            rotquaternion->w = (R[1][0]-R[0][1])*recip;
            
            rotquaternion->x = (R[0][2]+R[2][0])*recip;
            
            rotquaternion->y = (R[2][1]+R[1][2])*recip;
            
            return CQR_SUCCESS;
            
        }
        
        return CQR_BAD_ARGUMENT;
        
    }
    
    /* CQRQuaternion2Matrix -- Form the 3x3 rotation matrix from a quaternion */
    
    int CQRQuaternion2Matrix (double R[3][3], CQRQuaternionHandle rotquaternion) {
        
        double twoxy, twoyz, twoxz, twowx, twowy, twowz;
        double ww, xx, yy, zz;
        
        if (!R || !rotquaternion) return CQR_BAD_ARGUMENT;
        
        ww = (rotquaternion->w)*(rotquaternion->w);
        xx = (rotquaternion->x)*(rotquaternion->x);
        yy = (rotquaternion->y)*(rotquaternion->y);
        zz = (rotquaternion->z)*(rotquaternion->z);
        
        R[0][0] = ww + xx - yy - zz;
        R[1][1] = ww - xx + yy - zz;
        R[2][2] = ww - xx - yy + zz;
        
        twoxy = 2.*(rotquaternion->x)*(rotquaternion->y);
        twoyz = 2.*(rotquaternion->y)*(rotquaternion->z);
        twoxz = 2.*(rotquaternion->x)*(rotquaternion->z);
        twowx = 2.*(rotquaternion->w)*(rotquaternion->x);
        twowy = 2.*(rotquaternion->w)*(rotquaternion->y);
        twowz = 2.*(rotquaternion->w)*(rotquaternion->z);
        
        R[0][1] = twoxy - twowz;
        R[0][2] = twoxz + twowy;
        R[1][0] = twoxy + twowz;
        R[1][2] = twoyz - twowx;
        R[2][0] = twoxz - twowy;  
        R[2][1] = twoyz + twowx;
        
        return CQR_SUCCESS;
    }
    
    /* CQRQuaternion2Angles -- Convert a Quaternion into Euler Angles for Rz(Ry(Rx))) convention */
    
    int CQRQuaternion2Angles (double CQR_FAR * RotX, double CQR_FAR * RotY, double CQR_FAR * RotZ, CQRQuaternionHandle rotquaternion) {
        
        double SRX, SRY, SRZ, TRX, TRY, TRZ;
        double NSum;
        double TSum;
        
        
        double RMX0, RMX1, RMY0, RMY1, RMZ0, RMZ1, RMZ2;
        
        double PI;
        
        PI = 4.*atan2(1.,1.);
        
        if (!rotquaternion || !RotX || !RotY || !RotZ) return CQR_BAD_ARGUMENT;
        
        RMX0 = rotquaternion->w*rotquaternion->w + rotquaternion->x*rotquaternion->x
        - rotquaternion->y*rotquaternion->y - rotquaternion->z*rotquaternion->z;
        RMX1 = 2.*(rotquaternion->x*rotquaternion->y-rotquaternion->w*rotquaternion->z);
        RMY0 = 2.*(rotquaternion->x*rotquaternion->y+rotquaternion->w*rotquaternion->z);
        RMY1 = rotquaternion->w*rotquaternion->w - rotquaternion->x*rotquaternion->x
        + rotquaternion->y*rotquaternion->y - rotquaternion->z*rotquaternion->z;
        RMZ0 = 2.*(rotquaternion->x*rotquaternion->z-rotquaternion->w*rotquaternion->y);
        RMZ1 = 2.*(rotquaternion->w*rotquaternion->x+rotquaternion->y*rotquaternion->z);
        RMZ2 = rotquaternion->w*rotquaternion->w - rotquaternion->x*rotquaternion->x
        - rotquaternion->y*rotquaternion->y + rotquaternion->z*rotquaternion->z;
        
        if (RMZ0 < 1. ) {
            if (RMZ0 > -1.) {
                SRY = asin(-RMZ0);
            } else {
                SRY = .5*PI;
            }
        } else {
            SRY = -.5*PI;
        } 
        if (RMZ0 > .9999995) {
            SRX = atan2(-RMX1,RMY1);
            SRZ = 0;
        } else {
            if (RMZ0 < -.9999995 ) {
                SRX = atan2(RMX1,RMY1);
                SRZ = 0;
            } else {
                SRX = atan2(RMZ1,RMZ2);
                SRZ = atan2(RMY0,RMX0);
            }
        }

        TRX = PI+SRX;
        if ( TRX > 2.*PI ) TRX -= 2.*PI;
        TRY = PI+SRY;
        if ( TRY > 2.*PI ) TRY -= 2.*PI;
        TRZ = PI+SRZ;
        if ( TRZ > 2.*PI ) TRZ -= 2.*PI;

        NSum = 0;
        TSum = 0;
        NSum += fabs(cos(SRX)-cos(*RotX)) + fabs(sin(SRX)-sin(*RotX))
        + fabs(cos(SRY)-cos(*RotY)) + fabs(sin(SRY)-sin(*RotY))
        + fabs(cos(SRZ)-cos(*RotZ)) + fabs(sin(SRZ)-sin(*RotZ));
        TSum += fabs(cos(TRX)-cos(*RotX)) + fabs(sin(TRX)-sin(*RotX))
        + fabs(cos(TRY)-cos(*RotY)) + fabs(sin(TRY)-sin(*RotY))
        + fabs(cos(TRZ)-cos(*RotZ)) + fabs(sin(TRZ)-sin(*RotZ));
        
        if (NSum < TSum) {
            *RotX = SRX; *RotY = SRY; *RotZ = SRZ;
        } else {
            *RotX = TRX; *RotY = TRY; *RotZ = TRZ;
        }
        
        return CQR_SUCCESS;
    }
    
    /* CQRAngles2Quaternion -- Convert Euler Angles for Rz(Ry(Rx))) convention into a quaternion */
    
    int CQRAngles2Quaternion (CQRQuaternionHandle rotquaternion, double RotX, double RotY, double RotZ ) {
        
        double cx, cy, cz, sx, sy, sz;
        
        if (!rotquaternion) return CQR_BAD_ARGUMENT;
        
        cx = cos(RotX/2);
        sx = sin(RotX/2);
        
        cy = cos(RotY/2);
        sy = sin(RotY/2);
        
        cz = cos(RotZ/2);
        sz = sin(RotZ/2);
        
        rotquaternion->w = cx*cy*cz + sx*sy*sz;
        rotquaternion->x = sx*cy*cz - cx*sy*sz;
        rotquaternion->y = cx*sy*cz + sx*cy*sz;
        rotquaternion->z = cx*cy*sz - sx*sy*cz;
        
        return CQR_SUCCESS;
        
    }
    
    /* Represent a 3-vector as a quaternion with w=0 */
    
    int CQRPoint2Quaternion( CQRQuaternionHandle quaternion, double v[3] )
    {
        quaternion->w = 0.;
        quaternion->x = v[0];
        quaternion->y = v[1];
        quaternion->z = v[2];
        return CQR_SUCCESS;

    }
    
    
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
                  const double w1, const double w2)
    {
        CQRQuaternion s1;
        CQRQuaternion s2;
        CQRQuaternion st1;
        CQRQuaternion st2;
        CQRQuaternion sout;
        CQRQuaternion sout2;
        double normsq, norm1sq, norm2sq;
        double r1,r2, cosomega,sinomega, omega;
        double t, t1, t2;
        
        if (!quaternion || !q1 || !q2 ) return CQR_BAD_ARGUMENT;

        CQRMNormsq(norm1sq,*q1);
        CQRMNormsq(norm2sq,*q2);        
        
        t = w1/(w1+w2);
        
        if (norm1sq <= DBL_MIN) {
            CQRMScalarMultiply(*quaternion,*q2,(1-t));
            return CQR_SUCCESS;
        }
        
        if (norm2sq <= DBL_MIN) {
            CQRMScalarMultiply(*quaternion,*q1,t);
            return CQR_SUCCESS;
        }
        
        if (fabs(norm1sq-1.)<= DBL_MIN) {
            r1 = 1.;
            CQRMCopy(s1,*q1);
        } else {
            r1 = sqrt(norm1sq);
            CQRMScalarMultiply(s1,*q1,(1/r1));
        }
        
        if (fabs(norm2sq-1.)<= DBL_MIN) {
            r2 = 1.;
            CQRMCopy(s2,*q2);
        } else {
            r2 = sqrt(norm1sq);
            CQRMScalarMultiply(s2,*q2,(1/r2));
        }
        
        CQRMDot(cosomega,s1,s2);
        if (cosomega>=1. || cosomega<=-1.) {
            sinomega = 0.;
        } else {
            sinomega=sqrt(1.-cosomega*cosomega);
        }
        
        omega=atan2(sinomega,cosomega);
        
        if (sinomega <= 0.05) {
            t1=t*(1-t*t*omega*omega/6.);
            t2=(1-t)*(1.-(1-t)*(1-t)*omega*omega/6.);
            CQRMScalarMultiply(st1,s1,t1);
            CQRMScalarMultiply(st2,s2,t2);
            if (cosomega >=0.) {
                CQRMAdd(sout,st1,st2);
            } else {
                if (sinomega <= 0.00001) {
                    sout.w = -st1.x+st2.x;
                    sout.x =  st1.w-st2.w;
                    sout.y =  st1.z-st2.z;
                    sout.z = -st1.y+st2.y;
                } else {
                    CQRMAdd(sout,s1,s2);
                }
                CQRMNormsq(normsq,sout);
                CQRMScalarMultiply(sout,sout,1/sqrt(normsq));
                if (t >= 0.5) {
                    CQRSLERP(&sout2,&sout,&s1,2-2.*t,2.*t-1.);
                    CQRMCopy(sout,sout2);
                }else {
                    CQRSLERP(&sout2,&sout,&s2,2.*t,1.-2.*t-1.);
                    CQRMCopy(sout,sout2);
               }
            }
            CQRMNormsq(normsq,sout);
            if (normsq <= DBL_MIN) {
                CQRMSet(*quaternion,0.,0.,0.,0.);
            } else {
                CQRMScalarMultiply(*quaternion,sout,(t*r1+(1-t)*r2)/sqrt(normsq));
            }
            return CQR_SUCCESS;
        }
        
        t1 = sin(t*omega);
        t2 = sin((1-t)*omega);
        CQRMScalarMultiply(st1,s1,t1);
        CQRMScalarMultiply(st2,s2,t2);
        CQRMAdd(sout,st1,st2);
        CQRMNormsq(normsq,sout);
        CQRMScalarMultiply(*quaternion,sout,(r1*t+r2*(1-t))/sqrt(normsq));
        return CQR_SUCCESS;        
    }
    
    
    /*  HLERP -- Hemispherical Linear Interpolation   
     Take two quaternions and two weights and combine them
     following a great circle on the unit quaternion 4-D sphere
     and linear interpolation between the radii
     
     This is the hemispherical version, for use with quaternions
     representing rotations.  Use SLERP for full
     spherical interpolation.
     
     */
    
    int CQRHLERP (CQRQuaternionHandle quaternion, const CQRQuaternionHandle q1, const CQRQuaternionHandle q2,
                  const double w1, const double w2)
    {
        CQRQuaternion s1;
        CQRQuaternion s2;
        CQRQuaternion st1;
        CQRQuaternion st2;
        CQRQuaternion sout;
        double normsq, norm1sq, norm2sq;
        double r1,r2, cosomega,sinomega, omega;
        double t, t1, t2;
        
        if (!quaternion || !q1 || !q2 ) return CQR_BAD_ARGUMENT;

        CQRMNormsq(norm1sq,*q1);
        CQRMNormsq(norm2sq,*q2);        
        
        t = w1/(w1+w2);
        
        if (norm1sq <= DBL_MIN) {
            CQRMScalarMultiply(*quaternion,*q2,(1-t));
            return CQR_SUCCESS;
        }
        
        if (norm2sq <= DBL_MIN) {
            CQRMScalarMultiply(*quaternion,*q1,t);
            return CQR_SUCCESS;
        }
        
        if (fabs(norm1sq-1.)<= DBL_MIN) {
            r1 = 1.;
            CQRMCopy(s1,*q1);
        } else {
            r1 = sqrt(norm1sq);
            CQRMScalarMultiply(s1,*q1,(1/r1));
        }
        
        if (fabs(norm2sq-1.)<= DBL_MIN) {
            r2 = 1.;
            CQRMCopy(s2,*q2);
        } else {
            r2 = sqrt(norm1sq);
            CQRMScalarMultiply(s2,*q2,(1/r2));
        }
        
        CQRMDot(cosomega,s1,s2);
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
            CQRMScalarMultiply(st1,s1,t1);
            CQRMScalarMultiply(st2,s2,t2);
            CQRMAdd(sout,st1,st2);
            if (sout.w < 0.) {
                sout.w = -sout.w;
                sout.x = -sout.x;
                sout.y = -sout.y;
                sout.z = -sout.z;
            }
            CQRMNormsq(normsq,sout);
            if (normsq <= DBL_MIN) {
                CQRMSet(*quaternion,0.,0.,0.,0.);
            } else {
                CQRMScalarMultiply(*quaternion,sout,(t*r1+(1-t)*r2)/sqrt(normsq));
            }
            return CQR_SUCCESS;
        }
        
        t1 = sin(t*omega);
        t2 = sin((1-t)*omega);
        CQRMScalarMultiply(st1,s1,t1);
        CQRMScalarMultiply(st2,s2,t2);
        CQRMAdd(sout,st1,st2);
        if (sout.w < 0.) {
            sout.w = -sout.w;
            sout.x = -sout.x;
            sout.y = -sout.y;
            sout.z = -sout.z;
        }
        CQRMNormsq(normsq,sout);
        CQRMScalarMultiply(*quaternion,sout,(r1*t+r2*(1-t))/sqrt(normsq));
        return CQR_SUCCESS;                
    }
    
    /*  CQRSLERPDist -- Spherical Linear Interpolation distance
     Form the distance between two quaternions by summing
     the difference in the magnitude of the radii and
     the great circle distance along the sphere of the
     smaller quaternion.
     
     This version keeps a quaternion separate from the negative
     of the same quaternion and is not appropriate for
     quaternions representing rotations.  Use CQRHLERPDist
     to apply SLERPDist to quaternions representing rotations
     */
    
    int CQRSLERPDist (double CQR_FAR * dist, const CQRQuaternionHandle q1, const CQRQuaternionHandle q2)
    {
        CQRQuaternion s1;
        CQRQuaternion s2;
        double norm1sq, norm2sq;
        double r1,r2, cosomega,sinomega, omega;

        
        if (!dist || !q1 || !q2) return CQR_BAD_ARGUMENT;
        
        CQRMNormsq(norm1sq,*q1);
        CQRMNormsq(norm2sq,*q2);        

        if (norm1sq <= DBL_MIN) {*dist = sqrt(norm2sq); return CQR_SUCCESS;}
        
        if (norm2sq <= DBL_MIN) {*dist = sqrt(norm1sq); return CQR_SUCCESS;}
        
        if (fabs(norm1sq-1.)<= DBL_MIN) {
            r1 = 1.;
            CQRMCopy(s1,*q1);
        } else {
            r1 = sqrt(norm1sq);
            CQRMScalarMultiply(s1,*q1,(1/r1));
        }
        
        if (fabs(norm2sq-1.)<= DBL_MIN) {
            r2 = 1.;
            CQRMCopy(s2,*q2);
        } else {
            r2 = sqrt(norm1sq);
            CQRMScalarMultiply(s2,*q2,(1/r2));
        }
        
        CQRMDot(cosomega,s1,s2);
        if (cosomega>=1. || cosomega<=-1.) {
            sinomega = 0.;
        } else {
            sinomega=sqrt(1.-cosomega*cosomega);
        }
        
        omega=atan2(sinomega,cosomega);
        if (r1 <= r2) *dist = (r2-r1)+r1*fabs(omega);
        else *dist = (r1-r2)+r2*fabs(omega);
        
        return CQR_SUCCESS;
        
    }
    
    /*  CQRHLERPDist -- Hemispherical Linear Interpolation distance
     Form the distance between two quaternions by summing
     the difference in the magnitude of the radii and
     the great circle distance along the sphere of the
     smaller quaternion.
     
     This version keeps a quaternion separate from the negative
     of the same quaternion and is not appropriate for
     quaternions representing rotations.  Use CQRHLERPDist
     to apply SLERPDist to quaternions representing rotations
     */
    
    int CQRHLERPDist (double CQR_FAR * dist, const CQRQuaternionHandle q1, const CQRQuaternionHandle q2)
    {
        CQRQuaternion s1;
        CQRQuaternion s2;
        double norm1sq, norm2sq;
        double r1,r2, cosomega,sinomega, omega;
        
        if (!dist || !q1 || !q2) return CQR_BAD_ARGUMENT;

        CQRMNormsq(norm1sq,*q1);
        CQRMNormsq(norm2sq,*q2);        
        
        if (norm1sq <= DBL_MIN) {*dist = sqrt(norm2sq); return CQR_SUCCESS;}
        
        if (norm2sq <= DBL_MIN) {*dist = sqrt(norm1sq); return CQR_SUCCESS;}
        
        if (fabs(norm1sq-1.)<= DBL_MIN) {
            r1 = 1.;
            CQRMCopy(s1,*q1);
        } else {
            r1 = sqrt(norm1sq);
            CQRMScalarMultiply(s1,*q1,(1/r1));
        }
        
        if (fabs(norm2sq-1.)<= DBL_MIN) {
            r2 = 1.;
            CQRMCopy(s2,*q2);
        } else {
            r2 = sqrt(norm1sq);
            CQRMScalarMultiply(s2,*q2,(1/r2));
        }
        
        CQRMDot(cosomega,s1,s2);
        if (cosomega>=1. || cosomega<=-1.) {
            sinomega = 0.;
        } else {
            sinomega=sqrt(1.-cosomega*cosomega);
        }
        
        if (cosomega < 0.) {
             cosomega = -cosomega;
        }
        
        omega=atan2(sinomega,cosomega);
        if (r1 <= r2) *dist = (r2-r1)+r1*fabs(omega);
        else *dist = (r1-r2)+r2*fabs(omega);
        
        return CQR_SUCCESS;
    }
    
    
    
#ifdef __cplusplus
    
}

#endif
