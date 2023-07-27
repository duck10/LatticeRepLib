#include "LRL_CoordinateConversionMatrices.h"

MatMN LRL_CoordinateConversionMatrices::S6_FROM_G6 = MatMN(6, 6, "0 0 0 0.5 0 0   0 0 0 0 0.5 0   0 0 0 0 0 0.5   -1 0 0 0 -0.5 -0.5   0 -1 0 -0.5 0 -0.5   0 0 -1 -0.5 -0.5 0");
MatMN LRL_CoordinateConversionMatrices::G6_FROM_S6 = MatMN(6, 6, "0 -1 -1 -1 -0 -0 -1 0 -1 -0 -1 -0 -1 -1 0 0 0 -1 2 0 -0 0 0 0 -0 2 -0 0 0 0 0 -0 2 0 0 0");
MatMN LRL_CoordinateConversionMatrices::G6_FROM_D7 = MatMN(6, 7, "1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 1 0 0 0 0   0 -1 -1 0 1 0 0   -1 0 -1 0 0 1 0   -1 -1 0 0 0 0 1");
MatMN LRL_CoordinateConversionMatrices::D7_FROM_G6 = MatMN(7, 6, "1 0 0 0 0 0   0 1 0 0 0 0   0 0 1 0 0 0   1 1 1 1 1 1   0 1 1 1 0 0   1 0 1 0 1 0   1 1 0 0 0 1");
MatMN LRL_CoordinateConversionMatrices::S6_FROM_D7 = MatMN(6, 7, "0 -0.5 -0.5 0 0.5 0 0 -0.5 0 -0.5 0 0 0.5 0 -0.5 -0.5 0 0 0 0 0.5 0 0.5 0.5 0 0 -0.5 -0.5 0.5 0 0.5 0 -0.5 0 -0.5 0.5 0.5 0 0 -0.5 -0.5 0");
MatMN LRL_CoordinateConversionMatrices::D7_FROM_S6 = MatMN(7, 6, "0 -1 -1 -1 0 0    -1 0 -1 0 -1 0    -1 -1 0 0 0 -1    0 0 0 -1 -1 -1    0 -1 -1 0 -1 -1    -1 0 -1 -1 0 -1    -1 -1 0 -1 -1 0");

LRL_CoordinateConversionMatrices::LRL_CoordinateConversionMatrices() {
   S6_FROM_G6 = MatMN(6, 6, "0 0 0 0.5 0 0   0 0 0 0 0.5 0   0 0 0 0 0 0.5   -1 0 0 0 -0.5 -0.5   0 -1 0 -0.5 0 -0.5   0 0 -1 -0.5 -0.5 0");
   G6_FROM_S6 = MatMN(6, 6, "0 -1 -1 -1 -0 -0 -1 0 -1 -0 -1 -0 -1 -1 0 0 0 -1 2 0 -0 0 0 0 -0 2 -0 0 0 0 0 -0 2 0 0 0");
   G6_FROM_D7 = MatMN(6, 7, "1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 1 0 0 0 0   0 -1 -1 0 1 0 0   -1 0 -1 0 0 1 0   -1 -1 0 0 0 0 1");
   D7_FROM_G6 = MatMN(7, 6, "1 0 0 0 0 0   0 1 0 0 0 0   0 0 1 0 0 0   1 1 1 1 1 1   0 1 1 1 0 0   1 0 1 0 1 0   1 1 0 0 0 1");
   S6_FROM_D7 = MatMN(6, 7, "0 -0.5 -0.5 0 0.5 0 0 -0.5 0 -0.5 0 0 0.5 0 -0.5 -0.5 0 0 0 0 0.5 0 0.5 0.5 0 0 -0.5 -0.5 0.5 0 0.5 0 -0.5 0 -0.5 0.5 0.5 0 0 -0.5 -0.5 0");
   D7_FROM_S6 = MatMN(7, 6, "0 -1 -1 -1 0 0    -1 0 -1 0 -1 0    -1 -1 0 0 0 -1    0 0 0 -1 -1 -1    0 -1 -1 0 -1 -1    -1 0 -1 -1 0 -1    -1 -1 0 -1 -1 0");
}
