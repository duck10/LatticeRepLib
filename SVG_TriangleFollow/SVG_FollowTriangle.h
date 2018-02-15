#ifdef _MSC_VER
#define USE_LOCAL_HEADERS
#endif
#ifndef SVG_FOLLOWTRIANGLE_H
#define SVG_FOLLOWTRIANGLE_H
#include <sstream>
#include <string>

#include "GlobalTriangleFollowerConstants.h"
#include "LinearAxis.h"
#include "ProgressData.h"
#include "SVG_Tools.h"
#include "TriangleFollow.h"
#include "triple.h"


template<typename TVEC, typename TREDUCEMETHOD, typename TFOLLOWMETHOD >
class SVG_FollowTriangle {
public:

   SVG_FollowTriangle(const int boxWidth, const int boxHeight, const int border, const std::string& svgTriangles, TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD >& tf )
      : m_viewboxWidth( boxWidth )
      , m_viwboxHeight( boxHeight )
      , m_borderWidth( border )
      , m_triangleFollow( tf )
      , m_fontSize( 30.0*boxWidth/1800 )
      , m_OutputFilename( ) {
      std::stringstream os;

      /* the next line just generates a hash code so that
      if two files have exactly the same names, they can
      still be written separately. Before this was done,
      sometimes a name would be repeated, and two sets
      of data would be written to the svg file, making
      it unusable. Probably a better, but more complex 
      fix for the problem would be to use a deeper time
      function.
      */
      const unsigned long n = (unsigned long)( rand( ) | 64 );

      os << std::hex << std::setw( 1 ) << std::setfill( '0' ) << rand( );
      m_OutputFilename = LRL_CreateFileName::Create(GlobalConstants::globalFileNamePrefix, os.str() + std::string(".svg"));
      const unsigned long npoints = tf.m_triangleDiff[0].size( );
      m_rawDeltaDist12 = ( tf.m_inputVector1-tf.m_inputVector2 ).norm( )/( npoints-1U );
      m_rawDeltaDist13 = ( tf.m_inputVector1-tf.m_inputVector3 ).norm( )/( npoints-1U );
      m_rawDeltaDist23 = ( tf.m_inputVector3-tf.m_inputVector2 ).norm( )/( npoints-1U );

      const int trial_plotWidth = ( boxWidth-3*border )/2;
      const int trial_plotHeight = ( boxHeight-5*border )/2;
      const int boxSize = ( trial_plotHeight<trial_plotWidth ) ?  trial_plotHeight: trial_plotWidth;

      std::vector<std::string> finalsvg;
      finalsvg.push_back( ImageHeader( boxWidth+200, 3*boxHeight+400 ) );

      finalsvg.push_back( FormatInputVectors( ) );
      finalsvg.push_back( EncloseAsComment( std::string( "random seed " ) + LRL_ToString( tf.GetSeed( ) ) + std::string( "  trial # " ) + LRL_ToString( tf.GetTrialNumber( ) ) ) );
      finalsvg.push_back( EncloseAsComment( std::string( "Original File Name " ) + m_OutputFilename ) );
      finalsvg.push_back( EncloseAsComment(FormatWorstCase(tf.GetWorstTriple())));

      std::vector<std::string> svg;
      m_plotWidth = trial_plotWidth;
      m_plotHeight = trial_plotHeight;

      MarkTriangleViolations( );

      //m_dataType = eArea;
      //const std::vector<std::string> graph4 = PlaceSubPicture( Xpos( 1 ), 0, DrawGraph( border, 0 * border + -1 * m_plotHeight, "Sqrt Area", tf.m_triangleArea ) );
      //svg.insert( svg.end( ), graph4.begin( ), graph4.end( ) );

      m_dataType = edist23;
      const std::vector<std::string> graph2 = PlaceSubPicture( Xpos(0), 0, DrawGraph( border, -3 * border + -2 * m_plotHeight, "Dist23", tf.m_dist23 ) );
      svg.insert( svg.end( ), graph2.begin( ), graph2.end( ) );

      //m_dataType = etanhDeltaDist23;
      //const std::vector<std::string> graph1 = PlaceSubPicture( Xpos( 1 ), 0, DrawGraph( border, -6 * border + -3 * m_plotHeight, "tanh(Haar Dist23) Arbitrary Scale", tf.m_tanhDist23Delta ) );
      //svg.insert( svg.end( ), graph1.begin( ), graph1.end( ) );




      m_dataType = eTriangleViolation;
      const std::vector<std::string> graph3  = DrawGraph( border, Ypos( 0 ), "Triangle Violation Percent (of longest side)", tf.m_triangleDiff );
      svg.insert( svg.end( ), graph3.begin( ), graph3.end( ) );

      m_dataType = enormedDist23;
      const std::vector<std::string> graph7  = DrawGraph( Xpos(1), Ypos(0), "Dist12", tf.m_dist12);
      svg.insert( svg.end( ), graph7.begin( ), graph7.end( ) );

      m_dataType = eDeltaDist23;
      const std::vector<std::string> graph1A = DrawGraph(Xpos(1), Ypos( 1 ), std::string( "Dist13" ), tf.m_dist13 );
      svg.insert( svg.end( ), graph1A.begin( ), graph1A.end( ) );

      m_dataType = eLegend;
      const std::vector<std::string> graph6  = DrawLegend( border, Ypos( 2 ) );
      svg.insert( svg.end( ), graph6.begin( ), graph6.end( ) );




      const std::vector<std::string> scaledPic = PlaceSubPicture( 4*border, 4*border, ScaleSubPicture( 1, -1, svg ) );
      finalsvg.insert( finalsvg.end( ), scaledPic.begin( ), scaledPic.end( ) );
      finalsvg.push_back(EncloseAsComment(svgTriangles));
      const std::vector<std::string> vs( ImageFooter( m_triangleFollow.m_ProblemLog ) );
      finalsvg.insert( finalsvg.end( ), vs.begin( ), vs.end( ) );

      OutputSVG( finalsvg );
   }


   std::string GetOutputFilename( void ) const {
      return m_OutputFilename;
   };

private:
   const TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD >& m_triangleFollow;
   const int m_viewboxWidth;
   const int m_viwboxHeight;
   const int m_borderWidth;
   const double m_fontSize;
   eDataType m_dataType;

   double m_rawDeltaDist12;
   double m_rawDeltaDist13;
   double m_rawDeltaDist23;

   int m_plotWidth;
   int m_plotHeight;
   std::string m_OutputFilename;
   //   Triple<bool>   m_triangleViolations;
   std::vector<bool>   m_triangleViolations[3];



public:
   static std::pair<double, double> ComputeAxisLimitsForOneArray( const std::vector<double>& d ) {
      // compute the limits and increments along the x and y axes
      if ( d.empty( ) )
         return std::make_pair( 0.0, 0.0 );
      else
         return std::make_pair(
         *std::min_element( d.begin( ), d.end( ) ),
         *std::max_element( d.begin( ), d.end( ) ) );
   }

   static AxisLimits ComputeAxisLimits( const ProgressData<double> t[3] ) {

      std::pair<double, double> al1;
      std::pair<double, double> al2;
      std::pair<double, double> al3;
      bool b1 = false;
      bool b2 = false;
      bool b3 = false;

      if ( !t[0].empty( ) ) {
         b1 = true;
         al1 = ComputeAxisLimitsForOneArray( t[0].GetVector( ) );
      }

      if ( !t[1].empty( ) ) {
         b2 = true;
         al2 = ComputeAxisLimitsForOneArray( t[1].GetVector( ) );
      }

      if ( !t[2].empty( ) ) {
         b3 = true;
         al3 = ComputeAxisLimitsForOneArray( t[2].GetVector( ) );
      }

      double minValue;
      double maxValue;

      if ( ( !b1 ) && ( !b2 ) && ( !b2 ) ) throw;

      if ( b1 && b2 && b3 ) {
         minValue = minNC( al1.first, al2.first, al3.first );
         maxValue = maxNC( al1.second, al2.second, al3.second );
      }
      else if ( b1 && b2 ) {
         minValue = std::min( al1.first, al2.first );
         maxValue = std::max( al1.second, al2.second );
      }
      else if ( b1 && b3 ) {
         minValue = std::min( al1.first, al3.first );
         maxValue = std::max( al1.second, al3.second );
      }
      else { //( b2 && b3 )
         minValue = std::min( al2.first, al3.first );
         maxValue = std::max( al2.second, al3.second );
      }

      return LinearAxis( ).LinearAxisLimits( minValue, maxValue );
   }

   std::string DrawXTics( void ) {
      const int shiftForDeltaTypeData = ( m_dataType==eDeltaDist23 || m_dataType==etanhDeltaDist23 ) ? 1 : 0;
      const int minX = m_triangleFollow.GetMinX( ) + shiftForDeltaTypeData;
      const int maxX = m_triangleFollow.GetMaxX( );
      const AxisLimits al( LinearAxis( ).LinearAxisLimits( minX, maxX ) );
      const double deltaPos = ( 0.9*m_plotWidth-m_borderWidth )/( al.m_numSteps );
      std::string svg;
      for( unsigned long i = 0; i <= al.m_numSteps; ++i ) {
         const int pos = int( m_borderWidth + i*deltaPos );
         svg += DrawOneXTicMark( pos ) + "\n";
      }
      return svg;
   }

   std::string DrawOneXTicMark( const double x ) {
      return
         LRL_ToString( "<line fill=\"none\" stroke=\"black\"" )
         + " stroke-width=" + ToQuotedString( SVG_DistancePlotConstants::globalDataAxisWidth )
         + " x1=" + ToQuotedString( x )
         + " y1=" + ToQuotedString( -SVG_DistancePlotConstants::globalY_AxisTicMarkLength+m_borderWidth )
         + " x2=" + ToQuotedString( x )
         + " y2=" + ToQuotedString( m_borderWidth )
         + " />";
   }

   std::string DrawOneYTicMark( const double y ) {
      return
         LRL_ToString( "<line fill=\"none\" stroke=\"black\"" )
         + " stroke-width=" + ToQuotedString( SVG_DistancePlotConstants::globalDataAxisWidth )
         + " x1=" + ToQuotedString( -SVG_DistancePlotConstants::globalY_AxisTicMarkLength+m_borderWidth )
         + " y1=" + ToQuotedString( y )
         + " x2=" + ToQuotedString( m_borderWidth )
         + " y2=" + ToQuotedString( y )
         + " />";
   }

   std::string Label_X_Axis( void ) {
      const int shiftForDeltaTypeData = ( m_dataType==eDeltaDist23 || m_dataType==etanhDeltaDist23 ) ? 1 : 0;
      const int minX = m_triangleFollow.GetMinX( ) + shiftForDeltaTypeData;
      const int maxX = m_triangleFollow.GetMaxX( );
      const AxisLimits al( LinearAxis( ).LinearAxisLimits( minX, maxX ) );
      const double deltaPos = ( 0.9*m_plotWidth-m_borderWidth )/( al.m_numSteps );
      const double deltaValue = double( al.m_upperLimit-al.m_lowerLimit ) / double( al.m_numSteps );
      std::string svg;
      for( unsigned long i = 0; i <= al.m_numSteps; ++i ) {
         const int pos = int( 2.0*m_borderWidth + double( i )*deltaPos );
         const double posValue = al.m_lowerLimit + i * deltaValue;
         svg += DrawOneX_AxisLabel( pos, 0.1, posValue ) + "\n";
      }
      return svg;
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawOneX_AxisLabel( const double xTic, const double y, const double xPos ) {
      return(
         "<text transform=\"translate(0,0)  scale(1,-1)\" text-anchor=\"middle\" font-family=\"sans-serif\" font-size="+ ToQuotedString( m_fontSize )
         + LRL_ToString( " x=\"", xTic-25, "\"" )
         + LRL_ToString( " y=\"", -y+25, "\" >" )
         + LRL_ToString( xPos, "</text>" ) );
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawOneY_AxisLabel( const double y, const double yscale, const double value ) {
      std::string returnString = LRL_ToString( value );
      if ( std::fmod( value, 1000.0 ) < 1.0 && std::abs( value )>999.9 )
         returnString = LRL_ToString( value/1000.0, "K" );
      else if ( returnString == "0" && value != 0.0 )
         returnString = SmallValueFormat( value );

      return(
         "<text transform=\"translate(0," + LRL_ToString( m_plotHeight ) + ") scale(1,-1)\" text-anchor=\"end\" font-family=\"sans-serif\" font-size=" + ToQuotedString( m_fontSize )
         + LRL_ToString( " x=\"", -SVG_DistancePlotConstants::globalY_AxisTicMarkLength+5, "\"" )
         + LRL_ToString( " y=\"", -y+10, "\" >" )
         + returnString + "</text>" );
   }

   std::string Label_Y_Axis( const ProgressData<double>data[3] ) {
      const AxisLimits al( ComputeAxisLimits( data ) );
      std::string svg;
      const int deltaPos = int( ( 0.9*m_plotHeight-m_borderWidth )/al.m_numSteps );
      const double deltaValue = double( al.m_upperLimit-al.m_lowerLimit )/double( al.m_numSteps );





      for( unsigned long i = 0; i < al.m_numSteps + 1; ++i ) {
         double ticValue = al.m_upperLimit - i*deltaValue;
         if ( std::abs( ticValue ) <0.00016 ) ticValue = 0.0;
         const int pos = int(-2.5*m_borderWidth -double( i )*(double)deltaPos);
         svg += DrawOneY_AxisLabel( pos, 0.1, ticValue ) + "\n";
      }
      return svg;
   }

   std::string DrawYTics( const ProgressData<double>data[3] ) {
      const AxisLimits al( ComputeAxisLimits( data ) );
      std::string svg;
      const int delta = int( 0.9*m_plotHeight-m_borderWidth )/al.m_numSteps;
      for( unsigned long i = 0; i < al.m_numSteps + 1; ++i ) {
         svg += DrawOneYTicMark( m_borderWidth+i*delta ) + "\n";
      }
      return svg;
   }

   std::string DrawTitle( const std::string& title ) {
      return
         "<text transform=\"scale(1,-1)\" text-anchor=\"start\" font-family=\"sans-serif\" font-size=" + ToQuotedString( m_fontSize )
         + " x=" + ToQuotedString( m_plotWidth/12 )
         + " y=" + ToQuotedString( -0.9*m_plotHeight - 2*m_borderWidth/2 ) + " >"
         + title + "</text>";
   }

   std::string DrawYaxis( ) {
      return
         "<line label=" + ToQuotedString( "Yaxis" )
         + " fill=" + ToQuotedString( "none" )
         + " stroke-width=" + ToQuotedString( SVG_DistancePlotConstants::globalG6DataLineStrokeWidth)
         + " stroke=" + ToQuotedString( "black" )
         + " x1=" + ToQuotedString( m_borderWidth )
         + " y1=" + ToQuotedString( m_borderWidth )
         + " x2=" + ToQuotedString( m_borderWidth )
         + " y2=" + ToQuotedString( 0.9*m_plotHeight ) + " />";
   }

   std::string DrawXaxis( ) {
      return
         "<line label=" + ToQuotedString( "Xaxis" )
         + " fill=" + ToQuotedString( "none" )
         + " stroke-width=" + ToQuotedString( SVG_DistancePlotConstants::globalG6DataLineStrokeWidth)
         + " stroke=" + ToQuotedString( "black" )
         + " x1=" + ToQuotedString( m_borderWidth )
         + " y1=" + ToQuotedString( m_borderWidth )
         + " x2=" + ToQuotedString( 0.9*m_plotWidth )
         + " y2=" + ToQuotedString( m_borderWidth ) + " />";
   }

   double CalculateYStartPos( const AxisLimits& alY ) {
      const double yDataHeight = ( 0.9*m_plotHeight-m_borderWidth )*( alY.m_maxDataValue-alY.m_minDataValue )/( alY.m_upperLimit-alY.m_lowerLimit );
      return m_borderWidth + yDataHeight*( alY.m_minDataValue-alY.m_lowerLimit )/( alY.m_maxDataValue-alY.m_minDataValue );
   }

   static std::string WriteOneErrorLine( const std::string& color, const int strokeWidth, const std::string& errorString ) {
      return LRL_ToString( "   <polyline fill=\"none\" stroke=", ToQuotedString( color ), " stroke-width=", ToQuotedString( strokeWidth ), " points=\" ",
         errorString, "\" />" );
   }

   static std::vector<std::string> WriteErrorStrings( const std::vector<std::string>& errorStrings ) {
      std::vector<std::string> svg;
      if ( !errorStrings.empty( ) ) {
         for( unsigned long i = 0; i < errorStrings.size( ); ++i ) {
            svg.push_back( WriteOneErrorLine( "black", 6 * SVG_DistancePlotConstants::globalG6DataLineStrokeWidth, errorStrings[i] ) );
            svg.push_back( WriteOneErrorLine( "yellow", 4 * SVG_DistancePlotConstants::globalG6DataLineStrokeWidth, errorStrings[i] ) );
         }
      }
      return svg;
   }

   std::string DataPointIsError( const std::string& dataPoint, const double x, const double y ) {
      std::string errorString;

      if ( m_dataType == eAngleDiff && y>= 180.0 )
         errorString += dataPoint;
      else if ( m_dataType == eTriangleViolation && y>=0.0 )
         errorString += dataPoint;
      else if ( m_dataType == eArea && y <= 0.0 )
         errorString += dataPoint;

      return errorString;
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::vector<std::string> FormatAllPointsAsPolyline( const AxisLimits& alX, const AxisLimits& alY, const ProgressData<double>& d, const double minValue, const double range, const std::string& color )
      /*-------------------------------------------------------------------------------------*/
   {
      const int xmax( int( d.size( ) ) );

      std::string allDatapointsAsString = LRL_ToString( "   <polyline fill=\"none\" stroke=", ToQuotedString( color ), " stroke-width=\"", SVG_DistancePlotConstants::globalG6DataLineStrokeWidth, "\" points=\" " );
      const double actualLineXLength = ( 0.9*m_plotWidth-m_borderWidth )*( alX.m_maxDataValue-alX.m_minDataValue )/( alX.m_upperLimit-alX.m_lowerLimit );
      const double dataLineXDelta = actualLineXLength/double( xmax-1 );
      const double xStartPos = dataLineXDelta*( alX.m_minDataValue-alX.m_lowerLimit );

      const double yStartPos = CalculateYStartPos( alY );

      double xpos = m_borderWidth + xStartPos;
      double ypos;
      const double yscale = ( 0.9*m_plotHeight-m_borderWidth )/( alY.m_upperLimit-alY.m_lowerLimit );
      std::string errorString;
      std::vector<std::string> errorSegments;

      bool previousPointIsError = false;
      for ( int i=0; i<xmax; ++i ) {
         ypos = yStartPos + yscale*( d[i]-minValue );
         const std::string dataPoint( LRL_ToString( xpos, ",", ypos, " " ) );
         allDatapointsAsString += dataPoint;
         const std::string nextErrorString = DataPointIsError(dataPoint, xpos, d[i]);
         const bool dataPointIsError = !nextErrorString.empty();
		   errorString += nextErrorString;

         if ( dataPointIsError || ( ( !previousPointIsError ) && dataPointIsError ) ) {
            errorString += dataPoint;
         }

         if ( previousPointIsError && !dataPointIsError ) { // end of series of error points
            errorSegments.push_back( errorString );
            errorString.clear( );
         }

         xpos += dataLineXDelta;
         previousPointIsError = dataPointIsError;
      }

      if ( !errorString.empty( ) ) { // finished wihout outputting the last error string
         errorSegments.push_back( errorString );
      }

      std::vector<std::string> svg( WriteErrorStrings( errorSegments ) );
      svg.push_back( allDatapointsAsString + "\" />\n" );
      return( svg );
   }

   std::vector<std::string> PlotLine( const AxisLimits& alX, const AxisLimits& alY, const ProgressData<double>& d, const double minValue, const double range,
      const std::string& color ) {
      return FormatAllPointsAsPolyline( alX, alY, d, minValue, range, color );
   }

   std::vector<std::string> PlotData( const ProgressData<double> t[3] ) {
      const std::pair<double, double> al1 = ComputeAxisLimitsForOneArray( t[0].GetVector( ) );
      const std::pair<double, double> al2 = ComputeAxisLimitsForOneArray( t[1].GetVector( ) );
      const std::pair<double, double> al3 = ComputeAxisLimitsForOneArray( t[2].GetVector( ) );

      const double minValue = minNC( al1.first, al2.first, al3.first );
      const double maxValue = maxNC( al1.second, al2.second, al3.second );


      const AxisLimits alY = LinearAxis( ).LinearAxisLimits( minValue, maxValue );
      const double range = alY.m_upperLimit - alY.m_lowerLimit;
      const int startingPoint = ( m_dataType==eDeltaDist23 ) ? 2 : 1;

      const AxisLimits alX = LinearAxis( ).LinearAxisLimits( startingPoint, t[0].size( ) );

      std::vector<std::string> v;
      const std::vector<std::string> s1( PlotLine( alX, alY, t[0], minValue, range, "blue" ) );
      const std::vector<std::string> s2( PlotLine( alX, alY, t[1], minValue, range, "orange" ) );
      const std::vector<std::string> s3( PlotLine( alX, alY, t[2], minValue, range, "black" ) );

      v.insert( v.end( ), s1.begin( ), s1.end( ) );
      v.insert( v.end( ), s2.begin( ), s2.end( ) );
      v.insert( v.end( ), s3.begin( ), s3.end( ) );
      return v;
   }



//         return "";

   std::vector<std::string> DrawGraph( const int xpos, const int ypos, const std::string& title, const ProgressData<double> data[3] ) {
      std::vector<std::string> svgFig;
      svgFig.push_back( std::string( "<!--  " ) + title + std::string( "  -->" ) );
      svgFig.push_back( DrawBoundaryRectangle( m_plotWidth, m_plotHeight ) );
      svgFig.push_back( DrawAxes( ) );
      svgFig.push_back( DrawTitle( title ) );
      svgFig.push_back( Label_X_Axis( ) );
      svgFig.push_back( DrawXTics( ) );
      svgFig.push_back( Label_Y_Axis( data ) );
      svgFig.push_back( DrawYTics( data ) );


      const std::vector<std::string> plotData( PlotData( data ) );
      svgFig.insert( svgFig.end( ), plotData.begin( ), plotData.end( ) );

      return PlaceSubPicture( xpos, ypos, svgFig );
   }

   std::vector<std::string> DrawLegend( const int xpos, const int ypos ) {
      std::vector<std::string> svgFig;
      std::vector<std::string> s;
      svgFig.push_back( DrawBoundaryRectangle( m_plotWidth, m_plotHeight ) );
      svgFig.push_back( DrawTitle( "LEGEND" ) );
      int line = 1;
      const int spacing = -50;
      s = PlaceSubPicture(0, spacing*(line++), DrawTitle("Triangles are all scaled to area of input!"));
      svgFig.insert(svgFig.end(), s.begin(), s.end());
      s = PlaceSubPicture(0, spacing*(line++), DrawTitle("Blue -- Input vector #1 fixed"));
      svgFig.insert(svgFig.end(), s.begin(), s.end());
      s = PlaceSubPicture( 0, spacing*( line++ ), DrawTitle( "-  #2 = Input 2, #3 = Input 3" ) );
      svgFig.insert( svgFig.end( ), s.begin( ), s.end( ) );
      s = PlaceSubPicture( 0, spacing*( line++ ), DrawTitle( "Orange -- Input vector #2 fixed" ) );
      svgFig.insert( svgFig.end( ), s.begin( ), s.end( ) );
      s = PlaceSubPicture( 0, spacing*( line++ ), DrawTitle( "-  #2 = Input 3, #3 = Input 1" ) );
      svgFig.insert( svgFig.end( ), s.begin( ), s.end( ) );
      s = PlaceSubPicture( 0, spacing*( line++ ), DrawTitle( "Black -- Input vector #3 fixed" ) );
      svgFig.insert( svgFig.end( ), s.begin( ), s.end( ) );
      s = PlaceSubPicture( 0, spacing*( line++ ), DrawTitle( "-  #2 = Input 1, #3 = Input 2" ) );
      svgFig.insert( svgFig.end( ), s.begin( ), s.end( ) );
      s = PlaceSubPicture( 0, spacing*( line++ ), DrawTitle( "Dist23 -- Distance between the two moving points" ) );
      svgFig.insert( svgFig.end( ), s.begin( ), s.end( ) );
      s = PlaceSubPicture( 0, spacing*( line++ ), DrawTitle( "" ) );
      svgFig.insert( svgFig.end( ), s.begin( ), s.end( ) );
      s = PlaceSubPicture(0, spacing*(line++), DrawTitle("Percent violation are percent of longest side"));
      svgFig.insert(svgFig.end(), s.begin(), s.end());
      s = PlaceSubPicture(0, spacing*(line++), DrawTitle("Impossible regions are surrounded by black bars"));
      svgFig.insert(svgFig.end(), s.begin(), s.end());
      s = PlaceSubPicture(0, spacing*(line++), DrawTitle("and filled by yellow"));
      svgFig.insert( svgFig.end( ), s.begin( ), s.end( ) );
      s = PlaceSubPicture( 0, spacing*( line++ ), DrawTitle( "See log file for data" ) );
      svgFig.insert( svgFig.end( ), s.begin( ), s.end( ) );
      return PlaceSubPicture( xpos, ypos, svgFig );
   }

   std::string FormatInputVectors( void ) const {
      return LRL_ToString(
         std::string( " inputVector1" ), m_triangleFollow.m_inputVector1,
         std::string( "\n inputVector2" ), m_triangleFollow.m_inputVector2,
         std::string( "\n inputVector3" ), m_triangleFollow.m_inputVector3 );
   }

   int Xpos( const int columnNum ) {
      return columnNum * ( 4*m_borderWidth + m_plotWidth );
   }

   int Ypos( const int rowNum ) {
      return -3*rowNum*m_borderWidth - ( rowNum + 1 )*m_plotHeight;
   }

   void MarkTriangleViolations( void ) {
      const int size12_0 = m_triangleFollow.m_dist12[0].size( );
      const int size12_1 = m_triangleFollow.m_dist12[1].size( );
      const int size12_2 = m_triangleFollow.m_dist12[2].size( );
      m_triangleViolations[0].resize( m_triangleFollow.m_dist12[0].size( ) );
      m_triangleViolations[1].resize( m_triangleFollow.m_dist12[1].size( ) );
      m_triangleViolations[2].resize( m_triangleFollow.m_dist12[2].size( ) );
      for( unsigned long i = 0; i < m_triangleFollow.m_dist12[0].size( ); ++i ) {
         const double d12 = m_triangleFollow.m_dist12[0][i];
         const double d13 = m_triangleFollow.m_dist13[1][i];
         const double d23 = m_triangleFollow.m_dist23[2][i];
         m_triangleViolations[0][i] = ( 2.0*maxNC( d12, d13, d23 ) - d12 - d13 - d23 ) < 0.0;
         m_triangleViolations[1][i] =  d13-d12-d23 < 0.0;
         m_triangleViolations[2][i] =  d23-d12-d13 < 0.0;
      }
   }

   std::string FormatWorstCase( const triple<TVEC, TVEC, TVEC>& worstTriple ) {
      const triple<TVEC, TVEC, TVEC> worstCaseFound( m_triangleFollow.GetWorstTriple( ) );
      const D7& d1 = worstCaseFound.first ;
      const D7& d2 = worstCaseFound.second;
      const D7& d3 = worstCaseFound.third ;
      const double worstd12 = TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD >::DIST(d1, d2);
      const double worstd13 = TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD >::DIST(d1, d3);
      const double worstd23 = TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD >::DIST(d2, d3);
      const double worstTriangleViolation = 100.0*FollowerTools::TriangleInequality( worstd12, worstd13, worstd23 );

      std::string s;
      s += LRL_ToString( "worst case ==================     ", worstTriangleViolation, "%\n" );
      s += LRL_ToString( "worst case distances, (12), (13), (23)  ", LRL_ToString( worstd12 ), " ", LRL_ToString( worstd13 ), " ", LRL_ToString( worstd23 ), "%\n" );
      s += LRL_ToString( worstCaseFound.first, "\n" );
      s += LRL_ToString( worstCaseFound.second, "\n" );
      s += LRL_ToString( worstCaseFound.third, "\n" );
      return s;
   }

   void OutputSVG( const std::vector<std::string>& v ) {
      std::ofstream svgOut;

      svgOut.open( m_OutputFilename.c_str( ), std::ios::out | std::ios::app | std::ios::ate );

      for( unsigned long i = 0; i < v.size( ); ++i ) {
         svgOut << v[i] << std::endl;
      }
   }

   std::string DrawAxes( ) {
      const double start = 0.1*std::min( m_plotWidth, m_plotHeight );
      const double yEnd = 0.9*m_plotHeight;
      const double xEnd = 0.9*m_plotWidth;

      return DrawXaxis( ) + "\n" + DrawYaxis( );
   }

   std::string DrawBoundaryRectangle( const int width, const int height ) {
         return DrawRectangle(0, 0, width, height);
   }
};
#endif
