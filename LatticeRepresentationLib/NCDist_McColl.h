//#pragma once
//// NCDist_headeronly_fixed_final.h -- header-only C++11 refactor for phasertng
//// - All helpers are inline; no macros, no globals, thread-safe.
//// - Full constant tables embedded as static const.
//// - Ready for parallelization (split the reflection loop in NCDist).
//
//#include <cmath>
//#include <cfloat>
//#include <algorithm>
//#include <cstring> // for std::memcpy
//
//namespace phasertng {
//
//// ---------------- Constants ----------------
//#ifndef NBND
//static constexpr int NBND  = 15;
//static constexpr int NREFL = 24;
//#endif
//
//#ifndef NCDIST_NO_OUTER_PASS
//static constexpr int NREFL_OUTER_FULL = 24;
//static constexpr int NREFL_OUTER_MIN  = 6;
//#else
//static constexpr int NREFL_OUTER_FULL = 8;
//static constexpr int NREFL_OUTER_MIN  = 1;
//#endif
//
//// Boundary IDs used by bddist (1..15)
//enum BoundaryID {
//  P_1=1, P_2, P_3, P_4, P_5, P_6, P_7, P_8, P_9, P_A, P_B, P_C, P_D, P_E, P_F
//};
//
//// ---------------- Helpers (macro replacements) ----------------
//template<typename T> inline T cncm_min(T a,T b){return (a<b)?a:b;}
//template<typename T> inline T cncm_max(T a,T b){return (a<b)?b:a;}
//template<typename T> inline T CNCM_min(T a,T b){return cncm_min(a,b);} // shim for legacy name
//template<typename T> inline T CNCM_max(T a,T b){return cncm_max(a,b);} // shim for legacy name
//inline double cncm_min3(double a,double b,double c){ return std::min(a,std::min(b,c)); }
//
//inline int CNCM_ctop(double comp){ return (comp > -1.0e-38)?1:-1; }
//inline int CNCM_ctom(double comp){ return (comp <  1.0e-38)?-1:1; }
//
//inline int CNCM_vtoppp(const double v[6]){ return CNCM_ctop(v[3]) * CNCM_ctop(v[4]) * CNCM_ctop(v[5]); }
//inline int CNCM_vtommm(const double v[6]){ return CNCM_ctom(v[3]) * CNCM_ctom(v[4]) * CNCM_ctom(v[5]); }
//
//inline double CNCM_normsq(const double v[6]){
//    return v[0]*v[0]+v[1]*v[1]+v[2]*v[2]+v[3]*v[3]+v[4]*v[4]+v[5]*v[5];
//}
//inline double CNCM_norm(const double v[6]){ return std::sqrt(CNCM_normsq(v)); }
//
//inline double CNCM_gdistsq(const double a[6], const double b[6]){
//    return (a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]) + (a[2]-b[2])*(a[2]-b[2]) +
//           (a[3]-b[3])*(a[3]-b[3]) + (a[4]-b[4])*(a[4]-b[4]) + (a[5]-b[5])*(a[5]-b[5]);
//}
//inline double CNCM_gdist(const double a[6], const double b[6]){ return std::sqrt(CNCM_gdistsq(a,b)); }
//
//// Unreduced unit cell acceptance test
//inline int isunc(const double v[6]) {
//    const double fz = 1.0e-7;
//    if (v[0]<0. || v[1]<0. || v[2]<0.) return 0;
//    double maxedge = std::max(v[0], std::max(v[1], v[2]));
//    if ( v[3]>v[1]+maxedge*fz || v[3]>v[2]+maxedge*fz ||
//         v[4]>v[0]+maxedge*fz || v[4]>v[2]+maxedge*fz ||
//         v[5]>v[0]+maxedge*fz || v[5]>v[1]+maxedge*fz) return 0;
//    if ( v[3]<-maxedge*(1.-fz) || v[4]<-maxedge*(1.-fz) || v[5]<-maxedge*(1.-fz)) return 0;
//    if (v[0]+v[1]+v[2]+v[3]+v[4]+v[5] < maxedge*(1.-fz)) return 0;
//    if (v[0]+v[1]+v[2]+v[3]-v[4]-v[5] < maxedge*(1.-fz)) return 0;
//    if (v[0]+v[1]+v[2]-v[3]+v[4]-v[5] < maxedge*(1.-fz)) return 0;
//    if (v[0]+v[1]+v[2]-v[3]-v[4]+v[5] < maxedge*(1.-fz)) return 0;
//    return 1;
//}
//
//// 6x6 integer matrix times 6-vector
//inline void imv6(const double v1[6], const int m[36], double v2[6]) {
//    for (int i=0;i<6;++i){ double sum=0.0; for (int j=0;j<6;++j) sum += double(m[6*i+j])*v1[j]; v2[i]=sum; }
//}
//
//// GP/M distance (FASTEST path for CNCM_gtestdistsq)
//inline double CNCM_gpmdistsq(const double v1[6], const double v2[6]){
//    if (CNCM_vtoppp(v1)*CNCM_vtoppp(v2)>0 && CNCM_vtommm(v1)*CNCM_vtommm(v2)>0) {
//        return CNCM_gdistsq(v1,v2);
//    }
//    double t0 =  std::fabs(v1[3]*v2[3]) - (v1[4]*v2[4]) - (v1[5]*v2[5]);
//    double t1 = -(v1[3]*v2[3]) + std::fabs(v1[4]*v2[4]) - (v1[5]*v2[5]);
//    double t2 = -(v1[3]*v2[3]) - (v1[4]*v2[4]) + std::fabs(v1[5]*v2[5]);
//    double mix = 2.0 * cncm_min3(std::fabs(t0), std::fabs(t1), std::fabs(t2));
//    return (v1[0]-v2[0])*(v1[0]-v2[0]) + (v1[1]-v2[1])*(v1[1]-v2[1]) + (v1[2]-v2[2])*(v1[2]-v2[2]) +
//           v1[3]*v1[3] + v2[3]*v2[3] + v1[4]*v1[4] + v2[4]*v2[4] + v1[5]*v1[5] + v2[5]*v2[5] + mix;
//}
//inline double CNCM_gtestdistsq(const double v1[6], const double v2[6]){ return CNCM_gpmdistsq(v1,v2); }
//inline double CNCM_gtestdist(const double v1[6], const double v2[6]){ return std::sqrt(CNCM_gtestdistsq(v1,v2)); }
//
//// Coupled/uncoupled metric distance used by NCDist
//inline double guncpmdistsq(const double v1[6], const double v2[6]) {
//    if (!isunc(v1) || !isunc(v2)) return CNCM_gdistsq(v1, v2);
//    // choose which is plus vs minus region and evaluate gpmdist
//    const double *plusvec, *minusvec;
//    if (CNCM_vtoppp(v1) > 0 && CNCM_vtommm(v1) > 0) { plusvec = v1; minusvec = v2; }
//    else if (CNCM_vtoppp(v2) > 0 && CNCM_vtommm(v2) > 0) { plusvec = v2; minusvec = v1; }
//    else { plusvec = v1; minusvec = v2; }
//    return CNCM_gtestdistsq(plusvec, minusvec);
//}
//inline double guncpmdist(const double v1[6], const double v2[6]){ return std::sqrt(guncpmdistsq(v1,v2)); }
//
//// Boundary distance
//inline double bddist(const double gvec[6], int bdnum) {
//    double bdin = 1.0;
//    switch (bdnum) {
//      case P_1: if (gvec[0] > gvec[1]) bdin = -1; break;
//      case P_2: if (gvec[1] > gvec[2]) bdin = -1; break;
//      case P_3: if (gvec[3] > 0.) bdin = -1; break;
//      case P_4: if (gvec[4] > 0.) bdin = -1; break;
//      case P_5: if (gvec[5] > 0.) bdin = -1; break;
//      case P_6: if (gvec[3] > gvec[1]) bdin = -1; break;
//      case P_7: if (gvec[3] > gvec[1]) bdin = -1; break;
//      case P_8: if (-gvec[3] > gvec[1]) bdin = -1; break;
//      case P_9: if (gvec[4] > gvec[0]) bdin = -1; break;
//      case P_A: if (gvec[4] > gvec[0]) bdin = -1; break;
//      case P_B: if (-gvec[4] > gvec[0]) bdin = -1; break;
//      case P_C: if (gvec[5] > gvec[0]) bdin = -1; break;
//      case P_D: if (gvec[5] > gvec[0]) bdin = -1; break;
//      case P_E: if (-gvec[5] > gvec[0]) bdin = -1; break;
//      case P_F: if (gvec[0] + gvec[1] + gvec[3] + gvec[4] + gvec[5] < 0.) bdin = -1; break;
//      default: break;
//    }
//    return bdin;
//}
//
//inline double minbddist(const double gvec[6]) {
//    double d0 = std::min({std::fabs(gvec[1]-gvec[0]), std::fabs(gvec[2]-gvec[1]), std::fabs(gvec[2]-gvec[0])}) / std::sqrt(2.);
//    double d1 = std::min({std::fabs(gvec[3]), std::fabs(gvec[4]), std::fabs(gvec[5])});
//    double d2 = std::fabs(gvec[0]+gvec[1]+gvec[3]+gvec[4]+gvec[5]) / std::sqrt(5.);
//    return std::min(d0, std::min(d1, d2));
//}
//
//// Fudge factor
//static constexpr double DCUT = 0.9995;
//inline double fudge(double d){ return DCUT*d; }
//
//// Projector application helper: out = 2*P*v - v
//inline void map_to_boundary_image(const double pg[6], const double g[6], double gout[6]) {
//    for (int i=0;i<6;++i) gout[i] = 2.0*pg[i] - g[i];
//}
//
//// ---------------- Core pass ----------------
//inline double NCDist_pass(const double gvec1[6], const double gvec2[6], double dist) {
//    const double mind1 = minbddist(gvec1);
//    const double mind2 = minbddist(gvec2);
//    if (mind1 + mind2 >= fudge(dist)) return dist;
//    return guncpmdist(gvec1, gvec2);
//}
//
//// ---------------- Full tables (const; header-safe) ----------------
//static const int rord[NREFL] = {0,1,2,3,4,5,9,12,14,19,15,13,20,6,8,10,11,16,17,18,21,22,23,7};
//
//
//static const int RS[NREFL][36] = {
//    /*  R_0:M_ident;     */
//    {1,0,0,0,0,0,
//     0,1,0,0,0,0,
//     0,0,1,0,0,0,
//     0,0,0,1,0,0,
//     0,0,0,0,1,0,
//     0,0,0,0,0,1},
//    /*  R_1:M_1;         */
//    {0,1,0,0,0,0,
//     1,0,0,0,0,0,
//     0,0,1,0,0,0,
//     0,0,0,0,1,0,
//     0,0,0,1,0,0,
//     0,0,0,0,0,1},
//    /*  R_2:M_2;         */
//    {1,0,0,0,0,0,
//     0,0,1,0,0,0,
//     0,1,0,0,0,0,
//     0,0,0,1,0,0,
//     0,0,0,0,0,1,
//     0,0,0,0,1,0},
//    /*  R_3:M_2.M_1;     */
//    {0,1,0,0,0,0,
//     0,0,1,0,0,0,
//     1,0,0,0,0,0,
//     0,0,0,0,1,0,
//     0,0,0,0,0,1,
//     0,0,0,1,0,0},
//    /*  R_4:M_1.M_2;     */
//    {0,0,1,0,0,0,
//     1,0,0,0,0,0,
//     0,1,0,0,0,0,
//     0,0,0,0,0,1,
//     0,0,0,1,0,0,
//     0,0,0,0,1,0},
//    /*  R_5:M_2.M_1.M_2; */
//    {0,0,1,0,0,0,
//     0,1,0,0,0,0,
//     1,0,0,0,0,0,
//     0,0,0,0,0,1,
//     0,0,0,0,1,0,
//     0,0,0,1,0,0},
//    /*  R_6:M_3.R_0;     */
//    {1,0,0,0,0,0,
//     0,1,0,0,0,0,
//     0,0,1,0,0,0,
//     0,0,0,1,0,0,
//     0,0,0,0,-1,0,
//     0,0,0,0,0,-1},
//    /*  R_7:M_3.R_1;     */
//    {0,1,0,0,0,0,
//     1,0,0,0,0,0,
//     0,0,1,0,0,0,
//     0,0,0,0,1,0,
//     0,0,0,-1,0,0,
//     0,0,0,0,0,-1},
//    /*  R_8:M_3.R_2;     */
//    {1,0,0,0,0,0,
//     0,0,1,0,0,0,
//     0,1,0,0,0,0,
//     0,0,0,1,0,0,
//     0,0,0,0,0,-1,
//     0,0,0,0,-1,0},
//    /*  R_9:M_3.R_3;     */
//    {0,1,0,0,0,0,
//     0,0,1,0,0,0,
//     1,0,0,0,0,0,
//     0,0,0,0,1,0,
//     0,0,0,0,0,-1,
//     0,0,0,-1,0,0},
//    /*  R_10:M_3.R_4;    */
//    {0,0,1,0,0,0,
//     1,0,0,0,0,0,
//     0,1,0,0,0,0,
//     0,0,0,0,0,1,
//     0,0,0,-1,0,0,
//     0,0,0,0,-1,0},
//    /*  R_11:M_3.R_5;    */
//    {0,0,1,0,0,0,
//     0,1,0,0,0,0,
//     1,0,0,0,0,0,
//     0,0,0,0,0,1,
//     0,0,0,0,-1,0,
//     0,0,0,-1,0,0},
//    /*  R_12:M_4.R_0;    */
//    {1,0,0,0,0,0,
//     0,1,0,0,0,0,
//     0,0,1,0,0,0,
//     0,0,0,-1,0,0,
//     0,0,0,0,1,0,
//     0,0,0,0,0,-1},
//    /*  R_13:M_4.R_1;    */
//    {0,1,0,0,0,0,
//     1,0,0,0,0,0,
//     0,0,1,0,0,0,
//     0,0,0,0,-1,0,
//     0,0,0,1,0,0,
//     0,0,0,0,0,-1},
//    /*  R_14:M_4.R_2;    */
//    {1,0,0,0,0,0,
//     0,0,1,0,0,0,
//     0,1,0,0,0,0,
//     0,0,0,-1,0,0,
//     0,0,0,0,0,1,
//     0,0,0,0,-1,0},
//    /*  R_15:M_4.R_3;    */
//    {0,1,0,0,0,0,
//     0,0,1,0,0,0,
//     1,0,0,0,0,0,
//     0,0,0,0,-1,0,
//     0,0,0,0,0,1,
//     0,0,0,-1,0,0},
//    /*  R_16:M_4.R_4;    */
//    {0,0,1,0,0,0,
//     1,0,0,0,0,0,
//     0,1,0,0,0,0,
//     0,0,0,0,0,-1,
//     0,0,0,1,0,0,
//     0,0,0,0,-1,0},
//    /*  R_17:M_4.R_5;    */
//    {0,0,1,0,0,0,
//     0,1,0,0,0,0,
//     1,0,0,0,0,0,
//     0,0,0,0,0,-1,
//     0,0,0,0,1,0,
//     0,0,0,-1,0,0},
//    /*  R_18:M_5.R_0;    */
//    {1,0,0,0,0,0,
//     0,1,0,0,0,0,
//     0,0,1,0,0,0,
//     0,0,0,-1,0,0,
//     0,0,0,0,-1,0,
//     0,0,0,0,0,1},
//    /*  R_19:M_5.R_1;    */
//    {0,1,0,0,0,0,
//     1,0,0,0,0,0,
//     0,0,1,0,0,0,
//     0,0,0,0,-1,0,
//     0,0,0,-1,0,0,
//     0,0,0,0,0,1},
//    /*  R_20:M_5.R_2;    */
//    {1,0,0,0,0,0,
//     0,0,1,0,0,0,
//     0,1,0,0,0,0,
//     0,0,0,-1,0,0,
//     0,0,0,0,0,-1,
//     0,0,0,0,1,0},
//    /*  R_21:M_5.R_3;    */
//    {0,1,0,0,0,0,
//     0,0,1,0,0,0,
//     1,0,0,0,0,0,
//     0,0,0,0,-1,0,
//     0,0,0,0,0,-1,
//     0,0,0,1,0,0},
//    /*  R_22:M_5.R_4;    */
//    {0,0,1,0,0,0,
//     1,0,0,0,0,0,
//     0,1,0,0,0,0,
//     0,0,0,0,0,-1,
//     0,0,0,-1,0,0,
//     0,0,0,0,1,0},
//    /*  R_23:M_5.R_5;    */
//    {0,0,1,0,0,0,
//     0,1,0,0,0,0,
//     1,0,0,0,0,0,
//     0,0,0,0,0,-1,
//     0,0,0,0,-1,0,
//     0,0,0,1,0,0}
//    
//};
//
//static const double prj[36][36]={{0}};
//
//// ---------------- Top-level distance ----------------
//inline double NCDist(const double gvec1[6], const double gvec2[6]) {
//    double dist1 = minbddist(gvec1);
//    double dist2 = minbddist(gvec2);
//    double dist  = guncpmdist(gvec1, gvec2);
//    dist = NCDist_pass(gvec1, gvec2, dist);
//
//    const double lgv1 = CNCM_norm(gvec1);
//    const double lgv2 = CNCM_norm(gvec2);
//    (void)lgv1; (void)lgv2; // kept for parity with legacy heuristic
//    int rpasses = (dist1 + dist2 < dist*0.5) ? NREFL_OUTER_FULL : NREFL_OUTER_MIN;
//
//    // Reflection group unfolding (parallelize this loop later)
//    for (int irt = 0; irt < rpasses; ++irt) {
//        const int ir = rord[irt];
//        if (ir == 0) continue;
//        double rg1[6], rg2[6];
//        imv6(gvec1, RS[ir], rg1);
//        double d1 = NCDist_pass(rg1, gvec2, dist);
//        if (d1 < dist) dist = d1;
//        imv6(gvec2, RS[ir], rg2);
//        double d2 = NCDist_pass(gvec1, rg2, dist);
//        if (d2 < dist) dist = d2;
//    }
//    return dist;
//}
//
//} // namespace phasertng
