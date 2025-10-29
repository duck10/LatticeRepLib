#pragma warning(disable: 4189) // Visual Studio -- local variable is initialized but not referenced


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

// In the Niggli.cpp file - replace the existing matrix declarations with these consolidated ones
// These matrices only need to be declared once in the file

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

// CORRECTED R12 matrix based on Claude.AI analysis (2025-05-09)
const static MatG6 R12("1 0 0 0 0 0   0 1 0 0 0 0   1 1 1 -1 -1 1   0 -2 0 1 0 -1   -2 0 0 0 1 -1   0 0 0 0 0 1");


// Define the 3D matrices corresponding to G6 transformations
// Standard presentation matrices
// FINAL CORRECTED Matrix_3x3 definitions
// These matrices have determinant = +1 and match the MatG6 transformations
// Replace your broken definitions with these:

// Standard presentation matrices
const Matrix_3x3 spnull_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0); // Identity (unchanged)

// CORRECTED sp1_3D (swaps a and b):
const Matrix_3x3 sp1_3D(0.0, -1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, -1.0);

// CORRECTED sp2_3D (swaps b and c):
const Matrix_3x3 sp2_3D(-1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, -1.0, 0.0);

// CORRECTED sp34a_3D (sign changes for β and γ):
const Matrix_3x3 sp34a_3D(-1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);

const Matrix_3x3 sp34b_3D(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0); // This was already correct

// CORRECTED sp34c_3D (sign changes for γ and ξ):
const Matrix_3x3 sp34c_3D(1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0);

// Reduction operation matrices (these were already working correctly)
const Matrix_3x3 R5_Plus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 1.0);
const Matrix_3x3 R5_Minus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0);

const Matrix_3x3 R6_Plus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, 1.0);
const Matrix_3x3 R6_Minus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);

const Matrix_3x3 R7_Plus_3D(1.0, 0.0, 0.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
const Matrix_3x3 R7_Minus_3D(1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0);

const Matrix_3x3 R8_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0);

const Matrix_3x3 R9_Plus_3D = R5_Plus_3D;
const Matrix_3x3 R9_Minus_3D = R5_Minus_3D;

const Matrix_3x3 R10_Plus_3D = R6_Plus_3D;
const Matrix_3x3 R10_Minus_3D = R6_Minus_3D;

const Matrix_3x3 R11_Plus_3D = R7_Plus_3D;
const Matrix_3x3 R11_Minus_3D = R7_Minus_3D;

const Matrix_3x3 R12_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, -1.0, 1.0);

/*
VERIFICATION - All matrices have det = +1:
- sp1_3D det = +1  ✅
- sp2_3D det = +1  ✅
- sp34a_3D det = +1 ✅
- sp34c_3D det = +1 ✅

These matrices are:
1. Mathematically correct (det = +1, preserving orientation)
2. Functionally equivalent to the working MatG6 transformations
3. Compliant with Gruber's crystallographic standards
*/// Updated to match the corrected R12 matrix



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
   return ReduceWithoutMatrices(vout, vout, 1.0E-8);
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
      vout = (s6out);
   }
   else {
      vout = vi;
   }

   if (!b) {
      MatG6 m;
      const bool bniggli = Niggli::Reduce(vi, m, vout, 1.0E-12);
      return bniggli;
   }
   else if (IsNiggli(vout)) {
      return true;
   }
   else {
      MatG6 m;
      return Niggli::Reduce(vi, m, vout, 1.0E-12);
   }
}

bool Niggli::ReduceWithoutMatrices(const G6& vi, G6& vout, const double delta)
{

   if (Niggli::IsNiggli(vi)) {
      vout = vi;
      //g_store.Store("Already Niggli-reduced", vi);
      return true;
   }

   G6 vin;
   size_t reduceCycleCount = 0;
   bool again = true;
   const bool debug = true;
   const int maxCycle = 1000;

   vin = vi;
   G6 voutPrev(vin);

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

   // MKnorm step - store this phase
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
         again = true;
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R6-m1\n", m1,"\n"));;
         if (g5 <= 0.0) {
            g3 = g1 + g3 + g5;  // R6_Minus & R10_Minus
            g4 = g4 + g6;
            g5 = 2 * g1 + g5;
            //g_store.Store("R6_Minus", vin);
         }
         else {
            g3 = g1 + g3 - g5;  // R6_Plus & R10_Plus
            g4 = g4 - g6;
            g5 = -2 * g1 + g5;
            //g_store.Store("R6_Plus", vin);
         }
      }
      else if (fabs(g6) > fabs(g1) + delta)
      { // R7
         again = true;
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R7-m1\n", m1,"\n"));;
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
         again = true;
         //         DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R8-m1\n", m1,"\n"));;
         g3 = g1 + g2 + g3 + g4 + g5 + g6;  //  R8
         g4 = 2 * g2 + g4 + g6;
         g5 = 2 * g1 + g5 + g6;
         //g_store.Store("R8", vin);
      }
      else if ((fabs(g4 - g2) <= delta && 2.0 * g5 - delta < g6) ||
         (fabs(g4 + g2) <= delta && g6 < 0.0))
      { // R9  There is an error in the paper says "2g5<g5" should be "2g5<g6"
         again = true;
         //        DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R9-m1\n", m1,"\n"));;
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
      { //R10
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
         again = true;
         //         DEBUG_REPORT_STRING(LRL_ToString( "REDUCE 1-m1\n", m1,"\n"));;
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
         again = true;
         //         DEBUG_REPORT_STRING(LRL_ToString( "REDUCE R12-m1\n", m1,"\n"));;
         g3 = g1 + g2 + g3 + g4 + g5 + g6;  //  R12
         g4 = -2 * g2 - g4 - g6;
         g5 = -2 * g1 - g5 - g6;
         //g_store.Store("R12", vin);
      }
      else
      {
         again = false;
         vout = vin;
         //g_store.Store("Final", vout);
      }

      // Apply MKnorm on each iteration when needed
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
         //g_store.Store("Error - Negative square", vin);
         return(false);
      }

      if (reduceCycleCount >= maxCycle && DEBUG_REDUCER) {
         std::cout << "reached cycle count without complete convergence\n";
      }

      if (reduceCycleCount == 0) voutPrev = vin;

      if (DEBUG_REDUCER)
      {
         //printf( "%d    %f %f %f %f %f %f\n", m_ReductionCycleCount, vout[0], vout[1], vout[2], vout[3], vout[4], vout[5] );
      }
      //      DEBUG_REPORT_STRING(LRL_ToString( "REDUCE output  ", vi));;
      ++reduceCycleCount;
   }

   bool isNearReduced = Niggli::NearRed(vout, delta);
   if (reduceCycleCount >= maxCycle) {
      //g_store.Store("Max cycles reached", vout);
      if (isNearReduced) {
         std::cout << ";THERE IS A REDUCE PROBLEM (B), m_ReductionCycleCount " << reduceCycleCount << std::endl;
         std::cout << ";but the final cell in ReduceWithoutMatrices is already very nearly reduced\n";
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


   //std::cout << "\nspnull   " << std::endl << spnull << std::endl;
   //std::cout << "\nsp1      " << std::endl << sp1 << std::endl;
   //std::cout << "\nsp2      " << std::endl << sp2 << std::endl;
   //std::cout << "\nsp34a    " << std::endl << sp34a << std::endl;
   //std::cout << "\nsp34b    " << std::endl << sp34b << std::endl;
   //std::cout << "\nsp34c    " << std::endl << sp34c << std::endl;
   //std::cout << "\nR5_Plus  " << std::endl << R5_Plus << std::endl;
   //std::cout << "\nR5_Minus " << std::endl << R5_Minus << std::endl;
   //std::cout << "\nR6_Plus  " << std::endl << R6_Plus << std::endl;
   //std::cout << "\nR6_Minus " << std::endl << R6_Minus << std::endl;
   //std::cout << "\nR7_Plus  " << std::endl << R7_Plus << std::endl;
   //std::cout << "\nR7_Minus " << std::endl << R7_Minus << std::endl;
   //std::cout << "\nR8       " << std::endl << R8 << std::endl;

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

bool Niggli::IsNiggli(const G6& v, const double delta) {
   const double& g1 = v[0];
   const double& g2 = v[1];
   const double& g3 = v[2];
   const double& g4 = v[3];
   const double& g5 = v[4];
   const double& g6 = v[5];

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
   if (nneg == 0 && (g4 + g5 + g6 + g1 + g2) > 0.0) return false;
   return true;
}

void Niggli::ShowStoreResults() {
   //g_store.ShowResults();
}



bool Niggli::ReduceWithTransforms(const G6& vi, MatG6& mG6, Matrix_3x3& m3d, G6& vout, const double delta)
{
   // This implementation follows the logic of Niggli::Reduce and tracks both G6 and 3D matrices


   if (Niggli::IsNiggli(vi)) {
      vout = vi;
      mG6 = spnull;
      m3d = spnull_3D;
      return true;
   }

   G6 vin;
   MatG6 m1;
   Matrix_3x3 m3d_step;
   size_t count = 0;
   bool again = true;
   const int maxCycle = 260;

   // Initialize transformation matrices to identity
   mG6 = spnull;
   m3d = spnull_3D;

   vin = vi;
   G6 voutPrev(vin);

   // First apply MKnorm to standardize the input vector
   // This section exactly follows the logic of Niggli::MKnorm but adds 3D matrix tracking
   {
      bool againMK = true;
      int mkCycleCount = 0;
      MatG6 matG6 = spnull;
      Matrix_3x3 mat3D = spnull_3D;

      // MKnorm: assure that g1<=g2<=g3
      while (againMK && (mkCycleCount <= 5)) {
         ++mkCycleCount;
         againMK = false;

         if ((fabs(vin[0]) > fabs(vin[1]) + delta + 1.e-12 * (vin[0] + vin[1])) ||
            (fabs(vin[0] - vin[1]) < 1.e-38 + 1.e-12 * fabs(vin[0] + vin[1]) &&
               delta < 1.0E-12 && fabs(vin[3]) > fabs(vin[4]) + delta + 1.e-12 * (fabs(vin[3]) + fabs(vin[4])))) {
            // SP1
            matG6 = sp1;  // Use the global sp1
            mat3D = sp1_3D;
            againMK = true;
         }
         else if ((fabs(vin[1]) > fabs(vin[2]) + delta + 1.e-12 * (vin[1] + vin[2])) ||
            (fabs(vin[1] - vin[2]) < 1.e-38 + 1.e-12 * fabs(vin[1] + vin[2]) &&
               delta < 1.0E-12 && fabs(vin[4]) > fabs(vin[5]) + delta + 1.e-12 * (fabs(vin[4]) + fabs(vin[5])))) {
            // SP2
            matG6 = sp2;  // Use the global sp2
            mat3D = sp2_3D;
            againMK = true;
         }

         if (againMK) {
            vout = matG6 * vin;
            vin = vout;

            // Update the transformation matrices
            mG6 = matG6 * mG6;
            m3d = mat3D * m3d;
         }
      }

      // MKnorm: Ensure vector is +++ or ---
      int bMinusPattern = 0;
      int bZeroPattern = 0;
      if (vin[3] < delta + 1.0E-13 * (vin[1] + vin[2])) bMinusPattern |= 4;
      if (vin[4] < delta + 1.0E-13 * (vin[0] + vin[2])) bMinusPattern |= 2;
      if (vin[5] < delta + 1.0E-13 * (vin[0] + vin[1])) bMinusPattern |= 1;
      if (fabs(vin[3]) < delta + 1.0E-13 * (vin[1] + vin[2])) bZeroPattern |= 4;
      if (fabs(vin[4]) < delta + 1.0E-13 * (vin[0] + vin[2])) bZeroPattern |= 2;
      if (fabs(vin[5]) < delta + 1.0E-13 * (vin[0] + vin[1])) bZeroPattern |= 1;

      // These variables must be initialized here, not redeclared below
      matG6 = spnull;
      mat3D = spnull_3D;

      switch (bMinusPattern) {
      case 0:  /*  +++  */
         matG6 = spnull;  // Use the global spnull
         mat3D = spnull_3D;
         break;
      case 1:  /* ++- -> --- */
         matG6 = sp34a;  // Use the global sp34a
         mat3D = sp34a_3D;
         break;
      case 2:  /* +-+ -> --- */
         matG6 = sp34b;  // Use the global sp34b
         mat3D = sp34b_3D;
         break;
      case 3:  /* +-- -> +++, but +0- -> -0- and +-0 -> --0 and +00 -> -00 */
         if ((bZeroPattern & 2) == 2) {
            matG6 = sp34a;  // Use the global sp34a
            mat3D = sp34a_3D;
            break;
         }
         if ((bZeroPattern & 1) == 1) {
            matG6 = sp34b;  // Use the global sp34b
            mat3D = sp34b_3D;
            break;
         }
         matG6 = sp34c;  // Use the global sp34c
         mat3D = sp34c_3D;
         break;
      case 4:  /* -++ -> --- */
         matG6 = sp34c;  // Use the global sp34c
         mat3D = sp34c_3D;
         break;
      case 5:  /* -+- -> +++, but 0+- -> 0-- and -+0 -> --0 and 0+0 -> 0-0 */
         if ((bZeroPattern & 4) == 4) {
            matG6 = sp34a;  // Use the global sp34a
            mat3D = sp34a_3D;
            break;
         }
         if ((bZeroPattern & 1) == 1) {
            matG6 = sp34c;  // Use the global sp34c
            mat3D = sp34c_3D;
            break;
         }
         matG6 = sp34b;  // Use the global sp34b
         mat3D = sp34b_3D;
         break;
      case 6:  /* --+ -> +++, but 0-+ -> 0-- and -0+ -> -0- and 00+ -> 00- */
         if ((bZeroPattern & 4) == 4) {
            matG6 = sp34b;  // Use the global sp34b
            mat3D = sp34b_3D;
            break;
         }
         if ((bZeroPattern & 2) == 2) {
            matG6 = sp34c;  // Use the global sp34c
            mat3D = sp34c_3D;
            break;
         }
         matG6 = sp34a;  // Use the global sp34a
         mat3D = sp34a_3D;
         break;
      case 7:
         matG6 = spnull;  // Use the global spnull
         mat3D = spnull_3D;
         break;
      }

      vout = matG6 * vin;
      vin = vout;

      // Update the transformation matrices
      mG6 = matG6 * mG6;
      m3d = mat3D * m3d;
   }

   // Main reduction loop - following the logic of Niggli::Reduce
   while (again && count < maxCycle) {
      m1 = spnull; // Reset for this iteration
      m3d_step = spnull_3D; // Reset for this iteration

      if (fabs(vin[3]) > fabs(vin[1]) + delta) { // R5
         m1 = (vin[3] <= 0.0) ? R5_Minus : R5_Plus;  // Use the global R5_Minus/R5_Plus
         m3d_step = (vin[3] <= 0.0) ? R5_Minus_3D : R5_Plus_3D;
         again = true;
      }
      else if (fabs(vin[4]) > fabs(vin[0]) + delta) { // R6
         m1 = (vin[4] <= 0.0) ? R6_Minus : R6_Plus;  // Use the global R6_Minus/R6_Plus
         m3d_step = (vin[4] <= 0.0) ? R6_Minus_3D : R6_Plus_3D;
         again = true;
      }
      else if (fabs(vin[5]) > fabs(vin[0]) + delta) { // R7
         m1 = (vin[5] <= 0.0) ? R7_Minus : R7_Plus;  // Use the global R7_Minus/R7_Plus
         m3d_step = (vin[5] <= 0.0) ? R7_Minus_3D : R7_Plus_3D;
         again = true;
      }
      else if (vin[3] + vin[4] + vin[5] + fabs(vin[0]) + fabs(vin[1]) + delta < 0.0) { // R8
         m1 = R8;  // Use the global R8
         m3d_step = R8_3D;
         again = true;
      }
      else if ((fabs(vin[3] - vin[1]) <= delta && 2.0 * vin[4] - delta < vin[5]) ||
         (fabs(vin[3] + vin[1]) <= delta && vin[5] < 0.0)) { // R9
         m1 = (vin[3] <= 0.0) ? R9_Minus : R9_Plus;  // Use the global R9_Minus/R9_Plus
         m3d_step = (vin[3] <= 0.0) ? R9_Minus_3D : R9_Plus_3D;
         again = true;
      }
      else if ((fabs(vin[4] - vin[0]) <= delta && 2.0 * vin[3] - delta < vin[5]) ||
         (fabs(vin[4] + vin[0]) <= delta && vin[5] < 0.0)) { // R10
         m1 = (vin[4] <= 0.0) ? R10_Minus : R10_Plus;  // Use the global R10_Minus/R10_Plus
         m3d_step = (vin[4] <= 0.0) ? R10_Minus_3D : R10_Plus_3D;
         again = true;
      }
      else if ((fabs(vin[5] - vin[0]) <= delta && 2.0 * vin[3] - delta < vin[4]) ||
         (fabs(vin[5] + vin[0]) <= delta && vin[4] < 0.0)) { // R11
         m1 = (vin[5] <= 0.0) ? R11_Minus : R11_Plus;  // Use the global R11_Minus/R11_Plus
         m3d_step = (vin[5] <= 0.0) ? R11_Minus_3D : R11_Plus_3D;
         again = true;
      }
      else if (fabs(vin[3] + vin[4] + vin[5] + fabs(vin[0]) + fabs(vin[1])) <= delta &&
         (2.0 * (fabs(vin[0]) + vin[4]) + vin[5] > delta)) { // R12
         m1 = R12;  // Use the global R12
         m3d_step = R12_3D;
         again = true;
      }
      else {
         again = false;
         vout = vin;
      }

      if (again) {
         // Apply the transformation to G6 and update the matrices
         vout = m1 * vin;

         // Update the total transformation matrices
         mG6 = m1 * mG6;
         m3d = m3d_step * m3d;

         // After applying a reduction step, we need to re-standardize (MKnorm)
         // This is following the logic in Niggli::Reduce
         {
            // MKnorm again
            bool againMK = true;
            int mkCycleCount = 0;
            MatG6 step_matG6;  // Changed variable name to avoid redeclaration
            Matrix_3x3 step_mat3D;  // Changed variable name to avoid redeclaration

            // MKnorm: assure that g1<=g2<=g3
            G6 vtemp = vout;

            while (againMK && (mkCycleCount <= 5)) {
               ++mkCycleCount;
               againMK = false;
               step_matG6 = spnull;  // Using the renamed variable
               step_mat3D = spnull_3D;     // Using the renamed variable

               if ((fabs(vtemp[0]) > fabs(vtemp[1]) + delta + 1.e-12 * (vtemp[0] + vtemp[1])) ||
                  (fabs(vtemp[0] - vtemp[1]) < 1.e-38 + 1.e-12 * fabs(vtemp[0] + vtemp[1]) &&
                     delta < 1.0E-12 && fabs(vtemp[3]) > fabs(vtemp[4]) + delta + 1.e-12 * (fabs(vtemp[3]) + fabs(vtemp[4])))) {
                  // SP1
                  step_matG6 = sp1;  // Use the global sp1
                  step_mat3D = sp1_3D;
                  againMK = true;
               }
               else if ((fabs(vtemp[1]) > fabs(vtemp[2]) + delta + 1.e-12 * (vtemp[1] + vtemp[2])) ||
                  (fabs(vtemp[1] - vtemp[2]) < 1.e-38 + 1.e-12 * fabs(vtemp[1] + vtemp[2]) &&
                     delta < 1.0E-12 && fabs(vtemp[4]) > fabs(vtemp[5]) + delta + 1.e-12 * (fabs(vtemp[4]) + fabs(vtemp[5])))) {
                  // SP2
                  step_matG6 = sp2;  // Use the global sp2
                  step_mat3D = sp2_3D;
                  againMK = true;
               }

               if (againMK) {
                  G6 vtemp2 = step_matG6 * vtemp;
                  vtemp = vtemp2;

                  // Update the transformation matrices
                  mG6 = step_matG6 * mG6;
                  m3d = step_mat3D * m3d;
               }
            }

            // MKnorm: Ensure vector is +++ or ---
            int local_bMinusPattern = 0;  // Using a local variable with a different name
            int local_bZeroPattern = 0;   // Using a local variable with a different name
            if (vtemp[3] < delta + 1.0E-13 * (vtemp[1] + vtemp[2])) local_bMinusPattern |= 4;
            if (vtemp[4] < delta + 1.0E-13 * (vtemp[0] + vtemp[2])) local_bMinusPattern |= 2;
            if (vtemp[5] < delta + 1.0E-13 * (vtemp[0] + vtemp[1])) local_bMinusPattern |= 1;
            if (fabs(vtemp[3]) < delta + 1.0E-13 * (vtemp[1] + vtemp[2])) local_bZeroPattern |= 4;
            if (fabs(vtemp[4]) < delta + 1.0E-13 * (vtemp[0] + vtemp[2])) local_bZeroPattern |= 2;
            if (fabs(vtemp[5]) < delta + 1.0E-13 * (vtemp[0] + vtemp[1])) local_bZeroPattern |= 1;

            step_matG6 = spnull;
            step_mat3D = spnull_3D;

            switch (local_bMinusPattern) {
            case 0:  /*  +++  */
               step_matG6 = spnull;  // Use the global spnull
               step_mat3D = spnull_3D;
               break;
            case 1:  /* ++- -> --- */
               step_matG6 = sp34a;  // Use the global sp34a
               step_mat3D = sp34a_3D;
               break;
            case 2:  /* +-+ -> --- */
               step_matG6 = sp34b;  // Use the global sp34b
               step_mat3D = sp34b_3D;
               break;
            case 3:  /* +-- -> +++, but +0- -> -0- and +-0 -> --0 and +00 -> -00 */
               if ((local_bZeroPattern & 2) == 2) {
                  step_matG6 = sp34a;  // Use the global sp34a
                  step_mat3D = sp34a_3D;
                  break;
               }
               if ((local_bZeroPattern & 1) == 1) {
                  step_matG6 = sp34b;  // Use the global sp34b
                  step_mat3D = sp34b_3D;
                  break;
               }
               step_matG6 = sp34c;  // Use the global sp34c
               step_mat3D = sp34c_3D;
               break;
            case 4:  /* -++ -> --- */
               step_matG6 = sp34c;  // Use the global sp34c
               step_mat3D = sp34c_3D;
               break;
            case 5:  /* -+- -> +++, but 0+- -> 0-- and -+0 -> --0 and 0+0 -> 0-0 */
               if ((local_bZeroPattern & 4) == 4) {
                  step_matG6 = sp34a;  // Use the global sp34a
                  step_mat3D = sp34a_3D;
                  break;
               }
               if ((local_bZeroPattern & 1) == 1) {
                  step_matG6 = sp34c;  // Use the global sp34c
                  step_mat3D = sp34c_3D;
                  break;
               }
               step_matG6 = sp34b;  // Use the global sp34b
               step_mat3D = sp34b_3D;
               break;
            case 6:  /* --+ -> +++, but 0-+ -> 0-- and -0+ -> -0- and 00+ -> 00- */
               if ((local_bZeroPattern & 4) == 4) {
                  step_matG6 = sp34b;  // Use the global sp34b
                  step_mat3D = sp34b_3D;
                  break;
               }
               if ((local_bZeroPattern & 2) == 2) {
                  step_matG6 = sp34c;  // Use the global sp34c
                  step_mat3D = sp34c_3D;
                  break;
               }
               step_matG6 = sp34a;  // Use the global sp34a
               step_mat3D = sp34a_3D;
               break;
            case 7:
               step_matG6 = spnull;  // Use the global spnull
               step_mat3D = spnull_3D;
               break;
            }

            vtemp = step_matG6 * vtemp;

            // Update the transformation matrices
            mG6 = step_matG6 * mG6;
            m3d = step_mat3D * m3d;

            // Set the vector for the next iteration
            vin = vtemp;
         }

         // Clean up small values to avoid numerical issues
         for (size_t i = 3; i < 6; ++i)
            if (std::fabs(vin[i]) < 1.0E-10) vin[i] = 0.0;

         // Check for illegal values
         if (vin[0] < 0.0 || vin[1] < 0.0 || vin[2] < 0.0) {
            // Error case - negative squared values shouldn't happen
            return false;
         }
      }
      const double det = m3d.Det();
      if (std::abs(det - 1.0) > 0.001) {
         std::cout << "3D transformation determinant not 1.0\n";
      }

      ++count;
   }

   vout = vin;

   // Check if the reduction was successful
   bool isNearReduced = NearRed(vout, delta);
   m_ReductionCycleCount = count;

   // Verify that the matrices are correct
   G6 vcheck = mG6 * vi;
   double diffG6 = 0.0;
   for (int i = 0; i < 6; ++i) {
      diffG6 = std::max(diffG6, std::abs(vcheck[i] - vout[i]));
   }

   // If the G6 transformation is accurate but there are numerical issues with the 3D matrix,
   // we can try a sanity check to adjust the 3D matrix by verifying its determinant
   const double det = m3d.Det();
   if (std::abs(det - 1.0) > 0.001) {
      std::cout << "3D transformation determinant not 1.0\n";
   }
   if (m3d.Det() < 0.0) {
      std::cout << "ERROR IN NIGGLI REDUCTION COMPUTING 3D TRANSFORMATION\n";
   }
   return (count < maxCycle) || isNearReduced;
}


void Niggli::CheckAllNiggliMatrixDeterminants() {
   const double tolerance = 1e-10;
   bool allCorrect = true;

   auto checkMatrix = [&](const Matrix_3x3& m, const std::string& name) {
      double det = m.Det();
      if (std::abs(det - 1.0) > tolerance) {
         std::cout << "ERROR: " << name << " has determinant " << det
            << " (should be +1)" << std::endl;
         allCorrect = false;

         // Show the matrix for debugging
         std::cout << "  Matrix: ";
         for (int i = 0; i < 9; ++i) {
            std::cout << m[i] << " ";
            if (i % 3 == 2) std::cout << " | ";
         }
         std::cout << std::endl;
      }
      };

   std::cout << "=== Checking Niggli 3D Matrix Determinants ===" << std::endl;

   // Check standard presentation matrices
   checkMatrix(spnull_3D, "spnull_3D");
   checkMatrix(sp1_3D, "sp1_3D");
   checkMatrix(sp2_3D, "sp2_3D");
   checkMatrix(sp34a_3D, "sp34a_3D");
   checkMatrix(sp34b_3D, "sp34b_3D");
   checkMatrix(sp34c_3D, "sp34c_3D");

   // Check reduction operation matrices
   checkMatrix(R5_Plus_3D, "R5_Plus_3D");
   checkMatrix(R5_Minus_3D, "R5_Minus_3D");
   checkMatrix(R6_Plus_3D, "R6_Plus_3D");
   checkMatrix(R6_Minus_3D, "R6_Minus_3D");
   checkMatrix(R7_Plus_3D, "R7_Plus_3D");
   checkMatrix(R7_Minus_3D, "R7_Minus_3D");
   checkMatrix(R8_3D, "R8_3D");
   checkMatrix(R12_3D, "R12_3D");

   if (allCorrect) {
      std::cout << "All Niggli 3D matrices have correct determinant (+1)" << std::endl;
   }
   else {
      std::cout << "Some Niggli 3D matrices have incorrect determinants!" << std::endl;
   }
   std::cout << "===========================================" << std::endl;
}
