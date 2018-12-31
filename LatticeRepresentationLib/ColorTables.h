#ifndef COLORTABLES_H
#define COLORTABLES_H

#include <algorithm>
#include <string>

class ColorRange {
public:
   ColorRange(const double minv, const double maxv);
   double ColorFraction(const double color);
   int ColorIndex(const double color);
private:
   const double m_min;
   const double m_max;
};

class ColorTables {
public:
   static std::string GetCividisHEX(const unsigned long n);

   static void GetCividisRGB(const unsigned long n, int& rreturn, int& greturn, int& breturn);


   static const std::string BASIC_COLORS[];

private:


};

#endif // COLORTABLES_H
