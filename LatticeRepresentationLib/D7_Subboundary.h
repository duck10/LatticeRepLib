#ifndef D7_SUBBOUNDARY_H
#define D7_SUBBOUNDARY_H

#include "MatD7.h"

#include <string>


class D7_Subboundary
{
public:
   friend std::ostream& operator<< ( std::ostream&, const D7_Subboundary& );

   D7_Subboundary( );
   ~D7_Subboundary( );

   static D7_Subboundary ConstructSubboundary( const std::string& name, const MatD7& xform, const std::string& subspace, const std::string& condition );


   MatD7        GetTransform(void)  const { return m_xform; }
   std::string GetName        ( void )  const { return m_name ;      }
   std::string GetCondition   ( void )  const { return m_condition ; }
   std::string GetTransformsTo( void )  const { return m_transformsToSubB; }

   void  SetTransform( const MatD7&        xform    )  { m_xform     = xform    ; }
   void  SetName     ( const std::string& name     )  { m_name      = name     ; }
   void  SetCondition( const std::string& condition)  { m_condition = condition; }
   void  SetTransformsTo( const std::string& to ) { m_transformsToSubB = to; }

private:
   MatD7 m_xform;
   std::string m_name;
   std::string m_condition;
   std::string m_transformsToSubB;
};

#endif
