#include "stdafx.h"

#include <vector>

#include "D7DMS.h"



void D7DMS1(const double d[], double dd[]) {
   /* D7M_[1] 2*d[2] <d> 2*d[4] d5 <d> s6*/
   dd[0] = d[1];
   dd[1] = d[0];
   dd[2] = d[2];
   dd[3] = d[3];
   dd[4] = d[5];
   dd[5] = d[4];
   dd[6] = d[6];
}

void D7DMS2(const double d[], double dd[]) {
   /* D7M_2*d[1] 2*d[2] <d> d3 s6 <d> d7*/
   dd[0] = d[0];
   dd[1] = d[2];
   dd[2] = d[1];
   dd[3] = d[3];
   dd[4] = d[4];
   dd[5] = d[6];
   dd[6] = d[5];
}

void D7DMS3(const double d[], double dd[]) {
   /* D7M_3 d3 <d> b4 d5 <d> s6*/
   dd[0] = d[0];
   dd[1] = d[1];
   dd[2] = d[3];
   dd[3] = d[2];
   dd[4] = d[5];
   dd[5] = d[4];
   dd[6] = d[6];
}

void D7DMS4(const double d[], double dd[]) {
   /* D7M_4 2*d[2]d>2*d[2] 2*d[3]d>d3 d3d>s6 b4d>d7 d5d>d[6] s6d>b4 d7d>2d2+2d3dd5*/
   dd[0] = d[1];
   dd[1] = d[2];
   dd[2] = d[5];
   dd[3] = d[6];
   dd[4] = d[0];
   dd[5] = d[3];
   dd[6] = 2 * d[1] + 2 * d[2] - d[4];
}

void D7DMS5(const double d[], double dd[]) {
   /* D7M_5 2*d[2]d>b4 d3d>d5 b4d>d[4] d5d>d2 s6d>d3 d7d>d2d2d2d3+d5+2s6+2d7*/
   dd[0] = d[0];
   dd[1] = d[3];
   dd[2] = d[5];
   dd[3] = d[6];
   dd[4] = d[1];
   dd[5] = d[2];
   dd[6] = 2 * d[0] + 2 * d[3] - d[4];

}

void D7DMS6(const double d[], double dd[]) {
   /* D7M_6 2*d[2]d>d3 d3d>d5 b4d>d7 d5d>d2 s6d>b4 s6d>2d1+2d3ds6*/
   dd[0] = d[0];
   dd[1] = d[2];
   dd[2] = d[4];
   dd[3] = d[6];
   dd[4] = d[1];
   dd[5] = d[3];
   dd[6] = 2 * d[0] + 2 * d[2] - d[5];
}

void D7DMS7(const double d[], double dd[]) {
   /* D7M_7 2*d[2]d>d3 d3d>d5 b4d>d[4] d5d>d2 s6d>b4 s6d2d1+2d3ds6*/
   dd[0] = d[0];
   dd[1] = d[2];
   dd[2] = d[4];
   dd[3] = d[6];
   dd[4] = d[1];
   dd[5] = d[3];
   dd[6] = 2 * d[0] + 2 * d[2] - d[5];

}

void D7DMS8(const double d[], double dd[]) {
   /* D7M_8 d3d>d5 b4d>s6 d5d>d3 s6d>b4 d7d>2d[6]+2d2dd7*/
   dd[0] = d[0];
   dd[1] = d[1];
   dd[2] = d[4];
   dd[3] = d[5];
   dd[4] = d[2];
   dd[5] = d[3];
   dd[6] = 2 * d[0] + 2 * d[1] - d[6];
}

void D7DMS9(const double d[], double dd[]) {
   /* D7M_9 2*d[2]d>d3 2*d[3]d>s6 d3d>d5 d5d>d2d[5]d2d2+2d5+2s6+d7 s6d>d2 d7d>d1 */
   dd[0] = d[2];
   dd[1] = d[5];
   dd[2] = d[4];
   dd[3] = d[3];
   dd[4] = 2 * d[2] + 2 * d[3] - d[6];
   dd[5] = d[1];
   dd[6] = d[0];
}

std::vector< void(*)(const double[], double[])> GetDMSFunctions() {
   std::vector< void(*)(const double[], double[])> v;
   v.push_back(D7DMS1);
   v.push_back(D7DMS2);
   v.push_back(D7DMS3);
   v.push_back(D7DMS4);
   v.push_back(D7DMS5);
   v.push_back(D7DMS6);
   v.push_back(D7DMS7);
   v.push_back(D7DMS8);
   v.push_back(D7DMS9);
   return v;
}
