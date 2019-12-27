#ifndef DISTANCELINEDESCRIPTION_H
#define DISTANCELINEDESCRIPTION_H

#include <map>
#include <string>

#include "ColorTables.h"
#include "FollowerConstants.h"

class DistanceLineDescriptions {
public:
   class Line {
   public:
      Line(){}
      Line(const std::string& name, const std::string& color, const int lineWidth, const double time, const std::string& dashmode)
         : m_name(name)
         , m_color(color)
         , m_lineWidth(lineWidth)
         , m_time2Compute(time)
         , m_dashMode(dashmode)
      {
      }

      void SetTime( const double time ) {
         m_time2Compute = time;
      }

      std::string m_name;
      std::string m_color;
      int m_lineWidth;
      double m_time2Compute;
      std::string m_dashMode;
   };


public:
   DistanceLineDescriptions( void ) {
      m_lineColorMap.insert( std::make_pair( "S6", Line( "S6", "lightblue", 12, -1.0, "10,5" ) ) );
      m_lineColorMap.insert( std::make_pair( "G6", Line( "NCDist", "slategrey", 9, -1.0, "" ) ) );
      m_lineColorMap.insert( std::make_pair( "D7", Line( "D7", "violet", 6, -1.0, "" ) ) );
      m_lineColorMap.insert( std::make_pair( "CS", Line( "CS6Dist", "olive", 3, -1.0, "5,10" ) ) );
      m_lineColorMap.insert( std::make_pair( "V7", Line( "V7", "orange", 3, -1.0, "" ) ) );
      m_lineColorMap.insert( std::make_pair( "LM", Line( "LM", "black", 3, -1.0, "" ) ) );
      m_lineColorMap.insert( std::make_pair( "INVALID", Line( "INVALID", "orange", 3, -1.0, "10,10" ) ) );


      //size_t startcolor = 0;
      //size_t delcolor = 255 / 5;
      //m_lineColorMap.insert(std::make_pair("S6", Line("S6", ColorTables::GetCividisHEX(startcolor), 12, -1.0, "10,5")));
      //m_lineColorMap.insert(std::make_pair("G6", Line("NCDist", ColorTables::GetCividisHEX(startcolor + 1 * delcolor), 9, -1.0, "")));
      //m_lineColorMap.insert(std::make_pair("D7", Line("D7", ColorTables::GetCividisHEX(startcolor + 2 * delcolor), 6, -1.0, "")));
      //m_lineColorMap.insert(std::make_pair("CS", Line("CS6Dist", ColorTables::GetCividisHEX(startcolor + 3 * delcolor), 3, -1.0, "5,10")));
      //m_lineColorMap.insert( std::make_pair( "V7", Line( "V7", ColorTables::GetCividisHEX( startcolor + 4 * delcolor ), 3, -1.0, "" ) ) );
      //m_lineColorMap.insert( std::make_pair( "LM", Line( "LM", ColorTables::GetCividisHEX( startcolor + 5 * delcolor ), 3, -1.0, "" ) ) );
      //m_lineColorMap.insert(std::make_pair("INVALID", Line("INVALID", "orange", 3, -1.0, "10,10")));
   }

   void SetComputeTime( const std::string name, const double time ) {
      m_lineColorMap[name].SetTime(time);
   }

   void clear(void) { m_lineColorMap.clear(); }

   DistanceLineDescriptions( const DistanceLineDescriptions& dld) {
      m_lineColorMap = dld.m_lineColorMap;
   }
   
   DistanceLineDescriptions operator= ( const DistanceLineDescriptions& dld ) {
      m_lineColorMap = dld.m_lineColorMap;
      return *this;
   }

   std::string GetColor(const std::string& name) { return m_lineColorMap[name].m_color; }
   std::string GetName(const std::string& name) { 
      return m_lineColorMap[name].m_name; 
   }
   int GetLineWidth(const std::string& name) { return m_lineColorMap[name].m_lineWidth; }
   double GetComputeTime(const std::string& name) { return m_lineColorMap[name].m_time2Compute; }
   std::string GetDashMode(const std::string& name) { return m_lineColorMap[name].m_dashMode; }
private:
   std::map<std::string, Line> m_lineColorMap;
};


#endif  //  DISTANCELINEDESCRIPTION_H