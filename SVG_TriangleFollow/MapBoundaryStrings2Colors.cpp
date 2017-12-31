
#include <algorithm>
#include <utility>

#include "MapBoundaryStrings2Colors.h"

#include "GlobalTriangleFollowerConstants.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
MapBoundaryStrings2Colors::MapBoundaryStrings2Colors(void)
   : m_colorAssignments( )
   , m_nextColorIndex( 0 )
   , m_numberOfAssignableColors( (unsigned long)GlobalConstants::globalColorsForBoundaries.size() )
/*-------------------------------------------------------------------------------------*/
{
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
MapBoundaryStrings2Colors::~MapBoundaryStrings2Colors(void){
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::string MapBoundaryStrings2Colors::BoundaryIsAlreadyAssignedColor( const std::string& boundary ){
   const std::string nextColorString = (*m_colorAssignments.find( boundary )).second;
   return( nextColorString );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::string MapBoundaryStrings2Colors::BoundaryNeedsColorAssignment( const std::string& boundary ){
   m_nextColorIndex = (m_nextColorIndex+1) % m_numberOfAssignableColors;

   const std::string nextColorString = GlobalConstants::globalColorsForBoundaries[m_nextColorIndex];
   m_colorAssignments.insert( std::make_pair( boundary, nextColorString ) );

   return( nextColorString );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::string MapBoundaryStrings2Colors::operator() (const std::string& boundary ) {
   if ( m_colorAssignments.find( boundary ) != m_colorAssignments.end( ) )
      return( BoundaryIsAlreadyAssignedColor( boundary ) );
   else
      return( BoundaryNeedsColorAssignment( boundary ) );
}