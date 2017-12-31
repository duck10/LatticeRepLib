
#ifndef MAPBOUNDARYSTRINGS2COLORS_H
#define MAPBOUNDARYSTRINGS2COLORS_H

#include <map>
#include <string>

class MapBoundaryStrings2Colors
{
public:
   MapBoundaryStrings2Colors(void);
   ~MapBoundaryStrings2Colors(void);

   std::string operator()( const std::string& s );

private:
   std::string BoundaryIsAlreadyAssignedColor( const std::string& boundary );
   std::string BoundaryNeedsColorAssignment( const std::string& boundary );

   std::map<std::string, std::string> m_colorAssignments;
   unsigned long m_nextColorIndex;
   const unsigned long m_numberOfAssignableColors;

private: // don't implement
   MapBoundaryStrings2Colors& operator=( const MapBoundaryStrings2Colors& ) const;
   MapBoundaryStrings2Colors( const MapBoundaryStrings2Colors& );
};

#endif
