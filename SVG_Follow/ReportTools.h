#pragma once

#ifndef REPORTTOOLS_H
#define REPORTTOOLS_H

#define DEBUG_REPORTS
#ifdef DEBUG_REPORTS
#define DEBUG_REPORT_STRING( s ) \
   std::cout << s << std::endl;
#else
#define DEBUG_REPORT_STRING( s )
#endif

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

//#include "NC_Boundary.h"
#include "LRL_Cell.h"
#include "G6.h"
#include "MatG6.h"

#define MAKE_TOSTRING
#ifdef MAKE_TOSTRING

template<typename T>
std::string ToQuotedString( const T& t ) {
   return ( LRL_ToString( "\"", t, "\"" ) );
}




/*
    NC_Projector m_prj;
    NC_Projector m_perp;
    NC_Projector m_xform;
    std::string m_name;
    std::string m_subspace;
    std::string m_description;
    double m_distance;
    */


static std::string LRL_ToString( const char c ) {
   return std::string( 1, c );
}

template<typename T>
std::string LRL_ToString( const T& t ) {
   std::ostringstream ostr;
   ostr << t;
   return ( ostr.str( ) );
}

std::string LRL_ToString( const std::ostringstream& t );
template<typename T1, typename T2>
std::string LRL_ToString( const T1& t1, const T2& t2 ) {
   return LRL_ToString( t1 ) + " " + LRL_ToString( t2 );
}
template<typename T1, typename T2, typename T3>
std::string LRL_ToString( const T1& t1, const T2& t2, const T2& t3 ) {
   return LRL_ToString( t1 ) + " " + LRL_ToString( t2 ) + " " + LRL_ToString( t3 );
}
template<typename T1, typename T2, typename T3, typename T4>
std::string LRL_ToString( const T1& t1, const T2& t2, const T3& t3, const T4& t4 ) {
   return LRL_ToString( t1 ) + " " + LRL_ToString( t2 ) + " " + LRL_ToString( t3 ) + " "
      + LRL_ToString( t4 );
}
template<typename T1, typename T2, typename T3, typename T4, typename T5>
std::string LRL_ToString( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
   const T5& t5 ) {
   return LRL_ToString( t1 ) + " " + LRL_ToString( t2 ) + " " + LRL_ToString( t3 ) + " "
      + LRL_ToString( t4 ) + " " + LRL_ToString( t5 );
}
template<typename T1, typename T2, typename T3, typename T4, typename T5,
   typename T6>
   std::string LRL_ToString( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
   const T5& t5, const T6& t6 ) {
      return LRL_ToString( t1 ) + " " + LRL_ToString( t2 ) + " " + LRL_ToString( t3 ) + " "
         + LRL_ToString( t4 ) + " " + LRL_ToString( t5 ) + " " + LRL_ToString( t6 );
   }
template<typename T1, typename T2, typename T3, typename T4, typename T5,
   typename T6, typename T7>
   std::string LRL_ToString( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
   const T5& t5, const T6& t6, const T7& t7 ) {
      return LRL_ToString( t1 ) + " " + LRL_ToString( t2 ) + " " + LRL_ToString( t3 ) + " "
         + LRL_ToString( t4 ) + " " + LRL_ToString( t5 ) + " " + LRL_ToString( t6 ) + " "
         + LRL_ToString( t7 );
   }
template<typename T1, typename T2, typename T3, typename T4, typename T5,
   typename T6, typename T7, typename T8>
   std::string LRL_ToString( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
   const T5& t5, const T6& t6, const T7& t7, const T7& t8 ) {
      return LRL_ToString( t1 ) + " " + LRL_ToString( t2 ) + " " + LRL_ToString( t3 ) + " "
         + LRL_ToString( t4 ) + " " + LRL_ToString( t5 ) + " " + LRL_ToString( t6 ) + " "
         + LRL_ToString( t7 ) + " " + LRL_ToString( t8 );
   }
template<typename T1, typename T2, typename T3, typename T4, typename T5,
   typename T6, typename T7, typename T8, typename T9>
   std::string LRL_ToString( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
   const T5& t5, const T6& t6, const T7& t7, const T7& t8, const T7& t9 ) {
      return LRL_ToString( t1 ) + " " + LRL_ToString( t2 ) + " " + LRL_ToString( t3 ) + " "
         + LRL_ToString( t4 ) + " " + LRL_ToString( t5 ) + " " + LRL_ToString( t6 ) + " "
         + LRL_ToString( t7 ) + " " + LRL_ToString( t8 ) + " " + LRL_ToString( t9 );
   }
template<typename T>
static std::string LRL_ToString( const std::vector<T>& t ) {
   std::string s;
   const unsigned int n = t.size( );
   for ( unsigned int i = 0; i < n; ++i ) {
      s += LRL_ToString( t[i] ) + std::string( " " );
   }
   return s;
}


const double rad = 180.0/4.0/atan(1.0);
static std::string LRL_ToString( const LRL_Cell& c ) {
   return LRL_ToString( c[0], c[1], c[2], "   ",  rad*c[3], rad*c[4], rad*c[5] );
}

static std::string LRL_ToString( const G6& v ) {
   return LRL_ToString( v[0], v[1], v[2], v[3], v[4], v[5] );
}

static std::string LRL_ToString( const MatG6& m ) {
   std::string s;
   for ( unsigned int i = 0; i < 36; i += 6 )
      s += LRL_ToString( m[0 + i], m[1 + i], m[2 + i], m[3 + i], m[4 + i], m[5 + i],
      "\n" );
   return s;
}

//static std::string LRL_ToString( const NC_Projector& m ) {
//
//   std::string s;
//   for ( unsigned int i = 0; i < 36; i += 6 )
//      s += LRL_ToString( m[0 + i], m[1 + i], m[2 + i], m[3 + i], m[4 + i], m[5 + i],
//      "\n" );
//   return s;
//}

//static std::string LRL_ToString( const NC_Boundary& ncb ) {
//   return std::string( "\n" )
//      + std::string( "NC_BOUNDARY: " )  + ncb.GetName( ) + "\n"
//      + std::string( "description: " )  + ncb.GetDescription( ) + "\n"
//      + std::string( "NC_Subspace: " )  + ncb.GetSubspace( ) + "\n"
//      + std::string( "NC_Projector: \n" ) + LRL_ToString( ncb.GetProjector( ) ) + "\n"
//      + std::string( "NC_Perp: \n" )      + LRL_ToString( ncb.GetPerp( ) ) + "\n"
//      + std::string( "NC_Transform: \n" ) + LRL_ToString( ncb.GetTransform( ) ) + "\n"
//      + std::string( "Distance: \n" )     + LRL_ToString( ncb.GetDistance( ) ) + "\n\n";
//}

std::string LRL_ToString( const std::ostringstream& t );

template<typename T>
std::string LRL_ToString( const std::string& s, const T& t ) {
   return ( s + LRL_ToString( t ) );
}

template<typename T>
std::string LRL_ToString( const T& t, const std::string& s ) {
   return ( LRL_ToString( t ) + s );
}

template<typename T>
std::string LRL_ToString( const std::string& s1, const T& t, const std::string& s2 ) {
   return ( s1 + LRL_ToString( t ) + s2 );
}
#endif

#undef MAKE_TOSTRING

#endif
