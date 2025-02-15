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
      const DistanceLineDescriptions& lineDescriptions, const double timeToComputeFrame)
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
   void SVG_Footer( void ) {
      m_svg.push_back("-->\n</g>\n</g>");
      m_svg.push_back( "<!-- Run Constants" );
      m_svg.push_back( GLOBAL_Report::globalDataReport );
      m_svg.push_back( "-->" );
      m_svg.push_back( "</svg>" );
   };

   std::string WriteOneLineDescription( const std::string& name, const int height ) {
      std::string svg;

      const std::string timestamp = (GLOBAL_Files::globalShouldTimeStamp ?
         LRL_ToString( m_lineDescriptions.GetComputeTime( name ) ) +
         " msec " : "") + "</text>\n";

      if (FollowerConstants::IsEnabled( name )) {
         const std::string colorForName = m_lineDescriptions.GetColor( name );
         const std::string nameForType = m_lineDescriptions.GetName( name );


          const std::string dashmode = m_lineDescriptions.GetDashMode(name);
          const std::string lineWidth = LRL_ToString(m_lineDescriptions.GetLineWidth(name));
          const std::string strokeWidth = " stroke-width=" + ToQuotedString(lineWidth) + "";

         const std::string yheight = LRL_ToString(height);
         svg += "<text x=\"300\" y=\"" +
            yheight +
            "\"   font-family=\"sans-serif\" font-size=\"20\" fill=\"" +
            colorForName + "\">  " +
            nameForType + "  " + timestamp;
         svg += LRL_DataToSVG("\n   <line  x1=\"600\"   y1=", ToQuotedString(yheight), " x2 = \"900\" y2=", ToQuotedString(yheight));
         svg += LRL_DataToSVG(" fill=\"none\" stroke=\"", colorForName, "\"");
         svg += LRL_DataToSVG(strokeWidth, " stroke-dasharray = \"", dashmode, "\"></line> \n");
      }
      return svg;
   }

   std::string WriteLineDescriptions() {
      std::string svg;
      int y_pos = 60;
      const int y_delta = 25;
      const std::string svgs6 = WriteOneLineDescription("S6", y_pos );
      if (!svgs6.empty()) y_pos += y_delta;
      const std::string svgg6 = WriteOneLineDescription("G6", y_pos );
      if (!svgg6.empty()) y_pos += y_delta;
      const std::string svgd7 = WriteOneLineDescription("D7", y_pos);
      if (!svgd7.empty()) y_pos += y_delta;
      const std::string svgcs = WriteOneLineDescription("CS", y_pos);
      if (!svgcs.empty( )) y_pos += y_delta;
      const std::string svgv7 = WriteOneLineDescription( "V7", y_pos );
      if (!svgv7.empty( )) y_pos += y_delta;
      const std::string svglm = WriteOneLineDescription("LM", y_pos);
      if (!svglm.empty()) y_pos += y_delta;
      const std::string svgdc = WriteOneLineDescription("DC", y_pos);
      if (!svgdc.empty()) y_pos += y_delta;
      return svgs6 + svgg6 + svgd7 + svgcs + svgv7 + svglm + svgdc;
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void SVG_Header( const std::string& sFileName, const int startingVectorCounter ) {
      m_svg.push_back( "<?xml version=\"1.0\" standalone=\"no\"?>" );
      SVG_HeaderBoilerplate( sFileName );

      
      const std::string timestamp = (GLOBAL_Files::globalShouldTimeStamp ? 
      "  &#160;&#160;&#160;&#160;&#160; Wall time passed: "
         + LRL_ToString( m_computeTime )
         + " msec." : "" ) + "</text>";

       m_svg.push_back(" <text x=\"200\"  y=\"30\"  font-family=\"sans-serif\" font-size=\"20\" >" + sFileName +
       timestamp);

     m_svg.push_back(WriteLineDescriptions());
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string ScaleToGraph( const size_t n, const size_t totalItemCount ) const {
      const size_t placement = (size_t)( SVG_WriterConstants::globalGraphBorder +
         n  * (size_t)( SVG_WriterConstants::globalGraphSpace / double( totalItemCount ) ) );
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

      // CHANGE THE SCALE FACTOR YOU'D LIKE TO GET SMALL (OR LARGER IMAGES). WATCH OUT FOR 
      // THE WIDTH, HEIGHT, AND VIEWBOX SIZES ABOVE IF YOU MAKE IT LARGER!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      const double scaleFactor = 1.0;
       //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      m_svg.push_back( "<!-- the width and height set the total pixel output of the image in the browser -->" );
      m_svg.push_back( "<!-- whatever is within the viewbox will be remapped to show in this size -->" );

      const double& width = SVG_WriterConstants::globalGraphSpace;
      const std::string scaledWidth = LRL_DataToSVG(scaleFactor * width * 1.2);
      m_svg.push_back("<svg width=\"" + LRL_ToString(scaleFactor * width) + "\" height=\"" + LRL_ToString(scaleFactor * width) +
         "\" viewBox=\"1 0  " + LRL_ToString(scaleFactor * width * 1.2, scaleFactor * width * 1.2) +
         "\"  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">");
      m_svg.push_back(" <desc>ID (and file name) = " + sFileName + "  mode = " + ReadGlobalData::GetFollowerMode() + " </desc> ");

      m_svg.push_back("<g transform = \"scale(" + LRL_ToString(scaleFactor, scaleFactor) + ")\">\n");

      m_svg.push_back("<text x = \"-850\" y=\"50\" transform=\"rotate(-90)\"  font-family=\"sans-serif\" font-size=\"40\" stroke =\"lightgray\" fill=\"none\"> PointDistanceFollower   " __DATE__ "</text>");
      m_svg.push_back(" <rect fill = \"rgb(255,255,255)\" width = \"" + scaledWidth + "\" height = \"" +
         scaledWidth + "\" />");
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void SendFrameToFile( const std::string& sFileName ) {
      std::ofstream fileout;
      FileOperations::OpenOutputFile( fileout, sFileName.c_str( ) );

      if (fileout.is_open( ))
      {
         fileout.seekp(0);
         for (std::list<std::string>::const_iterator it = m_svg.begin( ); it != m_svg.end( ); ++it)
            fileout << (*it) << std::endl;
      }
      else
         std::cout << "Could not open file " << sFileName << " for write in SVGWriter.h" << std::endl;

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
