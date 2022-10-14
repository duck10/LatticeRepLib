#include "C3Plot.h"
#include "LRL_CreateFileName.h"
#include "LRL_ToString.h"
#include "LRL_DataToSVG.h"


#include <iostream>
#include <string>
#include <vector>


C3Plot::C3Plot(const int wx, const int wy, const int gx, const int gy)
   : m_wx(wx)
   , m_wy(wy)
   , m_gx(gx)
   , m_gy(gy)
   , m_svgIntro(BuildIntro())
{
}

std::string C3Plot::BuildIntro() {
   //const int wx = 2000, const int wy = 2000, const std::string& s = "") {
   const std::string swx = LRL_DataToSVG("\"", m_wx, "\"");
   const std::string swy = LRL_DataToSVG("\"", m_wy, "\"");
   
   const std::string filename = LRL_CreateFileName::Create("SEL_", "svg");

   return
   "<svg width=" + swx + " height=" + swy +
      " viewBox=\"1 0  " + LRL_DataToSVG(m_wx * 1.2, " ", m_wy * 1.2) +
      "\"  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" \nxmlns:xlink=\"http://www.w3.org/1999/xlink\">" +
      "\n<desc>File name  = " + filename + " </desc> """ +
      "\n\n";
}
