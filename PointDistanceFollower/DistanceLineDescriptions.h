#ifndef DISTANCELINEDESCRIPTION_H
#define DISTANCELINEDESCRIPTION_H

#include <map>
#include <string>

#include "FollowerConstants.h"

class DistanceLineDescriptions {
public:
   class Line {
   public:
      Line(){}
      Line(const std::string& name, const std::string& color, const int lineWidth)
      : m_name(name)
      , m_color(color)
      , m_lineWidth(lineWidth)
      {
         //svgS6 lineWidth - 3,  FollowerConstants::globalColorsForBoundaries[1]);
         //svgG6 lineWidth - 6,  FollowerConstants::globalColorsForBoundaries[3]);
         //svgD7 lineWidth - 9,  FollowerConstants::globalColorsForBoundaries[10]);
         //svgCS lineWidth - 12,  FollowerConstants::globalColorsForBoundaries[15]);

      }

      std::string m_name;
      std::string m_color;
      int m_lineWidth;
   };


public:
   DistanceLineDescriptions(void) {
   /*
   std::string BASIC_COLORS[] = { "red", "lightblue", "turquoise", "slategrey",
   "orange", "blueviolet", "coral", "saddlebrown", "blue", "pink", "violet",
   "deeppink", "mediumvioletred", "tomato", "greenyellow", "olive" };
   };
   */
      m_lineColorMap.insert(std::make_pair("S6", Line("S6", "lightblue", 12)));
      m_lineColorMap.insert(std::make_pair("G6", Line("NCDist", "slategrey", 9)));
      m_lineColorMap.insert(std::make_pair("D7", Line("D7", "violet", 6)));
      m_lineColorMap.insert(std::make_pair("CS", Line("CS6Dist", "olive", 3)));
   }

   DistanceLineDescriptions( const DistanceLineDescriptions& dld) {
      m_lineColorMap = dld.m_lineColorMap;
   }
   
   DistanceLineDescriptions operator= ( const DistanceLineDescriptions& dld ) {
      m_lineColorMap = dld.m_lineColorMap;
      return *this;
   }

   std::string GetColor(const std::string& name) { return m_lineColorMap[name].m_color; }
   std::string GetName(const std::string& name) { return m_lineColorMap[name].m_name; }
   int GetLineWidth(const std::string& name) { return m_lineColorMap[name].m_lineWidth; }

private:
   std::map<std::string, Line> m_lineColorMap;
};


#endif  //  DISTANCELINEDESCRIPTION_H