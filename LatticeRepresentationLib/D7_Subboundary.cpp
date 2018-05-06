


#include "D7_Subboundary.h"


#pragma warning( disable : 4100) //  unreferenced formal parameter

D7_Subboundary::D7_Subboundary( )
{
}

D7_Subboundary::~D7_Subboundary( )
{
}

D7_Subboundary D7_Subboundary::ConstructSubboundary( const std::string& name, const MatD7& xform, const std::string& subspace, const std::string& condition ) {
   D7_Subboundary dv;
   dv.SetName( std::string( name) );
   dv.SetTransform( xform );
   dv.SetCondition( condition );

   return dv;
}

std::ostream& operator<< ( std::ostream& o, const D7_Subboundary& v ) {
   o << "     Internal Boundary: " << v.GetName() << std::endl;
   o << "     Condition: " << v.GetCondition() << std::endl;
   o << "Transform " << std::endl << v.GetTransform()<< std::endl << std::endl;
   return o;
}
   MatD7 m_xform;
   std::string m_name;
   std::string m_condition;
