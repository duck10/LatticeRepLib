
//#include "stdafx.h"

#include "LRL_Cell.h"
#include "D7_Boundary.h"
#include "D7_BoundaryList.h"
#include "Delone.h"
#include "G6.h"
#include "Selling.h"
#include "LRL_StringTools.h"

#pragma warning( disable : 4100) //  unreferenced formal parameter


std::ostream& operator<< (std::ostream& o, const D7_Boundary& v) {
   o << "Boundary ------------------------------------ " << v.GetName() << std::endl;
   o << "Subspace: " << v.GetSubspace() << std::endl;
   o << "Condition: " << v.GetCondition() << std::endl << std::endl;
   o << "Projector " << std::endl << v.GetProjector() << std::endl << std::endl;
   o << "Perp " << std::endl << v.GetPerp() << std::endl << std::endl;

   const unsigned int nsubbounds = v.GetSubboundaryCount();
   for (unsigned int i = 0; i < nsubbounds; ++i) {
      o << v.GetSubboundary(i) << " ";
   }
   return o;
}

D7_Boundary::D7_Boundary( )
   : m_externalUnitNormalVector(ComputeExternalNormalUnitVector())
{
}

D7_Boundary::~D7_Boundary( )
{
}

void D7_Boundary::SetProjector    ( const MatD7 & m ){
   m_proj = m;
   if ( (m*m.transpose(m) - m).norm() > 1.0E-10 ) throw;
}

D7_Boundary D7_Boundary::ConstructSubboundary( const std::string& name, const MatD7& prj, const std::string& subspace, const std::string& condition ) {
   D7_Boundary b;
   b.SetName( std::string( name) );
   b.SetProjector( prj );
   MatD7 eye = MatD7::Eye();
   b.SetPerp( eye - prj );
   b.SetCondition( condition );

   return b;
}

const D7_Subboundary D7_Boundary::GetSubboundary( const std::string& s ) const {
   for (unsigned int i = 0; i < (*this).GetSubboundaryCount( ); ++i) {
       if ( (*this).GetSubboundary(i).GetName() == s ) return (*this).GetSubboundary(i);
  }

   return D7_Subboundary( );
}

const D7 D7_Boundary::ComputeExternalNormalUnitVector( void ) const {
   static const LRL_Cell cell( 1, 12, 15, 91, 95, 97 );
   S6 s6red;
   const bool b = Selling::Reduce(S6(cell),s6red);
   const D7 d7from6s = D7(s6red);
   D7 perpVector = (*this).GetPerp()*d7from6s;
   perpVector /= -perpVector.norm();

   const D7 check = (*this).GetPerp()*perpVector;

   return perpVector;
}
