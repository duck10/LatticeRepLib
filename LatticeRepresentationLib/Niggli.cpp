

#include "D7.h"
#include "LRL_Cell.h"
#include "LRL_ToString.h"
#include "MatG6.h"
#include "Niggli.h"
#include "S6.h"
#include "Selling.h"
#include "StoreResults.h"

#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <string>

//StoreResults<std::string, G6> g_store(5);

/*
R5
1 0 0 0 0 0
0 1 0 0 0 0
0 1 1 -s 0 0
0 -2s 0 1 0 0
0 0 0 0 1 -s
0 0 0 0 0 1
*/

/*  class Niggli
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
A class to implement Niggli reduction in the G6 space. Reduce returns a reduced cell
   and the matrix that converts the input cell to the reduced cell.

   Reduced(void)
                 == constructor -- nothing to do here
   static void Reduce( const G6& vi, MatG6& m, G6& vout, const double delta )
                 == returns vout as the reduced cell of vi and m, the conversion matrix
   static bool NearRed( const G6& gvec, const double delta );
                 == determines whether gvec is reduced within delta
   static bool Near2Red( const G6& gvec, const double delta, G6& vout, double& dist )
                 == determines whether gvec is reduced within delta, and returns the reduced cell
                    and how far from reduced
   static void Reporter( const std::string& text, const G6& vin, const G6& vout, const MatG6& m )
                 == prints information about each step in reduction (including standard presentation)

private:
   static void MKnorm( const G6& vi, MatG6& m, G6& vout, const double delta  )
                 == internal function to convert vi to standard presentation - the matrix
                    to implement that change is returned
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

const bool DEBUG_REDUCER(false);
size_t Niggli::m_ReductionCycleCount;

//-----------------------------------------------------------------------------
// Name: g6sign()
// Description: returns the value of the first variable with the sign of the second
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double g6sign(const double d, const double x)
{
   return(x > 0.0 ? d : -d);
}

static void PrintG6(const std::string& text, const G6& v)
{
   printf("%s  %f %f %f %f %f %f", text.c_str(), v[0], v[1], v[2], v[3], v[4], v[5]);
}

static void PrintMG6_INT(const std::string& text, const MatG6& v)
{
   for (int i = 0; i < 6; ++i)
   {
      printf("%s ", text.c_str());
      for (int j = 0; j < 36; j += 6)
      {
         printf(" %3d", int(v[i + j]));
      }
      printf("\n");
   }
}

//-----------------------------------------------------------------------------
// Name: Reporter()
// Description: prints out the intermediate step values and looks for
//              changes in the volume (indication of error)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void Niggli::Reporter(const std::string& text, const G6& vin, const G6& vout, const MatG6& m)
{
   if (!DEBUG_REDUCER) return;

   const double volume = LRL_Cell(vout).Volume();
   const double previousVolume = LRL_Cell(vin).Volume();
   const bool volumeChanged = (fabs(volume - previousVolume) / std::max(volume, previousVolume)) > 1.0E-12;
   if (volumeChanged)
   {
      printf("**************************************************************************************\n");
   }

   if (text.empty())
   {
      const int i19191 = 19191;
   }

   const std::string s1 = LRL_ToString(std::string("vin ") + text.c_str() + " ", vin);
   const std::string s2 = LRL_ToString("vout ", vout);
   printf("%s\n%s\n", s1.c_str(), s2.c_str());

   if (volumeChanged)
   {
      printf("\nVolume(vout) %f", volume);
      printf("    PREVIOUS VOLUME %f\n", previousVolume);
   }
   else
   {
      printf("\nVolume %f\n", volume);
   }
   printf("\n,%s\n", LRL_ToString("m ", m).c_str());

} // end Reporter

//-----------------------------------------------------------------------------
// Name: Niggli()
// Description: constructor
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Niggli::Niggli(void)
{
}

//-----------------------------------------------------------------------------
// Name: Niggli()
// Description: destructor
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Niggli::~Niggli(void)
{
}

void Niggli::MKnormWithoutMatrices(const G6& vi, G6& vout, const double delta) {

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   // These are the matrices that can be used to convert a vector to standard
   // presentation. They are used in MKnorm. There are NO OTHER POSSIBILITIES.
   // The numbering is from Andrews and Bernstein, 1988
   const static MatG6 spnull("1 0 0 0 0 0   0 1 0 0 0 0   0 0 1 0 0 0   0 0 0 1 0 0   0 0 0 0 1 0   0 0 0 0 0 1");

   const static MatG6 sp1("0 1 0 0 0 0   1 0 0 0 0 0   0 0 1 0 0 0   0 0 0 0 1 0   0 0 0 1 0 0   0 0 0 0 0 1");  //0,1  3,4
   const static MatG6 sp2("1 0 0 0 0 0   0 0 1 0 0 0   0 1 0 0 0 0   0 0 0 1 0 0   0 0 0 0 0 1   0 0 0 0 1 0");  //1,2  4,5


   const static MatG6 sp34a("1 0 0 0 0 0   0 1 0 0 0 0   0 0 1 0 0 0   0 0 0 -1 0 0   0 0 0 0 -1 0   0 0 0 0 0  1");  // -3,-4
   const static MatG6 sp34b("1 0 0 0 0 0   0 1 0 0 0 0   0 0 1 0 0 0   0 0 0 -1 0 0   0 0 0 0  1 0   0 0 0 0 0 -1");  // -3,-5
   const static MatG6 sp34c("1 0 0 0 0 0   0 1 0 0 0 0   0 0 1 0 0 0   0 0 0  1 0 0   0 0 0 0 -1 0   0 0 0 0 0 -1");  // -4,-5
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


   const static MatG6 R5_Plus(" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 -1 0 0   0 -2 0 1 0 0   0 0 0 0 1 -1   0 0 0 0 0 1");
   const static MatG6 R5_Minus(" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 +1 0 0   0 +2 0 1 0 0   0 0 0 0 1 +1   0 0 0 0 0 1");
   const static MatG6 R6_Plus(" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 -1 0   0 0 0 1 0 -1   -2 0 0 0 1 0   0 0 0 0 0 1");
   const static MatG6 R6_Minus(" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 +1 0   0 0 0 1 0 +1   +2 0 0 0 1 0   0 0 0 0 0 1");
   const static MatG6 R7_Plus(" 1 0 0 0 0 0   1 1 0 0 0 -1   0 0 1 0 0 0   0 0 0 1 -1 0   0 0 0 0 1 0   -2 0 0 0 0 1");
   const static MatG6 R7_Minus(" 1 0 0 0 0 0   1 1 0 0 0 +1   0 0 1 0 0 0   0 0 0 1 +1 0   0 0 0 0 1 0   +2 0 0 0 0 1");
   const static MatG6 R8(" 1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 2 0 1 0 1   2 0 0 0 1 1    0 0 0 0 0 1");
   const static MatG6 R9_Plus(R5_Plus);
   const static MatG6 R9_Minus(R5_Minus);
   const static MatG6 R10_Plus(R6_Plus);
   const static MatG6 R10_Minus(R6_Minus);
   const static MatG6 R11_Plus(R7_Plus);
   const static MatG6 R11_Minus(R7_Minus);
   const static MatG6 R12("1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 -2 0 -1 0 -1   -2 0 0 0 -1 -1   0 0 0 0 0 1");
   bool again = true;
   m_ReductionCycleCount = 0;
   G6 vin;

   vin = vi;

   double& g1 = vin[0];
   double& g2 = vin[1];
   double& g3 = vin[2];
   double& g4 = vin[3];
   double& g5 = vin[4];
   double& g6 = vin[5];

   size_t count = 0;

   // assure that g1<=g2<=g3
   while (again && (count < 5))
   {
      ++count;
      again = false;

      //std::string sptext;
      if ((fabs(vin[0]) > fabs(vin[1]) + delta + 1.e-12 * (vin[0] + vin[1])) ||
         (fabs(vin[0] - vin[1]) < 1.e-38 + 1.e-12 * fabs(vin[0] + vin[1])
            && delta<1.0E-12 && fabs(vin[3])>fabs(vin[4]) +
            delta + 1.e-12 * (fabs(vin[3]) + fabs(vin[4]))))
      { // SP1   0,1  3,4
         //mat = sp1;
         std::swap(g1, g2);
         std::swap(g4, g5);
         again = true;
         //sptext = "SP1";
         //g_store.Store(sptext, vin);
      }
      else if ((fabs(vin[1]) > fabs(vin[2]) + delta + 1.e-12 * (vin[1] + vin[2])) ||
         (fabs(vin[1] - vin[2]) < 1.e-38 + 1.e-12 * fabs(vin[1] + vin[2])
            && delta<1.0E-12 && fabs(vin[4])>fabs(vin[5]) +
            delta + 1.e-12 * (fabs(vin[4]) + fabs(vin[5]))))
      { // SP2 1,2  4,5
         //mat = sp2;
         std::swap(g2, g3);
         std::swap(g5, g6);
         again = true;
         //sptext = "SP2";
         //g_store.Store(sptext, vin);
      }

      //      DEBUG_REPORT_STRING(LRL_ToString( "   MKNORM input  "+sptext+"  ", vi));;
      //      DEBUG_REPORT_STRING(LRL_ToString( "   MKNORM output "+sptext+"  ", vout));;
   }

   // now we assure (within delta) that the vector is +++ or ---

   int bMinusPattern = 0;
   int bZeroPattern = 0;
   if (g4 < delta + 1.0E-13 * (g2 + g3)) bMinusPattern |= 4;
   if (g5 < delta + 1.0E-13 * (g1 + g3)) bMinusPattern |= 2;
   if (g6 < delta + 1.0E-13 * (g1 + g2)) bMinusPattern |= 1;
   if (fabs(g4) < delta + 1.0E-13 * (g2 + g3)) bZeroPattern |= 4;
   if (fabs(g5) < delta + 1.0E-13 * (g1 + g3)) bZeroPattern |= 2;
   if (fabs(g6) < delta + 1.0E-13 * (g1 + g2)) bZeroPattern |= 1;
   //std::string sptext2("Not_All_+++/---_in_MKnorm");

   switch (bMinusPattern)
   {
   case 0:  /*  +++  */
   {
      //mat = spnull;
      //sptext2 = "no_mknorm_action_sp1,sp2-0";
      //g_store.Store(sptext2, vin);
      break;
   }
   case 1:  /* ++- -> --- */
   {
      //mat = sp34a;
      g4 = -g4;
      g5 = -g5;
      //sptext2 = "SP34a-1";
      //g_store.Store(sptext2, vin);
      break;
   }
   case 2:  /* +-+ -> --- */
   {
      //mat = sp34b;
      g4 = -g4;
      g6 = -g6;
      //sptext2 = "SP34b-2";
      //g_store.Store(sptext2, vin);
      break;
   }
   case 3:  /* +-- -> +++, but +0- -> -0- and +-0 -> --0 and +00 -> -00 */
   {
      if ((bZeroPattern & 2) == 2) {
         //mat = sp34a;
         g4 = -g4;
         g5 = -g5;
         //sptext2 = "SP34a-3";
         //g_store.Store(sptext2, vin);
         break;
      }
      else if ((bZeroPattern & 1) == 1) {
         //mat = sp34b;
         //sptext2 = "SP34b-3";
         g4 = -g4;
         g6 = -g6;
         //g_store.Store(sptext2, vin);
         break;
      }
      else {
         //mat = sp34c;
         g5 = -g5;
         g6 = -g6;
         //sptext2 = "SP34c-3";
         //g_store.Store(sptext2, vin);
      }
      break;
   }
   case 4:  /* -++ -> --- */
   {
      //mat = sp34c;
      g5 = -g5;
      g6 = -g6;
      //sptext2 = "SP34c-4";
      //g_store.Store(sptext2, vin);
      break;
   }
   case 5:  /* -+- -> +++, but 0+- -> 0-- and -+0 -> --0 and 0+0 -> 0-0 */
   {
      //mat = sp34b;
      //sptext2 = "SP34b-5";
      if ((bZeroPattern & 4) == 4) {
         //mat = sp34a;
         g4 = -g4;
         g5 = -g5;
         //sptext2 = "SP34a-5";
         //g_store.Store(sptext2, vin);
         break;
      }
      else if ((bZeroPattern & 1) == 1) {
         //mat = sp34c;
         //sptext2 = "SP34c-5a";
         g5 = -g5;
         g6 = -g6;
         //g_store.Store(sptext2, vin);
         break;
      }
      else {
         //mat = sp34b;
         g4 = -g4;
         g6 = -g6;
         //sptext2 = "SP34b-5";
         //g_store.Store(sptext2, vin);
      }
      break;
   }
   case 6:  /* --+ - > +++, but 0-+ -> 0-- and -0+ - > -0- and 00+ -> 00- */
   {
      if ((bZeroPattern & 4) == 4) {
         //mat = sp34b;
         g4 = -g4;
         g6 = -g6;
         //sptext2 = "SP34b-5";
         ////g_store.Store(sptext2, vin);
         break;
      }
      else if ((bZeroPattern & 2) == 2) {
         //mat = sp34c;
         g5 = -g5;
         g6 = -g6;
         //sptext2 = "SP34c-5";
         //g_store.Store(sptext2, vin);
         break;
      }
      else {
         //mat = sp34a;
         g4 = -g4;
         g5 = -g5;
         //sptext2 = "SP34a-6";
         //g_store.Store(sptext2, vin);
      }
      break;
   }
   case 7:
   {
      //mat = spnull;
      //sptext2 = "no-mknorm_action_sp1,sp2-7";
      //g_store.Store(sptext2, vin);
      break;
   }
   }

   //vout = mat * vin;
   vout = vin;
   //   DEBUG_REPORT_STRING(LRL_ToString( "      MKNORM input  "+sptext2+"  ", vi));;
   //   DEBUG_REPORT_STRING(LRL_ToString( "      MKNORM output "+sptext2+"  ", vout));;
   //std::cout << std::flush;
}

//-----------------------------------------------------------------------------
// Name: MKnorm()
// Description: changes a G6 vector to standard presentation (often called
//              normalization in the literature) and returns the standard
//              vector and the matrix that changes the input vector to the
//              standard one
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void Niggli::MKnorm(const G6& vi, MatG6& m, G6& vout, const double delta) {
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   // These are the matrices that can be used to convert a vector to standard
   // presentation. They are used in MKnorm. There are NO OTHER POSSIBILITIES.
   // The numbering is from Andrews and Bernstein, 1988
   const static MatG6 spnull("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1");

   const static MatG6 sp1("0 1 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 1");
   const static MatG6 sp2("1 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 0");


   const static MatG6 sp34a("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1 0 0 0 0 0 0  1");
   const static MatG6 sp34b("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0  1 0 0 0 0 0 0 -1");
   const static MatG6 sp34c("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0  1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1");
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


   const static MatG6 R5_Plus(" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 -1 0 0   0 -2 0 1 0 0   0 0 0 0 1 -1   0 0 0 0 0 1");
   const static MatG6 R5_Minus(" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 +1 0 0   0 +2 0 1 0 0   0 0 0 0 1 +1   0 0 0 0 0 1");
   const static MatG6 R6_Plus(" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 -1 0   0 0 0 1 0 -1   -2 0 0 0 1 0   0 0 0 0 0 1");
   const static MatG6 R6_Minus(" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 +1 0   0 0 0 1 0 +1   +2 0 0 0 1 0   0 0 0 0 0 1");
   const static MatG6 R7_Plus(" 1 0 0 0 0 0   1 1 0 0 0 -1   0 0 1 0 0 0   0 0 0 1 -1 0   0 0 0 0 1 0   -2 0 0 0 0 1");
   const static MatG6 R7_Minus(" 1 0 0 0 0 0   1 1 0 0 0 +1   0 0 1 0 0 0   0 0 0 1 +1 0   0 0 0 0 1 0   +2 0 0 0 0 1");
   const static MatG6 R8(" 1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 2 0 1 0 1   2 0 0 0 1 1    0 0 0 0 0 1");
   const static MatG6 R9_Plus(R5_Plus);
   const static MatG6 R9_Minus(R5_Minus);
   const static MatG6 R10_Plus(R6_Plus);
   const static MatG6 R10_Minus(R6_Minus);
   const static MatG6 R11_Plus(R7_Plus);
   const static MatG6 R11_Minus(R7_Minus);
   //const MatG6 R12(R8);
   const static MatG6 R12("1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 -2 0 -1 0 -1   -2 0 0 0 -1 -1   0 0 0 0 0 1");
   bool again = true;
   int mkCycleCount = 0;
   MatG6 mat = MatG6::Eye();
   MatG6 accumulator = mat;
   G6 vin;
   m = MatG6::Eye();

   vin = vi;

   // assure that g1<=g2<=g3
   while (again && (mkCycleCount <= 5))
   {
      ++mkCycleCount = 0;
      again = false;

      std::string sptext;
      if ((fabs(vin[0]) > fabs(vin[1]) + delta + 1.e-12 * (vin[0] + vin[1])) ||
         (fabs(vin[0] - vin[1]) < 1.e-38 + 1.e-12 * fabs(vin[0] + vin[1])
            && delta<1.0E-12 && fabs(vin[3])>fabs(vin[4]) +
            delta + 1.e-12 * (fabs(vin[3]) + fabs(vin[4]))))
      { // SP1
         mat = sp1;
         accumulator = mat * accumulator;
         again = true;
         sptext = "SP1";
      }
      else if ((fabs(vin[1]) > fabs(vin[2]) + delta + 1.e-12 * (vin[1] + vin[2])) ||
         (fabs(vin[1] - vin[2]) < 1.e-38 + 1.e-12 * fabs(vin[1] + vin[2])
            && delta<1.0E-12 && fabs(vin[4])>fabs(vin[5]) +
            delta + 1.e-12 * (fabs(vin[4]) + fabs(vin[5]))))
      { // SP2
         mat = sp2;
         accumulator = mat * accumulator;
         again = true;
         sptext = "SP2";
      }

      if (again)
      {
         // Accumulate the total transformation from the input vector into vout and
         // the total transformation itself into matrix m.
         const MatG6 mtemp = mat * m;
         m = mtemp;
         vout = mat * vin;
         Reporter(sptext, vin, vout, mat);
         vin = vout;
      }
      //      DEBUG_REPORT_STRING(LRL_ToString( "   MKNORM input  "+sptext+"  ", vi));;
      //      DEBUG_REPORT_STRING(LRL_ToString( "   MKNORM output "+sptext+"  ", vout));;
   }

   // now we assure (within delta) that the vector is +++ or ---

   int bMinusPattern = 0;
   int bZeroPattern = 0;
   if (vin[3] < delta + 1.0E-13 * (vin[1] + vin[2])) bMinusPattern |= 4;
   if (vin[4] < delta + 1.0E-13 * (vin[0] + vin[2])) bMinusPattern |= 2;
   if (vin[5] < delta + 1.0E-13 * (vin[0] + vin[1])) bMinusPattern |= 1;
   if (fabs(vin[3]) < delta + 1.0E-13 * (vin[1] + vin[2])) bZeroPattern |= 4;
   if (fabs(vin[4]) < delta + 1.0E-13 * (vin[0] + vin[2])) bZeroPattern |= 2;
   if (fabs(vin[5]) < delta + 1.0E-13 * (vin[0] + vin[1])) bZeroPattern |= 1;
   std::string sptext2("Not_All_++ + / -- - _in_MKnorm");

   switch (bMinusPattern)
   {
   case 0:  /*  +++  */
   {
      mat = spnull;
      accumulator = mat * accumulator;
      sptext2 = "no_mknorm_action_sp1,sp2-0";
      break;
   }
   case 1:  /* ++- -> --- */
   {
      mat = sp34a;
      accumulator = mat * accumulator;
      sptext2 = "SP34a-1";
      break;
   }
   case 2:  /* +-+ -> --- */
   {
      mat = sp34b;
      accumulator = mat * accumulator;
      sptext2 = "SP34b-2";
      break;
   }
   case 3:  /* +-- -> +++, but +0- -> -0- and +-0 -> --0 and +00 -> -00 */
   {
      mat = sp34c;
      accumulator = mat * accumulator;
      sptext2 = "SP34c-3";
      if ((bZeroPattern & 2) == 2) {
         mat = sp34a;
         accumulator = mat * accumulator;
         sptext2 = "SP34a-3";
         break;
      }
      if ((bZeroPattern & 1) == 1) {
         mat = sp34b;
         accumulator = mat * accumulator;
         sptext2 = "SP34b-3";
         break;
      }
      break;
   }
   case 4:  /* -++ -> --- */
   {
      mat = sp34c;
      accumulator = mat * accumulator;
      sptext2 = "SP34c-4";
      break;
   }
   case 5:  /* -+- -> +++, but 0+- -> 0-- and -+0 -> --0 and 0+0 -> 0-0 */
   {
      mat = sp34b;
      accumulator = mat * accumulator;
      sptext2 = "SP34b-5";
      if ((bZeroPattern & 4) == 4) {
         mat = sp34a;
         accumulator = mat * accumulator;
         sptext2 = "SP34a-5";
         break;
      }
      if ((bZeroPattern & 1) == 1) {
         mat = sp34c;
         accumulator = mat * accumulator;
         sptext2 = "SP34c-5";
         break;
      }
      break;
   }
   case 6:  /* --+ - > +++, but 0-+ -> 0-- and -0+ - > -0- and 00+ -> 00- */
   {
      mat = sp34a;
      accumulator = mat * accumulator;
      sptext2 = "SP34a-6";
      if ((bZeroPattern & 4) == 4) {
         mat = sp34b;
         accumulator = mat * accumulator;
         sptext2 = "SP34b-5";
         break;
      }
      if ((bZeroPattern & 2) == 2) {
         mat = sp34c;
         accumulator = mat * accumulator;
         sptext2 = "SP34c-5";
         break;
      }
      break;
   }
   case 7:
   {
      mat = spnull;
      accumulator = mat * accumulator;
      sptext2 = "no_mknorm_action_sp1,sp2-7";
      break;
   }
   }

   // Accumulate the total transformation from the input vector into vout and
   // the total transformation itself into matrix m.
   vout = mat * vin;
   //   DEBUG_REPORT_STRING(LRL_ToString( "      MKNORM input  "+sptext2+"  ", vi));;
   //   DEBUG_REPORT_STRING(LRL_ToString( "      MKNORM output "+sptext2+"  ", vout));;
   //std::cout << std::flush;
   Reporter(sptext2, vin, vout, accumulator);
}  // end MKnorm

bool Niggli::Reduce(const G6& vi, G6& vout) {
   const bool b = Selling::Reduce(vi, vout);
   return ReduceWithoutMatrices(vout, vout, 0.0);
}

bool Niggli::Reduce(const G6& vi, G6& vout, const bool sellingFirst) {
   if (IsNiggli(vi)) {
      vout = vi;
      return true;
   }
   S6 s6out;
   bool b = true;
   if (sellingFirst) {
      b = Selling::Reduce(S6(vi), s6out);
      vout = D7(s6out);
   }
   else {
      vout = vi;
   }

   if (!b) {
      MatG6 m;
      const bool bniggli = Niggli::Reduce(vi, m, vout, 0.0);
      return bniggli;
   }
   else if (IsNiggli(vout)) {
      return true;
   }
   else {
      MatG6 m;
      return Niggli::Reduce(vi, m, vout, 0.0);
   }
}

bool Niggli::ReduceWithoutMatrices(const G6& vi, G6& vout, const double delta)
{

   if (Niggli::IsNiggli(vi)) {
      vout = vi;
      return true;
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   // These are the matrices that can be used to convert a vector to standard
   // presentation. They are used in MKnorm. There are NO OTHER POSSIBILITIES.
   // The numbering is from Andrews and Bernstein, 1988
   const static MatG6 spnull("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1");

   const static MatG6 sp1("0 1 0 0 0 0   1 0 0 0 0 0   0 0 1 0 0 0   0 0 0 0 1 0   0 0 0 1 0 0   0 0 0 0 0 1");
   const static MatG6 sp2("1 0 0 0 0 0   0 0 1 0 0 0   0 1 0 0 0 0   0 0 0 1 0 0   0 0 0 0 0 1   0 0 0 0 1 0");


   const static MatG6 sp34a("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1 0 0 0 0 0 0  1");
   const static MatG6 sp34b("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0  1 0 0 0 0 0 0 -1");
   const static MatG6 sp34c("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0  1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1");
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


   const static MatG6 R5_Plus(" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 -1 0 0   0 -2 0 1 0 0   0 0 0 0 1 -1   0 0 0 0 0 1");
   const static MatG6 R5_Minus(" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 +1 0 0   0 +2 0 1 0 0   0 0 0 0 1 +1   0 0 0 0 0 1");
   const static MatG6 R6_Plus(" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 -1 0   0 0 0 1 0 -1   -2 0 0 0 1 0   0 0 0 0 0 1");
   const static MatG6 R6_Minus(" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 +1 0   0 0 0 1 0 +1   +2 0 0 0 1 0   0 0 0 0 0 1");
   const static MatG6 R7_Plus(" 1 0 0 0 0 0   1 1 0 0 0 -1   0 0 1 0 0 0   0 0 0 1 -1 0   0 0 0 0 1 0   -2 0 0 0 0 1");
   const static MatG6 R7_Minus(" 1 0 0 0 0 0   1 1 0 0 0 +1   0 0 1 0 0 0   0 0 0 1 +1 0   0 0 0 0 1 0   +2 0 0 0 0 1");
   const static MatG6 R8(" 1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 2 0 1 0 1   2 0 0 0 1 1    0 0 0 0 0 1");
   const static MatG6 R9_Plus(R5_Plus);
   const static MatG6 R9_Minus(R5_Minus);
   const static MatG6 R10_Plus(R6_Plus);
   const static MatG6 R10_Minus(R6_Minus);
   const static MatG6 R11_Plus(R7_Plus);
   const static MatG6 R11_Minus(R7_Minus);
   //const MatG6 R12(R8);
   const static MatG6 R12("1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 -2 0 -1 0 -1   -2 0 0 0 -1 -1   0 0 0 0 0 1");
   G6 vin;
   size_t reduceCycleCount = 0;
   bool again = true;
   const bool debug = true;

   const int maxCycle = 1000;

   vin = vi;
   G6 voutPrev(vin);

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

   double& g1 = vout[0];
   double& g2 = vout[1];
   double& g3 = vout[2];
   double& g4 = vout[3];
   double& g5 = vout[4];
   double& g6 = vout[5];

   // Andrews and Bernstein, 1988
   MKnormWithoutMatrices(vin, vout, delta);
   while (again && reduceCycleCount < maxCycle)
   {
      //      DEBUG_REPORT_STRING(LRL_ToString( "REDUCE start cycle  ", ncycle, "  ", vin));;
      vin = vout;

      if (fabs(g4) > fabs(g2) + delta)
      { // R5
         //const MatG6 m1 =(vin[3] <= 0.0) ? R5_Minus : R5_Plus;
         again = true;
         ////         DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R5-m1\n", m1,"\n"));;
         //vout = m1 * vin;
         if (g4 <= 0.0) {
            g3 = g2 + g3 + g4;  // R5_Minus & R9_Minus
            g4 = 2 * g2 + g4;
            g5 = g5 + g6;
            //g_store.Store("R5_Minus", vin);
         }
         else {
            g3 = g2 + g3 - g4;  // R5_Plus & R9_Plus
            g4 = -2 * g2 + g4;
            g5 = g5 - g6;
            //g_store.Store("R5_Plus", vin);
         }
      }
      else if (fabs(g5) > fabs(g1) + delta)
      { // R6
         //const MatG6 m1 =(vin[4] <= 0.0) ? R6_Minus : R6_Plus;
         again = true;
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R6-m1\n", m1,"\n"));;
         //vout = m1 * vin;
         if (g5 <= 0.0) {
            g3 = g1 + g3 + g5;  // R6_Minus & R10_Minus
            g4 = g4 + g6;
            g5 = 2 * g1 + g5;
            //g_store.Store("R6_Minus", vin);
         }
         else {
            g3 = g1 + g3 - g5;  // R6_Plus & R10_Plus
            g4 = g4 - g6;
            //g_store.Store("R6_Plus", vin);
            g5 = -2 * g1 + g5;
         }
      }
      else if (fabs(g6) > fabs(g1) + delta)
      { // R7
         //const MatG6 m1 =(vin[5] <= 0.0) ? R7_Minus : R7_Plus;
         again = true;
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R7-m1\n", m1,"\n"));;
         //vout = m1 * vin;
         if (g6 <= 0.0) {
            g2 = g1 + g2 + g6;  // R7_Minus & R11_Minus
            g4 = g4 + g5;
            g6 = 2 * g1 + g6;
            //g_store.Store("R7_Minus", vin);
         }
         else {
            g2 = g1 + g2 - g6;  // R7_Plus && R11_Plus
            g4 = g4 - g5;
            g6 = -2 * g1 + g6;
            //g_store.Store("R7_Plus", vin);
         }
      }
      else if (g4 + g5 + g6 + fabs(g1) + fabs(g2) + delta < 0.0)
      { //R8
         //const MatG6 m1 = R8;
         again = true;
         //         DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R8-m1\n", m1,"\n"));;
         //vout = m1 * vin;
         g3 = g1 + g2 + g3 + g4 + g5 + g6;  //  R8
         g4 = 2 * g2 + g4 + g6;
         g5 = 2 * g1 + g5 + g6;
         //g_store.Store("R8", vin);
      }
      else if ((fabs(g4 - g2) <= delta && 2.0 * g5 - delta < g6) ||
         (fabs(g4 + g2) <= delta && g6 < 0.0))
      { // R9  There is an error in the paper says "2g5<g5" should be "2g5<g6"
         //const MatG6 m1 =(vin[3] <= 0.0) ? R9_Minus : R9_Plus;
         again = true;
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R9-m1\n", m1,"\n"));;
         //vout = m1 * vin;
         if (g4 <= 0.0) {
            g3 = g2 + g3 + g4;  // R5_Minus & R9_Minus
            g4 = 2 * g2 + g4;
            g5 = g5 + g6;
            //g_store.Store("R9_Minus", vin);
         }
         else {
            g3 = g2 + g3 - g4;  // R5_Plus & R9_Plus
            g4 = -2 * g2 + g4;
            g5 = g5 - g6;
            //g_store.Store("R9_Plus", vin);
         }
      }
      else if ((fabs(g5 - g1) <= delta && 2.0 * g4 - delta < g6) ||
         (fabs(g5 + g1) <= delta && g6 < 0.0))
      { //R10 (LAST=15 in ITERATE)
         //const MatG6 m1 =(vin[4] <= 0.0) ? R10_Minus : R10_Plus;
         again = true;
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R10-m\n", m,"\n"));
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R10-m1\n", m1,"\n"));
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R10-m2=m1*m\n", m2,"\n"));
         //vout = m1 * vin;
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R10-in  ", vin));
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R10-out  ", vout))
         if (g5 <= 0.0) {
            g3 = g1 + g3 + g5;  // R6_Minus & R10_Minus
            g4 = g4 + g6;
            g5 = 2 * g1 + g5;
            //g_store.Store("R10_Minus", vin);
         }
         else {
            g3 = g1 + g3 - g5;  // R6_Plus & R10_Plus
            g4 = g4 - g6;
            g5 = -2 * g1 + g5;
            //g_store.Store("R10_Plus", vin);
         }
      }
      else if ((fabs(g6 - g1) <= delta && 2.0 * g4 - delta < g5) ||
         (fabs(g6 + g1) <= delta && g5 < 0.0)) // paper says g6<0, but it seems wrong
      { // R11
         //const MatG6 m1 =(vin[5] <= 0.0) ? R11_Minus : R11_Plus;
         again = true;
         //         DEBUG_REPORT_STRING(LRL_ToString( "REDUCE 1-m1\n", m1,"\n"));;
         //vout = m1 * vin;
         if (g6 <= 0.0) {
            g2 = g1 + g2 + g6;  // R7_Minus & R11_Minus
            g4 = g4 + g5;
            g6 = 2 * g1 + g6;
            //g_store.Store("R11_Minus", vin);
         }
         else {
            g2 = g1 + g2 - g6;  // R7_Plus && R11_Plus
            g4 = g4 - g5;
            g6 = -2 * g1 + g6;
            //g_store.Store("R11_Plus", vin);
         }
      }
      else if (fabs(g4 + g5 + g6 + fabs(g1) + fabs(g2)) <= delta && (2.0 * (fabs(g1) + g5) + g6 > delta))
      { //R12 
         //const MatG6 m1 =R12;
         again = true;
         //         DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R12-m1\n", m1,"\n"));;
         //vout = m1 * vin;
         g3 = g1 + g2 + g3 + g4 + g5 + g6;  //  R12
         g4 = -2 * g2 - g4 - g6;
         g5 = -2 * g1 - g5 - g6;
         //g_store.Store("R12", vin);
      }
      else
      {
         again = false;
         vout = vin;
      }

      // probably don't need to do this group of code when again==false !!!!!!!!!!!!!!
      if (again) {
         MKnormWithoutMatrices(vout, vin, delta);
         vout = vin;
      }
      for (size_t i = 3; i < 6; ++i)
         if (std::fabs(vin[i]) < 1.0E-10) vin[i] = 0.0;

      if (vin[0] < 0.0 || vin[1] < 0.0 || vin[2] < 0.0) {
         // ERROR ERROR ERROR
         if (DEBUG_REDUCER) {
            fprintf(stderr, " Negative sq, axis %d \n", (int)(reduceCycleCount));
            fprintf(stderr, " vin: [%g,%g,%g,%g,%g,%g]\n",
               vin[0], vin[1], vin[2], vin[3], vin[4], vin[5]);
            fprintf(stderr, " vi: [%g,%g,%g,%g,%g,%g]\n",
               vi[0], vi[1], vi[2], vi[3], vi[4], vi[5]);
         }
         return(false);
      }

      if (reduceCycleCount == 0) voutPrev = vin;

      if (DEBUG_REDUCER)
      {
         //printf( "%d    %f %f %f %f %f %f\n", m_ReductionCycleCount, vout[0], vout[1], vout[2], vout[3], vout[4], vout[5] );
      }
      //      DEBUG_REPORT_STRING(LRL_ToString( "REDUCE output  ", vi));;

//      if (reduceCycleCount > 250) {
//         std::cout << std::endl;
//         std::cout << "cycle " << reduceCycleCount << std::endl;
//         std::cout << "\tvin  " << vin << std::endl;
//         std::cout << "\tvout " << vout << std::endl;
//      }

      ++reduceCycleCount;
   }

   bool isNearReduced = Niggli::NearRed(vout, delta);
   if (reduceCycleCount >= maxCycle) {
      if (isNearReduced) {
         std::cout << "THERE IS A REDUCE PROBLEM (B), m_ReductionCycleCount " << reduceCycleCount << std::endl;
         std::cout << vout << std::endl;
      }
   }
   m_ReductionCycleCount = reduceCycleCount;
   return (reduceCycleCount <= maxCycle) || isNearReduced;

}

//-----------------------------------------------------------------------------
// Name: Reduce()
// Description: performs Niggli reduction and returns the standard reduced
//              vector and the matrix that changes the input vector to the
//              reduced one
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool Niggli::Reduce(const G6& vi, MatG6& m, G6& vout, const double delta)
{

   MatG6 accumulator = MatG6::Eye();

   if (IsNiggli(vi)) {
      vout = vi;
      return true;
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   // These are the matrices that can be used to convert a vector to standard
   // presentation. They are used in MKnorm. There are NO OTHER POSSIBILITIES.
   // The numbering is from Andrews and Bernstein, 1988
   const static MatG6 spnull("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1");

   const static MatG6 sp1("0 1 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 1");
   const static MatG6 sp2("1 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 0");


   const static MatG6 sp34a("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1 0 0 0 0 0 0  1");
   const static MatG6 sp34b("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0  1 0 0 0 0 0 0 -1");
   const static MatG6 sp34c("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0  1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1");
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


   const static MatG6 R5_Plus(" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 -1 0 0   0 -2 0 1 0 0   0 0 0 0 1 -1   0 0 0 0 0 1");
   const static MatG6 R5_Minus(" 1 0 0 0 0 0   0 1 0 0 0 0  0 1 1 +1 0 0   0 +2 0 1 0 0   0 0 0 0 1 +1   0 0 0 0 0 1");
   const static MatG6 R6_Plus(" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 -1 0   0 0 0 1 0 -1   -2 0 0 0 1 0   0 0 0 0 0 1");
   const static MatG6 R6_Minus(" 1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 +1 0   0 0 0 1 0 +1   +2 0 0 0 1 0   0 0 0 0 0 1");
   const static MatG6 R7_Plus(" 1 0 0 0 0 0   1 1 0 0 0 -1   0 0 1 0 0 0   0 0 0 1 -1 0   0 0 0 0 1 0   -2 0 0 0 0 1");
   const static MatG6 R7_Minus(" 1 0 0 0 0 0   1 1 0 0 0 +1   0 0 1 0 0 0   0 0 0 1 +1 0   0 0 0 0 1 0   +2 0 0 0 0 1");
   const static MatG6 R8(" 1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 2 0 1 0 1   2 0 0 0 1 1    0 0 0 0 0 1");
   const static MatG6 R9_Plus(R5_Plus);
   const static MatG6 R9_Minus(R5_Minus);
   const static MatG6 R10_Plus(R6_Plus);
   const static MatG6 R10_Minus(R6_Minus);
   const static MatG6 R11_Plus(R7_Plus);
   const static MatG6 R11_Minus(R7_Minus);
   //const MatG6 R12(R8);
   const static MatG6 R12("1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 1 1 1   0 -2 0 -1 0 -1   -2 0 0 0 -1 -1   0 0 0 0 0 1");
   G6 vin;
   MatG6 m1;
   size_t count = 0;
   bool again = true;
   const bool debug = true;

   const int maxCycle = 260;

   m1 = MatG6::Eye();
   vin = vi;
   G6 voutPrev(vin);

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
   m1 = MatG6::Eye();
   MKnorm(vin, m1, vout, delta);
   accumulator = m1 * accumulator;
   vin = vout;
   while (again && count < maxCycle)
   {
      //      DEBUG_REPORT_STRING(LRL_ToString( "REDUCE start m_ReductionCycleCount  ", m_ReductionCycleCount, "  ", vin));;

      m1 = m;
      const MatG6 m2temp = m1 * m;
      m = m2temp;
      m1 = MatG6::Eye();

      if (fabs(vin[3]) > fabs(vin[1]) + delta)
      { // R5
         m1 = (vin[3] <= 0.0) ? R5_Minus : R5_Plus;
         accumulator = m1 * accumulator;
         again = true;
         ////         DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R5-m1\n", m1,"\n"));;
         vout = m1 * vin;
         Reporter("R5", vin, vout, m1);
      }
      else if (fabs(vin[4]) > fabs(vin[0]) + delta)
      { // R6
         m1 = (vin[4] <= 0.0) ? R6_Minus : R6_Plus;
         accumulator = m1 * accumulator;
         again = true;
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R6-m1\n", m1,"\n"));;
         vout = m1 * vin;
         Reporter("R6", vin, vout, m1);
      }
      else if (fabs(vin[5]) > fabs(vin[0]) + delta)
      { // R7
         m1 = (vin[5] <= 0.0) ? R7_Minus : R7_Plus;
         accumulator = m1 * accumulator;
         again = true;
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R7-m1\n", m1,"\n"));;
         vout = m1 * vin;
         Reporter("R7", vin, vout, m1);
      }
      else if (vin[3] + vin[4] + vin[5] + fabs(vin[0]) + fabs(vin[1]) + delta < 0.0)
      { //R8
         m1 = R8;
         accumulator = m1 * accumulator;
         again = true;
         //         DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R8-m1\n", m1,"\n"));;
         vout = m1 * vin;
         Reporter("R8", vin, vout, m1);
      }
      else if ((fabs(vin[3] - vin[1]) <= delta && 2.0 * vin[4] - delta < vin[5]) ||
         (fabs(vin[3] + vin[1]) <= delta &&
            vin[5] < 0.0))
      { // R9  There is an error in the paper says "2g5<g5" should be "2g5<g6"
         m1 = (vin[3] <= 0.0) ? R9_Minus : R9_Plus;
         accumulator = m1 * accumulator;
         again = true;
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R9-m1\n", m1,"\n"));;
         vout = m1 * vin;
         Reporter("R9", vin, vout, m1);
      }
      else if ((fabs(vin[4] - vin[0]) <= delta && 2.0 * vin[3] - delta < vin[5]) ||
         (fabs(vin[4] + vin[0]) <= delta &&
            vin[5] < 0.0))
      { //R10 (LAST=15 in ITERATE)
         m1 = (vin[4] <= 0.0) ? R10_Minus : R10_Plus;
         accumulator = m1 * accumulator;
         again = true;
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R10-m\n", m,"\n"));
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R10-m1\n", m1,"\n"));
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R10-m2=m1*m\n", m2,"\n"));
         vout = m1 * vin;
         Reporter("R10", vin, vout, m1);
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R10-in  ", vin));
          //       DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R10-out  ", vout))
      }
      else if ((fabs(vin[5] - vin[0]) <= delta && 2.0 * vin[3] - delta < vin[4]) ||
         (fabs(vin[5] + vin[0]) <= delta &&
            vin[4] < 0.0)) // paper says g6<0, but it seems wrong
      { // R11
         m1 = (vin[5] <= 0.0) ? R11_Minus : R11_Plus;
         accumulator = m1 * accumulator;
         again = true;
         //         DEBUG_REPORT_STRING(LRL_ToString( "REDUCE 1-m1\n", m1,"\n"));;
         vout = m1 * vin;
         Reporter("R11", vin, vout, m1);
      }
      else if (fabs(vin[3] + vin[4] + vin[5] + fabs(vin[0]) + fabs(vin[1])) <= delta && (2.0 * (fabs(vin[0]) + vin[4]) + vin[5] > delta))
      { //R12 (same as R8)
         m1 = R12;
         accumulator = m1 * accumulator;
         again = true;
         //         DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R12-m1\n", m1,"\n"));;
         vout = m1 * vin;
         Reporter("R12", vin, vout, m1);
      }
      else
      {
         again = false;
         vout = vin;
      }

      // probably don't need to do this group of code when again==false !!!!!!!!!!!!!!
      m1 = MatG6::Eye();
      MKnorm(vout, m1, vin, delta);
      //accumulator = m1 * accumulator;
      const MatG6 mtemp = m1 * m;
      m = mtemp;
      Reporter("vout after MKnorm at end of reduced m_ReductionCycleCount", vout, vin, m1);
      for (size_t i = 3; i < 6; ++i)
         if (std::fabs(vin[i]) < 1.0E-10) vin[i] = 0.0;

      if (vin[0] < 0.0 || vin[1] < 0.0 || vin[2] < 0.0) {
         // ERROR ERROR ERROR
         if (DEBUG_REDUCER) {
            fprintf(stderr, " Negative sq, axis %d \n", (int)(count));
            fprintf(stderr, " vin: [%g,%g,%g,%g,%g,%g]\n",
               vin[0], vin[1], vin[2], vin[3], vin[4], vin[5]);
            fprintf(stderr, " vi: [%g,%g,%g,%g,%g,%g]\n",
               vi[0], vi[1], vi[2], vi[3], vi[4], vi[5]);
         }
         return(false);
      }

      if (count == 0) voutPrev = vin;

      if (DEBUG_REDUCER)
      {
         //printf( "%d    %f %f %f %f %f %f\n", m_ReductionCycleCount, vout[0], vout[1], vout[2], vout[3], vout[4], vout[5] );
      }
      //      DEBUG_REPORT_STRING(LRL_ToString( "REDUCE output  ", vi));;

      ++count;
      //      if (count > 250) {
      //         std::cout << std::endl;
      //         std::cout << "cycle " << count << std::endl;
      //         std::cout << "\tvin  " << vin << std::endl;
      //         std::cout << vin[0] - vin[1] << " " << vin[0] - vin[2] << " " << vin[1] - vin[2] << std::endl;
      //         std::cout << "\tvout " << vout << std::endl;
      //         std::cout << vout[0] - vout[1] << " " << vout[0] - vout[2] << " " << vout[1] - vout[2] << std::endl;
      //      }
      m1 = MatG6::Eye();
      MKnorm(vout, m1, vin, delta);
      accumulator = m1 * accumulator;

      //std::cout << "in Niggli::Reduce  end of cycle " << count << " in reduced\n";
      //std::cout << "in Niggli::Reduce  m \n"<< m <<std::endl;
      //std::cout << "in Niggli::Reduce  vin " << vin << std::endl;
      //std::cout << "in Niggli::Reduce  vi " << vi << std::endl;
      //std::cout << "in Niggli::Reduce  vout " << vout << std::endl;
      //std::cout << "in Niggli::Reduce m1*vi " << m1 * vi << std::endl << std::endl;
      //std::cout << "in Niggli::Reduce m*vi " << m * vi << std::endl << std::endl;
      m = m1;
   }

   vout = vin;
   bool isNearReduced = NearRed(vout, delta);
   if (count >= maxCycle) {
      if (!isNearReduced) {
         std::cout << " vi " << vi << std::endl;
         std::cout << "THERE IS A REDUCE PROBLEM (A), m_ReductionCycleCount " << count << std::endl;
      }
   }
   m_ReductionCycleCount = count;
   m = accumulator;
   return (count < maxCycle) || isNearReduced;

} // end of Reduce


//-----------------------------------------------------------------------------
// Name: NearRed()
// Description: test whether a vector is nearly Niggli reduced
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool Niggli::NearRed(const G6& gvec, const double delta)
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
   if (gvec[0] < -delta ||
      gvec[1] < -delta ||
      gvec[2] < -delta ||
      gvec[0] > gvec[1] + delta ||
      gvec[1] > gvec[2] + delta)
   {
      return(false);
   }
   //C
   //C     TEST FOR NON-REDUCED SIGN COMBINATIONS IN
   //C     G4, G5 AND G6
   //C
   if ((gvec[3] <= delta ||
      gvec[4] <= delta ||
      gvec[5] <= delta) &&
      (gvec[4] > delta ||
         gvec[4] > delta ||
         gvec[5] > delta))
   {
      return(false);
   }
   //C
   //C     TEST ABS(G{4,5,6}) AGAINST G{1,2,3}
   //C
   if (fabs(gvec[3]) > fabs(gvec[2]) + delta ||
      fabs(gvec[4]) > fabs(gvec[0]) + delta ||
      fabs(gvec[5]) > fabs(gvec[0]) + delta)
   {
      return(false);
   }
   //C
   //C     TEST THE BODY DIAGONAL
   //C
   if (gvec[3] + gvec[4] + gvec[5] + fabs(gvec[0]) + fabs(gvec[1]) + delta < 0.0) return(false);
   if (delta > 0.0) return(true);
   //C
   //C     TEST THE 678, 9AB, CDE BOUNDARY FOLDS
   //C
   if ((gvec[3] == gvec[1] && 2.0 * gvec[4] < gvec[5]) ||
      (gvec[4] == gvec[0] && 2.0 * gvec[3] < gvec[5]) ||
      (gvec[5] == gvec[0] && 2.0 * gvec[3] < gvec[4]) ||
      (gvec[3] + gvec[1] == 0.0 && gvec[5] <= 0.0) ||
      (gvec[4] + gvec[0] == 0.0 && gvec[5] <= 0.0) ||
      (gvec[5] + gvec[0] == 0.0 && gvec[4] <= 0.0)) return(false);
   //C
   //C     TEST THE F BOUDARY FOLD
   //C
   if (fabs(gvec[3] + gvec[4] + gvec[5] + gvec[0] + gvec[1]) <= delta) return(false);

   return(true);
} // end NearRed

bool Niggli::IsNiggli(const S6& s) {
   G6 g{ D7{ s } };
   return IsNiggli(g);
}

bool Niggli::IsNiggli(const D7& d) {
   G6 g{ d };
   return IsNiggli(g);
}

bool Niggli::IsNiggli(const G6& v) {
   const double& g1 = v[0];
   const double& g2 = v[1];
   const double& g3 = v[2];
   const double& g4 = v[3];
   const double& g5 = v[4];
   const double& g6 = v[5];


   const double delta = 10E-6;

   if (g1 <= 0.0) return false;
   if (g2 <= 0.0) return false;
   if (g3 <= 0.0) return false;

   if (g1 > g2) return false;
   if (g2 > g3) return false;

   if (abs(g4) > g2) return false;
   if (abs(g5) > g1) return false;
   if (abs(g6) > g1) return false;
   int nneg = 0;
   for (size_t i = 3; i < 6; ++i) if (v[i] <= 0.0) ++nneg;
   if (nneg != 0 && nneg != 3) return false;

   if (g4 == g2 && g6 > 2.0 * g5) return false;
   if (g5 == g1 && g6 > 2.0 * g4) return false;
   if (g6 == g1 && g5 > 2.0 * g4) return false;


   if (abs(g2 - g3) < delta && abs(g5) > abs(g6)) return false;
   if (abs(g3) > g1 + g2 + g3 + g4 + g5 + g6) return false;

   if (g4 == g2 && g6 != 0) return false;
   if (g5 == g1 && g6 != 0) return false;
   if (g6 == g1 && g5 != 0) return false;

   if (g3 == (g1 + g2 + g3 + g4 + g5 + g6) && 2.0 * (g1 + g5) > 0.0) return false;

   return true;
}

void Niggli::ShowStoreResults() {
   //g_store.ShowResults();
}

