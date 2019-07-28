// B4_rotations.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <vector>
#include "B4.h"
#include "MatB4.h"
#include "TNear.h"

int main()
{
   std::vector<MatB4> v3fold;
   std::vector<MatB4> v2fold;

   MatB4 vunit(MatB4("1 0 0 0  0 1 0 0  0 0 1 0  0 0 0 1")); // 

   v3fold.push_back(MatB4("0 0 1 0  1 0 0 0  0 1 0 0  0 0 0 1")); // abc
   v3fold.push_back(MatB4("0 0 0 1  0 1 0 0  0 0 1 0  1 0 0 0")); // acd
   v3fold.push_back(MatB4("0 0 0 1  1 0 0 0  0 0 1 0  0 1 0 0")); // adb
   v3fold.push_back(MatB4("1 0 0 0  0 0 0 1  0 1 0 0  0 0 1 0")); // bcd

   v2fold.push_back(MatB4("0 1 0 0  1 0 0 0  0 0 0 1  0 0 1 0")); // ab/cd
   v2fold.push_back(MatB4("0 0 1 0  0 0 0 1  1 0 0 0  0 1 0 0")); // ac/bd
   v2fold.push_back(MatB4("0 0 0 1  0 0 1 0  0 1 0 0  1 0 0 0")); // ad/bc

   CNearTree<MatB4> nt;
   nt.insert(vunit);

   int count = 0;
   int prevcount = 19191;

   MatB4 close;

   while (count != prevcount) {
      prevcount = nt.size();
      for (size_t i3 = 0; i3 < v3fold.size(); ++i3) {
         for (size_t knt = 0; knt < nt.size(); ++knt) {
            MatB4 m = v3fold[i3] * nt[knt];
            nt.NearestNeighbor(0.01, close, m);
            const double d = (m - close).norm();
            if (!nt.NearestNeighbor(0.01, close, m)) {
               nt.insert(m);
               nt.CompleteDelayedInsert();
            }
         }
      }
      count = nt.size();
   }

   prevcount = 19191;

   while (count != prevcount) {
      prevcount = nt.size();
      for (size_t i2 = 0; i2 < v2fold.size(); ++i2) {
         for (size_t knt = 0; knt < nt.size(); ++knt) {
            MatB4 m = v2fold[i2] * nt[knt];
            //std::cout << "m   " << std::endl << m << std::endl << std::endl;
            nt.NearestNeighbor(0.01, close, m);
            //std::cout << "close   " << std::endl << close << std::endl << std::endl;
            const double d = (m - close).norm();
            //std::cout << d << std::endl << std::endl;
            if (!nt.NearestNeighbor(0.01, close, m)) {
               nt.insert(m);
               nt.CompleteDelayedInsert();
            }
         }
      }
      count = nt.size();
   }


   for (size_t knt = 0; knt < nt.size(); ++knt) {
      MatB4 m = nt[knt];
      std::cout << m << std::endl << std::endl;
   }

   std::cout << "nt size  " << nt.size() << std::endl;


   return 0;
}

