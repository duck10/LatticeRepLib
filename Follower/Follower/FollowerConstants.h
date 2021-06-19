#if _MSC_VER
#pragma once
#endif

#ifndef FOLLOWERCONSTANTS_H
#define FOLLOWERCONSTANTS_H

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "G6.h"

namespace FollowerConstants {

   enum enumFollowerMode{ globalSinglePoint, globalLine, globalChord, globalChord3, globalTriangle
   };

   extern enumFollowerMode   globalFollowerMode;
   extern bool   globalPrintAllDistanceData;
   extern bool   globalOutputGlitchesOnly;
          
   extern double globalFractionalAmountToPerturb;
   extern double globalPercentChangeToDetect; // GLITCH DETECTION LEVEL - fraction of deltas in either of the top two histogram bins
   extern double globalFractionToDetermineCloseToBoundary;

   extern std::vector<std::string> globalDistanceEnableList;
   extern std::string globalDistanceDisable;

   extern size_t    globalStepsPerFrame;
   extern size_t    globalNumberOfTrialsToAttempt;  // applies when not in movie generation
   extern bool   globalPlotAllSegmentsAsBlack;

   extern std::string globalFileNamePrefix;

   extern bool NiggliSellingFirst;

   static bool IsEnabled(const std::string& s){
      return std::find(globalDistanceEnableList.begin(), globalDistanceEnableList.end(), s) != globalDistanceEnableList.end(); 
   }
};

namespace SVG_WriterConstants {
   extern double globalGraphSpace;  // px
   extern double globalGraphBorder; // px
   extern double globalPlotSpace;
};

namespace SVG_DistancePlotConstants {
   extern int globalDataAxisWidth       ; // px
   extern int globalX_AxisTicMarkLength ; // px
   extern int globalY_AxisTicMarkLength ; // px
};

namespace GLOBAL_Report {
   extern std::string globalDataReport;
};

namespace GLOBAL_RunInputVector {
   extern bool globalConstantRandomSeed;
   extern int globalInputRandomSeed;
};

namespace GLOBAL_Files {
   extern size_t globalCounter;
   extern bool globalShouldTimeStamp;
}

#endif
