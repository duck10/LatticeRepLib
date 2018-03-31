#include <fstream>

#include "LRL_CreateFileName.h"
#include "FileOperations.h"
#include "FollowerTools.h"
#include "GlobalTriangleFollowerConstants.h"
#include "LRL_ToString.h"
#include "SVG_Tools.h"

//#include "stdafx.h"

std::vector< std::string> ScaleSubPicture(const double xscale, const double yscale, const std::vector< std::string>& s) {
   std::vector< std::string> svg;
   svg.push_back("<g transform=\"scale(" + LRL_ToString(xscale) + "," + LRL_ToString(yscale) + ") \">");
   svg.insert(svg.end(), s.begin(), s.end());
   svg.push_back("</g>");

   return svg;
}

std::vector< std::string> InvertSubPicture(const std::vector< std::string>& s) {
   std::vector< std::string> svg;
   svg.push_back("<g transform=\" scale(1 -1)\">");
   svg.insert(svg.end(), s.begin(), s.end());
   svg.push_back("</g>");

   return svg;
}
std::vector< std::string> InvertSubPicture(const std::string& s) {
   std::vector< std::string> svg;
   svg.push_back("<g transform=\" scale(1 -1)\">");
   svg.push_back(s);
   svg.push_back("</g>");

   return svg;
}

std::vector< std::string> PlaceSubPicture(const int x, const int y, const std::vector< std::string>& s) {
   std::vector< std::string> svg;
   svg.push_back("<g transform=\"translate(" + LRL_ToString(x) + "," + LRL_ToString(y) + ") scale(1 1)\" >");
   svg.insert(svg.end(), s.begin(), s.end());
   svg.push_back("</g>");

   return svg;
}

std::vector< std::string> ScaleSubPicture(const int xscale, const int yscale, const std::string& s) {
   std::vector< std::string> svg;
   svg.push_back("<g transform=\"scale(" + LRL_ToString(xscale) + "," + LRL_ToString(yscale) + ") \">");
   svg.push_back(s);
   svg.push_back("</g>");

   return svg;
}

std::vector< std::string> PlaceSubPicture(const int x, const int y, const std::string& s) {
   std::vector< std::string> svg;
   svg.push_back("<g transform=\"translate(" + LRL_ToString(x) + "," + LRL_ToString(y) + ") scale(1 1)\" >");
   svg.push_back(s);
   svg.push_back("</g>"); // this is a  test

   return svg;
}

std::string DrawRectangle(const double x, const double y, const double width, const double height)
{
   return "<rect x =" + ToQuotedString(x) + " y =" + ToQuotedString(y) +
      " width =" + ToQuotedString(width) + " height =" + ToQuotedString(height) +
      " style =" + ToQuotedString("fill:whitesmoke; stroke:black; stroke-width:5; ") + " />";
}

std::string ImageHeader(const int imageWidth, const int imageHeight)
{
   return
      std::string("<?xml version=\"1.0\" standalone=\"no\"?>\n")
      + "<svg width=" + ToQuotedString(imageWidth)
      + " height =" + ToQuotedString(imageHeight)
      + " viewBox = \"1 0 " + LRL_ToString(imageWidth) + " " + LRL_ToString(imageHeight) +
      +"\" \nversion = \"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
      + LRL_CreateFileName::Create( "<!-- Creation date and time ", "-->")
      ;
}

std::vector<std::string> ImageFooter( const std::vector<std::string>& vs )
{
   std::vector<std::string> vss;
   vss.push_back( std::string("<!--\n") );
   vss.insert( vss.end(), vs.begin(), vs.end() );
   vss.push_back( "-->" );
   vss.push_back("<!-- Run Constants");
   vss.push_back(GLOBAL_Report::globalDataReport);
   vss.push_back("-->");
   vss.push_back( "</svg>" );
   return vss;
}

std::string EncloseAsComment(const std::string& s) {
   return std::string("\n<!--\n") + s + std::string("\n-->\n");
}