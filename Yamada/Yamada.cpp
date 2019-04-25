// Yamada.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "TNear.h"

class V2 {
public:
   V2() :m1(19191), m2(19191) {}
   V2(const double a, const double b ) :m1(a), m2(b) {}
   V2 operator- (const V2 d) { V2 a; a.m1 = (*this).m1 = d.m1; a.m2 = (*this).m2 - d.m2; }
   double norm(const V2 d) { return sqrt(d.m1*d.m1 + d.m2 - d.m2); }

private: 
   double m1;
   double m2;
}

;
int main()
{
   CNearTree<V2> nt;
   nt.insert(V2(78.669, 37.694));
   nt.insert(V2(79.576, 37.52));
   nt.insert(V2(79.73, 36.544));
   nt.insert(V2(79.753, 35.89));
   nt.insert(V2(79.864, 35.291));

}
