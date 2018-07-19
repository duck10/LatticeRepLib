#pragma once

#ifndef FOLLOWERTOOLS_H
#define FOLLOWERTOOLS_H

#include <algorithm>
#include <sstream>
#include <string>
#include <utility>

#include "D7.h"
#include "G6.h"
#include "ProjectorTools.h"
#include "triple.h"

std::pair<int,std::string>  IdentifyNearbyBoundaries(const double v[6], const double cutoff);
std::pair<int, std::string>  IdentifyNearbyBoundaries(const G6& g, const double cutoff);
std::pair<int, std::string>  D7_IdentifyNearbyBoundaries(const D7& d, const double cutoff);

namespace FollowerTools {
   void OpenOutputFile( std::ofstream& svgOut, const std::string& sFileName );
};
#endif
