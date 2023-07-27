// Yamada.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>

#include "TNear.h"

class V2 {
public:
   V2() :m1(19191), m2(19191) {}
   V2(const double a, const double b ) :m1(a), m2(b) {}

   V2 operator- (const V2 d) const {
      V2 a; 
      a.m1 = (*this).m1 - d.m1;
      a.m2 = (*this).m2 - d.m2;
      return a; 
   }

   double norm() const { return sqrt(m1*m1 + m2*m2);; }
   double norm(const V2 d) const { d.norm(); }
   double operator[] (const size_t n) const { return (n==0) ? m1 : m2; }

private: 
   double m1;
   double m2;
};

std::string lattice;


V2 ReadOneYamadaCell() {
   double a, b, c;
   std::string dummy;
   std::cin >> lattice;
   std::cin >> a;
   std::cin >> b;
   std::cin >> c;
   std::getline(std::cin, dummy);
   return V2(b, c);
}

int main()
{
   std::cout << "yamada" << std::endl;
   CNearTree<V2> nt;
   nt.insert(V2(78.669, 37.694));
   nt.insert(V2(79.576, 37.52 ));
   nt.insert(V2(79.73, 36.544 ));
   nt.insert(V2(79.753, 35.89 ));
   nt.insert(V2(79.864, 35.291));

   std::cout << nt.size() << std::endl;

   std::vector<int> count(5);
   int nread = 0;
   while (nread < 1000) {
      const V2 v2 = ReadOneYamadaCell();
      if ( nread%100 == 0) std::cout << v2[0] << " " << v2[1] << std::endl;
      CNearTree<V2>::iterator nti = nt.NearestNeighbor(100000, v2);
      const size_t index = nti.get_position();
      if (index > nt.size()-1) break;
      ++count[index];
      ++nread;
   }
   std::cout << std::endl;
   for (size_t i = 0; i < count.size(); ++i) std::cout << count[i] << std::endl;
   int i19191 = 19191;;
}
