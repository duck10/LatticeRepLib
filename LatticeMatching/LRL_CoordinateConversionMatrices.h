#ifndef COORDINATECONVERSIONMATRICES_H
#define COORDINATECONVERSIONMATRICES_H

#include "MatMN.h"
/*
From HJB
G6toD7:matrix([1,0,0,0,0,0],[0,1,0,0,0,0],[0,0,1,0,0,0],[1,1,1,1,1,1],[0,1,1,1,0,0],[1,0,1,0,1,0],[1,1,0,0,0,1]);
D7toG6:matrix([1,0,0,0,0,0,0],[0,1,0,0,0,0,0],[0,0,1,0,0,0,0],[0,-1,-1,0,1,0,0],[-1,0,-1,0,0,1,0],[-1,-1,0,0,0,0,1]);
G6toD7:matrix("1 0 0 0 0 0   0 1 0 0 0 0   0 0 1 0 0 0   1 1 1 1 1 1   0 1 1 1 0 0   1 0 1 0 1 0   1 1 0 0 0 1"    );
D7toG6:matrix("1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 1 0 0 0 0   0 -1 -1 0 1 0 0   -1 0 -1 0 0 1 0   -1 -1 0 0 0 0 1");

G6to7:matrix(
[1,0,0,0,0,0],
[0,1,0,0,0,0],
[0,0,1,0,0,0],
[1,1,1,1,1,1],
[0,1,1,1,0,0],
[1,0,1,0,1,0],
[1,1,0,0,0,1]);
G7to6:matrix(
[1,0,0,0,0,0,0],
[0,1,0,0,0,0,0],
[0,0,1,0,0,0,0],
[0,-1,-1,0,1,0,0],
[-1,0,-1,0,0,1,0],
[-1,-1,0,0,0,0,1]);
*/

class LRL_CoordinateConversionMatrices {

public:
   LRL_CoordinateConversionMatrices();

   public:
   static MatMN S6_FROM_G6;
   static MatMN G6_FROM_S6;
   static MatMN G6_FROM_D7;
   static MatMN D7_FROM_G6;
   static MatMN S6_FROM_D7;
   static MatMN D7_FROM_S6;

};
#endif  //  COORDINATECONVERSIONMATRICES_H