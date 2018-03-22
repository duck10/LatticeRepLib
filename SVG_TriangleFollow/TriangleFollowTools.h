#ifdef _MSC_VER
#define USE_LOCAL_HEADERS
#endif

#ifndef TRIANGLEFOLLOWTOOLS_H
#define TRIANGLEFOLLOWTOOLS_H

#include <iomanip>

#include "FollowerTools.h"
#include "LRL_ToString.h"

template<typename T>
struct Triple
{
   std::vector<T> sl1;
   std::vector<T> sl2;
   std::vector<T> sl3;
};

static
G6 SetCellParams(const G6& vin)
{
   G6 v(vin);
   if (v[3] * v[4] * v[5] <= 0.0) {
      v[5] = -std::abs(v[5]);
      v[3] = std::abs(v[3]);
      v[4] = std::abs(v[4]);
   }
   return v;
};

static double TriangleAreaFromSides(const double a, const double b, const double c)
{
   const double s = (a + b + c) / 2.0; // s is half the perimeter
   const double trialValue = s * (s - a)*(s - b)*(s - c);
   const double sign = (trialValue >= 0.0) ? 1.0 : -1.0;
   return sign * std::sqrt(std::abs(trialValue));
}

static double SqrtTriangleAreaFromSides(const double a, const double b, const double c)
{
   const double trialValue = TriangleAreaFromSides(a, b, c); // Heron's formula
   const double sign = (trialValue >= 0.0) ? 1.0 : -1.0;
   return sign * std::sqrt(std::abs(trialValue));
}

static std::string SmallValueFormat( const double value )
{
   std::ostringstream oss;
   oss << std::setprecision(1) << std::scientific << value;
   unsigned int ePos = (unsigned long)oss.str().find("e");
   const std::string output(oss.str());
   const std::string mantissa = output.substr(0,ePos);
   std::string exponent = output.substr(ePos+1,output.length());
   unsigned int eZeroPos;
   while (eZeroPos=(unsigned long)exponent.find("0"), eZeroPos!=(unsigned long)std::string::npos)
   {
      exponent.erase(eZeroPos,1);
   }
   if ( ! exponent.empty() ) exponent = "E" + exponent;
   return mantissa + exponent;
}

template<typename TVEC>
static std::string LogWork( const unsigned int n, const std::string& label, const TVEC& v1, const TVEC& v2, const TVEC& v3,
   const double dist12, const double dist13, const double dist23,
   const double angleDiff, const double triangleDiff, const double area)
{
   return
    "\n" +
   LRL_ToString("POINT ", n, "  ", label, "\n" ) +
   LRL_ToString("vec1 ", v1, "   ", ::IdentifyNearbyBoundaries(D7(v1),0.01*D7(v1).norm()).second , "\n" )+
   LRL_ToString("vec2 ", v2, "   ", ::IdentifyNearbyBoundaries(D7(v2),0.01*D7(v2).norm()).second , "\n" )+
   LRL_ToString("vec3 ", v3, "   ", ::IdentifyNearbyBoundaries(D7(v3),0.01*D7(v3).norm()).second , "\n" )+
   LRL_ToString("dist12 ", dist12, ",  " ) +
   LRL_ToString("dist13 ", dist13, ",  " ) +
   LRL_ToString("dist23 ", dist23 , "\n" ) +
   LRL_ToString("angleDiff ", angleDiff ,   (angleDiff>=180.0)  ? "  ERROR" : "", "\n" ) +
   LRL_ToString("Viol. ",     triangleDiff, (triangleDiff>=0.0) ? "  ERROR" : "", "\n" ) +
   LRL_ToString("Area ",      area ,        (area<=0.0)         ? "  ERROR" : "", "\n" );
}

static double AngleDiffFromSides(const double a, const double b, const double c)
{
static const double RadToDeg = 180.0 / (4.0*atan(1.0));
   const double Acos = (b*b + c*c - a*a) / (2.0*b*c);
   const double Bcos = (a*a + c*c - b*b) / (2.0*a*c);
   const double Ccos = (a*a + b*b - c*c) / (2.0*a*b);
   if (Acos >= -1.0 && Bcos >= -1.0 && Ccos >= -1.0 && std::abs(Acos) <= 1.0 && std::abs(Bcos) <= 1.0 && std::abs(Ccos) <= 1.0)
   {
      const double Aangle = acos(Acos)*RadToDeg;
      const double Bangle = acos(Bcos)*RadToDeg;
      const double Cangle = acos(Ccos)*RadToDeg;
      const double maxAngle = maxNC(Aangle, Bangle, Cangle);
      const double minAngle = minNC(Aangle, Bangle, Cangle);
      return maxAngle - minAngle;
   } else {
      return 180.0;
   }
}


static double AngleCosineFromSides(const double d1, const double d2, const double d3) {
   return (d1*d1 - d2*d2 - d3*d3) / (-2.0 * d2*d3);
}

#endif