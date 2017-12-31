#ifndef D7DIST_CPP_H
#define D7DIST_CPP_H

#include <math.h>
#include <float.h>

/*#define D7DIST_DEBUG */
#ifdef D7DIST_DEBUG
static int changed = 0;
static double oldvalue;
#include <stdio.h>
#define D7report_double(prolog,value,epilog) \
oldvalue=value; fprintf(stderr,"%s%g%s",prolog,value,epilog);
#define D7report_integer(prolog,value,epilog) \
fprintf(stderr,"%s%d%s",prolog,value,epilog);
#define D7report_double_if_changed(prolog,value,epilog) \
changed=0; if (fabs(value-oldvalue)>1.e-8*(fabs(value)+fabs(oldvalue)+1.e-12)) {oldvalue=value; changed=1; fprintf(stderr,"%s%g%s",prolog,value,epilog);}
#define D7also_if_changed_report(prolog,value,epilog) \
if(changed) {fprintf(stderr,"%s%s%s",prolog,value,epilog);}
#define D7also_if_changed_report_integer(prolog,value,epilog) \
if(changed) {fprintf(stderr,"%s%d%s",prolog,value,epilog);}
#define D7also_if_changed_report_double(prolog,value,epilog) \
if(changed) {fprintf(stderr,"%s%g%s",prolog,value,epilog);}
#define D7also_if_changed_report_double_vector(prolog,value,epilog) \
if(changed) {fprintf(stderr,"%s[%g, %g, %g, %g, %g, %g, %g]%s",prolog,value[0],value[1],value[2],value[3],value[4],value[5],value[6],epilog);}
#define D7report_double_vector(prolog,value,epilog) \
{fprintf(stderr,"%s[%g, %g, %g, %g, %g, %g, %g]%s",prolog,value[0],value[1],value[2],value[3],value[4],value[5],value[6],epilog);}
#else
#define D7report_double(prolog,value,epilog)
#define D7report_integer(prolog,value,epilog)
#define D7report_double_if_changed(prolog,value,epilog)
#define D7also_if_changed_report(prolog,value,epilog)
#define D7also_if_changed_report_integer(prolog,value,epilog)
#define D7also_if_changed_report_double(prolog,value,epilog)
#define D7also_if_changed_report_double_vector(prolog,value,epilog)
#define D7report_double_vector(prolog,value,epilog)
#endif


#define CD7M_min(a,b) (((a)<(b))?(a):(b))
#define CD7M_max(a,b) (((a)<(b))?(b):(a))

#define ND7BND 9

#define D7P_1 0
#define D7P_2 1
#define D7P_3 2
#define D7P_4 3
#define D7P_5 4
#define D7P_6 5
#define D7P_7 6
#define D7P_8 7
#define D7P_9 8

#define D7NBDPRJ 9



inline static double d71234dotprod(double v1[7], double v2[7]);
inline static double d7eucldistsq(double v1[7], double v2[7]);
inline static double d7eucldist(double v1[7], double v2[6]);
inline static double d71234distsq(double v1[7], double v2[7]);
inline static void d7cpyvn(int n, double src[], double dst[]);
inline static void imv7(double v1[7], int m[49], double v2[7]);
inline static void rmv7(double v1[7], double m[49], double v2[7]);
inline static void d7twoPminusI(double pg[7], double g[7], double gout[7]);
inline static int d7notred(double gvec[7], int *errors);
inline static double d7bddist(double gvec[7], int bdnum);
inline static double d7minbddist(double gvec[7]);
inline static void d7bdmaps(double gvec[7],
   double dists[ND7BND],
   int iord[ND7BND],
   double pgs[ND7BND][7],
   double rgs[ND7BND][7],
   double mpgs[ND7BND][7],
   double mvecs[ND7BND][7],
   double maxdist,
   int * ngood);
inline static double D7Dist_2bds_rev(double g_lft[7], double g_rgt[7],
   double pg_lft_up[7], double mpg_lft_up[7],
   double pg_rgt_up[7], double mpg_rgt_up[7], int bd_up,
   double pg_rgt_dwn[7], double mpg_rgt_dwn[7],
   double pg_lft_dwn[7], double mpg_lft_dwn[7], int bd_dwn,
   double dist);
inline static double D7Dist_pass(double gvec1[7], double gvec2[7], double dist);
double D7Dist_cpp(double * gvec1, double * gvec2);

#endif  ///D7DIST_CPP_H