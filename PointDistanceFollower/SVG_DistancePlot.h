#if _MSC_VER
#pragma once
#endif

#ifndef SVG_DISTANCEPLOT
#define SVG_DISTANCEPLOT

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



//#include "MapBoundaryStrings2Colors.h"
#include "LinearAxis.h"

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
      //SVG_Writer<TVEC, TMAT, TREDUCE>(s_SVG_FileName +".svg", multiFollow);
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
//   std::vector<std::string> FindTransformRuns( ) {
 //     std::set<std::string> boundaries;
//
 //     const unsigned long n = m_follow.size( );
 //     for( unsigned long i = 0; i < n; ++i) {
 //        const std::string s = m_follow.GetBoundaryString( i );
 //        boundaries.insert( s );
 //     }
//
 //     return std::vector<std::string>( boundaries.begin( ), boundaries.end( ) );
//   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> FormatAllPointsAsPolyline( const std::vector<double>& distances,
      const int lineWidth, const double minimumDistance, const double xscale, const double yscale,      const std::string& color)
      /*-------------------------------------------------------------------------------------*/
   {
      const int xmax( int( distances.size( ) ) );

      std::string s = LRL_ToString( "   <polyline fill=\"none\" stroke=\"", color, "\" stroke-width=\"", lineWidth, "\" points=\" " );

      std::vector<double>::const_iterator it = distances.begin( );
      for (int i = 1; i <= xmax; ++i, ++it)
         s += LRL_DataToSVG( double( i )*xscale, "," ) +
         LRL_DataToSVG( ((*it)-minimumDistance) *yscale, " " );

      s += "\" />";
      std::list<std::string> svg( 1, s );

      return(svg);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> FormatEachDatumAsSeparateLineSegment( const std::vector<double>& distances, const double minimumDistance, const double xscale, const double yscale )
      /*-------------------------------------------------------------------------------------*/
   {
      const int xmax( int( distances.size( ) ) );
      std::list<std::string> svg;
      std::vector<double>::const_iterator it = distances.begin( );
      double yprev = ((*it) - minimumDistance)*yscale;
      for (int i = 1; i <= xmax; ++i, ++it)
      {
         const std::string bound/* = m_follow.GetBoundaryString( i - 1 )*/;
         const double yNext = ((*it) - minimumDistance)*yscale;
         svg.push_back( DrawSingleLineSegment( bound, double( i - 1 )*xscale, yprev, double( i )*xscale, yNext ) );
         yprev = yNext;
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
      for( unsigned long i = 0; i < alY.m_numSteps + 1; ++i)
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
      const unsigned long& xsteps = alX.m_numSteps; // does not include the starting point, just the steps
      double xPosition = xMinAxis*xscale + 0.5;
      for (unsigned long i = 0; i <= xsteps; ++i)
      {
         const double& y = -SVG_DistancePlotConstants::globalX_AxisTicMarkLength;
         const double xDelta = (xMaxAxis - xMinAxis) / double( xsteps );
         svg.push_back( DrawOneX_AxisTicMark( xPosition, y ) );
         svg.push_back( DrawOneX_AxisLabel( xPosition, y, double(i*int( xDelta ) + int( xMinAxis )), xMinAxis ) );
         xPosition += xDelta*xscale;
      }

      svg.push_back( "" );
      return(svg);
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static std::string Draw_XY_Axes( const AxisLimits& alY, const double minimumDistance, const double yscale )
      /*-------------------------------------------------------------------------------------*/
   {
      return(
         std::string( "\n<!--Draw the X and Y axes -->\n" )
         + std::string( "<polyline fill=\"none\" stroke=\"black\"" )
         + LRL_DataToSVG( " stroke-width=\"", SVG_DistancePlotConstants::globalDataAxisWidth, "\" " )
         + LRL_DataToSVG( " points=\" 600,0 0,0 0,", (alY.m_upperLimit - minimumDistance)*yscale, "\" />" )
         + "\n<!-- -->\n");
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> DrawGlitches( const std::set<unsigned long>& glitches, const double minimumDistance, const double xscale, const double yscale ) const
      /*-------------------------------------------------------------------------------------*/
   {
      std::list<std::string> svg;
      if (!glitches.empty( )) {
         svg.push_back( "\n<!--Draw Glitches -->\n" );

         for( unsigned long iglitch = 0; iglitch < glitches.size( ); ++iglitch)
         {
            //const double xGlitchOrdinal = double( glitches[iglitch].GetGlitchElement1( ).GetPosition( ) );
            //const double x = double( xGlitchOrdinal * xscale );
            //const double y = (glitches[iglitch].GetGlitchElement1( ).GetDistance( ) - minimumDistance) * yscale;
            //svg.push_back( DrawGlitchLocation( x, y ) );
            //svg.push_back( DrawGlitchLabel( x, y, int( xGlitchOrdinal ) ) );
         }

         svg.push_back( "\n<!-- -->\n" );
      }
      return(svg);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static std::string DrawGlitchLocation( const double x, const double y ) {
      return(
         "<line fill=\"none\" stroke=\"black\" stroke-width=\"2\""
         + LRL_DataToSVG( " x1=\"", x, "\"" )
         + LRL_DataToSVG( " y1=\"", y, "\"" )
         + LRL_DataToSVG( " x2=\"", x, "\"" )
         + LRL_DataToSVG( " y2=\"", y - 50, "\" />" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> DrawTheDistanceLine(
      const bool drawLineAsBlack,
      const std::vector<double>& distances,
      const int lineWidth,
      const double minimumDistance,
      const double xscale,
      const double yscale,
      const std::string& color)
      /*-------------------------------------------------------------------------------------*/
   {
      return
         (drawLineAsBlack)
         ? FormatAllPointsAsPolyline( distances, lineWidth, minimumDistance, xscale, yscale, color )
         : FormatEachDatumAsSeparateLineSegment( distances, minimumDistance, xscale, yscale );
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void WriteDistanceDataForExcel( const std::list<double>& distances, const AxisLimits& alY )
      /*-------------------------------------------------------------------------------------*/
   {
      std::string groupedDataFileName( m_fileName );
      const unsigned long firstPeriodLocation = (unsigned long)(m_fileName.find( '.' ));
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
      const std::string& color)
   {
      return DrawTheDistanceLine(FollowerConstants::globalPlotAllSegmentsAsBlack,
         distances, lineWidth, minimumDistance, xscale, yscale, color); // keep g++ happy
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
      int lineWidth = 15;
      std::list<std::string> svg;
      std::list<std::string> svgS6 = DrawOneDistanceLine(multiFollow.GetS6().GetDistances(), lines.GetLineWidth("S6"), minimumDistance, xscale, yscale, lines.GetColor("S6"));
      std::list<std::string> svgG6 = DrawOneDistanceLine(multiFollow.GetG6().GetDistances(), lines.GetLineWidth("G6"), minimumDistance, xscale, yscale, lines.GetColor("G6"));
      std::list<std::string> svgD7 = DrawOneDistanceLine(multiFollow.GetD7().GetDistances(), lines.GetLineWidth("D7"), minimumDistance, xscale, yscale, lines.GetColor("D7"));
      std::list<std::string> svgCS = DrawOneDistanceLine(multiFollow.GetCS().GetDistances(), lines.GetLineWidth("CS"), minimumDistance, xscale, yscale, lines.GetColor("CS"));
      svg.splice(svg.end(), svgS6);
      svg.splice(svg.end(), svgG6);
      svg.splice(svg.end(), svgD7);
      svg.splice(svg.end(), svgCS);
      return svg;
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string>  DrawDistancePlot( const MultiFollower& multiFollow ) {
      std::list<std::string> svg = DistancePlotHeader( ); // keep g++ happy

      std::pair<double,double> ylimits = multiFollow.GetMinMax();
      // compute the limits and increments along the x and y axes
      LinearAxis la;
      const AxisLimits alX = la.LinearAxisLimits( 0, int( multiFollow.size( ) )-1 );
      const AxisLimits alY = la.LinearAxisLimits(ylimits.first, ylimits.second);

      // Calculate the scale factors that will make all of the distance values fit on the graph space.
      const double xscale = SVG_WriterConstants::globalPlotSpace / (alX.m_upperLimit - alX.m_lowerLimit);
      const double yscale = SVG_WriterConstants::globalPlotSpace / (alY.m_upperLimit - alY.m_lowerLimit);

      std::list<std::string> svgLines = DrawAllLines(multiFollow, alY.m_lowerLimit, xscale, yscale);
      svg.splice(svg.end(), svgLines);

      svg.push_back( Draw_XY_Axes( alY, alY.m_lowerLimit, yscale ) );
      std::list<std::string> labelsAndTicsFor_X_Axis( LabelsAndTicsFor_X_Axis( alX, alY.m_lowerLimit, xscale, yscale ) );
      svg.splice( svg.end( ), labelsAndTicsFor_X_Axis ); // keep g++ happy

      std::list<std::string> labelsAndTicsFor_Y_Axis( LabelsAndTicsFor_Y_Axis( alY, alY.m_lowerLimit, yscale ) );
      svg.splice( svg.end( ), labelsAndTicsFor_Y_Axis ); // keep g++ happy

      //std::list<std::string> drawGlitches( DrawGlitches( glitches, alY.m_lowerLimit, xscale, yscale ) );  // lca need to implement PER LINE
      //svg.splice( m_svg.end( ), drawGlitches ); // keep g++ happy

      //InsertVectorAndDifferenceData( distances, Delonedistances);

      //WriteDistanceDataForExcel( distances, alY );
      return svg;
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawSingleLineSegment( const std::string& bound, const double x1, const double y1, const double x2, const double y2 )
      /*-------------------------------------------------------------------------------------*/
   {
      const std::string svg =
         LRL_DataToSVG("<line fill=\"none\" stroke-width=\"", SVG_DistancePlotConstants::globalG6DataLineStrokeWidth, "\"")
         + LRL_DataToSVG(" boundary-string=\"", bound, "\" ")
         //+ LRL_DataToSVG(" stroke=\"", m_colorMap(bound), "\"")
         + LRL_DataToSVG(" x1=\"", x1, "\"")
         + LRL_DataToSVG(" y1=\"", y1, "\"")
         + LRL_DataToSVG(" x2=\"", x2, "\"")
         + LRL_DataToSVG(" y2=\"", y2, "\" />");

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
   static std::string DrawGlitchLabel( const double x, const double y, const int ordinal ) {
      return(
         "<text transform=\"scale(1,-1)\" font-family=\"sans-serif\" font-size=\"30\""
         + LRL_DataToSVG( " x=\"", x + 5, "\"" )
         + LRL_DataToSVG( " y=\"", -y + 60, "\" >" )
         + LRL_DataToSVG( ordinal, "</text>" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> DistancePlotHeader( ) {
      std::list<std::string> svg;
      svg.push_back( "\n\n <!--DISTANCE PLOT BEGINS-->\n\n" );
      svg.push_back( "<!-- the following will scale and place the distance plot -->" );

      svg.push_back( (FollowerConstants::globalDrawDistancePlot && FollowerConstants::globalDrawCirclePlot)
         ? "<g  transform=\"translate(0,0) scale(.3, .3) \">"
         : "<g  transform=\"translate(0,50) \">" );

      svg.push_back( "<g transform=\"translate(150,700) scale(1 -1)\">" );

      return(svg);
   }





};

#endif
