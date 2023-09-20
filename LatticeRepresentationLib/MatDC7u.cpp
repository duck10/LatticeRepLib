#include "LRL_CoordinateConversionMatrices.h"
#include "MatDC7u.h"
#include "MatMN.h"
#include "MatN.h"

#include <iomanip>
#include <string>
#include <vector>

#pragma warning( disable : 4702) //  unreachable code
#pragma warning( disable : 4100) //  unreferenced formal parameter

//std::vector<MatDC7u> MatDC7u::vDU_Refl = MatDC7u::GetReflections();

std::vector<MatDC7u> MatDC7u::MakeBoundaryMatrices() {
   std::vector<MatDC7u> vmat;

   //M1_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to --- or +++ to +++ at g6[g6_A2]=g6[G6_B2]
   // matrix 0,1,0,0,0,0,  1,0,0,0,0,0,  0,0,1,0,0,0,  0,0,0,0,1,0,  0,0,0,1,0,0,  0,0,0,0,0,1)
   //M1_g6.[r,r,t,u,v,w]:
   // matrix [r],[r],[t],[v],[u],[w])
   vmat.emplace_back(MatDC7u(std::vector<double>({
      0, 1, 0, 0, 0, 0, 0,
      1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      -1, -1, -1, 1, 1, 1, 0 })));
   const std::string descr1("//M1_g6  { boundary transform from --- to --- or +++ to +++ at g6[g6_A2]=g6[G6_B2]\n   "
      "//matrix ([ 0,1,0,0,0,0, 1,0,0,0,0,0, 0,0,1,0,0,0, 0,0,0,0,1,0, 0,0,0,1,0,0, 0,0,0,0,0,1])\n  //M1_g6.[r,r,t,u,v,w]:\n  // matrix [r],[r],[t],[v],[u],[w])");
   vmat.back().SetDescr(descr1);
   vmat.emplace_back(MatDC7u(std::vector<double>({ 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, -1, 1, 1, -1, 1, 0 })));
   vmat.back().SetDescr(descr1);
   vmat.emplace_back(MatDC7u(std::vector<double>({ 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, -1, 1, 1, -1, 1, 1, 0 })));
   vmat.back().SetDescr(descr1);
   vmat.emplace_back(MatDC7u(std::vector<double>({ 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, -1, 1, 1, -1, 0 })));
   vmat.back().SetDescr(descr1);


   ////M2_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to --- or +++ to +++ at g6[g6_B2]=g6[g6_C2]"
   //// matrix 1,0,0,0,0,0,  0,0,1,0,0,0,  0,1,0,0,0,0,  0,0,0,1,0,0,  0,0,0,0,0,1,  0,0,0,0,1,0)
   ////M2_g6.[r,s,s,u,v,w]:
      //vmat.emplace_back(MatDC7u(std::vector<double>({ ([r],[s],[s],[u],[w],[v])
   const std::string descr2("//M2_g6 matrix boundary transform from --- to --- or +++ to +++ at g6[g6_B2]=g6[g6_C2\n  "
      "//matrix([1,0,0,0,0,0, 0,0,1,0,0,0, 0,1,0,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0,1, 0,0,0,0,1,0])\n  "
      "//M2_g6.[r,s,s,u,v,w]:\n  // ([r],[s],[s],[u],[w],[v])");
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, -1, -1, -1, 1, 1, 1, 0 })));
   vmat.back().SetDescr(descr2);
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, -1, 1, 1, -1, 1, 1, 0 })));
   vmat.back().SetDescr(descr2);
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, -1, 1, 1, -1, 0 })));
   vmat.back().SetDescr(descr2);
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, -1, 1, 1, -1, 1, 0 })));
   vmat.back().SetDescr(descr2);

   const std::string descr3("//M3_g6 matrix boundary transform from --- to +++ for g6[g6_2BC]=0\n  "
      "//matrix([1,0,0,0,0,0,  0,1,0,0,0,0,  0,0,1,0,0,0,  0,0,1.0E-15,1,0,0,  0,0,0,0,-1,0,  0,0,0,0,0,-1])\n  "
      "//M3_g6.[r,s,t,0,v,w]:\n  // ([r],[s],[t],[1.0E-15*t],[-v],[-w])\n  "
      "//M3_dc7upmm");
      ////M3_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to +++ for g6[g6_2BC]=0
   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  0,0,1,0,0,0,  0,0,1.0E-15,1,0,0,  0,0,0,0,-1,0,  0,0,0,0,0,-1)
   ////M3_g6.[r,s,t,0,v,w]:
   //// matrix [r],[s],[t],[1.0E-15*t],[-v],[-w])
   ////M3_dc7upmm
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 1.999999999999999, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, -1.0, -0.999999999999999, 1.0, 1.0, 1.0, 0.0 })));
   vmat.back().SetDescr(descr3);
      
   const std::string descr4("//M4_g6 matrix boundary transform from --- to +++ for g6[g6_2AC]=0\n  "
      "//matrix([1,0,0,0,0,0,  0,1,0,0,0,0,  0,0,1,0,0,0,  0,0,0,-1,0,0,  0,0,1.0E-15,0,1,0,  0,0,0,0,0,-1])\n  "
      "//M4_g6.[r,s,t,u,0,w]:\n  // ([r],[s],[t],[-u],[1.0E-15*t],[-w])\n  "
      "//M4_dc7umpm");

   ////M4_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to +++ for g6[g6_2AC]=0
   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  0,0,1,0,0,0,  0,0,0,-1,0,0,  0,0,1.0E-15,0,1,0,  0,0,0,0,0,-1)
   ////M4_g6.[r,s,t,u,0,w]:
   //// matrix [r],[s],[t],[-u],[1.0E-15*t],[-w])
   ////M4_dc7umpm
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 2.0, 0.0, 1.999999999999999, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, -1.0, -0.999999999999999, 1.0, 1.0, 1.0, 0.0 })));
   vmat.back().SetDescr(descr4);
   ////M5_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to +++ for g6[g6_2AB]=0
   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  0,0,1,0,0,0,  0,0,0,-1,0,0,  0,0,0,0,-1,0,  0,0,1.0E-15,0,0,1)
   ////M5_g6.[r,s,t,u,v,0]:
   //// matrix [r],[s],[t],[-u],[-v],[1.0E-15*t])
   ////M5_dc7ummp


   const std::string descr5("//M5_g6 matrix boundary transform from --- to +++ for g6[g6_2AB]=0\n  "
      "//matrix([1,0,0,0,0,0,  0,1,0,0,0,0,  0,0,1,0,0,0,  0,0,0,-1,0,0,  0,0,0,0,-1,0,  0,0,1.0E-15,0,0,1])\n  "
      "//M5_g6.[r,s,t,u,v,0]:\n  "
      "// ([r],[s],[t],[-u],[-v],[1.0E-15*t])\n  "
      "//M5_dc7ummp");
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 2.0, 2.0, -1.0E-15, 0.0, 0.0, -1.0, 0.0, -1.0, -1.0, -0.999999999999999, 1.0, 1.0, 1.0, 0.0 })));
   vmat.back().SetDescr(descr5);

   ////M6_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from +++ to --- for g6[g6_B2]=g6[g6_2BC], g6[g6_2AC]>=g6[g6_2AB]
   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  0,1,1,-1,0,0,  0,-2,0,1,0,0,  0,0,0,0,-1,1,  0,0,0,0,0,-1)
   ////M6_g6.[r,s,t,s,v,w]:
   //// matrix [r],[s],[t],[-s],[w-v],[-w])
   ////M6_dc7ummm
   const std::string descr6("//M6_g6 matrix boundary transform from +++ to --- for g6[g6_B2]=g6[g6_2BC], g6[g6_2AC]>=g6[g6_2AB]\n  "
      "//matrix([1,0,0,0,0,0,  0,1,0,0,0,0,  0,1,1,-1,0,0,  0,-2,0,1,0,0,  0,0,0,0,-1,1,  0,0,0,0,0,-1])\n  "
      "//M6_g6.[r,s,t,s,v,w]:\n  "
      "// ([r],[s],[t],[-s],[w-v],[-w])\n  "
      "//M6_dc7ummm");
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, -1, 1, 1, -1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0 })));
   vmat.back().SetDescr(descr6);

   ////M7_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from +++ to --- for g6[g6_B2]=g6[g6_2BC], g6[g6_2AC]<g6[g6_2AB]
   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  0,1,1,-1,0,0,  0,2,0,-1,0,0,  0,0,0,0,-1,1,  0,0,0,0,0,1)
   //// matrix [r],[s],[t],[s],[w-v],[w])
   ////M7_dc7umpm

   const std::string descr7("//M7_g6 matrix boundary transform from +++ to --- for g6[g6_B2]=g6[g6_2BC], g6[g6_2AC]<g6[g6_2AB]\n  "
      "//note that g6[g6_2AC] <= g2[g6_A2] <= g2[g6_B2] = g6[g6_2BC], so g6[g6_2AC]=v is minimal among\n  "
      "//u, v, w and there is only one dc7u +++ MBD case to consider\n  "

      "// ([1,0,0,0,0,0],[0,1,0,0,0,0],[0,1,1,-1,0,0],[0,2,0,-1,0,0], [0,0,0,0,-1,1],[0,0,0,0,0,1])\n  "
      "// ([r],[s],[t],[s],[w-v],[w])\n  "
      "//M7_dc7umpm");

      vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, -1, 1, 1, -1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0 })));
      vmat.back().SetDescr(descr7);
      
      
   ////M8_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to +++ for  g6[g6_B2]=-g6[g6_2BC]
   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  0,1,1,1,0,0,  0,2,0,1,0,0,  0,0,0,0,-1,-1,  0,0,0,0,0,-1)
   ////M8_g6.[r,s,t,-s,v,w]:
   //// matrix [r],[s],[t],[s],[(-w)-v],[-w])
   ////M8_dc7ummp:
   const std::string descr8("//M8_g6 matrix boundary transform from --- to +++ for  g6[g6_B2]=-g6[g6_2BC]\n  "
         "// ([1,0,0,0,0,0, 0,1,0,0,0,0, 0,1,1,1,0,0, 0,2,0,1,0,0, 0,0,0,0,-1,-1, 0,0,0,0,0,-1])\n  "
         "// M8_g6.[r,s,t,-s,v,w]:\n  "
         "// ([r],[s],[t],[s],[(-w)-v],[-w])\n  "
         "// M8_dc7ummp");
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, -1, -1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 2, 0, -1, 0, 0 })));
   vmat.back().SetDescr(descr8);

   const std::string descr9("//M9_g6 matrix boundary transform from +++ to --- for  g6[g6_A2]=g6[g6_2AC], g6[g6_2BC] >= g6[g6_AB]\n  "
      "// ([1,0,0,0,0,0,  0,1,0,0,0,0,  1,0,1,0,1,0,  0,0,0,-1,0,1,  -2,0,0,0,1,0,  0,0,0,0,0,-1])\n  "
      "//M9_g6.[r,s,t,u,r,w]:\n  "
      "// ([r],[s],[t+2*r],[w-u],[-r],[-w])\n  "
      "//M9_dc7ummp:\n  "
      "//mat_g6ppp2dc7ummo");
   //vmat.emplace_back(MatDC7u(std::vector<double>({ 1,0,0,0,0,0,0, 0,1,0,0,0,0,0, 0,0,0,0,1,0,0, -1,0,1,-1,0,1,0, -3,0,-1,0,1,0,0, 1,1,0,0,0,-1,0 })));
   //vmat.back().SetDescr(descr9);

   ////M9_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from +++ to --- for  g6[g6_A2]=g6[g6_2AC], g6[g6_2BC] >= g6[g6_AB]
   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  1,0,1,0,1,0,  0,0,0,-1,0,1,  -2,0,0,0,1,0,  0,0,0,0,0,-1)
   ////M9_g6.[r,s,t,u,r,w]:
   //// matrix [r],[s],[t+2*r],[w-u],[-r],[-w])
   ////M9_dc7ummp:
   ////mat_g6ppp2dc7ummo .    vmat.emplace_back(MatDC7u(std::vector<double>({ [1,0,0,0,0,0,0],[0,1,0,0,0,0,0],[0,0,0,0,1,0,0],[-1,0,1,-1,0,1,0],[-3,0,-1,0,1,0,0],[1,1,0,0,0,-1,0])


   ////MA_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from +++ to +++ for  g6[g6_A2]=g6[g6_2AC], g6[g6_2BC] < g6[g6_AB] 
   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  1,0,1,0,-1,0,  0,0,0,-1,0,1,  2,0,0,0,-1,0,  0,0,0,0,0,1)
   ////MA_g6.[r,s,t,u,r,w]:
   //// matrix [r],[s],[t],[w-u],[r],[w])
   ////MA_dc7upmm:
   const std::string descrA("//MA_g6 matrix boundary transform from +++ to +++ for  g6[g6_A2]=g6[g6_2AC], g6[g6_2BC] < g6[g6_AB]\n  "
      "// ([1,0,0,0,0,0],[0,1,0,0,0,0],[1,0,1,0,-1,0],[0,0,0,-1,0,1],[2,0,0,0,-1,0],[0,0,0,0,0,1])\n  "
      "// MA_g6.[r,s,t,u,r,w]:\n  "
      "// ([r],[s],[t],[w-u],[r],[w])\n  "
      "// MA_dc7upmm:");
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -1, 1, 1, -1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 })));
   vmat.back().SetDescr(descrA);

   ////MB_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to +++ for  g6[g6_A2]=-g6[g6_2AC]
   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,0,  1,0,1,0,1,0,  0,0,0,-1,0,-1,  2,0,0,0,1,0,  0,0,0,0,0,-1)
   ////MB_g6.[r,s,t,u,-r,w]:
   //// matrix [r],[s],[t],[(-w)-u],[r],[-w])
   ////MB_dc7ummp:
   const std::string descrB("//MB_g6 matrix boundary transform from --- to +++ for  g6[g6_A2]=-g6[g6_2AC]\n  "
      "// ([1,0,0,0,0,0,  0,1,0,0,0,0, 1,0,1,0,1,0, 0,0,0,-1,0,-1, 2,0,0,0,1,0, 0,0,0,0,0,-1)\n  "
      "//MB_g6.[r,s,t,u,-r,w]:\n  "
      "//matrix([r],[s],[t],[(-w)-u],[r],[-w])\n  "
      "//MB_dc7ummp");
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -1, -1, -1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -2, -2, 1, 2, 2, 0 })));
   vmat.back().SetDescr(descrB);


   ////MC_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from +++ to --- for  g6[g6_A2]=g6[g6_2AB], g6[g6_2BC] >= g6[g6_2AC]"
   //// matrix 1,0,0,0,0,0,  1,1,0,0,0,-1,  0,0,1,0,0,0,  0,0,0,-1,1,0,  0,0,0,0,-1,0,  -2,0,0,0,0,1)


   ////MC_g6.[r,s,t,u,v,r]:
   //// matrix [r],[s],[t],[v-u],[-v],[-r])
   ////MC_dc7upmm:
   const std::string descrC("//MC_g6 matrix boundary transform from +++ to --- for  g6[g6_A2]=g6[g6_2AB], g6[g6_2BC] >= g6[g6_2AC]\n  "
      "// ([1,0,0,0,0,0,  1,1,0,0,0,-1,  0,0,1,0,0,0,  0,0,0,-1,1,0,  0,0,0,0,-1,0,  -2,0,0,0,0,1])\n  "
      "// MC_g6.[r,s,t,u,v,r]:\n  "
      "// ([r],[s],[t],[v-u],[-v],[-r])\n  "
      "// MC_dc7upmm");
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, -1, 1, 1, -1, 1, 1, 0, 2, 0, 2, 0, -1, 0, 0, 2, -1, 0, 0, 0, 2, 0, 4, -2, 2, 1, -2, 2, 0 })));
   vmat.back().SetDescr(descrC);

   ////MD_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from +++ to +++ for  g6[g6_A2]=g6[g6_2AB], g6[g6_2BC] < g6[g6_2AC]
   //// matrix 1,0,0,0,0,0,  1,1,0,0,0,-1,  0,0,1,0,0,0,  0,0,0,-1,1,0,  0,0,0,0,1,0,  2,0,0,0,0,-1)
   ////MD_g6.[r,s,t,u,v,r]:
   //// matrix [r],[s],[t],[v-u],[v],[r])
   // 
   ////MD_dc7upmm:
   const std::string descrD("//MD_g6 matrix boundary transform from +++ to +++ for  g6[g6_A2]=g6[g6_2AB], g6[g6_2BC] < g6[g6_2AC]\n  "
      "// ([1,0,0,0,0,0,  1,1,0,0,0,-1,  0,0,1,0,0,0,  0,0,0,-1,1,0,  0,0,0,0,1,0,  2,0,0,0,0,-1)\n  "
      "// MD_g6.[r,s,t,u,v,r]:\n  "
      "// ([r],[s],[t],[v-u],[v],[r])\n  "
      "// MD_dc7upmm");
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0, 0, 3, 1, 1, 1, -1, -1, 0, 2, 0, 2, 0, -1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 2, -1, 0, 0, 0 })));
   vmat.back().SetDescr(descrD);
   ////ME_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to +++ for  g6[g6_A2]=-g6[g6_2AB]
   //// matrix 1,0,0,0,0,0,  1,1,0,0,0,1,  0,0,1,0,0,0,  0,0,0,-1,-1,0,  0,0,0,0,-1,0,  2,0,0,0,0,1)
   ////ME_g6.[r,s,t,u,v,-r]:
   //// matrix [r],[s],[t],[(-v)-u],[-v],[r])
   ////ME_dc7umpm:
   const std::string descrE("//ME_g6 matrix boundary transform from --- to +++ for  g6[g6_A2]=-g6[g6_2AB]\n  "
      "// ([1,0,0,0,0,0,  1,1,0,0,0,1,  0,0,1,0,0,0,  0,0,0,-1,-1,0,  0,0,0,0,-1,0,  2,0,0,0,0,1)\n  "
      "//ME_g6.[r,s,t,u,v,-r]:\n  "
      "//([r],[s],[t],[(-v)-u],[-v],[r])\n  "
      "//ME_dc7umpm");
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, -1, -1, -1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 })));
   vmat.back().SetDescr(descrE);

   ////MF_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to --- for  g6[g6_A2]+g6[g6_B2]+g6[g6_C2]+g6[g6_2BC]+g6[g6_2AC]+g6[g6_2AB]=g6[g6_C2]
   //// matrix 1,0,0,0,0,0,  0,1,0,0,0,1,  1,1,1,1,1,1,  0,-2,0,-1,0,-1,  -2,0,0,0,-1,-1,  0,0,0,0,0,1)
   ////MF_g6.[r,s,t,u,v,-r-s-u-v]:
   //// matrix [r],[(-v)-u-r],[t],[v-s+r],[u+s-r],[(-v)-u-s-r])
   ////"MF_dc7ummm:"
   ////MF_g6    vmat.emplace_back(MatDC7u(std::vector<double>({ boundary transform from --- to --- for  
   const std::string descrF("//MF_g6 matrix boundary transform from --- to --- for  g6[g6_A2]+g6[g6_B2]+g6[g6_C2]+g6[g6_2BC]+g6[g6_2AC]+g6[g6_2AB]=g6[g6_C2]\n  "
      "// ([1,0,0,0,0,0,  0,1,0,0,0,1,  1,1,1,1,1,1,  0,-2,0,-1,0,-1,  -2,0,0,0,-1,-1,  0,0,0,0,0,1)\n  "
      "// MF_g6.[r,s,t,u,v,-r-s-u-v]:\n  "
      "// matrix([r],[(-v)-u-r],[t],[v-s+r],[u+s-r],[(-v)-u-s-r])\n  "
      "// MF_dc7ummm:");
   vmat.emplace_back(MatDC7u(std::vector<double>({ 1, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, -1, -1, -1, 1, 1, 1, 0, -1, -1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, -1, -1, 0, 0, 0, 2, 0, -1, -1, 1, 0, 0, 1, 0 })));
   vmat.back().SetDescr(descrF);


   return vmat;
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
   return m*d;
}

MatDC7u operator/(const double d, const MatDC7u& m) {
   return m / d;
}

DC7u MatDC7u::operator* (const DC7u& v) const {
   DC7u g7(v);
   g7.SetVector( (m_mat * v.GetVector()).GetVector());
   g7.SetValid(g7[0]>0.001 && g7[1]>0.001 && g7[2]>0.001);
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
   for (size_t i = 0; i < 7*7; ++i) sum += m_mat[i] * m_mat[i];
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
   //  transpose a symmetrical matrix
   const int size = 7;
   MatDC7u m;
   for (int count = 0; count<size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/7) +7*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
   return m;
}

void MatDC7u::transpose(void) {
   //  transpose a symmetrical matrix
   const int size = 7;
   MatDC7u& m(*this);
   MatDC7u m2(*this);
   for (int count = 0; count<size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/7) +7*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
}

//std::ostream& operator<< (std::ostream& o, const MatDC7u& dc) {
//   std::streamsize oldPrecision = o.precision();
//   o << std::fixed << std::setprecision(5);
//   for (size_t i = 0; i < dc.size(); ++i) {
//      if (i % 7 == 0) o << std::endl;
//      o << dc[i] << " ";
//      //o << std::setw(9) << dc[i] << " ";
//   }
//   o << std::setprecision(oldPrecision);
//   o.unsetf(std::ios::floatfield);
//   return o;
//}

