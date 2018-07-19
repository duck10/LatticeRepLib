#if _MSC_VER
#pragma once
#endif

#include "FollowerTools.h"
#include "ProjectorTools.h"
#include "ToString.h"

#include <fstream>
#include <iostream>
#include <string>

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::string ToString( const std::ostringstream& t ) {
   return( t.str( ) );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void FollowerTools::OpenOutputFile( std::ofstream& svgOut, const std::string& sFileName ) {
   if ( svgOut.is_open( ) ) return;
   if ( ! svgOut ) {
      std::cout << "Did not create the file" << std::endl;
   } else {
      svgOut.open ( sFileName.c_str(), std::ios::out | std::ios::app | std::ios::ate );
      int count = 0;

      // do our best to open the output file
      while ( count < 1000 && ! svgOut.is_open( ) )
         ++count;
   }
}

///*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//std::pair<int,std::string> IdentifyNearbyBoundaries(const double v[6], const double cutoff)
///*-------------------------------------------------------------------------------------*/
//{
//   std::string s1, s2;
//
//   if (std::abs(v[0] - v[1]) < cutoff)
//   {
//      s1 += '1';
//      s2 += '1';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (std::abs(v[1] - v[2]) < cutoff)
//   {
//      s1 += '2';
//      s2 += '2';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (std::abs(v[3]) < cutoff)
//   {
//      s1 += '3';
//      s2 += '3';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (std::abs(v[4]) < cutoff)
//   {
//      s1 += '4';
//      s2 += '4';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (std::abs(v[5]) < cutoff)
//   {
//      s1 += '5';
//      s2 += '5';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (std::abs(v[1] - v[3]) < cutoff &&
//      (v[5] - v[4]) < cutoff)
//   {
//      s1 += '6';
//      s2 += '6';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (std::abs(v[1] - v[3]) < cutoff &&
//      (v[4] - v[5]) < cutoff)
//   {
//      s1 += '7';
//      s2 += '7';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (std::abs(v[1] + v[3]) < cutoff)
//   {
//      s1 += '8';
//      s2 += '8';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (std::abs(v[0] - v[4]) < cutoff &&
//      (v[5] - v[3]) < cutoff)
//   {
//      s1 += '9';
//      s2 += '9';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (std::abs(v[0] - v[4]) < cutoff &&
//      (v[3] - v[5]) < cutoff)
//   {
//      s1 += 'A';
//      s2 += 'A';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (std::abs(v[0] + v[4]) < cutoff)
//   {
//      s1 += 'B';
//      s2 += 'B';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (std::abs(v[0] - v[5]) < cutoff &&
//      (v[4] - v[3]) < cutoff)
//   {
//      s1 += 'C';
//      s2 += 'C';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (std::abs(v[0] - v[5]) < cutoff &&
//      (v[3] - v[4]) < cutoff)
//   {
//      s1 += 'D';
//      s2 += 'D';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (std::abs(v[0] + v[5]) < cutoff)
//   {
//      s1 += 'E';
//      s2 += 'E';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   if (abs(v[0] + v[1] + v[3] + v[4] + v[5]) < cutoff)
//   {
//      s1 += 'F';
//      s2 += 'F';
//   }
//   else
//   {
//      s2 += '.';
//   }
//
//   return(s1.empty()) ? "" : "(" + s1 + ") \t(" + s2 + ")";
//}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::pair<int,std::string> IdentifyNearbyBoundaries(const double v[6], const double cutoff)
/*-------------------------------------------------------------------------------------*/
{
   std::string s1, s2;

   if (std::abs(v[0] - v[1]) < cutoff)
   {
      s1 += '1';
      s2 += '1';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[1] - v[2]) < cutoff)
   {
      s1 += '2';
      s2 += '2';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[3]) < cutoff)
   {
      s1 += '3';
      s2 += '3';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[4]) < cutoff)
   {
      s1 += '4';
      s2 += '4';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[5]) < cutoff)
   {
      s1 += '5';
      s2 += '5';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[1] - v[3]) < cutoff &&
      (v[5] - v[4]) < cutoff)
   {
      s1 += '6';
      s2 += '6';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[1] - v[3]) < cutoff &&
      (v[4] - v[5]) < cutoff)
   {
      s1 += '7';
      s2 += '7';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[1] + v[3]) < cutoff)
   {
      s1 += '8';
      s2 += '8';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[0] - v[4]) < cutoff &&
      (v[5] - v[3]) < cutoff)
   {
      s1 += '9';
      s2 += '9';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[0] - v[4]) < cutoff &&
      (v[3] - v[5]) < cutoff)
   {
      s1 += 'A';
      s2 += 'A';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[0] + v[4]) < cutoff)
   {
      s1 += 'B';
      s2 += 'B';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[0] - v[5]) < cutoff &&
      (v[4] - v[3]) < cutoff)
   {
      s1 += 'C';
      s2 += 'C';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[0] - v[5]) < cutoff &&
      (v[3] - v[4]) < cutoff)
   {
      s1 += 'D';
      s2 += 'D';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[0] + v[5]) < cutoff)
   {
      s1 += 'E';
      s2 += 'E';
   }
   else
   {
      s2 += '.';
   }

   if (abs(v[0] + v[1] + v[3] + v[4] + v[5]) < cutoff)
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
std::pair<int, std::string> IdentifyNearbyBoundaries(const G6& v, const double cutoff)
/*-------------------------------------------------------------------------------------*/
{
   double ar[6];
   for (size_t i = 0; i<6; ++i) ar[i] = v[i];
   return IdentifyNearbyBoundaries(ar, cutoff);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::pair<int, std::string> D7_IdentifyNearbyBoundaries(const D7& v, const double cutoff)
/*-------------------------------------------------------------------------------------*/
{
   std::string s1, s2;
   int boundaryNumber= -1;
   const double& d1 = v[0];
   const double& d2 = v[1];
   const double& d3 = v[2];
   const double& d4 = v[3];
   const double& d5 = v[4];
   const double& d6 = v[5];
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

   if (std::abs(d3 - d4) < cutoff)
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

   if (std::abs(d1+d4-d5) < cutoff)
   {
      s1 += '5';
      s2 += '5';
      boundaryNumber = 5;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d1+d3-d6) < cutoff)
   {
      s1 += '6';
      s2 += '6';
      boundaryNumber = 6;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d2+d4-d6) < cutoff)
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

   if (std::abs(d3+d4-d7) < cutoff)
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

