#ifndef FOLLOWER_H
#define FOLLOWER_H
#ifdef _MSC_VER
#pragma once
#endif

template<typename T1, typename T2> 
class Follower {
public:
   static std::vector<T1> Follow( const unsigned int n, const T1& t1, const T2& t2 ) {
      std::vector<T1> v;
      v.push_back( t1 );

      for ( unsigned int i = 1; i <= n; ++i ) {
         v.push_back( Interpolate( double(i)/double(n+1), t1, t2 ) );
      }
      
      v.push_back( t2 );

      return v;
   }

   static T1 Interpolate( const double fract, const T1& t1, const T2& t2 ) {
      return (1.0-fract)*t1 + fract*t2;
   }
};

#endif

