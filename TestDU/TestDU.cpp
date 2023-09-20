
#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

#include "DC7u.h"
#include "GenerateRandomLattice.h"
#include "MatDC7u.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ToString.h"
#include "LRL_ReadLatticeData.h"
#include "NCDist.h"
#include "StoreResults.h"
#include "rhrand.h"
#include "S6.h"

template<typename T>
double sqr(const T a) { return a * a; }

RHrand ran;

template<typename T>
T CreateTotallyRandom_Unit_OrthogonalComponent(const T& t) {
   // assume t is not null and rand is not parallel to t
   const T base = t / t.norm();
   T temp;
   for (size_t i = 0; i < t.GetDim(); ++i) temp[i] = ran.urand() - 0.5;
   temp /= temp.norm();
   const double d = dot(VecN(base.GetVector()), VecN(T(temp).GetVector()));
   T ortho = temp - d * base;
   ortho.SetValid(true);
   return ortho / ortho.norm();
}

//std::vector<MatDC7u> MakeBoundaryMatrices() {
//   std::vector<MatDC7u> vmat;
//
//   //M1_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to --- or +++ to +++ at g6[g6_A2]=g6[G6_B2]
//   // matrix 0,1,0,0,0,0,  1,0,0,0,0,0,  0,0,1,0,0,0,  0,0,0,0,1,0,  0,0,0,1,0,0,  0,0,0,0,0,1)
//   //M1_g6.[r,r,t,u,v,w]:
//   // matrix [r],[r],[t],[v],[u],[w])
//   vmat.emplace_back(MatDC7u(std::vector<double>({
//      0, 1, 0, 0, 0, 0, 0, 
//      1, 0, 0, 0, 0, 0, 0, 
//      0, 0, 1, 0, 0, 0, 0, 
//      0, 0, 0, 0, 1, 0, 0, 
//      0, 0, 0, 1, 0, 0, 0, 
//      0, 0, 0, 0, 0, 1, 0, 
//      -1, -1, -1, 1, 1, 1, 0 })));
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, -1, 1, 1, -1, 1, 0})));
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, -1, 1, 1, -1, 1, 1, 0})));
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, -1, 1, 1, -1, 0})));
//   ////M2_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to --- or +++ to +++ at g6[g6_B2]=g6[g6_C2]
//   //// matrix 1,0,0,0,0,0,  0,0,1,0,0,0,  0,1,0,0,0,0,  0,0,0,1,0,0,  0,0,0,0,0,1,  0,0,0,0,1,0)
//   ////M2_g6.[r,s,s,u,v,w]:
//      //vmat.emplace_back(MatDC7u(std::vector<double>({ ([r],[s],[s],[u],[w],[v])
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, -1, -1, -1, 1, 1, 1, 0})));
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, -1, 1, 1, -1, 1, 1, 0})));
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, -1, 1, 1, -1, 0})));
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, -1, 1, 1, -1, 1, 0})));
//   ////M3_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to +++ for g6[g6_2BC]=0
//   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  0,0,1,0,0,0,  0,0,1.0E-15,1,0,0,  0,0,0,0,-1,0,  0,0,0,0,0,-1)
//   ////M3_g6.[r,s,t,0,v,w]:
//   //// matrix [r],[s],[t],[1.0E-15*t],[-v],[-w])
//   ////M3_dc7upmm
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 1.999999999999999, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, -1.0, -0.999999999999999, 1.0, 1.0, 1.0, 0.0})));
//   ////M4_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to +++ for g6[g6_2AC]=0
//   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  0,0,1,0,0,0,  0,0,0,-1,0,0,  0,0,1.0E-15,0,1,0,  0,0,0,0,0,-1)
//   ////M4_g6.[r,s,t,u,0,w]:
//   //// matrix [r],[s],[t],[-u],[1.0E-15*t],[-w])
//   ////M4_dc7umpm
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 2.0, 0.0, 1.999999999999999, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, -1.0, -0.999999999999999, 1.0, 1.0, 1.0, 0.0})));
//   ////M5_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to +++ for g6[g6_2AB]=0
//   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  0,0,1,0,0,0,  0,0,0,-1,0,0,  0,0,0,0,-1,0,  0,0,1.0E-15,0,0,1)
//   ////M5_g6.[r,s,t,u,v,0]:
//   //// matrix [r],[s],[t],[-u],[-v],[1.0E-15*t])
//   ////M5_dc7ummp
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 2.0, 2.0, -1.0E-15, 0.0, 0.0, -1.0, 0.0, -1.0, -1.0, -0.999999999999999, 1.0, 1.0, 1.0, 0.0})));
//   ////M6_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from +++ to --- for g6[g6_B2]=g6[g6_2BC], g6[g6_2AC]>=g6[g6_2AB]
//   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  0,1,1,-1,0,0,  0,-2,0,1,0,0,  0,0,0,0,-1,1,  0,0,0,0,0,-1)
//   ////M6_g6.[r,s,t,s,v,w]:
//   //// matrix [r],[s],[t],[-s],[w-v],[-w])
//   ////M6_dc7ummm
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, -1, 1, 1, -1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0})));
//   ////M7_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from +++ to --- for g6[g6_B2]=g6[g6_2BC], g6[g6_2AC]<g6[g6_2AB]
//   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  0,1,1,-1,0,0,  0,2,0,-1,0,0,  0,0,0,0,-1,1,  0,0,0,0,0,1)
//   //// matrix [r],[s],[t],[s],[w-v],[w])
//   ////M7_dc7umpm
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, -1, 1, 1, -1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0})));
//   ////M8_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to +++ for  g6[g6_B2]=-g6[g6_2BC]
//   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  0,1,1,1,0,0,  0,2,0,1,0,0,  0,0,0,0,-1,-1,  0,0,0,0,0,-1)
//   ////M8_g6.[r,s,t,-s,v,w]:
//   //// matrix [r],[s],[t],[s],[(-w)-v],[-w])
//   ////M8_dc7ummp:
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, -1, -1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 2, 0, -1, 0, 0})));
//   ////M9_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from +++ to --- for  g6[g6_A2]=g6[g6_2AC], g6[g6_2BC] >= g6[g6_AB]
//   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  1,0,1,0,1,0,  0,0,0,-1,0,1,  -2,0,0,0,1,0,  0,0,0,0,0,-1)
//   ////M9_g6.[r,s,t,u,r,w]:
//   //// matrix [r],[s],[t+2*r],[w-u],[-r],[-w])
//   ////M9_dc7ummp:
//   ////mat_g6ppp2dc7ummo .    vmat.emplace_back(MatDC7u(std::vector<double>({ [1,0,0,0,0,0,0],[0,1,0,0,0,0,0],[0,0,0,0,1,0,0],[-1,0,1,-1,0,1,0],[-3,0,-1,0,1,0,0],[1,1,0,0,0,-1,0])
//   ////MA_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from +++ to +++ for  g6[g6_A2]=g6[g6_2AC], g6[g6_2BC] < g6[g6_AB] 
//   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  1,0,1,0,-1,0,  0,0,0,-1,0,1,  2,0,0,0,-1,0,  0,0,0,0,0,1)
//   ////MA_g6.[r,s,t,u,r,w]:
//   //// matrix [r],[s],[t],[w-u],[r],[w])
//   ////MA_dc7upmm:
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -1, 1, 1, -1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0})));
//   ////MB_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to +++ for  g6[g6_A2]=-g6[g6_2AC]
//   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  1,0,1,0,1,0,  0,0,0,-1,0,-1,  2,0,0,0,1,0,  0,0,0,0,0,-1)
//   ////MB_g6.[r,s,t,u,-r,w]:
//   //// matrix [r],[s],[t],[(-w)-u],[r],[-w])
//   ////MB_dc7ummp:
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -1, -1, -1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -2, -2, 1, 2, 2, 0})));
//   ////MC_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from +++ to --- for  g6[g6_A2]=g6[g6_2AB], g6[g6_2BC] >= g6[g6_2AC]"
//   //// matrix 1,0,0,0,0,0,  1,1,0,0,0,-1,  0,0,1,0,0,0,  0,0,0,-1,1,0,  0,0,0,0,-1,0,  -2,0,0,0,0,1)
//   ////MC_g6.[r,s,t,u,v,r]:
//   //// matrix [r],[s],[t],[v-u],[-v],[-r])
//   ////MC_dc7upmm:
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, -1, 1, 1, -1, 1, 1, 0, 2, 0, 2, 0, -1, 0, 0, 2, -1, 0, 0, 0, 2, 0, 4, -2, 2, 1, -2, 2, 0})));
//   ////MD_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from +++ to +++ for  g6[g6_A2]=g6[g6_2AB], g6[g6_2BC] < g6[g6_2AC]
//   //// matrix 1,0,0,0,0,0,  1,1,0,0,0,-1,  0,0,1,0,0,0,  0,0,0,-1,1,0,  0,0,0,0,1,0,  2,0,0,0,0,-1)
//   ////MD_g6.[r,s,t,u,v,r]:
//   //// matrix [r],[s],[t],[v-u],[v],[r])
//   ////MD_dc7upmm:
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0, 0, 3, 1, 1, 1, -1, -1, 0, 2, 0, 2, 0, -1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 2, -1, 0, 0, 0})));
//   ////ME_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to +++ for  g6[g6_A2]=-g6[g6_2AB]
//   //// matrix 1,0,0,0,0,0,  1,1,0,0,0,1,  0,0,1,0,0,0,  0,0,0,-1,-1,0,  0,0,0,0,-1,0,  2,0,0,0,0,1)
//   ////ME_g6.[r,s,t,u,v,-r]:
//   //// matrix [r],[s],[t],[(-v)-u],[-v],[r])
//   ////ME_dc7umpm:
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, -1, -1, -1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0})));
//   ////MF_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to --- for  g6[g6_A2]+g6[g6_B2]+g6[g6_C2]+g6[g6_2BC]+g6[g6_2AC]+g6[g6_2AB]=g6[g6_C2]
//   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,1,  1,1,1,1,1,1,  0,-2,0,-1,0,-1,  -2,0,0,0,-1,-1,  0,0,0,0,0,1)
//   ////MF_g6.[r,s,t,u,v,-r-s-u-v]:
//   //// matrix [r],[(-v)-u-r],[t],[v-s+r],[u+s-r],[(-v)-u-s-r])
//   ////"MF_dc7ummm:"
//   ////MF_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to --- for  
//      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, -1, -1, -1, 1, 1, 1, 0, -1, -1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, -1, -1, 0, 0, 0, 2, 0, -1, -1, 1, 0, 0, 1, 0})));
//
//
//   return vmat;
//}

double DC7uReflDist(const DC7u& dc_1, const DC7u dc_2) {
   const double d1 = sqrt(sqr(dc_1[0] - dc_2[0]) + sqr(dc_1[1] - dc_2[1]) + sqr(dc_1[2] - dc_2[2]) + sqr(dc_1[3] - dc_2[3]) + sqr(dc_1[4] - dc_2[4]) + sqr(dc_1[5] - dc_2[5]) + sqr(dc_1[6] - dc_2[6]));
   const double d2 = sqrt(sqr(dc_1[0] - dc_2[0]) + sqr(dc_1[1] - dc_2[2]) + sqr(dc_1[1] - dc_2[1]) + sqr(dc_1[3] - dc_2[3]) + sqr(dc_1[4] - dc_2[5]) + sqr(dc_1[5] - dc_2[4]) + sqr(dc_1[6] - dc_2[6]));
   const double d3 = sqrt(sqr(dc_1[0] - dc_2[1]) + sqr(dc_1[1] - dc_2[0]) + sqr(dc_1[1] - dc_2[2]) + sqr(dc_1[3] - dc_2[4]) + sqr(dc_1[4] - dc_2[3]) + sqr(dc_1[5] - dc_2[5]) + sqr(dc_1[6] - dc_2[6]));
   const double d4 = sqrt(sqr(dc_1[0] - dc_2[1]) + sqr(dc_1[1] - dc_2[2]) + sqr(dc_1[1] - dc_2[0]) + sqr(dc_1[3] - dc_2[4]) + sqr(dc_1[4] - dc_2[5]) + sqr(dc_1[5] - dc_2[3]) + sqr(dc_1[6] - dc_2[6]));
   const double d5 = sqrt(sqr(dc_1[0] - dc_2[2]) + sqr(dc_1[1] - dc_2[0]) + sqr(dc_1[1] - dc_2[1]) + sqr(dc_1[3] - dc_2[5]) + sqr(dc_1[4] - dc_2[3]) + sqr(dc_1[5] - dc_2[4]) + sqr(dc_1[6] - dc_2[6]));
   const
      double d6 = sqrt(sqr(dc_1[0] - dc_2[2]) + sqr(dc_1[1] - dc_2[1]) + sqr(dc_1[1] - dc_2[0]) + sqr(dc_1[3] - dc_2[5]) + sqr(dc_1[4] - dc_2[4]) + sqr(dc_1[5] - dc_2[3]) + sqr(dc_1[6] - dc_2[6]));
   const double dist = minNC(d1, d2, d3, d4, d5, d6);
   //std::cout << dc7_1 << std::endl;
   //std::cout << dc7_2 << std::endl;
   return sqrt(dist);
}

static bool CheckDC7u( const DC7u& dc7, const int& line=0) {
   bool good = true;
   std::string s(line!=0?LRL_ToString(" line=",line) : "");
   std::string issues;
   if (dc7.GetVector().size() != 7) {
      good = false;
      issues.append(LRL_ToString("   vector size equal to  ", 
         dc7.GetVector().size(),"\n"));
   }
   if (dc7.GetDim() != 7) {
      good = false;
      issues.append(LRL_ToString("   m_dim not equal to 7",
         dc7.GetDim(), "\n"));
      issues.append(LRL_ToString("   ", "\n"));
   }
   if (dc7.GetCell().GetVector().size() != 6) {
      good = false;
      issues.append(LRL_ToString("   cell dim not 6 ",
         dc7.GetCell().GetVector().size(),"\n"));
   }
   if (dc7.norm() == 0.0) {
      good = false;
      issues.append(LRL_ToString("   norm == 0.0", "\n"));
   }
   if (!dc7.GetValid()) {
      good = false;
      issues.append(LRL_ToString("   m_valid == false", "\n"));
   }

   if (!good) {
      std::cout << LRL_ToString("\n*********** problem DC7u, ", s, "\n")
         << "DC7U " << dc7 << std::endl
         << " m_cell " << dc7.GetCell() << std::endl
         << issues << std::endl;
   }
   return good;
}

void TestBoundaryMatrices(const std::vector<LRL_ReadLatticeData>& inputList) {
   const std::vector<MatDC7u> vmat = MatDC7u::MakeBoundaryMatrices();
   //for (size_t i = 0; i < vmat.size(); ++i) {
   //   std::cout << "matrix # " << i + 1 << std::endl;
   //   std::cout << vmat[i] << std::endl << std::endl;
   //}

  // MatDC7u m3; m3.SetVector({
  //6. / 7, -1. / 7, -1. / 7, 1. / 7, 1. / 7, 1. / 7, -1. / 7,
  //-1. / 7, 5. / 14, 5. / 14, 1. / 7, 1. / 7, 1. / 7, -1. / 7,
  //-1. / 7, 5. / 14, 5. / 14, 1. / 7, 1. / 7, 1. / 7, -1. / 7,
  //1. / 7, 1. / 7, 1. / 7, 6. / 7, -1. / 7, -1. / 7, 1. / 7,
  //1. / 7, 1. / 7, 1. / 7, -1. / 7, 6. / 7, -1. / 7, 1. / 7,
  //1. / 7, 1. / 7, 1. / 7, -1. / 7, -1. / 7, 6. / 7, 1. / 7,
  //-1. / 7, -1. / 7, -1. / 7, 1. / 7, 1. / 7, 1. / 7, 6. / 7
  //    });
  // m3 = m3 * 0.2;
  // m3 = 5. * m3;
  // m3 *= 3;
  // m3 /= 3;
  // m3 = m3 / 5;

  // m3[0] = -39393;
  // std::cout << m3 << std::endl << std::endl;
  // std::cout << m3 * 2 << std::endl << std::endl;
  // std::cout << m3 / 2 << std::endl << std::endl;
  // std::cout << m3 * m3 << std::endl << std::endl;

   //LRL_Cell cell("10 12 14 86 87 88");
   //DC7u fromCell(cell);
   //CheckDC7u(fromCell);
   //CheckDC7u(m3 * fromCell, __LINE__);
   //std::cout << m3 * dc7u << std::endl;

   //DC7u dc7u("1 1 1 1 1 1 1"); // expected to be invalid
   //CheckDC7u(dc7u, __LINE__); // expected to be invalid

   //DC7u xxxx(LRL_Cell("10 10 10 90 90 90"));
   ////std::cout << xxxx << std::endl;
   //CheckDC7u(xxxx, __LINE__);

   //DC7u dc1(G6("100 100 100 0 0 0"));
   //CheckDC7u(dc1, __LINE__);
   //DC7u dc2(G6("200 200 200 0 0 10"));
   //CheckDC7u(dc2);

   //std::cout << "d1  " << d1 << std::endl;
   //std::cout << "d2  " << d2 << std::endl;
   //std::cout << "minus  " << d2 - d1 << std::endl;



   for (size_t i = 0; i < inputList.size(); ++i) {
      const G6 g(inputList[i].GetCell());
      G6 gRed;
      Niggli::Reduce(g, gRed);
      const DC7u dcRed(gRed);
      std::cout << "gRed " << gRed << std::endl;
      for (size_t k = 0; k < vmat.size(); ++k) {
         //std::cout << "start k = " << k << std::endl;
         //if (k == 16 || k == 17 || k == 18) {
         //   std::cout << "skipping k = " << k << std::endl;
         //   continue;
         //}
         const DC7u dcMod = vmat[k] * dcRed;
         const G6 g2(dcMod);
         G6 g2Red;
         Niggli::Reduce(g2, g2Red);
         double difference = (gRed - g2Red).norm();
         if (difference < 1.0E-8) difference = 0.0;
         //std::cout << "g2Red " << g2Red << std::endl;
         if (difference > -1.0e-6) {
            //std::cout << " input " << i << " matrix " << k << std::endl;
            std::cout << " i,k " << i << "," << k << " difference " << difference << std::endl;
         }
         //std::cout << "finished i,k " << i << " " << k << std::endl;
      }
   }
}

DC7u GenerateReducedPerturbation(const G6& g6) {
   const DC7u dcInput(g6);
   if (!dcInput.IsReduced()) std::cout << "unreduced A" << std::endl;

   //DC7u perturbed;
   int count = 0;
   //do{
      const DC7u unitPerturbation =
         CreateTotallyRandom_Unit_OrthogonalComponent(dcInput);
   //   if (count != 0) std::cout << count << std::endl;
   //   perturbed = dcInput + unitPerturbation * dcInput.norm() / 100.0;
   //   ++count;
   //} while (!perturbed.IsReduced() && count < 10);

   const DC7u perturbed = dcInput + unitPerturbation * dcInput.norm() / 100.0;
   if (!perturbed.IsReduced()) std::cout << "unreduced G" << std::endl;

   G6 g6Red;
   Niggli::Reduce(G6(perturbed), g6Red);
   const DC7u perturbedReduced(g6Red);
   if (!perturbedReduced.IsReduced()) std::cout << "unreduced B" << std::endl;
   return perturbedReduced;
}

void TestDistanceBetween() {
   StoreResults<double, G6> store(5);

   store.SetTitle("DC7u distance to norm ratio as the Key, the data is " + G6::GetName());

   const size_t test = 420000;
   G6 g6randReduced;
   for (size_t i = 0; i < test; ++i) {
      const G6 g6rand = G6::rand();
      if (!LRL_Cell(g6rand).IsValid()) continue;
      if (!DC7u(g6rand).IsReduced()) std::cout << "unred E" << std::endl;
      const DC7u perturbed = GenerateReducedPerturbation(g6rand);
      if (!perturbed.IsReduced()) std::cout << "unred D" << std::endl;
      if (!perturbed.IsReduced()) continue;

      Niggli::Reduce(g6rand, g6randReduced);
      const DC7u dc7uRed(g6randReduced);
      const double dist = DistanceBetween(dc7uRed, perturbed);

      if (!DC7u(g6rand).IsReduced()) std::cout << "unred D" << std::endl;
      const double ratio = dist /DC7u(g6rand).norm();
      if (ratio < 0.0001 || ratio > 0.5) {
         std::cout << "odd ratio i= " << i << std::endl;
         std::cout << "dist = " << dist << std::endl;
         std::cout << "ratio = " << ratio << std::endl;
         std::cout << " g6rand " << g6rand << std::endl;
         std::cout << "dc7uRed " << dc7uRed << std::endl;
         std::cout << "perturbed " << perturbed << std::endl;
         std::cout << std::endl;
      }
      store.Store(int(ratio * 100000000) / 100000000.0, g6randReduced);
   }

   store.ShowResults();

}

void TestDistanceBetween(const std::vector<LRL_ReadLatticeData>& inputList) {
   StoreResults<double, G6> store(5);

   store.SetTitle("DC7u distance to norm ratio as the Key, the data is " + G6::GetName());

   G6 inputReduced;
   for (size_t i = 0; i < inputList.size(); ++i) {
      const G6 g6test = inputList[i].GetCell();
      if (!LRL_Cell(g6test).IsValid()) continue;
      if (!DC7u(g6test).IsReduced()) std::cout << "unred E" << std::endl;
      const DC7u perturbed = GenerateReducedPerturbation(g6test);
      if (!perturbed.IsReduced()) std::cout << "unred D" << std::endl;
      if (!perturbed.IsReduced()) continue;

      Niggli::Reduce(g6test, inputReduced);
      const DC7u dc7uRed(inputReduced);
      const double dist = DistanceBetween(dc7uRed, perturbed);

      if (!DC7u(g6test).IsReduced()) std::cout << "unred D" << std::endl;
      const double ratio = dist / DC7u(g6test).norm();
      if (ratio < 0.0001 || ratio > 0.5) {
         std::cout << "odd ratio i= " << i << std::endl;
         std::cout << "dist = " << dist << std::endl;
         std::cout << "ratio = " << ratio << std::endl;
         std::cout << " g6rand " << g6test << std::endl;
         std::cout << "dc7uRed " << dc7uRed << std::endl;
         std::cout << "perturbed " << perturbed << std::endl;
         std::cout << std::endl;
      }
      store.Store(int(ratio * 100000000) / 100000000.0, inputReduced);
   }

   store.ShowResults();
}

double  TestDistanceWithBoundaryMatrices(const double dist, 
   const G6& g6_1, const G6& g6_2) {
   static std::vector<MatDC7u> bounds = MatDC7u::MakeBoundaryMatrices();

   const DC7u dc1(g6_1);
   const DC7u dc2(g6_2);
   //const double dist = DistanceBetween(dc1, dc2);

   double newDist =  DBL_MAX;
   for (size_t i = 0; i < bounds.size(); ++i) {
      const double d = DistanceBetween(bounds[i] * dc1, dc2);
      newDist = std::min(newDist, d);
   }

   //if (newDist < dist) {
   //   std::cout << "shorter " << dist << " " << newDist << std::endl;
   //}
   //else {
   //   std::cout << "no improvement" << std::endl;
   //}

   return std::min(dist,newDist);

}

void VerifyBoundaryTransforms(const G6& g6) {
   const DC7u dcInput(g6);
   G6 red;
   Niggli::Reduce(g6, red);
   const DC7u dcRed(red);
   const std::vector<MatDC7u> vm = MatDC7u::MakeBoundaryMatrices();

   for (size_t i = 0; i < vm.size(); ++i) {
      std::cout << vm[i].GetDescr() << std::endl;
      std::cout << vm[i] << std::endl;
      std::cout << std::endl;
   }

   for (size_t i = 0; i < vm.size(); ++i) {
      if (vm[i].size() != 49) std::cout << "matrix size wrong " << vm[i].size() << std::endl;

      G6 g6red2;
      Niggli::Reduce(G6(vm[i] * dcRed), g6red2);
      const DC7u afterMultiply = vm[i] * dcRed;
      if (!afterMultiply.IsValid()) {
         std::cout << std::endl;
         std::cout << "invalid DC7u after reduction " << std::endl;
         std::cout << "g6red2 " << g6red2 << std::endl;
         std::cout << "dcRed " << dcRed << std::endl;

         std::cout << "afterMultiply " << afterMultiply << std::endl << std::endl;
      }



      if (!g6red2.IsValid()) {
         std::cout << std::endl;
         std::cout << "invalid after reduction " << std::endl;
         std::cout << "g6red2 " << g6red2 << std::endl;
         std::cout << "dcRed " << dcRed << std::endl;
         std::cout << "afterMultiply " << afterMultiply << std::endl << std::endl;
         std::cout << std::endl << vm[i] << std::endl;
      }
      else 
      {
         double distDC7u = DistanceBetween(dcRed, DC7u(g6red2));
         double distNiggli = (red - g6red2).norm();
         double distNCDist = NCDist(red.data(), g6red2.data());
         std::cout << "red " << red << std::endl;
         std::cout << "g6red2 " << g6red2 << std::endl;
         std::cout
            << "DC7u dist = " << distDC7u
            << "  niggli dist = " << distNiggli
            << "  NCDist = " << distNCDist << std::endl;
      }
   }
}


int main()
{
   std::cout << "; Test DC7unsrt" << std::endl;

   //TestDistanceBetween();
   //exit(0);

   LRL_ReadLatticeData reader;
   const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();

   StoreResults<double, G6> store(5);

   for (size_t i = 0; i < inputList.size(); ++i) {
      std::cout << " ======" << std::endl;
      LRL_Cell cell = inputList[i].GetCell();
      VerifyBoundaryTransforms(cell);
      //G6 cellRed;
      //Niggli::Reduce(cell, cellRed);
      //const DC7u dcCell(cell);
      //const DC7u perturbed = GenerateReducedPerturbation(DC7u(cellRed));
      //const double dist = DistanceBetween(DC7u(cellRed), DC7u(perturbed));
      //const double newDist = TestDistanceWithBoundaryMatrices(dist,cell,perturbed);
      //store.Store(int((newDist/dist)*100)/100.0, cell);
      ////std::cout << newDist << " " << dist << std::endl;
   }

   store.ShowResults();



   //TestDistanceBetween(inputList);

}
