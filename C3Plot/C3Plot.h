#ifndef C3PLOT_H
#define C3PLOT_H

#include <string>

class C3Plot {
public:
   C3Plot(const int wx, const int wy, const int gx, const int gy);

   std::string GetIntro() { return m_svgIntro; }
   std::string GetFoot() { return m_svgFoot; }
   std::string BuildIntro();

protected:
   const int m_wx;
   const int m_wy;
   const int m_gx;
   const int m_gy;

   std::string m_svgIntro;
   std::string m_svgFoot;

private:
};

#endif  // C3PLOT_H