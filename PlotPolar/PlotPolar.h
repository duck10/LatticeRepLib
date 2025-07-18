#ifndef PLOTPOLAR_H
#define PLOTPOLAR_H

#include <cmath>
#include <string>
#include <vector>

#include "ColorTables.h"
#include "Polar.h"
#include "LRL_Vector3.h"

class PlotPolar {
public:
   PlotPolar(const std::string& filename, const int wx, const int wy, const int gx, const int gy);

   std::string GetIntro(const std::string& filename) const { return m_svgIntro; }
   std::string GetFoot() const { return m_svgFoot; }
   double CellScale(const std::vector<Polar>& v);
   double CellScaleFactor() const;
   double GetWx() const { return m_wx; }
   double GetWy() const { return m_wy; }
   double GetGx() const { return m_gx; }
   double GetGy() const { return m_gy; }
   void SendFrameToFile(const std::string& sFileName, const std::string& data) const;

private:
   const int m_wx;
   const int m_wy;
   const int m_gx;
   const int m_gy;

   int m_color;

   std::string m_svgIntro;
   std::string m_svgFoot;

   double m_maxScalar = -DBL_MAX;

   std::string BuildIntro(const std::string& filename) const;
};

class ScalarProperties {

public:
   size_t index1;
   size_t index2;
   int moveX;
   int moveY;

   explicit ScalarProperties(const size_t scalar) {
      if (scalar == 1) {
         index1 = 0;
         index2 = 1;
         moveX = 0;
         moveY = 0;
      }
      else if (scalar == 2) {
         index1 = 1;
         index2 = 4;
         moveX = 550;
         moveY = 0;
      }
      else {
         index1 = 0;
         index2 = 1;
         moveX = 550;
         moveY = 0;
      }
   }

};

#endif  // PLOTPOLAR_H

