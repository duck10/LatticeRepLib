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
#include "Cell.h"
#include "G6.h"
#include "Mat66.h"

#define MAKE_TOSTRING
#ifdef MAKE_TOSTRING

template<typename T>
std::string ToQuotedString( const T& t ) {
   return ( ToString( "\"", t, "\"" ) );
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


static std::string ToString( const char c ) {
   return std::string( 1, c );
}

template<typename T>
std::string ToString( const T& t ) {
   std::ostringstream ostr;
   ostr << t;
   return ( ostr.str( ) );
}

std::string ToString( const std::ostringstream& t );
template<typename T1, typename T2>
std::string ToString( const T1& t1, const T2& t2 ) {
   return ToString( t1 ) + " " + ToString( t2 );
}
template<typename T1, typename T2, typename T3>
std::string ToString( const T1& t1, const T2& t2, const T2& t3 ) {
   return ToString( t1 ) + " " + ToString( t2 ) + " " + ToString( t3 );
}
template<typename T1, typename T2, typename T3, typename T4>
std::string ToString( const T1& t1, const T2& t2, const T3& t3, const T4& t4 ) {
   return ToString( t1 ) + " " + ToString( t2 ) + " " + ToString( t3 ) + " "
      + ToString( t4 );
}
template<typename T1, typename T2, typename T3, typename T4, typename T5>
std::string ToString( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
   const T5& t5 ) {
   return ToString( t1 ) + " " + ToString( t2 ) + " " + ToString( t3 ) + " "
      + ToString( t4 ) + " " + ToString( t5 );
}
template<typename T1, typename T2, typename T3, typename T4, typename T5,
   typename T6>
   std::string ToString( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
   const T5& t5, const T6& t6 ) {
      return ToString( t1 ) + " " + ToString( t2 ) + " " + ToString( t3 ) + " "
         + ToString( t4 ) + " " + ToString( t5 ) + " " + ToString( t6 );
   }
template<typename T1, typename T2, typename T3, typename T4, typename T5,
   typename T6, typename T7>
   std::string ToString( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
   const T5& t5, const T6& t6, const T7& t7 ) {
      return ToString( t1 ) + " " + ToString( t2 ) + " " + ToString( t3 ) + " "
         + ToString( t4 ) + " " + ToString( t5 ) + " " + ToString( t6 ) + " "
         + ToString( t7 );
   }
template<typename T1, typename T2, typename T3, typename T4, typename T5,
   typename T6, typename T7, typename T8>
   std::string ToString( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
   const T5& t5, const T6& t6, const T7& t7, const T7& t8 ) {
      return ToString( t1 ) + " " + ToString( t2 ) + " " + ToString( t3 ) + " "
         + ToString( t4 ) + " " + ToString( t5 ) + " " + ToString( t6 ) + " "
         + ToString( t7 ) + " " + ToString( t8 );
   }
template<typename T1, typename T2, typename T3, typename T4, typename T5,
   typename T6, typename T7, typename T8, typename T9>
   std::string ToString( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
   const T5& t5, const T6& t6, const T7& t7, const T7& t8, const T7& t9 ) {
      return ToString( t1 ) + " " + ToString( t2 ) + " " + ToString( t3 ) + " "
         + ToString( t4 ) + " " + ToString( t5 ) + " " + ToString( t6 ) + " "
         + ToString( t7 ) + " " + ToString( t8 ) + " " + ToString( t9 );
   }
template<typename T>
static std::string ToString( const std::vector<T>& t ) {
   std::string s;
   const unsigned int n = t.size( );
   for ( unsigned int i = 0; i < n; ++i ) {
      s += ToString( t[i] ) + std::string( " " );
   }
   return s;
}


const double rad = 180.0/4.0/atan(1.0);
static std::string ToString( const Cell& c ) {
   return ToString( c[0], c[1], c[2], "   ",  rad*c[3], rad*c[4], rad*c[5] );
}

static std::string ToString( const G6& v ) {
   return ToString( v[0], v[1], v[2], v[3], v[4], v[5] );
}

static std::string ToString( const Mat66& m ) {
   std::string s;
   for ( unsigned int i = 0; i < 36; i += 6 )
      s += ToString( m[0 + i], m[1 + i], m[2 + i], m[3 + i], m[4 + i], m[5 + i],
      "\n" );
   return s;
}

//static std::string ToString( const NC_Projector& m ) {
//
//   std::string s;
//   for ( unsigned int i = 0; i < 36; i += 6 )
//      s += ToString( m[0 + i], m[1 + i], m[2 + i], m[3 + i], m[4 + i], m[5 + i],
//      "\n" );
//   return s;
//}

//static std::string ToString( const NC_Boundary& ncb ) {
//   return std::string( "\n" )
//      + std::string( "NC_BOUNDARY: " )  + ncb.GetName( ) + "\n"
//      + std::string( "description: " )  + ncb.GetDescription( ) + "\n"
//      + std::string( "NC_Subspace: " )  + ncb.GetSubspace( ) + "\n"
//      + std::string( "NC_Projector: \n" ) + ToString( ncb.GetProjector( ) ) + "\n"
//      + std::string( "NC_Perp: \n" )      + ToString( ncb.GetPerp( ) ) + "\n"
//      + std::string( "NC_Transform: \n" ) + ToString( ncb.GetTransform( ) ) + "\n"
//      + std::string( "Distance: \n" )     + ToString( ncb.GetDistance( ) ) + "\n\n";
//}

std::string ToString( const std::ostringstream& t );

template<typename T>
std::string ToString( const std::string& s, const T& t ) {
   return ( s + ToString( t ) );
}

template<typename T>
std::string ToString( const T& t, const std::string& s ) {
   return ( ToString( t ) + s );
}

template<typename T>
std::string ToString( const std::string& s1, const T& t, const std::string& s2 ) {
   return ( s1 + ToString( t ) + s2 );
}
#endif

#undef MAKE_TOSTRING

#endif
