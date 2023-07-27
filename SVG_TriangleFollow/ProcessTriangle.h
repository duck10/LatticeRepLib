
#ifndef PROCESSTRIANGLE_H
#define PROCESSTRIANGLE_H

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <list>
#include <utility>
#include <vector>

#include "LRL_Cell.h"
#include "FollowerTools.h"
#include "LRL_RandTools.h"
#include "Niggli.h"
#include "LRL_ToString.h"
#include "SVG_FollowTriangle.h"
#include "TriangleFollow.h"


  // !!!!!!!!!!!!!!!!!!!!!!!!!!!! specialized for G6  !!!!!!!!!!!!!!!!!!!!!!!!!!!!
template<typename TVEC, typename TREDUCEMETHOD, typename TFOLLOWMETHOD>
   static triple<double, double, double> FindTriangleEdges( const triple<TVEC, TVEC, TVEC>& violation ) {  // specialized for G6
      const TVEC v1 = violation.first;   // specialized for G6
      const TVEC v2 = violation.second;  // specialized for G6
      const TVEC v3 = violation.third;   // specialized for G6
      const double d1 = DIST( v1, v3);
      const double d2 = DIST( v2, v3);
      const double d3 = DIST( v1, v2);
      return triple<double, double, double>( d1, d2, d3 );
   }



template<typename TVEC, typename TREDUCEMETHOD, typename TFOLLOWMETHOD>
class ProcessTriangle
{
public:
   // Triangle.cpp : Defines the entry point for the console application.
   //
   //double VectorLength( const double* a ) {

   void PrintProcessVector( const TVEC& v ) {
      std::cout << "\t";
      std::cout << LRL_ToString( v ) << " ";
      std::cout << std::string( "XX" ) << std::endl;
   }

   bool GenRandReducedArray( double* a, double aRan[7], const unsigned int length ) {
      G6 vRan, v;  // used in Niggli::Reduce

      bool bTest( true );
      MatG6 m;
      vRan = 1000 * G6::rand( );

      bool btemp = false;
      while (btemp = TREDUCEMETHOD::ReduceUnsortedByFunction(vRan, m, v, 0.0), !btemp) {
         vRan = 1000 * G6::rand( );
      }
      //D7 v7( v );
      //a[0] = v7.at( 0 );
      //a[1] = v7.at( 1 );
      //a[2] = v7.at( 2 );
      //a[3] = v7.at( 3 );
      //a[4] = v7.at( 4 );
      //a[5] = v7.at( 5 );
      //a[6] = v7.at( 6 );
      //ConvertVectorToArray( vRan, aRan );
      return btemp;
   }

  // !!!!!!!!!!!!!!!!!!!!!!!!! particular for G6 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   static std::pair< std::string, double> FindNearestBoundary( const G6& v ){  // specialized for G6
      double r = DBL_MAX;
      std::string nearBoundary = "ERROR";

      if (std::abs( v[0] - v[1] ) < r)
      {
         r = std::abs( v[0] - v[1] );
         nearBoundary = "1";
      }

      if (std::abs( v[1] - v[2] ) < r)
      {
         r = std::abs( v[1] - v[2] );
         nearBoundary = "2";
      }

      if (std::abs( v[3] ) < r)
      {
         r = std::abs( v[3] );
         nearBoundary = "3";
      }

      if (std::abs( v[4] ) < r)
      {
         r = std::abs( v[4] );
         nearBoundary = "4";
      }

      if (std::abs( v[5] ) < r)
      {
         r = std::abs( v[5] );
         nearBoundary = "5";
      }

      if (std::abs( v[1] - v[3] ) < r &&
         (v[5] - v[4]) < r)
      {
         r = std::max( std::abs( v[1] - v[3] ), v[5] - v[4] );
         nearBoundary = "6";
      }

      if (std::abs( v[1] - v[3] ) < r &&
         (v[4] - v[5]) > r)
      {
         r = std::max( std::abs( v[1] - v[3] ), v[4] - v[5] );
         nearBoundary = "7";
      }

      if (std::abs( v[1] + v[3] ) < r)
      {
         r = std::abs( v[1] + v[3] );
         nearBoundary = "8";
      }

      if (std::abs( v[0] - v[4] ) < r &&
         (v[5] - v[3]) < r)
      {
         r = std::max( std::abs( v[0] - v[4] ), v[5] - v[3] );
         nearBoundary = "9";
      }

      if (std::abs( v[0] - v[4] ) < r &&
         (v[3] - v[5]) < r)
      {
         r = std::max( std::abs( v[0] - v[4] ), v[3] - v[5] );
         nearBoundary = "A";
      }

      if (std::abs( v[0] + v[4] ) < r)
      {
         r = std::abs( v[0] + v[4] );
         nearBoundary = "B";
      }

      if (std::abs( v[0] - v[5] ) < r &&
         (v[4] - v[3]) < r)
      {
         r = std::max( std::abs( v[0] - v[5] ), v[4] - v[3] );
         nearBoundary = "C";
      }

      if (std::abs( v[0] - v[5] ) < r &&
         (v[3] - v[4]) < r)
      {
         r = std::max( std::abs( v[0] - v[5] ), v[3] - v[4] );
         nearBoundary = "D";
      }

      if (std::abs( v[0] + v[5] ) < r)
      {
         r = std::abs( v[0] + v[5] );
         nearBoundary = "E";
      }

      if (abs( v[0] + v[1] + v[3] + v[4] + v[5] ) < r)
      {
         r = abs( v[0] + v[1] + v[3] + v[4] + v[5] );
         nearBoundary = "F";
      }

      return std::make_pair( nearBoundary, r );
   }

   /*
   g1:     292.73 925.747 949.503 -1.45515 -199.264 -260.49
   37.9889 40.4402 737.339 -21.5135 -0.278502 -13.5016
   90.8647 134.863 752.96 -19.2477 -55.1196 -44.0231g2: g3:

   NCDist g1 g2 686.718  // comment
   NCDist g1 g3 584.628  // comment
   NCDist g2 g3 118.159  // comment
   g123 g1 g2 994.675
   g123 g1 g3 879.107
   g123 g2 g3 120.797
   Herberts-MacBook-Pro:triang yaya$ ./triang
   */

   //static double DIST( const TVEC& v1, const TVEC& v2 ) { return (v1 - v2).norm( ); }


   triple<double, double, double> FindTriangleEdges( const triple<TVEC, TVEC, TVEC>& violation ) {
      S6Dist s6dist(5.0);
      const TVEC v1 = violation.first;
      const TVEC v2 = violation.second;
      const TVEC v3 = violation.third;
      const double d1 = s6dist.DistanceBetween( S6(v1), S6(v2) );
      const double d2 = s6dist.DistanceBetween( S6(v1), S6(v3) );
      const double d3 = s6dist.DistanceBetween( S6(v2), S6(v3) );
      return triple<double, double, double>( d1, d2, d3 );
   }

   bool ProcessViolation( const TVEC& v1, const TVEC& v2, const TVEC& v3,
      const std::list<double>& worstCases, TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD >& tf,
      const int count, const double percentViolation, const double minimumViolationToReport ) {

      tf.clear( );

      const triple<double, double, double> violation = FindTriangleEdges( triple<TVEC,TVEC,TVEC>(v1, v2, v3) );
      const double d1 = violation.GetFirst();
      const double d2 = violation.GetSecond();
      const double d3 = violation.GetThird();
      if (d1<1.0E-10 || d2<1.0E-10 || d3<1.0E-10) {
         std::cout << "rejected due to zero area" << std::endl;
         return false;
      }

      tf.Follow( v1, v2, v3, minimumViolationToReport );

      const triple<TVEC, TVEC, TVEC> worstCaseFound( tf.GetWorstTriple( ) );

      const triple<double, double, double> worstR = FindTriangleEdges( triple<TVEC,TVEC,TVEC> (worstCaseFound.first, worstCaseFound.second, worstCaseFound.third ) );
      const double worstd12 =  worstR.GetFirst();
      const double worstd13 =  worstR.GetSecond();
      const double worstd23 =  worstR.GetThird();
      const double worstTriangleViolation = 100.0*FollowerTools::TriangleInequality( worstd12, worstd13, worstd23 );
      /*if (worstTriangleViolation >= minimumViolationToReport)*/ {  //  lca -- this should be changed to be on glitch or both
         std::cout << std::endl << std::endl << -GLOBAL_RunInputVector::globalInputRandomSeed << "   test# " << count << "    violation " << percentViolation << "%" << std::endl;
         std::cout << "Output File " << tf.GetOutputFilename( ) << std::endl;
         std::cout << "(1,3) " << d1 << "   (2,3) " << d2 << "   (1,3) " << d3 << std::endl;
         std::cout << "red ";
         PrintProcessVector( v1 );
         std::cout << "red ";
         PrintProcessVector( v2 );
         std::cout << "red ";
         PrintProcessVector( v3 );
         std::cout << "worst case -----------------     " << worstTriangleViolation << "%" << std::endl;
         std::cout << "worst case distances, (12), (13), (23)  " << worstd12 << "  " << worstd13 << "  " << worstd23 << std::endl;
         PrintProcessVector( worstCaseFound.first  );
         PrintProcessVector( worstCaseFound.second );
         PrintProcessVector( worstCaseFound.third  );
      }
      std::cout << std::endl;
      return false;
   }


};


#endif