// MakeDeloneGrid.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>

#include "DeloneGridClasses.h"
#include "MakeDeloneGrid.h"
#include "MakeTriangles.h"
#include "S6.h"
#include "SVG_Tools.h"





std::string WrapInSVGHeaderFooter(const int width, const int height, const std::string& s) {
   std::string out;
   out += ImageHeader(width, height);
   out += s;
   const std::vector<std::string> footer = ImageFooter("");
   for (const std::string& foot : footer) {
      out += foot;
   }
   return out;
}

std::string WrapInSVGHeaderFooter(const int width, const int height, const std::vector<std::string>& vs) {
   std::string s;
   for (const auto& si : vs) {
      s += si;
   }
   return WrapInSVGHeaderFooter(width, height, s);
}

std::string WrapSVGInAGroup(const int x, const int y, const std::string& s) {
   std::ostringstream o;
   o << "\n\n<!-- _________________________________________________________ -->\n";
   o << "<g transform=\"translate(" << x << ", " << y << ")\">\n";
   o << s;
   o << "</g>\n";
   return o.str();
}

std::string BottomText(const int nx, const int ny, const int delta, const int bottomStart) {
   std::ostringstream bottomText;
   bottomText << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:25px\" x = \""
      << nx - delta / 2
      << "\" y=\""
      << bottomStart
      << "\">\n"
      << "*" << " Not a full-dimensional Bravais type"
      << "</text>\n"
      << "\n";

   bottomText << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:18px\" x = \""
      << nx
      << "\" y=\""
      << bottomStart + 20
      << "\">\n"
      << "O3 is a 2-D manifold between O2 and O1B"
      << "</text>\n"
      << "\n";

   bottomText << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:18px\" x = \""
      << nx
      << "\" y=\""
      << bottomStart + 40
      << "\">\n"
      << "M3 is a 3-D manifold between M2A and M1B"
      << "</text>\n"
      << "\n";

   bottomText << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:18px\" x = \""
      << nx
      << "\" y=\""
      << bottomStart + 60
      << "\">\n"
      << "M2B is a 3-D manifold between M1A and M1B"
      << "</text>\n"
      << "\n";

   bottomText 
      << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:18px\" x = \""
      << nx - delta / 2
      << "\" y=\""
      << bottomStart + 90
      << "\">\n"
      << "[modified after Delone, Galiulin, and Shtogrin, 1975]"
      << "</text>\n"
      << "\n";

   return bottomText.str();
}

static const double rectangleTranslation = 100;

void MakeAllDeloneRows() {
   const CRow rowC;
   const TRow rowT;
   const RRow rowR;
   const O1Row rowO1;
   const O2Row rowO2;
   const M1Row rowM1;
   const M2Row rowM2;
   const ARow rowA;
   const HRow rowH;
   std::vector<std::string> vs;
   //vs.emplace_back(CRow().m_row);
}

template<typename T> 
std::ostringstream Wrapper(const int x, const int y, const T& t) {
   std::ostringstream osx;
   std::ostringstream temp;
   temp << t;
   osx << WrapSVGInAGroup(x,y, temp.str());
   return osx;
}

void MakeVerticalGrid() {
   constexpr int delta = 100;
   int nx = 0;
   int ny = 0;

   std::ostringstream num_row;
   num_row << Wrapper(delta * nx++, delta * ny, KeyFrame()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Digit_1()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Digit_2()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Digit_3()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Digit_4()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Digit_5()).str();

   nx = 0;
   ny++;

   std::ostringstream c_row;
   c_row << Wrapper(delta * nx++, delta * ny, Letter_C()).str();
   c_row << Wrapper(delta * nx++, delta * ny, C1()).str();
   c_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   c_row << Wrapper(delta * nx++, delta * ny, C3()).str();
   c_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   c_row << Wrapper(delta * nx++, delta * ny, C5()).str();

   nx = 0;
   ny++;
   std::ostringstream t_row;
   t_row << Wrapper(delta * nx++, delta * ny, Letter_T()).str();
   t_row << Wrapper(delta * nx++, delta * ny, T1()).str();
   t_row << Wrapper(delta * nx++, delta * ny, T2()).str();
   t_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   t_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   t_row << Wrapper(delta * nx++, delta * ny, T5()).str();

   nx = 0;
   ny++;
   std::ostringstream r_row;
   r_row << Wrapper(delta * nx++, delta * ny, Letter_R()).str();
   r_row << Wrapper(delta * nx++, delta * ny, R1()).str();
   r_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   r_row << Wrapper(delta * nx++, delta * ny, R3()).str();
   r_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   r_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();

   nx = 0;
   ny++;
   std::ostringstream o1_row;;
   o1_row << Wrapper(delta * nx++, delta * ny, Letter_O()).str();
   o1_row << Wrapper(delta * nx++, delta * ny, O1A()).str();
   o1_row << Wrapper(delta * nx++, delta * ny, O2()).str();
   o1_row << Wrapper(delta * nx++, delta * ny, O3()).str();
   o1_row << Wrapper(delta * nx++, delta * ny, O4()).str();
   o1_row << Wrapper(delta * nx++, delta * ny, O5()).str();

   nx = 0;
   ny++;
   std::ostringstream o2_row;;
   o2_row << Wrapper(delta * nx++, delta * ny, Letter_O()).str();
   o2_row << Wrapper(delta * nx++, delta * ny, O1B()).str();
   o2_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   o2_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   o2_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   o2_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();

   nx = 0;
   ny++;
   std::ostringstream m1_row;;
   m1_row << Wrapper(delta * nx++, delta * ny, Letter_M()).str();
   m1_row << Wrapper(delta * nx++, delta * ny, M1A()).str();
   m1_row << Wrapper(delta * nx++, delta * ny, M2A()).str();
   m1_row << Wrapper(delta * nx++, delta * ny, M3()).str();
   m1_row << Wrapper(delta * nx++, delta * ny, M4()).str();
   m1_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();

   nx = 0;
   ny++;
   std::ostringstream m2_row;;
   m2_row << Wrapper(delta * nx++, delta * ny, Letter_M()).str();
   m2_row << Wrapper(delta * nx++, delta * ny, M1B()).str();
   m2_row << Wrapper(delta * nx++, delta * ny, M2B()).str();
   m2_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   m2_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   m2_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();

   nx = 0;
   ny++;
   std::ostringstream a_row;;
   a_row << Wrapper(delta * nx++, delta * ny, Letter_A()).str();
   a_row << Wrapper(delta * nx++, delta * ny, A1()).str();
   a_row << Wrapper(delta * nx++, delta * ny, A2()).str();
   a_row << Wrapper(delta * nx++, delta * ny, A3()).str();
   a_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   a_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();

   nx = 0; ny = 9;
   std::ostringstream h_row;;
   h_row << Wrapper(delta * nx++, delta * ny, Letter_H()).str();
   h_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   h_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   h_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   h_row << Wrapper(delta * nx++, delta * ny, H4()).str();
   h_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();

   std::ostringstream bottomText;

   const int bottomStart = delta * ny + 1.5 * delta;
   nx = 1.5 * delta; ny = 11;

   const std::string newBottomText = BottomText(nx, ny, delta, bottomStart);

   std::ostringstream total;
   total << num_row.str() << c_row.str() << t_row.str() << r_row.str() << o1_row.str()
      << o2_row.str() << m1_row.str() << m2_row.str() << a_row.str() << h_row.str()
      << newBottomText << std::endl;
   std::cout << WrapInSVGHeaderFooter(650, 1200, total.str());
}

void MakeHorizontalGrid() {

   std::ostringstream num_row;
   std::ostringstream one_row ;
   std::ostringstream two_row ;
   std::ostringstream three_row ;
   std::ostringstream four_row;
   std::ostringstream five_row;

   constexpr int delta = 100;
   int nx = 0;
   int ny = 0;

   num_row << Wrapper(delta * nx++, delta * ny, KeyFrame()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Letter_C()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Letter_T()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Letter_R()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Letter_O()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Letter_O()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Letter_M()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Letter_M()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Letter_A()).str();
   num_row << Wrapper(delta * nx++, delta * ny, Letter_H()).str();

   nx = 0; ny++;
   one_row << Wrapper(delta * nx++, delta * ny, Digit_1()).str();
   one_row << Wrapper(delta * nx++, delta * ny, C1()).str();
   one_row << Wrapper(delta * nx++, delta * ny, T1()).str();
   one_row << Wrapper(delta * nx++, delta * ny, R1()).str();
   one_row << Wrapper(delta * nx++, delta * ny, O1A()).str();
   one_row << Wrapper(delta * nx++, delta * ny, O1B()).str();
   one_row << Wrapper(delta * nx++, delta * ny, M1A()).str();
   one_row << Wrapper(delta * nx++, delta * ny, M1B()).str();
   one_row << Wrapper(delta * nx++, delta * ny, A1()).str();
   one_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();

   ////std::cout << num_row.str() << std::endl;
   nx = 0; ny++;
   two_row << Wrapper(delta * nx++, delta * ny, Digit_2()).str();
   two_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   two_row << Wrapper(delta * nx++, delta * ny, T2()).str();
   two_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   two_row << Wrapper(delta * nx++, delta * ny, O2()).str();
   two_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   two_row << Wrapper(delta * nx++, delta * ny, M2A()).str();
   two_row << Wrapper(delta * nx++, delta * ny, M2B()).str();
   two_row << Wrapper(delta * nx++, delta * ny, A2()).str();
   two_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();

   nx = 0; ny++;
   three_row << Wrapper(delta * nx++, delta * ny, Digit_3()).str();
   three_row << Wrapper(delta * nx++, delta * ny, C3()).str();
   three_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   three_row << Wrapper(delta * nx++, delta * ny, R3()).str();
   three_row << Wrapper(delta * nx++, delta * ny, O3()).str();
   three_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   three_row << Wrapper(delta * nx++, delta * ny, M3()).str();
   three_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   three_row << Wrapper(delta * nx++, delta * ny, A3()).str();
   three_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();

   nx = 0; ny++;
   four_row << Wrapper(delta * nx++, delta * ny, Digit_4()).str();
   four_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   four_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   four_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   four_row << Wrapper(delta * nx++, delta * ny, O4()).str();
   four_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   four_row << Wrapper(delta * nx++, delta * ny, M4()).str();
   four_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   four_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   four_row << Wrapper(delta * nx++, delta * ny, H4()).str();

   nx = 0; ny++;
   five_row << Wrapper(delta * nx++, delta * ny, Digit_5()).str();
   five_row << Wrapper(delta * nx++, delta * ny, C5()).str();
   five_row << Wrapper(delta * nx++, delta * ny, T5()).str();
   five_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   five_row << Wrapper(delta * nx++, delta * ny, O5()).str();
   five_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   five_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   five_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   five_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();
   five_row << Wrapper(delta * nx++, delta * ny, GrayRectangle()).str();

   std::ostringstream bottomText;

   const int bottomStart = delta * ny++ + 1.5 * delta;
   nx = 1.5 * delta; 
   ny = 11;

   const std::string newBottomText = BottomText(nx, ny, delta, bottomStart);
   std::ostringstream total;
   total << num_row.str() << one_row.str() << two_row.str() << three_row.str() << four_row.str()
      << five_row.str()
      << newBottomText << std::endl;
   std::cout << WrapInSVGHeaderFooter(1100, 750, total.str());
}

int main()
{
   //MakeVerticalGrid();
   MakeHorizontalGrid();
}
