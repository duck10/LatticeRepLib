#ifndef COLORTABLES_H
#define COLORTABLES_H

#include <algorithm>
#include <string>

#include "HSV.h"
#include "RGB.h"

class ColorRange {
public:
   ColorRange(const unsigned long minv, const unsigned long maxv);
   double ColorFraction(const double color);
   int GetColorFromRangeFraction(const double frac) const;
   void GetHSVFromRangeFraction(const double frac,  unsigned long& h,
       unsigned long& s,  unsigned long& v) const;
   void GetRGBFromRangeFraction(const double frac,  unsigned long& r,
       unsigned long& g,  unsigned long& b) const;
   int ColorIndex(const double color);
private:
   const unsigned long m_minhex;
   const unsigned long m_maxhex;
   double m_minh;
   double m_mins;
   double m_minv;
   double m_maxh;
   double m_maxs;
   double m_maxv;
};

class ColorTables {
public:
   static std::string GetCividisHEX(const size_t n);

   static void GetCividisRGB(const size_t n, int& rreturn, int& greturn, int& breturn);


   static const std::string BASIC_COLORS[];

   static std::string NumberToHexString(const int n);
   static void ConvertHexColorToRGB(const unsigned long n, 
      unsigned long& red, unsigned long& green, unsigned long& blue);
   static std::string interpolateColor(size_t index, size_t total);
   static const unsigned long START_COLOR = 0x0077BB;  // Blue
   static const unsigned long END_COLOR = 0xEE7733;    // Orange

private:
   static std::string makeHexColor(unsigned long r, unsigned long g, unsigned long b);


};

#endif // COLORTABLES_H
