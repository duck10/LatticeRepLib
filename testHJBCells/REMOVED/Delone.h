#ifndef Delone_H
#define Delone_H

#ifdef _MSC_VER
#pragma once
#endif

#include "Mat66.h"
#include "G6.h"
#include "D7.h"
#include "VectorTools.h"

#include <ostream>
#include <string>

class Delone {

public:
   static bool Reduce( const G6& vi, Mat66& m, G6& vout, const double delta  );
   static void sortN( G6& v, Mat66& m );
   static G6 Reduce( const G6& vi );
   static void Report( void );
   static bool IsAllMinus(const G6& v, const double delta);
   class Key {
   public:
      Key( void ) : key(std::make_pair(false,0.0)) { }

      Key( const std::pair<bool, double>& k ) : key(k) { }

      bool operator< ( const Key& k) const { return key<k.key; }

      friend std::ostream& operator<< ( std::ostream&o, const Key& k ) {
         const double& d = k.key.second;
         if ( d == 0   ) o << "Unit (no error) ";
         if ( d == 3.1 ) o << "mDEL3a";
         if ( d == 3.2 ) o << "mDEL3b";
         if ( d == 4.1 ) o << "mDEL4a";
         if ( d == 4.2 ) o << "mDEL4b";
         if ( d == 5.1 ) o << "mDEL5a";
         if ( d == 5.2 ) o << "mDEL5b";

         return o;
      }

      Key operator- ( const Key& k) const { 
         Key kk(k); 
         kk.key = std::make_pair( key.first != k.key.first, (key.second-k.key.second) ); 
         return kk; 
      }

      double norm( void ) const { return 1000*(double(int(key.first)) + key.second); }
      double Norm( void ) const { return norm(); }
      static double DistanceBetween( const Key& k1, const Key& k2 ) { return 1000*(double(int(k1.key.first)) + k1.key.second); }

   private:
      std::pair<bool, double> key;
   };
   
   static bool IsDelone( const D7& v, const double delta = 1.0E-6 );

};

#endif

