#if _MSC_VER
#pragma once
#endif

#ifndef SVG_DISTANCEPLOT
#define SVG_DISTANCEPLOT

#include <fstream>
#include <iostream>
#include <map>
#include <list>
#include <vector>

#include "FileOperations.h"
#include "Follow.h"
#include "FollowerConstants.h"
#include "Glitch.h"
#include "LRL_ToString.h"
#include "SVG_Writer.h"

#include <fstream>
#include <set>


#include "MapBoundaryStrings2Colors.h"
#include "LinearAxis.h"

template<typename TVEC, typename TMAT>
class SVG_DistancePlot
{
public:

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   SVG_DistancePlot( Follow<TVEC, TMAT>& follow, MapBoundaryStrings2Colors& colorMap, const std::string& fileName, const std::vector<Glitch<TVEC> >& glitches )
      : m_follow( follow )
      , m_fileName( fileName )
      , m_glitches( glitches )
      , m_colorMap( colorMap )
      /*-------------------------------------------------------------------------------------*/
   {
      if (FollowerConstants::globalDrawDistancePlot)
         DrawDistancePlot( follow.GetDistances( ), follow.GetDeloneDistances( ), glitches );
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
   std::vector<std::string> FindTransformRuns( ) {
      std::set<std::string> boundaries;

      const unsigned long n = m_follow.size( );
      for( unsigned long i = 0; i < n; ++i) {
         const std::string s = m_follow.GetBoundaryString( i );
         boundaries.insert( s );
      }

      return std::vector<std::string>( boundaries.begin( ), boundaries.end( ) );
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> FormatAllPointsAsPolyline( const std::list<double>& distances, const double minimumDistance, const double xscale, const double yscale )
      /*-------------------------------------------------------------------------------------*/
   {
      const int xmax( int( distances.size( ) ) );

      std::string s = LRL_ToString( "   <polyline fill=\"none\" stroke=\"black\" stroke-width=\"", SVG_DistancePlotConstants::globalDeloneDataLineStrokeWidth, "\" points=\" " );

      std::list<double>::const_iterator it = distances.begin( );
      for (int i = 1; i <= xmax; ++i, ++it)
         s += LRL_ToString( double( i )*xscale, "," ) +
         LRL_ToString( ((*it)-minimumDistance) *yscale, " " );

      s += "\" />";
      std::list<std::string> svg( 1, s );

      return(svg);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> FormatEachDatumAsSeparateLineSegment( const std::list<double>& distances, const double minimumDistance, const double xscale, const double yscale )
      /*-------------------------------------------------------------------------------------*/
   {
      const int xmax( int( distances.size( ) ) );
      std::list<std::string> svg;
      std::list<double>::const_iterator it = distances.begin( );
      double yprev = ((*it) - minimumDistance)*yscale;
      for (int i = 1; i <= xmax; ++i, ++it)
      {
         const std::string bound = m_follow.GetBoundaryString( i - 1 );
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
         + LRL_ToString( " stroke-width=\"", SVG_DistancePlotConstants::globalDataAxisWidth, "\" " )
         + LRL_ToString( " points=\" 600,0 0,0 0,", (alY.m_upperLimit - minimumDistance)*yscale, "\" />" )
         + "\n<!-- -->\n");
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> DrawGlitches( const std::vector<Glitch<TVEC> >& glitches, const double minimumDistance, const double xscale, const double yscale ) const
      /*-------------------------------------------------------------------------------------*/
   {
      std::list<std::string> svg;
      if (!glitches.empty( )) {
         svg.push_back( "\n<!--Draw Glitches -->\n" );

         for( unsigned long iglitch = 0; iglitch < glitches.size( ); ++iglitch)
         {
            const double xGlitchOrdinal = double( glitches[iglitch].GetGlitchElement1( ).GetPosition( ) );
            const double x = double( xGlitchOrdinal * xscale );
            const double y = (glitches[iglitch].GetGlitchElement1( ).GetDistance( ) - minimumDistance) * yscale;
            svg.push_back( DrawGlitchLocation( x, y ) );
            svg.push_back( DrawGlitchLabel( x, y, int( xGlitchOrdinal ) ) );
         }

         svg.push_back( "\n<!-- -->\n" );
      }
      return(svg);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static std::string DrawGlitchLocation( const double x, const double y ) {
      return(
         "<line fill=\"none\" stroke=\"black\" stroke-width=\"2\""
         + LRL_ToString( " x1=\"", x, "\"" )
         + LRL_ToString( " y1=\"", y, "\"" )
         + LRL_ToString( " x2=\"", x, "\"" )
         + LRL_ToString( " y2=\"", y - 50, "\" />" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> DrawTheDistanceLine(
      const bool drawLineAsBlack,
      const std::list<double>& distances,
      const double minimumDistance,
      const double xscale,
      const double yscale )
      /*-------------------------------------------------------------------------------------*/
   {
      return
         (drawLineAsBlack)
         ? FormatAllPointsAsPolyline( distances, minimumDistance, xscale, yscale )
         : FormatEachDatumAsSeparateLineSegment( distances, minimumDistance, xscale, yscale );
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> DrawTheDeloneDistanceLine(
      const std::list<double>& distances, 
      const double minimumDistance,
      const double xscale,
      const double yscale )
      /*-------------------------------------------------------------------------------------*/
   {
      return FormatAllPointsAsPolyline( distances, minimumDistance, xscale, yscale );
//      return std::list<std::string>();
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


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void InsertVectorAndDifferenceData( const std::list<double>& distances, const std::list<double>& Delonedistances )
   {
      const std::vector<TVEC> secondProbeList( m_follow.GetSecondProbeList( ) );

      m_svg.push_back( "\n<!-- Reduced vector followed by D7dist from the final reduced cell" );

      typename std::list<double>::const_iterator iDist1 = distances.begin();
      typename std::list<double>::const_iterator iDist2 = Delonedistances.begin();
      typename std::vector<TVEC>::const_iterator iProbe = secondProbeList.begin( );
      int count = 0;

      for (; iProbe != secondProbeList.end( ); ++iProbe, ++iDist1, ++iDist2, ++count)
      {
         m_svg.push_back( LRL_ToString( *iProbe  )
            + "   (" + LRL_ToString(*iDist1)
            + LRL_ToString(",  ", *iDist2, ")")
            + LRL_ToString( "   ", m_follow.GetBoundaryString( count ) ) );
      }

      m_svg.push_back( "-->\n</g>\n</g>" );
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void DrawDistancePlot( const std::list<double>& distances, const std::list<double>& Delonedistances, const std::vector<Glitch<TVEC> >& glitches ) {
      std::list<std::string> distancePlotHeader( DistancePlotHeader( ) ); // keep g++ happy
      m_svg.splice( m_svg.end( ), distancePlotHeader );

      // compute the limits and increments along the x and y axes
      LinearAxis la;
      double yAxisMin = std::min(*std::min_element(distances.begin(), distances.end()), *std::min_element(Delonedistances.begin(), Delonedistances.end()));
      double yAxisMax = std::max(*std::max_element(distances.begin(), distances.end()), *std::max_element(Delonedistances.begin(), Delonedistances.end()));
      const AxisLimits alX = la.LinearAxisLimits( 1, int( distances.size( ) ) );
      const AxisLimits alY = la.LinearAxisLimits(yAxisMin, yAxisMax);

      // Calculate the scale factors that will make all of the distance values fit on the graph space.
      const double xscale = SVG_WriterConstants::globalPlotSpace / (alX.m_upperLimit - alX.m_lowerLimit);
      const double yscale = SVG_WriterConstants::globalPlotSpace / (alY.m_upperLimit - alY.m_lowerLimit);


      std::list<std::string> distDelonePlotHeader = DrawTheDeloneDistanceLine( Delonedistances, alY.m_lowerLimit, xscale, yscale ); // keep g++ happy
      m_svg.splice( m_svg.end( ), distDelonePlotHeader ); // keep g++ happy
      m_svg.push_back( Draw_XY_Axes( alY, alY.m_lowerLimit, yscale ) );


      std::list<std::string> distPlotHeader = DrawTheDistanceLine(FollowerConstants::globalPlotAllSegmentsAsBlack,
         distances, alY.m_lowerLimit, xscale, yscale ); // keep g++ happy
      m_svg.splice( m_svg.end( ), distPlotHeader ); // keep g++ happy



      m_svg.push_back( Draw_XY_Axes( alY, alY.m_lowerLimit, yscale ) );
      std::list<std::string> labelsAndTicsFor_X_Axis( LabelsAndTicsFor_X_Axis( alX, alY.m_lowerLimit, xscale, yscale ) );
      m_svg.splice( m_svg.end( ), labelsAndTicsFor_X_Axis ); // keep g++ happy

      std::list<std::string> labelsAndTicsFor_Y_Axis( LabelsAndTicsFor_Y_Axis( alY, alY.m_lowerLimit, yscale ) );
      m_svg.splice( m_svg.end( ), labelsAndTicsFor_Y_Axis ); // keep g++ happy

      std::list<std::string> drawGlitches( DrawGlitches( glitches, alY.m_lowerLimit, xscale, yscale ) );
      m_svg.splice( m_svg.end( ), drawGlitches ); // keep g++ happy

      const std::vector<TVEC> secondProbeList( m_follow.GetSecondProbeList( ) );

      InsertVectorAndDifferenceData( distances, Delonedistances);

      WriteDistanceDataForExcel( distances, alY );
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawSingleLineSegment( const std::string& bound, const double x1, const double y1, const double x2, const double y2 )
      /*-------------------------------------------------------------------------------------*/
   {
      return(
         LRL_ToString( "<line fill=\"none\" stroke-width=\"", SVG_DistancePlotConstants::globalG6DataLineStrokeWidth, "\"" )
         + LRL_ToString( " boundary-string=\"", bound, "\" " )
         + LRL_ToString( " stroke=\"", m_colorMap( bound ), "\"" )
         + LRL_ToString( " x1=\"", x1, "\"" )
         + LRL_ToString( " y1=\"", y1, "\"" )
         + LRL_ToString( " x2=\"", x2, "\"" )
         + LRL_ToString( " y2=\"", y2, "\" />" ));
   }

   std::list<std::string> m_svg;
   const Follow<TVEC, TMAT> m_follow;
   const std::string m_fileName;
   const std::vector<Glitch<TVEC> >& m_glitches;
   MapBoundaryStrings2Colors& m_colorMap;

private: // don't implement
   SVG_DistancePlot& operator=(const SVG_DistancePlot&) const;
   SVG_DistancePlot( const SVG_DistancePlot& );

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static std::string DrawOneY_AxisTicMark( const double y ) {
      return(
         LRL_ToString( "<line fill=\"none\" stroke=\"black\"" )
         + LRL_ToString( " stroke-width=\"", SVG_DistancePlotConstants::globalDataAxisWidth, "\"" )
         + LRL_ToString( " x1=\"0.0\"" )
         + LRL_ToString( " y1=\"", y, "\"" )
         + LRL_ToString( " x2=\"", -SVG_DistancePlotConstants::globalY_AxisTicMarkLength, "\"" )
         + LRL_ToString( " y2=\"", y, "\" />" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static std::string DrawOneY_AxisLabel( const double y, const double yscale, const double minimumDistance ) {
      return(
         "<text transform=\"scale(1,-1)\" text-anchor=\"end\" font-family=\"sans-serif\" font-size=\"30\""
         + LRL_ToString( " x=\"", -SVG_DistancePlotConstants::globalY_AxisTicMarkLength - 5, "\"" )
         + LRL_ToString( " y=\"", -y + 10, "\" >" )
         + LRL_ToString( y / yscale + minimumDistance, "</text>" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawOneX_AxisTicMark( const double xTic, const double y ) {
      return(
         "<line fill=\"none\" stroke=\"black\" "
         + LRL_ToString( " stroke-width=\"", SVG_DistancePlotConstants::globalDataAxisWidth, "\" " )
         + LRL_ToString( " x1=\"", xTic, "\"" )
         + LRL_ToString( " y1=\"", 0.0, "\"" )
         + LRL_ToString( " x2=\"", xTic, "\"" )
         + LRL_ToString( " y2=\"", y, "\" />" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawOneX_AxisLabel( const double xTic, const double y, const double xPos, const double xMinAxis ) {
      return(
         "<text transform=\"scale(1,-1)\" font-family=\"sans-serif\" font-size=\"30\""
         + LRL_ToString( " x=\"", xTic - 25, "\"" )
         + LRL_ToString( " y=\"", -y + 25, "\" >" )
         + LRL_ToString( xPos, "</text>" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static std::string DrawGlitchLabel( const double x, const double y, const int ordinal ) {
      return(
         "<text transform=\"scale(1,-1)\" font-family=\"sans-serif\" font-size=\"30\""
         + LRL_ToString( " x=\"", x + 5, "\"" )
         + LRL_ToString( " y=\"", -y + 60, "\" >" )
         + LRL_ToString( ordinal, "</text>" ));
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
