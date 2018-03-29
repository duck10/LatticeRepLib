#if _MSC_VER
#pragma once
#endif
#ifndef SVG_Writer_H
#define SVG_Writer_H

#include <algorithm>
#include <cassert>
#include <cfloat>
#include <climits>
#include <fstream>
#include <iostream>
#include <list>
#include <set>
#include <utility>

#include "FileOperations.h"
#include "Follow.h"
#include "FollowerConstants.h"
#include "FollowerIO.h"
#include "Glitch.h"
#include "MapBoundaryStrings2Colors.h"
#include "LRL_ToString.h"
#include "LRL_Cell_Degrees.h"
#include "SVG_CirclePlot.h"
#include "SVG_DistancePlot.h"
#include "SVG_Writer.h"
#include "triple.h"

class MapBoundaryStrings2Colors;

template<typename TVEC, typename TMAT, typename TREDUCE>
class SVG_Writer
{
public:

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   SVG_Writer( const std::string&               sFileName,
      Follow<TVEC, TMAT,TREDUCE>&                    follow,
      const int                        startingVectorCounter,
      MapBoundaryStrings2Colors& colorMap,
      const std::vector<Glitch<TVEC> >&       glitches,
      const std::vector<int>&          rejectedFrames,
      const int                        ordinalBeingWorkedOn,
      const int fractionWalked = -1,
      const int numberToWalk = 0 )
      : m_follow( follow )
      , m_which2plot( follow.GetWhichToPlot( ) )
      /*-------------------------------------------------------------------------------------*/
   {
      if (FollowerConstants::globalDrawCirclePlot || FollowerConstants::globalDrawDistancePlot) {
         FollowerIO::OutputInfo( sFileName + "\n" );

         SVG_Header( sFileName, startingVectorCounter );
         std::list<std::string> svgProgressBars( DrawProgressBars( ordinalBeingWorkedOn, fractionWalked, numberToWalk, rejectedFrames ) );
         m_svg.splice( m_svg.end( ), svgProgressBars );
         std::list<std::string> svgDistancePlot( SVG_DistancePlot<TVEC, TMAT,TREDUCE>( follow, colorMap, sFileName, glitches ).Format( ) );
         m_svg.splice( m_svg.end( ), svgDistancePlot );
         std::list<std::string> svgCirclePlot( SVG_CirclePlot<TVEC, TMAT, TREDUCE>( follow, colorMap, sFileName ).Format( ) );
         m_svg.splice( m_svg.end( ), svgCirclePlot );

         SVG_Footer( );
         EmitSVG( sFileName );
      }
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   // send the now-constructed SVG to the output file
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void EmitSVG( const std::string& sFileName ) {
      if (FollowerConstants::globalOutputGlitchesOnly && m_follow.GetGlitches().empty())
      {
      }
      else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalWeb)
         SendFrameTo_stdout( );
      else
         SendFrameToFile( sFileName );

      m_svg.clear( );
   }


private:

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static bool Not_INT_MAX( const int n ) {
      return(n != INT_MAX);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static bool Is_INT_MAX( const int n ) {
      return(n == INT_MAX);
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static double Progress( const unsigned long position, const unsigned long totalCount ) {
      return(800.0 * (double)( position + 1 ) / (double)(std::max(( 1UL, totalCount ) ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::vector<int> DetermineWhichFramesToReject( const std::vector<int>& rejectedFrames ) const {
      std::vector<int> rejects( rejectedFrames );

      for( unsigned long i = 1; i != rejectedFrames.size( ) - 1; ++i)
      if ((rejectedFrames[i - 1] == rejectedFrames[i] - 1) &&
         (rejectedFrames[i + 1] == rejectedFrames[i] + 1))
         rejects[i] = INT_MAX;

      return(rejects);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   static std::vector<int> BuildListOfRejectedRegions( const std::vector<int>& rejects ) {
      std::vector<int>::const_iterator it = rejects.begin( );
      std::vector<int>::const_iterator it2 = rejects.end( );
      const std::vector<int>::const_iterator rejectEnd = rejects.end( );
      std::vector<int> rejectsOutput;

      while (it != rejectEnd && it + 1 != rejectEnd) {
         it2 = std::find_if( it + 1, rejectEnd, Not_INT_MAX );
         rejectsOutput.push_back( *it );
         rejectsOutput.push_back( (it2 - it == 1) ? *it : *it2 );
         it = (it2 == rejectEnd) ? it : it2;
      }

      return(rejectsOutput);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::vector<int> FilterRejectedFrameList( const std::vector<int>& rejectedFrames ) const {
      return(BuildListOfRejectedRegions( DetermineWhichFramesToReject( rejectedFrames ) ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   // Output which two variables will be plotted in the circle plot
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string GetWhichToPlot( const std::pair<int, int>& which2plot ) {
      return(LRL_ToString( "Plotting g" ) + LRL_ToString( which2plot.second + 1 ) + LRL_ToString( " vs. g" ) + LRL_ToString( which2plot.first + 1 ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawTitleOfWhichG6ElementsToPlot( ) {
      return
         "<text x=\"450\" y=\"30\" font-family=\"sans-serif\" font-size=\"20\" fill=\"black\" > " +
         GetWhichToPlot( m_which2plot ) +
         "</text>";
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void SVG_Footer( void ) {
      m_svg.push_back( "<!-- Run Constants" );
      m_svg.push_back( GLOBAL_Report::globalDataReport );
      m_svg.push_back( "-->" );
      m_svg.push_back( "</svg>" );
   };


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void SVG_Header( const std::string& sFileName, const int startingVectorCounter ) {
      m_svg.push_back( "<?xml version=\"1.0\" standalone=\"no\"?>" );
      SVG_HeaderBoilerplate( sFileName );
      SVG_HeaderFrameDescription( startingVectorCounter );
      m_svg.push_back( DrawTitleOfWhichG6ElementsToPlot( ) );
      m_svg.push_back( DrawStartingEndingCells( ) );
      m_svg.push_back( DrawFirstGlitch( ) );
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawOverallProgressBar( const int counter, const unsigned long totalListCount ) const {
      const std::string strEndY_Progress( LRL_ToString( 100 + Progress( counter - 1, totalListCount ) ) );

      return(
         std::string( "<!-- Overall Progress Bar -->\n" ) + std::string( "<g>\n" )
         + "   <line x1=\"990\" y1=\"100\" x2=\"990\" y2=\"" + strEndY_Progress + "\" stroke-width=\"10\" fill=\"black\" stroke=\"black\" />\n"
         + "</g>");
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawOverallProgressBarBase( const unsigned long totalListCount ) const
      /*-------------------------------------------------------------------------------------*/
   {
      std::string str = std::string( "<!-- Overall Progress Bar (base) -->\n" ) + std::string( "<g>\n" );
      std::string previousSegmentEnd = "100";

      for( unsigned long i = 0; i < totalListCount; ++i)
      {
         const std::string strEndY_ProgressSegment = LRL_ToString( 100 + Progress( i, totalListCount ) );
         str += OnePieceOfProgressBar( i, totalListCount, previousSegmentEnd );
         previousSegmentEnd = strEndY_ProgressSegment;
      }
      str += "</g>";

      return(str);
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawSegmentProgressBar( const int ordinalBeingWorkedOn ) const {
      return(
         std::string( "<!-- Segment Progress Bar Progress -->\n" )
         + "<g>\n"
         + "   <line x1=\"970\" y1=\"100\" x2=\"970\" y2=\""
         + ScaleToGraph( ordinalBeingWorkedOn, FollowerConstants::globalFramesPerSegment )
         + "\" stroke-width=\"10\" fill=\"black\"     stroke=\"black\" />\n"
         + "</g>");
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawSegmentProgressBarBase( void ) const {
      return(
         std::string( "<!-- Segment Progress Bar Base -->\n" )
         + "<g>\n"
         + "   <line x1=\"970\" y1=\"100\" x2=\"970\" y2=\""
         + ScaleToGraph(FollowerConstants::globalFramesPerSegment, FollowerConstants::globalFramesPerSegment )
         + "\" stroke-width=\"10\" fill=\"lightgrey\" stroke=\"lightgrey\" />\n"
         + "</g>");
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawStartingEndingCells( void ) const {
      TVEC vec = m_follow.GetProbe( );
      std::string s = "<text x=\"300\" y=\"60\"  font-family=\"sans-serif\" font-size=\"20\" > Beginning vector  " + LRL_ToString( vec ) + "</text>\n";
                 s += "<text x=\"300\" y=\"85\"  font-family=\"sans-serif\" font-size=\"20\" > Beginning cell  " + LRL_ToString( LRL_Cell(vec) ) + "</text>\n";
      vec = m_follow.GetSecondProbe( );
      s += "<text x=\"300\" y=\"110\"  font-family=\"sans-serif\" font-size=\"20\" > Ending vector  " + LRL_ToString(vec) + "</text>\n";
      s += "<text x=\"300\" y=\"135\"  font-family=\"sans-serif\" font-size=\"20\" > Ending cell  " + LRL_ToString(LRL_Cell_Degrees(vec)  ) + "</text>\n";
      return(s);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawFirstGlitch( ) const {
      std::string s;
      const std::vector<Glitch<TVEC> > glitchList = m_follow.GetGlitches( );
      if (!glitchList.empty( ))
      {
         TVEC vG6;
         vG6 = (*glitchList.begin( )).GetGlitchElement1( ).GetVector( );
         const double dist1 = (*glitchList.begin( )).GetGlitchDistance1( );
         const double dist2 = (*glitchList.begin( )).GetGlitchDistance2( );
         s += "<text x=\"300\" y=\"160\"  font-family=\"sans-serif\" font-size=\"20\" > Glitch  " + LRL_ToString( vG6 ) + " (" + LRL_ToString( dist1 ) + " )</text>\n";
         vG6 = (*glitchList.begin( )).GetGlitchElement2( ).GetVector( );
         s += "<text x=\"300\" y=\"185\"  font-family=\"sans-serif\" font-size=\"20\" > Glitch  " + LRL_ToString( vG6 ) + " (" + LRL_ToString( dist2 ) + ")</text>\n";
      }

      return(s);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawSegmentProgressBarRejectedFrames( const std::vector<int>& rejectedFrames ) const
      /*-------------------------------------------------------------------------------------*/
   {
      const std::vector<int> rejects = FilterRejectedFrameList( rejectedFrames );
      std::string str;

      for( unsigned long i = 0; i < rejects.size( ); i += 2)
         str += DrawSingleRejectedSegment( rejects[i], rejects[i + 1] );

      return(str);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string ScaleToGraph( const unsigned long n, const unsigned long totalItemCount ) const {
      const unsigned long placement = (unsigned long)( SVG_WriterConstants::globalGraphBorder +
         n  * (unsigned long)( SVG_WriterConstants::globalGraphSpace / double( totalItemCount ) ) );
      return(LRL_ToString( placement ));
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string DrawSingleRejectedSegment( const unsigned long reject1, const unsigned long reject2 ) const
      /*-------------------------------------------------------------------------------------*/
   {
      if (reject1 >= 0 && reject1 != reject2){
         std::string str;
         str += "<!-- Segment Rejection Bar -->\n";
         str += "<g>\n";
         str += "   <line x1=\"970\" y1=\"" +
            ScaleToGraph( reject1, FollowerConstants::globalFramesPerSegment ) + "\" x2=\"970\" y2=\"" +
            ScaleToGraph( reject2 + 1UL, FollowerConstants::globalFramesPerSegment ) +
            "\" stroke-width=\"16\" fill=\"red\" stroke=\"red\"  opacity =\"0.4\" />\n";
         str += "</g>\n";
         return(str);
      }
      else {
         return("");
      }
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void SVG_HeaderBoilerplate( const std::string& sFileName ) {
      //   m_svg.push_back( "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" " );
      //   m_svg.push_back( "  \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"> " );
      /*
      recommended at https://jwatt.org/svg/authoring/
      <svg xmlns="http://www.w3.org/2000/svg"
      xmlns:xlink="http://www.w3.org/1999/xlink"
      xmlns:ev="http://www.w3.org/2001/xml-events">
      */
      m_svg.push_back( "<!-- the width and height set the total pixel output of the image in the browser -->" );
      m_svg.push_back( "<!-- whatever is within the viewbox will be remapped to show in this size -->" );
      m_svg.push_back( "<svg width=\"1000\" height=\"1000\" viewBox=\"1 0 1000 1000\"  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">" );
      m_svg.push_back( " <desc>ID (and file name) = " + sFileName + " </desc> " );
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void SVG_HeaderFrameDescription( const int startingVectorCounter ) {
      if (startingVectorCounter >= 0) {
         m_svg.push_back( " <text x=\"280\"  y=\"30\"  font-family=\"sans-serif\" font-size=\"20\" > Trial vector = " + LRL_ToString( startingVectorCounter ) + "  </text> " );
         m_svg.push_back( " <text x=\"700\" y=\"30\" font-family=\"sans-serif\" font-size=\"20\">Wall time passed: "
            + LRL_ToString( m_follow.GetTime2ComputerFrame( ) )
            + " msec.</text>" );
      }
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void SendFrameToFile( const std::string& sFileName ) {
      std::ofstream fileout;
      FileOperations::OpenOutputFile( fileout, sFileName.c_str( ) );

      if (fileout.is_open( ))
      {
         for (std::list<std::string>::const_iterator it = m_svg.begin( ); it != m_svg.end( ); ++it)
            fileout << (*it) << std::endl;
      }
      else
         std::cout << "Could not open file " << sFileName << " for write" << std::endl;

      fileout.close( );
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void SendFrameTo_stdout( ) const {
      std::list<std::string>::const_iterator it;
      for (it = m_svg.begin( ); it != m_svg.end( ); ++it)
         std::cout << (*it) << std::endl;
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string OnePieceOfProgressBar( const unsigned long i, const unsigned long totalListCount, const std::string& previousSegmentEnd ) const
   {
      const std::string strEndY_ProgressSegment = LRL_ToString( 100 + Progress( i, totalListCount ) );
      const std::string color = (i % 2 == 0) ? "lightgrey" : "lightgreen";
      std::string str;
      return("   <line x1=\"990\" y1=\"" + previousSegmentEnd + "\" x2=\"990\" y2=\"" + strEndY_ProgressSegment +
         "\" stroke-width=\"20\" fill=\"" + color + "\" stroke=\"" + color + "\"     />\n");
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::list<std::string> DrawProgressBars( const int ordinalBeingWorkedOn,
      const int segmentsCompleted,
      const int numberToWalk,
      const std::vector<int>& rejectedFrames ) const
      /*-------------------------------------------------------------------------------------*/
   {
      std::list<std::string> svgList;
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalMovie && ordinalBeingWorkedOn >= 0)
      {
         svgList.push_back( DrawOverallProgressBarBase( numberToWalk ) );
         svgList.push_back( DrawOverallProgressBar( segmentsCompleted, numberToWalk ) );
         svgList.push_back( DrawSegmentProgressBarBase( ) );
         svgList.push_back( DrawSegmentProgressBar( ordinalBeingWorkedOn ) );
         svgList.push_back( DrawSegmentProgressBarRejectedFrames( rejectedFrames ) );
      }
      return(svgList);
   }

   std::list<std::pair<TVEC, TVEC> >                    m_path;
   std::list<std::string>                                          m_svg;
   std::pair<double, double>                                        m_viewboxOrigin;
   std::pair< std::pair<double, double>, std::pair<double, double> > m_MinMax;
   double                                                          m_scale;
   std::pair<double, double>                                        m_viewboxXYSize;
   const std::pair<int, int>                                        m_which2plot;
   const std::string                                               m_fileName;
   const Follow<TVEC, TMAT,TREDUCE>&                                        m_follow;

private: // don't implement
   SVG_Writer& operator=(const SVG_Writer&) const;
   SVG_Writer( const SVG_Writer& );
};


#endif
