// MakeDeloneGrid.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>

#include "DeloneGridClasses.h"
#include "MakeDeloneGrid.h"
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
   o << "<g transform=\"translate(" << x << ", " << y << ")\">\n";
   o << s;
   o << "</g>\n";
   return o.str();
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
   temp << t;  osx << WrapSVGInAGroup(x,y, temp.str());
   return osx;
}

int main()
{
   const int delta = 100;
   int n1 = 0; 
   int n2 = 0;

   std::ostringstream num_row;
   num_row << Wrapper(delta *n1++, delta*  n2,GrayRectangle()).str();
   num_row <<Wrapper(delta * n1++, delta * n2, Digit_1()).str();
   num_row <<Wrapper(delta * n1++, delta * n2, Digit_2()).str();
   num_row <<Wrapper(delta * n1++, delta * n2, Digit_3()).str();
   num_row <<Wrapper(delta * n1++, delta * n2, Digit_4()).str();
   num_row <<Wrapper(delta * n1++, delta * n2, Digit_5()).str();

   //std::cout << num_row.str() << std::endl;
   n1 = 0;
   n2 = 1;

   std::ostringstream c_row;
   c_row << Wrapper(delta * n1++, delta * n2, Letter_C()).str();
   c_row << Wrapper(delta * n1++, delta * n2, C1()).str();
   c_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();
   c_row << Wrapper(delta * n1++, delta * n2, C3()).str();
   c_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();
   c_row << Wrapper(delta * n1++, delta * n2, C5()).str();

   n1 = 0;
   n2 = 2;
   std::ostringstream t_row;
   t_row << Wrapper(delta * n1++, delta * n2, Letter_T()).str();
   t_row << Wrapper(delta * n1++, delta * n2, T1()).str();
   t_row << Wrapper(delta * n1++, delta * n2, T2()).str();
   t_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();
   t_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();
   t_row << Wrapper(delta * n1++, delta * n2, T5()).str();

   n1 = 0;
   n2 = 3;
   std::ostringstream r_row;
   r_row << Wrapper(delta * n1++, delta * n2,Letter_R()).str();
   r_row << Wrapper(delta * n1++, delta * n2,R1()).str();
   r_row << Wrapper(delta * n1++, delta * n2,GrayRectangle()).str();
   r_row << Wrapper(delta * n1++, delta * n2,R3()).str();
   r_row << Wrapper(delta * n1++, delta * n2,GrayRectangle()).str();
   r_row << Wrapper(delta * n1++, delta * n2,GrayRectangle()).str();

   n1 = 0; n2 = 4;
   std::ostringstream o1_row;;
   o1_row << Wrapper(delta * n1++, delta * n2, Letter_O()).str();
   o1_row << Wrapper(delta * n1++, delta * n2, O1A()).str();
   o1_row << Wrapper(delta * n1++, delta * n2, O2()).str();
   o1_row << Wrapper(delta * n1++, delta * n2, O3()).str();
   o1_row << Wrapper(delta * n1++, delta * n2, O4()).str();
   o1_row << Wrapper(delta * n1++, delta * n2, O5()).str();

   n1 = 0; n2 = 5;
   std::ostringstream o2_row;;
   o2_row << Wrapper(delta * n1++, delta * n2, Letter_O()).str();
   o1_row << Wrapper(delta * n1++, delta * n2, O1B()).str();
   o2_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();
   o2_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();
   o2_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();
   o2_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();

   n1 = 0; n2 = 6;
   std::ostringstream m1_row;;
   m1_row << Wrapper(delta * n1++, delta * n2, Letter_M()).str();
   m1_row << Wrapper(delta * n1++, delta * n2, M1A()).str();
   m1_row << Wrapper(delta * n1++, delta * n2, M1B()).str();
   m1_row << Wrapper(delta * n1++, delta * n2, M3()).str();
   m1_row << Wrapper(delta * n1++, delta * n2, M4()).str();
   o2_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();

   n1 = 0; n2 = 7;
   std::ostringstream m2_row;;
   m1_row << Wrapper(delta * n1++, delta * n2, Letter_M()).str();
   m1_row << Wrapper(delta * n1++, delta * n2, M1B()).str();
   m1_row << Wrapper(delta * n1++, delta * n2, M2B()).str();
   o2_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();
   o2_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();
   o2_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();

   n1 = 0; n2 = 8;
   std::ostringstream a_row;;
   a_row << Wrapper(delta * n1++, delta * n2, Letter_A()).str();
   a_row << Wrapper(delta * n1++, delta * n2, A1()).str();
   a_row << Wrapper(delta * n1++, delta * n2, A2()).str();
   a_row << Wrapper(delta * n1++, delta * n2, A3()).str();
   a_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();
   a_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();

   n1 = 0; n2 = 9;
   std::ostringstream h_row;;
   a_row << Wrapper(delta * n1++, delta * n2, Letter_H()).str();
   a_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();
   a_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();
   a_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();
   h_row << Wrapper(delta * n1++, delta * n2, H4()).str();
   a_row << Wrapper(delta * n1++, delta * n2, GrayRectangle()).str();


   std::ostringstream total;
   total << num_row.str() << c_row.str() << t_row.str() << r_row.str() <<o1_row.str()
      << o2_row.str() << m1_row.str() << m2_row.str() << a_row.str() << h_row.str() << std::endl;
   std::cout <<  WrapInSVGHeaderFooter(1000,1000, total.str());
}
