#ifndef SVG_TOOLS_H
#define SVG_TOOLS_H

#include "LRL_ToString.h"
#include <vector>

enum eDataType { eDeltaDist23, eAngleDiff, eTriangleViolation, eArea, etanhDeltaDist23, edist12, edist13, edist23, enormedDist23, eLegend };


std::vector< std::string> ScaleSubPicture(const double xscale, const double yscale, const std::vector<std::string>& s);
std::vector< std::string> InvertSubPicture(const std::vector<std::string>& s);
std::vector< std::string> PlaceSubPicture(const int x, const int y, const std::vector<std::string>& s);

std::vector< std::string> ScaleSubPicture(const int xscale, const int yscale, const std::string& s);
std::vector< std::string> InvertSubPicture(const std::string& s);
std::vector< std::string> PlaceSubPicture(const int x, const int y, const std::string& s);
std::string ImageHeader(const int imageWidth, const int imageHeight);
std::string ImageHeader(const std::string& imageWidth, const std::string& imageHeight);
std::vector<std::string> ImageFooter(const std::vector<std::string>& vs);
std::vector<std::string> ImageFooter(const std::string& s);

std::string DrawRectangle(const double x, const double y, const double width, const double height);
 std::string EncloseAsComment(const std::string& s);

std::string DrawRectangle(const double x, const double y, const double width, const double height);
std::string EncloseAsComment(const std::string& s);

#endif
