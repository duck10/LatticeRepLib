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
   static std::string GetCividisHEX(const size_t n);

   static void GetCividisRGB(const size_t n, int& rreturn, int& greturn, int& breturn);


   static const std::string BASIC_COLORS[];

private:


};

#endif // COLORTABLES_H
