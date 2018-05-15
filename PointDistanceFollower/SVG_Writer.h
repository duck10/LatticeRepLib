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
#include "FollowerConstants.h"
#include "FollowerIO.h"
#include "LRL_ToString.h"
#include "LRL_Cell_Degrees.h"
#include "SVG_DistancePlot.h"
#include "SVG_Writer.h"
#include "triple.h"

class MapBoundaryStrings2Colors;

template<typename TVEC>
class SVG_Writer
{
public:

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   SVG_Writer(const std::string& sFileName, std::list<std::string> svgDistancePlot,
      const DistanceLineDescriptions& lineDescriptions, double timeToComputeFrame)
      : m_computeTime(timeToComputeFrame)
      , m_lineDescriptions(lineDescriptions)
   {
      SVG_Header(sFileName, 19197);
      m_svg.splice(m_svg.end(), svgDistancePlot);
      SVG_Footer();
      EmitSVG(sFileName);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   // send the now-constructed SVG to the output file
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void EmitSVG( const std::string& sFileName ) {
         SendFrameToFile( sFileName );
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
      return 800.0 * (double)( position + 1 ) / (double)(std::max( 1UL, totalCount ));
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
   void SVG_Footer( void ) {
      m_svg.push_back("-->\n</g>\n</g>");
      m_svg.push_back( "<!-- Run Constants" );
      m_svg.push_back( GLOBAL_Report::globalDataReport );
      m_svg.push_back( "-->" );
      m_svg.push_back( "</svg>" );
   };

   std::string WriteLineDescriptions() {
      std::string svg;
      svg += "<text x=\"300\" y=\"60\"   font-family=\"sans-serif\" font-size=\"20\" fill=\"" + m_lineDescriptions.GetColor("S6") + "\">" + m_lineDescriptions.GetName("S6") + "  " + m_lineDescriptions.GetColor("S6") + "</text>\n";
      svg += "<text x=\"300\" y=\"85\"   font-family=\"sans-serif\" font-size=\"20\" fill=\"" + m_lineDescriptions.GetColor("G6") + "\">" + m_lineDescriptions.GetName("G6") + "  " + m_lineDescriptions.GetColor("G6") + "</text>\n";
      svg += "<text x=\"300\" y=\"110\"  font-family=\"sans-serif\" font-size=\"20\" fill=\"" + m_lineDescriptions.GetColor("D7") + "\">" + m_lineDescriptions.GetName("D7") + "  " + m_lineDescriptions.GetColor("D7") + "</text>\n";
      svg += "<text x=\"300\" y=\"135\"  font-family=\"sans-serif\" font-size=\"20\" fill=\"" + m_lineDescriptions.GetColor("CS") + "\">" + m_lineDescriptions.GetName("CS") + "  " + m_lineDescriptions.GetColor("CS") + "</text>\n" ;
      return svg;
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void SVG_Header( const std::string& sFileName, const int startingVectorCounter ) {
      m_svg.push_back( "<?xml version=\"1.0\" standalone=\"no\"?>" );
      SVG_HeaderBoilerplate( sFileName );
      SVG_HeaderFrameDescription( startingVectorCounter );
      m_svg.push_back(WriteLineDescriptions());
      m_svg.push_back(" <text x=\"140\"  y=\"30\"  font-family=\"sans-serif\" font-size=\"20\" >" + sFileName +"  </text> ");
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string ScaleToGraph( const unsigned long n, const unsigned long totalItemCount ) const {
      const unsigned long placement = (unsigned long)( SVG_WriterConstants::globalGraphBorder +
         n  * (unsigned long)( SVG_WriterConstants::globalGraphSpace / double( totalItemCount ) ) );
      return(LRL_ToString( placement ));
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
      m_svg.push_back( "<svg width=\"900\" height=\"800\" viewBox=\"1 0 900 800\"  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">" );
      m_svg.push_back( " <desc>ID (and file name) = " + sFileName + " </desc> " );
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void SVG_HeaderFrameDescription( const int startingVectorCounter ) {
      if (startingVectorCounter >= 0) {
         //m_svg.push_back( " <text x=\"280\"  y=\"30\"  font-family=\"sans-serif\" font-size=\"20\" > Trial vector = " + LRL_ToString( startingVectorCounter ) + "  </text> " );
         m_svg.push_back( " <text x=\"560\" y=\"30\" font-family=\"sans-serif\" font-size=\"20\">Wall time passed: "
            + LRL_ToString(m_computeTime)
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

   std::list<std::pair<TVEC, TVEC> >                                 m_path;
   std::list<std::string>                                            m_svg;
   std::pair<double, double>                                         m_viewboxOrigin;
   std::pair< std::pair<double, double>, std::pair<double, double> > m_MinMax;
   double                                                            m_scale;
   std::pair<double, double>                                         m_viewboxXYSize;
   const std::pair<int, int>                                         m_which2plot;
   const std::string                                                 m_fileName;
   const double                                                      m_computeTime;
   DistanceLineDescriptions                                          m_lineDescriptions;
private: // don't implement
   SVG_Writer& operator=(const SVG_Writer&) const;
   SVG_Writer( const SVG_Writer& );
};


#endif
