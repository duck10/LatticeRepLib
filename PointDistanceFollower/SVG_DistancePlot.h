#if _MSC_VER
#pragma once
#endif

#ifndef SVG_DISTANCEPLOT
#define SVG_DISTANCEPLOT

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <list>
#include <set>
#include <vector>

#include "FileOperations.h"
#include "FileWriter.h"
#include "FollowerConstants.h"
#include "LRL_DataToSVG.h"
#include "LRL_ToString.h"
#include "MultiFollower.h"
#include "SVG_Writer.h"



#include "LinearAxis.h"
bool IsPositiveOrZero(const double d) { return d >= 0.0; }
bool IsNegative(const double d) { return d < 0.0; }

bool IsGreater(const double d1, const double d2) {
   return d2>d1;
}

template<typename TVEC>
class SVG_DistancePlot
{
public:

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   SVG_DistancePlot(MultiFollower& multiFollow, const std::string& fileName)
      : m_fileName( fileName )
      /*-------------------------------------------------------------------------------------*/
   {
      const std::list<std::string> svg = DrawDistancePlot(multiFollow);
      FileWriter<TVEC>(fileName + ".txt", multiFollow).Write();
      SVG_Writer<TVEC> writer(fileName + ".svg", svg, multiFollow.GetLineDescriptions(), multiFollow.GetTime2ComputerFrame());
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> Format( ) {
      return(m_svg);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   ~SVG_DistancePlot( void ) {
   }

private:

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> FormatAllPointsAsPolyline( const std::vector<double>& distances,
      const int lineWidth, const double minimumDistance, const double xscale, const double yscale,
      const std::string& color, const std::string& dashMode)
      /*-------------------------------------------------------------------------------------*/
   {
      std::list<std::string> svg;
      if (!distances.empty()) {
         const size_t xmax(distances.size());
         const std::vector<double>::const_iterator firstNegative = std::find_if(distances.begin(), distances.end(), IsNegative);
         const size_t lastPositiveIndex = (size_t)((firstNegative == distances.end()) ? xmax : firstNegative - distances.begin());

         std::string s = LRL_ToString("   <polyline fill=\"none\" stroke=\"", color, "\" stroke-width=\"", lineWidth, "\" stroke-dasharray = \"", dashMode, "\" points=\" ");

         std::vector<double>::const_iterator it = distances.begin();
         for (size_t i = 1; i <= lastPositiveIndex; ++i, ++it)
            s += LRL_DataToSVG(double(i)*xscale, ",") +
            LRL_DataToSVG(((*it) - minimumDistance) *yscale, " ");

         s += "\" />\n";
         svg.push_back(s);
         if (lastPositiveIndex != xmax) {
            DistanceLineDescriptions colorList;
            svg.push_back(DrawInvalidLineSegment(colorList.GetColor("INVALID"), 6, double(lastPositiveIndex + 1UL)*xscale, SVG_WriterConstants::globalPlotSpace / 2.0, double(xmax)*xscale));
         }
      }
      return(svg);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> FormatEachDatumAsSeparateLineSegment( const std::vector<double>& distances,
      const int lineWidth, const double minimumDistance, const double xscale, const double yscale, const std::string& color)
   /*-------------------------------------------------------------------------------------*/
   {
      std::list<std::string> svg;
      if (!distances.empty()) {
         const std::pair<std::vector<double>::const_iterator, std::vector<double>::const_iterator> distminmax = std::minmax_element(distances.begin(), distances.end());
         LinearAxis la;
         const AxisLimits alY = la.LinearAxisLimits(*distminmax.first, *distminmax.second);
         const double distmid = (alY.m_lowerLimit + alY.m_upperLimit) / 2.0;

         const int xmax(int(distances.size()));
         std::vector<double>::const_iterator it = distances.begin();
         double yprev = ((*it) - minimumDistance)*yscale;
         for (int i = 1; i <= xmax; ++i, ++it)
         {
            const std::string bound/* = m_follow.GetBoundaryString( i - 1 )*/;
            const double yNext = ((*it) - minimumDistance)*yscale;
            if ((*it) >= 0.0)
               svg.push_back(DrawSingleLineSegment(color, lineWidth, double(i - 1)*xscale, yprev, double(i)*xscale, yNext));
            else {
               DistanceLineDescriptions colorList;
               svg.push_back(DrawInvalidLineSegment(colorList.GetColor("INVALID"), 6, double(i - 1)*xscale, SVG_WriterConstants::globalPlotSpace/2.0, double(i)*xscale));
            }
            yprev = yNext;
         }
      }

      return(svg);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> LabelsAndTicsFor_Y_Axis( const AxisLimits& alY, const double minimumDistance, const double yscale ) const
      /*-------------------------------------------------------------------------------------*/
   {
      std::list<std::string> svg;
      svg.push_back( "\n<!--Draw labels and tics for Y axis -->\n" );

      const double yDelta = (alY.m_upperLimit - alY.m_lowerLimit) / alY.m_numSteps;
      for( size_t i = 0; i < alY.m_numSteps + 1; ++i)
      {
         const double y = (alY.m_lowerLimit + double( i )*yDelta - minimumDistance) * yscale;
         svg.push_back( DrawOneY_AxisTicMark( y ) );
         svg.push_back( DrawOneY_AxisLabel( y, yscale, minimumDistance ) );
      }

      svg.push_back( "" );
      return(svg);
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> LabelsAndTicsFor_X_Axis( const AxisLimits& alX, const double minimumDistance, const double xscale, const double yscale )
      /*-------------------------------------------------------------------------------------*/
   {
      std::list<std::string> svg;
      svg.push_back( "\n<!--Draw labels and tics for X axis -->\n" );
      const double& xMinAxis = alX.m_lowerLimit;
      const double& xMaxAxis = alX.m_upperLimit;
      const size_t& xsteps = alX.m_numSteps; // does not include the starting point, just the steps
      double xPosition = xMinAxis*xscale + 0.5;
      for (size_t i = 0; i <= xsteps; ++i)
      {
         const double& y = -SVG_DistancePlotConstants::globalX_AxisTicMarkLength;
         const double xDelta = (xMaxAxis - xMinAxis) / double( xsteps );
         svg.push_back( DrawOneX_AxisTicMark( xPosition, y ) );
         svg.push_back( DrawOneX_AxisLabel( xPosition, y, double(i)* xDelta + xMinAxis, xMinAxis ) );
         xPosition += xDelta*xscale;
      }

      svg.push_back( "" );
      return(svg);
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static std::string Draw_XY_Axes( const AxisLimits& alY, const double minimumDistance, const double yscale )
      /*-------------------------------------------------------------------------------------*/
   {
      return
         std::string("\n<!--Draw the X and Y axes -->\n")
         + std::string("<polyline fill=\"none\" stroke=\"black\"")
         + LRL_DataToSVG(" stroke-width=\"", SVG_DistancePlotConstants::globalDataAxisWidth, "\" ")
         + LRL_DataToSVG(" points=\" ", LRL_ToString(SVG_WriterConstants::globalPlotSpace), ",0 0,0 0,", (alY.m_upperLimit - minimumDistance)*yscale, "\" />\n")
         + std::string("<polyline fill=\"none\" stroke=\"black\"")
         + LRL_DataToSVG(" stroke-width=\"", 1, "\" ")
         + LRL_DataToSVG(" points=\" ""0 ", (alY.m_upperLimit - minimumDistance)*yscale, " ", SVG_WriterConstants::globalPlotSpace, " ",(alY.m_upperLimit - minimumDistance)*yscale, " ", SVG_WriterConstants::globalPlotSpace, " 0" "\" />")
         + "\n<!-- -->\n";
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> DrawOneDistanceGlitches(const std::vector<double>& distances, const std::set<size_t>& glitches, 
      const double minimumDistance, const double xscale, const double yscale, const std::string& color) const
      /*-------------------------------------------------------------------------------------*/
   {
      std::list<std::string> svg;
      if (!glitches.empty( )) {
         svg.push_back( "\n<!--Draw Glitches -->\n" );
         std::set<size_t>::iterator it;
         for( it=glitches.begin(); it != glitches.end( ); ++it)
         {
            const double currentDistance = distances[*it];
            const double xGlitchOrdinal = double( *it );
            const double x = double( (1UL+xGlitchOrdinal) * xscale );
            double y;
            if (currentDistance >= 0.0) {
               y = (currentDistance - minimumDistance) * yscale;
               svg.push_back(DrawGlitchLocation(x, y, "black"));
               svg.push_back(DrawGlitchLabel(x, y, int(xGlitchOrdinal), color));
            }
         }

         svg.push_back( "\n<!-- -->\n" );
      }
      return(svg);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static std::string DrawGlitchLocation( const double x, const double y, const std::string& color ) {
      return
         "<line fill=\"none\" stroke=\"" + color +"\" stroke-width=\"4\""
         + LRL_DataToSVG( " x1=\"", x, "\"" )
         + LRL_DataToSVG( " y1=\"", y - 10, "\"" )
         + LRL_DataToSVG( " x2=\"", x, "\"" )
         + LRL_DataToSVG( " y2=\"", y - 60, "\" ")
         + " stroke-dasharray = \"10,5\" />";
   }

   enum eLineCondition { eOnlyData, eOneDataLineAndOneInvalid, eMixed, eOnlyInvalid } eCondition;

   eLineCondition TestLine(const std::vector<double>& distances) {
      if (distances.empty()) return eOnlyData;
      const double minDistance = *std::min_element(distances.begin(), distances.end());
      const double maxDistance = *std::max_element(distances.begin(), distances.end());
      if (minDistance >= 0.0) {
         return eOnlyData;
      }
      else if ( maxDistance < 0.0) {
         return eOnlyInvalid;
      }
      else
      {
         std::vector<double>::const_iterator itDataSubrangeBegin;
         std::vector<double>::const_iterator itDataSubrangeEnd;
         std::vector<double>::const_iterator itDataSubrangeSecondBegin;
         std::vector<double>::const_iterator itDataSubrangeSecondEnd;

         itDataSubrangeBegin = std::find_if(distances.begin(), distances.end(), IsPositiveOrZero);
         itDataSubrangeEnd = std::find_if(distances.begin(), distances.end(), IsNegative);

         size_t firstPositiveSubrange = (size_t)(itDataSubrangeEnd - itDataSubrangeBegin);

         itDataSubrangeSecondBegin = std::find_if(itDataSubrangeEnd, distances.end(), IsNegative);
         itDataSubrangeSecondEnd = std::find_if(itDataSubrangeEnd, distances.end(), IsPositiveOrZero);

         if ((firstPositiveSubrange + (itDataSubrangeSecondEnd - itDataSubrangeSecondBegin)) == distances.size())//  the branch where there is ONLY one (starting) + range followed by a single - range
         {
            return eOneDataLineAndOneInvalid;
         }
         else
         {
            return eMixed;
         }
      }
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> DrawTheDistanceLine(
      const bool drawLineAsBlack,
      const std::vector<double>& distances,
      const int lineWidth,
      const double minimumDistance,
      const double xscale,
      const double yscale,
      const std::string& color,
      const std::string& dashMode)
      /*-------------------------------------------------------------------------------------*/
   {
      const enum eLineCondition eConditionType = TestLine(distances);
      return
         (eConditionType==eOnlyData || eConditionType==eOneDataLineAndOneInvalid)
         ? FormatAllPointsAsPolyline(distances, lineWidth, minimumDistance, xscale, yscale, color, dashMode)
         : FormatEachDatumAsSeparateLineSegment( distances, lineWidth, minimumDistance, xscale, yscale, color );
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void WriteDistanceDataForExcel( const std::list<double>& distances, const AxisLimits& alY )
      /*-------------------------------------------------------------------------------------*/
   {
      std::string groupedDataFileName( m_fileName );
      const size_t firstPeriodLocation = m_fileName.find( '.' );
      groupedDataFileName.resize( firstPeriodLocation + 1 ); // cut off the .svg and count

      // now do the distance data file content
      const std::string fileName( LRL_ToString( groupedDataFileName ) + "_DIST.txt" );
      std::ofstream folOut;
      FileOperations::OpenOutputFile( folOut, fileName.c_str( ) );
      folOut << fileName << " " << alY.m_upperLimit << " ";

      for (std::list<double>::const_iterator it = distances.begin( ); it != distances.end( ); ++it)
         folOut << *it << " ";

      folOut << std::endl;
      folOut.close( );
   }

   std::list<std::string>  DrawOneDistanceLine(const std::vector<double> distances,
      const int lineWidth,
      const double minimumDistance,
      const double xscale,
      const double yscale,
      const std::string& color,
      const std::string& dashMode)
   {
      return DrawTheDistanceLine(FollowerConstants::globalPlotAllSegmentsAsBlack,
         distances, lineWidth, minimumDistance, xscale, yscale, color, dashMode); // keep g++ happy
   }

   std::list<std::string> DrawAllLines(const MultiFollower& multiFollow,
      const double minimumDistance,
      const double xscale,
      const double yscale)
   {

/*
std::string BASIC_COLORS[] = { "red", "lightblue", "turquoise", "slategrey",
"orange", "blueviolet", "coral", "saddlebrown", "blue", "pink", "violet",
"deeppink", "mediumvioletred", "tomato", "greenyellow", "olive" };
};
*/
      DistanceLineDescriptions lines(multiFollow.GetLineDescriptions());
      std::list<std::string> svg;
      std::list<std::string> svgS6 = DrawOneDistanceLine(multiFollow.GetS6().GetDistances(), lines.GetLineWidth("S6"), minimumDistance, xscale, yscale, lines.GetColor("S6"), lines.GetDashMode("S6"));
      std::list<std::string> svgG6 = DrawOneDistanceLine(multiFollow.GetG6().GetDistances(), lines.GetLineWidth("G6"), minimumDistance, xscale, yscale, lines.GetColor("G6"), lines.GetDashMode("G6"));
      std::list<std::string> svgD7 = DrawOneDistanceLine(multiFollow.GetD7().GetDistances(), lines.GetLineWidth("D7"), minimumDistance, xscale, yscale, lines.GetColor("D7"), lines.GetDashMode("D7"));
      std::list<std::string> svgCS = DrawOneDistanceLine(multiFollow.GetCS().GetDistances(), lines.GetLineWidth("CS"), minimumDistance, xscale, yscale, lines.GetColor("CS"), lines.GetDashMode("CS"));
      std::list<std::string> svgV7 = DrawOneDistanceLine(multiFollow.GetV7().GetDistances(), lines.GetLineWidth("V7"), minimumDistance, xscale, yscale, lines.GetColor("V7"), lines.GetDashMode("V7"));
      svg.splice(svg.end(), svgS6);
      svg.splice(svg.end(), svgG6);
      svg.splice(svg.end(), svgD7);
      svg.splice(svg.end(), svgCS);
      svg.splice(svg.end(), svgV7);
      return svg;
   }

   std::list<std::string> DrawAllGlitches(const MultiFollower& multiFollow,
      const double minimumDistance,
      const double xscale,
      const double yscale)

   {
      DistanceLineDescriptions lines(multiFollow.GetLineDescriptions());

      const LRL_Path<S6> pathS6 = multiFollow.GetS6();
      const LRL_Path<G6> pathG6 = multiFollow.GetG6();
      const LRL_Path<D7> pathD7 = multiFollow.GetD7();
      const LRL_Path<S6> pathCS = multiFollow.GetCS();
      const LRL_Path<G6> pathV7 = multiFollow.GetV7();

      const std::set<size_t> glitchesS6 = pathS6.GetGlitches();
      const std::set<size_t> glitchesG6 = pathG6.GetGlitches();
      const std::set<size_t> glitchesD7 = pathD7.GetGlitches();
      const std::set<size_t> glitchesCS = pathCS.GetGlitches();
      const std::set<size_t> glitchesV7 = pathV7.GetGlitches();

      std::list<std::string> svgS6 = DrawOneDistanceGlitches(multiFollow.GetS6().GetDistances(), glitchesS6, minimumDistance, xscale, yscale, lines.GetColor("S6"));
      std::list<std::string> svgG6 = DrawOneDistanceGlitches(multiFollow.GetG6().GetDistances(), glitchesG6, minimumDistance, xscale, yscale, lines.GetColor("G6"));
      std::list<std::string> svgD7 = DrawOneDistanceGlitches(multiFollow.GetD7().GetDistances(), glitchesD7, minimumDistance, xscale, yscale, lines.GetColor("D7"));
      std::list<std::string> svgCS = DrawOneDistanceGlitches(multiFollow.GetCS().GetDistances(), glitchesCS, minimumDistance, xscale, yscale, lines.GetColor("CS"));
      std::list<std::string> svgV7 = DrawOneDistanceGlitches(multiFollow.GetV7().GetDistances(), glitchesCS, minimumDistance, xscale, yscale, lines.GetColor("V7"));

      std::list<std::string> svg;
      svg.splice(svg.end(), svgS6);
      svg.splice(svg.end(), svgG6);
      svg.splice(svg.end(), svgD7);
      svg.splice(svg.end(), svgCS);
      svg.splice(svg.end(), svgV7);
      return svg;
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string>  DrawDistancePlot( const MultiFollower& multiFollow ) {
      std::list<std::string> svg = DistancePlotHeader( ); // keep g++ happy

      std::pair<double,double> ylimits = multiFollow.GetMinMax();
      // compute the limits and increments along the x and y axes
      LinearAxis la;
      const AxisLimits alX = la.LinearAxisLimits( 1, int( multiFollow.size( ) ) );
      const AxisLimits alY = la.LinearAxisLimits(ylimits.first, ylimits.second);

      // Calculate the scale factors that will make all of the distance values fit on the graph space.
      const double xscale = SVG_WriterConstants::globalPlotSpace / (alX.m_upperLimit - alX.m_lowerLimit+1);
      const double yscale = SVG_WriterConstants::globalPlotSpace / (alY.m_upperLimit - alY.m_lowerLimit);

      std::list<std::string> svgLines = DrawAllLines(multiFollow, alY.m_lowerLimit, xscale, yscale);
      svg.splice(svg.end(), svgLines);

      std::list<std::string> svgGlitches = DrawAllGlitches(multiFollow, alY.m_lowerLimit, xscale, yscale);
      svg.splice(svg.end(), svgGlitches);

      svg.push_back( Draw_XY_Axes( alY, alY.m_lowerLimit, yscale ) );
      std::list<std::string> labelsAndTicsFor_X_Axis( LabelsAndTicsFor_X_Axis( alX, alY.m_lowerLimit, xscale, yscale ) );
      svg.splice( svg.end( ), labelsAndTicsFor_X_Axis ); // keep g++ happy

      std::list<std::string> labelsAndTicsFor_Y_Axis( LabelsAndTicsFor_Y_Axis( alY, alY.m_lowerLimit, yscale ) );
      svg.splice( svg.end( ), labelsAndTicsFor_Y_Axis ); // keep g++ happy
      return svg;
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawSingleLineSegment(const std::string& color, const int lineWidth, const double x1, const double y1, const double x2, const double y2)
      /*-------------------------------------------------------------------------------------*/
   {
      const std::string svg =
         LRL_DataToSVG("<line fill=\"none\"")
         + LRL_DataToSVG(" stroke=\"", color, "\"")
         + LRL_DataToSVG(" stroke-width=\"", lineWidth, "\"")
         + LRL_DataToSVG(" x1=\"", x1, "\"")
         + LRL_DataToSVG(" y1=\"", y1, "\"")
         + LRL_DataToSVG(" x2=\"", x2, "\"")
         + LRL_DataToSVG(" y2=\"", y2, "\" />");

      return svg;
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawInvalidLineSegment(const std::string& color, const int lineWidth, const double x1, const double y, const double x2)
      /*-------------------------------------------------------------------------------------*/
   {
      const std::string svg =
         LRL_DataToSVG("<line fill=\"none\"")
         + LRL_DataToSVG(" stroke-dasharray = \"10, 10\" ")
         + LRL_DataToSVG(" stroke=\"", color, "\"")
         + LRL_DataToSVG(" stroke-width=\"", lineWidth, "\"")
         + LRL_DataToSVG(" x1=\"", x1, "\"")
         + LRL_DataToSVG(" y1=\"", y, "\"")
         + LRL_DataToSVG(" x2=\"", x2, "\"")
         + LRL_DataToSVG(" y2=\"", y, "\" />");

      return svg;
   }

   std::list<std::string> m_svg;
   const MultiFollower mf;
   const std::string m_fileName;

private: // don't implement
   SVG_DistancePlot& operator=(const SVG_DistancePlot&) const;
   SVG_DistancePlot( const SVG_DistancePlot& );

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static std::string DrawOneY_AxisTicMark( const double y ) {
      return(
         LRL_DataToSVG( "<line fill=\"none\" stroke=\"black\"" )
         + LRL_DataToSVG( " stroke-width=\"", SVG_DistancePlotConstants::globalDataAxisWidth, "\"" )
         + LRL_DataToSVG( " x1=\"0.0\"" )
         + LRL_DataToSVG( " y1=\"", y, "\"" )
         + LRL_DataToSVG( " x2=\"", -SVG_DistancePlotConstants::globalY_AxisTicMarkLength, "\"" )
         + LRL_DataToSVG( " y2=\"", y, "\" />" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static std::string DrawOneY_AxisLabel( const double y, const double yscale, const double minimumDistance ) {
      return(
         "<text transform=\"scale(1,-1)\" text-anchor=\"end\" font-family=\"sans-serif\" font-size=\"30\""
         + LRL_DataToSVG( " x=\"", -SVG_DistancePlotConstants::globalY_AxisTicMarkLength - 5, "\"" )
         + LRL_DataToSVG( " y=\"", -y + 10, "\" >" )
         + LRL_DataToSVG( y / yscale + minimumDistance, "</text>" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawOneX_AxisTicMark( const double xTic, const double y ) {
      return(
         "<line fill=\"none\" stroke=\"black\" "
         + LRL_DataToSVG( " stroke-width=\"", SVG_DistancePlotConstants::globalDataAxisWidth, "\" " )
         + LRL_DataToSVG( " x1=\"", xTic, "\"" )
         + LRL_DataToSVG( " y1=\"", 0.0, "\"" )
         + LRL_DataToSVG( " x2=\"", xTic, "\"" )
         + LRL_DataToSVG( " y2=\"", y, "\" />" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawOneX_AxisLabel( const double xTic, const double y, const double xPos, const double xMinAxis ) {
      return(
         "<text transform=\"scale(1,-1)\" font-family=\"sans-serif\" font-size=\"30\""
         + LRL_DataToSVG( " x=\"", xTic - 25, "\"" )
         + LRL_DataToSVG( " y=\"", -y + 25, "\" >" )
         + LRL_DataToSVG( xPos, "</text>" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static std::string DrawGlitchLabel( const double x, const double y, const int ordinal, const std::string& color ) {
      return(
         "<text transform=\"scale(1,-1)\" font-family=\"sans-serif\" font-size=\"30\" stroke = \"rgb(0,0,0)\" line = \"rgb(0,0,0)\"  fill = \"" + color + "\""
         + LRL_DataToSVG( " x=\"", x + 5, "\"" )
         + LRL_DataToSVG( " y=\"", -y + 60, "\" >" )
         + LRL_DataToSVG( ordinal, "</text>" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> DistancePlotHeader( ) {
      std::list<std::string> svg;
      svg.push_back( "\n\n <!--DISTANCE PLOT BEGINS   mode = " + ReadGlobalData::GetFollowerMode()  + "-->\n\n" );
      svg.push_back( "<!-- the following will scale and place the distance plot -->" );

      svg.push_back("<g transform=\"translate(150,975) scale(1 -1)\">");

      return(svg);
   }





};

#endif
