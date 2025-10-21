#include <iostream>
#include <cfloat>
#include <limits>

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "G6.h"
#include "Niggli.h"
#include "B4.h"
#include "EisensteinControls.h"
#include "EdgeTransformationMatrices.h"
#include "ProgramSetup.h"

double GetTrace(const G6& g6) {
   return g6[0] + g6[1] + g6[2];
}

inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
   // Convert cell to B4 representation
   B4 b4Cell(cell);

   // Apply transformation in B4 space
   B4 transformedB4 = matrix * b4Cell;

   // Convert back to LRL_Cell
   return LRL_Cell(transformedB4);
}

std::pair<double, G6> TestToConvergence(const LRL_Cell& cell, const int nmax,
   const std::vector<Matrix_3x3>& matrices, const double tolerance) {

   double minTrace = std::numeric_limits<double>::max();;
   G6 besttransformed(19191, 19191, 19191, 19191, 19191, 19191);

   int loopcounter;
   for (int loopcounter = 0; loopcounter < nmax; ++loopcounter) {
      const int& i = loopcounter;

      for (const auto m : matrices)
      {
         const G6 transformed = m * cell;
         const double trace = transformed[0] + transformed[1] + transformed[2];
         if (trace < minTrace) {
            minTrace = trace;
            besttransformed = transformed;
         }
      }

   }
   return { minTrace, besttransformed };

}


int main() {
   std::cout << "; Eisenstein Reduction" << std::endl;

   const int nmax = 10;
   const double tolerance = 1.0E-8;

   const std::vector<Matrix_3x3> simpleMatrices = SimpleEdgeTransforms;
   const std::vector<Matrix_3x3> doubleMatrices = DoubleEdgeTransforms;
   const std::vector<Matrix_3x3> allMatrices = GetAllEdgeTransforms();

   try {
      EisenControls controls;
      const BasicProgramInput<EisenControls> dc_setup("Eisenstein", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }


      //std::cout << "===========  SIMPLE MATRICS ============\n";
      for (const auto& input : dc_setup.getInputList()) {
      //   const std::pair<double, G6> out = 
      //      TestToConvergence(input.getCell(), nmax, allMatrices, tolerance);
      //   std::cout << "\n;Eisenstein input trace=" << GetTrace(input.getCell()) << input << std::endl;
      //   std::cout << "simpleMatrices trace=" << out.first << "   " << out.second << std::endl;

      //G6 niggliOut;
      //Niggli::ReduceWithoutMatrices(out.second, niggliOut, 1.0E-8);
      //std::cout << "Niggli reduced " << niggliOut << std::endl;

         G6 out;
         const G6 testv(input.getCell());
         G6 testv2 = testv;
         for (int i = 0; i < 3; ++i)
         {
            testv2[i] = 1.0E-12;
            const bool b = Niggli::ReduceWithoutMatrices(testv2, out, 1.0E-8);
            if (b)
            {
               std::cout << "ERROR, ReduceWithoutMatrices returned true "
                  << "G6 " << testv2 << " i = " << i << std::endl;
               std::cout << "                   returned vector         "
                  << "G6 " << out << " i = " << i << std::endl << std::endl;

               const int i19191 = 19191;
            }
         }
      }



      return 0;
   }
   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}
