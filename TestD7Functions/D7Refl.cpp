#include "stdafx.h"

#include "D7Refl.h"

void D7refl0(const double d[], double dd[]) {
   /*D7Refl_dd[] = d[0];dd[] = d1234567Ident*/
/*DR_0*/
   dd[0] = d[0];
   dd[1] = d[1];
   dd[2] = d[2];
   dd[3] = d[3];
   dd[4] = d[4];
   dd[5] = d[5];
   dd[6] = d[6];
}

void D7refl1(const double d[], double dd[]) {
   /*D7Refl_4 {1,2,4,3,6,5,7},   c <-> d */
   /*DR_1*/   dd[0] = d[0];
   dd[1] = d[1];
   dd[2] = d[3];
   dd[3] = d[2];
   dd[4] = d[5];
   dd[5] = d[4];
   dd[6] = d[6];
}

void D7refl2(const double d[], double dd[]) {
   /*D7Refl_3dd[] = d1324576b<->c*/
/*DR_2*/
   dd[0] = d[0];
   dd[1] = d[2];
   dd[2] = d[1];
   dd[3] = d[3];
   dd[4] = d[4];
   dd[5] = d[6];
   dd[6] = d[5];
}

void D7refl3(const double d[], double dd[]) {
   /*dd[] = d1342756*/
   /*DR_3*/
   dd[0] = d[0];
   dd[1] = d[2];
   dd[2] = d[3];
   dd[3] = d[1];
   dd[4] = d[6];
   dd[5] = d[4];
   dd[6] = d[5];
}

void D7refl4(const double d[], double dd[]) {
   /*dd[] = d1423675*/
/*DR_4*/
   dd[0] = d[0];
   dd[1] = d[3];
   dd[2] = d[1];
   dd[3] = d[2];
   dd[4] = d[5];
   dd[5] = d[6];
   dd[6] = d[4];
}

void D7refl5(const double d[], double dd[]) {
   /* {1,4,3,2,6.7,5} */
/*DR_5*/
   dd[0] = d[0];
   dd[1] = d[3];
   dd[2] = d[2];
   dd[3] = d[1];
   dd[4] = d[6];
   dd[5] = d[5];
   dd[6] = d[4];
}

void D7refl6(const double d[], double dd[]) {
   /*D7Refl_2 {2,1,3,4,6,5,7},   a <-> b */
/*DR_6*/
   dd[0] = d[1];
   dd[1] = d[0];
   dd[2] = d[2];
   dd[3] = d[3];
   dd[4] = d[5];
   dd[5] = d[4];
   dd[6] = d[6];
}

void D7refl7(const double d[], double dd[]) {
   /*DR_7*/
   dd[0] = d[1];
   dd[1] = d[0];
   dd[2] = d[3];
   dd[3] = d[2];
   dd[4] = d[4];
   dd[5] = d[5];
   dd[6] = d[6];
}

void D7refl8(const double d[], double dd[]) {
   /*DR_8*/
   dd[0] = d[1];
   dd[1] = d[2];
   dd[2] = d[0];
   dd[3] = d[3];
   dd[4] = d[5];
   dd[5] = d[6];
   dd[6] = d[4];
}

void D7refl9(const double d[], double dd[]) {
   /*DR_9*/
   dd[0] = d[1];
   dd[1] = d[2];
   dd[2] = d[3];
   dd[3] = d[0];
   dd[4] = d[6];
   dd[5] = d[5];
   dd[6] = d[4];
}

void D7refl10(const double d[], double dd[]) {
   /*DR_dd[] = d[0];x*/
   dd[0] = d[1];
   dd[1] = d[3];
   dd[2] = d[0];
   dd[3] = d[2];
   dd[4] = d[4];
   dd[5] = d[6];
   dd[6] = d[5];
}

void D7refl11(const double d[], double dd[]) {
   /*DR_dd[] = d[0];dd[] = d[0];*/
   dd[0] = d[1];
   dd[1] = d[3];
   dd[2] = d[2];
   dd[3] = d[0];
   dd[4] = d[6];
   dd[5] = d[4];
   dd[6] = d[5];
}

void D7refl12(const double d[], double dd[]) {
   /*3dd[] = d[2];24756*/
/*DR_dd[] = d[0];2*/
   dd[0] = d[2];
   dd[1] = d[0];
   dd[2] = d[1];
   dd[3] = d[3];
   dd[4] = d[6];
   dd[5] = d[4];
   dd[6] = d[5];
}

void D7refl13(const double d[], double dd[]) {
   /*DR_dd[] = d[0];3*/
   dd[0] = d[2];
   dd[1] = d[0];
   dd[2] = d[3];
   dd[3] = d[1];
   dd[4] = d[4];
   dd[5] = d[6];
   dd[6] = d[5];
}

void D7refl14(const double d[], double dd[]) {
   /*DR_dd[] = d[0];4*/
   dd[0] = d[2];
   dd[1] = d[1];
   dd[2] = d[0];
   dd[3] = d[3];
   dd[4] = d[6];
   dd[5] = d[5];
   dd[6] = d[4];
}

void D7refl15(const double d[], double dd[]) {
   /*DR_dd[] = d[0];5*/
   dd[0] = d[2];
   dd[1] = d[1];
   dd[2] = d[3];
   dd[3] = d[0];
   dd[4] = d[5];
   dd[5] = d[6];
   dd[6] = d[4];
}

void D7refl16(const double d[], double dd[]) {
   /*DR_dd[] = d[0];6*/
   dd[0] = d[2];
   dd[1] = d[3];
   dd[2] = d[0];
   dd[3] = d[1];
   dd[4] = d[4];
   dd[5] = d[5];
   dd[6] = d[6];
}

void D7refl17(const double d[], double dd[]) {
   /*DR_dd[] = d[0];7*/
   dd[0] = d[2];
   dd[1] = d[3];
   dd[2] = d[1];
   dd[3] = d[0];
   dd[4] = d[5];
   dd[5] = d[4];
   dd[6] = d[6];
}

void D7refl18(const double d[], double dd[]) {
   /*DR_dd[] = d[0];8*/
   dd[0] = d[3];
   dd[1] = d[0];
   dd[2] = d[1];
   dd[3] = d[2];
   dd[4] = d[6];
   dd[5] = d[5];
   dd[6] = d[4];
}

void D7refl19(const double d[], double dd[]) {
   /*DR_dd[] = d[0];9*/
   dd[0] = d[3];
   dd[1] = d[0];
   dd[2] = d[2];
   dd[3] = d[1];
   dd[4] = d[5];
   dd[5] = d[6];
   dd[6] = d[4];
}

void D7refl20(const double d[], double dd[]) {
   /*DR_2x*/
   dd[0] = d[3];
   dd[1] = d[1];
   dd[2] = d[0];
   dd[3] = d[2];
   dd[4] = d[6];
   dd[5] = d[4];
   dd[6] = d[5];
}

void D7refl21(const double d[], double dd[]) {
   /*DR_2dd[] = d[0];*/
   dd[0] = d[3];
   dd[1] = d[1];
   dd[2] = d[2];
   dd[3] = d[0];
   dd[4] = d[4];
   dd[5] = d[6];
   dd[6] = d[5];
}

void D7refl22(const double d[], double dd[]) {
   /*DR_22*/
   dd[0] = d[3];
   dd[1] = d[2];
   dd[2] = d[0];
   dd[3] = d[1];
   dd[4] = d[5];
   dd[5] = d[4];
   dd[6] = d[6];
}

void D7refl23(const double d[], double dd[]) {
   /*DR_23*/
   dd[0] = d[3];
   dd[1] = d[2];
   dd[2] = d[1];
   dd[3] = d[0];
   dd[4] = d[4];
   dd[5] = d[5];
   dd[6] = d[6];
}

std::vector< void(*)(const double[], double[])> GetReflectionFunctions() {
   std::vector< void(*)(const double[], double[])> v;
   v.push_back(D7refl0);
   v.push_back(D7refl1);
   v.push_back(D7refl2);
   v.push_back(D7refl3);
   v.push_back(D7refl4);
   v.push_back(D7refl5);
   v.push_back(D7refl6);
   v.push_back(D7refl7);
   v.push_back(D7refl8);
   v.push_back(D7refl9);

   v.push_back(D7refl10);
   v.push_back(D7refl11);
   v.push_back(D7refl12);
   v.push_back(D7refl13);
   v.push_back(D7refl14);
   v.push_back(D7refl15);
   v.push_back(D7refl16);
   v.push_back(D7refl17);
   v.push_back(D7refl18);
   v.push_back(D7refl19);

   v.push_back(D7refl20);
   v.push_back(D7refl21);
   v.push_back(D7refl22);
   v.push_back(D7refl23);
   return v;
}