
#include <string>
#include <utility>
#include <vector>

#include "GlobalTriangleFollowerConstants.h"
#include <cfloat>

namespace {
   // cannot access outside this file
   std::string BASIC_COLORS[] = { "red", "lightblue", "turquoise", "slategrey", 
                     "orange", "blueviolet", "coral", "saddlebrown", "blue", "pink", "violet", 
                     "deeppink", "mediumvioletred", "tomato", "greenyellow", "olive" };
};

   GlobalConstants::enumMovieMode GlobalConstants::globalMovieMode = GlobalConstants::globalStar;
   GlobalConstants::enumFollowerMode GlobalConstants::globalFollowerMode = GlobalConstants::globalFollow;

   //These are which G6 components to put into the SVG file !!!!!!!!!!!!!!!!
   std::pair<int,int> GlobalConstants::globalWhichComponentsToPlot = std::make_pair( 3,4 );  // Here they are ZERO-BASED !!!!!!!

   bool GlobalConstants::globalDrawCirclePlot       = true;
   bool GlobalConstants::globalDrawDistancePlot     = true;
   bool GlobalConstants::globalPrintAllDistanceData = true;
   bool GlobalConstants::globalOutputGlitchesOnly   = false;

   double GlobalConstants::globalFractionalAmountToPerturb          = 0.05;
   double GlobalConstants::globalPercentChangeToDetect                = -DBL_MAX; // GLITCH DETECTION LEVEL - fraction of deltas in either of the top two histogram bins
   double GlobalConstants::globalFractionToDetermineCloseToBoundary = 0.065;
   double GlobalConstants::globalMovieMaxDistRejectionTest          = 0.1;
   
   unsigned long GlobalConstants::globalFramesPerSegment        = 200;
   unsigned long GlobalConstants::globalStepsPerFrame           = 100;
   unsigned long GlobalConstants::globalNumberOfTrialsToAttempt = 20;  // applies when not in movie generation
   bool GlobalConstants::globalPlotAllSegmentsAsBlack  = false;

   std::vector<std::string> GlobalConstants::globalColorsForBoundaries = std::vector<std::string>( BASIC_COLORS, BASIC_COLORS + sizeof(BASIC_COLORS)/sizeof(BASIC_COLORS[0]) );
   std::string GlobalConstants::globalFileNamePrefix   = "TF";


   double SVG_WriterConstants::globalGraphSpace  = 800; // px
   double SVG_WriterConstants::globalGraphBorder = 140; // px
   double SVG_WriterConstants::globalPlotSpace   = SVG_WriterConstants::globalGraphSpace - 2*SVG_WriterConstants::globalGraphBorder;


   int SVG_CirclePlotConstants::globalCirclePlotSize    = 2000; // px
   int SVG_CirclePlotConstants::globalCircleRadius      = 300; // px
   int SVG_CirclePlotConstants::globalCircleStrokeWidth = 30; // px


   int SVG_DistancePlotConstants::globalG6DataLineStrokeWidth = 3; // px
   int SVG_DistancePlotConstants::globalDeloneDataLineStrokeWidth = 10; // px
   int SVG_DistancePlotConstants::globalDataAxisWidth       = 6; // px
   int SVG_DistancePlotConstants::globalY_AxisTicMarkLength = 20; // px
   int SVG_DistancePlotConstants::globalX_AxisTicMarkLength = SVG_DistancePlotConstants::globalY_AxisTicMarkLength; // px


   std::string GLOBAL_Report::globalDataReport = "";

   std::vector<G6> GLOBAL_RunInputVector::globalData;

   bool GLOBAL_RunInputVector::globalConstantRandomSeed = false;
   int GLOBAL_RunInputVector::globalInputRandomSeed = 19;

   unsigned long GLOBAL_FileNames::globalCounter = 0;
   bool GLOBAL_FileNames::globalShouldTimeStamp = true;
