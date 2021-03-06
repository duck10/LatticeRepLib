// S6TestDist.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "CS6Dist.h"
#include "CS6Dist.c"
#include "LRL_Cell.h"
#include "LRL_ToString.h"
#include "S6.h"
#include "S6Dist.h"
#include "Selling.h"
#include "StoreResults.h"

int main()
{
   StoreResults<int, std::string> store(10);
   S6Dist s6dist(1);
   S6 s61;
   S6 s62;

   double factor = 1.0;

   /*while (factor < 100.0) */{
      store.clear();
      for (size_t i = 0; i < 100000; ++i) {
         S6 out;
         Selling::Reduce(S6::rand(), out);
         s61 = out;
         Selling::Reduce(S6::rand(), out);
         s62 = factor * out;


         s6dist.OneBoundaryDistance(s61, s62);
         const double dist1 = s6dist.GetMin();
         s6dist.TwoBoundaryDistance(s61, s62);
         const double dist2 = s6dist.GetMin();
         const double dist3 = CS6Dist(s61.data(), s62.data());
         //std::cout << dist1 << "  " << dist2 << std::endl;
         double diff = 100.0*abs(dist1 - dist2) / dist1;
         store.Store(int(diff), LRL_ToString(dist1, dist2, dist3, "    ", 
            C3::ConvertToFundamentalUnit(s61), "  ", 
            C3::ConvertToFundamentalUnit(s62)));
      }
      store.SetTitle(LRL_ToString("multiplier of 2nd random s6  ", factor));
      store.ShowResultsByKeyAscending();
      factor *= 1.2;
   }

}
