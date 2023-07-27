#ifndef CS6M_SELLING_REDUCE
  #define CS6M_SELLING_REDUCE

  /* #define CS6M_DEBUG zz */ 

  /* Make a universal NULL */
  #ifndef NULL
  #ifdef __cplusplus
     #define NULL nullptr
  #else
     #define NULL 0
  #endif
  #endif


  #ifdef CS6M_DEBUG_PRINT_COUT
    #undef CS6M_DEBUG_PRINT_COUT
  #endif
  #ifdef CS6M_DEBUG_PRINT_FPRINTF
    #undef CS6M_DEBUG_PRINT_FPRINTF
  #endif
  #ifdef CS6M_DEBUG
    #ifdef __cplusplus
      #include <iostream>
      #define CS6M_DEBUG_PRINT_COUT yes
    #else
      #include <stdio.h>
      #define CS6M_DEBUG_PRINT_FPRINTF yes
    #endif
  #endif

  #ifdef __cplusplus
    #define CS6M_sqrt(x) std::sqrt(x)
  #else
    #define CS6M_sqrt(x) dsqrt(x)
  #endif


 /* #define CS6M_DEBUG_PRINT_COUT yes */
  #define CS6M_S6BC 0
  #define CS6M_S6AC 1
  #define CS6M_S6AB 2
  #define CS6M_S6AD 3
  #define CS6M_S6BD 4
  #define CS6M_S6CD 5

  #define CS6M_D7A2 0
  #define CS6M_D7B2 1
  #define CS6M_D7C2 2
  #define CS6M_D7D2 3
  #define CS6M_D7_A_D_2 4
  #define CS6M_D7_B_D_2 5
  #define CS6M_D7_C_D_2 6

  #define CS6M_G6A2 0
  #define CS6M_G6B2 1
  #define CS6M_G6C2 2
  #define CS6M_G62BC 3
  #define CS6M_G62AC 4
  #define CS6M_G62AB 5

  #define CS6M_CELLA 0
  #define CS6M_CELLB 1
  #define CS6M_CELLC 2
  #define CS6M_CELLALPHA 3
  #define CS6M_CELLBETA 4
  #define CS6M_CELLGAMMA 5

  #define CS6M_V7A 0
  #define CS6M_V7B 1
  #define CS6M_V7C 2
  #define CS6M_V7ASTARINV 3
  #define CS6M_V7BSTARINV 4
  #define CS6M_V7CSTARINV 5
  #define CS6M_V7VOLCROOT 6

  #define CS6M_HORNM_SXX 0
  #define CS6M_HORNM_SXY 1
  #define CS6M_HORNM_SXZ 2
  #define CS6M_HORNM_SYX 3
  #define CS6M_HORNM_SYY 4
  #define CS6M_HORNM_SYZ 5
  #define CS6M_HORNM_SZX 6
  #define CS6M_HORNM_SZY 7
  #define CS6M_HORNM_SZZ 8

  #define CS6M_QR_W 0
  #define CS6M_QR_X 1
  #define CS6M_QR_Y 2
  #define CS6M_QR_Z 3
  


  #define CS6M_abs(x) ((x)<0?-(x):(x))

  #define CS6M_Vec3_Dot_Prod(vectorleft,vectorright,dotprod) {    \
    int ii;                                                       \
    dotprod=0.;                                                   \
    for (ii=0; ii<3; ii++) dotprod+=vectorleft[ii]*vectorright[ii];\
  }

  #define CS6M_Vec4_Dot_Prod(vectorleft,vectorright,dotprod) {    \
    int ii;                                                       \
    dotprod=0.;                                                   \
    for (ii=0; ii<4; ii++) dotprod+=vectorleft[ii]*vectorright[ii];\
  }

  #define CS6M_Vec6_Dot_Prod(vectorleft,vectorright,dotprod) {    \
    int ii;                                                       \
    dotprod=0.;                                                   \
    for (ii=0; ii<6; ii++) dotprod+=vectorleft[ii]*vectorright[ii];\
  }

  #define CS6M_CountPositive(s6vec,delta)  \
    ( ((s6vec[CS6M_S6BC]>delta)?1:0)  \
    + ((s6vec[CS6M_S6AC]>delta)?1:0)  \
    + ((s6vec[CS6M_S6AB]>delta)?1:0)  \
    + ((s6vec[CS6M_S6AD]>delta)?1:0)  \
    + ((s6vec[CS6M_S6BD]>delta)?1:0)  \
    + ((s6vec[CS6M_S6CD]>delta)?1:0)  )

  #define CS6M_NegativeSumOfScalars(s6vec) \
     (-s6vec[CS6M_S6BC]-s6vec[CS6M_S6AC]-s6vec[CS6M_S6AB]-s6vec[CS6M_S6AD]-s6vec[CS6M_S6BD]-s6vec[CS6M_S6CD]) 

  #define CS6M_comptovec3(x,y,z,vec3) { \
    vec3[0]=x;vec3[1]=y;vec3[2]=z; }

  #define CS6M_vecstoE3bycol(A,B,C,mat33) { \
    mat33[0]=A[0]; \
    mat33[1]=B[0]; \
    mat33[2]=C[0]; \
    mat33[3]=A[1]; \
    mat33[4]=B[1]; \
    mat33[5]=C[1]; \
    mat33[6]=A[2]; \
    mat33[7]=B[2]; \
    mat33[8]=C[2]; \
  }

  #define CS6M_E3bycoltovecs(mat33,A,B,C) { \
    A[0]=mat33[0]; \
    B[0]=mat33[1]; \
    C[0]=mat33[2]; \
    A[1]=mat33[3]; \
    B[1]=mat33[4]; \
    C[1]=mat33[5]; \
    A[2]=mat33[6]; \
    B[2]=mat33[7]; \
    C[2]=mat33[8]; \
  }

  #define CS6M_E3bycoltoABCD(mat33,A,B,C,D) { \
    A[0]=mat33[0]; \
    B[0]=mat33[1]; \
    C[0]=mat33[2]; \
    D[0]=-A[0]-B[0]-C[0]; \
    A[1]=mat33[3]; \
    B[1]=mat33[4]; \
    C[1]=mat33[5]; \
    D[1]=-A[1]-B[1]-C[1]; \
    A[2]=mat33[6]; \
    B[2]=mat33[7]; \
    C[2]=mat33[8]; \
    D[2]=-A[2]-B[2]-C[2]; \
  }

  #define CS6M_vecstoG6(A,B,C,g6vec) { \
    CS6M_Vec3_Dot_Prod(A,A,g6vec[CS6M_G6A2]); \
    CS6M_Vec3_Dot_Prod(B,B,g6vec[CS6M_G6B2]); \
    CS6M_Vec3_Dot_Prod(C,C,g6vec[CS6M_G6B2]); \
    CS6M_Vec3_Dot_Prod(B,C,g6vec[CS6M_G62BC]); g6vec[CS6M_G62BC]*=2.;\
    CS6M_Vec3_Dot_Prod(A,C,g6vec[CS6M_G62AC]); g6vec[CS6M_G62AC]*=2.;\
    CS6M_Vec3_Dot_Prod(A,B,g6vec[CS6M_G62AB]); g6vec[CS6M_G62AB]*=2.;\
  }

  #define CS6M_E3toG6(mat33,g6vec) {  \
    double A[3], B[3], C[3];          \
    CS6M_E3bycoltovecs(mat33,A,B,C);  \
    CS6M_vecstoG6(A,B,C,g6vec);       \
  }

  #define CS6M_vecstoS6(A,B,C,s6vec) { \
    double D[3];                       \
    CS6M_Vec3_Dot_Prod(B,C,s6vec[CS6M_G6BC]);\
    CS6M_Vec3_Dot_Prod(A,C,s6vec[CS6M_G6AC]);\
    CS6M_Vec3_Dot_Prod(A,B,s6vec[CS6M_G6AB]);\
    D[0]=-A[0]-B[0]-C[0];\
    D[1]=-A[1]-B[1]-C[1];\
    D[2]=-A[2]-B[2]-C[2];\
    CS6M_Vec3_Dot_Prod(A,D,s6vec[CS6M_G6AD]);\
    CS6M_Vec3_Dot_Prod(B,D,s6vec[CS6M_G6BD]);\
    CS6M_Vec3_Dot_Prod(C,D,s6vec[CS6M_G6CD]);\
  }

  #define CS6M_E3toS6(mat33,g6vec) {  \
    double A[3], B[3], C[3];          \
    CS6M_E3bycoltovecs(mat33,A,B,C);  \
    CS6M_vecstoS6(A,B,C,g6vec);       \
  }

  #define CS6M_E3cellstoHornMatM(E3cell1,E3cell2,HormMatM) { \
    double A1[3],B1[3],C1[3],D1[3],A2[3],B2[3],C2[3],D2[3];  \
    int ii;                                                  \
    CS6M_E3bycoltovecs(E3cell1,A1,B1,C1);                    \
    CS6M_E3bycoltovecs(E3cell2,A2,B2,C2);                    \
    for(ii=0;ii<3;ii++) {                                    \
      D1[ii]=-A1[ii]-B1[ii]-C1[ii];                          \
      D2[ii]=-A2[ii]-B2[ii]-C2[ii];                          \
    }                                                        \
    HornMatM[CS6M_HORNM_SXX]=A1[0]*A2[0]+B1[0]*B2[0]+C1[0]*C2[0]+D1[0]*D2[0]; \
    HornMatM[CS6M_HORNM_SXY]=A1[0]*A2[1]+B1[0]*B2[1]+C1[0]*C2[1]+D1[0]*D2[1]; \
    HornMatM[CS6M_HORNM_SXZ]=A1[0]*A2[2]+B1[0]*B2[2]+C1[0]*C2[2]+D1[0]*D2[2]; \
    HornMatM[CS6M_HORNM_SYX]=A1[1]*A2[0]+B1[1]*B2[0]+C1[1]*C2[0]+D1[1]*D2[0]; \
    HornMatM[CS6M_HORNM_SYY]=A1[1]*A2[1]+B1[1]*B2[1]+C1[1]*C2[1]+D1[1]*D2[1]; \
    HornMatM[CS6M_HORNM_SYZ]=A1[1]*A2[2]+B1[1]*B2[2]+C1[1]*C2[2]+D1[1]*D2[2]; \
    HornMatM[CS6M_HORNM_SZX]=A1[2]*A2[0]+B1[2]*B2[0]+C1[2]*C2[0]+D1[2]*D2[0]; \
    HornMatM[CS6M_HORNM_SZY]=A1[2]*A2[1]+B1[2]*B2[1]+C1[2]*C2[1]+D1[2]*D2[1]; \
    HornMatM[CS6M_HORNM_SZZ]=A1[2]*A2[2]+B1[2]*B2[2]+C1[2]*C2[2]+D1[2]*D2[2]; \
  }

  #define CS6M_HornMatMtoHornMatN(HornMatM,HornMatN) { \
    HornMatN[0] = HornMatM[CS6M_HORNM_SXX]+HornMatM[CS6M_HORNM_SYY]+HornMatM[CS6M_HORNM_SZZ]; \
    HornMatN[1] = HornMatM[CS6M_HORNM_SYZ]-HornMatM[CS6M_HORNM_SZY]; \
    HornMatN[2] = HornMatM[CS6M_HORNM_SZX]-HornMatM[CS6M_HORNM_SXZ]; \
    HornMatN[3] = HornMatM[CS6M_HORNM_SXY]-HornMatM[CS6M_HORNM_SYX]; \
    HornMatN[4] = HornMatM[CS6M_HORNM_SYZ]-HornMatM[CS6M_HORNM_SZY]; \
    HornMatN[5] = HornMatM[CS6M_HORNM_SXX]-HornMatM[CS6M_HORNM_SYY]-HornMatM[CS6M_HORNM_SZZ]; \
    HornMatN[6] = HornMatM[CS6M_HORNM_SXY]+HornMatM[CS6M_HORNM_SYX]; \
    HornMatN[7] = HornMatM[CS6M_HORNM_SZX]+HornMatM[CS6M_HORNM_SXZ]; \
    HornMatN[8] = HornMatM[CS6M_HORNM_SZX]-HornMatM[CS6M_HORNM_SXZ]; \
    HornMatN[9] = HornMatM[CS6M_HORNM_SXY]+HornMatM[CS6M_HORNM_SYX]; \
    HornMatN[10]=-HornMatM[CS6M_HORNM_SXX]+HornMatM[CS6M_HORNM_SYY]-HornMatM[CS6M_HORNM_SZZ]; \
    HornMatN[11]= HornMatM[CS6M_HORNM_SYZ]+HornMatM[CS6M_HORNM_SZY]; \
    HornMatN[12]= HornMatM[CS6M_HORNM_SXY]-HornMatM[CS6M_HORNM_SYX]; \
    HornMatN[13]= HornMatM[CS6M_HORNM_SZX]+HornMatM[CS6M_HORNM_SXZ]; \
    HornMatN[14]= HornMatM[CS6M_HORNM_SYZ]+HornMatM[CS6M_HORNM_SZY]; \
    HornMatN[15]=-HornMatM[CS6M_HORNM_SXX]-HornMatM[CS6M_HORNM_SYY]+HornMatM[CS6M_HORNM_SZZ]; \
  }

  #define CS6M_comptovec6(a,b,c,d,e,f,vec6) { \
    vec6[0]=a;vec6[1]=b;vec6[2]=c;vec6[3]=d;vec6[4]=e;vec6[5]=f; }

  #define CS6M_CellradtoG6(cellrad,g6vec) { \
    g6vec[CS6M_G6A2] = cellrad[CS6M_CELLA]*cellrad[CS6M_CELLA]; \
    g6vec[CS6M_G6B2] = cellrad[CS6M_CELLB]*cellrad[CS6M_CELLB]; \
    g6vec[CS6M_G6C2] = cellrad[CS6M_CELLC]*cellrad[CS6M_CELLC]; \
    g6vec[CS6M_G62BC] = cellrad[CS6M_CELLB]*cellrad[CS6M_CELLC]*2.*cos(cellrad[CS6M_CELLALPHA]); \
    g6vec[CS6M_G62AC] = cellrad[CS6M_CELLA]*cellrad[CS6M_CELLC]*2.*cos(cellrad[CS6M_CELLBETA]); \
    g6vec[CS6M_G62AB] = cellrad[CS6M_CELLA]*cellrad[CS6M_CELLB]*2.*cos(cellrad[CS6M_CELLGAMMA]); }

  #define CS6M_D7toG6(d7vec,g6vec) { \
    g6vec[CS6M_G6A2] = d7vec[CS6M_D7A2]; \
    g6vec[CS6M_G6B2] = d7vec[CS6M_D7B2]; \
    g6vec[CS6M_G6C2] = d7vec[CS6M_D7C2]; \
    g6vec[CS6M_G62BC] = d7vec[CS6M_D7_A_D_2]-d7vec[CS6M_D7C2]-d7vec[CS6M_D7B2];\
    g6vec[CS6M_G62AC] = d7vec[CS6M_D7_B_D_2]-d7vec[CS6M_D7C2]-d7vec[CS6M_D7A2];\
    g6vec[CS6M_G62AB] = d7vec[CS6M_D7_C_D_2]-d7vec[CS6M_D7A2]-d7vec[CS6M_D7B2];\
  }


  #define CS6M_D7toS6(d7vec,s6vec) { \
    s6vec[CS6M_S6BC] = -(-d7vec[CS6M_D7_A_D_2]+d7vec[CS6M_D7C2]+d7vec[CS6M_D7B2])/2;\
    s6vec[CS6M_S6AC] = -(-d7vec[CS6M_D7_B_D_2]+d7vec[CS6M_D7C2]+d7vec[CS6M_D7A2])/2;\
    s6vec[CS6M_S6AB] = -(-d7vec[CS6M_D7_C_D_2]+d7vec[CS6M_D7B2]+d7vec[CS6M_D7A2])/2;\
    s6vec[CS6M_S6AD] = -(-d7vec[CS6M_D7_A_D_2]+d7vec[CS6M_D7D2]+d7vec[CS6M_D7A2])/2;\
    s6vec[CS6M_S6BD] = -(-d7vec[CS6M_D7_B_D_2]+d7vec[CS6M_D7D2]+d7vec[CS6M_D7B2])/2;\
    s6vec[CS6M_S6CD] = (-d7vec[CS6M_D7_B_D_2]-d7vec[CS6M_D7_A_D_2]+d7vec[CS6M_D7B2]+d7vec[CS6M_D7A2])/2;\
  }

  #define CS6M_G6toCell(g6vec,cell) {                                                     \
    double cosalpha, cosbeta, cosgamma;                                                   \
    double sinalpha, sinbeta, singamma;                                                   \
    cell[CS6M_CELLA]=CS6M_sqrt(fabs(g6vec[CS6M_G6A2]));                                   \
    cell[CS6M_CELLB]=CS6M_sqrt(fabs(g6vec[CS6M_G6B2]));                                   \
    cell[CS6M_CELLC]=CS6M_sqrt(fabs(g6vec[CS6M_G6C2]));                                   \
    cosalpha=g6vec[CS6M_G62BC]/(2.*cell[CS6M_CELLB]*cell[CS6M_CELLC]);                       \
    cosbeta =g6vec[CS6M_G62AC]/(2.*cell[CS6M_CELLA]*cell[CS6M_CELLC]);                       \
    cosgamma=g6vec[CS6M_G62AB]/(2.*cell[CS6M_CELLA]*cell[CS6M_CELLB]);                       \
    sinalpha=CS6M_sqrt(fabs((1.-cosalpha*cosalpha)));                                     \
    sinbeta =CS6M_sqrt(fabs((1.-cosbeta* cosbeta)));                                      \
    singamma=CS6M_sqrt(fabs((1.-cosgamma*cosgamma)));                                     \
    cell[CS6M_CELLALPHA]=atan2(sinalpha,cosalpha);                                        \
    cell[CS6M_CELLBETA] =atan2(sinbeta, cosbeta);                                         \
    cell[CS6M_CELLGAMMA]=atan2(singamma,cosgamma);                                        \
  }

  #define CS6M_G6toCelldeg(g6vec,cell) {                                                  \
    double pi=3.1415926535897932384626433832795;                                          \
    double todeg=180./pi;                                                                 \
    double cosalpha, cosbeta, cosgamma;                                                   \
    double sinalpha, sinbeta, singamma;                                                   \
    cell[CS6M_CELLA]=CS6M_sqrt(fabs(g6vec[CS6M_G6A2]));                                   \
    cell[CS6M_CELLB]=CS6M_sqrt(fabs(g6vec[CS6M_G6B2]));                                   \
    cell[CS6M_CELLC]=CS6M_sqrt(fabs(g6vec[CS6M_G6C2]));                                   \
    cosalpha=g6vec[CS6M_G62BC]/(2.*cell[CS6M_CELLB]*cell[CS6M_CELLC]);                       \
    cosbeta =g6vec[CS6M_G62AC]/(2.*cell[CS6M_CELLA]*cell[CS6M_CELLC]);                       \
    cosgamma=g6vec[CS6M_G62AB]/(2.*cell[CS6M_CELLA]*cell[CS6M_CELLB]);                       \
    sinalpha=CS6M_sqrt(fabs((1.-cosalpha*cosalpha)));                                     \
    sinbeta =CS6M_sqrt(fabs((1.-cosbeta* cosbeta)));                                      \
    singamma=CS6M_sqrt(fabs((1.-cosgamma*cosgamma)));                                     \
    cell[CS6M_CELLALPHA]=todeg*atan2(sinalpha,cosalpha);                                  \
    cell[CS6M_CELLBETA] =todeg*atan2(sinbeta, cosbeta);                                   \
    cell[CS6M_CELLGAMMA]=todeg*atan2(singamma,cosgamma);                                  \
  }

  #define CS6M_CelldegtoG6(cell,g6vec) {                                            \
    double delta;                                                                         \
    double pi=3.1415926535897932384626433832795;                                          \
    double torad=pi/180.;                                                                 \
    size_t i;                                                                             \
    delta = fabs(cell[CS6M_CELLA]);                                                       \
    if (cell[CS6M_CELLB] > delta) delta = cell[CS6M_CELLB];                               \
    if (cell[CS6M_CELLC] > delta) delta = cell[CS6M_CELLC];                               \
    delta *= 1.e-12;                                                                      \
    g6vec[CS6M_G6A2] = cell[CS6M_CELLA]*cell[CS6M_CELLA];                                 \
    g6vec[CS6M_G6B2] = cell[CS6M_CELLB]*cell[CS6M_CELLB];                                 \
    g6vec[CS6M_G6C2] = cell[CS6M_CELLC]*cell[CS6M_CELLC];                                 \
    g6vec[CS6M_G62BC] = 2.0*cell[CS6M_CELLB]*cell[CS6M_CELLC]*cos(cell[CS6M_CELLALPHA]*torad);  \
    g6vec[CS6M_G62AC] = 2.0*cell[CS6M_CELLA]*cell[CS6M_CELLC]*cos(cell[CS6M_CELLBETA]*torad);   \
    g6vec[CS6M_G62AB] = 2.0*cell[CS6M_CELLA]*cell[CS6M_CELLB]*cos(cell[CS6M_CELLGAMMA]*torad);  \
    for ( i=3; i<6; ++i ) if ( std::fabs(g6vec[i]) < delta ) g6vec[i] = 0.0;              \
  }

  #define CS6M_CelltoG6(cell,g6vec) {                                                     \
    double delta;                                                                         \
    size_t i;                                                                             \
    double pi=3.1415926535897932384626433832795;                                          \
    if (cell[CS6M_CELLALPHA]>2.*pi || cell[CS6M_CELLALPHA]<-2.*pi                         \
        || cell[CS6M_CELLBETA]>2.*pi || cell[CS6M_CELLBETA]<-2.*pi                        \
        || cell[CS6M_CELLGAMMA]>2.*pi || cell[CS6M_CELLGAMMA]<-2.*pi) {                   \
        CS6M_CelldegtoG6(cell,g6vec);                                                     \
    } else {                                                                              \
      delta = fabs(cell[CS6M_CELLA]);                                                     \
      if (cell[CS6M_CELLB] > delta) delta = cell[CS6M_CELLB];                             \
      if (cell[CS6M_CELLC] > delta) delta = cell[CS6M_CELLC];                             \
      delta *= 1.e-12;                                                                    \
      g6vec[CS6M_G6A2] = cell[CS6M_CELLA]*cell[CS6M_CELLA];                               \
      g6vec[CS6M_G6B2] = cell[CS6M_CELLB]*cell[CS6M_CELLB];                               \
      g6vec[CS6M_G6C2] = cell[CS6M_CELLC]*cell[CS6M_CELLC];                               \
      g6vec[CS6M_G62BC] = 2.0*cell[CS6M_CELLB]*cell[CS6M_CELLC]*cos(cell[CS6M_CELLALPHA]);\
      g6vec[CS6M_G62AC] = 2.0*cell[CS6M_CELLA]*cell[CS6M_CELLC]*cos(cell[CS6M_CELLBETA]); \
      g6vec[CS6M_G62AB] = 2.0*cell[CS6M_CELLA]*cell[CS6M_CELLB]*cos(cell[CS6M_CELLGAMMA]);\
      for ( i=3; i<6; ++i ) if ( std::fabs(g6vec[i]) < delta ) g6vec[i] = 0.0;            \
    }                                                                                     \
  }



  #define CS6M_G6toD7(g6vec,d7vec) { \
    d7vec[CS6M_D7A2] = g6vec[CS6M_G6A2]; \
    d7vec[CS6M_D7B2] = g6vec[CS6M_G6B2]; \
    d7vec[CS6M_D7C2] = g6vec[CS6M_G6C2]; \
    d7vec[CS6M_D7D2] = g6vec[CS6M_G6A2]+g6vec[CS6M_G6B2]+g6vec[CS6M_G6C2]\
               +g6vec[CS6M_G62BC]+g6vec[CS6M_G62AC]+g6vec[CS6M_G62AB]; \
    d7vec[CS6M_D7_A_D_2] = g6vec[CS6M_G6B2]+g6vec[CS6M_G6C2]+g6vec[CS6M_G62BC]; \
    d7vec[CS6M_D7_B_D_2] = g6vec[CS6M_G6A2]+g6vec[CS6M_G6C2]+g6vec[CS6M_G62AC]; \
    d7vec[CS6M_D7_C_D_2] = g6vec[CS6M_G6A2]+g6vec[CS6M_G6B2]+g6vec[CS6M_G62AB]; \
  }

  #define CS6M_CelltoD7(cell,d7vec) { \
    double g6vec[6];           \
    CS6M_CelltoG6(cell,g6vec); \
    CS6M_G6toD7(g6vec,d7vec);  \
  }

  static double CS6M_G6toS6_ConvMat[36] = {
     0., 0., 0.,0.5, 0., 0.,
     0., 0., 0., 0.,0.5, 0.,
     0., 0., 0., 0., 0.,0.5,
    -1., 0., 0., 0.,-.5,-.5,
     0.,-1., 0.,-.5, 0.,-.5,
     0., 0.,-1.,-.5,-.5, 0.};

  #define CS6M_G6toS6_Mat(g6vec,s6vec,mat66) { \
    double mat66out[36];                     \
    int ii;                                  \
    s6vec[CS6M_S6BC] = g6vec[CS6M_G62BC]/2.; \
    s6vec[CS6M_S6AC] = g6vec[CS6M_G62AC]/2.; \
    s6vec[CS6M_S6AB] = g6vec[CS6M_G62AB]/2.; \
    s6vec[CS6M_S6AD] = -g6vec[CS6M_G6A2]-s6vec[CS6M_S6AB]-s6vec[CS6M_S6AC]; \
    s6vec[CS6M_S6BD] = -s6vec[CS6M_S6AB]-g6vec[CS6M_G6B2]-s6vec[CS6M_S6BC]; \
    s6vec[CS6M_S6CD] = -s6vec[CS6M_S6AC]-s6vec[CS6M_S6BC]-g6vec[CS6M_G6C2]; \
    CS6M_Mat66_Mat66_Mult(CS6M_G6toS6_ConvMat,mat66,mat66out);              \
    for (ii=0; ii < 36; ii++) mat66[ii]=mat66out[ii];                       \
  }

  #define CS6M_G6toS6(g6vec,s6vec) { \
    s6vec[CS6M_S6BC] = g6vec[CS6M_G62BC]/2.; \
    s6vec[CS6M_S6AC] = g6vec[CS6M_G62AC]/2.; \
    s6vec[CS6M_S6AB] = g6vec[CS6M_G62AB]/2.; \
    s6vec[CS6M_S6AD] = -g6vec[CS6M_G6A2]-s6vec[CS6M_S6AB]-s6vec[CS6M_S6AC]; \
    s6vec[CS6M_S6BD] = -s6vec[CS6M_S6AB]-g6vec[CS6M_G6B2]-s6vec[CS6M_S6BC]; \
    s6vec[CS6M_S6CD] = -s6vec[CS6M_S6AC]-s6vec[CS6M_S6BC]-g6vec[CS6M_G6C2]; \
  }

  #define CS6M_S6toD7(s6vec,d7vec) { \
    d7vec[CS6M_D7A2] = -s6vec[CS6M_S6AD] -s6vec[CS6M_S6AB] -s6vec[CS6M_S6AC];\
    d7vec[CS6M_D7B2] = -s6vec[CS6M_S6BD] -s6vec[CS6M_S6AB] -s6vec[CS6M_S6BC];\
    d7vec[CS6M_D7C2] = -s6vec[CS6M_S6CD] -s6vec[CS6M_S6AC] -s6vec[CS6M_S6BC];\
    d7vec[CS6M_D7D2] = -s6vec[CS6M_S6AD] -s6vec[CS6M_S6BD] -s6vec[CS6M_S6CD];\
    d7vec[CS6M_D7_A_D_2] = -s6vec[CS6M_S6BD] -s6vec[CS6M_S6AB] -s6vec[CS6M_S6CD] -s6vec[CS6M_S6AC];\
    d7vec[CS6M_D7_B_D_2] = -s6vec[CS6M_S6AD] -s6vec[CS6M_S6AB] -s6vec[CS6M_S6CD] -s6vec[CS6M_S6BC];\
    d7vec[CS6M_D7_C_D_2] = -s6vec[CS6M_S6AD] -s6vec[CS6M_S6AC] -s6vec[CS6M_S6BD] -s6vec[CS6M_S6BC];\
  }




  static double CS6M_S6toG6_ConvMat[36] = {
     0.,-1.,-1.,-1., 0., 0.,
    -1., 0.,-1., 0.,-1., 0.,
    -1.,-1., 0., 0., 0.,-1.,
     0., 0., 0., 2., 0., 0.,
     0., 0., 0., 0., 2., 0.,
     0., 0., 0., 0., 0., 2.};


  #define CS6M_S6toG6_Mat(s6vec,g6vec,mat66) { \
    double mat66out[36]; int ii;               \
    g6vec[CS6M_G6A2] = -s6vec[CS6M_S6AD] -s6vec[CS6M_S6AB] -s6vec[CS6M_S6AC];\
    g6vec[CS6M_G6B2] = -s6vec[CS6M_S6BD] -s6vec[CS6M_S6AB] -s6vec[CS6M_S6BC];\
    g6vec[CS6M_G6C2] = -s6vec[CS6M_S6CD] -s6vec[CS6M_S6AC] -s6vec[CS6M_S6BC];\
    g6vec[CS6M_G62BC] = 2.*s6vec[CS6M_S6BC];\
    g6vec[CS6M_G62AC] = 2.*s6vec[CS6M_S6AC];\
    g6vec[CS6M_G62AB] = 2.*s6vec[CS6M_S6AB];\
    CS6M_Mat66_Mat66_Mult(CS6M_S6toG6_ConvMat,mat66,mat66out);               \
    for (ii=0; ii < 36; ii++) mat66[ii]=mat66out[ii];                        \
  }


  #define CS6M_S6toG6(s6vec,g6vec) { \
    g6vec[CS6M_G6A2] = -s6vec[CS6M_S6AD] -s6vec[CS6M_S6AB] -s6vec[CS6M_S6AC];\
    g6vec[CS6M_G6B2] = -s6vec[CS6M_S6BD] -s6vec[CS6M_S6AB] -s6vec[CS6M_S6BC];\
    g6vec[CS6M_G6C2] = -s6vec[CS6M_S6CD] -s6vec[CS6M_S6AC] -s6vec[CS6M_S6BC];\
    g6vec[CS6M_G62BC] = 2.*s6vec[CS6M_S6BC];\
    g6vec[CS6M_G62AC] = 2.*s6vec[CS6M_S6AC];\
    g6vec[CS6M_G62AB] = 2.*s6vec[CS6M_S6AB];\
  }

  #ifdef CS6M_DEBUG
     #ifndef CS6M_VOLCHECK_PREP
      #define CS6M_VOLCHECK_PREP(xg6unred)  \
        LRL_Cell cellred; \
        LRL_Cell newcellred; \
        G6 g6cellred; \
        G6 g6newcellred; \
        double entryvol,curvol;\
        CS6M_G6toCell(xg6unred,cellred);\
        CS6M_Cellvolume(cellred,entryvol); 
      #define CS6M_VOLCHECK(label,xg6red)       \
        CS6M_G6toCell(xg6red,newcellred);   \
        CS6M_Cellvolume(newcellred,curvol);\
        CS6M_CellradtoG6(cellred,g6cellred);  \
        CS6M_CellradtoG6(newcellred,g6newcellred); \
        if (fabs(entryvol-curvol)> 1.e-20 ) { \
          std::cerr<<label<<": Volume change: "<< entryvol << " to "<<curvol<<std::endl; \
          std::cerr<<"cellred: " << cellred[0] <<", "    \
          << cellred[1] <<", "                           \
          << cellred[2] <<", "                           \
          << cellred[3] <<", "                           \
          << cellred[4] <<", "                           \
          << cellred[5] <<std::endl;                     \
          std::cerr<<"g6cellred: " << g6cellred[0] <<", "    \
          << g6cellred[1] <<", "                           \
          << g6cellred[2] <<", "                           \
          << g6cellred[3] <<", "                           \
          << g6cellred[4] <<", "                           \
          << g6cellred[5] <<std::endl;                     \
          std::cerr<<"newcellred: " << newcellred[0] <<", "    \
            << newcellred[1] <<", "                           \
            << newcellred[2] <<", "                           \
            << newcellred[3] <<", "                           \
            << newcellred[4] <<", "                           \
            << newcellred[5] <<std::endl;                     \
          std::cerr<<"g6newcellred: " << g6newcellred[0] <<", "    \
            << g6newcellred[1] <<", "                           \
            << g6newcellred[2] <<", "                           \
            << g6newcellred[3] <<", "                           \
            << g6newcellred[4] <<", "                           \
            << g6newcellred[5] <<std::endl;                     \
           entryvol=curvol;                                     \
         } 
    #endif
  #else
    #ifndef CS6M_VOLCHECK_PREP
      #define  CS6M_VOLCHECK_PREP(dummy)
      #define  CS6M_VOLCHECK(label,dummy)
    #endif
  #endif

   static double CS6M_MAT_SP1[36]={
      0., 1., 0., 0., 0., 0.,
      1., 0., 0., 0., 0., 0.,
      0., 0., 1., 0., 0., 0.,
      0., 0., 0., 0., 1., 0.,
      0., 0., 0., 1., 0., 0.,
      0., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_SP2[36]={
      1., 0., 0., 0., 0., 0.,
      0., 0., 1., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      0., 0., 0., 1., 0., 0.,
      0., 0., 0., 0., 0., 1.,
      0., 0., 0., 0., 1., 0.
   };
   static double CS6M_MAT_SP3PPP[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      0., 0., 1., 0., 0., 0.,
      0., 0., 0., 1., 0., 0.,
      0., 0., 0., 0., 1., 0.,
      0., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_SP3PMM[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      0., 0., 1., 0., 0., 0.,
      0., 0., 0., 1., 0., 0.,
      0., 0., 0., 0.,-1., 0.,
      0., 0., 0., 0., 0.,-1.
   };
   static double CS6M_MAT_SP3MPM[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      0., 0., 1., 0., 0., 0.,
      0., 0., 0.,-1., 0., 0.,
      0., 0., 0., 0., 1., 0.,
      0., 0., 0., 0., 0.,-1.
   };
   static double CS6M_MAT_SP3MMP[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      0., 0., 1., 0., 0., 0.,
      0., 0., 0.,-1., 0., 0.,
      0., 0., 0., 0.,-1., 0.,
      0., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_SP4MMM[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      0., 0., 1., 0., 0., 0.,
      0., 0., 0., 1., 0., 0.,
      0., 0., 0., 0., 1., 0.,
      0., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_SP4MPP[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      0., 0., 1., 0., 0., 0.,
      0., 0., 0., 1., 0., 0.,
      0., 0., 0., 0.,-1., 0.,
      0., 0., 0., 0., 0.,-1.
   };
   static double CS6M_MAT_SP4PMP[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      0., 0., 1., 0., 0., 0.,
      0., 0., 0.,-1., 0., 0.,
      0., 0., 0., 0., 1., 0.,
      0., 0., 0., 0., 0.,-1.
   };
   static double CS6M_MAT_SP4PPM[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      0., 0., 1., 0., 0., 0.,
      0., 0., 0.,-1., 0., 0.,
      0., 0., 0., 0.,-1., 0.,
      0., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_R54P[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      0., 1., 1.,-1., 0., 0.,
      0.,-2., 0., 1., 0., 0.,
      0., 0., 0., 0.,-1., 1.,
      0., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_R54M[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      0., 1., 1., 1., 0., 0.,
      0., 2., 0., 1., 0., 0.,
      0., 0., 0., 0., 1., 1.,
      0., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_R65P[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      1., 0., 1., 0.,-1., 0.,
      0., 0., 0., 1., 0.,-1.,
     -2., 0., 0., 0., 1., 0.,
      0., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_R65M[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      1., 0., 1., 0., 1., 0.,
      0., 0., 0., 1., 0., 1.,
      2., 0., 0., 0., 1., 0.,
      0., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_R76P[36]={
      1., 0., 0., 0., 0., 0.,
      1., 1., 0., 0., 0.,-1.,
      0., 0., 1., 0., 0., 0.,
      0., 0., 0.,-1., 1., 0.,
      0., 0., 0., 0.,-1., 0.,
     -2., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_R76M[36]={
      1., 0., 0., 0., 0., 0.,
      1., 1., 0., 0., 0., 1.,
      0., 0., 1., 0., 0., 0.,
      0., 0., 0.,-1.,-1., 0.,
      0., 0., 0., 0.,-1., 0.,
      2., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_R8[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      1., 1., 1., 1., 1., 1.,
      0.,-2., 0.,-1., 0.,-1.,
     -2., 0., 0., 0.,-1.,-1.,
      0., 0., 0., 0., 0., 1.
   };


  #define CS6M_G6Reduce_Mat(g6unred,g6red,mat66,reduced) { \
    double mat66out[36];                      \
    double s6in[6];                           \
    double s6out[6];                          \
    int notdone;                              \
    int ii;                                   \
    notdone = 1;                              \
    int redpass;                              \
    double temp;                              \
    double delta = 0;                         \
    CS6M_VOLCHECK_PREP(g6unred);              \
    for (ii=0; ii < 36; ii++) mat66out[ii]=mat66[ii]; \
    CS6M_G6toS6_Mat(g6unred,s6in,mat66);      \
    reduced=1;                                \
    CS6M_S6Reduce_Mat(s6in,s6out,mat66,reduced);\
    if (reduced) {                            \
       CS6M_S6toG6_Mat(s6out,g6red,mat66);    \
    } else {                                  \
      for (ii=0; ii < 6; ii++) g6red[ii]=g6unred[ii];   \
      for (ii=0; ii < 36; ii++) mat66[ii]=mat66out[ii]; \
    }                                         \
    delta=g6unred[2];                         \
    CS6M_VOLCHECK("S6",g6red);                \
    if (g6unred[1] > delta) delta=g6unred[1]; \
    if (g6unred[0] > delta) delta=g6unred[0]; \
    delta = delta * 1.e-12;                   \
    if (delta < 1.e-12) delta=1.e-12;         \
    if (fabs(g6red[CS6M_G62BC])  < delta ) g6red[CS6M_G62BC] = 0;      \
    if (fabs(g6red[CS6M_G62AC])  < delta ) g6red[CS6M_G62AC] = 0;      \
    if (fabs(g6red[CS6M_G62AB])  < delta ) g6red[CS6M_G62AB] = 0;      \
                                                                       \
    redpass=0;                                                         \
    while ( notdone && redpass < 1000) {                               \
    if (g6red[CS6M_G6A2] < 0.                                          \
        || g6red[CS6M_G6B2] < 0.                                       \
        || g6red[CS6M_G6C2] < 0.                                       \
        || g6red[CS6M_G6B2] + g6red[CS6M_G6C2] + g6red[CS6M_G62BC] < 0.\
        || g6red[CS6M_G6A2] + g6red[CS6M_G6C2] + g6red[CS6M_G62AC] < 0.\
        || g6red[CS6M_G6A2] + g6red[CS6M_G6B2] + g6red[CS6M_G62AB] < 0.\
        || g6red[CS6M_G6B2] + g6red[CS6M_G6C2] - g6red[CS6M_G62BC] < 0.\
        || g6red[CS6M_G6A2] + g6red[CS6M_G6C2] - g6red[CS6M_G62AC] < 0.\
        || g6red[CS6M_G6A2] + g6red[CS6M_G6B2] - g6red[CS6M_G62AB] < 0.\
    ) {                                                                \
      notdone=0; break;                                                \
    }                                                                  \
      /* g1 < g2 or g1=g2 && |g4| < |g5| */                            \
      if ( g6red[CS6M_G6A2] > g6red[CS6M_G6B2]+delta                   \
           || ( CS6M_abs(g6red[CS6M_G6A2] - g6red[CS6M_G6B2]) < delta      \
           && CS6M_abs(g6red[CS6M_G62BC]) > CS6M_abs(g6red[CS6M_G62AC])+delta) ) { \
        temp = g6red[CS6M_G6A2];                                 \
        g6red[CS6M_G6A2] = g6red[CS6M_G6B2];                     \
        g6red[CS6M_G6B2] = temp;                                 \
        temp = g6red[CS6M_G62BC];                                \
        g6red[CS6M_G62BC] = g6red[CS6M_G62AC];                   \
        g6red[CS6M_G62AC] = temp;                                \
        CS6M_Mat66_Mat66_Mult(CS6M_MAT_SP1,mat66,mat66out);      \
        for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];             \
        /* std::cout<<"bd1 "<<g6red[0]<<" "<<g6red[1]<<" "       \
        <<g6red[2]<<" "<<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]  \
        <<" "<<std::endl; */                                     \
        CS6M_VOLCHECK("swap a b ",g6red);                        \
        redpass++; continue;                                     \
      }                                                          \
      /* g2 < g3 or g2=g3 && |g5| < |g6| */                      \
      if ( g6red[CS6M_G6B2] > g6red[CS6M_G6C2]+delta             \
            ||  (fabs(g6red[CS6M_G6B2] - g6red[CS6M_G6C2]) < delta \
            && CS6M_abs(g6red[CS6M_G62AC]) > CS6M_abs(g6red[CS6M_G62AB])+delta) ) {\
        temp = g6red[CS6M_G6B2];                                 \
        g6red[CS6M_G6B2] = g6red[CS6M_G6C2];                     \
        g6red[CS6M_G6C2] = temp;                                 \
        temp = g6red[CS6M_G62AC];                                \
        g6red[CS6M_G62AC] = g6red[CS6M_G62AB];                   \
        g6red[CS6M_G62AB] = temp;                                \
        CS6M_Mat66_Mat66_Mult(CS6M_MAT_SP2,mat66,mat66out);      \
        for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];             \
        CS6M_VOLCHECK("swap b c ",g6red);                        \
        /* std::cout<<"bd2 "<<g6red[0]<<" "<<g6red[1]<<" "       \
        <<g6red[2]<<" "<<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]  \
        <<" "<<std::endl; */                                     \
        redpass++; continue;                                     \
      }                                                          \
      if ( g6red[CS6M_G62BC]*g6red[CS6M_G62AC]*g6red[CS6M_G62AB] > 0. ) { \
        int mmm=0, ppp=0;                                        \
        if (g6red[CS6M_G62BC] < 0. || g6red[CS6M_G62AC] < 0.     \
          || g6red[CS6M_G62AC] < 0. ) {                          \
          if ( g6red[CS6M_G62BC] < 0. ) g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]; mmm +=1; \
          if ( g6red[CS6M_G62AC] < 0. ) g6red[CS6M_G62AC] = -g6red[CS6M_G62AC]; mmm +=2; \
          if ( g6red[CS6M_G62AB] < 0. ) g6red[CS6M_G62AB] = -g6red[CS6M_G62AB]; mmm +=4; \
          switch (mmm) {                                                                 \
              case 6:                                                                    \
                CS6M_Mat66_Mat66_Mult(CS6M_MAT_SP3PMM,mat66,mat66out);                   \
                for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                             \
                break;                                                                   \
              case 5:                                                                    \
                CS6M_Mat66_Mat66_Mult(CS6M_MAT_SP3MPM,mat66,mat66out);                   \
                for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                             \
                break;                                                                   \
              case 3:                                                                    \
                CS6M_Mat66_Mat66_Mult(CS6M_MAT_SP3MMP,mat66,mat66out);                   \
                for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                             \
                break;                                                                   \
          }                                                                              \
          /* std::cout<<"+++ "<<g6red[0]<<" "<<g6red[1]<<" "     \
          <<g6red[2]<<" "<<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]\
          <<" "<<std::endl; */                                   \
          CS6M_VOLCHECK("--- ",g6red);                           \
          redpass++; continue;                                   \
        }                                                        \
      } else {                                                   \
        if (g6red[CS6M_G62BC] > 0. || g6red[CS6M_G62AC] > 0.     \
          || g6red[CS6M_G62AC] > 0. ) {                          \
          if ( g6red[CS6M_G62BC] > 0. ) g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]; ppp += 1; \
          if ( g6red[CS6M_G62AC] > 0. ) g6red[CS6M_G62AC] = -g6red[CS6M_G62AC]; ppp += 2; \
          if ( g6red[CS6M_G62AB] > 0. ) g6red[CS6M_G62AB] = -g6red[CS6M_G62AB]; ppp += 4; \
            switch (ppp) {                                                                \
              case 6:                                                                    \
                CS6M_Mat66_Mat66_Mult(CS6M_MAT_SP4MPP,mat66,mat66out);                   \
                for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                             \
                break;                                                                   \
              case 5:                                                                    \
                CS6M_Mat66_Mat66_Mult(CS6M_MAT_SP4PMP,mat66,mat66out);                   \
                for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                             \
                break;                                                                   \
              case 3:                                                                    \
                CS6M_Mat66_Mat66_Mult(CS6M_MAT_SP4PPM,mat66,mat66out);                   \
                for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                             \
                break;                                                                   \
          }                                                                              \
        /* std::cout<<"--- "<<g6red[0]<<" "<<g6red[1]<<" "     \
          <<g6red[2]<<" "<<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]\
          <<" "<<std::endl; */                                   \
          CS6M_VOLCHECK("+++ ",g6red);                           \
          redpass++; continue;                                   \
        }                                                        \
      }                                                          \
                                                                 \
      /* 678 boundaries outside */                               \
      if (g6red[CS6M_G62BC] >  g6red[CS6M_G6B2]+delta) {         \
          g6red[CS6M_G6C2] = g6red[CS6M_G6B2]+g6red[CS6M_G6C2]-g6red[CS6M_G62BC];\
          g6red[CS6M_G62BC] = -2.*g6red[CS6M_G6B2]+g6red[CS6M_G62BC];            \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC]+g6red[CS6M_G62AB];              \
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R54P,mat66,mat66out);                   \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bd6+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("6+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62BC] < -g6red[CS6M_G6B2]-delta) {                         \
          g6red[CS6M_G6C2] = g6red[CS6M_G6B2]+g6red[CS6M_G6C2]+g6red[CS6M_G62BC];\
          g6red[CS6M_G62BC] = 2.*g6red[CS6M_G6B2]+g6red[CS6M_G62BC];             \
          g6red[CS6M_G62AC] = g6red[CS6M_G62AC]+g6red[CS6M_G62AB];               \
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R54M,mat66,mat66out);                   \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bd8+"<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3 ]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/           \
          CS6M_VOLCHECK("8+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /* 9AB boundaries outside */                                               \
      if (g6red[CS6M_G62AC] > g6red[CS6M_G6A2]+delta) {                          \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2]+g6red[CS6M_G6C2]-g6red[CS6M_G62AC];\
          g6red[CS6M_G62AC] = -2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AC];            \
          g6red[CS6M_G62BC] = g6red[CS6M_G62BC]-g6red[CS6M_G62AB];               \
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R65P,mat66,mat66out);                   \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bd9+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("9+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62AC] < -g6red[CS6M_G6A2]-delta) {                         \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2]+g6red[CS6M_G6C2]+g6red[CS6M_G62AC];\
          g6red[CS6M_G62AC] = 2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AC];             \
          g6red[CS6M_G62BC] = g6red[CS6M_G62BC]+g6red[CS6M_G62AB];               \
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R65M,mat66,mat66out);                   \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bdB+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("B+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /*  CDE boundaries outside */                                              \
      if (g6red[CS6M_G62AB] > g6red[CS6M_G6A2]+delta) {                          \
          g6red[CS6M_G6B2] = g6red[CS6M_G6A2]+g6red[CS6M_G6B2]-g6red[CS6M_G62AB];\
          g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]+g6red[CS6M_G62AC];              \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC];                                \
          g6red[CS6M_G62AB] = -2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AB];            \
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R76P,mat66,mat66out);                   \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bdC+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("C+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62AB] < -g6red[CS6M_G6A2]-delta) {                         \
          g6red[CS6M_G6B2] = g6red[CS6M_G6A2]+g6red[CS6M_G6B2]+g6red[CS6M_G62AB];\
          g6red[CS6M_G62BC] =-g6red[CS6M_G62BC]-g6red[CS6M_G62AC];               \
          g6red[CS6M_G62AB] = 2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AB];             \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC];                                \
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R76M,mat66,mat66out);                   \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bdE+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("E+ ",g6red);                                            \
         redpass++; continue;                                                    \
      }                                                                          \
      /* F+ boundary outside */                                                  \
      if (g6red[CS6M_G6A2] + g6red[CS6M_G6B2] + g6red[CS6M_G62BC]                \
          + g6red[CS6M_G62AC] + g6red[CS6M_G62AB] < -delta) {                    \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2] + g6red[CS6M_G6B2]                 \
          + g6red[CS6M_G6C2] + g6red[CS6M_G62BC] + g6red[CS6M_G62AC] + g6red[CS6M_G62AB]; \
          g6red[CS6M_G62AC] = -2.*g6red[CS6M_G6A2] - g6red[CS6M_G62AC] - g6red[CS6M_G62AB];\
          g6red[CS6M_G62BC] = -2.*g6red[CS6M_G6B2] - g6red[CS6M_G62BC] - g6red[CS6M_G62AB];\
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R8,mat66,mat66out);                     \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bdF+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("F+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /* 678 boundaries exact */                                                 \
      if (g6red[CS6M_G62BC] ==  g6red[CS6M_G6B2] && 2.*g6red[CS6M_G62AC] < g6red[CS6M_G62AB]) { \
          g6red[CS6M_G6C2] = g6red[CS6M_G6B2]+g6red[CS6M_G6C2]-g6red[CS6M_G62BC];\
          g6red[CS6M_G62BC] = -2.*g6red[CS6M_G6B2]+g6red[CS6M_G62BC];            \
          g6red[CS6M_G62AC] = g6red[CS6M_G62AC]-g6red[CS6M_G62AB];               \
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R54P,mat66,mat66out);                   \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bd6 "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("6= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (fabs(g6red[CS6M_G62BC] + g6red[CS6M_G6B2]) < delta && g6red[CS6M_G62AB] < -delta) {    \
          g6red[CS6M_G6C2] = g6red[CS6M_G6B2]+g6red[CS6M_G6C2]+g6red[CS6M_G62BC];\
          g6red[CS6M_G62BC] = 2.*g6red[CS6M_G6B2]+g6red[CS6M_G62BC];             \
          g6red[CS6M_G62AC] = g6red[CS6M_G62AC]+g6red[CS6M_G62AB];               \
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R54M,mat66,mat66out);                   \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bd8 "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "<<    \
          g6red[3 ]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/             \
          CS6M_VOLCHECK("8= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /* 9AB boundaries exsct */                                                 \
      if (fabs(g6red[CS6M_G62AC] - g6red[CS6M_G6A2]) < delta && 2.*g6red[CS6M_G62BC] < g6red[CS6M_G62AB]-delta) { \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2]+g6red[CS6M_G6C2]-g6red[CS6M_G62AC];\
          g6red[CS6M_G62AC] = -2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AC];            \
          g6red[CS6M_G62BC] = g6red[CS6M_G62BC]-g6red[CS6M_G62AB];               \
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R65P,mat66,mat66out);                   \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bd9 "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("9= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62AC] < -g6red[CS6M_G6A2]-delta && g6red[CS6M_G62AB] < -delta) {     \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2]+g6red[CS6M_G6C2]+g6red[CS6M_G62AC];\
          g6red[CS6M_G62AC] = 2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AC];             \
          g6red[CS6M_G62BC] = g6red[CS6M_G62BC]+g6red[CS6M_G62AB];               \
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R65M,mat66,mat66out);                   \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bdB "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("B= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /*  CDE boundaries exact */                                                \
      if (fabs(g6red[CS6M_G62AB] - g6red[CS6M_G6A2]) < delta &&                  \
          2.*g6red[CS6M_G62BC] < g6red[CS6M_G62AC]-delta) {                      \
          g6red[CS6M_G6B2] = g6red[CS6M_G6A2]+g6red[CS6M_G6B2]-g6red[CS6M_G62AB];\
          g6red[CS6M_G62AB] = -2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AB];            \
          g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]+g6red[CS6M_G62AC];              \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC];                                \
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R76P,mat66,mat66out);                   \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bdC "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("C= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62AB] < -g6red[CS6M_G6A2] -delta &&                        \
          g6red[CS6M_G62AB] < -delta) {                                          \
          g6red[CS6M_G6B2] = g6red[CS6M_G6A2]+g6red[CS6M_G6B2]+g6red[CS6M_G62AB];\
          g6red[CS6M_G62AB] = 2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AB];             \
          g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]-g6red[CS6M_G62AC];              \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC];                                \
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R76M,mat66,mat66out);                   \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bdE "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("E= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /* F boundary exact */                                                     \
      if (fabs(g6red[CS6M_G6A2] + g6red[CS6M_G6B2] + g6red[CS6M_G62BC]           \
          + g6red[CS6M_G62AC] + g6red[CS6M_G62AB]) < -delta &&                   \
          2.*g6red[CS6M_G6A2]+2.*g6red[CS6M_G62AC]+g6red[CS6M_G62AB] > delta) {  \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2] + g6red[CS6M_G6B2]                 \
          + g6red[CS6M_G6C2] + g6red[CS6M_G62BC] + g6red[CS6M_G62AC] + g6red[CS6M_G62AB]; \
          g6red[CS6M_G62AC] = -2.*g6red[CS6M_G6A2] - g6red[CS6M_G62AC] - g6red[CS6M_G62AB];\
          g6red[CS6M_G62BC] = -2.*g6red[CS6M_G6B2] - g6red[CS6M_G62BC] - g6red[CS6M_G62AB];\
          CS6M_Mat66_Mat66_Mult(CS6M_MAT_R8,mat66,mat66out);                     \
          for(ii=0;ii<36;ii++) mat66[ii]=mat66out[ii];                           \
          /* std::cout<<"bdF "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("F= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      notdone = 0;                                                               \
      reduced = 1;                                                               \
    }                                                                            \
  }

  #define CS6M_G6Reduce_unscaled(g6unred,g6red,reduced) { \
    double s6in[6];                           \
    double s6out[6];                          \
    int notdone;                              \
    int ii;                                   \
    notdone = 1;                              \
    int redpass;                              \
    double temp;                              \
    double delta = 0;                         \
    CS6M_VOLCHECK_PREP(g6unred);              \
    CS6M_G6toS6(g6unred,s6in);                \
    reduced=1;                                \
    CS6M_S6Reduce(s6in,s6out,reduced);        \
    if (reduced) {                            \
       CS6M_S6toG6(s6out,g6red);              \
    } else {                                  \
      for (ii=0; ii < 6; ii++) g6red[ii]=g6unred[ii];\
    }                                         \
    delta=g6unred[2];                         \
    CS6M_VOLCHECK("S6",g6red);                \
    if (g6unred[1] > delta) delta=g6unred[1]; \
    if (g6unred[0] > delta) delta=g6unred[0]; \
    delta = delta * 1.e-12;                   \
    if (delta < 1.e-12) delta=1.e-12;         \
    if (fabs(g6red[CS6M_G62BC])  < delta ) g6red[CS6M_G62BC] = 0;      \
    if (fabs(g6red[CS6M_G62AC])  < delta ) g6red[CS6M_G62AC] = 0;      \
    if (fabs(g6red[CS6M_G62AB])  < delta ) g6red[CS6M_G62AB] = 0;      \
                                                                       \
    redpass=0;                                                         \
    while ( notdone && redpass < 1000) {                               \
    if (g6red[CS6M_G6A2] < 0.                                          \
        || g6red[CS6M_G6B2] < 0.                                       \
        || g6red[CS6M_G6C2] < 0.                                       \
        || g6red[CS6M_G6B2] + g6red[CS6M_G6C2] + g6red[CS6M_G62BC] < 0.\
        || g6red[CS6M_G6A2] + g6red[CS6M_G6C2] + g6red[CS6M_G62AC] < 0.\
        || g6red[CS6M_G6A2] + g6red[CS6M_G6B2] + g6red[CS6M_G62AB] < 0.\
        || g6red[CS6M_G6B2] + g6red[CS6M_G6C2] - g6red[CS6M_G62BC] < 0.\
        || g6red[CS6M_G6A2] + g6red[CS6M_G6C2] - g6red[CS6M_G62AC] < 0.\
        || g6red[CS6M_G6A2] + g6red[CS6M_G6B2] - g6red[CS6M_G62AB] < 0.\
    ) {                                                                \
      notdone=0; break;                                                \
    }                                                                  \
      /* g1 < g2 or g1=g2 && |g4| < |g5| */                            \
      if ( g6red[CS6M_G6A2] > g6red[CS6M_G6B2]+delta                   \
           || ( CS6M_abs(g6red[CS6M_G6A2] - g6red[CS6M_G6B2]) < delta      \
           && CS6M_abs(g6red[CS6M_G62BC]) > CS6M_abs(g6red[CS6M_G62AC])+delta) ) { \
        temp = g6red[CS6M_G6A2];                                 \
        g6red[CS6M_G6A2] = g6red[CS6M_G6B2];                     \
        g6red[CS6M_G6B2] = temp;                                 \
        temp = g6red[CS6M_G62BC];                                \
        g6red[CS6M_G62BC] = g6red[CS6M_G62AC];                   \
        g6red[CS6M_G62AC] = temp;                                \
        /* std::cout<<"bd1 "<<g6red[0]<<" "<<g6red[1]<<" "       \
        <<g6red[2]<<" "<<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]  \
        <<" "<<std::endl; */                                     \
        CS6M_VOLCHECK("swap a b ",g6red);                        \
        redpass++; continue;                                     \
      }                                                          \
      /* g2 < g3 or g2=g3 && |g5| < |g6| */                      \
      if ( g6red[CS6M_G6B2] > g6red[CS6M_G6C2]+delta             \
            ||  (fabs(g6red[CS6M_G6B2] - g6red[CS6M_G6C2]) < delta \
            && CS6M_abs(g6red[CS6M_G62AC]) > CS6M_abs(g6red[CS6M_G62AB])+delta) ) {\
        temp = g6red[CS6M_G6B2];                                 \
        g6red[CS6M_G6B2] = g6red[CS6M_G6C2];                     \
        g6red[CS6M_G6C2] = temp;                                 \
        temp = g6red[CS6M_G62AC];                                \
        g6red[CS6M_G62AC] = g6red[CS6M_G62AB];                   \
        g6red[CS6M_G62AB] = temp;                                \
        CS6M_VOLCHECK("swap b c ",g6red);                        \
        /* std::cout<<"bd2 "<<g6red[0]<<" "<<g6red[1]<<" "       \
        <<g6red[2]<<" "<<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]  \
        <<" "<<std::endl; */                                     \
        redpass++; continue;                                     \
      }                                                          \
      if ( g6red[CS6M_G62BC]*g6red[CS6M_G62AC]*g6red[CS6M_G62AB] > 0. ) { \
        if (g6red[CS6M_G62BC] < 0. || g6red[CS6M_G62AC] < 0.     \
          || g6red[CS6M_G62AC] < 0. ) {                          \
          if ( g6red[CS6M_G62BC] < 0. ) g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]; \
          if ( g6red[CS6M_G62AC] < 0. ) g6red[CS6M_G62AC] = -g6red[CS6M_G62AC]; \
          if ( g6red[CS6M_G62AB] < 0. ) g6red[CS6M_G62AB] = -g6red[CS6M_G62AB]; \
          /* std::cout<<"+++ "<<g6red[0]<<" "<<g6red[1]<<" "     \
          <<g6red[2]<<" "<<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]\
          <<" "<<std::endl; */                                   \
          CS6M_VOLCHECK("--- ",g6red);                           \
          redpass++; continue;                                   \
        }                                                        \
      } else {                                                   \
        if (g6red[CS6M_G62BC] > 0. || g6red[CS6M_G62AC] > 0.     \
          || g6red[CS6M_G62AC] > 0. ) {                          \
          if ( g6red[CS6M_G62BC] > 0. ) g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]; \
          if ( g6red[CS6M_G62AC] > 0. ) g6red[CS6M_G62AC] = -g6red[CS6M_G62AC]; \
          if ( g6red[CS6M_G62AB] > 0. ) g6red[CS6M_G62AB] = -g6red[CS6M_G62AB]; \
          /* std::cout<<"--- "<<g6red[0]<<" "<<g6red[1]<<" "     \
          <<g6red[2]<<" "<<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]\
          <<" "<<std::endl; */                                   \
          CS6M_VOLCHECK("+++ ",g6red);                           \
          redpass++; continue;                                   \
        }                                                        \
      }                                                          \
                                                                 \
      /* 678 boundaries outside */                               \
      if (g6red[CS6M_G62BC] >  g6red[CS6M_G6B2]+delta) {         \
          g6red[CS6M_G6C2] = g6red[CS6M_G6B2]+g6red[CS6M_G6C2]-g6red[CS6M_G62BC];\
          g6red[CS6M_G62BC] = -2.*g6red[CS6M_G6B2]+g6red[CS6M_G62BC];            \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC]+g6red[CS6M_G62AB];               \
          /* std::cout<<"bd6+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("6+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62BC] < -g6red[CS6M_G6B2]-delta) {                         \
          g6red[CS6M_G6C2] = g6red[CS6M_G6B2]+g6red[CS6M_G6C2]+g6red[CS6M_G62BC];\
          g6red[CS6M_G62BC] = 2.*g6red[CS6M_G6B2]+g6red[CS6M_G62BC];             \
          g6red[CS6M_G62AC] = g6red[CS6M_G62AC]+g6red[CS6M_G62AB];               \
          /* std::cout<<"bd8+"<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3 ]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/           \
          CS6M_VOLCHECK("8+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /* 9AB boundaries outside */                                               \
      if (g6red[CS6M_G62AC] > g6red[CS6M_G6A2]+delta) {                          \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2]+g6red[CS6M_G6C2]-g6red[CS6M_G62AC];\
          g6red[CS6M_G62AC] = -2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AC];            \
          g6red[CS6M_G62BC] = g6red[CS6M_G62BC]-g6red[CS6M_G62AB];               \
          /* std::cout<<"bd9+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("9+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62AC] < -g6red[CS6M_G6A2]-delta) {                         \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2]+g6red[CS6M_G6C2]+g6red[CS6M_G62AC];\
          g6red[CS6M_G62AC] = 2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AC];             \
          g6red[CS6M_G62BC] = g6red[CS6M_G62BC]+g6red[CS6M_G62AB];               \
          /* std::cout<<"bdB+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("B+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /*  CDE boundaries outside */                                              \
      if (g6red[CS6M_G62AB] > g6red[CS6M_G6A2]+delta) {                          \
          g6red[CS6M_G6B2] = g6red[CS6M_G6A2]+g6red[CS6M_G6B2]-g6red[CS6M_G62AB];\
          g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]+g6red[CS6M_G62AC];              \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC];                                \
          g6red[CS6M_G62AB] = -2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AB];            \
          /* std::cout<<"bdC+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("C+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62AB] < -g6red[CS6M_G6A2]-delta) {                         \
          g6red[CS6M_G6B2] = g6red[CS6M_G6A2]+g6red[CS6M_G6B2]+g6red[CS6M_G62AB];\
          g6red[CS6M_G62BC] =-g6red[CS6M_G62BC]-g6red[CS6M_G62AC];               \
          g6red[CS6M_G62AB] = 2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AB];             \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC];                                \
          /* std::cout<<"bdE+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("E+ ",g6red);                                            \
         redpass++; continue;                                                    \
      }                                                                          \
      /* F+ boundary outside */                                                  \
      if (g6red[CS6M_G6A2] + g6red[CS6M_G6B2] + g6red[CS6M_G62BC]                \
          + g6red[CS6M_G62AC] + g6red[CS6M_G62AB] < -delta) {                    \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2] + g6red[CS6M_G6B2]                 \
          + g6red[CS6M_G6C2] + g6red[CS6M_G62BC] + g6red[CS6M_G62AC] + g6red[CS6M_G62AB]; \
          g6red[CS6M_G62AC] = -2.*g6red[CS6M_G6A2] - g6red[CS6M_G62AC] - g6red[CS6M_G62AB];\
          g6red[CS6M_G62BC] = -2.*g6red[CS6M_G6B2] - g6red[CS6M_G62BC] - g6red[CS6M_G62AB];\
          /* std::cout<<"bdF+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("F+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /* 678 boundaries exact */                                                 \
      if (g6red[CS6M_G62BC] ==  g6red[CS6M_G6B2] && 2.*g6red[CS6M_G62AC] < g6red[CS6M_G62AB]) { \
          g6red[CS6M_G6C2] = g6red[CS6M_G6B2]+g6red[CS6M_G6C2]-g6red[CS6M_G62BC];\
          g6red[CS6M_G62BC] = -2.*g6red[CS6M_G6B2]+g6red[CS6M_G62BC];            \
          g6red[CS6M_G62AC] = g6red[CS6M_G62AC]-g6red[CS6M_G62AB];               \
          /* std::cout<<"bd6 "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("6= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (fabs(g6red[CS6M_G62BC] + g6red[CS6M_G6B2]) < delta && g6red[CS6M_G62AB] < -delta) {    \
          g6red[CS6M_G6C2] = g6red[CS6M_G6B2]+g6red[CS6M_G6C2]+g6red[CS6M_G62BC];\
          g6red[CS6M_G62BC] = 2.*g6red[CS6M_G6B2]+g6red[CS6M_G62BC];             \
          g6red[CS6M_G62AC] = g6red[CS6M_G62AC]+g6red[CS6M_G62AB];               \
          /* std::cout<<"bd8 "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "<<    \
          g6red[3 ]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/             \
          CS6M_VOLCHECK("8= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /* 9AB boundaries exsct */                                                 \
      if (fabs(g6red[CS6M_G62AC] - g6red[CS6M_G6A2]) < delta && 2.*g6red[CS6M_G62BC] < g6red[CS6M_G62AB]-delta) { \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2]+g6red[CS6M_G6C2]-g6red[CS6M_G62AC];\
          g6red[CS6M_G62AC] = -2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AC];            \
          g6red[CS6M_G62BC] = g6red[CS6M_G62BC]-g6red[CS6M_G62AB];               \
          /* std::cout<<"bd9 "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("9= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62AC] < -g6red[CS6M_G6A2]-delta && g6red[CS6M_G62AB] < -delta) {     \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2]+g6red[CS6M_G6C2]+g6red[CS6M_G62AC];\
          g6red[CS6M_G62AC] = 2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AC];             \
          g6red[CS6M_G62BC] = g6red[CS6M_G62BC]+g6red[CS6M_G62AB];               \
          /* std::cout<<"bdB "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("B= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /*  CDE boundaries exact */                                                \
      if (fabs(g6red[CS6M_G62AB] - g6red[CS6M_G6A2]) < delta &&                  \
          2.*g6red[CS6M_G62BC] < g6red[CS6M_G62AC]-delta) {                      \
          g6red[CS6M_G6B2] = g6red[CS6M_G6A2]+g6red[CS6M_G6B2]-g6red[CS6M_G62AB];\
          g6red[CS6M_G62AB] = -2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AB];            \
          g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]+g6red[CS6M_G62AC];              \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC];                                \
          /* std::cout<<"bdC "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("C= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62AB] < -g6red[CS6M_G6A2] -delta &&                        \
          g6red[CS6M_G62AB] < -delta) {                                          \
          g6red[CS6M_G6B2] = g6red[CS6M_G6A2]+g6red[CS6M_G6B2]+g6red[CS6M_G62AB];\
          g6red[CS6M_G62AB] = 2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AB];             \
          g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]-g6red[CS6M_G62AC];              \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC];                                \
          /* std::cout<<"bdE "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("E= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /* F boundary exact */                                                     \
      if (fabs(g6red[CS6M_G6A2] + g6red[CS6M_G6B2] + g6red[CS6M_G62BC]           \
          + g6red[CS6M_G62AC] + g6red[CS6M_G62AB]) < -delta &&                   \
          2.*g6red[CS6M_G6A2]+2.*g6red[CS6M_G62AC]+g6red[CS6M_G62AB] > delta) {  \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2] + g6red[CS6M_G6B2]                 \
          + g6red[CS6M_G6C2] + g6red[CS6M_G62BC] + g6red[CS6M_G62AC] + g6red[CS6M_G62AB]; \
          g6red[CS6M_G62AC] = -2.*g6red[CS6M_G6A2] - g6red[CS6M_G62AC] - g6red[CS6M_G62AB];\
          g6red[CS6M_G62BC] = -2.*g6red[CS6M_G6B2] - g6red[CS6M_G62BC] - g6red[CS6M_G62AB];\
          /* std::cout<<"bdF "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("F= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      notdone = 0;                                                               \
      reduced = 1;                                                               \
    }                                                                            \
  }


  #define CS6M_S6Scaled(s6vec,s6vecscaled,scale) { \
    int ii;                                   \
    double szmax, szmin, sztmp;               \
    double d7vec[7];                          \
    CS6M_S6toD7(s6vec,d7vec)                  \
    scale = 1.;                               \
    szmin=szmax = fabs(d7vec[0]);             \
    szmin = szmax;                            \
    for (ii=1;ii<4;ii++) {                    \
      sztmp = fabs(d7vec[ii]);                \
      if (sztmp > szmax) szmax = sztmp;       \
      if (sztmp < szmin && sztmp > 1.e-20) szmin = sztmp;    \
    }                                                        \
    if (szmax <= 1.e-20 ) szmax=szmin=0;                     \
    if (szmin > 1.e20 ) {                                    \
      scale = szmin;                                         \
      for (ii=0;ii<6;ii++) { s6vecscaled[ii] = s6vec[ii]/scale;}\
    } else {                                                 \
      for (ii=0;ii<6;ii++) { s6vecscaled[ii] = s6vec[ii]; }  \
    }                                                        \
  }

  #define CS6M_G6Scaled(g6vec,g6vecscaled,scale) { \
    int ii;                                   \
    double szmax, szmin, sztmp;               \
    scale = 1.;                               \
    szmin=szmax = fabs(g6vec[0]);             \
    szmin = szmax;                            \
    for (ii=1;ii<3;ii++) {                    \
      sztmp = fabs(g6vec[ii]);                \
      if (sztmp > szmax) szmax = sztmp;       \
      if (sztmp < szmin && sztmp > 1.e-20) szmin = sztmp;    \
    }                                                        \
    if (szmax <= 1.e-20 ) szmax=szmin=0;                     \
    if (szmin > 1.e20 ) {                                    \
      scale = szmin;                                         \
      for (ii=0;ii<6;ii++) { g6vecscaled[ii] = g6vec[ii]/scale; } \
    } else {                                                 \
      for (ii=0;ii<6;ii++) { g6vecscaled[ii] = g6vec[ii]; }  \
    }                                         \
  }

  #define CS6M_G6Reduce(g6unred,g6red,reduced) { \
    double s6in[6];                           \
    double s6out[6];                          \
    int notdone;                              \
    int ii;                                   \
    notdone = 1;                              \
    int redpass;                              \
    double temp;                              \
    double delta = 0;                         \
    double scale;                             \
    double g6scaled[6];                       \
    CS6M_G6Scaled(g6unred,g6scaled,scale);    \
    CS6M_VOLCHECK_PREP(g6scaled);             \
    CS6M_G6toS6(g6scaled,s6in);               \
    reduced=1;                                \
    CS6M_S6Reduce(s6in,s6out,reduced);        \
    if (reduced) {                            \
       CS6M_S6toG6(s6out,g6red);              \
    } else {                                  \
      for (ii=0; ii < 6; ii++) g6red[ii]=g6scaled[ii];\
    }                                         \
    delta=g6scaled[2];                        \
    CS6M_VOLCHECK("S6",g6red);                \
    if (g6scaled[1] > delta) delta=g6scaled[1]; \
    if (g6scaled[0] > delta) delta=g6scaled[0]; \
    delta = delta * 1.e-12;                   \
    if (delta < 1.e-12) delta=1.e-12;         \
    if (fabs(g6red[CS6M_G62BC])  < delta ) g6red[CS6M_G62BC] = 0;      \
    if (fabs(g6red[CS6M_G62AC])  < delta ) g6red[CS6M_G62AC] = 0;      \
    if (fabs(g6red[CS6M_G62AB])  < delta ) g6red[CS6M_G62AB] = 0;      \
                                                                       \
    redpass=0;                                                         \
    while ( notdone && redpass < 1000) {                               \
    if (g6red[CS6M_G6A2] < 0.                                          \
        || g6red[CS6M_G6B2] < 0.                                       \
        || g6red[CS6M_G6C2] < 0.                                       \
        || g6red[CS6M_G6B2] + g6red[CS6M_G6C2] + g6red[CS6M_G62BC] < 0.\
        || g6red[CS6M_G6A2] + g6red[CS6M_G6C2] + g6red[CS6M_G62AC] < 0.\
        || g6red[CS6M_G6A2] + g6red[CS6M_G6B2] + g6red[CS6M_G62AB] < 0.\
        || g6red[CS6M_G6B2] + g6red[CS6M_G6C2] - g6red[CS6M_G62BC] < 0.\
        || g6red[CS6M_G6A2] + g6red[CS6M_G6C2] - g6red[CS6M_G62AC] < 0.\
        || g6red[CS6M_G6A2] + g6red[CS6M_G6B2] - g6red[CS6M_G62AB] < 0.\
    ) {                                                                \
      notdone=0; break;                                                \
    }                                                                  \
      /* g1 < g2 or g1=g2 && |g4| < |g5| */                            \
      if ( g6red[CS6M_G6A2] > g6red[CS6M_G6B2]+delta                   \
           || ( CS6M_abs(g6red[CS6M_G6A2] - g6red[CS6M_G6B2]) < delta      \
           && CS6M_abs(g6red[CS6M_G62BC]) > CS6M_abs(g6red[CS6M_G62AC])+delta) ) { \
        temp = g6red[CS6M_G6A2];                                 \
        g6red[CS6M_G6A2] = g6red[CS6M_G6B2];                     \
        g6red[CS6M_G6B2] = temp;                                 \
        temp = g6red[CS6M_G62BC];                                \
        g6red[CS6M_G62BC] = g6red[CS6M_G62AC];                   \
        g6red[CS6M_G62AC] = temp;                                \
        /* std::cout<<"bd1 "<<g6red[0]<<" "<<g6red[1]<<" "       \
        <<g6red[2]<<" "<<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]  \
        <<" "<<std::endl; */                                     \
        CS6M_VOLCHECK("swap a b ",g6red);                        \
        redpass++; continue;                                     \
      }                                                          \
      /* g2 < g3 or g2=g3 && |g5| < |g6| */                      \
      if ( g6red[CS6M_G6B2] > g6red[CS6M_G6C2]+delta             \
            ||  (fabs(g6red[CS6M_G6B2] - g6red[CS6M_G6C2]) < delta \
            && CS6M_abs(g6red[CS6M_G62AC]) > CS6M_abs(g6red[CS6M_G62AB])+delta) ) {\
        temp = g6red[CS6M_G6B2];                                 \
        g6red[CS6M_G6B2] = g6red[CS6M_G6C2];                     \
        g6red[CS6M_G6C2] = temp;                                 \
        temp = g6red[CS6M_G62AC];                                \
        g6red[CS6M_G62AC] = g6red[CS6M_G62AB];                   \
        g6red[CS6M_G62AB] = temp;                                \
        CS6M_VOLCHECK("swap b c ",g6red);                        \
        /* std::cout<<"bd2 "<<g6red[0]<<" "<<g6red[1]<<" "       \
        <<g6red[2]<<" "<<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]  \
        <<" "<<std::endl; */                                     \
        redpass++; continue;                                     \
      }                                                          \
      if ( g6red[CS6M_G62BC]*g6red[CS6M_G62AC]*g6red[CS6M_G62AB] > 0. ) { \
        if (g6red[CS6M_G62BC] < 0. || g6red[CS6M_G62AC] < 0.     \
          || g6red[CS6M_G62AC] < 0. ) {                          \
          if ( g6red[CS6M_G62BC] < 0. ) g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]; \
          if ( g6red[CS6M_G62AC] < 0. ) g6red[CS6M_G62AC] = -g6red[CS6M_G62AC]; \
          if ( g6red[CS6M_G62AB] < 0. ) g6red[CS6M_G62AB] = -g6red[CS6M_G62AB]; \
          /* std::cout<<"+++ "<<g6red[0]<<" "<<g6red[1]<<" "     \
          <<g6red[2]<<" "<<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]\
          <<" "<<std::endl; */                                   \
          CS6M_VOLCHECK("--- ",g6red);                           \
          redpass++; continue;                                   \
        }                                                        \
      } else {                                                   \
        if (g6red[CS6M_G62BC] > 0. || g6red[CS6M_G62AC] > 0.     \
          || g6red[CS6M_G62AC] > 0. ) {                          \
          if ( g6red[CS6M_G62BC] > 0. ) g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]; \
          if ( g6red[CS6M_G62AC] > 0. ) g6red[CS6M_G62AC] = -g6red[CS6M_G62AC]; \
          if ( g6red[CS6M_G62AB] > 0. ) g6red[CS6M_G62AB] = -g6red[CS6M_G62AB]; \
          /* std::cout<<"--- "<<g6red[0]<<" "<<g6red[1]<<" "     \
          <<g6red[2]<<" "<<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]\
          <<" "<<std::endl; */                                   \
          CS6M_VOLCHECK("+++ ",g6red);                           \
          redpass++; continue;                                   \
        }                                                        \
      }                                                          \
                                                                 \
      /* 678 boundaries outside */                               \
      if (g6red[CS6M_G62BC] >  g6red[CS6M_G6B2]+delta) {         \
          g6red[CS6M_G6C2] = g6red[CS6M_G6B2]+g6red[CS6M_G6C2]-g6red[CS6M_G62BC];\
          g6red[CS6M_G62BC] = -2.*g6red[CS6M_G6B2]+g6red[CS6M_G62BC];            \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC]+g6red[CS6M_G62AB];               \
          /* std::cout<<"bd6+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("6+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62BC] < -g6red[CS6M_G6B2]-delta) {                         \
          g6red[CS6M_G6C2] = g6red[CS6M_G6B2]+g6red[CS6M_G6C2]+g6red[CS6M_G62BC];\
          g6red[CS6M_G62BC] = 2.*g6red[CS6M_G6B2]+g6red[CS6M_G62BC];             \
          g6red[CS6M_G62AC] = g6red[CS6M_G62AC]+g6red[CS6M_G62AB];               \
          /* std::cout<<"bd8+"<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3 ]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/           \
          CS6M_VOLCHECK("8+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /* 9AB boundaries outside */                                               \
      if (g6red[CS6M_G62AC] > g6red[CS6M_G6A2]+delta) {                          \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2]+g6red[CS6M_G6C2]-g6red[CS6M_G62AC];\
          g6red[CS6M_G62AC] = -2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AC];            \
          g6red[CS6M_G62BC] = g6red[CS6M_G62BC]-g6red[CS6M_G62AB];               \
          /* std::cout<<"bd9+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("9+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62AC] < -g6red[CS6M_G6A2]-delta) {                         \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2]+g6red[CS6M_G6C2]+g6red[CS6M_G62AC];\
          g6red[CS6M_G62AC] = 2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AC];             \
          g6red[CS6M_G62BC] = g6red[CS6M_G62BC]+g6red[CS6M_G62AB];               \
          /* std::cout<<"bdB+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("B+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /*  CDE boundaries outside */                                              \
      if (g6red[CS6M_G62AB] > g6red[CS6M_G6A2]+delta) {                          \
          g6red[CS6M_G6B2] = g6red[CS6M_G6A2]+g6red[CS6M_G6B2]-g6red[CS6M_G62AB];\
          g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]+g6red[CS6M_G62AC];              \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC];                                \
          g6red[CS6M_G62AB] = -2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AB];            \
          /* std::cout<<"bdC+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("C+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62AB] < -g6red[CS6M_G6A2]-delta) {                         \
          g6red[CS6M_G6B2] = g6red[CS6M_G6A2]+g6red[CS6M_G6B2]+g6red[CS6M_G62AB];\
          g6red[CS6M_G62BC] =-g6red[CS6M_G62BC]-g6red[CS6M_G62AC];               \
          g6red[CS6M_G62AB] = 2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AB];             \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC];                                \
          /* std::cout<<"bdE+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("E+ ",g6red);                                            \
         redpass++; continue;                                                    \
      }                                                                          \
      /* F+ boundary outside */                                                  \
      if (g6red[CS6M_G6A2] + g6red[CS6M_G6B2] + g6red[CS6M_G62BC]                \
          + g6red[CS6M_G62AC] + g6red[CS6M_G62AB] < -delta) {                    \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2] + g6red[CS6M_G6B2]                 \
          + g6red[CS6M_G6C2] + g6red[CS6M_G62BC] + g6red[CS6M_G62AC] + g6red[CS6M_G62AB]; \
          g6red[CS6M_G62AC] = -2.*g6red[CS6M_G6A2] - g6red[CS6M_G62AC] - g6red[CS6M_G62AB];\
          g6red[CS6M_G62BC] = -2.*g6red[CS6M_G6B2] - g6red[CS6M_G62BC] - g6red[CS6M_G62AB];\
          /* std::cout<<"bdF+ "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "     \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("F+ ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /* 678 boundaries exact */                                                 \
      if (g6red[CS6M_G62BC] ==  g6red[CS6M_G6B2] && 2.*g6red[CS6M_G62AC] < g6red[CS6M_G62AB]) { \
          g6red[CS6M_G6C2] = g6red[CS6M_G6B2]+g6red[CS6M_G6C2]-g6red[CS6M_G62BC];\
          g6red[CS6M_G62BC] = -2.*g6red[CS6M_G6B2]+g6red[CS6M_G62BC];            \
          g6red[CS6M_G62AC] = g6red[CS6M_G62AC]-g6red[CS6M_G62AB];               \
          /* std::cout<<"bd6 "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("6= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (fabs(g6red[CS6M_G62BC] + g6red[CS6M_G6B2]) < delta && g6red[CS6M_G62AB] < -delta) {    \
          g6red[CS6M_G6C2] = g6red[CS6M_G6B2]+g6red[CS6M_G6C2]+g6red[CS6M_G62BC];\
          g6red[CS6M_G62BC] = 2.*g6red[CS6M_G6B2]+g6red[CS6M_G62BC];             \
          g6red[CS6M_G62AC] = g6red[CS6M_G62AC]+g6red[CS6M_G62AB];               \
          /* std::cout<<"bd8 "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "<<    \
          g6red[3 ]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/             \
          CS6M_VOLCHECK("8= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /* 9AB boundaries exsct */                                                 \
      if (fabs(g6red[CS6M_G62AC] - g6red[CS6M_G6A2]) < delta && 2.*g6red[CS6M_G62BC] < g6red[CS6M_G62AB]-delta) { \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2]+g6red[CS6M_G6C2]-g6red[CS6M_G62AC];\
          g6red[CS6M_G62AC] = -2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AC];            \
          g6red[CS6M_G62BC] = g6red[CS6M_G62BC]-g6red[CS6M_G62AB];               \
          /* std::cout<<"bd9 "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("9= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62AC] < -g6red[CS6M_G6A2]-delta && g6red[CS6M_G62AB] < -delta) {     \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2]+g6red[CS6M_G6C2]+g6red[CS6M_G62AC];\
          g6red[CS6M_G62AC] = 2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AC];             \
          g6red[CS6M_G62BC] = g6red[CS6M_G62BC]+g6red[CS6M_G62AB];               \
          /* std::cout<<"bdB "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("B= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /*  CDE boundaries exact */                                                \
      if (fabs(g6red[CS6M_G62AB] - g6red[CS6M_G6A2]) < delta &&                  \
          2.*g6red[CS6M_G62BC] < g6red[CS6M_G62AC]-delta) {                      \
          g6red[CS6M_G6B2] = g6red[CS6M_G6A2]+g6red[CS6M_G6B2]-g6red[CS6M_G62AB];\
          g6red[CS6M_G62AB] = -2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AB];            \
          g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]+g6red[CS6M_G62AC];              \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC];                                \
          /* std::cout<<"bdC "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("C= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      if (g6red[CS6M_G62AB] < -g6red[CS6M_G6A2] -delta &&                        \
          g6red[CS6M_G62AB] < -delta) {                                          \
          g6red[CS6M_G6B2] = g6red[CS6M_G6A2]+g6red[CS6M_G6B2]+g6red[CS6M_G62AB];\
          g6red[CS6M_G62AB] = 2.*g6red[CS6M_G6A2]+g6red[CS6M_G62AB];             \
          g6red[CS6M_G62BC] = -g6red[CS6M_G62BC]-g6red[CS6M_G62AC];              \
          g6red[CS6M_G62AC] = -g6red[CS6M_G62AC];                                \
          /* std::cout<<"bdE "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("E= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      /* F boundary exact */                                                     \
      if (fabs(g6red[CS6M_G6A2] + g6red[CS6M_G6B2] + g6red[CS6M_G62BC]           \
          + g6red[CS6M_G62AC] + g6red[CS6M_G62AB]) < -delta &&                   \
          2.*g6red[CS6M_G6A2]+2.*g6red[CS6M_G62AC]+g6red[CS6M_G62AB] > delta) {  \
          g6red[CS6M_G6C2] = g6red[CS6M_G6A2] + g6red[CS6M_G6B2]                 \
          + g6red[CS6M_G6C2] + g6red[CS6M_G62BC] + g6red[CS6M_G62AC] + g6red[CS6M_G62AB]; \
          g6red[CS6M_G62AC] = -2.*g6red[CS6M_G6A2] - g6red[CS6M_G62AC] - g6red[CS6M_G62AB];\
          g6red[CS6M_G62BC] = -2.*g6red[CS6M_G6B2] - g6red[CS6M_G62BC] - g6red[CS6M_G62AB];\
          /* std::cout<<"bdF "<<g6red[0]<<" "<<g6red[1]<<" "<<g6red[2]<<" "      \
          <<g6red[3]<<" "<<g6red[4]<<" "<<g6red[5]<<" "<<std::endl;*/            \
          CS6M_VOLCHECK("F= ",g6red);                                            \
          redpass++; continue;                                                   \
      }                                                                          \
      notdone = 0;                                                               \
      reduced = 1;                                                               \
    }                                                                            \
    for (ii=0; ii < 6; ii++) {g6red[ii]*=scale;}                                 \
  }

  static double CS6M_Reduce11_RedMat[36]={
    -1., 0., 0., 0., 0., 0.,
     1., 1., 0., 0., 0., 0.,
     1., 0., 0., 0., 1., 0.,
    -1., 0., 0., 1., 0., 0.,
     1., 0., 1., 0., 0., 0.,
     1., 0., 0., 0., 0., 1.
  };

  static double CS6M_Reduce21_RedMat[36]={
     0., 1., 0., 0., 0., 1.,
     0.,-1., 0., 0., 0., 0.,
     0., 1., 1., 0., 0., 0.,
     0., 1., 0., 1., 0., 0.,
     0.,-1., 0., 0., 1., 0.,
     1., 1., 0., 0., 0., 0.
  };

  static double CS6M_Reduce31_RedMat[36]={
     1., 0., 1., 0., 0., 0.,
     0., 0., 1., 1., 0., 0.,
     0., 0.,-1., 0., 1., 0.,
     0., 1., 1., 1., 0., 0.,
     0., 0., 1., 0., 1., 0.,
     0., 0.,-1., 0., 0., 1.
  };

  static double CS6M_Reduce41_RedMat[36]={
     1., 0., 0.,-1., 0., 0.,
     0., 0., 1., 1., 0., 0.,
     0., 1., 0., 1., 0., 0.,
     0., 0., 0.,-1., 0., 0.,
     0., 0., 0., 1., 1., 0.,
     0., 0., 0., 1., 0., 1.
  };

  static double CS6M_Reduce51_RedMat[36]={
     0., 0., 1., 0., 1., 0.,
     0., 1., 0., 0.,-1., 0.,
     1., 0., 0., 0., 1., 0.,
     0., 0., 0., 1., 1., 0.,
     0., 0., 0., 0.,-1., 0.,
     0., 0., 0., 0., 1., 1.
  };

  static double CS6M_Reduce61_RedMat[36]={
     0., 1., 0., 0., 0., 1.,
     1., 0., 0., 0., 0., 1.,
     0., 0., 1., 0., 0.,-1.,
     0., 0., 0., 1., 0., 1.,
     0., 0., 0., 0., 1., 1.,
     0., 0., 0., 0., 0.,-1.
  };

  #define CS6M_Reduce11_Mat(din,dout,mat66) { \
    double mat66out[36]; int ii;   \
    dout[0] = -din[0];             \
    dout[1] = din[0] + din[1];     \
    dout[2] = din[0] + din[4];     \
    dout[3] = -din[0] + din[3];    \
    dout[4] = din[0] + din[2];     \
    dout[5] = din[0] + din[5];     \
    CS6M_Mat66_Mat66_Mult(CS6M_Reduce11_RedMat,mat66,mat66out); \
    for (ii=0; ii < 36; ii++) mat66[ii]=mat66out[ii]; \
  }

  #define CS6M_Reduce21_Mat(din,dout,mat66) { \
    double mat66out[36]; int ii;   \
    dout[0] = din[1] + din[5];     \
    dout[1] = -din[1];             \
    dout[2] = din[1] + din[2];     \
    dout[3] = din[1] + din[3];     \
    dout[4] = -din[1] + din[4];    \
    dout[5] = din[1] + din[0];     \
    CS6M_Mat66_Mat66_Mult(CS6M_Reduce21_RedMat,mat66,mat66out); \
    for (ii=0; ii < 36; ii++) mat66[ii]=mat66out[ii]; \
  }

  #define CS6M_Reduce31_Mat(din,dout,mat66) { \
    double mat66out[36]; int ii;   \
    dout[0] = din[2] + din[0];     \
    dout[1] = din[2] + din[3];     \
    dout[2] = -din[2];             \
    dout[3] = din[2] + din[1];     \
    dout[4] = din[2] + din[4];     \
    dout[5] = -din[2] + din[5];    \
    CS6M_Mat66_Mat66_Mult(CS6M_Reduce31_RedMat,mat66,mat66out); \
    for (ii=0; ii < 36; ii++) mat66[ii]=mat66out[ii]; \
  }

  #define CS6M_Reduce41_Mat(din,dout,mat66) { \
    double mat66out[36]; int ii;   \
    dout[0] = -din[3] + din[0];    \
    dout[1] = din[3] + din[2];     \
    dout[2] = din[3] + din[1];     \
    dout[3] = -din[3];             \
    dout[4] = din[3] + din[4];     \
    dout[5] = din[3] + din[5];     \
    CS6M_Mat66_Mat66_Mult(CS6M_Reduce41_RedMat,mat66,mat66out); \
    for (ii=0; ii < 36; ii++) mat66[ii]=mat66out[ii]; \
  }

  #define CS6M_Reduce51_Mat(din,dout,mat66) { \
    double mat66out[36]; int ii;   \
    dout[0] = din[4] + din[2];     \
    dout[1] = -din[4] + din[1];    \
    dout[2] = din[4] + din[0];     \
    dout[3] = din[4] + din[3];     \
    dout[4] = -din[4];             \
    dout[5] = din[4] + din[5];     \
    CS6M_Mat66_Mat66_Mult(CS6M_Reduce51_RedMat,mat66,mat66out); \
    for (ii=0; ii < 36; ii++) mat66[ii]=mat66out[ii]; \
  }

  #define CS6M_Reduce61_Mat(din,dout,mat66) { \
    double mat66out[36]; int ii;   \
    dout[0] = din[5] + din[1];     \
    dout[1] = din[5] + din[0];     \
    dout[2] = -din[5] + din[2];    \
    dout[3] = din[5] + din[3];     \
    dout[4] = din[5] + din[4];     \
    dout[5] = -din[5];             \
    CS6M_Mat66_Mat66_Mult(CS6M_Reduce61_RedMat,mat66,mat66out); \
    for (ii=0; ii < 36; ii++) mat66[ii]=mat66out[ii]; \
  }


  #define CS6M_Reduce11(din,dout) { \
    dout[0] = -din[0];             \
    dout[1] = din[0] + din[1];     \
    dout[2] = din[0] + din[4];     \
    dout[3] = -din[0] + din[3];    \
    dout[4] = din[0] + din[2];     \
    dout[5] = din[0] + din[5];     \
  }

  #define CS6M_Reduce21(din,dout) { \
    dout[0] = din[1] + din[5];     \
    dout[1] = -din[1];             \
    dout[2] = din[1] + din[2];     \
    dout[3] = din[1] + din[3];     \
    dout[4] = -din[1] + din[4];    \
    dout[5] = din[1] + din[0];     \
  }

  #define CS6M_Reduce31(din,dout) { \
    dout[0] = din[2] + din[0];     \
    dout[1] = din[2] + din[3];     \
    dout[2] = -din[2];             \
    dout[3] = din[2] + din[1];     \
    dout[4] = din[2] + din[4];     \
    dout[5] = -din[2] + din[5];    \
  }

  #define CS6M_Reduce41(din,dout) { \
    dout[0] = -din[3] + din[0];    \
    dout[1] = din[3] + din[2];     \
    dout[2] = din[3] + din[1];     \
    dout[3] = -din[3];             \
    dout[4] = din[3] + din[4];     \
    dout[5] = din[3] + din[5];     \
  }

  #define CS6M_Reduce51(din,dout) { \
    dout[0] = din[4] + din[2];     \
    dout[1] = -din[4] + din[1];    \
    dout[2] = din[4] + din[0];     \
    dout[3] = din[4] + din[3];     \
    dout[4] = -din[4];             \
    dout[5] = din[4] + din[5];     \
  }

  #define CS6M_Reduce61(din,dout) { \
    dout[0] = din[5] + din[1];     \
    dout[1] = din[5] + din[0];     \
    dout[2] = -din[5] + din[2];    \
    dout[3] = din[5] + din[3];     \
    dout[4] = din[5] + din[4];     \
    dout[5] = -din[5];             \
  }


  #define CS6M_S6Reduce_Mat(in,out,mat66,reduced) {      \
    double mat66out[36];                     \
    size_t maxIndex       ;                  \
    int reductionCycleCount;                 \
    int reduction;                           \
    int ii;                                  \
    double maxScalar;                        \
    double delta=0.0;                        \
    double outtemp[6];                       \
    maxIndex=99999;                          \
    reductionCycleCount=0;                   \
    reduction=1;                             \
    for (ii=0; ii<6; ii++) {                 \
      out[ii] = in[ii];                      \
      delta += ((out[ii]>0.)?out[ii]:(-out[ii])); \
    }                                        \
    delta *= 1.e-12;                         \
    while ( (CS6M_CountPositive(out,delta)) != 0) {\
        maxScalar = -1.0E20;                  \
                                              \
        for ( ii = 0; ii < 6; ii++) {         \
           if (out[ii] > maxScalar) {         \
              maxIndex = ii;                  \
              maxScalar = out[ii];            \
           }                                  \
        }                                     \
                                              \
        if (maxIndex < 6) {                   \
          switch (maxIndex) {                       \
            case 0: CS6M_Reduce11(out,outtemp); break;  \
            case 1: CS6M_Reduce21(out,outtemp); break;  \
            case 2: CS6M_Reduce31(out,outtemp); break;  \
            case 3: CS6M_Reduce41(out,outtemp); break;  \
            case 4: CS6M_Reduce51(out,outtemp); break;  \
            case 5: CS6M_Reduce61(out,outtemp); break;  \
          }                                   \
          for (ii=0; ii<6; ii++) out[ii] = outtemp[ii]; \
        }                                               \
        ++reductionCycleCount;                          \
        if (reductionCycleCount > 1000 ) {              \
          reduction=0; break;                           \
        }                                               \
    }                                                   \
    reduced=0;                                          \
    if (CS6M_CountPositive(out,delta) == 0) reduced=1;  \
  }



  #define CS6M_S6Reduce(in,out,reduced) {      \
    size_t maxIndex       ;                  \
    int reductionCycleCount;                 \
    int reduction;                           \
    int ii;                                  \
    double maxScalar;                        \
    double delta=0.0;                        \
    double outtemp[6];                       \
    maxIndex=99999;                          \
    reductionCycleCount=0;                   \
    reduction=1;                             \
    for (ii=0; ii<6; ii++) {                 \
      out[ii] = in[ii];                      \
      delta += ((out[ii]>0.)?out[ii]:(-out[ii])); \
    }                                        \
    delta *= 1.e-12;                         \
    while ( (CS6M_CountPositive(out,delta)) != 0) {\
        maxScalar = -1.0E20;                  \
                                              \
        for ( ii = 0; ii < 6; ii++) {         \
           if (out[ii] > maxScalar) {         \
              maxIndex = ii;                  \
              maxScalar = out[ii];            \
           }                                  \
        }                                     \
                                              \
        if (maxIndex < 6) {                   \
          switch (maxIndex) {                       \
            case 0: CS6M_Reduce11(out,outtemp); break;  \
            case 1: CS6M_Reduce21(out,outtemp); break;  \
            case 2: CS6M_Reduce31(out,outtemp); break;  \
            case 3: CS6M_Reduce41(out,outtemp); break;  \
            case 4: CS6M_Reduce51(out,outtemp); break;  \
            case 5: CS6M_Reduce61(out,outtemp); break;  \
          }                                   \
          for (ii=0; ii<6; ii++) out[ii] = outtemp[ii]; \
        }                                               \
        ++reductionCycleCount;                          \
        if (reductionCycleCount > 1000 ) {              \
          reduction=0; break;                           \
        }                                               \
    }                                                   \
    reduced=0;                                          \
    if (CS6M_CountPositive(out,delta) == 0) reduced=1;  \
  }

  

  #define CS6M_D7Reduce(in,out,reduced) {     \
    double s6in[6];                           \
    double s6out[6];                          \
    double temp;                              \
    double delta;                             \
    int ii;                                   \
    CS6M_D7toS6(in,s6in);                     \
    reduced=1;                                \
    CS6M_S6Reduce(s6in,s6out,reduced);        \
    if (reduced) {                            \
       CS6M_S6toD7(s6out,out);                \
    } else {                                  \
      for (ii=0; ii < 7; ii++) out[ii]=in[ii];\
    }                                         \
    delta=out[3];                             \
    if (out[2] > delta) delta=out[2];         \
    if (out[1] > delta) delta=out[1];         \
    if (out[0] > delta) delta=out[0];         \
    delta = delta * 1.e-12;                   \
    if (delta < 1.e-12) delta=1.e-12;         \
    while (out[0] > out[1]+delta || out[1] > out[2]+delta || out[2] > out[3]+delta ) {\
       if (out[2] > out[3]+delta) {           \
         temp = out[2];                       \
         out[2] = out[3];                     \
         out[3]=temp;                         \
         temp = out[4];                       \
         out[4] = out[5];                     \
         out[5] = temp;                       \
         continue;                            \
       }                                      \
       if (out[1] > out[2]+delta) {           \
         temp = out[1];                       \
         out[1] = out[2];                     \
         out[2]=temp;                         \
         temp = out[5];                       \
         out[5] = out[6];                     \
         out[6] = temp;                       \
         continue;                            \
       }                                      \
       if (out[0] > out[1]+delta) {           \
         temp = out[0];                       \
         out[0] = out[1];                     \
         out[1]=temp;                         \
         temp = out[4];                       \
         out[4] = out[5];                     \
         out[5] = temp;                       \
         continue;                            \
       }                                      \
    }                                         \
  }

  #define CS6M_Cellvolume(cell,volume)       {  \
   double cosAlpha=cos(cell[CS6M_CELLALPHA]);  \
   double cosBeta =cos(cell[CS6M_CELLBETA ]);  \
   double cosGamma=cos(cell[CS6M_CELLGAMMA]);  \
   volume=                                    \
     cell[CS6M_CELLA]*cell[CS6M_CELLB]*cell[CS6M_CELLC] \
       * CS6M_sqrt( fabs(                          \
           1.0                                \
           -cosAlpha*cosAlpha                 \
           -cosBeta*cosBeta                   \
           -cosGamma*cosGamma                 \
           +2.*cosAlpha*cosBeta*cosGamma));   \
  }

  #define CS6M_Cellinverse(cell,inversecell) {        \
    const double cosAlpha=cos(cell[CS6M_CELLALPHA]);  \
    const double cosBeta =cos(cell[CS6M_CELLBETA ]);  \
    const double cosGamma=cos(cell[CS6M_CELLGAMMA]);  \
    const double sinAlpha=sin(cell[CS6M_CELLALPHA]);  \
    const double sinBeta =sin(cell[CS6M_CELLBETA ]);  \
    const double sinGamma=sin(cell[CS6M_CELLGAMMA]);  \
                                                      \
    double v;                                         \
    CS6M_Cellvolume(cell,v);                          \
                                                      \
    const double astar = cell[CS6M_CELLB]*cell[CS6M_CELLC]*sinAlpha/v; \
    const double bstar = cell[CS6M_CELLA]*cell[CS6M_CELLC]*sinBeta /v; \
    const double cstar = cell[CS6M_CELLA]*cell[CS6M_CELLB]*sinGamma/v; \
                                                                    \
    const double cosAlphaStar = (cosBeta *cosGamma-cosAlpha)/fabs(sinBeta*sinGamma); \
    const double cosBetaStar  = (cosAlpha*cosGamma-cosBeta )/fabs(sinAlpha*sinGamma);\
    const double cosGammaStar = (cosAlpha*cosBeta -cosGamma)/fabs(sinAlpha*sinBeta); \
                                                                    \
    inversecell[CS6M_CELLA] = astar;                                          \
    inversecell[CS6M_CELLB] = bstar;                                          \
    inversecell[CS6M_CELLC] = cstar;                                          \
    inversecell[CS6M_CELLALPHA] = atan2( fabs(CS6M_sqrt(1.0-cosAlphaStar*cosAlphaStar)), cosAlphaStar);\
    inversecell[CS6M_CELLBETA]  = atan2( fabs(CS6M_sqrt(1.0-cosBetaStar*cosBetaStar)),   cosBetaStar); \
    inversecell[CS6M_CELLGAMMA] = atan2( fabs(CS6M_sqrt(1.0-cosGammaStar*cosGammaStar)), cosGammaStar);\
  }


  #define CS6M_G6toV7(g6vec,v7vec) {                                  \
    double cell[6], inversecell[6];                                   \
    double g6inversecell[6], g6redinversecell[6], redinversecell[6];  \
    double volume;                                                    \
    int reduced;                                                      \
    int ii;                                                           \
    for(ii=0;ii<7;ii++) v7vec[ii]=0;                                  \
    CS6M_G6toCell(g6vec,cell);                                        \
    CS6M_Cellinverse(cell,inversecell);                               \
    CS6M_CelltoG6(inversecell,g6inversecell);                         \
    CS6M_G6Reduce(g6inversecell,g6redinversecell,reduced);            \
    if (reduced) {                                                    \
      CS6M_G6toCell(g6redinversecell,redinversecell);                 \
      CS6M_Cellvolume(cell,volume);                                   \
      v7vec[CS6M_V7A] = cell[CS6M_CELLA];                             \
      v7vec[CS6M_V7B] = cell[CS6M_CELLB];                             \
      v7vec[CS6M_V7C] = cell[CS6M_CELLC];                             \
      v7vec[CS6M_V7ASTARINV] = 1./redinversecell[CS6M_CELLA];         \
      v7vec[CS6M_V7BSTARINV] = 1./redinversecell[CS6M_CELLB];         \
      v7vec[CS6M_V7CSTARINV] = 1./redinversecell[CS6M_CELLC];         \
      v7vec[CS6M_V7VOLCROOT] = pow(volume,1./3.);                     \
    }                                                                 \
  }

  #define CS6M_celltovecs(cell,A,B,C) { \
    const double cosAlpha=cos(cell[CS6M_CELLALPHA]);  \
    const double cosBeta =cos(cell[CS6M_CELLBETA ]);  \
    const double cosGamma=cos(cell[CS6M_CELLGAMMA]);  \
    const double sinAlpha=sin(cell[CS6M_CELLALPHA]);  \
    const double sinBeta =sin(cell[CS6M_CELLBETA ]);  \
    const double sinGamma=sin(cell[CS6M_CELLGAMMA]);  \
    double volume;                                    \
    CS6M_Cellvolume(cell,volume);                     \
    A[0] = cell[CS6M_CELLA];                          \
    A[1] = 0.;                                        \
    A[2] = 0.;                                        \
    B[0] = cell[CS6M_CELLB]*cosGamma;                 \
    B[1] = cell[CS6M_CELLB]*sinGamma;                 \
    B[2] = 0.;                                        \
    C[0] = cell[CS6M_CELLC]*cosBeta;                  \
    C[1] = cell[CS6M_CELLC]*(cosAlpha - cosBeta*cosGamma) / sinGamma; \
    C[2] = volume/(cell[CS6M_CELLA]*cell[CS6M_CELLB]*sinGamma);       \
  } 

  #define CS6M_celltoE3(cell,mat33) { \
    const double cosAlpha=cos(cell[CS6M_CELLALPHA]);  \
    const double cosBeta =cos(cell[CS6M_CELLBETA ]);  \
    const double cosGamma=cos(cell[CS6M_CELLGAMMA]);  \
    const double sinAlpha=sin(cell[CS6M_CELLALPHA]);  \
    const double sinBeta =sin(cell[CS6M_CELLBETA ]);  \
    const double sinGamma=sin(cell[CS6M_CELLGAMMA]);  \
    double volume;                                    \
    CS6M_Cellvolume(cell,volume);                     \
    mat33[0] = cell[CS6M_CELLA];                      \
    mat33[3] = 0.;                                    \
    mat33[6] = 0.;                                    \
    mat33[1] = cell[CS6M_CELLB]*cosGamma;             \
    mat33[4] = cell[CS6M_CELLB]*sinGamma;             \
    mat33[7] = 0.;                                    \
    mat33[2] = cell[CS6M_CELLC]*cosBeta;              \
    mat33[5] = cell[CS6M_CELLC]*(cosAlpha - cosBeta*cosGamma) / sinGamma; \
    mat33[8] = volume/(cell[CS6M_CELLA]*cell[CS6M_CELLB]*sinGamma);       \
  } 


  #define CS6M_Mat33_Scalar_Mult(scalar,mat33in,mat33out) { \
    int ii;                                               \
    for (ii=0; ii<9; ii++) mat33out=scalar*mat33in;       \
  }

  #define CS6M_Mat44_Scalar_Mult(scalar,mat44in,mat44out) { \
    int ii;                                               \
    for (ii=0; ii<16; ii++) mat44out=scalar*mat44in;       \
  }

  #define CS6M_Mat66_Scalar_Mult(scalar,mat66in,mat66out) { \
    int ii;                                               \
    for (ii=0; ii<36; ii++) mat66out=scalar*mat66in;      \
  }


  #ifdef CS6M_DEBUG_PRINT_COUT
    #define CS6M_PRINT_VECTOR3(name,vec) { \
      std::cerr<<#name<<": [" << vec[0] << ", " << vec[1] << ", " << vec[2] << " ]" << std::endl; \
    }
    #define CS6M_PRINT_MATRIX33(name,mat) { \
      std::cerr<<#name<<":" << std::endl \
      << "  [" << mat[0] << ", " << mat[1] << ", " << mat[2] << " ]" << std::endl \
      << "  [" << mat[3] << ", " << mat[4] << ", " << mat[5] << " ]" << std::endl \
      << "  [" << mat[6] << ", " << mat[7] << ", " << mat[8] << " ]" << std::endl; \
    }
    #define CS6M_PRINT_VECTOR4(name,vec) { \
      std::cerr<<#name<<": [" << vec[0] << ", " << vec[1] <<  ", " << vec[2] << ", " << vec[3] << " ]" << std::endl; \
    }
    #define CS6M_PRINT_MATRIX44(name,mat) { \
      std::cerr<<#name<<":" << std::endl \
      << "  [" << mat[0]  << ", " << mat[1]  << ", " << mat[2]  << ", " << mat[3]  << " ]" << std::endl \
      << "  [" << mat[4]  << ", " << mat[5]  << ", " << mat[6]  << ", " << mat[7]  << " ]" << std::endl \
      << "  [" << mat[8]  << ", " << mat[9]  << ", " << mat[10] << ", " << mat[11] << " ]" << std::endl \
      << "  [" << mat[12] << ", " << mat[13] << ", " << mat[14] << ", " << mat[15] << " ]" << std::endl; \
    }
    #define CS6M_PRINT_VECTOR6(name,vec) { \
      std::cerr<<#name<<": [" << vec[0] << ", " << vec[1] << ", " << vec[2] \
      << ", "<< vec[3] << ", "<< vec[4] << ", " << vec[5] <<  " ]" << std::endl; \
    }
    #define CS6M_PRINT_MATRIX66(name,mat) { \
      std::cerr<<#name<<":" << std::endl \
      << "  [" << mat[0]  << ", " << mat[1]  << ", " << mat[2]  << ", "<< mat[3]  << ", "<< mat[4]  << ", "<< mat[5]  <<  " ]" << std::endl \
      << "  [" << mat[6]  << ", " << mat[7]  << ", " << mat[8]  << ", "<< mat[9]  << ", "<< mat[10] << ", "<< mat[11] <<  " ]" << std::endl \
      << "  [" << mat[12] << ", " << mat[13] << ", " << mat[14] << ", "<< mat[15] << ", "<< mat[16] << ", "<< mat[17] <<  " ]" << std::endl \
      << "  [" << mat[18] << ", " << mat[19] << ", " << mat[15] << ", "<< mat[21] << ", "<< mat[22] << ", "<< mat[23] <<  " ]" << std::endl \
      << "  [" << mat[24] << ", " << mat[25] << ", " << mat[26] << ", "<< mat[27] << ", "<< mat[28] << ", "<< mat[29] <<  " ]" << std::endl \
      << "  [" << mat[30] << ", " << mat[31] << ", " << mat[32] << ", "<< mat[33] << ", "<< mat[34] << ", "<< mat[35] <<  " ]" << std::endl; \
    }
  #else
    #ifdef CS6M_DEBUG_PRINT_FPRINTF
      #define CS6M_PRINT_VECTOR3(name,vec) { \
        fprintf(stderr," %s: [%g, %g, %g ]\n",#name,vec[0],vec[1],vec[2]);
      }
      #define CS6M_PRINT_MATRIX33(name,mat) { \
        fprintf(stderr," %s: \n [%g, %g, %g ]" \
                            "\n [%g, %g, %g ]" \
                            "\n [%g, %g, %g ]" \
                            "\n ", name, \
                          mat[0],mat[1],mat[2], \
                          mat[3],mat[4],mat[5], \
                          mat[6],mat[7],mat[8]); \
      }
      #define CS6M_PRINT_VECTOR4(name,vec) { \
        fprintf(stderr," %s: [%g, %g, %g, %g ]\n",#name,vec[0],vec[1],vec[2],vec[3]);
      }
      #define CS6M_PRINT_MATRIX44(name,mat) { \
        fprintf(stderr," %s: \n [%g, %g, %g, %g ]" \
                            "\n [%g, %g, %g, %g ]" \
                            "\n [%g, %g, %g, %g ]" \
                            "\n [%g, %g, %g, %g ]" \
                          "\n ", name, \
                          mat[0] ,mat[1] ,mat[2] ,mat[3] , \
                          mat[4] ,mat[5] ,mat[6] ,mat[7] , \
                          mat[8] ,mat[9] ,mat[10],mat[11], \
                          mat[12],mat[13],mat[14],mat[15]); \
      }
      #define CS6M_PRINT_VECTOR6(name,vec) { \
        fprintf(stderr," %s: [%g, %g, %g, %g, %g, %g ]\n",#name,vec[0],vec[1],vec[2],vec[3],vec[4],vec[5]);
      }
      #define CS6M_PRINT_MATRIX66(name,mat) { \
        fprintf(stderr," %s: \n [%g, %g, %g, %g, %g, %g ]" \
                          "\n [%g, %g, %g, %g, %g, %g ]" \
                          "\n [%g, %g, %g, %g, %g, %g ]" \
                          "\n [%g, %g, %g, %g, %g, %g ]" \
                          "\n [%g, %g, %g, %g, %g, %g ]" \
                          "\n [%g, %g, %g, %g, %g, %g ]", #name, \
                          mat[0] ,mat[1] ,mat[2] ,mat[3] ,mat[4] ,mat[5] , \
                          mat[6] ,mat[7] ,mat[8] ,mat[9] ,mat[10],mat[11], \
                          mat[12],mat[13],mat[14],mat[15],mat[16],mat[17], \
                          mat[18],mat[19],mat[20],mat[21],mat[22],mat[23], \
                          mat[24],mat[25],mat[26],mat[27],mat[28],mat[29], \
                          mat[30],mat[31],mat[32],mat[33],mat[34],mat[35]); \
      }
    #else
      #define CS6M_PRINT_VECTOR3(name,vec)  
      #define CS6M_PRINT_MATRIX33(name,mat)  
      #define CS6M_PRINT_VECTOR4(name,vec)  
      #define CS6M_PRINT_MATRIX44(name,mat)  
      #define CS6M_PRINT_VECTOR6(name,vec)  
      #define CS6M_PRINT_MATRIX66(name,mat)  
    #endif
  #endif


  #define CS6M_Mat33_Vector_Mult(mat33in,vectorin,vectorout) { \
    int ii, jj, kk;                                          \
    CS6M_PRINT_MATRIX33(mat33in,mat33in)                     \
    CS6M_PRINT_VECTOR3(vectorin,vectorin)                    \
    for (ii=0; ii<3; ii++) {                                 \
      vectorout[ii]=0.;                                      \
      for (jj=0; jj<3; jj++) {                               \
         kk=ii*3+jj;                                         \
         vectorout[ii]+=mat33in[kk]*vectorin[jj];            \
      }                                                      \
    }                                                        \
    CS6M_PRINT_VECTOR3(vectorout,vectorout)                  \
  }

  #define CS6M_Mat33_Mat33_Mult(mat33inleft,mat33inright,mat33out) { \
    int ii, jj, kk, ll;                                            \
    CS6M_PRINT_MATRIX33(mat33inleft,mat33inleft)                   \
    CS6M_PRINT_MATRIX33(mat33inright,mat33inright)                 \
    for (ii=0; ii<3; ii++){      \
      for (jj=0; jj<3; jj++) {   \
        ll=ii*3+jj;              \
        mat33out[ll]=0;          \
        for (kk=0;kk<3;kk++) {   \
          mat33out[ll]+=mat33inleft[ii*3+kk]*mat33inright[kk*3+jj]; \
        }                        \
      }                          \
    }                            \
    CS6M_PRINT_MATRIX33(mat33out,mat33out)                          \
  }

  #define CS6M_Mat44_Vector_Mult(mat44in,vectorin,vectorout) { \
    int ii, jj, kk;                                          \
    CS6M_PRINT_MATRIX44(mat44in,mat44in)                     \
    CS6M_PRINT_VECTOR4(vectorin,vectorin)                    \
    for (ii=0; ii<4; ii++) {                                 \
      vectorout[ii]=0.;                                      \
      for (jj=0; jj<4; jj++) {                               \
         kk=ii*4+jj;                                         \
         vectorout[ii]+=mat44in[kk]*vectorin[jj];            \
      }                                                      \
    }                                                        \
    CS6M_PRINT_VECTOR4(vectorout,vectorout)                  \
  }

  #define CS6M_Mat44_Mat44_Mult(mat44inleft,mat44inright,mat44out) { \
    int ii, jj, kk, ll;                                            \
    CS6M_PRINT_MATRIX44(mat44inleft,mat44inleft)                   \
    CS6M_PRINT_MATRIX44(mat44inright,mat44inright)                 \
    for (ii=0; ii<4; ii++){      \
      for (jj=0; jj<4; jj++) {   \
        ll=ii*4+jj;              \
        mat44out[ll]=0;          \
        for (kk=0;kk<4;kk++) {   \
          mat44out[ll]+=mat44inleft[ii*4+kk]*mat44inright[kk*4+jj]; \
        }                        \
      }                          \
    }                            \
    CS6M_PRINT_MATRIX44(mat44out,mat44out)                          \
  }

  #define CS6M_Mat66_Vector_Mult(mat66in,vectorin,vectorout) { \
    int ii, jj, kk;                                          \
    CS6M_PRINT_MATRIX66(mat66in,mat66in)                     \
    CS6M_PRINT_VECTOR6(vectorin,vectorin)                    \
    for (ii=0; ii<6; ii++) {                                 \
      vectorout[ii]=0.;                                      \
      for (jj=0; jj<6; jj++) {                               \
         kk=ii*6+jj;                                         \
         vectorout[ii]+=mat66in[kk]*vectorin[jj];            \
      }                                                      \
    }                                                        \
    CS6M_PRINT_VECTOR6(vectorout,vectorout)                  \
  }

  #define CS6M_Mat66_Mat66_Mult(mat66inleft,mat66inright,mat66out) { \
    int ii, jj, kk, ll;                                            \
    CS6M_PRINT_MATRIX66(mat66inleft,mat66inleft)                   \
    CS6M_PRINT_MATRIX66(mat66inright,mat66inright)                 \
    for (ii=0; ii<6; ii++){      \
      for (jj=0; jj<6; jj++) {   \
        ll=ii*6+jj;              \
        mat66out[ll]=0;          \
        for (kk=0;kk<6;kk++) {   \
          mat66out[ll]+=mat66inleft[ii*6+kk]*mat66inright[kk*6+jj]; \
        }                        \
      }                          \
    }                            \
    CS6M_PRINT_MATRIX66(mat66out,mat66out)                          \
  }


   static double CS6M_MAT_P[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      0., 0., 1., 0., 0., 0.,
      0., 0., 0., 1., 0., 0.,
      0., 0., 0., 0., 1., 0.,
      0., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_I[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
     .25,.25,.25,.25,.25,.25,
      0., 1., 0., .5, 0., .5,
      1., 0., 0., 0., .5, .5,
      0., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_A[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
      0.,.25,.25,.25, 0., 0.,
      0. ,1., 0., .5, 0., 0.,
      0., 0., 0., 0., .5, .5,
      0., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_B[36]={
      1., 0., 0., 0., 0., 0.,
      0., 1., 0., 0., 0., 0.,
     .25, 0.,.25, 0.,.25, 0.,
      0., 0., 0., .5, 0., .5,
      1., 0., 0., 0., .5, 0.,
      0., 0., 0., 0., 0., 1.
   };
   static double CS6M_MAT_C[36]={
      1., 0., 0., 0., 0., 0.,
     .25,.25, 0., 0., 0.,.25,
      0., 0., 1., 0., 0., 0.,
      0., 0., 0., .5, .5, 0.,
      0., 0., 0., 0., 1., 0.,
      1., 0., 0., 0. ,0.,.5
   };
   static double CS6M_MAT_F[36]={
     .25,.25, 0., 0., 0.,.25,
     .25, 0.,.25, 0.,.25, 0.,
      0.,.25,.25,.25, 0., 0.,
      0., 0., .5,.25,.25,.25,
      0., .5, 0.,.25,.25,.25,
      .5, 0., 0.,.25,.25,.25
   };
   static double CS6M_MAT_H[36]={
      1., 1., 1., 1.,-1.,-1.,
      4., 1., 1., 1., 2., 2.,
      1., 4., 1.,-2.,-1., 2.,
     -4.,-4., 2.,-1., 1.,-5.,
      2.,-4., 2.,-1.,-2., 1.,
     -4., 2., 2., 2., 1., 1
   };
   static double CS6M_MAT_R[36]={
      1.,0.,0.,0.,0.,0.,
      0.,1.,0.,0.,0.,0.,
      0.,0.,1.,0.,0.,0.,
      0.,0.,0.,1.,0.,0.,
      0.,0.,0.,0.,1.,0.,
      0.,0.,0.,0.,0.,1.
   };
   static double CS6M_HEXPERP[36]={
      .6666666666667,-.3333333333333, 0., 0., 0.,.3333333333333,
     -.3333333333333, .6666666666667, 0., 0., 0.,.3333333333333,
      0., 0.,0., 0., 0., 0.,
      0., 0., 0.,-1., 0., 0.,
      0., 0., 0., 0.,-1., 0.,
      .3333333333333, .333333333333, 0., 0., 0., .6666666666667
   };
   static double CS6M_RHMPERP[36]={
      .6666666666667,-.3333333333333,-.3333333333333, 0., 0., 0.,
     -.3333333333333, .6666666666667,-.3333333333333, 0., 0., 0.,
     -.3333333333333,-.3333333333333, .6666666666667, 0., 0., 0.,
      0., 0., 0., .6666666666667,-.3333333333333,-.3333333333333,
      0., 0., 0.,-.3333333333333, .6666666666667,-.3333333333333,
      0., 0., 0.,-.3333333333333,-.3333333333333, .6666666666667 
   };


  #define CS6M_multi_redcells(g6vecin,s6redout,d7redout,g6redout,v7redout) { \
    int reduced;                                                             \
    int ii;                                                                  \
    double s6vecin(6);                                                       \
    double d7vecin(7);                                                       \
    double v7vecin(7);                                                       \
    CS6M_G6toS6(g6vecin,s6vecin);                                            \
    reduced=0;                                                               \
    CS6M_S6Reduce(s6vecin,s6redout,reduced);                                 \
    if (!reduced) for (ii=0;ii<6;ii++) s6redout(ii)=0.;                      \
    CS6M_G6toD7(g6vecin,d7vecin);                                            \
    reduced=0;                                                               \
    CS6M_D7Reduce(d7vecin,d7redout,reduced);                                 \
    if (!reduced) for (ii=0;ii<7;ii++) d7redout(ii)=0.;                      \
    CS6M_G6toV7(g6vecin,v7vecin);                                            \
    reduced=0;                                                               \
    CS6M_V7Reduce(v7vecin,v7redout,reduced);                                 \
    if (!reduced) for (ii=0;ii<7;ii++) v7redout(ii)=0.;                      \
   };


  #define CS6M_LatSymMat66(g6vec,latsym,mat66,g6vecout){\
    int ii;                                           \
    double temp66[36];                                \
    double hexperp[6];                                \
    double rhmperp[6];                                \
    double hexperpnormsq,rhmperpnormsq;               \
    switch (latsym) {                                 \
      case 'P':                                       \
      case 'p':                                       \
        for (ii=0; ii<36; ii++) mat66[ii]= CS6M_MAT_P[ii]; \
        break;                                        \
      case 'I':                                       \
      case 'i':                                       \
        for (ii=0; ii<36; ii++) mat66[ii]= CS6M_MAT_I[ii]; \
        break;                                        \
      case 'A':                                       \
      case 'a':                                       \
        for (ii=0; ii<36; ii++) mat66[ii]= CS6M_MAT_A[ii]; \
        break;                                        \
      case 'B':                                       \
      case 'b':                                       \
        for (ii=0; ii<36; ii++) mat66[ii]= CS6M_MAT_B[ii]; \
        break;                                        \
      case 'C':                                       \
      case 'c':                                       \
        for (ii=0; ii<36; ii++) mat66[ii]= CS6M_MAT_C[ii]; \
        break;                                        \
      case 'F':                                       \
      case 'f':                                       \
         for (ii=0; ii<36; ii++) mat66[ii]= CS6M_MAT_F[ii];\
        break;                                        \
      case 'H':                                       \
      case 'h':                                       \
      case 'R':                                       \
      case 'r':                                       \
        CS6M_Mat66_Vector_Mult(CS6M_HEXPERP,g6vec,hexperp);  \
        CS6M_Mat66_Vector_Mult(CS6M_RHMPERP,g6vec,rhmperp);  \
        CS6M_Vec6_Dot_Prod(hexperp,hexperp,hexperpnormsq); \
        CS6M_Vec6_Dot_Prod(rhmperp,rhmperp,rhmperpnormsq); \
        if (hexperpnormsq < rhmperpnormsq) {          \
          for (ii=0; ii<36; ii++) mat66[ii]= CS6M_MAT_H[ii]/9.; \
          break;                                      \
        }                                             \
      default:                                        \
        for (ii=0; ii<36; ii++) mat66[ii]= CS6M_MAT_P[ii];  \
        break;                                        \
    }                                                 \
    CS6M_Mat66_Vector_Mult(mat66,g6vec,g6vecout);     \
    /* std::cout<< "mat66: " << mat66  << std::endl;*/\
    /* std::cout<< "g6vec: " << g6vec  << std::endl;*/\
    /* std::cout<< "g6vecout: " << g6vecout << std::endl; */\
  }


  #define CS6M_EigenMV_Mat44(mat44,eigenvec,eigenval) {       \
    double mat44sq[16], mat44tmp[16];                         \
    double normsign;                                          \
    double vector1[4], vector2[4], vector3[4], vector4[4];    \
    double norm,normsq,normsq1, normsq2, normsq3, normsq4;    \
    int ii, jj, keig;                                         \
    double trace;                                             \
    CS6M_Mat44_Mat44_Mult(mat44,mat44,mat44tmp);              \
    for (ii=0; ii < 18; ii++){                                \
      trace=mat44tmp[0]+mat44tmp[5]+mat44tmp[10]+mat44tmp[15];\
      if (fabs(trace) < 1.e-20) trace= 1.0;                   \
      for (jj=0; jj < 16; jj++){mat44sq[jj]=mat44tmp[jj]/trace;}\
      CS6M_Mat44_Mat44_Mult(mat44sq,mat44sq,mat44tmp);        \
    }                                                         \
    vector1[0]=mat44tmp[0] ;vector1[1]=mat44tmp[1] ;vector1[2]=mat44tmp[2] ;vector1[3]=mat44tmp[3]; \
    normsq1=vector1[0]*vector1[0]+vector1[1]*vector1[1]+vector1[2]*vector1[2]+vector1[3]*vector1[3];\
    keig=1; normsq=normsq1;                                         \
    vector2[0]=mat44tmp[4] ;vector2[1]=mat44tmp[5] ;vector2[2]=mat44tmp[6] ;vector2[3]=mat44tmp[7]; \
    normsq2=vector2[0]*vector2[0]+vector2[1]*vector2[1]+vector2[2]*vector2[2]+vector2[3]*vector2[3];\
    if(normsq2 > normsq) {keig=2; normsq=normsq2;}                  \
    vector3[0]=mat44tmp[8] ;vector3[1]=mat44tmp[9] ;vector3[2]=mat44tmp[10];vector3[3]=mat44tmp[11];\
    normsq3=vector3[0]*vector3[0]+vector3[1]*vector3[1]+vector3[2]*vector3[2]+vector3[3]*vector3[3];\
    if(normsq3 > normsq) {keig=3; normsq=normsq3;}                  \
    vector4[0]=mat44tmp[12];vector4[1]=mat44tmp[13];vector4[2]=mat44tmp[14];vector4[3]=mat44tmp[15];\
    normsq4=vector4[0]*vector4[0]+vector4[1]*vector4[1]+vector4[2]*vector4[2]+vector4[3]*vector4[3];\
    if(normsq4 > normsq) {keig=4; normsq=normsq4;}                  \
    norm =CS6M_sqrt(fabs(normsq));                                  \
    if (keig==1) {                                                                                  \
      for(ii=0;ii < 4;ii++) eigenvec[ii]= vector1[ii]/norm;                                         \
    } else if (keig==2) {                                                                           \
      for(ii=0;ii < 4;ii++) eigenvec[ii]= vector2[ii]/norm;                                         \
    } else if (keig==3) {                                                                           \
      for(ii=0;ii < 4;ii++) eigenvec[ii]= vector3[ii]/norm;                                         \
    } else {                                                                                        \
      for(ii=0;ii < 4;ii++) eigenvec[ii]= vector4[ii]/norm;                                         \
    }                                                                                               \
    CS6M_Mat44_Vector_Mult(mat44,eigenvec,vector1);                                                 \
    normsq=vector1[0]*vector1[0]+vector1[1]*vector1[1]+vector1[2]*vector1[2]+vector1[3]*vector1[3]; \
    norm=CS6M_sqrt(fabs(normsq));                                                                   \
    CS6M_Vec4_Dot_Prod(eigenvec,vector1,eigenval);                                                  \
  }

  #define CS6M_QuaterniontoRotmat(quaternion,mat33) {     \
                                                          \
     double twoxy, twoyz, twoxz, twowx, twowy, twowz;     \
     double ww, xx, yy, zz;                               \
                                                          \
     ww = (quaternion[CS6M_QR_W])*(quaternion[CS6M_QR_W]);\
     xx = (quaternion[CS6M_QR_X])*(quaternion[CS6M_QR_X]);\
     yy = (quaternion[CS6M_QR_Y])*(quaternion[CS6M_QR_Y]);\
     zz = (quaternion[CS6M_QR_Z])*(quaternion[CS6M_QR_Z]);\
                                                          \
     mat33[0] = ww + xx - yy - zz;                        \
     mat33[4] = ww - xx + yy - zz;                        \
     mat33[8] = ww - xx - yy + zz;                        \
                                                          \
     twoxy = 2.*(quaternion[CS6M_QR_X])*(quaternion[CS6M_QR_Y]);\
     twoyz = 2.*(quaternion[CS6M_QR_Y])*(quaternion[CS6M_QR_Z]);\
     twoxz = 2.*(quaternion[CS6M_QR_X])*(quaternion[CS6M_QR_Z]);\
     twowx = 2.*(quaternion[CS6M_QR_W])*(quaternion[CS6M_QR_X]);\
     twowy = 2.*(quaternion[CS6M_QR_W])*(quaternion[CS6M_QR_Y]);\
     twowz = 2.*(quaternion[CS6M_QR_W])*(quaternion[CS6M_QR_Z]);\
                                                          \
     mat33[3] = twoxy - twowz;                            \
     mat33[6] = twoxz + twowy;                            \
     mat33[1] = twoxy + twowz;                            \
     mat33[7] = twoyz - twowx;                            \
     mat33[2] = twoxz - twowy;                            \
     mat33[5] = twoyz + twowx;                            \
 }

  #define CS6M_rmsd(e3cell1,e3cell2,rmsd)  {                 \
    double A1[3],B1[3],C1[3],D1[3],A2[3],B2[3],C2[3],D2[3];  \
    double Adiff[3], Bdiff[3], Cdiff[3], Ddiff[3];           \
    double Anormsq, Bnormsq, Cnormsq, Dnormsq;               \
    int ii;                                                  \
    CS6M_E3bycoltovecs(e3cell1,A1,B1,C1);                    \
    CS6M_E3bycoltovecs(e3cell2,A2,B2,C2);                    \
    Anormsq=Bnormsq=Cnormsq=Dnormsq=0.;                      \
    for(ii=0; ii<3; ii++){                                   \
      D1[ii]=-A1[ii]-B1[ii]-C1[ii];                          \
      D2[ii]=-A2[ii]-B2[ii]-C2[ii];                          \
      Adiff[ii]=A1[ii]-A2[ii];                               \
      Bdiff[ii]=B1[ii]-B2[ii];                               \
      Cdiff[ii]=C1[ii]-C2[ii];                               \
      Ddiff[ii]=D1[ii]-D2[ii];                               \
      Anormsq+=Adiff[ii]*Adiff[ii];                          \
      Bnormsq+=Bdiff[ii]*Bdiff[ii];                          \
      Cnormsq+=Cdiff[ii]*Cdiff[ii];                          \
      Dnormsq+=Ddiff[ii]*Ddiff[ii];                          \
    }                                                        \
    rmsd=CS6M_sqrt(Anormsq+Bnormsq+Cnormsq+Dnormsq)/2.;      \
  }


#endif
