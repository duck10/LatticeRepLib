#include "DeloneGridClasses.h"
#include "MakeTriangles.h"

#include <sstream>

BasicDeloneRectangle::BasicDeloneRectangle() {}

std::string DeloneBackgroundWithTriangle::MakeSVG() {
   Triangles tr;
   const std::string triangle = tr.MakeBasicTriangle();
   return triangle;
}

std::string KeyFrame::MakeSVG() {
   const static std::string rect = "   <rect id=\"Basic\" x=\"0\" y=\"0\" width=\"102\" height=\"102\" stroke-width=\"2\" stroke=\"black\" fill=\"none\" />\n";
   std::string bdr = BasicDeloneRectangle::MakeSVG();
   Triangles tr;
   const std::string triangle = tr.MakeBasicTriangle();
   tr.MakeSlashes();
   return rect + triangle;

}

std::string GrayRectangleWithTriangle::MakeSVG() {
   const static std::string rect = "   <rect id=\"Basic\" x=\"0\" y=\"0\" width=\"102\" height=\"102\" stroke-width=\"2\" stroke=\"black\" fill=\"lightgrey\" />\n";
   std::string bdr = BasicDeloneRectangle::MakeSVG();
   Triangles tr;
   //tr.OneSlashAC();
   const std::string triangle = tr.MakeBasicTriangle();
   //std::cout << triangle << std::endl;
   std::string xxxxx = GrayRectangle::MakeSVG();
   tr.MakeSlashes();
   return rect+triangle;
}

std::string BasicDeloneRectangle::MakeSVG() {
   const static std::string rect = "   <rect id=\"Basic\" x=\"0\" y=\"0\" width=\"102\" height=\"102\" stroke-width=\"2\" stroke=\"black\" fill=\"none\" />\n";
   return rect;
}

std::string GrayRectangle::MakeSVG() {
   const static std::string rect = "   <rect id=\"Basic\" x=\"0\" y=\"0\" width=\"102\" height=\"102\" stroke-width=\"2\" stroke=\"black\" fill=\"gray\" />\n";
   return rect;
}

std::string LightGrayRectangle::MakeSVG() {
   const static std::string rect = "   <rect id=\"Basic\" x=\"0\" y=\"0\" width=\"102\" height=\"102\" stroke-width=\"2\" stroke=\"black\" fill=\"lightgray\" />\n";
   return rect;
}

std::ostringstream& operator<< (std::ostringstream& o, const NumericRow& nr) {
   for (const auto& row : nr.m_row) {
      o << row << std::endl;
   }
   return o;
}

std::ostringstream& operator<< (std::ostringstream& o, const BasicDeloneRows& dr) {
   for (const auto& row : dr.m_row) {
      o << row << std::endl;
   }
   return o;
}

std::ostringstream& operator<< (std::ostringstream& o, const CRow& cr) {
   for (const auto& row : cr.m_row) {
      o << row << std::endl;
   }
   return o;
}

std::ostringstream& operator<< (std::ostringstream& o, const DeloneRows& dr) {
   for (const auto& row : dr.m_row) {
      o << row << std::endl;
   }
   return o;
}

template< typename T>
std::ostringstream& OutputRectangle(std::ostringstream& o, const T& t) {
   if (t.m_deloneName == "O3" || t.m_deloneName == "M3" || t.m_deloneName == "M2B" ||
      t.m_deloneName == "A2" || t.m_deloneName == "A3")
   {
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:60px\" x = \"75\" y = \"55\" >" << "*" << "</text>\n";
   }
      o << BasicDeloneRectangle().MakeSVG();
   if ( t.m_textName.empty()) 
   {
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:25px\" x = \"5\" y = \"25\" >" << t.m_deloneName << "</text>\n";
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:16px\" x = \"5\" y = \"95\" >" << t.m_bravaisType << "</text>\n";
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:16px\" x = \"32\" y = \"95\" >" << t.m_character << "</text>\n";
   }
   else if (t.m_textName == "KeyFrame") {
      o << "   <g transform=\" translate(18, 78)  \"> \n"
         << DeloneBackgroundWithTriangle().MakeSVG()
         << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:16px\" x = \"44\" y = \"-7\" >T</text>"
         << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:16px\" x = \"50\" y = \"-23\" >P</text>"
         << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:16px\" x = \"10\" y = \"-23\" >Q</text>"
         << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:16px\" x = \"29\" y = \"-29\" >U</text>"
         << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:16px\" x = \"29\" y = \"5\" >R</text>"
         << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:16px\" x = \"10\" y = \"-4\" >S</text>"
         << "\n</g>\n";

   }
   else {
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:75px\" x = \"28\" y = \"80\" >" << t.m_textName << "</text>\n";
   }
   return o;
}

std::ostringstream& operator<< (std::ostringstream& o, const C1& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const C3& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const C5& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}


std::ostringstream& operator<< (std::ostringstream& o, const T1& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const T2& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const T5& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const R1& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const R3& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const O1A& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const O2& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const O3& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const O4& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const O5& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const O1B& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const M1A& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const M1B& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const M2A& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const M2B& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const M3& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const M4& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const A1& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const A2& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const A3& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes+"\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const H4& lat) {
   o << DeloneBackgroundWithTriangle().WrapTriangleAndSlashesToTranslate(lat.triangle + lat.slashes + "\n");
   return OutputRectangle(o, lat);
}

std::ostringstream& operator<< (std::ostringstream& o, const Letter_C& cc) {
   return OutputRectangle(o, cc);
}

std::ostringstream& operator<< (std::ostringstream& o, const Letter_T& cc) {
   return OutputRectangle(o, cc);
}

std::ostringstream& operator<< (std::ostringstream& o, const Letter_R& cc) {
   return OutputRectangle(o, cc);
}

std::ostringstream& operator<< (std::ostringstream& o, const Letter_O& cc) {
   return OutputRectangle(o, cc);
}

std::ostringstream& operator<< (std::ostringstream& o, const Letter_M& cc) {
   return OutputRectangle(o, cc);
}

std::ostringstream& operator<< (std::ostringstream& o, const Letter_A& cc) {
   return OutputRectangle(o, cc);
}

std::ostringstream& operator<< (std::ostringstream& o, const Letter_H& cc) {
   return OutputRectangle(o, cc);
}

std::ostringstream& operator<< (std::ostringstream& o, const KeyFrame& br) {
   return OutputRectangle(o, br);
}

std::ostringstream& operator<< (std::ostringstream& o, const BlankRectangle& br) {
   return OutputRectangle(o, br);
}

std::ostringstream& operator<< (std::ostringstream& o, const GrayRectangle& br) {
   o << GrayRectangle().MakeSVG();
   return o;
}

std::ostringstream& operator<< (std::ostringstream& o, const LightGrayRectangle& br) {
   o << LightGrayRectangle().MakeSVG();
   return o;
}

std::ostringstream& operator<< (std::ostringstream& o, const Digit_1& d) {
   return OutputRectangle(o, d);
}
std::ostringstream& operator<< (std::ostringstream& o, const Digit_2& d) {
   return OutputRectangle(o, d);
}
std::ostringstream& operator<< (std::ostringstream& o, const Digit_3& d) {
   return OutputRectangle(o, d);
}
std::ostringstream& operator<< (std::ostringstream& o, const Digit_4& d) {
   return OutputRectangle(o, d);
}
std::ostringstream& operator<< (std::ostringstream& o, const Digit_5& d) {
   return OutputRectangle(o, d);
}

std::string DeloneBackgroundWithTriangle::WrapTriangleAndSlashesToTranslate(const std::string& s)
{
   const std::string base = triangle + slashes;
   std::ostringstream o;

   o << "   <g transform=\" translate(24, 75)  \"> \n"
      << s
      << "\n</g>\n";

   return o.str();
}


KeyFrame::KeyFrame()
   : DeloneBackgroundWithTriangle()
{
   Triangles tr;
   const std::string triangle = tr.MakeBasicTriangle();
   m_textName = "KeyFrame";
}