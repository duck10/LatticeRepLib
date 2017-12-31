
#include "FollowerTools.h"
#include "LRL_ToString.h"

#include <fstream>
#include <iostream>
#include <string>

#include "D7.h"
#include "G6.h"
#include "B4.h"
#include "LRL_Cell.h"
#include "S6.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::string LRL_ToString( const std::ostringstream& t ) {
   return( t.str( ) );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::pair<int,std::string> IdentifyNearbyBoundaries(const G6& g6, const double cutoff)
/*-------------------------------------------------------------------------------------*/
{
   std::string s1, s2;

   if (std::abs(g6[0] - g6[1]) < cutoff)
   {
      s1 += '1';
      s2 += '1';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(g6[1] - g6[2]) < cutoff)
   {
      s1 += '2';
      s2 += '2';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(g6[3]) < cutoff)
   {
      s1 += '3';
      s2 += '3';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(g6[4]) < cutoff)
   {
      s1 += '4';
      s2 += '4';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(g6[5]) < cutoff)
   {
      s1 += '5';
      s2 += '5';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(g6[1] - g6[3]) < cutoff &&
      (g6[5] - g6[4]) < cutoff)
   {
      s1 += '6';
      s2 += '6';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(g6[1] - g6[3]) < cutoff &&
      (g6[4] - g6[5]) < cutoff)
   {
      s1 += '7';
      s2 += '7';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(g6[1] + g6[3]) < cutoff)
   {
      s1 += '8';
      s2 += '8';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(g6[0] - g6[4]) < cutoff &&
      (g6[5] - g6[3]) < cutoff)
   {
      s1 += '9';
      s2 += '9';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(g6[0] - g6[4]) < cutoff &&
      (g6[3] - g6[5]) < cutoff)
   {
      s1 += 'A';
      s2 += 'A';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(g6[0] + g6[4]) < cutoff)
   {
      s1 += 'B';
      s2 += 'B';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(g6[0] - g6[5]) < cutoff &&
      (g6[4] - g6[3]) < cutoff)
   {
      s1 += 'C';
      s2 += 'C';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(g6[0] - g6[5]) < cutoff &&
      (g6[3] - g6[4]) < cutoff)
   {
      s1 += 'D';
      s2 += 'D';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(g6[0] + g6[5]) < cutoff)
   {
      s1 += 'E';
      s2 += 'E';
   }
   else
   {
      s2 += '.';
   }

   if (abs(g6[0] + g6[1] + g6[3] + g6[4] + g6[5]) < cutoff)
   {
      s1 += 'F';
      s2 += 'F';
   }
   else
   {
      s2 += '.';
   }

   return std::make_pair( -1, (s1.empty()) ? "" : "(" + s1 + ") \t(" + s2 + ")" );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::pair<int, std::string> IdentifyNearbyBoundaries(const D7& v, const double cutoff)
/*-------------------------------------------------------------------------------------*/
{
   std::string s1, s2;
   int boundaryNumber= -1;
   const double& d1 = v[0];
   const double& d2 = v[1];
   const double& d3 = v[2];
   const double& b4 = v[3];
   const double& d5 = v[4];
   const double& s6 = v[5];
   const double& d7 = v[6];

   if (std::abs(d1 - d2) < cutoff)
   {
      s1 += '1';
      s2 += '1';
      boundaryNumber = 1;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d2 - d3) < cutoff)
   {
      s1 += '2';
      s2 += '2';
      boundaryNumber = 2;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d3 - b4) < cutoff)
   {
      s1 += '3';
      s2 += '3';
      boundaryNumber = 3;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d2+d3-d5) < cutoff)
   {
      s1 += '4';
      s2 += '4';
      boundaryNumber = 4;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d1+b4-d5) < cutoff)
   {
      s1 += '5';
      s2 += '5';
      boundaryNumber = 5;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d1+d3-s6) < cutoff)
   {
      s1 += '6';
      s2 += '6';
      boundaryNumber = 6;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d2+b4-s6) < cutoff)
   {
      s1 += '7';
      s2 += '7';
      boundaryNumber = 7;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d1+d2-d7) < cutoff)
   {
      s1 += '8';
      s2 += '8';
      boundaryNumber = 8;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d3+b4-d7) < cutoff)
   {
      s1 += '9';
      boundaryNumber = 9;
      s2 += '9';
   }
   else
   {
      s2 += '.';
   }


   return std::make_pair( boundaryNumber, (s1.empty()) ? "" : "(" + s1 + ") \t(" + s2 + ")" );
}

