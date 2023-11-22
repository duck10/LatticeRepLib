#ifndef C3PLOT_H
#define C3PLOT_H

#include <string>
#include <vector>

#include "S6.h"
#include "LRL_Vector3.h"

class C3Plot {
public:
   C3Plot(const std::string& filename, const int wx, const int wy, const int gx, const int gy);

   std::string GetIntro(const std::string& filename) { return m_svgIntro; }
   std::string GetFoot() { return m_svgFoot; }
   double CellScale(const std::vector<S6>& v);
   double CellScaleFactor();
   std::string DrawCells(const size_t scalar, const std::vector<S6>& v);
   double GetWx() const { return m_wx; }
   double GetWy() const { return m_wy; }
   double GetGx() const { return m_gx; }
   double GetGy() const { return m_gy; }
   //std::string DrawGraphs(const std::vector<S6>& v);
   std::string PlotC3(const size_t scalar, const int wx, const int wy, const std::string& s);

   //std::string WrapSVG(const std::string& fileName,
   //   const int wx = 2000, const int wy = 2000, const std::string& s = "");

   S6 FindNearestReflection(const S6& ref, const S6& var);
   std::vector<S6> PrepareCells();
   std::string CreatePolylineFromPoints(const size_t scalarr, const std::string& width, const std::vector<S6>& v);
   void SendFrameToFile(const std::string& sFileName, const std::string& data);
   int GetColor() const { return m_color; }
   void SetColor(const int color) { m_color = color; }
   void SetColorDelta(const int color) { m_deltaColor = color; }
   int GetColorDelta() const { return m_deltaColor; }

protected:
   const int m_wx;
   const int m_wy;
   const int m_gx;
   const int m_gy;

   int m_color;
   int m_deltaColor;

   std::string m_svgIntro;
   std::string m_svgFoot;

private:
   std::string BuildIntro(const std::string& filename);
   double m_maxScalar;

};

class ScalarProperties {

public:
   size_t index1, index2;
   int moveX, moveY;

   ScalarProperties(const size_t scalar) {
      if (scalar == 1) {
         index1 = 0;
         index2 = 3;
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
         index1 = 2;
         index2 = 5;
         moveX = 0;
         moveY = 550;
      }
   }

};


#endif  // C3PLOT_H

