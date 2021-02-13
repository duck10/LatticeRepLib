// TestRandom.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <iostream>

#include "B4.h"
#include "C3.h"
#include "D7.h"
#include "G6.h"
#include "S6.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ToString.h"

template<typename T>
void DoRandDeloneUnreduced() {
   T t;
   t = T::randDeloneUnreduced();
   std::cout << T::GetName() << "  " << t << std::endl;
   std::cout << LRL_Cell_Degrees(LRL_Cell(t)) << std::endl << std::endl;
}

template<typename T>
void DoRandDelone() {
   T t;
   t = T::rand();
   std::cout << T::GetName() << "  " << t << std::endl;
   std::cout << LRL_Cell_Degrees(LRL_Cell(t)) << std::endl << std::endl;
}

template<typename T>
void DoRand() {
   T t;
   t = T::randDeloneReduced();
   std::cout << T::GetName() << "  " << t << std::endl;
   std::cout << LRL_Cell_Degrees(LRL_Cell(t)) << "  corresponding " << LRL_Cell_Degrees::GetName() << std::endl << std::endl;
}

void TestRand() {
   DoRand<B4>();
   DoRand<D7>();
   DoRand<G6>();
   DoRand<S6>();
   DoRand<C3>();
   DoRand<LRL_Cell>();

   std::cout << "*********************************************" << std::endl;
}

void TestRandDelone() {
   DoRandDelone<B4>();
   DoRandDelone<D7>();
   DoRandDelone<G6>();
   DoRandDelone<S6>();
   DoRandDelone<C3>();
   DoRandDelone<LRL_Cell>();

   std::cout << "*********************************************" << std::endl;
}

void TestRandUndelone() {
   DoRandDeloneUnreduced<B4>();
   DoRandDeloneUnreduced<D7>();
   DoRandDeloneUnreduced<G6>();
   DoRandDeloneUnreduced<S6>();
   DoRandDeloneUnreduced<C3>();
   DoRandDeloneUnreduced<LRL_Cell>();

   std::cout << "*********************************************" << std::endl;
}

int main()
{

   //MatN mn(9), mn2(9), mn3(9);
   //mn = MatN::Eye(9);
   //mn2.Eye();
   //std::cout << mn << std::endl << std::endl;
   //std::cout << mn2 << std::endl << std::endl;
   //exit(0);
   TestRand();
   TestRandDelone();
   TestRandUndelone();
   return 0;
}

