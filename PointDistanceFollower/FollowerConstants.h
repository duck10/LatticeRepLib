#if _MSC_VER
#pragma once
#endif

#ifndef FOLLOWERCONSTANTS_H
#define FOLLOWERCONSTANTS_H

#include <string>
#include <utility>
#include <vector>

#include "G6.h"

namespace FollowerConstants {

   enum enumFollowerMode{ globalSinglePoint, globalLine, globalLine3 };
   //These are which G6 components to put into the SVG file !!!!!!!!!!!!!!!!
   extern std::pair<int,int> globalWhichComponentsToPlot;  // Here they are ZERO-BASED !!!!!!!

   extern enumFollowerMode   globalFollowerMode;
   extern bool   globalPrintAllDistanceData;
   extern bool   globalOutputGlitchesOnly;
          
   extern double globalFractionalAmountToPerturb;
   extern double globalPercentChangeToDetect; // GLITCH DETECTION LEVEL - fraction of deltas in either of the top two histogram bins
   extern double globalFractionToDetermineCloseToBoundary;
   extern double globalMovieMaxDistRejectionTest;

   extern std::vector<std::string> globalDistanceEnableList;
   extern std::string globalDistanceDisable;

   extern unsigned long    globalFramesPerSegment;
   extern unsigned long    globalStepsPerFrame;
   extern unsigned long    globalNumberOfTrialsToAttempt;  // applies when not in movie generation
   extern bool   globalPlotAllSegmentsAsBlack;

   extern std::vector<std::string> globalColorsForBoundaries;

   extern std::string globalFileNamePrefix;

   static bool IsEnabled(const std::string& s) {return std::find(globalDistanceEnableList.begin(), globalDistanceEnableList.end(), s) != globalDistanceEnableList.end(); }
};

namespace SVG_WriterConstants {
   extern double globalGraphSpace;  // px
   extern double globalGraphBorder; // px
   extern double globalPlotSpace;
};

namespace SVG_DistancePlotConstants {
   extern int globalG6DataLineStrokeWidth; // px
   extern int globalDeloneDataLineStrokeWidth; // px
   extern int globalDataAxisWidth       ; // px
   extern int globalX_AxisTicMarkLength ; // px
   extern int globalY_AxisTicMarkLength ; // px
};

namespace GLOBAL_Report {
   extern std::string globalDataReport;
};

namespace GLOBAL_RunInputVector {
   extern std::vector<G6> globalData;
   extern bool globalConstantRandomSeed;
   extern int globalInputRandomSeed;
};

namespace GLOBAL_Files {
   extern unsigned long globalCounter;
   extern bool globalShouldTimeStamp;
}

#endif
