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

   enum enumMovieMode{ globalStar, globalTrail };
   enum enumFollowerMode{ globalFollow, globalMovie, globalWeb };
   //These are which G6 components to put into the SVG file !!!!!!!!!!!!!!!!
   extern std::pair<int,int> globalWhichComponentsToPlot;  // Here they are ZERO-BASED !!!!!!!

   extern enumFollowerMode   globalFollowerMode;
   extern bool   globalDrawCirclePlot;
   extern bool   globalDrawDistancePlot;
   extern bool   globalPrintAllDistanceData;
   extern bool   globalOutputGlitchesOnly;
          
   extern double globalFractionalAmountToPerturb;
   extern double globalAboveThisValueIsBad; // GLITCH DETECTION LEVEL - fraction of deltas in either of the top two histogram bins
   extern double globalFractionToDetermineCloseToBoundary;
   extern double globalMovieMaxDistRejectionTest;


   extern size_t    globalFramesPerSegment;
   extern size_t    globalStepsPerFrame;
   extern size_t    globalNumberOfTrialsToAttempt;  // applies when not in movie generation
   extern bool   globalPlotAllSegmentsAsBlack;

   extern std::vector<std::string> globalColorsForBoundaries;

   extern std::string globalFileNamePrefix;
   
   extern enumMovieMode globalMovieMode;
};

namespace SVG_WriterConstants {
   extern double globalGraphSpace;  // px
   extern double globalGraphBorder; // px
   extern double globalPlotSpace;
};

namespace SVG_CirclePlotConstants {
   extern int globalCirclePlotSize    ; // px
   extern int globalCircleRadius      ; // px
   extern int globalCircleStrokeWidth ; // px
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

#endif
