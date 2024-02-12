#include "DeloneGridClasses.h"

#include <sstream>

BasicDeloneRectangle::BasicDeloneRectangle() {}

std::string BasicDeloneRectangle::MakeSVG() {
   const static std::string rect = "   <rect id=\"Basic\" x=\"0\" y=\"0\" width=\"102\" height=\"102\" stroke-width=\"2\" stroke=\"black\" fill=\"none\" />\n";
   return rect;
}

std::string GrayRectangle::MakeSVG() {
   const static std::string rect = "   <rect id=\"Basic\" x=\"0\" y=\"0\" width=\"102\" height=\"102\" stroke-width=\"2\" stroke=\"black\" fill=\"gray\" />\n";
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
   o << BasicDeloneRectangle().MakeSVG();
   if ( t.m_textName.empty()) 
   {
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:30px\" x = \"5\" y = \"30\" >" << t.m_deloneName << "</text>\n";
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:16px\" x = \"5\" y = \"95\" >" << t.m_bravaisType << "</text>\n";
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:16px\" x = \"32\" y = \"95\" >" << t.m_character << "</text>\n";
   }
   else {
      o << "<text style = \"font-family:sans-serif; font-weight:bold; font-size:75px\" x = \"28\" y = \"75\" >" << t.m_textName << "</text>\n";
   }
   return o;
}

std::ostringstream& operator<< (std::ostringstream& o, const C1& c1) {
   return OutputRectangle(o, c1);
}

std::ostringstream& operator<< (std::ostringstream& o, const C3& c3) {
   return OutputRectangle(o, c3);
}

std::ostringstream& operator<< (std::ostringstream& o, const C5& c5) {
   return OutputRectangle(o, c5);
}


std::ostringstream& operator<< (std::ostringstream& o, const T1& c1) {
   return OutputRectangle(o, c1);
}

std::ostringstream& operator<< (std::ostringstream& o, const T2& c3) {
   return OutputRectangle(o, c3);
}

std::ostringstream& operator<< (std::ostringstream& o, const T5& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const R1& c3) {
   return OutputRectangle(o, c3);
}

std::ostringstream& operator<< (std::ostringstream& o, const R3& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const O1A& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const O2& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const O3& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const O4& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const O5& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const O1B& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const M1A& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const M1B& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const M2A& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const M2B& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const M3& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const M4& c5) {
   return OutputRectangle(o, c5);
}


std::ostringstream& operator<< (std::ostringstream& o, const A1& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const A2& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const A3& c5) {
   return OutputRectangle(o, c5);
}

std::ostringstream& operator<< (std::ostringstream& o, const H4& c5) {
   return OutputRectangle(o, c5);
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

std::ostringstream& operator<< (std::ostringstream& o, const BlankRectangle& br) {
   return OutputRectangle(o, br);
}

std::ostringstream& operator<< (std::ostringstream& o, const GrayRectangle& br) {
   o << GrayRectangle().MakeSVG();
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