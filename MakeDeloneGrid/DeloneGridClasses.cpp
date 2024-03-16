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
   else if (
      t.m_textName == "1" || 
      t.m_textName == "2" || 
      t.m_textName == "3" || 
      t.m_textName == "4" || 
      t.m_textName == "5") {

      if ( t.m_textName == "5")
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:75px\" x = \"28\" y = \"76\" >" << t.m_textName << "</text>\n";
      else
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:75px\" x = \"28\" y = \"65\" >" << t.m_textName << "</text>\n";

      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:13px\" x = \"7\" y = \"76\" >";
      o << t.m_polyhedronName1;
      o << "</text>   \n";
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:13px\" x = \"7\" y = \"92\" >";
      o << t.m_polyhedronName2;
      o << "</text>   \n";
   }
   else {
      // this is intended ONLY for Letter_?, where ? is in CTROMHA
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:75px\" x = \"28\" y = \"75\" >" << t.m_textName << "</text>\n";
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:13px\" x = \"5\" y = \"96\" >" << t.m_description << "</text>\n";
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

Digit_1::Digit_1() {
   m_textName = "1";
   m_polyhedronName1 = "truncated";
   m_polyhedronName2 = "octahedron";
}

Digit_2::Digit_2() {
   m_textName = "2";
   m_polyhedronName1 = "elongated";
   m_polyhedronName2 = "dodecahedron";
}

Digit_3::Digit_3() {
   m_textName = "3";
   m_polyhedronName1 = "truncated";
   m_polyhedronName2 = "octahedron";
}

Digit_4::Digit_4() {
   m_textName = "4";
   m_polyhedronName1 = "hexagonal";
   m_polyhedronName2 = "prism";
}

Digit_5::Digit_5() {
   m_textName = "5";
   m_polyhedronName1 = "";
   m_polyhedronName2 = "cuboid";
}

LatticeType1::LatticeType1() {
   this->m_description = "A1";

   m_polyhedron =
      BasicDeloneRectangle().MakeSVG() + "\n"
      "<g transform = \"translate(35, 33)  scale(.5,.5)\" >\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"27.0046, -46.2506  -52.7479, 6.34286  -23.7111, 5.78468  -8.52081, 1.9355  71.2317, -50.658  42.1949, -50.0998  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"99.8762, -27.4417  81.47, -46.2506  1.71751, 6.34286  14.5893, 87.4417  32.9954, 106.251  112.748, 53.6571  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-21.47, 106.251  -11.2317, 110.658  17.8051, 110.1  -0.601001, 91.2909  -10.8393, 86.8835  -39.8762, 87.4417  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"71.2317, -50.658  -8.52081, 1.9355  1.71751, 6.34286  81.47, -46.2506  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-52.7479, 6.34286  -39.8762, 87.4417  -10.8393, 86.8835  -23.7111, 5.78468  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"17.8051, 110.1  32.9954, 106.251  14.5893, 87.4417  -0.601001, 91.2909  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-23.7111, 5.78468  -10.8393, 86.8835  -0.601001, 91.2909  14.5893, 87.4417  1.71751, 6.34286  -8.52081, 1.9355  \" stroke-linejoin = \"round\" />\n"
      "<text style = \"font-family:sans-serif; font-weight:bold; font-size:15px\" x = \"0\" y = \"129\" >aP, type 1 </text>\n"
      "<text style = \"font-family:sans-serif; font-weight:bold; font-size:30px\" x = \"-60\" y = \"-30\" >A1</text>\n\""
      "</g>\n";
}

LatticeType2::LatticeType2() {
   m_polyhedron =
      BasicDeloneRectangle().MakeSVG() + "\n"
      "<g transform = \"translate(35, 33)  scale(.5,.5)\" >\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"67.369, -1.1095  68.2535, 60.0399  79.8088, 101.964  102.485, 61.1095  101.6, -0.0399133  90.0447, -41.9638  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-42.4846, -1.1095  7.10675, 8.50327  67.369, -1.1095  90.0447, -41.9638  40.4534, -51.5766  -19.8088, -41.9638  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-41.6001, 60.0399  -30.0447, 101.964  19.5466, 111.577  7.9912, 69.6527  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"7.9912, 69.6527  19.5466, 111.577  79.8088, 101.964  68.2535, 60.0399  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-42.4846, -1.1095  -41.6001, 60.0399  7.9912, 69.6527  7.10675, 8.50327  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"7.9912, 69.6527  68.2535, 60.0399  67.369, -1.1095  7.10675, 8.50327  \" stroke-linejoin = \"round\" />\n"
      "<text style = \"font-family:sans-serif; font-weight:bold; font-size:15px\" x = \"0\" y = \"130\" >tI, type 2 </text>\n"
      "<text style = \"font-family:sans-serif; font-weight:bold; font-size:30px\" x = \"-60\" y = \"-30\" >T2</text>\n\""
      "</g>\n";
}

LatticeType3::LatticeType3() {
   m_polyhedron =
      BasicDeloneRectangle().MakeSVG() + "\n"
      "<g transform = \"translate(35, 33)  scale(.5,.5)\" >\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\"  points = \"51.1177, 76.6069  104.739, 53.3034  85.6502, 87.6947  32.0292, 110.998  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-25.6502, -27.6947  -44.7387, 6.69656  8.88226, -16.6069  27.9708, -50.9982  \" stroke-linejoin = \"round\" />\n "
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"102.709, -27.6947  83.621, 6.69656  85.6502, 87.6947  104.739, 53.3034  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-42.7095, 87.6947  32.0292, 110.998  85.6502, 87.6947  10.9115, 64.3913  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"27.9708, -50.9982  8.88226, -16.6069  83.621, 6.69656  102.709, -27.6947  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-42.7095, 87.6947  10.9115, 64.3913  8.88226, -16.6069  -44.7387, 6.69656  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"8.88226, -16.6069  10.9115, 64.3913  85.6502, 87.6947  83.621, 6.69656  \" stroke-linejoin = \"round\" />\n"
      "<text style = \"font-family:sans-serif; font-weight:bold; font-size:15px\" x = \"0\" y = \"129\" >cF, type 3 </text>\n"
      "<text style = \"font-family:sans-serif; font-weight:bold; font-size:30px\" x = \"-60\" y = \"-30\" >C3</text>\n\""
      "</g>\n";
}

LatticeType4::LatticeType4() {
   m_polyhedron =
      BasicDeloneRectangle().MakeSVG() + "\n"
      "<g transform = \"translate(35, 33)  scale(.5,.5)\" >\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"1.09903, -19.5901  -4.22921, 66.5386  45.8495, 116.129  101.256, 79.5901  106.585, -6.53857  56.506, -56.1286  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-41.2564, -19.5901  1.09903, -19.5901  56.506, -56.1286  14.1505, -56.1286  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-46.5846, 66.5386  3.49405, 116.129  45.8495, 116.129  -4.22921, 66.5386  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-41.2564, -19.5901  -46.5846, 66.5386  -4.22921, 66.5386  1.09903, -19.5901  \" stroke-linejoin = \"round\" />\n"
      "<text style = \"font-family:sans-serif; font-weight:bold; font-size:15px\" x = \"0\" y = \"129\" >hP, type 4 </text>\n"
      "<text style = \"font-family:sans-serif; font-weight:bold; font-size:30px\" x = \"-60\" y = \"-30\" >H4</text>\n\""
      "</g>\n";
}

LatticeType5::LatticeType5() {
   m_polyhedron =
      BasicDeloneRectangle().MakeSVG() + "\n"
      "<g transform = \"translate(35, 33)  scale(.5,.5)\" >\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-54.4043, 12.1906  -5.42519, 12.1906  83.3551, -46.3563  34.376, -46.3563  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-5.42519, 12.1906  25.624, 106.356  114.404, 47.8094  83.3551, -46.3563  \" stroke-linejoin = \"round\" />\n"
      "<polygon fill = \"CYAN \" stroke = \"black\" stroke-width = \"2\" points = \"-54.4043, 12.1906  -23.3551, 106.356  25.624, 106.356  -5.42519, 12.1906  \" stroke-linejoin = \"round\" />\n"
      "<text style = \"font-family:sans-serif; font-weight:bold; font-size:15px\" x = \"0\" y = \"129\" >oP, type 5 </text>\n"
      "<text style = \"font-family:sans-serif; font-weight:bold; font-size:30px\" x = \"-60\" y = \"-30\" >O5</text>\n\""
      "</g>\n";
}

std::ostringstream& operator<< (std::ostringstream& o, const LatticeType2& lt) {
   o << OutputRectangle(o, lt).str();
   o << lt.m_polyhedron << "\n";
   return o;
}
std::ostringstream& operator<< (std::ostringstream& o, const LatticeType3& lt) {
   o << OutputRectangle(o, lt).str();
   o << lt.m_polyhedron << "\n";
   return o;
}
std::ostringstream& operator<< (std::ostringstream& o, const LatticeType4& lt) {
   o << OutputRectangle(o, lt).str();
   o << lt.m_polyhedron << "\n";
   return o;
}
std::ostringstream& operator<< (std::ostringstream& o, const LatticeType5& lt) {
   o << OutputRectangle(o, lt).str();
   o << lt.m_polyhedron << "\n";
   return o;
}


std::ostringstream& operator<< (std::ostringstream& o, const LatticeType1a& lt) {
   o << OutputRectangle(o, lt).str();
   o << lt.m_polyhedron << "\n";
   return o;
}
std::ostringstream& operator<< (std::ostringstream& o, const LatticeType2a& lt) {
   o << OutputRectangle(o, lt).str();
   o << lt.m_polyhedron << "\n";
   return o;
}
std::ostringstream& operator<< (std::ostringstream& o, const LatticeType3a& lt) {
   o << OutputRectangle(o, lt).str();
   o << lt.m_polyhedron << "\n";
   return o;
}
std::ostringstream& operator<< (std::ostringstream& o, const LatticeType4a& lt) {
   o << OutputRectangle(o, lt).str();
   o << lt.m_polyhedron << "\n";
   return o;
}
std::ostringstream& operator<< (std::ostringstream& o, const LatticeType5a& lt) {
   o << OutputRectangle(o, lt).str();
   o << lt.m_polyhedron << "\n";
   return o;
}

LatticeType1a::LatticeType1a() {
   m_polyhedron =
      BasicDeloneRectangle().MakeSVG() + "\n"
      "<!-- this is the problem -->\n"
     "<g transform = \"translate(33, 35) scale( .5, .5)\">\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"39.8407, 5.8924  91.5418, 8.76279  87.2362, 65.5892  35.5352, 62.7188  \"/>\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"-45.3988, -2.71878  -0.626247, -26.8264  39.8407, 5.8924  35.5352, 62.7188  -9.23742, 86.8264  -49.7044, 54.1076  \"/>\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"-0.626247, -26.8264  10.6078, -56.6748  62.3089, -53.8044  102.776, -21.0856  91.5418, 8.76279  39.8407, 5.8924  \"/>\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"-9.23742, 86.8264  35.5352, 62.7188  87.2362, 65.5892  94.1647, 92.5672  49.3922, 116.675  -2.30892, 113.804  \"/>\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"-45.3988, -2.71878  -34.1647, -32.5672  10.6078, -56.6748  -0.626247, -26.8264  \"/>\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"-49.7044, 54.1076  -9.23742, 86.8264  -2.30892, 113.804  -42.7759, 81.0856  \"/>\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"91.5418, 8.76279  102.776, -21.0856  109.704, 5.8924  105.399, 62.7188  94.1647, 92.5672  87.2362, 65.5892  \"/>\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"-45.3988, -2.71878  -49.7044, 54.1076  -42.7759, 81.0856  -31.5418, 51.2372  -27.2362, -5.58917  -34.1647, -32.5672  \"/>\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"62.3089, -53.8044  69.2374, -26.8264  109.704, 5.8924  102.776, -21.0856  \"/>\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"49.3922, 116.675  94.1647, 92.5672  105.399, 62.7188  60.6262, 86.8264  \"/>\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"-34.1647, -32.5672  -27.2362, -5.58917  24.4648, -2.71878  69.2374, -26.8264  62.3089, -53.8044  10.6078, -56.6748  \"/>\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"-42.7759, 81.0856  -2.30892, 113.804  49.3922, 116.675  60.6262, 86.8264  20.1593, 54.1076  -31.5418, 51.2372  \"/>\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"24.4648, -2.71878  20.1593, 54.1076  60.6262, 86.8264  105.399, 62.7188  109.704, 5.8924  69.2374, -26.8264  \"/>\n"
     "  <polygon fill=\"CYAN \" stroke=\"black\" stroke-width=\"2\" points=\"-27.2362, -5.58917  -31.5418, 51.2372  20.1593, 54.1076  24.4648, -2.71878  \"/>\n"
     "<text style=\"font-family:sans-serif; font-weight:bold; font-size:15px\" x=\"0\" y=\"129\" >cI, type 1 </text>\n"
     "<text style=\"font-family:sans-serif; font-weight:bold; font-size:30px\" x=\"-60\" y=\"-30\" >C1</text>\n"
     "</g>\n";
}
LatticeType2a::LatticeType2a() {
   m_polyhedron =
      BasicDeloneRectangle().MakeSVG() + "\n"
      "<!--type 2 -->\n"
      "<g transform = \"translate(33, 35) scale( .5, .5)\">\n"
      "<polygon fill=\"CYAN\" stroke=\"black\" stroke-width=\"2\" points=\"-38.3615, -8.21251  -10.4472, -15.1751  36.879, -59.3123  8.96479, -52.3497 \"/>\n"
      "<polygon fill=\"CYAN\" stroke=\"black\" stroke-width=\"2\" points=\"25.1376, -8.21251  21.8013, 59.2784  51.0352, 112.35  98.3615, 68.2125  101.698, 0.7216  72.4639, -52.3497 \"/>\n"
      "<polygon fill=\"CYAN\" stroke=\"black\" stroke-width=\"2\" points=\"-10.4472, -15.1751  25.1376, -8.21251  72.4639, -52.3497  36.879, -59.3123 \"/>\n"
      "<polygon fill=\"CYAN\" stroke=\"black\" stroke-width=\"2\" points=\"-41.6978, 59.2784  -12.4639, 112.35  23.121, 119.312  51.0352, 112.35  21.8013, 59.2784  -13.7835, 52.3158 \"/>\n"
      "<polygon fill=\"CYAN\" stroke=\"black\" stroke-width=\"2\" points=\"-38.3615, -8.21251  -41.6978, 59.2784  -13.7835, 52.3158  -10.4472, -15.1751 \"/>\n"
      "<polygon fill=\"CYAN\" stroke=\"black\" stroke-width=\"2\" points=\"-10.4472, -15.1751  -13.7835, 52.3158  21.8013, 59.2784  25.1376, -8.21251 \"/>\n"
      "<text style=\"font-family:sans-serif; font-weight:bold; font-size:15px\" x=\"0\" y=\"129\" >mC, type 2 </text>\n"
      "<text style=\"font-family:sans-serif; font-weight:bold; font-size:30px\" x=\"-60\" y=\"-30\" >M2a</text>\n"
      "</g>\n";
}
LatticeType3a::LatticeType3a() {
   m_polyhedron =
      BasicDeloneRectangle().MakeSVG() + "\n"
      "<g transform = \"translate(33, 35) scale( .5, .5)\">\n"
      "<polygon fill=\"cyan\" stroke=\"black\" stroke-width=\"2\" points=\"7.57635, -32.9103  -52.2116, 22.8489  -9.83738, 1.73758  49.9506, -54.0216 \"/>\n"
      "<polygon fill=\"cyan\" stroke=\"black\" stroke-width=\"2\" points=\"103.864, -32.9103  44.0758, 22.8489  52.4237, 92.9103  112.212, 37.1511    \"/>\n"
      "<polygon fill=\"cyan\" stroke=\"black\" stroke-width=\"2\" points=\"49.9506, -54.0216  -9.83738, 1.73758  44.0758, 22.8489  103.864, -32.9103  \"/>\n"
      "<polygon fill=\"cyan\" stroke=\"black\" stroke-width=\"2\" points=\"-43.8638, 92.9103  10.0494, 114.022  52.4237, 92.9103  -1.4895, 71.7989    \"/>\n"
      "<polygon fill=\"cyan\" stroke=\"black\" stroke-width=\"2\" points=\"-43.8638, 92.9103  -1.4895, 71.7989  -9.83738, 1.73758  -52.2116, 22.8489  \"/>\n"
      "<polygon fill=\"cyan\" stroke=\"black\" stroke-width=\"2\" points=\"-9.83738, 1.73758  -1.4895, 71.7989  52.4237, 92.9103  44.0758, 22.8489    \"/>\n"
      "<text style=\"font-family:sans-serif; font-weight:bold; font-size:15px\" x=\"0\" y=\"129\" >oI, type 3 </text>\n"
      "<text style=\"font-family:sans-serif; font-weight:bold; font-size:30px\" x=\"-60\" y=\"-30\" >O3</text>\n"
      "</g>\n";
}
LatticeType4a::LatticeType4a() {
   m_polyhedron =
      BasicDeloneRectangle().MakeSVG() + "\n"
      "  <g transform = \"translate(35, 33) scale(.5 .5)\" >\n"
      "  <polygon fill=\"cyan\" stroke=\"black\" stroke-width=\"2\" points=\"-26.7141, 97.9372  111.526, 23.5026  120, 8.14268  -18.2397, 82.5773  \" />                                       \n"
      "  <polygon fill=\"cyan\" stroke=\"black\" stroke-width=\"2\" points=\"-51.5256, 36.4974  -30.6455, 51.8573  107.594, -22.5773  86.7141, -37.9372  \" />                                 \n"
      "  <polygon fill=\"cyan\" stroke=\"black\" stroke-width=\"2\" points=\"-30.6455, 51.8573  -18.2397, 82.5773  120, 8.14268  107.594, -22.5773  \" />                                      \n"
      "  <polygon fill=\"cyan\" stroke=\"black\" stroke-width=\"2\" points=\"-60, 51.8573  -47.5942, 82.5773  -26.7141, 97.9372  -18.2397, 82.5773  -30.6455, 51.8573  -51.5256, 36.4974  \" />\n"
      "<text style=\"font-family:sans-serif; font-weight:bold; font-size:15px\" x=\"0\" y=\"129\" >hP, type 4 </text>\n"
      "<text style=\"font-family:sans-serif; font-weight:bold; font-size:30px\" x=\"-60\" y=\"-30\" >H4</text>\n"
      "</g>\n";
}
LatticeType5a::LatticeType5a() {
   m_polyhedron =
      BasicDeloneRectangle().MakeSVG() + "\n"
     "<g transform=\"translate(35, 33) scale(.5 .5)\" >"
     "<polygon fill=\"cyan\" stroke=\"black\" stroke-width=\"2\" points=\"-60, 3.63106  -5.48374, 3.63106  80.0943, -42.4481  25.578, -42.4481  \" />"
     "<polygon fill=\"cyan\" stroke=\"black\" stroke-width=\"2\" points=\"-60, 3.63106  -20.0943, 102.448  34.422, 102.448  -5.48374, 3.63106  \" />"
     "<polygon fill=\"cyan\" stroke=\"black\" stroke-width=\"2\" points=\"-5.48374, 3.63106  34.422, 102.448  120, 56.3689  80.0943, -42.4481  \" />"
      "<text style=\"font-family:sans-serif; font-weight:bold; font-size:15px\" x=\"0\" y=\"129\" >cP, type 5 </text>\n"
      "<text style=\"font-family:sans-serif; font-weight:bold; font-size:30px\" x=\"-60\" y=\"-30\" >C5</text>\n"
      "</g>\n";
}
