
#ifndef D7_BOUNDARYLIST_H
#define D7_BOUNDARYLIST_H

#include "D7_boundary.h"
#include "ConstSingleton_T.h"

#include <vector>

class D7_BoundaryList : public ConstSingleton_T<D7_BoundaryList>
{
public:
   D7_BoundaryList( );
   virtual ~D7_BoundaryList( );

   static D7_Boundary GetBoundary( const std::string& name );
   static int GetBoundaryIndex( const std::string& name );

   static std::vector<D7_Boundary> ConstructPrimaryBoundaryList( void );

   static std::vector<MatD7> Calculate24InterchangeMatrices( void );
   static std::vector<MatD7>& Get24InterchangeMatrices( void ) { return m_interchangematrics; };

   static std::vector<D7_Boundary> GetVectorOfBoundaries( void );
   static D7_Boundary GetBoundary( const unsigned int n ) { return m_boundaries[n]; }


   static D7_Boundary Case1( void );
   static D7_Boundary Case2( void );
   static D7_Boundary Case3( void );
   static D7_Boundary Case4( void );
   static D7_Boundary Case5( void );
   static D7_Boundary Case6( void );
   static D7_Boundary Case7( void );
   static D7_Boundary Case8( void );
   static D7_Boundary Case9( void );
   static unsigned long size( void ) {return (unsigned long)( m_boundaries.size( ));
}

public:
   static std::vector<MatD7> m_interchangematrics;
   static std::vector<D7_Boundary> m_boundaries;
};

#endif
