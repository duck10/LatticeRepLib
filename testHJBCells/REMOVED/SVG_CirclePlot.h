#if _MSC_VER
#pragma once
#endif

#ifndef SVG_CIRCLEPLOT_H
#define SVG_CIRCLEPLOT_H

#include <list>
#include <string>
#include <vector>

#include "Follow.h"
#include "FollowerConstants.h"
#include "FollowerTools.h"
#include "MapBoundaryStrings2Colors.h"
#include "ToString.h"
#include "triple.h"

template<typename TVEC, typename TMAT> class Follow;

template<typename TVEC, typename TMAT>
class SVG_CirclePlot
{
public:

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   SVG_CirclePlot( Follow<TVEC, TMAT>& follow,
      MapBoundaryStrings2Colors& colorMap,
      const std::string fileName )
      : m_follow( follow )
      , m_MinMax( follow.GetLimits( ) )
      , m_which2plot( follow.GetWhichToPlot( ) )
      /*-------------------------------------------------------------------------------------*/
   {
      if (FollowerConstants::globalDrawCirclePlot) {

         ConstructCircleViewbox( m_MinMax );
         CircleHeader( m_MinMax );
         CircleDef( );

         DrawAllCirclesInFrame( colorMap );

         const std::vector<triple<double, double, double> > angles = m_follow.GetLinesFromAngles( );
         const std::vector<long>  runList = follow.IdentifyRunsOfAngle( angles );
         DrawLinesBetweenCircleRuns( angles, runList );

         CircleFooter( );
      }
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> Format( ) const {
      return(m_svg);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   ~SVG_CirclePlot( void ){
   }

private:

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void DrawAllCirclesInFrame( MapBoundaryStrings2Colors& colorMap ) {
      int iCircle = 0;
      const std::vector<TVEC> reducedList = m_follow.GetSecondProbeList( );
      typename std::vector<TVEC>::const_reverse_iterator it;
      for (it = reducedList.rbegin( ); it != reducedList.rend( ); ++it)
      {
         const std::string boundColor = m_follow.GetBoundaryString( iCircle );
         FormatOnePointAsCircle( *it, colorMap( boundColor ) );
         ++iCircle;
      }
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void ConstructCircleViewbox(
      const std::pair<std::pair<double, double>, std::pair<double, double> >& xy_MinMax )
      /*-------------------------------------------------------------------------------------*/
   {
      const double xmin = xy_MinMax.first.first;
      const double ymin = xy_MinMax.second.first;
      const double xmax = xy_MinMax.first.second;
      const double ymax = xy_MinMax.second.second;
      const double xRange = xmax - xmin;
      const double yRange = ymax - ymin;

      m_viewboxOrigin = std::make_pair( ComputeViewboxAxisMin( xmin, xRange, yRange ), ComputeViewboxAxisMin( ymin, xRange, yRange ) );
      m_scale = 13000.0 / std::max( xRange, yRange );
      m_viewboxXYSize = std::make_pair( 1.1*xRange*m_scale, 1.1*yRange*m_scale );
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   bool DeltaExceedsThresholdForDrawingLine( const double x, const double y ) {
      return(std::max( std::abs( x - m_xprev ), std::abs( y - m_yprev ) ) >
         5.0*SVG_CirclePlotConstants::globalCircleRadius / m_scale);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string FormatOneLineBetweenCircles( const double x, const double y, const double angle ) {
      return(
         ToString( "<line x1=\"", m_xprev *m_scale + m_use.first )
         + ToString( "\" y1=\"", m_yprev *m_scale + m_use.second )
         + ToString( "\" x2=\"", x     *m_scale )
         + ToString( "\" y2=\"", y     *m_scale )
         + ToString( "\" angle=\"", angle * 180.0 / 4.0 / std::atan( 1.0 ) )
         + "\" stroke-width=\"15\"  fill=\"black\" stroke=\"black\" marker-end=\"url(#arrowhead)\"/>");
   }

   const double Pi = 4.0*std::atan2( 1.0, 1.0 );
   const double twoPi = 2.0*Pi;
   const double limitingDelta = 1.0E-6;
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   double ComputeSlopeBetweenPoints( const double x, const double y ) {
      double angle = std::atan2( y - m_yprev, x - m_xprev );
      if ((std::abs( angle ) < limitingDelta) || (twoPi - std::abs( angle ) < limitingDelta)) angle = 0.0;
      if (std::abs( Pi - angle ) < limitingDelta) angle = Pi;
      return(angle);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void DrawLinesBetweenCircleRuns( const std::vector<triple<double, double, double> >& angles,
      const std::vector<long>& runList )
      /*-------------------------------------------------------------------------------------*/
   {
      m_xprev = angles[0].second;
      m_yprev = angles[0].third;

      // Write the label for the first point
      DrawLabelsOnEndsOfRuns( long(angles.size( )), m_xprev*m_scale + 100, m_yprev*m_scale + 100, 0.0, 0.0 );

      for( size_t counter = 1; counter != angles.size( ); ++counter)
      {
         const double x = angles[counter].second;
         const double y = angles[counter].third;
         const double xDelta = std::abs( x - m_xprev );
         const double yDelta = std::abs( y - m_yprev );

         if (runList[counter] == 1 || DeltaExceedsThresholdForDrawingLine( x, y ) /*|| counter == 1  || counter==2*/)
         {
            const double angle = ComputeSlopeBetweenPoints( x, y );
            m_use = StartXY( m_xprev, m_yprev, x, y );
            m_svg.push_back( FormatOneLineBetweenCircles( x, y, angle ) );
            DrawLabelsOnEndsOfRuns( counter, m_xprev*m_scale, m_yprev*m_scale, m_use.second, m_use.first );
            DrawLabelsOnEndsOfRuns( counter + 1, x*m_scale, y*m_scale, m_use.second, m_use.first );
         }

         m_xprev = x;
         m_yprev = y;
      }
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   // Output the labels for the steps at the ends of runs
   void DrawLabelsOnEndsOfRuns( const size_t index, const double x, const double y, const double dx, const double dy )
      /*-------------------------------------------------------------------------------------*/
   {
      m_svg.push_back(
         ToString( "<text x=\"") + ToString( x + dx )
         + ToString( "\" y=\"") + ToString( y - dy )
         + ToString( "\" font-family=\"sans-serif\" font-size=\"", 2.0*SVG_CirclePlotConstants::globalCircleRadius )
         + ToString( "px\" fill=\"black\">   ", index, "   </text>\n" ) );
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string CircleHeaderBoilerPlate( void ) {
      return(
         std::string( "<!-- CIRCLE PLOT BEGINS -->\n" )
         + std::string( "<!-- scale affects rendering NOT the original coordinates -->\n" )
         + std::string( "<!-- the scale times the translated coordinates must fit in the viewbox -->\n" )
         + std::string( "<!-- the second translate moves the image to where it is placed -->" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string Circle_SVG_ID( const std::pair<std::pair<double, double>, std::pair<double, double> >& xy_MinMax ) {
      return(
         ToString( "<g id=\"CirclePlot\" transform=\" scale(.05) translate(",
         -xy_MinMax.first.first*m_scale, ", " )
         + ToString( -xy_MinMax.second.first*m_scale )
         + ToString( ") translate(", 400, ", " )
         + ToString( 6000, ")\" >" ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void CircleHeader( const std::pair<std::pair<double, double>, std::pair<double, double> >& xy_MinMax )
      /*-------------------------------------------------------------------------------------*/
   {
      m_svg.push_back( CircleHeaderBoilerPlate( ) );
      m_svg.push_back( Circle_SVG_ID( xy_MinMax ) );
      m_svg.push_back( " <desc>Output circle plot for Follower</desc> " );
      m_svg.push_back( DefineArrowhead( ) );
      m_svg.push_back( "" );
   };

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void SVG_Footer( void ) {
      m_svg.push_back( "</svg>" );
   };

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void CircleFooter( void ) {
      m_svg.push_back( "</g>" );
   };

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   // construct the prototype circle that will be used (by SVG "use" commands" to
   // generate all the image circles
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void CircleDef( void ) {
      //   sample     <circle id="cir" r="150" fill="white" stroke="black" stroke-width="30"  />
      m_svg.push_back( "<defs>" );
      m_svg.push_back(
         ToString( "        <circle id=\"cir\" r=\"", SVG_CirclePlotConstants::globalCircleRadius ) +
         ToString( "\" stroke-width=\"", SVG_CirclePlotConstants::globalCircleStrokeWidth, "\"  />" ) );
      m_svg.push_back( "</defs>\n" );
   };

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   // This makes the arrowhead that goes on every line between segments of circles
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DefineArrowhead( void ) {
      return
         std::string( " <defs><marker id=\"arrowhead\" viewBox=\"0 0 400 400\" refX=\"100\" refY=\"50\" \n" )
         + std::string( "     markerUnits=\"strokeWidth\" orient=\"auto\" \n" )
         + std::string( "     markerWidth=\"60\" markerHeight=\"60\"> \n" )
         + std::string( "     <polyline points=\"0,0 100,50 0,100 q\" fill=\"black\" /> \n" )
         + std::string( " </marker></defs> \n" );
   }
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void FormatOnePointAsCircle( const TVEC& p, const std::string& boundColor )
      /*-------------------------------------------------------------------------------------*/
   {
      // sample <circle r="211.8" cx="2254.14" cy="3673.6" stroke-width="10"  fill="white" stroke="rgb( 14.1%, 14.1%, 14.1% )"/>
      m_svg.push_back(
         ToString( "<use x=\"", p[m_which2plot.first] * m_scale )
         + ToString( "\" y=\"", p[m_which2plot.second] * m_scale )
         + "\" xlink:href=\"#cir\""
         + ToString( " fill=\"", boundColor, "\"" )
         + ToString( " stroke=\"", ((boundColor == "black") ? "white" : "black"), "\"  />" ) );
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   // shorten the starting point of a line by the radius so that each arrow
   // starts at the edge of the circle that begins the line, rather than at its center
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::pair<double, double> StartXY( const double x1, const double y1, const double x2, const double y2 ) const
      /*-------------------------------------------------------------------------------------*/
   {
      const double angle = std::atan2( y2 - y1, x2 - x1 );
      const double dx = SVG_CirclePlotConstants::globalCircleRadius * std::cos( angle );
      const double dy = SVG_CirclePlotConstants::globalCircleRadius * std::sin( angle );

      return(std::make_pair( dx, dy ));
   }

   std::list<std::string>                                                m_svg;
   std::pair<double, double>                                              m_viewboxOrigin;
   std::pair<double, double>                                              m_viewboxXYSize;
   double                                                                m_scale;
   double                                                                m_xprev;
   double                                                                m_yprev;
   std::pair<double, double>                                              m_use;
   const std::pair<int, int>                                              m_which2plot;
   const std::pair< std::pair<double, double>, std::pair<double, double> > m_MinMax; //xlimits, ylimits
   const Follow<TVEC, TMAT>&                                              m_follow;

private: // don't implement
   SVG_CirclePlot& operator=(const SVG_CirclePlot&) const;
   SVG_CirclePlot( const SVG_CirclePlot& );

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   double ComputeViewboxAxisMin( const double minDataValue, const double xRange, const double yRange ) {
      return(minDataValue - 0.05*std::min( xRange, yRange ));
   }

};

#endif