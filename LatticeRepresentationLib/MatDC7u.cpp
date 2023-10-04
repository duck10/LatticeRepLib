#include "LRL_CoordinateConversionMatrices.h"
#include "MatDC7u.h"
#include "MatMN.h"
#include "MatN.h"

#include <iomanip>
#include <string>
#include <vector>

#pragma warning( disable : 4702) // unreachable code
#pragma warning( disable : 4100) // unreferenced formal parameter

//std::vector<MatDC7u> MatDC7u::vDU_Refl = MatDC7u::GetReflections();

std::vector<MatDC7u> MatDC7u::MakeBoundaryMatrices() {
   std::vector<MatDC7u> vmat;

   const std::string descr1("//M1_g6 { boundary transform from --- to --- or +++ to +++ at g6[g6_A2]=g6[G6_B2]\n "
      "//matrix ([ 0,1,0,0,0,0, 1,0,0,0,0,0, 0,0,1,0,0,0, 0,0,0,0,1,0, 0,0,0,1,0,0, 0,0,0,0,0,1])\n //M1_g6.[r,r,t,u,v,w]:\n // matrix [r],[r],[t],[v],[u],[w])");

   vmat.emplace_back(std::vector<double>{
      0, 1, 0, 0, 0, 0, 0,
      1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      -1, -1, -1, 1, 1, 1, 0 });
   vmat.back().SetDescr(descr1);
   vmat.back().SetFtype('-');
   vmat.back().SetName("M1a");

   vmat.emplace_back(std::vector<double>{
      0, 1, 0, 0, 0, 0, 0,
      1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      1, -1, 1, 1, -1, 1, 0 });
   vmat.back().SetDescr(descr1);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M1b");

   vmat.emplace_back(std::vector<double>{
      0, 1, 0, 0, 0, 0, 0,
      1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      -1, 1, 1, -1, 1, 1, 0 });
   vmat.back().SetDescr(descr1);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M1c");

   vmat.emplace_back(std::vector<double>{
      0, 1, 0, 0, 0, 0, 0,
      1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      1, 1, -1, 1, 1, -1, 0 });
   vmat.back().SetDescr(descr1);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M1d");


   const std::string descr2("//M2_g6 matrix boundary transform from --- to --- or +++ to +++ at g6[g6_B2]=g6[g6_C2\n "
      "//matrix([1,0,0,0,0,0, 0,0,1,0,0,0, 0,1,0,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0,1, 0,0,0,0,1,0])\n "
      "//M2_g6.[r,s,s,u,v,w]:\n // ([r],[s],[s],[u],[w],[v])");
   vmat.emplace_back(std::vector<double>{
      1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 1, 0, 0,
      -1, -1, -1, 1, 1, 1, 0 });
   vmat.back().SetDescr(descr2);
   vmat.back().SetFtype('-');
   vmat.back().SetName("M2a");

   vmat.emplace_back(std::vector<double>{
      1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 1, 0, 0,
      -1, 1, 1, -1, 1, 1, 0 });
   vmat.back().SetDescr(descr2);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M2b");

   vmat.emplace_back(std::vector<double>{
      1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 1, 0, 0,
      1, 1, -1, 1, 1, -1, 0 });
   vmat.back().SetDescr(descr2);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M2c");

   vmat.emplace_back(std::vector<double>{
      1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 1, 0, 0,
      1, -1, 1, 1, -1, 1, 0 });
   vmat.back().SetDescr(descr2);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M2d");

   const std::string descr3("//M3_g6 matrix boundary transform from --- to +++ for g6[g6_2BC]=0\n "
      "//matrix([1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,1.0E-15,1,0,0, 0,0,0,0,-1,0, 0,0,0,0,0,-1])\n "
      "//M3_g6.[r,s,t,0,v,w]:\n // ([r],[s],[t],[1.0E-15*t],[-v],[-w])\n "
      "//M3_dc7upmm");
   vmat.emplace_back(std::vector<double>{
      1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 2.0, 1.999999999999999, -1.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
      -1.0, -1.0, -0.999999999999999, 1.0, 1.0, 1.0, 0.0 });
   vmat.back().SetDescr(descr3);
   vmat.back().SetFtype('-');
   vmat.back().SetName("M3");

   const std::string descr4("//M4_g6 matrix boundary transform from --- to +++ for g6[g6_2AC]=0\n "
      "//matrix([1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,-1,0,0, 0,0,1.0E-15,0,1,0, 0,0,0,0,0,-1])\n "
      "//M4_g6.[r,s,t,u,0,w]:\n // ([r],[s],[t],[-u],[1.0E-15*t],[-w])\n "
      "//M4_dc7umpm");

   vmat.emplace_back(std::vector<double>{
      1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
      2.0, 0.0, 1.999999999999999, 0.0, -1.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
      -1.0, -1.0, -0.999999999999999, 1.0, 1.0, 1.0, 0.0 });
   vmat.back().SetDescr(descr4);
   vmat.back().SetFtype('-');
   vmat.back().SetName("M4");

   const std::string descr5("//M5_g6 matrix boundary transform from --- to +++ for g6[g6_2AB]=0\n "
      "//matrix([1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,-1,0,0, 0,0,0,0,-1,0, 0,0,1.0E-15,0,0,1])\n "
      "//M5_g6.[r,s,t,u,v,0]:\n "
      "// ([r],[s],[t],[-u],[-v],[1.0E-15*t])\n "
      "//M5_dc7ummp");
   vmat.emplace_back(std::vector<double>{
      1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
      2.0, 2.0, -1.0E-15, 0.0, 0.0, -1.0, 0.0,
      -1.0, -1.0, -0.999999999999999, 1.0, 1.0, 1.0, 0.0 });
   vmat.back().SetDescr(descr5);
   vmat.back().SetFtype('-');
   vmat.back().SetName("M5");

   const std::string descr6("//M6_g6 matrix boundary transform from +++ to --- for g6[g6_B2]=g6[g6_2BC], g6[g6_2AC]>=g6[g6_2AB]\n "
      "//matrix([1,0,0,0,0,0, 0,1,0,0,0,0, 0,1,1,-1,0,0, 0,-2,0,1,0,0, 0,0,0,0,-1,1, 0,0,0,0,0,-1])\n "
      "//M6_g6.[r,s,t,s,v,w]:\n "
      "// ([r],[s],[t],[-s],[w-v],[-w])\n "
      "//M6_dc7ummm");
   vmat.emplace_back(std::vector<double>{
      1, 0, 0, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      1, 1, -1, 1, 1, -1, 0,
      0, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 1, 0, 0 });
   vmat.back().SetDescr(descr6);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M6");

   const std::string descr7("//M7_g6 matrix boundary transform from +++ to --- for g6[g6_B2]=g6[g6_2BC], g6[g6_2AC]<g6[g6_2AB]\n "
      "//note that g6[g6_2AC] <= g2[g6_A2] <= g2[g6_B2] = g6[g6_2BC], so g6[g6_2AC]=v is minimal among\n "
      "//u, v, w and there is only one dc7u +++ MBD case to consider\n "
      "// ([1,0,0,0,0,0],[0,1,0,0,0,0],[0,1,1,-1,0,0],[0,2,0,-1,0,0], [0,0,0,0,-1,1],[0,0,0,0,0,1])\n "
      "// ([r],[s],[t],[s],[w-v],[w])\n "
      "//M7_dc7umpm");
   vmat.emplace_back(std::vector<double>{
      1, 0, 0, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      1, -1, 1, 1, -1, 1, 0,
      0, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 1, 0, 0 });
   vmat.back().SetDescr(descr7);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M7");

   const std::string descr8("//M8_g6 matrix boundary transform from --- to +++ for g6[g6_B2]=-g6[g6_2BC]\n "
      "// ([1,0,0,0,0,0, 0,1,0,0,0,0, 0,1,1,1,0,0, 0,2,0,1,0,0, 0,0,0,0,-1,-1, 0,0,0,0,0,-1])\n "
      "// M8_g6.[r,s,t,-s,v,w]:\n "
      "// ([r],[s],[t],[s],[(-w)-v],[-w])\n "
      "// M8_dc7ummp");
   vmat.emplace_back(std::vector<double>{
      1, 0, 0, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      -1, -1, -1, 1, 1, 1, 0,
      0, 0, 0, 0, 0, 1, 0,
      2, 0, 2, 0, -1, 0, 0 });
   vmat.back().SetDescr(descr8);
   vmat.back().SetFtype('-');
   vmat.back().SetName("M8");

   const std::string descr9("//M9_g6 matrix boundary transform from +++ to --- for g6[g6_A2]=g6[g6_2AC], g6[g6_2BC] >= g6[g6_AB]\n "
      "// ([1,0,0,0,0,0, 0,1,0,0,0,0, 1,0,1,0,1,0, 0,0,0,-1,0,1, -2,0,0,0,1,0, 0,0,0,0,0,-1])\n "
      "// ([1,0,0,0,0,0, 0,1,0,0,0,0, 1,0,1,0,-1,0, 0,0,0,-1,0,1, -2,0,0,0,1, 0,0,0,0,0,0,-1])\n "
   "//M9_g6.[r,s,t,u,r,w]:\n "
      "// ([r],[s],[t+2*r],[w-u],[-r],[-w])\n "
      "//M9_dc7ummp:\n "
      "//mat_g6ppp2dc7ummo");
   vmat.emplace_back(std::vector<double>{
      1,0,0,0,0,0,0,
      0,1,0,0,0,0,0,
      2,0,2,0,-1,0,0,
      3,1,1,1,-1,-1,0,
      6,0,3,0,-2,0,0,
      0,0,0,0,0,1,0,
      8,2,2,1,-2,-2,0 }); // corrected 2023/09/24
   vmat.back().SetDescr(descr9);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M9");

   const std::string descrA("//MA_g6 matrix boundary transform from +++ to +++ for g6[g6_A2]=g6[g6_2AC], g6[g6_2BC] < g6[g6_AB]\n "
      "// ([1,0,0,0,0,0],[0,1,0,0,0,0],[1,0,1,0,-1,0],[0,0,0,-1,0,1],[2,0,0,0,-1,0],[0,0,0,0,0,1])\n "
      "// MA_g6.[r,s,t,u,r,w]:\n "
      "// ([r],[s],[t],[w-u],[r],[w])\n "
      "// MA_dc7upmm:");
   vmat.emplace_back(std::vector<double>{
      1, 0, 0, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0,
      -1, 1, 1, -1, 1, 1, 0,
      0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 1, 0, 0, 0 });
   vmat.back().SetDescr(descrA);
   vmat.back().SetFtype('+');
   vmat.back().SetName("MA");

   const std::string descrB("//MB_g6 matrix boundary transform from --- to +++ for g6[g6_A2]=-g6[g6_2AC]\n "
      "// ([1,0,0,0,0,0, 0,1,0,0,0,0, 1,0,1,0,1,0, 0,0,0,-1,0,-1, 2,0,0,0,1,0, 0,0,0,0,0,-1)\n "
      "//MB_g6.[r,s,t,u,-r,w]:\n "
      "//matrix([r],[s],[t],[(-w)-u],[r],[-w])\n "
      "//MB_dc7ummp");
   vmat.emplace_back(std::vector<double>{
      1, 0, 0, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0,
      -1, -1, -1, 1, 1, 1, 0,
      0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      0, -2, -2, 1, 2, 2, 0 });
   vmat.back().SetDescr(descrB);
   vmat.back().SetFtype('-');
   vmat.back().SetName("MB");

   const std::string descrC("//MC_g6 matrix boundary transform from +++ to --- for g6[g6_A2]=g6[g6_2AB], g6[g6_2BC] >= g6[g6_2AC]\n "
      "// ([1,0,0,0,0,0, 1,1,0,0,0,-1, 0,0,1,0,0,0, 0,0,0,-1,1,0, 0,0,0,0,-1,0, -2,0,0,0,0,1])\n "
      "// MC_g6.[r,s,t,u,v,r]:\n "
      "// ([r],[s],[t],[v-u],[-v],[-r])\n "
      "// MC_dc7upmm");
   vmat.emplace_back(std::vector<double>{
      1, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      0, 0, 1, 0, 0, 0, 0,
      -1, 1, 1, -1, 1, 1, 0,
      2, 0, 2, 0, -1, 0, 0,
      2, -1, 0, 0, 0, 2, 0,
      4, -2, 2, 1, -2, 2, 0 });
   vmat.back().SetDescr(descrC);
   vmat.back().SetFtype('+');
   vmat.back().SetName("MC");

   const std::string descrD("//MD_g6 matrix boundary transform from +++ to +++ for g6[g6_A2]=g6[g6_2AB], g6[g6_2BC] < g6[g6_2AC]\n "
      "// ([1,0,0,0,0,0, 1,1,0,0,0,-1, 0,0,1,0,0,0, 0,0,0,-1,1,0, 0,0,0,0,1,0, 2,0,0,0,0,-1)\n "
      "// MD_g6.[r,s,t,u,v,r]:\n "
      "// ([r],[s],[t],[v-u],[v],[r])\n "
      "// MD_dc7upmm");
   vmat.emplace_back(std::vector<double>{
      1, 0, 0, 0, 0, 0, 0,
      2, 2, 0, 0, 0, -1, 0,
      0, 0, 1, 0, 0, 0, 0,
      3, 1, 1, 1, -1, -1, 0,
      2, 0, 2, 0, -1, 0, 0,
      0, 1, 0, 0, 0, 0, 0,
      0, 2, 2, -1, 0, 0, 0 });
   vmat.back().SetDescr(descrD);
   vmat.back().SetFtype('+');
   vmat.back().SetName("MD");

   const std::string descrE("//ME_g6 matrix boundary transform from --- to +++ for g6[g6_A2]=-g6[g6_2AB]\n "
      "// ([1,0,0,0,0,0, 1,1,0,0,0,1, 0,0,1,0,0,0, 0,0,0,-1,-1,0, 0,0,0,0,-1,0, 2,0,0,0,0,1)\n "
      "//ME_g6.[r,s,t,u,v,-r]:\n "
      "//([r],[s],[t],[(-v)-u],[-v],[r])\n "
      "//ME_dc7umpm");
   vmat.emplace_back(std::vector<double>{
      1, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      0, 0, 1, 0, 0, 0, 0,
      -1, -1, -1, 1, 1, 1, 0,
      0, 0, 0, 0, 1, 0, 0,
      0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 1, 0, 0, 0 });
   vmat.back().SetDescr(descrE);
   vmat.back().SetFtype('-');
   vmat.back().SetName("ME");

   const std::string descrF("//MF_g6 matrix boundary transform from --- to --- for g6[g6_A2]+g6[g6_B2]+g6[g6_C2]+g6[g6_2BC]+g6[g6_2AC]+g6[g6_2AB]=g6[g6_C2]\n "
      "// ([1,0,0,0,0,0, 0,1,0,0,0,0, 1,1,1,1,1,1, 0,-2,0,-1,0,-1, -2,0,0,0,-1,-1, 0,0,0,0,0,1)\n "
      "// MF_g6.[r, s, t, u, v, -r - s - u - v]:\n "
      "// matrix([r],[s],[t],[v-s+r],[u+s-r],[-v-u-s-r])\n "
      "// MF_dc7ummm:");
   vmat.emplace_back(std::vector<double>{
      1, 0, 0, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0,
      -1, -1, -1, 1, 1, 1, 0,
      0,0,0,0,1,0,0,
      0,0,0,1,0,0,0,
      0,0,0,0,0,1,0,
      0,0,1,0,0,0,0 });
   vmat.back().SetDescr(descrF);
   vmat.back().SetFtype('-');
   vmat.back().SetName("MF");
   return vmat;
}

template<typename T>
bool IsPlusPlusPlus(const T& t) {
   const G6 g6(t);
   return (g6[3] > 0) && (g6[4] > 0) && (g6[5] > 0);
}

bool MatDC7u::ShouldApply(const G6& g) const
{
   const MatDC7u& m(*this);
   const char& ftype = m.m_Ftype;

   return (ftype == '0') ||
      (ftype == '+' && IsPlusPlusPlus(g)) ||
      (ftype == '-' && !IsPlusPlusPlus(g));
}

MatDC7u::MatDC7u(void)
   : m_mat(49)
{}

MatDC7u::MatDC7u(const std::string& s)
   : MatDC7u()
{
   MatN mn(m_mat.FromString(s));
   (*this).m_mat = mn;
}

MatDC7u::MatDC7u(const std::vector<double>& v)
   : MatDC7u()
{
   for (size_t i = 0; i < 49; ++i) {
      m_mat[i] = v[i];
   }
}

MatDC7u& MatDC7u::operator= (const MatB4& m) {
   m_mat.resize(49);
   throw("we don't know how to do this");
   return *this;
}

MatDC7u& MatDC7u::operator= (const MatG6& m) {
   throw ("how to do this");
   return *this;
}

MatDC7u& MatDC7u::operator= (const MatS6& m) {
   MatMN mn = LRL_CoordinateConversionMatrices::G6_FROM_S6 * m * LRL_CoordinateConversionMatrices::S6_FROM_G6;
   m_mat.SetVector(mn.GetVector());
   return *this;
}

MatDC7u& MatDC7u::operator= (const MatDC7u& m) {
   m_mat.resize(49);
   m_mat = m.m_mat;
   return *this;
}

MatDC7u& MatDC7u::operator= (const MatD7& m) {
   MatMN mn = LRL_CoordinateConversionMatrices::G6_FROM_D7 * m * LRL_CoordinateConversionMatrices::D7_FROM_G6;
   m_mat.SetVector(mn.GetVector());
   return *this;
}

MatDC7u& MatDC7u::operator+= (const MatDC7u& d) {
   for (size_t i = 0; i < 49; ++i) m_mat[i] += d[i];
   return *this;
}

MatDC7u& MatDC7u::operator-= (const MatDC7u& d) {
   for (size_t i = 0; i < 49; ++i) m_mat[i] -= d[i];
   return *this;
}

bool MatDC7u::operator== (const MatDC7u& m) const {
   for (size_t i = 0; i < 49; ++i) if (m_mat[i] != m[i]) return false;
   return true;
}

bool MatDC7u::operator!= (const MatDC7u& m) const {
   return !((*this) == m);
}

MatDC7u& MatDC7u::operator/= (const double d) {
   for (size_t i = 0; i < 49; ++i) m_mat[i] /= d;
   return *this;
}

MatDC7u& MatDC7u::operator*= (const double d) {
   for (size_t i = 0; i < 49; ++i) m_mat[i] *= d;
   return *this;
}

MatDC7u MatDC7u::operator+ (const MatDC7u& m) const {
   MatDC7u m7(*this);
   return (m7 += m);
}

MatDC7u MatDC7u::operator- (const MatDC7u& m) const {
   MatDC7u m7(*this);
   return (m7 -= m);
}

MatDC7u MatDC7u::operator- (void) const { // unary
   MatDC7u m(*this);
   m.m_mat = -m.m_mat;
   return m; // unary
}

MatDC7u MatDC7u::operator* (const MatDC7u& m2) const {
   MatDC7u m(*this);
   m.m_mat *= m2.m_mat;
   return m;
}

MatDC7u MatDC7u::operator* (const double d) const {
   MatDC7u m(*this);
   for (size_t i = 0; i < 49; ++i) m.m_mat[i] *= d;
   return m;
}

MatDC7u MatDC7u::operator/ (const double d) const {
   MatDC7u m(*this);
   for (size_t i = 0; i < 49; ++i) m.m_mat[i] /= d;
   return m;
}

MatDC7u operator*(const double d, const MatDC7u& m) {
   return m * d;
}

MatDC7u operator/(const double d, const MatDC7u& m) {
   return m / d;
}

DC7u MatDC7u::operator* (const DC7u& v) const {
   DC7u g7(v);
   g7.SetVector((m_mat * v.GetVector()).GetVector());
   g7.SetValid(g7[0] > 0.001 && g7[1] > 0.001 && g7[2] > 0.001);
   return g7;
}

double MatDC7u::operator[] (const size_t n) const {
   return m_mat[n];
}

double& MatDC7u::operator[] (const size_t n) {
   return m_mat[n];
}


double MatDC7u::norm() const {
   double sum = 0.0;
   for (size_t i = 0; i < 7 * 7; ++i) sum += m_mat[i] * m_mat[i];
   return sqrt(sum);
}

double MatDC7u::norm(const MatDC7u& t) const {
   return t.norm();
}

double MatDC7u::Norm() const {
   return norm();
}

double MatDC7u::Norm(const MatDC7u& t) const {
   return t.norm();
}


double MatDC7u::DistanceBetween(const MatDC7u& v1, const MatDC7u& v2) {
   return ((v1 - v2).norm());
}

bool MatDC7u::IsUnit() const {
   long row = -1;
   for (size_t i = 0; i < 49; ++i) {
      long column = i % 6;
      if (column == 0) ++row;
      if (column == row && m_mat[i] != 1.0) return false;
      if (column != row && m_mat[i] != 0.0) return false;
   }
   return true;
}

MatDC7u MatDC7u::Eye(void) {
   MatDC7u m;
   for (size_t i = 0; i < 49; ++i)
      m.m_mat[i] = 0.0;
   for (size_t i = 0; i < 49; i += 7)
      m.m_mat[i] = 1.0;
   return m;
}

MatDC7u MatDC7u::Zero(void) {
   MatDC7u m;
   for (size_t i = 0; i < 49; ++i)
      m[i] = 0.0;
   return m;
}


MatDC7u MatDC7u::transpose(const MatDC7u& m2) const {
   // transpose a symmetrical matrix
   const int size = 7;
   MatDC7u m;
   for (int count = 0; count < size * size; ++count) {
      const int transposeIndex = count / size + size * (count % size); //'=int(rowindex/7) +7*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
   return m;
}

void MatDC7u::transpose(void) {
   // transpose a symmetrical matrix
   const int size = 7;
   MatDC7u& m(*this);
   MatDC7u m2(*this);
   for (int count = 0; count < size * size; ++count) {
      const int transposeIndex = count / size + size * (count % size); //'=int(rowindex/7) +7*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
}

std::vector<MatDC7u> MatDC7u::MakeBoundaryProjectors() {
   std::vector<MatDC7u> vmat;

   const std::string descr1_Prj("//Case 1. $r=s$:  The cells in this case may be either $+++$ or $---$.\n");
   //$dc7_{ unsrt,1 } = dc7_{ unsrt,2 }$.
   //\[dc7_{ unsrt }  \text{ subspace 1 } = [r, r, t, r + t - | u |, r + t - | v |, 2r - | w |, \]
   //\[2r + t - | u | -| v | -| w | +min(max(0, u), max(0, v), max(0, w))]\]
   //\[dc7_{ unsrt } \text{ boundary 1 }-- - \text{ projector } = \]

   vmat.emplace_back(std::vector<double>{
      5.0 / 14, 5.0 / 14, -1.0 / 7, 1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      5.0 / 14, 5.0 / 14, -1.0 / 7, 1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      -1.0 / 7, -1.0 / 7, 6.0 / 7, 1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      1.0 / 7, 1.0 / 7, 1.0 / 7, 6.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7,
      1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, 6.0 / 7, -1.0 / 7, 1.0 / 7,
      1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, -1.0 / 7, 6.0 / 7, 1.0 / 7,
      -1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, 1.0 / 7, 6.0 / 7 });
   vmat.back().SetDescr(descr1_Prj);
   vmat.back().SetFtype('-');
   vmat.back().SetName("M1a");

   //\[dc7_{ unsrt } \text{ boundary 1 } ++ + \text{ minimal }u \text{ projector } = \]

   vmat.emplace_back(std::vector<double>{
      1.0 / 2, 1.0 / 2, 0, 0, 0, 0, 0,
      1.0 / 2, 1.0 / 2, 0, 0, 0, 0, 0,
      0, 0, 4.0 / 5, 1.0 / 5, -1.0 / 5, -1.0 / 5, 1.0 / 5,
      0, 0, 1.0 / 5, 4.0 / 5, 1.0 / 5, 1.0 / 5, -1.0 / 5,
      0, 0, -1.0 / 5, 1.0 / 5, 4.0 / 5, -1.0 / 5, 1.0 / 5,
      0, 0, -1.0 / 5, 1.0 / 5, -1.0 / 5, 4.0 / 5, 1.0 / 5,
      0, 0, 1.0 / 5, -1.0 / 5, 1.0 / 5, 1.0 / 5, 4.0 / 5 });
   vmat.back().SetDescr(descr1_Prj);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M1b");


   //\[dc7_{ unsrt } \text{ boundary 1 } ++ + \text{ minimal }v \text{ projector } = \]

   vmat.emplace_back(std::vector<double>{
      1.0 / 2, 1.0 / 2, 0, 0, 0, 0, 0,
      1.0 / 2, 1.0 / 2, 0, 0, 0, 0, 0,
      0, 0, 4.0 / 5, -1.0 / 5, 1.0 / 5, -1.0 / 5, 1.0 / 5,
      0, 0, -1.0 / 5, 4.0 / 5, 1.0 / 5, -1.0 / 5, 1.0 / 5,
      0, 0, 1.0 / 5, 1.0 / 5, 4.0 / 5, 1.0 / 5, -1.0 / 5,
      0, 0, -1.0 / 5, -1.0 / 5, 1.0 / 5, 4.0 / 5, 1.0 / 5,
      0, 0, 1.0 / 5, 1.0 / 5, -1.0 / 5, 1.0 / 5, 4.0 / 5 });
   vmat.back().SetDescr(descr1_Prj);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M1c");


   //\[dc7_{ unsrt } \text{ boundary 1 } ++ + \text{ minimal }w \text{ projector } = \]

   vmat.emplace_back(std::vector<double>{
      5.0 / 14, 5.0 / 14, 1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7,
      5.0 / 14, 5.0 / 14, 1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7,
      1.0 / 7, 1.0 / 7, 6.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, -1.0 / 7,
      -1.0 / 7, -1.0 / 7, 1.0 / 7, 6.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7,
      -1.0 / 7, -1.0 / 7, 1.0 / 7, -1.0 / 7, 6.0 / 7, 1.0 / 7, 1.0 / 7,
      1.0 / 7, 1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, 6.0 / 7, -1.0 / 7,
      1.0 / 7, 1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, 6.0 / 7 });
   vmat.back().SetDescr(descr1_Prj);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M1d");

   //\[dc7_{ unsrt } \text{ boundary 1 transforms } = \]
   //   \[-- - :[s, r, t, v + t + r, u + t + s, w + s + r, w + v + u + t + s + r] \]
   //   \[++ + :[s, r, t, -v + t + r, -u + t + s, -w + s + r, -w - v + 2 min(u, v, w) - u + t + s + r] \]


   const std::string descr2_Prj("//Case 2. $s=t$:  The cells in this case may be either $+++$ or $---$.\n");
   //$dc7_{ unsrt,2 } = dc7_{ unsrt,3 }$.
   //\[dc7_{ unsrt }  \text{ subspace 2 } = [r, s, s, 2s - | u |, r + s - | v |, r + s - | w |, \]
   //\[r + 2s - | u | -| v | -| w | +min(max(0, u), max(0, v), max(0, w))]\]
   //\[dc7_{ unsrt } \text{ boundary 2 }-- - \text{ projector } = \]

   vmat.emplace_back(std::vector<double>{
      6.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      -1.0 / 7, 5.0 / 14, 5.0 / 14, 1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      -1.0 / 7, 5.0 / 14, 5.0 / 14, 1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      1.0 / 7, 1.0 / 7, 1.0 / 7, 6.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7,
      1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, 6.0 / 7, -1.0 / 7, 1.0 / 7,
      1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, -1.0 / 7, 6.0 / 7, 1.0 / 7,
      -1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, 1.0 / 7, 6.0 / 7 });
   vmat.back().SetDescr(descr2_Prj);
   vmat.back().SetFtype('-');
   vmat.back().SetName("M2a");


   //\[dc7_{ unsrt } \text{ boundary 2 } ++ + \text{ minimal }u \text{ projector } = \]

   vmat.emplace_back(std::vector<double>{
      6.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      1.0 / 7, 5.0 / 14, 5.0 / 14, 1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7,
      1.0 / 7, 5.0 / 14, 5.0 / 14, 1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7,
      -1.0 / 7, 1.0 / 7, 1.0 / 7, 6.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7, 6.0 / 7, -1.0 / 7, 1.0 / 7,
      1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7, -1.0 / 7, 6.0 / 7, 1.0 / 7,
      -1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, 6.0 / 7 });
   vmat.back().SetDescr(descr2_Prj);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M2b");


   //\[dc7_{ unsrt } \text{ boundary 2 } ++ + \text{ minimal }v \text{ projector } = \]

   vmat.emplace_back(std::vector<double>{
      4.0 / 5, 0, 0, -1.0 / 5, 1.0 / 5, -1.0 / 5, 1.0 / 5,
      0, 1.0 / 2, 1.0 / 2, 0, 0, 0, 0,
            0, 1.0 / 2, 1.0 / 2, 0, 0, 0, 0,
            -1.0 / 5, 0, 0, 4.0 / 5, 1.0 / 5, -1.0 / 5, 1.0 / 5,
            1.0 / 5, 0, 0, 1.0 / 5, 4.0 / 5, 1.0 / 5, -1.0 / 5,
            -1.0 / 5, 0, 0, -1.0 / 5, 1.0 / 5, 4.0 / 5, 1.0 / 5,
            1.0 / 5, 0, 0, 1.0 / 5, -1.0 / 5, 1.0 / 5, 4.0 / 5 });
   vmat.back().SetDescr(descr2_Prj);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M2c");




   //\[dc7_{ unsrt } \text{ boundary 2 } ++ + \text{ minimal }w \text{ projector } = \]

   vmat.emplace_back(std::vector<double>{
      4.0 / 5, 0, 0, -1.0 / 5, -1.0 / 5, 1.0 / 5, 1.0 / 5,
      0, 1.0 / 2, 1.0 / 2, 0, 0, 0, 0,
      0, 1.0 / 2, 1.0 / 2, 0, 0, 0, 0,
      -1.0 / 5, 0, 0, 4.0 / 5, -1.0 / 5, 1.0 / 5, 1.0 / 5,
      -1.0 / 5, 0, 0, -1.0 / 5, 4.0 / 5, 1.0 / 5, 1.0 / 5,
      1.0 / 5, 0, 0, 1.0 / 5, 1.0 / 5, 4.0 / 5, -1.0 / 5,
      1.0 / 5, 0, 0, 1.0 / 5, 1.0 / 5, -1.0 / 5, 4.0 / 5 });
   vmat.back().SetDescr(descr2_Prj);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M2d");



   //\[dc7_{ unsrt } \text{ boundary 2 transforms } = \]
   //   \[-- - :[r, t, s, u + t + s, w + s + r, v + t + r, w + v + u + t + s + r] \]
   //   \[++ + :[r, t, s, -u + t + s, -w + s + r, -v + t + r, -w - v + 2 min(u, v, w) - u + t + s + r] \]
   //   \end{ itemize }

   //   The special - position subspaces $\hat{ 1 }$ and $\hat{ 2 }$ are obtained by adding the constraints $1 ^ \prime\!: \{u = v\}$ and
   //   $2^ \prime\!:\{v = w\}$, respectively.

   //   \subsection{ $90 ^ \circ$ case }

   //   The $90^ \circ$ case marks a possible transition between $-- - $ and $++ + $.
   //   All the cells with a $90 ^ \circ$ angle are in $-- - $.

   const std::string descr3_Prj("//Case 3. $u=0$:  The cells in this case must be $---$.\n");
   //$dc7_{ unsrt,2 }\, +\, dc7_{ unsrt,3 }\, -\, dc7_{ unsrt,4 } = 0$.
   //\[dc7_{ unsrt }  \text{ subspace 3 } = [r, s, t, s + t, r + t + v, r + s + w, r + s + t + v + w]\]
   //\[dc7_{ unsrt } \text{ boundary 3 projector } = \]

   vmat.emplace_back(std::vector<double>{
      3.0 / 4, 0, 0, 0, 1.0 / 4, 1.0 / 4, -1.0 / 4,
      0, 2.0 / 3, -1.0 / 3, 1.0 / 3, 0, 0, 0,
      0, -1.0 / 3, 2.0 / 3, 1.0 / 3, 0, 0, 0,
      0, 1.0 / 3, 1.0 / 3, 2.0 / 3, 0, 0, 0,
      1.0 / 4, 0, 0, 0, 3.0 / 4, -1.0 / 4, 1.0 / 4,
      1.0 / 4, 0, 0, 0, -1.0 / 4, 3.0 / 4, 1.0 / 4,
      -1.0 / 4, 0, 0, 0, 1.0 / 4, 1.0 / 4, 3.0 / 4 });
   vmat.back().SetDescr(descr3_Prj);
   vmat.back().SetFtype('-');
   vmat.back().SetName("M3");

   //\[dc7_{ unsrt } \text{ boundary 3 transform } = \]
   //   \ [[r, s, t, -u + t + s, v + t + r, w + s + r, w + v + 2 min(u, -v, -w) - u + t + s + r]\]

   const std::string descr4_Prj("//Case 4. $v=0$:  The cells in this case must be $---$.\n");
   //$dc7_{ unsrt,1 }\, +\, dc7_{ unsrt,3 }\, -\, dc7_{ unsrt,5 } = 0$.
   //\[dc7_{ unsrt } \text{ subspace 4 } = [r, s, t, s + t + u, r + t, r + s + w, r + s + t + u + w]\]
   //\[dc7_{ unsrt } \text{ boundary 4 projector } = \]

   vmat.emplace_back(std::vector<double>{
      2.0 / 3, 0, -1.0 / 3, 0, 1.0 / 3, 0, 0,
      0, 3.0 / 4, 0, 1.0 / 4, 0, 1.0 / 4, -1.0 / 4,
      -1.0 / 3, 0, 2.0 / 3, 0, 1.0 / 3, 0, 0,
      0, 1.0 / 4, 0, 3.0 / 4, 0, -1.0 / 4, 1.0 / 4,
      1.0 / 3, 0, 1.0 / 3, 0, 2.0 / 3, 0, 0,
      0, 1.0 / 4, 0, -1.0 / 4, 0, 3.0 / 4, 1.0 / 4,
      0, -1.0 / 4, 0, 1.0 / 4, 0, 1.0 / 4, 3.0 / 4 });
   vmat.back().SetDescr(descr4_Prj);
   vmat.back().SetFtype('-');
   vmat.back().SetName("M4");

   //\[dc7_{ unsrt } \text{ boundary 4 transform } = \]
   //   \ [[r, s, t, u + t + s, -v + t + r, w + s + r, w - v + u + 2 min(-u, v, -w) + t + s + r]\]

   const std::string descr5_Prj("//Case 5. $w=0$:  The cells in this case must be $---$.\n");
   //$dc7_{ unsrt,1 }\, +\, dc7_{ unsrt,2 }\, -\, dc7_{ unsrt,6 } = 0$.
   //\[dc7_{ unsrt } \text{ subspace 5 } = [r, s, t, s + t + u, r + t + v, r + s, r + s + t + u + v]\]
   //\[dc7_{ unsrt } \text{ boundary 5 projector } = \]

   vmat.emplace_back(std::vector<double>{
      2.0 / 3, -1.0 / 3, 0, 0, 0, 1.0 / 3, 0,
      -1.0 / 3, 2.0 / 3, 0, 0, 0, 1.0 / 3, 0,
      0, 0, 3.0 / 4, 1.0 / 4, 1.0 / 4, 0, -1.0 / 4,
      0, 0, 1.0 / 4, 3.0 / 4, -1.0 / 4, 0, 1.0 / 4,
      0, 0, 1.0 / 4, -1.0 / 4, 3.0 / 4, 0, 1.0 / 4,
      1.0 / 3, 1.0 / 3, 0, 0, 0, 2.0 / 3, 0,
      0, 0, -1.0 / 4, 1.0 / 4, 1.0 / 4, 0, 3.0 / 4 });
   vmat.back().SetDescr(descr5_Prj);
   vmat.back().SetFtype('-');
   vmat.back().SetName("M5");


   //\[dc7_{ unsrt } \text{ boundary 5 transform } = \]
   //   \ [[r, s, t, u + t + s, v + t + r, -w + s + r, -w + v + u + 2 min(-u, -v, w) + t + s + r]\]
   //   \end{ itemize }

   //   In each ninety - degree case, the special - position subspace consists of $\hat{ 3 }, \hat{ 4 }, \hat{ 5 }\!: \{u = v = w = 0\}$,
   //   \it i.e.} the primitive orthorhombic case, and we take
   //   $3^ \prime\!: \{v = w = 0\}$, $4^ \prime\!: \{u = w = 0\}$, and $5^ \prime\!: \{u = v = 0\}$.


   //   \subsection{ Face - diagonal  case }

   //   Recall that $ | u | \leq s$, $ | v | \leq r$, and $ | w | \leq r$.Equality marks
   //   the transition from edges being smaller than face diagonals to face diagonals
   //   possibly being smaller than the Niggli - reduced cell edges.


   const std::string descr6_Prj("Case 6. $s=u, v ge w$:  The cells in this case must be $+++$.\n");
   //$\tau = -\sum_{ i = 1 } ^ 3(-dc7_{ unsrt,i }) + \sum_{ i = 4 } ^ 6(dc7_{ unsrt,i }) < dc7_{ unsrt,7 }$\\
         //$dc7_{ unsrt,2 } = dc7_{ unsrt,2 }\, +\, dc7_{ unsrt,3 }\, -\, dc7_{ unsrt,4 }$\\
         //equivalent to
         //$dc7_{ unsrt,3 }\, =\, dc7_{ unsrt,4 }$\\
         //$dc7_{ unsrt,3 }\, -\, dc7_{ unsrt,5 } \ge
         //dc7_{ unsrt,2 }\, -\, dc7_{ unsrt,6 }$.
         //\[dc7_{ unsrt } \text{ subspace 6 } = [r, s, t, t, r + t - v, r + s - w, r + t - v + w], \, v \geq w > 0 \]
         //\[dc7_{ unsrt } \text{ boundary 6 projector } = \]

   vmat.emplace_back(std::vector<double>{
      4.0 / 5, -1.0 / 5, 0, 0, -1.0 / 5, 1.0 / 5, 1.0 / 5,
      -1.0 / 5, 4.0 / 5, 0, 0, -1.0 / 5, 1.0 / 5, 1.0 / 5,
      0, 0, 1.0 / 2, 1.0 / 2, 0, 0, 0,
      0, 0, 1.0 / 2, 1.0 / 2, 0, 0, 0,
      -1.0 / 5, -1.0 / 5, 0, 0, 4.0 / 5, 1.0 / 5, 1.0 / 5,
      1.0 / 5, 1.0 / 5, 0, 0, 1.0 / 5, 4.0 / 5, -1.0 / 5,
      1.0 / 5, 1.0 / 5, 0, 0, 1.0 / 5, -1.0 / 5, 4.0 / 5 });
   vmat.back().SetDescr(descr6_Prj);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M6");



   //\[dc7_{ unsrt } \text{ boundary 6 transform } = \]
   //   \ [[r, s, -u + t + s, t, w - v - u + t + s + r, -w + s + r, -v + t + r]\]

   const std::string descr7_Prj("//Case 7. $s=u, v < w$:  The cells in this case must be $+++$.\n");
   //$\tau = -\sum_{ i = 1 } ^ 3(-dc7_{ unsrt,i }) + \sum_{ i = 4 } ^ 6(dc7_{ unsrt,i }) < dc7_{ unsrt,7 }$\\
         //$dc7_{ unsrt,2 } = dc7_{ unsrt,2 }\, +\, dc7_{ unsrt,3 }\, -\, dc7_{ unsrt,4 }$\\
         //equivalent to
         //$dc7_{ unsrt,3 }\, =\, dc7_{ unsrt,4 }$\\
         //$dc7_{ unsrt,3 }\, -\, dc7_{ unsrt,5 } <
         //dc7_{ unsrt,2 }\, -\, dc7_{ unsrt,6 }$.
         //\[dc7_{ unsrt } \text{ subspace 7 } = [r, s, t, t, r + t - v, r + s - w, r + t + v - w], \, w > v > 0 \]
         //\[dc7_{ unsrt } \text{ boundary 7 projector } = \]

   vmat.emplace_back(std::vector<double>{
      6.0 / 7, 1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7, -1.0 / 7, 1.0 / 7,
      1.0 / 7, 6.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, 1.0 / 7, -1.0 / 7,
      -1.0 / 7, 1.0 / 7, 5.0 / 14, 5.0 / 14, 1.0 / 7, -1.0 / 7, 1.0 / 7,
      -1.0 / 7, 1.0 / 7, 5.0 / 14, 5.0 / 14, 1.0 / 7, -1.0 / 7, 1.0 / 7,
      1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, 6.0 / 7, 1.0 / 7, -1.0 / 7,
      -1.0 / 7, 1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7, 6.0 / 7, 1.0 / 7,
      1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, 1.0 / 7, 6.0 / 7 });
   vmat.back().SetDescr(descr7_Prj);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M7");


   //\[dc7_{ unsrt } \text{ boundary 7 transform } = \]
   //   \ [[r, s, -u + t + s, t, -w + v - u + t + s + r, -w + s + r, -2 * w + v + 2 * min(2 * s - u, w, w - v) + t + r]\]

   const std::string descr8_Prj("//Case 8. $s=-u$:  The cells in this case must be $---$.\n");
   //$\tau = -\sum_{ i = 1 } ^ 3(-dc7_{ unsrt,i }) + \sum_{ i = 4 } ^ 6(dc7_{ unsrt,i }) = dc7_{ unsrt,7 }$\\
         //$dc7_{ unsrt,2 } = dc7_{ unsrt,2 }\, \, +dc7_{ unsrt,3 }\, -\, dc7_{ unsrt,4 }$\\
         //equivalent to
         //$dc7_{ unsrt,3 }\, =\, dc7_{ unsrt,4 }$.
         //\[dc7_{ unsrt } \text{ subspace 8 } = [r, s, t, t, r + t + v, r + s + w, r + t + v + w], \, v, w \leq 0 \]
         //\[dc7_{ unsrt } \text{ boundary 8 projector } = \]

   vmat.emplace_back(std::vector<double>{
      4.0 / 5, -1.0 / 5, 0, 0, 1.0 / 5, 1.0 / 5, -1.0 / 5,
      -1.0 / 5, 4.0 / 5, 0, 0, 1.0 / 5, 1.0 / 5, -1.0 / 5,
      0, 0, 1.0 / 2, 1.0 / 2, 0, 0, 0,
      0, 0, 1.0 / 2, 1.0 / 2, 0, 0, 0,
      1.0 / 5, 1.0 / 5, 0, 0, 4.0 / 5, -1.0 / 5, 1.0 / 5,
      1.0 / 5, 1.0 / 5, 0, 0, -1.0 / 5, 4.0 / 5, 1.0 / 5,
      -1.0 / 5, -1.0 / 5, 0, 0, 1.0 / 5, 1.0 / 5, 4.0 / 5 });
   vmat.back().SetDescr(descr8_Prj);
   vmat.back().SetFtype('-');
   vmat.back().SetName("M8");


   //\[dc7_{ unsrt } \text{ boundary 8 transform } = \]
   //   \ [[r, s, u + t + s, t, w + v + u + t + s + r, w + s + r, 2 w + v + 2 min(u + 2 * s, -w - v, -w) + t + r]\]

   const std::string descr9_Prj("Case 9. $r=v, u ge w$:  The cells in this case must be $+++$.\n");
   //$\tau = -\sum_{ i = 1 } ^ 3(-dc7_{ unsrt,i }) + \sum_{ i = 4 } ^ 6(dc7_{ unsrt,i }) < dc7_{ unsrt,7 }$\\
         //$dc7_{ unsrt,1 } = dc7_{ unsrt,1 }\, +\, dc7_{ unsrt,3 }\, -\, dc7_{ unsrt,5 }$\\
         //equivalent to
         //$dc7_{ unsrt,3 }\, =\, dc7_{ unsrt,5 }$\\
         //$dc7_{ unsrt,3 }\, -\, dc7_{ unsrt,4 } \ge
         //dc7_{ unsrt,1 }\, -\, dc7_{ unsrt,6 }$.
         //\[dc7_{ unsrt } \text{ subspace 9 } = [r, s, t, s + t - u, t, r + s - w, s + t - u + w], \, 0 < w \leq u \]
         //\[dc7_{ unsrt } \text{ boundary 9 projector } = \]

   vmat.emplace_back(std::vector<double>{
      4.0 / 5, -1.0 / 5, 0, -1.0 / 5, 0, 1.0 / 5, 1.0 / 5,
      -1.0 / 5, 4.0 / 5, 0, -1.0 / 5, 0, 1.0 / 5, 1.0 / 5,
      0, 0, 1.0 / 2, 0, 1.0 / 2, 0, 0,
      -1.0 / 5, -1.0 / 5, 0, 4.0 / 5, 0, 1.0 / 5, 1.0 / 5,
      0, 0, 1.0 / 2, 0, 1.0 / 2, 0, 0,
      1.0 / 5, 1.0 / 5, 0, 1.0 / 5, 0, 4.0 / 5, -1.0 / 5,
      1.0 / 5, 1.0 / 5, 0, 1.0 / 5, 0, -1.0 / 5, 4.0 / 5 });
   vmat.back().SetDescr(descr9_Prj);
   vmat.back().SetFtype('+');
   vmat.back().SetName("M9");


   //\[dc7_{ unsrt } \text{ boundary 9 transform } = \]
   //   \ [[r, s, -v + t + r, w - v - u + t + s + r, t, -w + s + r, -u + t + s]\]

   const std::string descrA_Prj("//Case A. $r=v, u < w$:  The cells in this case must be $+++$.\n");
   //$\tau = -\sum_{ i = 1 } ^ 3(-dc7_{ unsrt,i }) + \sum_{ i = 4 } ^ 6(dc7_{ unsrt,i }) < dc7_{ unsrt,7 }$\\
         //$dc7_{ unsrt,1 } = dc7_{ unsrt,1 }\, +\, dc7_{ unsrt,3 }\, -\, dc7_{ unsrt,5 }$\\
         //equivalent to
         //$dc7_{ unsrt,3 }\, =\, dc7_{ unsrt,5 }$\\
         //$dc7_{ unsrt,3 }\, -\, dc7_{ unsrt,4 } <
         //dc7_{ unsrt,1 }\, -\, dc7_{ unsrt,6 }$.
         //\[dc7_{ unsrt } \text{ subspace A } = [r, s, t, s + t - u, t, r + s - w, s + t + u - w], \, 0 < u < w \]
         //\[dc7_{ unsrt } \text{ boundary A projector } = \]

   vmat.emplace_back(std::vector<double>{
      6.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      1.0 / 7, 6.0 / 7, -1.0 / 7, 1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7,
      1.0 / 7, -1.0 / 7, 5.0 / 14, 1.0 / 7, 5.0 / 14, -1.0 / 7, 1.0 / 7,
      -1.0 / 7, 1.0 / 7, 1.0 / 7, 6.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      1.0 / 7, -1.0 / 7, 5.0 / 14, 1.0 / 7, 5.0 / 14, -1.0 / 7, 1.0 / 7,
      1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7, -1.0 / 7, 6.0 / 7, 1.0 / 7,
      -1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, 6.0 / 7 });
   vmat.back().SetDescr(descrA_Prj);
   vmat.back().SetFtype('+');
   vmat.back().SetName("MA");


   //\[dc7_{ unsrt } \text{ boundary A transform } = \]
   //   \ [[r, s, -v + t + r, -w - v + u + t + s + r, t, -w + s + r, -2 w + 2 min(2r - v, w, w - u) + u + t + s]\]

   const std::string descrB_Prj("//Case B. $r=-v$:  The cells in this case must be $---$.\n");
   //$\tau = -\sum_{ i = 1 } ^ 3(-dc7_{ unsrt,i }) + \sum_{ i = 4 } ^ 6(dc7_{ unsrt,i }) = dc7_{ unsrt,7 }$\\
         //$dc7_{ unsrt,1 } = dc7_{ unsrt,1 }\, +\, dc7_{ unsrt,3 }\, -\, dc7_{ unsrt,5 }$\\
         //equivalent to
         //$dc7_{ unsrt,3 }\, =\, dc7_{ unsrt,5 }$.
         //\[dc7_{ unsrt } \text{ subspace B } = [r, s, t, s + t + u, t, r + s + w, s + t + u + w], \, u, w \leq 0 \]
         //\[dc7_{ unsrt } \text{ boundary B projector } = \]

   vmat.emplace_back(std::vector<double>{
      4.0 / 5, -1.0 / 5, 0, 1.0 / 5, 0, 1.0 / 5, -1.0 / 5,
      -1.0 / 5, 4.0 / 5, 0, 1.0 / 5, 0, 1.0 / 5, -1.0 / 5,
      0, 0, 1.0 / 2, 0, 1.0 / 2,0, 0,
      1.0 / 5, 1.0 / 5, 0, 4.0 / 5, 0, -1.0 / 5, 1.0 / 5,
      0, 0, 1.0 / 2, 0, 1.0 / 2, 0, 0,
      1.0 / 5, 1.0 / 5, 0, -1.0 / 5, 0, 4.0 / 5, 1.0 / 5,
      -1.0 / 5, -1.0 / 5, 0, 1.0 / 5, 0, 1.0 / 5, 4.0 / 5 });
   vmat.back().SetDescr(descrB_Prj);
   vmat.back().SetFtype('-');
   vmat.back().SetName("MB");


   //\[dc7_{ unsrt } \text{ boundary B transform } = \]
   //   \ [[r, s, v + t + r, w + v + u + t + s + r, t, w + s + r, 2 w + 2 min(v + 2 * r, -w - u, -w) + u + t + s]\]

   const std::string descrC_Prj("Case C. $r=w, u ge v$:  The cells in this case must be $+++$.\n");
   //$\tau = -\sum_{ i = 1 } ^ 3(-dc7_{ unsrt,i }) + \sum_{ i = 4 } ^ 6(dc7_{ unsrt,i }) < dc7_{ unsrt,7 }$\\
         //$dc7_{ unsrt,1 } = dc7_{ unsrt,1 }\, +\, dc7_{ unsrt,2 }\, -\, dc7_{ unsrt,6 }$\\
         //equivalent to
         //$dc7_{ unsrt,2 }\, =\, dc7_{ unsrt,6 }$\\
         //$dc7_{ unsrt,2 }\, -\, dc7_{ unsrt,4 } \ge
         //dc7_{ unsrt,1 }\, -\, dc7_{ unsrt,5 }$.
         //\[dc7_{ unsrt } \text{ subspace C } = [r, s, t, s + t - u, r + t - v, s, s + t - u + v], \, 0 < v \leq u \]
         //\[dc7_{ unsrt } \text{ boundary C projector } = \]

   vmat.emplace_back(std::vector<double>{
      4.0 / 5, 0, -1.0 / 5, -1.0 / 5, 1.0 / 5, 0, 1.0 / 5,
      0, 1.0 / 2, 0, 0, 0, 1.0 / 2, 0,
      -1.0 / 5, 0, 4.0 / 5, -1.0 / 5, 1.0 / 5, 0, 1.0 / 5,
      -1.0 / 5, 0, -1.0 / 5, 4.0 / 5, 1.0 / 5, 0, 1.0 / 5,
      1.0 / 5, 0, 1.0 / 5, 1.0 / 5, 4.0 / 5, 0, -1.0 / 5,
      0, 1.0 / 2, 0, 0, 0, 1.0 / 2, 0,
      1.0 / 5, 0, 1.0 / 5, 1.0 / 5, -1.0 / 5, 0, 4.0 / 5 });
   vmat.back().SetDescr(descrC_Prj);
   vmat.back().SetFtype('+');
   vmat.back().SetName("MC");


   //\[dc7_{ unsrt } \text{ boundary C transform } = \]
   //   \ [[r, -w + s + r, t, -w + v - u + t + s + r, -v + t + r, s, -u + t + s]\]

   const std::string descrD_Prj("//Case D. $r=w, u < v$:  The cells in this case must be $+++$.\n");
   //$\tau = -\sum_{ i = 1 } ^ 3(-dc7_{ unsrt,i }) + \sum_{ i = 4 } ^ 6(dc7_{ unsrt,i }) <
   //dc7_{ unsrt,7 }$\\
         //$dc7_{ unsrt,1 } = dc7_{ unsrt,1 }\, +\, dc7_{ unsrt,2 }\, -\, dc7_{ unsrt,6 }$\\
         //equivalent to
         //$dc7_{ unsrt,2 }\, =\, dc7_{ unsrt,6 }$\\
         //$dc7_{ unsrt,2 }\, -\, dc7_{ unsrt,4 } <
         //dc7_{ unsrt,1 }\, -\, dc7_{ unsrt,5 }$.
         //\[dc7_{ unsrt }\text{ subspace D } = [r, s, t, s + t - u, r + t - v, s, s + t + u - v], \, 0 < u < v \]
         //\[dc7_{ unsrt } \text{ boundary D projector } = \]

   vmat.emplace_back(std::vector<double>{
      6.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      1.0 / 7, 5.0 / 14, -1.0 / 7, 1.0 / 7, -1.0 / 7, 5.0 / 14, 1.0 / 7,
      1.0 / 7, -1.0 / 7, 6.0 / 7, 1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7,
      -1.0 / 7, 1.0 / 7, 1.0 / 7, 6.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      1.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7, 6.0 / 7, -1.0 / 7, 1.0 / 7,
      1.0 / 7, 5.0 / 14, -1.0 / 7, 1.0 / 7, -1.0 / 7, 5.0 / 14, 1.0 / 7,
      -1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, 6.0 / 7 });
   vmat.back().SetDescr(descrD_Prj);
   vmat.back().SetFtype('+');
   vmat.back().SetName("MD");



   //\[dc7_{ unsrt } \text{ boundary D transform } = \]
   //   \ [[r, -w + s + r, t, -w - v + u + t + s + r, -v + t + r, s, 2min(v, v - u, 2r - w) - 2v + u + t + s]\]

   const std::string descrE_Prj("//Case E. $r=-w$:  The cells in this case must be $---$.\n");
   //$\tau = -\sum_{ i = 1 } ^ 3(-dc7_{ unsrt,i }) + \sum_{ i = 4 } ^ 6(dc7_{ unsrt,i }) = dc7_{ unsrt,7 }$\\
         //$dc7_{ unsrt,1 } = dc7_{ unsrt,1 }\, +\, dc7_{ unsrt,2 }\, -\, dc7_{ unsrt,6 }$\\
         //equivalent to
         //$dc7_{ unsrt,2 }\, =\, dc7_{ unsrt,6 }$.
         //\[dc7_{ unsrt } \text{ subspace E } = [r, s, t, s + t + u, r + t + v, s, s + t + u + v], u, v \leq 0\]
         //\[dc7_{ unsrt } \text{ boundary E projector } = \]

   vmat.emplace_back(std::vector<double>{
      4.0 / 5, 0, -1.0 / 5, 1.0 / 5, 1.0 / 5, 0, -1.0 / 5,
      0, 1.0 / 2, 0, 0, 0, 1.0 / 2, 0,
      -1.0 / 5, 0, 4.0 / 5, 1.0 / 5, 1.0 / 5, 0, -1.0 / 5,
      1.0 / 5, 0, 1.0 / 5, 4.0 / 5, -1.0 / 5, 0, 1.0 / 5,
      1.0 / 5, 0, 1.0 / 5, -1.0 / 5, 4.0 / 5, 0, 1.0 / 5,
      0, 1.0 / 2, 0, 0, 0, 1.0 / 2, 0,
      -1.0 / 5, 0, -1.0 / 5, 1.0 / 5, 1.0 / 5, 0, 4.0 / 5 });
   vmat.back().SetDescr(descrE_Prj);
   vmat.back().SetFtype('-');
   vmat.back().SetName("ME");


   //\[dc7_{ unsrt } \text{ boundary E transform } = \]
   //   \ [[r, w + s + r, t, w + v + u + t + s + r, v + t + r, s, 2v + 2min((-v) - u, -v, w + 2 * r) + u + t + s]\]




   const std::string descrF_Prj("//case F. $t = r+s+t+u+v+w$.  The cells in this case must be $---$.\n");
   //\[\tau = -\sum_{ i = 1 } ^ 3(-dc7_{ unsrt,i }) + \sum_{ i = 4 } ^ 6(dc7_{ unsrt,i })\]
   //\[=dc7_{ unsrt,7 } = dc7_{ unsrt,3 }\]
   //\[dc7_{ unsrt } \text{ subspace F } = [r, s, t, s + t + u, r + t + v, -u - v, t], \, u, v \leq 0 \]
   //\[dc7_{ unsrt } \text{ boundary F projector } = \]

   vmat.emplace_back(std::vector<double>{
      6.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      -1.0 / 7, 6.0 / 7, -1.0 / 7, 1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7,
      -1.0 / 7, -1.0 / 7, 5.0 / 14, 1.0 / 7, 1.0 / 7, 1.0 / 7, 5.0 / 14,
      1.0 / 7, 1.0 / 7, 1.0 / 7, 6.0 / 7, -1.0 / 7, -1.0 / 7, 1.0 / 7,
      1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, 6.0 / 7, -1.0 / 7, 1.0 / 7,
      1.0 / 7, 1.0 / 7, 1.0 / 7, -1.0 / 7, -1.0 / 7, 6.0 / 7, 1.0 / 7,
      -1.0 / 7, -1.0 / 7, 5.0 / 14, 1.0 / 7, 1.0 / 7, 1.0 / 7, 5.0 / 14 });
   vmat.back().SetDescr(descrF_Prj);
   vmat.back().SetFtype('-');
   vmat.back().SetName("MF");


   //\[dc7_{ unsrt } \text{ boundary F transform } = \]
   //   \ [[r, s, w + v + u + t + s + r, v + t + r, u + t + s, w + s + r, t]\]

   return vmat;

}