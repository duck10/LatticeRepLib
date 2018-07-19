
#include "Cell.h"
#include "Mat66.h"
#include "Reducer.h"
#include "ToString.h"
#include "vector_3d.h"

#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <string>

/*
R5
1 0 0 0 0 0
0 1 0 0 0 0
0 1 1 -s 0 0
0 -2s 0 1 0 0
0 0 0 0 1 -s
0 0 0 0 0 1
*/

/*  class Reducer
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
A class to implement Niggli reduction in the G6 space. Reduce returns a reduced cell
   and the matrix that converts the input cell to the reduced cell.

   Reduced(void)
                 == constructor -- nothing to do here
   static void Reduce( const G6& vi, Mat66& m, G6& vout, const double delta )
                 == returns vout as the reduced cell of vi and m, the conversion matrix
   static bool NearRed( const G6& gvec, const double delta );
                 == determines whether gvec is reduced within delta
   static bool Near2Red( const G6& gvec, const double delta, G6& vout, double& dist )
                 == determines whether gvec is reduced within delta, and returns the reduced cell
                    and how far from reduced
   static void Reporter( const std::string& text, const G6& vin, const G6& vout, const Mat66& m )
                 == prints information about each step in reduction (including standard presentation)

private:
   static void MKnorm( const G6& vi, Mat66& m, G6& vout, const double delta  )
                 == internal function to convert vi to standard presentation - the matrix
                    to implement that change is returned
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

const bool DEBUG_REDUCER(false);

//-----------------------------------------------------------------------------
// Name: g6sign()
// Description: returns the value of the first variable with the sign of the second
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double g6sign( const double d, const double x )
{
   return( x>0.0 ? d : -d );
}

static void PrintG6 ( const std::string& text, const G6& v )
{
      printf( "%s  %f %f %f %f %f %f", text.c_str(), v[0],v[1],v[2],v[3],v[4],v[5] );
}

static void PrintM66_INT ( const std::string& text, const Mat66& v )
{
      for ( int i=0; i<6; ++i )
      {
         printf( "%s ", text.c_str() );
         for ( int j=0; j<36; j+=6 )
         {
            printf( " %3d", int(v[i+j]) );
         }
         printf( "\n");
      }
}

//-----------------------------------------------------------------------------
// Name: Reporter()
// Description: prints out the intermediate step values and looks for
//              changes in the volume (indication of error)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void Reducer::Reporter( const std::string& text, const G6& vin, const G6& vout, const Mat66& m )
{
   if ( ! DEBUG_REDUCER ) return;

   const double volume = Cell(vout).Volume( );
   const double previousVolume = Cell(vin).Volume( );
   const bool volumeChanged = ( fabs(volume-previousVolume) / std::max(volume, previousVolume) ) > 1.0E-12;
   if ( volumeChanged )
   {
      printf( "**************************************************************************************\n" );
   }

   if ( text.empty( ) )
   {
      const int i19191 = 19191;
   }

   const std::string s1 = ToString( std::string("vin ")+text.c_str()+" ", vin );
   const std::string s2 = ToString( "vout ", vout );
   printf( "%s\n%s\n", s1.c_str(), s2.c_str() );

   if ( volumeChanged )
   {
      printf( "\nVolume(vout) %f", volume );
      printf( "    PREVIOUS VOLUME %f\n", previousVolume );
   }
   else
   {
      printf( "\nVolume %f\n", volume );
   }
   printf( "\n,%s\n", ToString( "m ", m ).c_str() );

} // end Reporter

//-----------------------------------------------------------------------------
// Name: Reducer()
// Description: constructor
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Reducer::Reducer(void)
{
}

//-----------------------------------------------------------------------------
// Name: Reducer()
// Description: destructor
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Reducer::~Reducer(void)
{
}

//-----------------------------------------------------------------------------
// Name: MKnorm()
// Description: changes a G6 vector to standard presentation (often called
//              normalization in the literature) and returns the standard
//              vector and the matrix that changes the input vector to the
//              standard one
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void Reducer::MKnorm( const G6& vi, Mat66& m, G6& vout, const double delta  )
{
 
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// These are the matrices that can be used to convert a vector to standard
// presentation. They are used in MKnorm. There are NO OTHER POSSIBILITIES.
// The numbering is from Andrews and Bernstein, 1988
   const static Mat66 spnull( "1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1" );

   const static Mat66 sp1( "0 1 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 1" );
   const static Mat66 sp2( "1 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 0" );


   const static Mat66 sp34a( "1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1 0 0 0 0 0 0  1" );
   const static Mat66 sp34b( "1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0  1 0 0 0 0 0 0 -1" );
   const static Mat66 sp34c( "1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0  1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1" );
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


const Mat66 R5_Plus  (" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 -1 0 0   0 -2 0 1 0 0   0 0 0 0 1 -1   0 0 0 0 0 1");
const Mat66 R5_Minus (" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 +1 0 0   0 +2 0 1 0 0   0 0 0 0 1 +1   0 0 0 0 0 1");
const Mat66 R6_Plus  (" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 -1 0   0 0 0 1 0 -1   -2 0 0 0 1 0   0 0 0 0 0 1");
const Mat66 R6_Minus (" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 +1 0   0 0 0 1 0 +1   +2 0 0 0 1 0   0 0 0 0 0 1"); 
const Mat66 R7_Plus  (" 1 0 0 0 0 0   1 1 0 0 0 -1   0 0 1 0 0 0   0 0 0 1 -1 0   0 0 0 0 1 0   -2 0 0 0 0 1");
const Mat66 R7_Minus (" 1 0 0 0 0 0   1 1 0 0 0 +1   0 0 1 0 0 0   0 0 0 1 +1 0   0 0 0 0 1 0   +2 0 0 0 0 1");
const Mat66 R8       (" 1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 2 0 1 0 1   2 0 0 0 1 1    0 0 0 0 0 1");
const Mat66 R9_Plus(R5_Plus);
const Mat66 R9_Minus(R5_Minus);
const Mat66 R10_Plus(R6_Plus);
const Mat66 R10_Minus(R6_Minus);
const Mat66 R11_Plus(R7_Plus);
const Mat66 R11_Minus(R7_Minus);
//const Mat66 R12(R8);
const Mat66 R12      ( "1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 -2 0 -1 0 -1   -2 0 0 0 -1 -1   0 0 0 0 0 1");
   bool again = true;
   int ncycle = 0;
   Mat66 mat = Mat66::Eye();
   G6 vin;
   m = Mat66::Eye();

   vin = vi;

   // assure that g1<=g2<=g3
   while ( again && (ncycle < 5) )
   {
      ++ncycle;
      again = false;

      std::string sptext;
      if ( (fabs(vin[0]) > fabs(vin[1])+delta+1.e-12*(vin[0]+vin[1])) ||
           (fabs(vin[0]-vin[1])<1.e-38+1.e-12*fabs(vin[0]+vin[1])
	    && delta<1.0E-12 && fabs(vin[3])>fabs(vin[4])+
	    delta+1.e-12*(fabs(vin[3])+fabs(vin[4]))))
      { // SP1
         mat    = sp1;
         again  = true;
         sptext = "SP1";
      }
      else if ( (fabs(vin[1]) > fabs(vin[2])+delta+1.e-12*(vin[1]+vin[2])) ||
                (fabs(vin[1]-vin[2])<1.e-38+1.e-12*fabs(vin[1]+vin[2])
                 && delta<1.0E-12 && fabs(vin[4])>fabs(vin[5])+
                 delta+1.e-12*(fabs(vin[4])+fabs(vin[5]))))
      { // SP2
         mat    = sp2;
         again  = true;
         sptext = "SP2";
      }

      if ( again )
      {
         // Accumulate the total transformation from the input vector into vout and
         // the total transformation itself into matrix m.
         const Mat66 mtemp = mat*m;
         m = mtemp;
         vout = mat*vin;
         Reporter( sptext, vin, vout, mat );
         vin = vout;
      }
//      DEBUG_REPORT_STRING(ToString( "   MKNORM input  "+sptext+"  ", vi));;
//      DEBUG_REPORT_STRING(ToString( "   MKNORM output "+sptext+"  ", vout));;
   }

   // now we assure (within delta) that the vector is +++ or ---

   int bMinusPattern = 0;
   int bZeroPattern = 0;
   if( vin[3] < delta+1.0E-13*(vin[1]+vin[2]) ) bMinusPattern |= 4;
   if( vin[4] < delta+1.0E-13*(vin[0]+vin[2]) ) bMinusPattern |= 2;
   if( vin[5] < delta+1.0E-13*(vin[0]+vin[1]) ) bMinusPattern |= 1;
   if( fabs(vin[3]) < delta+1.0E-13*(vin[1]+vin[2]) ) bZeroPattern |= 4;
   if( fabs(vin[4]) < delta+1.0E-13*(vin[0]+vin[2]) ) bZeroPattern |= 2;
   if( fabs(vin[5]) < delta+1.0E-13*(vin[0]+vin[1]) ) bZeroPattern |= 1;
   std::string sptext2( "ERROR" );;

   switch( bMinusPattern )
   {
   case 0:  /*  +++  */
      {
         mat = spnull;
         sptext2 = "no mknorm action sp1,sp2-0";
         break;
      }
   case 1:  /* ++- -> --- */
      {
         mat = sp34a;
         sptext2 = "SP34a-1";
         break;
      }
   case 2:  /* +-+ -> --- */
      {
         mat = sp34b;
         sptext2 = "SP34b-2";
         break;
      }
   case 3:  /* +-- -> +++, but +0- -> -0- and +-0 -> --0 and +00 -> -00 */
      {
         mat = sp34c;
         sptext2 = "SP34c-3";
         if ((bZeroPattern&2) == 2 ) {
            mat = sp34a;
            sptext2 = "SP34a-3";
            break;
         }
         if ((bZeroPattern&1) == 1 ) {
            mat = sp34b;
            sptext2 = "SP34b-3";
            break;
         }
         break;
      }
   case 4:  /* -++ -> --- */
      {
         mat = sp34c;
         sptext2 = "SP34c-4";
         break;
      }
   case 5:  /* -+- -> +++, but 0+- -> 0-- and -+0 -> --0 and 0+0 -> 0-0 */
      {
         mat = sp34b;
         sptext2 = "SP34b-5";
         if ((bZeroPattern&4) == 4 ) {
            mat = sp34a;
            sptext2 = "SP34a-5";
            break;
         }
         if ((bZeroPattern&1) == 1 ) {
            mat = sp34c;
            sptext2 = "SP34c-5";
            break;
         }
         break;
      }
   case 6:  /* --+ - > +++, but 0-+ -> 0-- and -0+ - > -0- and 00+ -> 00- */
      {
         mat = sp34a;
         sptext2 = "SP34a-6";
         if ((bZeroPattern&4) == 4 ) {
            mat = sp34b;
            sptext2 = "SP34b-5";
            break;
         }
         if ((bZeroPattern&2) == 2 ) {
            mat = sp34c;
            sptext2 = "SP34c-5";
            break;
         }
         break;
      }
   case 7:
      {
         mat = spnull;
         sptext2 = "no mknorm action sp1,sp2-7";
         break;
      }
   }

   // Accumulate the total transformation from the input vector into vout and
   // the total transformation itself into matrix m.
   const Mat66 mtemp = mat*m;
   m = mtemp;
   vout = mat*vin;
//   DEBUG_REPORT_STRING(ToString( "      MKNORM input  "+sptext2+"  ", vi));;
//   DEBUG_REPORT_STRING(ToString( "      MKNORM output "+sptext2+"  ", vout));;
   std::cout << std::flush;
   Reporter( sptext2, vin, vout, mat );
}  // end MKnorm

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
G6 Reducer::Reduce(const G6& viv) {
   G6 v;
   Mat66 m;
   const bool result = Reduce(viv, m, v, 0.0);
   v.SetValid(result);
   return v;
}

//-----------------------------------------------------------------------------
// Name: Reduce()
// Description: performs Niggli reduction and returns the standard reduced
//              vector and the matrix that changes the input vector to the
//              reduced one
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool Reducer::Reduce( const G6& vi, Mat66& m, G6& vout, const double delta )
{

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// These are the matrices that can be used to convert a vector to standard
// presentation. They are used in MKnorm. There are NO OTHER POSSIBILITIES.
// The numbering is from Andrews and Bernstein, 1988
   const static Mat66 spnull( "1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1" );

   const static Mat66 sp1( "0 1 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 1" );
   const static Mat66 sp2( "1 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 0" );


   const static Mat66 sp34a( "1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1 0 0 0 0 0 0  1" );
   const static Mat66 sp34b( "1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0  1 0 0 0 0 0 0 -1" );
   const static Mat66 sp34c( "1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0  1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1" );
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


const Mat66 R5_Plus  (" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 -1 0 0   0 -2 0 1 0 0   0 0 0 0 1 -1   0 0 0 0 0 1");
const Mat66 R5_Minus (" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 +1 0 0   0 +2 0 1 0 0   0 0 0 0 1 +1   0 0 0 0 0 1");
const Mat66 R6_Plus  (" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 -1 0   0 0 0 1 0 -1   -2 0 0 0 1 0   0 0 0 0 0 1");
const Mat66 R6_Minus (" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 +1 0   0 0 0 1 0 +1   +2 0 0 0 1 0   0 0 0 0 0 1"); 
const Mat66 R7_Plus  (" 1 0 0 0 0 0   1 1 0 0 0 -1   0 0 1 0 0 0   0 0 0 1 -1 0   0 0 0 0 1 0   -2 0 0 0 0 1");
const Mat66 R7_Minus (" 1 0 0 0 0 0   1 1 0 0 0 +1   0 0 1 0 0 0   0 0 0 1 +1 0   0 0 0 0 1 0   +2 0 0 0 0 1");
const Mat66 R8       (" 1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 2 0 1 0 1   2 0 0 0 1 1    0 0 0 0 0 1");
const Mat66 R9_Plus(R5_Plus);
const Mat66 R9_Minus(R5_Minus);
const Mat66 R10_Plus(R6_Plus);
const Mat66 R10_Minus(R6_Minus);
const Mat66 R11_Plus(R7_Plus);
const Mat66 R11_Minus(R7_Minus);
//const Mat66 R12(R8);
const Mat66 R12      ( "1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 -2 0 -1 0 -1   -2 0 0 0 -1 -1   0 0 0 0 0 1");
   G6 vin;
   Mat66 m1;
   int ncycle = 0;
   bool again = true;
   const bool debug = true;

   const int maxCycle = 26;

   m1 = Mat66::Eye();
   vin = vi;
   G6 voutPrev( vin );

   /* Mapping from Fortran indices:

   1  2  3  4  5  6
   0  6 12 18 24 30

   7  8  9 10 11 12
   1  7 13 19 25 31

   13 14 15 16 17 18
   2  8 14 20 26 32

   19 20 21 22 23 24
   3  9 15 21 27 33

   25 26 27 28 29 30
   4 10 16 22 28 34

   31 32 33 34 35 36
   5 11 17 23 29 35

   */

   // Try some number of times to reduce the input vector and accumulate
   // the changing vector and total transformation matrix
   // The limit on the number of cycles is because (whether because of
   // floating point rounding or algorithm issues) there might be a
   // case of an infinite loop. The designations R5-R12 are from
   // Andrews and Bernstein, 1988
   while ( again && ncycle < maxCycle )
   {
//      DEBUG_REPORT_STRING(ToString( "REDUCE start cycle  ", ncycle, "  ", vin));;

      m1 = m;
      MKnorm( vin, m1, vout, delta );
      vin = vout;
      const Mat66 m2temp = m1*m;
      m = m2temp;
      m1 = Mat66::Eye();

      if ( fabs(vin[3]) > fabs(vin[1])+delta )
      { // R5
         m1 = (vin[3]<=0.0) ? R5_Minus : R5_Plus;
         again  = true;
////         DEBUG_REPORT_STRING(ToString( "REDUCE R5-m1\n", m1,"\n"));;
         const Mat66 m2 = m1*m;
         m = m2;
         vout = m1*vin;
         Reporter( "R5", vin, vout, m1 );
      }
      else if ( fabs(vin[4]) > fabs(vin[0])+delta )
      { // R6
         m1 = (vin[4]<=0.0) ? R6_Minus : R6_Plus;
         again  = true;
 //        DEBUG_REPORT_STRING(ToString( "REDUCE R6-m1\n", m1,"\n"));;
         const Mat66 m2 = m1*m;
         m = m2;
         vout = m1*vin;
         Reporter( "R6", vin, vout, m1 );
      }
      else if ( fabs(vin[5]) > fabs(vin[0])+delta )
      { // R7
         m1 = (vin[5]<=0.0) ? R7_Minus : R7_Plus;
         again  = true;
 //        DEBUG_REPORT_STRING(ToString( "REDUCE R7-m1\n", m1,"\n"));;
         const Mat66 m2 = m1*m;
         m = m2;
         vout = m1*vin;
         Reporter( "R7", vin, vout, m1 );
      }
      else if ( vin[3]+vin[4]+vin[5]+fabs(vin[0])+fabs(vin[1])+delta < 0.0 )
      { //R8
         m1 = R8;
         again = true;
//         DEBUG_REPORT_STRING(ToString( "REDUCE R8-m1\n", m1,"\n"));;
         const Mat66 m2 = m1*m;
         m = m2;
         vout = m1*vin;
         Reporter( "R8", vin, vout, m1 );
      }
      else if ( (fabs(vin[3]-vin[1])<=delta && 2.0*vin[4]-delta<vin[5] ) ||
         (fabs(vin[3]+vin[1])<=delta &&
         vin[5]-delta<=0.0 ) )
      { // R9  There is an error in the paper says "2g5<g5" should be "2g5<g6"
         m1 = (vin[3]<=0.0) ? R9_Minus : R9_Plus;
         again = true;
 //        DEBUG_REPORT_STRING(ToString( "REDUCE R9-m1\n", m1,"\n"));;
         const Mat66 m2 = m1*m;
         m = m2;
         vout = m1*vin;
         Reporter( "R9", vin, vout, m1 );
      }
      else if ( (fabs(vin[4]-vin[0])<=delta && 2.0*vin[3]-delta<vin[5] ) ||
         (fabs(vin[4]+vin[0])<=delta &&
         vin[5]-delta<=0.0 ) )
      { //R10 (LAST=15 in ITERATE)
         m1 = (vin[4]<=0.0) ? R10_Minus : R10_Plus;
         again = true;
         const Mat66 m2 = m1*m;
 //        DEBUG_REPORT_STRING(ToString( "REDUCE R10-m\n", m,"\n"));
 //        DEBUG_REPORT_STRING(ToString( "REDUCE R10-m1\n", m1,"\n"));
 //        DEBUG_REPORT_STRING(ToString( "REDUCE R10-m2=m1*m\n", m2,"\n"));
         m = m2;
         vout = m1*vin;
         Reporter( "R10", vin, vout, m1 );
 //        DEBUG_REPORT_STRING(ToString( "REDUCE R10-in  ", vin));
  //       DEBUG_REPORT_STRING(ToString( "REDUCE R10-out  ", vout))
      }
      else if ( (fabs(vin[5]-vin[0])<=delta && 2.0*vin[3]-delta<vin[4] ) ||
         (fabs(vin[5]+vin[0])<=delta &&
         vin[4]-delta<=0.0 ) ) // paper says g6<0, but it seems wrong
      { // R11
         m1 = (vin[5]<=0.0) ? R11_Minus : R11_Plus;
         again = true;
//         DEBUG_REPORT_STRING(ToString( "REDUCE 1-m1\n", m1,"\n"));;
         const Mat66 m2 = m1*m;
         vout = m1*vin;
         Reporter( "R11", vin, vout, m1 );
      }
      else if ( fabs(vin[3]+vin[4]+vin[5]+fabs(vin[0])+fabs(vin[1])) <= delta && (2.0*(fabs(vin[0])+vin[4])+vin[5]>delta) )
      { //R12 (same as R8)
         m1 = R12;
         again = true;
//         DEBUG_REPORT_STRING(ToString( "REDUCE R12-m1\n", m1,"\n"));;
         const Mat66 m2 = m1*m;
         m = m2;
         vout = m1*vin;
         Reporter( "R12", vin, vout, m1 );
      }
      else
      {
         again = false;
         vout = vin;
      }

      // probably don't need to do this group of code when again==false !!!!!!!!!!!!!!
      m1 = Mat66::Eye();
      MKnorm( vout, m1, vin, delta );
      const Mat66 mtemp = m1*m;
      m = mtemp;
      Reporter( "vout after MKnorm at end of reduced cycle", vout, vin, m1 );
      for( size_t i=3; i<6; ++i )
         if ( std::fabs(vin[i]) < 1.0E-10 ) vin[i] = 0.0;

      if ( vin[0]<0.0 || vin[1]<0.0 || vin[2]<0.0 ) {
         // ERROR ERROR ERROR
         if ( DEBUG_REDUCER ) {
            fprintf( stderr, " Negative sq, axis %d \n", ncycle );
            fprintf( stderr, " vin: [%g,%g,%g,%g,%g,%g]\n",
               vin[0], vin[1], vin[2], vin[3], vin[4], vin[5] );
            fprintf( stderr, " vi: [%g,%g,%g,%g,%g,%g]\n",
               vi[0], vi[1], vi[2], vi[3], vi[4], vi[5] );
         }
         return( false );
      }

      if ( ncycle == 0 ) voutPrev = vin;

      if ( DEBUG_REDUCER )
      {
         //printf( "%d    %f %f %f %f %f %f\n", ncycle, vout[0], vout[1], vout[2], vout[3], vout[4], vout[5] );
      }
//      DEBUG_REPORT_STRING(ToString( "REDUCE output  ", vi));;

      ++ncycle;
   }
   if (ncycle >= maxCycle) {
      std::cout << "THERE IS A REDUCE PROBLEM, cycle " << ncycle << std::endl;
   }
   
   return ncycle < maxCycle;

} // end of Reduce


//-----------------------------------------------------------------------------
// Name: NearRed()
// Description: test whether a vector is nearly Niggli reduced
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool Reducer::NearRed( const G6& gvec, const double delta )
{
//C
//C     RETURNS .true. IF GVEC IS NEARLY NIGGLI REDUCED
//C     ALLOWING A NON_REDUCTION ERROR OF DELTA.  NO
//C     MATRICES OR VECTORS ARE KEPT.
//C
//C     IF DELTA .EQ. 0.D0, THE TESTS ARE ON REDUCTION
//C     RATHER THAN NEAR REDUCTION
//C
//C     ALL CASES OF BEING ON THE WRONG SIDES OF THE
//C     FOLDING BOUNDARIES ARE ACCEPTED AS NEAR
//C     REDUCED
//C----------------------------------------------------------------------C
//C     TEST FOR G1, G2, G3 OUT OF BOUNDS OR WRONG ORDER
//C
   if ( gvec[0] < -delta        ||
        gvec[1] < -delta        ||
        gvec[2] < -delta        ||
        gvec[0] > gvec[1]+delta ||
        gvec[1] > gvec[2]+delta )
   {
      return( false );
   }
//C
//C     TEST FOR NON-REDUCED SIGN COMBINATIONS IN
//C     G4, G5 AND G6
//C
   if ( (gvec[3] <= delta ||
         gvec[4] <= delta ||
         gvec[5] <= delta ) &&
        (gvec[4] >  delta ||
         gvec[4] >  delta ||
         gvec[5] >  delta ) )
   {
      return( false );
   }
//C
//C     TEST ABS(G{4,5,6}) AGAINST G{1,2,3}
//C
   if ( fabs(gvec[3]) > fabs(gvec[2])+delta ||
        fabs(gvec[4]) > fabs(gvec[0])+delta ||
        fabs(gvec[5]) > fabs(gvec[0])+delta )
   {
      return( false );
   }
//C
//C     TEST THE BODY DIAGONAL
//C
   if ( gvec[3]+gvec[4]+gvec[5] + fabs(gvec[0])+fabs(gvec[1])+delta < 0.0 ) return( false );
   if ( delta > 0.0 ) return( true );
//C
//C     TEST THE 678, 9AB, CDE BOUNDARY FOLDS
//C
   if ( (gvec[3]         == gvec[1] && 2.0*gvec[4] < gvec[5] ) ||
        (gvec[4]         == gvec[0] && 2.0*gvec[3] < gvec[5] ) ||
        (gvec[5]         == gvec[0] && 2.0*gvec[3] < gvec[4] ) ||
        (gvec[3]+gvec[1] == 0.0     && gvec[5]     <= 0.0 )    ||
        (gvec[4]+gvec[0] == 0.0     && gvec[5]     <= 0.0 )    ||
        (gvec[5]+gvec[0] == 0.0     && gvec[4]     <= 0.0 ) ) return( false );
//C
//C     TEST THE F BOUDARY FOLD
//C
   if ( fabs(gvec[3]+gvec[4]+gvec[5]+gvec[0]+gvec[1]) <= delta ) return( false );

   return( true );
} // end NearRed


//-----------------------------------------------------------------------------
// Name: NearRed()
// Description: test whether a vector is nearly Niggli reduced
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//bool Reducer::Near2Red( const G6& gvec, const double delta, G6& vout, double& dist )
//{
////C
////C     RETURNS .true. IF GVEC IS NEARLY NIGGLI REDUCED
////C     ALLOWING A NON_REDUCTION ERROR OF DELTA.
////C     A VECTOR VOUT IS PRODUCED WHICH IS REDUCED, IF
////C     POSSIBLE.  NO MATRIX IS KEPT.  DIST IS THE DISTANCE
////C     FROM GVEC TO VOUT.
////C
////C     IF DELTA .EQ. 0.D0, THE TESTS ARE ON REDUCTION
////C     RATHER THAN NEAR REDUCTION
////C
////C     ALL CASES OF BEING ON THE WRONG SIDES OF THE
////C     FOLDING BOUNDARIES ARE ACCEPTED AS NEAR
////C     REDUCED
////C
////C----------------------------------------------------------------------C
//
//   const double xdelta = 1.0E-6 * delta;
////C
////C     TEST FOR G1, G2, G3 OUT OF BOUNDS OR WRONG ORDER
////C
//   if ( gvec[0] < -xdelta ||
//        gvec[1] < -xdelta ||
//        gvec[2] < -xdelta ||
//        gvec[0] > gvec[1]+xdelta ||
//        gvec[1] > gvec[2]+xdelta )
//   {
//
//      if ( gvec[0] < 0.0 ) vout[0] = 0.0;
//      if ( gvec[1] < 0.0 ) vout[1] = 0.0;
//      if ( gvec[2] < 0.0 ) vout[2] = 0.0;
//      if ( vout[2]> std::max(vout[0],vout[1] ) )
//      {
//         if ( vout[1] < vout[0] )
//         {
//            vout[1] = (vout[0]+vout[1]) / 2.0;
//            vout[0] = vout[1];
//         }
//      }
//      else if ( vout[0] < std::min( vout[1],vout[2] ) )
//      {
//         if ( vout[2] < vout[1] )
//         {
//            vout[2] = (vout[1]+vout[2]) / 2.0;
//            vout[1] = vout[2];
//         }
//         else
//         {
//            vout[0] = (vout[0]+vout[1]+vout[2]) / 3.0;
//            vout[1] = vout[0];
//            vout[2] = vout[0];
//         }
//      }
//   }
////C
//   //C     TEST FOR NON-REDUCED SIGN COMBINATIONS IN
//   //C     G4, G5 AND G6
//   //C
//   double s456 = 1.0;
//   if ( gvec[3] <= xdelta ||
//        gvec[4] <= xdelta ||
//        gvec[5] <= xdelta )
//   {
//      s456 = -1.0;
//      if ( gvec[3] > 0.0 ) vout[3] = 0.0;
//      if ( gvec[4] > 0.0 ) vout[4] = 0.0;
//      if ( gvec[5] > 0.0 ) vout[5] = 0.0;
//   }
//
////C     TEST ABS(G{4,5,6}) AGAINST G{1,2,3}
////C
//   if ( fabs(gvec[3]) > fabs(gvec[1])+xdelta ||
//        fabs(gvec[4]) > fabs(gvec[0])+xdelta ||
//        fabs(gvec[5]) > fabs(gvec[0])+xdelta )
//   {
//      vout[3] = s456 * std::min( fabs(gvec[1]) , fabs(vout[3]) );
//      vout[4] = s456 * std::min( fabs(gvec[0]) , fabs(vout[4]) );
//      vout[5] = s456 * std::min( fabs(gvec[0]) , fabs(vout[5]) );
//   }
////C
////C     TEST THE BODY DIAGONAL
////C
//   if ( gvec[3]+gvec[4]+gvec[5]+fabs(gvec[0])+fabs(gvec[1])+xdelta < 0.0 )
//   {
//      vout[5] = -vout[0]-vout[1]-vout[3]-vout[4];
//   }
//
////C     IF DELTA IS NON-ZERO, WE STOP HERE
//
//   if ( delta <= 0.0 )
//   {
////C
////C     TEST THE 678, 9AB, CDE BOUNDARY FOLDS
////C
//   if ( (gvec[3]         ==gvec[1] && 2.0*gvec[4] < gvec[5]) ||
//        (gvec[4]         ==gvec[0] && 2.0*gvec[3] < gvec[5]) ||
//        (gvec[5]         ==gvec[0] && 2.0*gvec[3] < gvec[4]) ||
//        (gvec[5]         == 0.0    &&     gvec[4] <= 0.0 ) ||
//        (gvec[3]+gvec[1] == 0.0    &&     gvec[5] <= 0.0 ) ||
//        (gvec[4]+gvec[0] == 0.0    &&     gvec[5] <= 0.0) )
//        return( false );
////C
////C     TEST THE F BOUNDARY FOLD
////C
//   if ( fabs(gvec[3]+gvec[4]+gvec[5]+gvec[0]+gvec[1]) <= xdelta &&
//      2.0*(gvec[0]+gvec[4])+gvec[5] > 0.0 )
//      return( false );
//   }
//   dist = BasicDistance::g456dist( gvec, vout );
//
////C      if (DIST.GT.DELTA) near2red = .false.
////C      write(*,*)"NEAR2RED",delta,near2red,dist
////C      call printg6("GVEC",GVEC)
////C      call printg6("Vout",vout)
//   return( dist <= delta );
//} // end Near2Red

static const double degreesPerRad = 180.0 / ( 4.0 * atan(1.0) );

//class Basis {
//private:
//   //G6 m_basis[3];
//   Vector_3 m_basis[3];
//public:
//   Basis(const G6& niggli) {
//      const double a = std::sqrt(niggli[0]);
//      const double b = std::sqrt(niggli[1]);
//      const double c = std::sqrt(niggli[2]);
//
//      const double alpha = std::acos(niggli[3] / 2.0 /b /c) * degreesPerRad;
//      const double beta  = std::acos(niggli[4] / 2.0 /a /c) * degreesPerRad;
//      const double gamma = std::acos(niggli[5] / 2.0 /a /b) * degreesPerRad;
//
//      const Matrix_3x3 amat(Matrix_3x3::Cart(a,b,c,alpha,beta,gamma));
//      //Vector_3 v( amat*Vector_3( 1.0, 0.0, 0.0 ) );
//      //m_basis[0][0] = v[0];
//      //m_basis[0][1] = v[1];
//      //m_basis[0][2] = v[2];
//      //v = amat*Vector_3( 0.0, 1.0, 0.0 );
//      //m_basis[1][0] = v[1];
//      //m_basis[1][1] = v[2];
//      //m_basis[1][2] = v[0];
//      //v = amat*Vector_3( 0.0, 0.0, 1.0 );
//      //m_basis[2][0] = v[2];
//      //m_basis[2][1] = v[0];
//      //m_basis[2][2] = v[1];
//      m_basis[0] = amat*Vector_3(1.0,0.0,0.0);
//      m_basis[1] = amat*Vector_3(0.0,1.0,0.0);
//      m_basis[2] = amat*Vector_3(0.0,0.0,1.0);
//   } // end of constructor
//
//   Vector_3 GetBasis( const size_t n ) { return m_basis[n]; }
//
//};  // end class Basis
//
//   class Diagonals
//   {
//   public:
//      Diagonals( const Vector_3 v[3] )
//      {
//         int n = 0;
//         m_diags[n  ] = v[0] + v[1];
//         m_diags[++n] = v[0] - v[1];
//         m_diags[++n] = v[0] + v[2];
//         m_diags[++n] = v[0] - v[2];
//         m_diags[++n] = v[1] + v[2];
//         m_diags[++n] = v[1] - v[2];
//
//         m_diags[++n] = m_diags[0] + v[2];
//         m_diags[++n] = m_diags[0] - v[2];
//         m_diags[++n] = m_diags[1] + v[2];
//         m_diags[++n] = m_diags[1] - v[2];
//
//         for ( int i=0; i<10; ++i )
//         {
//            m_diags[i+10] = -m_diags[i];
//         }
//      }
//
//      Vector_3 m_diags[20];
//   }; // end class Diagonals
//
//
//
//G6 Reducer::Niggli2Delone( const G6& v )
//{
//   if ( v[3] <= 0.0 )
//   {
//      return( v );
//   }
//   else
//   {
//      Basis basis( v );
//   }
//
//}
//
//
//class Vector4D {
//public:
//   Vector4D(const G6& v) {
//      Basis b(v);
//      std::vector<double> v10;
//      const Vector_3 v3Counter = b.GetBasis(0) + b.GetBasis(1) + b.GetBasis(2);
//      m_v10[ 0] = b.GetBasis(0).Norm()*b.GetBasis(0).Norm();
//      m_v10[ 5] = b.GetBasis(1).Norm()*b.GetBasis(1).Norm();
//      m_v10[10] = b.GetBasis(2).Norm()*b.GetBasis(2).Norm();
//      m_v10[15] = b.GetBasis(3).Norm()*b.GetBasis(3).Norm();
//
//      m_v10[ 1] = 2.0*b.GetBasis(0).Norm()*b.GetBasis(1).Norm();
//      m_v10[ 2] = 2.0*b.GetBasis(0).Norm()*b.GetBasis(2).Norm();
//      m_v10[ 3] = 2.0*b.GetBasis(0).Norm()*b.GetBasis(3).Norm();
//      m_v10[ 6] = 2.0*b.GetBasis(1).Norm()*b.GetBasis(2).Norm();
//      m_v10[ 7] = 2.0*b.GetBasis(1).Norm()*b.GetBasis(3).Norm();
//      m_v10[11] = 2.0*b.GetBasis(2).Norm()*b.GetBasis(3).Norm();
//   }
//
//   std::vector<double> GetVector10( void ) const { return m_v10; }
//
//   double Norm( void ) const {
//      double d;
//      for( size_t i=0; i<10; ++i )
//         d += m_v10[i]*m_v10[i];
//      return d;
//   }
//   
//   friend std::ostream& operator<< ( std::ostream& o, const Vector4D& v ) {
//      for( size_t i=0; i<10; ++i ) 
//         o << v.m_v10[i] << " ";
//    return o;
//}
//
//private:
//
//   std::vector<double> m_v10;
//};
//

/*

      double precision A(4,4),A2(3,3),AC(6),AC2(6),AD(4,4),AV(4,4),AV2(3,3),AW(3,3),E(4,4),G(4,4),GG(3,3),GN(4,4),GS(7,7),GV(4,4),GV2(3,3),GW(3,3),O(7,7),PP(4,4,6),AAC,ACL,PN,SF,SIGR,SIGR1,SIGR2,SIGR3,SIGR4,TH,vc,vc1,ZW,ZW1,ZW2,ZW3,ZW4,ZW5,ZWX


Delone reduction from DELOS90
G is the metric tensor
GN is a temporary copy of G so that the near zeros in G can be zeroed
VC is the cell volume
PN is the delta around zero
PP  ????? (4x4x6) seems to be the actual reduction transforms
MC is an index into some matrix to positive value
TP ?????????? transformation matrix (4x4x7) for none, 6 x unit matrix

AD(4,4)
AD/1.,4*0.,1.,4*0.,1.,4*0.,1./

PP/-1.,  0., 0.,0.,1.,0.,1.,0.,0.,1.,0.,0.,1.,0.,0.,1.,
   -1.,  0., 0.,0.,1.,1.,0.,0.,0.,0.,1.,0.,1.,0.,0.,1.,
   -1.,  0., 0.,0.,1.,1.,0.,0.,0.,0.,0.,1.,1.,0.,1.,0.,
     0.,-1., 0.,0.,1.,1.,0.,0.,0.,0.,1.,0.,0.,1.,0.,1.,
     0.,-1., 0.,0.,1.,1.,0.,0.,0.,0.,0.,1.,0.,1.,1.,0.,
     0., 0.,-1.,0.,1.,0.,1.,0.,0.,0.,0.,1.,0.,1.,1.,0./

PP/-1.,3*0.,1.,0.,1.,2*0.,1.,2*0.,1.,2*0.,1.,-1.,3*0.,2*1.,4*0.,1.,0.,1.,2*0.,1.,-1.,3*0.,2*1.,5*0.,2*1.,0.,1.,2*0.,-1.,2*0.,2*1.,4*0.,1.,2*0.,1.,0.,1.,0.,-1.,2*0.,2*1.,5*0.,1.,0.,2*1.,3*0.,-1.,0.,1.,0.,1.,4*0.,1.,0.,2*1.,0./
     !Normalization of the metric to unit volume*10

      VC=G(1,1)*G(2,2)*G(3,3)-G(1,1)*G(2,3)**2-G(2,2)*G(1,3)**2-G(3,3)*G(1,2)**2+2.0*G(1,2)*G(1,3)*G(2,3)
      VC=DSQRT(VC)
      do i=1,3
       do j=1,3
        g(i,j)=100.*g(i,j)/vc 
       enddo
      enddo
      
      do I=1,3             ! extension on Delone-parameters
       do J=1,3
        G(4,4)=G(4,4)+G(I,J)
        G(I,4)=G(I,4)-G(I,J)
       enddo
       G(4,I)=G(I,4)
      enddo
  104 MC=0                ! search for positive element in G
      do I=1,4            ! MC = Permutationsindex
       do J=1,4
        A(I,J)=AD(I,J)
        GN(I,J)=G(I,J)   !Save G in GN, because G is slightly idealized in the next line
        if (DABS(G(I,J)).LT.6.*PN) G(I,J)=0.0 !Threshold PN=.00001, can be changed in DATA
        if (MC.EQ.0.AND.J.GT.I.AND.G(I,J).GT.0.0) MC=J+2*I-3-I/3 !Address (1,...,6) for a positive scalar product
       enddo
      enddo
      if (MC.EQ.0) goto 109 !Exit for the case that the base is reduced

      do I=1,4    ! transformation of matrix AD and metric tensor G
       do J=1,4
        G(I,J)=0.0
        do K=1,4
         AD(I,J)=AD(I,J)+A(I,K)*PP(K,J,MC)
         do L=1,4
          G(I,J)=G(I,J)+GN(K,L)*PP(K,I,MC)*PP(L,J,MC)     !Delone reduction
         enddo
        enddo
       enddo
      enddo
      goto 104  !Return to the next reduction cycle
      
      
      

   */
