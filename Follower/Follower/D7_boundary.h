#ifndef D7_Boundary_H
#define D7_Boundary_H

#include "MatD7.h"
#include "D7.h"
#include "D7_Subboundary.h"


#include <string>
#include <vector>


class D7_Boundary
{
public:
   friend std::ostream& operator<< ( std::ostream&, const D7_Boundary& );

   D7_Boundary( );
   ~D7_Boundary( );

   static D7_Boundary ConstructSubboundary( const std::string& name, const MatD7& xform, const std::string& subspace, const std::string& condition );

   const MatD7                        GetProjector    ( void )                 const { return m_proj ; }
   const MatD7                        GetPerp         ( void )                 const { return m_perp ; }
   const std::string                 GetName         ( void )                 const { return m_name ; }
   const std::string                 GetCondition    ( void )                 const { return m_condition ; }
   const std::vector<D7_Subboundary> GetSubboundaries( void )                 const { return m_vSubBounds; };
   const std::string                 GetSubspace     ( void )                 const { return m_subspace ;  }
   const D7_Subboundary&             GetSubboundary  ( const unsigned int n ) const { return m_vSubBounds[n]; }
   const D7_Subboundary              GetSubboundary  ( const std::string& s ) const;
   const D7_Boundary                 GetBoundary     ( const std::string& s ) const;
   const D7                          GetExternalNormalUnitVector ( void )     const { return m_externalUnitNormalVector; }

   void SetProjector    ( const MatD7                       & m );
   void SetPerp         ( const MatD7                       & m )    { m_perp = m         ; }
   void SetName         ( const std::string                & s )    { m_name = s         ; }
   void SetCondition    ( const std::string                & s )    { m_condition = s    ; }
   void SetSubspace ( const std::string& subspace )                 { m_subspace  = subspace ; }
   void SetSubboundaries( const std::vector<D7_Subboundary>& v )    { m_vSubBounds = v   ; }
   void SetSubboundary  ( const unsigned int n, D7_Subboundary& b ) { m_vSubBounds[n] = b; }

   unsigned int GetSubboundaryCount( void ) const { 
      const int thesize = int(m_vSubBounds.size( ));
      return thesize;
   }

   size_t size( void ) const { return (size_t)(m_vSubBounds.size( )); }

private:
   const D7 ComputeExternalNormalUnitVector(void) const;

private:
   MatD7 m_proj;
   MatD7 m_perp;
   std::string m_name;
   std::string m_condition;
   std::string m_subspace;
   std::vector<D7_Subboundary> m_vSubBounds;
   D7 m_externalUnitNormalVector;
};

#endif
